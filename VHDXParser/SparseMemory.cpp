
#include "stdafx.h"
#include "SparseMemory.h"


#if 0

namespace SmartLib
{

CSparseMemory::CSparseMemory(LONG blockSize /*must be 2^n*/, LONGLONG InitialSize) :
	m_refCount{ 0 },
	m_CurrentPointer{ 0 },
	m_blockSize{ blockSize },
	m_InitialSize{ InitialSize }
{

}

HRESULT CSparseMemory::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	m_sbm.Attach(new CSparseBlockMemory(m_blockSize, m_InitialSize));
	if (nullptr == m_sbm.m_p)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("new"), hr, TEXT("CSparseBlockMemory"));
		LEAVE_BLOCK(0);
	}

	hr = m_sbm->CreateMember();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateMember"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	m_cacheIO.Attach(new CCacheIO(m_sbm.m_p, FALSE, m_blockSize, 0));
	if (nullptr == m_cacheIO.m_p)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("new"), hr, TEXT("CCacheIO"));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CSparseMemory::DestroyMember()
{
	m_cacheIO.Free();
	m_sbm.Free();
	return S_OK;
}

HRESULT CSparseMemory::SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod)
{
	HRESULT hr = S_OK;


	LONGLONG TempCurrent = 0;
	if (FILE_BEGIN == ulMoveMethod)
	{
		TempCurrent = llDistanceToMove;
	}
	else if (FILE_END == ulMoveMethod)
	{
		LONGLONG currentFileSize = 0;
		hr = m_sbm->GetFileSize(&currentFileSize);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
		else
		{
			TempCurrent = currentFileSize + llDistanceToMove;
		}

	}
	else if (FILE_CURRENT == ulMoveMethod)
	{
		TempCurrent = m_CurrentPointer + llDistanceToMove;
	}
	else
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT("invaid MoveMethod [%d]"), ulMoveMethod);
	}

	m_CurrentPointer = TempCurrent;

	return hr;
}

HRESULT CSparseMemory::Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer)
{
	HRESULT hr = m_cacheIO->ReadAtOffset(m_CurrentPointer, nNumberOfBytesToRead, pBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CacheIO.Read"), hr, TEXT(""));
	}
	else
	{
		if (lpNumberOfBytesRead)
		{
			*lpNumberOfBytesRead = nNumberOfBytesToRead;
		}
		m_CurrentPointer += nNumberOfBytesToRead;
	}
	return hr;
}

HRESULT CSparseMemory::Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer)
{
	HRESULT hr = m_cacheIO->WriteAtOffset(m_CurrentPointer, nNumberOfBytesToWrite, pBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CacheIO.Write"), hr, TEXT(""));
	}
	else
	{
		if (lpNumberOfBytesWritten)
		{
			*lpNumberOfBytesWritten = nNumberOfBytesToWrite;
		}
		m_CurrentPointer += nNumberOfBytesToWrite;
	}
	return hr;
}

HRESULT CSparseMemory::GetFileSize(LONGLONG * pFileSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = m_cacheIO->Flush();
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Flush"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = m_sbm->GetFileSize(pFileSize);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
	
	return hr;
}

HANDLE CSparseMemory::GetRawHandle()
{
	return nullptr;
}

HRESULT CSparseMemory::Flush()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (m_cacheIO.m_p)
	{
		hr = m_cacheIO->Flush();
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

HRESULT CreateInstanceCSparseMemory(LONG blockSize /*must be 2^n*/, LONGLONG InitialSize, IFileDeviceFlush ** ppIFileDevice, BOOL bCreateMember /*= TRUE*/)
{
	return CREATE_INSTCANCE_COBJECT(
		new CSparseMemory(blockSize /*must be 2^n*/, InitialSize),
		ppIFileDevice,
		bCreateMember
		);
}




HRESULT test_CSparseMemory(LPCTSTR srcFile, LPCTSTR destFile)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CONST LONG BLOCK_SIZE = 64 * 1024;

	CComPtr<IFileDeviceFlush> spSM;
	hr = CreateInstanceCSparseMemory(BLOCK_SIZE, 0, &spSM, TRUE);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceCSparseMemory"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	CComPtr<IFileDevice> spSrc;
	hr = CreateInstanceFileDeviceWin32(
		srcFile,//in lpctstr strfilename,
		GENERIC_READ,//in ulong uldesiredaccess,
		FILE_SHARE_READ,//in ulong ulsharemode,
		OPEN_EXISTING,//in ulong ulcreationdisposition,
		0,//in ulong ulflagsandattributes,
		&spSrc,//out ifiledevice ** ppfiledevice,
		TRUE//bool bcreatemember
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
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	

	//////////////////////////////////////////////////////////////////////////
	CONST LONG IO_SIZE = BLOCK_SIZE * 3 + 17;
	CVMemPtr<BYTE> buffer;
	buffer.Allocate(IO_SIZE);
	ULONG NumberOfBytesRead = 0;
	ULONG NumberOfBytesWritten = 0;

	LONGLONG newFP = 0;
	spSrc->SetPointerEx(0, &newFP, FILE_BEGIN);
	spSM->SetPointerEx(0, &newFP, FILE_BEGIN);
	for (;;)
	{
		NumberOfBytesRead = 0;
		spSrc->Read(IO_SIZE, &NumberOfBytesRead, buffer);
		if (0 == NumberOfBytesRead)
		{
			break;
		}

		NumberOfBytesWritten = 0;
		spSM->Write(NumberOfBytesRead, &NumberOfBytesWritten, buffer);
	}
	spSM->Flush();


	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spDest;
	hr = CreateInstanceFileDeviceWin32(
		destFile,//in lpctstr strfilename,
		GENERIC_WRITE,//in ulong uldesiredaccess,
		0,//in ulong ulsharemode,
		CREATE_ALWAYS,//in ulong ulcreationdisposition,
		0,//in ulong ulflagsandattributes,
		&spDest,//out ifiledevice ** ppfiledevice,
		TRUE//bool bcreatemember
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
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	spSM->SetPointerEx(0, &newFP, FILE_BEGIN);
	spDest->SetPointerEx(0, &newFP, FILE_BEGIN);

	LONGLONG fileSize = 0;
	spSM->GetFileSize(&fileSize);
	LONG curSize = 0;
	while (fileSize > 0)
	{
		fileSize > IO_SIZE ? curSize = IO_SIZE : curSize = fileSize;

		NumberOfBytesRead = 0;
		spSM->Read(curSize, &NumberOfBytesRead, buffer);
		if (0 == NumberOfBytesRead)
		{
			break;
		}

		NumberOfBytesWritten = 0;
		spDest->Write(NumberOfBytesRead, &NumberOfBytesWritten, buffer);

		fileSize -= curSize;
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

} //end namespace SmartLib
#endif