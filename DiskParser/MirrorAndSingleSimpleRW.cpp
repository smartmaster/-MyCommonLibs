#include "stdafx.h"
#include "MirrorAndSingleSimpleRW.h"
#include "CacheIOMaker.h"

namespace SmartLib
{



CMirrorAndSingleSimpleRW::CMirrorAndSingleSimpleRW(std::vector<DiskExtent> & disks) :
	m_disks(disks)
{

}

HRESULT CMirrorAndSingleSimpleRW::CreateMember()
{
	return S_OK;
}

HRESULT CMirrorAndSingleSimpleRW::DestroyMember()
{
	return S_OK;
}

HRESULT CMirrorAndSingleSimpleRW::SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod)
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

HRESULT CMirrorAndSingleSimpleRW::Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	for (auto & disk : m_disks)
	{
		hr = ReadAtOffsetHelper(disk.m_disk, m_pointer + disk.m_offset, nNumberOfBytesToRead, lpNumberOfBytesRead, pBuffer);
		if (SUCCEEDED(hr))
		{
			break;
		}
		else
		{
			D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
		}
	}

	if (SUCCEEDED(hr))
	{
		m_pointer += nNumberOfBytesToRead;
	}

	return hr;
}

HRESULT CMirrorAndSingleSimpleRW::Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	for (auto & disk : m_disks)
	{
		hr = WriteAtOffsetHelper(disk.m_disk, m_pointer + disk.m_offset, nNumberOfBytesToWrite, lpNumberOfBytesWritten, pBuffer);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
			break;
		}
	}

	if (SUCCEEDED(hr))
	{
		m_pointer += nNumberOfBytesToWrite;
	}

	return hr;
}

HRESULT CMirrorAndSingleSimpleRW::GetFileSize(LONGLONG * pFileSize)
{
	LONGLONG fileSize = 0;
	for (auto & disk : m_disks)
	{
		fileSize += disk.m_length;
	}
	fileSize /= m_disks.size();
	*pFileSize = fileSize;
	return S_OK;
}

HANDLE CMirrorAndSingleSimpleRW::GetRawHandle()
{
	return nullptr;
}

HRESULT CreateInstanceCMirrorAndSingleSimpleRW(std::vector<DiskExtent> & disks, IFileDevice ** ppObj, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(
		new CMirrorAndSingleSimpleRW(disks),
		ppObj,
		bCreateMember
		);
}


HRESULT CreateInstanceCMirrorAndSingleSimpleRWCacheIOMaker(std::vector<DiskExtent> & disks, IFileDeviceFlush ** ppObj, BOOL bCreateMember)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceCMirrorAndSingleSimpleRW(disks, &spIFileDevice, bCreateMember);
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

} //namespace SmartLib