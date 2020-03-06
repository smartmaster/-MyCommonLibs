#include "stdafx.h"
#include "SpannedAndSpannedSimpleRW.h"
#include "CacheIOMaker.h"

namespace SmartLib
{


CSpannedAndSpannedSimpleRW::CSpannedAndSpannedSimpleRW(std::vector<DiskExtent> & disks) :
	m_disks(disks)
{
	std::sort(std::begin(m_disks), std::end(m_disks),
	[](CONST SmartLib::DiskExtent & v1, CONST SmartLib::DiskExtent & v2) 
	{
		return v1.m_volumeOffset < v2.m_volumeOffset; 
	});
}

HRESULT CSpannedAndSpannedSimpleRW::CreateMember()
{
	return S_OK;
}

HRESULT CSpannedAndSpannedSimpleRW::DestroyMember()
{
	return S_OK;
}

HRESULT CSpannedAndSpannedSimpleRW::SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod)
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

	//if (llDistanceToMove % CACHE_SIZE)
	//{
	//	hr = E_INVALIDARG;
	//	D_API_ERR(0, TEXT(""), hr, TEXT("DistanceToMove must be integral of %d"), CACHE_SIZE);
	//	LEAVE_BLOCK(0);
	//}

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

HRESULT CSpannedAndSpannedSimpleRW::Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONG startDisk = -1;
	std::vector<LONGLONG> segment;
	segment.reserve(m_disks.size() + 2);
	Split(
		m_pointer,//IN LONGLONG start,
		m_pointer + nNumberOfBytesToRead,//IN LONGLONG end,
		m_disks,//IN CONST std::vector<DiskExtent> & disks,
		startDisk,//OUT LONG & startDisk,
		segment//,//OUT std::vector<LONGLONG> & segment
		);
	if (-1 == startDisk)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("Split"), hr, TEXT("Invalid IO offset"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONGLONG bufferOffset = 0;
	ULONG NumberOfBytesReadTemp = 0;
	for (size_t ii = 0; ii < segment.size() - 1; ++ii)
	{
		LONGLONG offset = segment[ii];
		LONGLONG length = segment[ii + 1] - segment[ii];
		DiskExtent & disk = m_disks[startDisk++];

		NumberOfBytesReadTemp = 0;
		hr = ReadAtOffsetHelper(disk.m_disk, disk.m_offset + offset - disk.m_volumeOffset, length, &NumberOfBytesReadTemp, pBuffer + bufferOffset);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
			break;
		}

		bufferOffset += length;
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	if (SUCCEEDED(hr))
	{
		m_pointer += nNumberOfBytesToRead;
	}
	END_BLOCK(0);

	return hr;
}

HRESULT CSpannedAndSpannedSimpleRW::Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONG startDisk = -1;
	std::vector<LONGLONG> segment;
	segment.reserve(m_disks.size() + 2);
	Split(
		m_pointer,//IN LONGLONG start,
		m_pointer + nNumberOfBytesToWrite,//IN LONGLONG end,
		m_disks,//IN CONST std::vector<DiskExtent> & disks,
		startDisk,//OUT LONG & startDisk,
		segment//,//OUT std::vector<LONGLONG> & segment
		);
	if (-1 == startDisk)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("Split"), hr, TEXT("Invalid IO offset"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONGLONG bufferOffset = 0;
	ULONG NumberOfBytesWrittenTemp = 0;
	for (size_t ii = 0; ii < segment.size() - 1; ++ii)
	{
		LONGLONG offset = segment[ii];
		LONGLONG length = segment[ii + 1] - segment[ii];
		DiskExtent & disk = m_disks[startDisk++];

		NumberOfBytesWrittenTemp = 0;
		hr = WriteAtOffsetHelper(disk.m_disk, disk.m_offset + offset - disk.m_volumeOffset, length, &NumberOfBytesWrittenTemp, pBuffer + bufferOffset);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
			break;
		}

		bufferOffset += length;
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	if (SUCCEEDED(hr))
	{
		m_pointer += nNumberOfBytesToWrite;
	}
	END_BLOCK(0);

	return hr;
}

HRESULT CSpannedAndSpannedSimpleRW::GetFileSize(LONGLONG * pFileSize)
{
	LONGLONG fileSize = 0;
	for (auto & disk : m_disks)
	{
		fileSize += disk.m_length;
	}
	*pFileSize = fileSize;
	return S_OK;
}

HANDLE CSpannedAndSpannedSimpleRW::GetRawHandle()
{
	return nullptr;
}

VOID CSpannedAndSpannedSimpleRW::Split(IN LONGLONG start, IN LONGLONG end, IN CONST std::vector<DiskExtent> & disks, OUT LONG & startDisk, OUT std::vector<LONGLONG> & segment)
{
	segment.push_back(start);
	for (size_t ii = 0; ii < disks.size(); ++ii)
	{
		CONST DiskExtent & disk = disks[ii];
		if (start >= disk.m_volumeOffset && start < disk.m_volumeOffset + disk.m_length)
		{
			startDisk = ii;
		}

		if (disk.m_volumeOffset > start && disk.m_volumeOffset < end)
		{
			segment.push_back(disk.m_volumeOffset);
		}

		if (disk.m_volumeOffset >= end)
		{
			break;
		}
	}
	segment.push_back(end);
}

HRESULT CreateInstanceCSpannedAndSpannedSimpleRW(std::vector<DiskExtent> & disks, IFileDevice ** ppObj, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(
		new CSpannedAndSpannedSimpleRW(disks),
		ppObj,
		bCreateMember
		);
}


HRESULT CreateInstanceCSpannedAndSpannedSimpleRWCacheIOMaker(std::vector<DiskExtent> & disks, IFileDeviceFlush ** ppObj, BOOL bCreateMember)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceCSpannedAndSpannedSimpleRW(disks, &spIFileDevice, bCreateMember);
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

	CONST LONG  CACHE_SIZE = 2 << 20; //2MB cache block size
	hr = CreateInstanceCCacheIoMaker(
		spIFileDevice,//IFileDevice * pFileDevice,
		TRUE,//BOOL bAsOwner,
		CACHE_SIZE,//LONG blockSize,
		FALSE,//BOOL bWriteNewAndSequential,
		FALSE,//BOOL isFlushable,
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

}// namespace SmartLib