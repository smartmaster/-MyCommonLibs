#include "stdafx.h"

#include "StrippedAndRaid5RW.h"
#include "CacheIOMaker.h"


namespace SmartLib
{



void CRaid5BlockHelper::LogicalBlockToPhysicalBlock(BOOL isRaid5, LONGLONG logicalBlock, LONG diskCount, LONGLONG & physicalBlock, LONG & diskNumber)
{

	if (isRaid5)
	{
		//physicalBlock = logicalBlock / (diskCount - 1);
		//diskNumber = logicalBlock % (diskCount - 1);

		//LONG parityDiskNumber = diskCount - 1 - physicalBlock % diskCount; //the first parity block is in the last disk
		////LONG parityDiskNumber = physicalBlock % diskCount; //the first parity block is in the first disk
		////LONG SHIFT_COUNT = diskCount - 1;
		////parityDiskNumber = (parityDiskNumber + SHIFT_COUNT) % diskCount;
		//if (diskNumber >= parityDiskNumber)
		//{
		//	++diskNumber;
		//}
		physicalBlock = logicalBlock / (diskCount - 1);
		diskNumber = logicalBlock % diskCount;
	}
	else
	{
		physicalBlock = logicalBlock / diskCount;
		diskNumber = logicalBlock % diskCount;
	}
}

void CRaid5BlockHelper::Test()
{
	CONST LONGLONG logicalBlockCount = 21;
	CONST LONG diskCount = 4;

	LONGLONG physicalBlock = -1LL;
	LONG  diskNumber = -1;

	LONG width[] = { 16, 12, 16 };
	_ftprintf_s(stdout, TEXT("|%*s|%*s|%*s|") TEXT("\r\n"), width[0], TEXT("LogicalBlock"), width[1], TEXT("DiskNumber"), width[2], TEXT("PhysicalBlock"));
	for (LONGLONG ii = 0; ii < 21; ++ii)
	{
		physicalBlock = -1LL;
		diskNumber = -1;
		LogicalBlockToPhysicalBlock(TRUE, ii, diskCount, physicalBlock, diskNumber);
		_ftprintf_s(stdout, TEXT("|%*I64d|%*d|%*I64d|") TEXT("\r\n"), width[0], ii, width[1], diskNumber, width[2], physicalBlock);
	}
}

CStrippedAndRaid5RW::CStrippedAndRaid5RW(std::vector<DiskExtent> & disks, LONG strippedBlockSize, BOOL isRaid5) :
	m_disks(disks),
	m_strippedBlockSize(strippedBlockSize),
	m_isRaid5(isRaid5)
{

}

HRESULT CStrippedAndRaid5RW::CreateMember()
{
	HRESULT hr = S_OK;
	m_diskCachIo.resize(m_disks.size());
	size_t ii = 0;
	for (auto & diskExt : m_disks)
	{
		hr = CreateInstanceCCacheIO(
			diskExt.m_disk,//IFileDevice * pFileDevice, 
			FALSE,//BOOL bAsOwner, 
			CACHE_SIZE,//LONG BlockSize, 
			FALSE,//BOOL bWriteNewAndSequential,
			&m_diskCachIo[ii++],//OUT ICCacheIO ** ppobj,
			TRUE//,//BOOL bCreateMember
			);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("CreateInstanceCCacheIO"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
	}
	return hr;
}

HRESULT CStrippedAndRaid5RW::DestroyMember()
{
	Flush();
	for (auto & disk : m_diskCachIo)
	{
		disk.Release();
	}
	m_diskCachIo.clear();
	return S_OK;
}

HRESULT CStrippedAndRaid5RW::SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (FILE_BEGIN != ulMoveMethod)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT(""), hr, TEXT("MoveMethod must be FILE_BEGIN"));
		LEAVE_BLOCK(0);
	}

	if (llDistanceToMove % CACHE_SIZE)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT(""), hr, TEXT("DistanceToMove must be integral of %d"), CACHE_SIZE);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	m_pointer = llDistanceToMove;
	if (lpNewFilePointer)
	{
		*lpNewFilePointer = m_pointer;
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CStrippedAndRaid5RW::Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (CACHE_SIZE != nNumberOfBytesToRead)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT(""), hr, TEXT("IO size must be %d"), CACHE_SIZE);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONG blockCount = CACHE_SIZE / m_strippedBlockSize;
	LONGLONG startBlock = m_pointer / m_strippedBlockSize;
	LONGLONG bufferOffset = 0;
	ULONG NumberOfBytesReadTmp = 0;
	for (LONGLONG logicalBlock = startBlock; logicalBlock < startBlock + blockCount; ++logicalBlock)
	{
		LONGLONG physicalBlock = 1;
		LONG diskNumber = -1;
		CRaid5BlockHelper::LogicalBlockToPhysicalBlock(m_isRaid5, logicalBlock, m_diskCachIo.size(), physicalBlock, diskNumber);
		NumberOfBytesReadTmp = 0;
		hr = ReadAtOffsetHelper(m_diskCachIo[diskNumber], m_disks[diskNumber].m_offset + physicalBlock * m_strippedBlockSize, m_strippedBlockSize, &NumberOfBytesReadTmp, pBuffer + bufferOffset);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
			break;
		}

		bufferOffset += m_strippedBlockSize;
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	m_pointer += nNumberOfBytesToRead;
	if (lpNumberOfBytesRead)
	{
		*lpNumberOfBytesRead = nNumberOfBytesToRead;
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CStrippedAndRaid5RW::Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (CACHE_SIZE != nNumberOfBytesToWrite)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT(""), hr, TEXT("IO size must be %d"), CACHE_SIZE);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONG blockCount = CACHE_SIZE / m_strippedBlockSize;
	LONGLONG startBlock = m_pointer / m_strippedBlockSize;
	LONGLONG bufferOffset = 0;
	ULONG NumberOfBytesWrittenTmp = 0;
	for (LONGLONG logicalBlock = startBlock; logicalBlock < startBlock + blockCount; ++logicalBlock)
	{
		LONGLONG physicalBlock = 1;
		LONG diskNumber = -1;
		CRaid5BlockHelper::LogicalBlockToPhysicalBlock(m_isRaid5, logicalBlock, m_diskCachIo.size(), physicalBlock, diskNumber);
		NumberOfBytesWrittenTmp = 0;
		hr = WriteAtOffsetHelper(m_diskCachIo[diskNumber], m_disks[diskNumber].m_offset + physicalBlock * m_strippedBlockSize, m_strippedBlockSize, &NumberOfBytesWrittenTmp, pBuffer + bufferOffset);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
			break;
		}

		bufferOffset += m_strippedBlockSize;
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	m_pointer += nNumberOfBytesToWrite;
	if (lpNumberOfBytesWritten)
	{
		*lpNumberOfBytesWritten = nNumberOfBytesToWrite;
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CStrippedAndRaid5RW::GetFileSize(LONGLONG * pFileSize)
{
	LONG dataCount;
	m_isRaid5 ? dataCount = m_disks.size() - 1 : dataCount = m_disks.size();
	*pFileSize = dataCount * m_disks[0].m_length;
	return S_OK;
}

HANDLE CStrippedAndRaid5RW::GetRawHandle()
{
	return nullptr;
}

HRESULT CStrippedAndRaid5RW::Flush()
{
	HRESULT hr = S_OK;
	for (auto & disk : m_diskCachIo)
	{
		if (disk)
		{
			HRESULT hrTmp = disk->Flush();
			if (FAILED(hr))
			{
				hr = hrTmp;
			}
		}
	}
	return S_OK;
}

//blockSize is size of stripped blocks (not cache block size)
HRESULT CreateInstanceCStrippedAndRaid5RW(std::vector<DiskExtent> & disks, LONG blockSize, BOOL isRaid5, IFileDeviceFlush ** ppObj, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(
		new CStrippedAndRaid5RW(disks, blockSize, isRaid5),
		ppObj,
		bCreateMember
		);
}

HRESULT CreateInstanceCStrippedAndRaid5RWCacheIOMaker(std::vector<DiskExtent> & disks, LONG blockSize, BOOL isRaid5, IFileDeviceFlush ** ppObj, BOOL bCreateMember)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDeviceFlush> spIFileDeviceFlush;
	hr = CreateInstanceCStrippedAndRaid5RW(disks, blockSize, isRaid5, &spIFileDeviceFlush, bCreateMember);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceCStrippedAndRaid5RW"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = CreateInstanceCCacheIoMaker(
		spIFileDeviceFlush,//IFileDevice * pFileDevice,
		TRUE,//BOOL bAsOwner,
		CStrippedAndRaid5RW::CACHE_SIZE,//LONG blockSize,
		FALSE,//BOOL bWriteNewAndSequential,
		TRUE,//BOOL isFlushable,
		ppObj,//IFileDeviceFlush ** ppObj,
		bCreateMember//,//BOOL bCreateMmeber
		);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceCCacheIoMaker"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

} //namespace SmartLib