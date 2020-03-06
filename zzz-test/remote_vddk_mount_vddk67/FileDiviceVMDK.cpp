#include "stdafx.h"
#include "FileDiviceVMDK.h"


namespace SmartLib
{

CFileDeviceVMDK::CFileDeviceVMDK(VixDiskLibHandle diskHandle) :
	m_diskHandle(diskHandle)
{

}

HRESULT CFileDeviceVMDK::CreateMember()
{
	return S_OK;
}

HRESULT CFileDeviceVMDK::DestroyMember()
{
	return S_OK;
}

HRESULT CFileDeviceVMDK::SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (FILE_BEGIN != ulMoveMethod)
	{
		PRINT_LINE_A("MoveMethod is not FILE_BEGIN [%d]", ulMoveMethod);
		hr = E_INVALIDARG;
		LEAVE_BLOCK(0);
	}

	if (llDistanceToMove % SECTOR_SIZE)
	{
		PRINT_LINE_A("DistanceToMove is not sector size aligned [%I64d]", llDistanceToMove);
		hr = E_INVALIDARG;
		LEAVE_BLOCK(0);
	}

	m_curpointer = llDistanceToMove;
	if (lpNewFilePointer)
	{
		*lpNewFilePointer = m_curpointer;
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CFileDeviceVMDK::Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (nNumberOfBytesToRead % SECTOR_SIZE)
	{
		PRINT_LINE_A("NumberOfBytesToRead is not sector size aligned [%d]", nNumberOfBytesToRead);
		hr = E_INVALIDARG;
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	VixDiskLibSectorType startSector = m_curpointer / SECTOR_SIZE;
	VixDiskLibSectorType numSectors = nNumberOfBytesToRead / SECTOR_SIZE;
	VixError err = VixDiskLib_Read(
		m_diskHandle,//VixDiskLibHandle diskHandle,
		startSector,//VixDiskLibSectorType startSector,
		numSectors,//VixDiskLibSectorType numSectors,
		pBuffer//,//uint8 *readBuffer
		);
	if (VIX_FAILED(err))
	{
		char * errMsg = VixDiskLib_GetErrorText(err, NULL);
		CString strErrMsg;
		Utf8To16String(errMsg, strErrMsg);

		PRINT_LINE_A("VixDiskLib_Read failed with %I64d %s", err, errMsg);
		hr = E_FAIL;
		LEAVE_BLOCK(0);
	}

	if (lpNumberOfBytesRead)
	{
		*lpNumberOfBytesRead = nNumberOfBytesToRead;
	}

	//////////////////////////////////////////////////////////////////////////
	m_curpointer += nNumberOfBytesToRead;
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CFileDeviceVMDK::Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (nNumberOfBytesToWrite % SECTOR_SIZE)
	{
		PRINT_LINE_A("NumberOfBytesToWrite is not sector size aligned [%d]", nNumberOfBytesToWrite);
		hr = E_INVALIDARG;
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	VixDiskLibSectorType startSector = m_curpointer / SECTOR_SIZE;
	VixDiskLibSectorType numSectors = nNumberOfBytesToWrite / SECTOR_SIZE;
	VixError err = VixDiskLib_Write(
		m_diskHandle,//VixDiskLibHandle diskHandle,
		startSector,//VixDiskLibSectorType startSector,
		numSectors,//VixDiskLibSectorType numSectors,
		pBuffer//,//uint8 *readBuffer
		);
	if (VIX_FAILED(err))
	{
		char * errMsg = VixDiskLib_GetErrorText(err, NULL);
		CString strErrMsg;
		Utf8To16String(errMsg, strErrMsg);

		PRINT_LINE_A("VixDiskLib_Write failed with %I64d %s", err, errMsg);
		hr = E_FAIL;
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	if (lpNumberOfBytesWritten)
	{
		*lpNumberOfBytesWritten = nNumberOfBytesToWrite;
	}
	m_curpointer += nNumberOfBytesToWrite;
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CFileDeviceVMDK::GetFileSize(LONGLONG * pFileSize)
{
	HRESULT hr = S_OK;
	if (-1LL == m_diskSize)
	{
		VixDiskLibInfo *info = nullptr;
		VixError err = VixDiskLib_GetInfo(
			m_diskHandle,//VixDiskLibHandle diskHandle,
			&info//,//VixDiskLibInfo **info
			);
		if (VIX_FAILED(err))
		{
			char * errMsg = VixDiskLib_GetErrorText(err, NULL);
			CString strErrMsg;
			Utf8To16String(errMsg, strErrMsg);

			PRINT_LINE_A("VixDiskLib_GetInfo failed with %I64d %s", err, errMsg);
			hr = E_FAIL;
		}
		else
		{
			m_diskSize = info->capacity * SECTOR_SIZE;
			VixDiskLib_FreeInfo(info); info = nullptr;
		}
	}

	*pFileSize = m_diskSize;
	return hr;
}

HANDLE CFileDeviceVMDK::GetRawHandle()
{
	return m_diskHandle;
}

HRESULT CFileDeviceVMDK::Flush()
{
	return S_OK;
}

HRESULT CreateInstanceCFileDeviceVMDK(VixDiskLibHandle diskHandle, IFileDeviceFlush ** ppobj, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(
		new CFileDeviceVMDK(diskHandle),
		ppobj,
		bCreateMember
		);
}

}//namespace SmartLib