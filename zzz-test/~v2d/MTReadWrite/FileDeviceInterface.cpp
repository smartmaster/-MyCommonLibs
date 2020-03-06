#include "stdafx.h"
#include "FileDeviceInterface.h"
#include "DTraceLib.h"


HRESULT ReadAtOffsetHelper( IFileDevice * pIFileDevice, LONGLONG Offset, ULONG nNumberOfBytesToRead, ULONG * pNumberOfBytesRead, BYTE* pBuffer )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	LONGLONG NewFilePointer = -1;
	hr = pIFileDevice->SetPointerEx(Offset, &NewFilePointer, FILE_BEGIN);
	if (FAILED(hr))
	{
		DSETLASTSTATUS(hr, -1);
		DAPIERR(0, TEXT("SetPointerEx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	ULONG NumberOfBytesRead = 0;
	hr = pIFileDevice->Read(nNumberOfBytesToRead, (pNumberOfBytesRead? pNumberOfBytesRead : &NumberOfBytesRead), pBuffer);
	if (FAILED(hr))
	{
		DSETLASTSTATUS(hr, -1);
		DAPIERR(0, TEXT("Read"), hr, TEXT(""));
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
		DSETLASTSTATUS(hr, -1);
		DAPIERR(0, TEXT("SetPointerEx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	ULONG NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(nNumberOfBytesToWrite, (pNumberOfBytesWritten? pNumberOfBytesWritten : &NumberOfBytesWritten), pBuffer);
	if (FAILED(hr))
	{
		DSETLASTSTATUS(hr, -1);
		DAPIERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}
