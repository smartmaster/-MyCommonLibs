#include "stdafx.h"

#if 0


#include "vhdxSkipZeroWriter.h"


namespace
{
	static BOOL IsAllZero(CONST BYTE * Buffer, LONG BufferLen)
	{
		static CONST LONGLONG ZERO_BUFFER[512 / sizeof(LONGLONG)] = { 0 };

		LONG CurrentCount;
		BOOL bRet = TRUE;

		while (BufferLen)
		{
			if (BufferLen > sizeof(ZERO_BUFFER))
			{
				CurrentCount = sizeof(ZERO_BUFFER);
			}
			else
			{
				CurrentCount = BufferLen;
			}

			if (memcmp(Buffer, ZERO_BUFFER, CurrentCount))
			{
				bRet = FALSE;
				break;
			}


			Buffer += CurrentCount;
			BufferLen -= CurrentCount;
		}

		return bRet;
	}
}


namespace SmartLib
{




CVHDXSkipZeroWriter::CVHDXSkipZeroWriter(LPCTSTR vhdxName, LONGLONG diskSize, LONG vhdxBlockSize) :
	m_vhdxName(vhdxName),
	m_diskSize(diskSize),
	m_vhdxBlockSize(diskSize)
{

}

HRESULT CVHDXSkipZeroWriter::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CreateInstanceCDiskImageVhdxParams params =
	{
		nullptr,//LPCTSTR		parentVhdxFileName;
		m_vhdxName.GetString(),//LPCTSTR		vhdxFileName;
		GENERIC_WRITE,//DWORD		DesiredAccess;
		CREATE_NEW,//DWORD		CreationDisposition;
		m_diskSize,//LONGLONG	VirtualDiskSize;
		512,//LONG		LogicalSectorSize;
		4096,//LONG		PhysicalSectorSize;
		m_vhdxBlockSize,//LONG		BlockSize;
		VHDX_DISK_TYPE_ENUM::DYNAMIC//LONG		VhdxDiskType; ,//<sonmi01>2014-4-21 ###??? ,//take effect only if create_new
	};
	hr = CreateInstanceCDiskImageVhdx(params, &m_vhdx, TRUE);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceCDiskImageVhdx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CVHDXSkipZeroWriter::DestroyMember()
{
	m_vhdx.Release();
	return S_OK;
}

HRESULT CVHDXSkipZeroWriter::SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod)
{
	return m_vhdx->SetPointerEx(llDistanceToMove, lpNewFilePointer, ulMoveMethod);
}

HRESULT CVHDXSkipZeroWriter::Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer)
{
	return m_vhdx->Read(nNumberOfBytesToRead, lpNumberOfBytesRead, pBuffer);
}

HRESULT CVHDXSkipZeroWriter::Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	if (IsAllZero(pBuffer, nNumberOfBytesToWrite))
	{
		LONGLONG NewFilePointer = 0;
		hr = m_vhdx->SetPointerEx(nNumberOfBytesToWrite, &NewFilePointer, FILE_CURRENT);
	}
	else
	{
		hr = m_vhdx->Write(nNumberOfBytesToWrite, lpNumberOfBytesWritten, pBuffer);
	}

	return hr;
}

HRESULT CVHDXSkipZeroWriter::GetFileSize(LONGLONG * pFileSize)
{
	return m_vhdx->GetFileSize(pFileSize);
}

HANDLE CVHDXSkipZeroWriter::GetRawHandle()
{
	return m_vhdx->GetRawHandle();
}

HRESULT CVHDXSkipZeroWriter::Flush()
{
	HRESULT hr1 = m_vhdx->FlushMetadata();
	HRESULT hr2 = m_vhdx->FlushData();
	return
		FAILED(hr1) ? hr1 :
		FAILED(hr2) ? hr2 :
		S_OK;
}



}//namespace SmartLib


#endif //#if 0