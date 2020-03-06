#include "stdafx.h"
#include "..\ServerFileDevice\ServerFileDevice_i.h"

INT FileDeviceTest(LPCTSTR szFile)
{
	DSTART(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		CComPtr<IFileDeviceObject> spIFileDevice;
		hr = spIFileDevice.CoCreateInstance(__uuidof(FileDeviceObject));
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("CoCreateInstance"), hr));
			break;
		}

		//******************************************
		CComBSTR bsrFileName(szFile);
		hr = spIFileDevice->Create( 
			bsrFileName,///* [in] */ BSTR bstrFileName,
			GENERIC_READ,///* [in] */ ULONG ulDesiredAccess,
			0,///* [in] */ ULONG ulShareMode,
			OPEN_EXISTING,///* [in] */ ULONG ulCreationDisposition,
			0///* [in] */ ULONG ulFlagsAndAttributes
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("___com_error_name"), hr, TEXT("[%s]"), (BSTR)bsrFileName));
			break;
		}

		//******************************************
		TCHAR szBuffer[128]  = {0};
		for (;;)
		{
			ULONG ulNumberOfBytesRead = 0;
			hr = spIFileDevice->Read( 
				sizeof(szBuffer),///* [in] */ ULONG nNumberOfBytesToRead,
				&ulNumberOfBytesRead,///* [out] */ ULONG *lpNumberOfBytesRead,
				(BYTE*)szBuffer///* [size_is][out] */ BYTE *pBuffer
				);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("Read"), hr));
				break;
			}
			else
			{
				if (ulNumberOfBytesRead < sizeof(szBuffer))
				{
					break;
				}
			}
		}
		if (nStatus)
		{
			break;
		}

		//******************************************
		hr = spIFileDevice->Close();
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("Close"), hr));
			break;
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}