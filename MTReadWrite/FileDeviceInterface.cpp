#include "stdafx.h"
#include "FileDeviceInterface.h"

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"


HRESULT ReadAtOffsetHelper( IFileDevice * pIFileDevice, LONGLONG Offset, ULONG nNumberOfBytesToRead, ULONG * pNumberOfBytesRead, BYTE* pBuffer )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	LONGLONG NewFilePointer = -1;
	hr = pIFileDevice->SetPointerEx(Offset, &NewFilePointer, FILE_BEGIN);
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	ULONG NumberOfBytesRead = 0;
	hr = pIFileDevice->Read(nNumberOfBytesToRead, (pNumberOfBytesRead? pNumberOfBytesRead : &NumberOfBytesRead), pBuffer);
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);

	return hr;
}

HRESULT WriteAtOffsetHelper( IFileDevice * pIFileDevice, LONGLONG Offset, ULONG nNumberOfBytesToWrite, ULONG * pNumberOfBytesWritten, CONST BYTE* pBuffer )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);

	LONGLONG NewFilePointer = -1;
	hr = pIFileDevice->SetPointerEx(Offset, &NewFilePointer, FILE_BEGIN);
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	ULONG NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(nNumberOfBytesToWrite, (pNumberOfBytesWritten? pNumberOfBytesWritten : &NumberOfBytesWritten), pBuffer);
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}


HRESULT WriteAllHelper(IFileDevice * pIFileDevice, LONG TotalBytesToWrite, LONG * pTotalDataWritten, CONST BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	
	LONG TotalDataWritten = 0;
	ULONG CurrentSizeWritten = 0;

	while (TotalBytesToWrite)
	{
		//////////////////////////////////////////////////////////////////////////
		CurrentSizeWritten = 0;
		hr = pIFileDevice->Write(TotalBytesToWrite, &CurrentSizeWritten, pBuffer + TotalDataWritten);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
			break;
		}

		if (0 == CurrentSizeWritten)
		{
			break;
		}

		TotalBytesToWrite -= CurrentSizeWritten;
		TotalDataWritten += CurrentSizeWritten;
	}


	*pTotalDataWritten = TotalDataWritten;
	
	return hr;
}


HRESULT ReadAllHelper(IFileDevice * pIFileDevice, LONG TotalBytesToRead, LONG * pTotalDataRead, BYTE* pBuffer)
{
	HRESULT hr = S_OK;

	LONG TotalDataRead = 0;
	ULONG CurrentSizeRead = 0;

	while (TotalBytesToRead)
	{
		//////////////////////////////////////////////////////////////////////////
		CurrentSizeRead = 0;
		hr = pIFileDevice->Read(TotalBytesToRead, &CurrentSizeRead, pBuffer + TotalDataRead);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
			break;
		}

		if (0 == CurrentSizeRead)
		{
			break;
		}

		TotalBytesToRead -= CurrentSizeRead;
		TotalDataRead += CurrentSizeRead;
	}

	*pTotalDataRead = TotalDataRead;

	return hr;
}

