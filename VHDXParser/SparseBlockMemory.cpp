
#include "stdafx.h"
#include "SparseBlockMemory.h"
#include "MergeBuffer.h"
#include "..\PublicHeader\PublicVMemPtr.h"

namespace SmartLib
{



BOOL CSparseBlockMemory::IsValidOffset(LONGLONG Offset)
{
	return TRUE; //always
	//return 0 == (Offset % m_blockSize);
}

CSparseBlockMemory::CSparseBlockMemory(LONG blockSize /*must be 2^n*/, LONGLONG InitialSize) :
	m_refCount{ 0 },
	m_CurrentPointer{ 0 },
	m_blockSize{ blockSize },
	m_InitialSize{ InitialSize }
{

}

HRESULT CSparseBlockMemory::CreateMember()
{
	m_bat.resize(AtlAlignUp(m_InitialSize, m_blockSize) / m_blockSize);
	if (m_bat.size())
	{
		ZeroMemory(&m_bat[0], m_bat.size() * sizeof(LPBYTE));
	}
	return S_OK;
}

HRESULT CSparseBlockMemory::DestroyMember()
{
	for (size_t ii = 0; ii < m_bat.size(); ++ii)
	{
		CVMemPtr<BYTE> temp(m_bat[ii]);
		m_bat[ii] = nullptr;
	}
	m_bat.clear();
	return S_OK;
}

HRESULT CSparseBlockMemory::SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod)
{
	HRESULT hr = S_OK;

	LONGLONG TempCurrent = 0;
	if (FILE_BEGIN == ulMoveMethod)
	{
		TempCurrent = llDistanceToMove;
	}
	else if (FILE_END == ulMoveMethod)
	{
		LONGLONG currentSize = (LONGLONG)(m_bat.size()) * (LONGLONG)m_blockSize;
		TempCurrent = currentSize + llDistanceToMove;
	}
	else if (FILE_CURRENT == ulMoveMethod)
	{
		TempCurrent = m_CurrentPointer + llDistanceToMove;
	}
	else
	{
		hr = E_INVALIDARG;
	}

	if (SUCCEEDED(hr))
	{
		if (IsValidOffset(TempCurrent))
		{
			//if (TempCurrent > currentSize)
			//{
			//	m_bat.resize(AtlAlignUp(TempCurrent, m_blockSize) / m_blockSize);
			//}
			m_CurrentPointer = TempCurrent;
			*lpNewFilePointer = m_CurrentPointer;
		}
		else
		{
			hr = E_INVALIDARG;
			D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT("DistanceToMove=%I64d, MoveMethod=%u, --%s--"), llDistanceToMove, ulMoveMethod, GetCurrentModuleName());
		}
	}

	return hr;
}

HRESULT CSparseBlockMemory::Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONGLONG start = AtlAlignDown(m_CurrentPointer, m_blockSize);
	LONGLONG end = AtlAlignUp(m_CurrentPointer + nNumberOfBytesToRead, m_blockSize);

	if (start + m_blockSize == end) //buffer is in one block
	{
		hr = ReadPartialBlock(m_CurrentPointer, nNumberOfBytesToRead, lpNumberOfBytesRead, pBuffer);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("ReadPartialBlock"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		m_CurrentPointer += (*lpNumberOfBytesRead);
	}
	else
	{
		LONGLONG offset1 = m_CurrentPointer;
		LONGLONG offset2 = AtlAlignUp(m_CurrentPointer, m_blockSize);
		LONGLONG offset3 = AtlAlignDown(m_CurrentPointer + nNumberOfBytesToRead, m_blockSize);
		LONGLONG offset4 = m_CurrentPointer + nNumberOfBytesToRead;

		//////////////////////////////////////////////////////////////////////////
		ULONG currReadSize = 0;
		ULONG TotalRead = 0;
		if (offset2 > offset1)
		{
			hr = ReadPartialBlock(offset1, offset2 - offset1, &currReadSize, pBuffer + TotalRead);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("ReadPartialBlock"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			
			TotalRead += currReadSize;
			if (0 == currReadSize)
			{
				m_CurrentPointer += TotalRead;
				*lpNumberOfBytesRead = TotalRead;
				LEAVE_BLOCK(0);
			}
		}


		//////////////////////////////////////////////////////////////////////////
		BOOL noMoreData = FALSE;
		while (offset3 > offset2)
		{
			currReadSize = 0;
			hr = ReadBlock(offset2, m_blockSize, &currReadSize, pBuffer + TotalRead);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("ReadBlock"), hr, TEXT(""));
				break;
			}

			offset2 += currReadSize;
			TotalRead += currReadSize;

			if (0 == currReadSize)
			{
				m_CurrentPointer += TotalRead;
				*lpNumberOfBytesRead = TotalRead;
				noMoreData = TRUE;
				break;
			}
		}
		if (FAILED(hr))
		{
			LEAVE_BLOCK(0);
		}
		if (noMoreData)
		{
			LEAVE_BLOCK(0);
		}


		//////////////////////////////////////////////////////////////////////////
		if (offset4 > offset3)
		{
			currReadSize = 0;
			hr = ReadPartialBlock(offset3, offset4 - offset3, &currReadSize, pBuffer + TotalRead);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("ReadPartialBlock"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}
			TotalRead += currReadSize;

			if (0 == currReadSize)
			{
				m_CurrentPointer += TotalRead;
				*lpNumberOfBytesRead = TotalRead;
				LEAVE_BLOCK(0);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		m_CurrentPointer += TotalRead;
		*lpNumberOfBytesRead = TotalRead;
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CSparseBlockMemory::Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONGLONG start = AtlAlignDown(m_CurrentPointer, m_blockSize);
	LONGLONG end = AtlAlignUp(m_CurrentPointer + nNumberOfBytesToWrite, m_blockSize);

	if (start + m_blockSize == end) //buffer is in one block
	{
		hr = WritePartialBlock(m_CurrentPointer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, pBuffer);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("WritePartialBlock"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		m_CurrentPointer += (*lpNumberOfBytesWritten);
	}
	else
	{
		LONGLONG offset1 = m_CurrentPointer;
		LONGLONG offset2 = AtlAlignUp(m_CurrentPointer, m_blockSize);
		LONGLONG offset3 = AtlAlignDown(m_CurrentPointer + nNumberOfBytesToWrite, m_blockSize);
		LONGLONG offset4 = m_CurrentPointer + nNumberOfBytesToWrite;

		//////////////////////////////////////////////////////////////////////////
		ULONG currWritteneSize = 0;
		ULONG TotalWritten = 0;
		if (offset2 > offset1)
		{
			hr = WritePartialBlock(offset1, offset2 - offset1, &currWritteneSize, pBuffer + TotalWritten);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("WritePartialBlock"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}
			TotalWritten += currWritteneSize;
		}


		//////////////////////////////////////////////////////////////////////////
		while (offset3 > offset2)
		{
			currWritteneSize = 0;
			hr = WriteBlock(offset2, m_blockSize, &currWritteneSize, pBuffer + TotalWritten);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("WriteBlock"), hr, TEXT(""));
				break;
			}

			offset2 += currWritteneSize;
			TotalWritten += currWritteneSize;
		}
		if (FAILED(hr))
		{
			LEAVE_BLOCK(0);
		}


		//////////////////////////////////////////////////////////////////////////
		if (offset4 > offset3)
		{
			currWritteneSize = 0;
			hr = WritePartialBlock(offset3, offset4 - offset3, &currWritteneSize, pBuffer + TotalWritten);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("WritePartialBlock"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}
			TotalWritten += currWritteneSize;
		}

		//////////////////////////////////////////////////////////////////////////
		m_CurrentPointer += TotalWritten;
		*lpNumberOfBytesWritten = TotalWritten;
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

//file pointer not move forward
HRESULT CSparseBlockMemory::ReadBlock(LONGLONG offset, ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (nNumberOfBytesToRead != m_blockSize)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("Read"), hr, TEXT("invalid size [%d, %d]"), nNumberOfBytesToRead, m_blockSize);
		LEAVE_BLOCK(0);
	}


	if (offset % m_blockSize)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("Read"), hr, TEXT("invalid file pointer [%I64d, %d]"), offset, m_blockSize);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t index = offset / m_blockSize;
	if (index >= m_bat.size())
	{
		*lpNumberOfBytesRead = 0;
	}
	else
	{
		LPCBYTE pBlock = m_bat[index];
		if (nullptr == pBlock)
		{
			ZeroMemory(pBuffer, nNumberOfBytesToRead);
		}
		else
		{
			CopyMemory(pBuffer, pBlock, nNumberOfBytesToRead);
		}

		*lpNumberOfBytesRead = nNumberOfBytesToRead;
		//offset += nNumberOfBytesToRead;
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CSparseBlockMemory::ReadPartialBlock(LONGLONG offset, ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONGLONG start = AtlAlignDown(offset, m_blockSize);
	LONGLONG end = AtlAlignUp(offset + nNumberOfBytesToRead, m_blockSize);
	ATLASSERT(start + m_blockSize == end); //buffer is in one block

	//////////////////////////////////////////////////////////////////////////
	if (start == offset && nNumberOfBytesToRead == m_blockSize)
	{
		hr = ReadBlock(offset, nNumberOfBytesToRead, lpNumberOfBytesRead, pBuffer);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t index = start / m_blockSize;
	if (index >= m_bat.size())
	{
		*lpNumberOfBytesRead = 0;
	}
	else
	{
		LPCBYTE pBlock = m_bat[index];
		if (nullptr == pBlock)
		{
			ZeroMemory(pBuffer, nNumberOfBytesToRead);
		}
		else
		{
			LONGLONG offsetInBlock = offset % m_blockSize;
			CopyMemory(pBuffer, pBlock + offsetInBlock, nNumberOfBytesToRead);
		}
		*lpNumberOfBytesRead = nNumberOfBytesToRead;
		//offset += nNumberOfBytesToRead;
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

//file pointer not move forward
HRESULT CSparseBlockMemory::WriteBlock(LONGLONG offset, ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (nNumberOfBytesToWrite != m_blockSize)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("Write"), hr, TEXT("invalid size [%d, %d]"), nNumberOfBytesToWrite, m_blockSize);
		LEAVE_BLOCK(0);
	}


	if (offset % m_blockSize)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("Write"), hr, TEXT("invalid file pointer [%I64d, %d]"), offset, m_blockSize);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t index = offset / m_blockSize;
	if (index >= m_bat.size())
	{
		m_bat.resize(index + 1);
	}

	//////////////////////////////////////////////////////////////////////////
	if (IsAllZero(pBuffer, nNumberOfBytesToWrite))
	{
		if (nullptr != m_bat[index])
		{
			CVMemPtr<BYTE> temp;
			temp.Attach(m_bat[index]);
			m_bat[index] = nullptr;
		}
	}
	else
	{
		if (nullptr == m_bat[index])
		{
			CVMemPtr<BYTE> temp;
			temp.Allocate(m_blockSize);
			if (nullptr == temp.m_pData)
			{
				hr = E_OUTOFMEMORY;
				D_API_ERR(0, TEXT("Allocate"), hr, TEXT("failed to allocate [%d]"), m_blockSize);
				LEAVE_BLOCK(0);
			}
			m_bat[index] = temp.Detach();
		}
		CopyMemory(m_bat[index], pBuffer, nNumberOfBytesToWrite);
	}
	*lpNumberOfBytesWritten = nNumberOfBytesToWrite;
	//offset += nNumberOfBytesToWrite;

	END_BLOCK(0);

	return hr;
}

HRESULT CSparseBlockMemory::WritePartialBlock(LONGLONG offset, ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONGLONG start = AtlAlignDown(offset, m_blockSize);
	LONGLONG end = AtlAlignUp(offset + nNumberOfBytesToWrite, m_blockSize);
	ATLASSERT(start + m_blockSize == end); //buffer is in one block

	//////////////////////////////////////////////////////////////////////////
	if (start == offset && nNumberOfBytesToWrite == m_blockSize)
	{
		hr = WriteBlock(offset, nNumberOfBytesToWrite, lpNumberOfBytesWritten, pBuffer);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t index = start / m_blockSize;
	if (index >= m_bat.size())
	{
		m_bat.resize(index + 1);
	}

	BOOL bIsAllZero = IsAllZero(pBuffer, nNumberOfBytesToWrite);
	BOOL bNoWrite = (nullptr == m_bat[index] && bIsAllZero);
	if (bNoWrite)
	{
		//do nothing - no allocate and no write
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		if (nullptr == m_bat[index])
		{
			CVMemPtr<BYTE> temp;
			temp.Allocate(m_blockSize);
			if (nullptr == temp.m_pData)
			{
				hr = E_OUTOFMEMORY;
				D_API_ERR(0, TEXT("Allocate"), hr, TEXT("failed to allocate [%d]"), m_blockSize);
				LEAVE_BLOCK(0);
			}
			m_bat[index] = temp.Detach();
		}

		//////////////////////////////////////////////////////////////////////////
		LONGLONG offsetInBlock = offset % m_blockSize;
		CopyMemory(m_bat[index] + offsetInBlock, pBuffer, nNumberOfBytesToWrite);

		//////////////////////////////////////////////////////////////////////////
		if (bIsAllZero)
		{
			if (IsAllZero(m_bat[index], m_blockSize))
			{
				CVMemPtr<BYTE> temp;
				temp.Attach(m_bat[index]);
				m_bat[index] = nullptr;
			}
		}
	}
	*lpNumberOfBytesWritten = nNumberOfBytesToWrite;


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CSparseBlockMemory::GetFileSize(LONGLONG * pFileSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	*pFileSize = (LONGLONG)(m_bat.size())* (LONGLONG)m_blockSize;
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HANDLE CSparseBlockMemory::GetRawHandle()
{
	return nullptr;
}


HRESULT CreateInstanceCSparseBlockMemory(LONG blockSize /*must be 2^n*/, LONGLONG InitialSize, IFileDevice ** ppIFileDevice, BOOL bCreateMember /*= TRUE*/)
{
	return CREATE_INSTCANCE_COBJECT(
		new CSparseBlockMemory(blockSize /*must be 2^n*/, InitialSize),
		ppIFileDevice,
		bCreateMember
		);
}


HRESULT test_CSparseBlockMemory(LPCTSTR srcFile, LPCTSTR destFile)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CONST LONG BLOCK_SIZE = 64 * 1024;
	//CONST LONG BLOCK_SIZE = 64;

	CComPtr<IFileDevice> spSM;
	hr = CreateInstanceCSparseBlockMemory(BLOCK_SIZE, 0, &spSM, TRUE);
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
	//spSM->Flush();


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
#if 1
	for (;;)
	{
		NumberOfBytesRead = 0;
		spSM->Read(IO_SIZE, &NumberOfBytesRead, buffer);
		if (0 == NumberOfBytesRead)
		{
			break;
		}

		NumberOfBytesWritten = 0;
		spDest->Write(NumberOfBytesRead, &NumberOfBytesWritten, buffer);
	}
#else
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
#endif


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

} //end namespace SmartLib