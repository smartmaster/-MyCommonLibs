// ClientBackupMain.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CmdLine.h"
#include "BackupFlowMain.h"

int _tmain(int argc, _TCHAR* argv[])
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	BOOL bCoInitialized = FALSE;
	CAutoPtr<CBackupFlowMain> apBackupFlowMain;

	do 
	{
		//******************************************
		hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("CoInitializeEx"), hr));
			break;
		}
		bCoInitialized = TRUE;

		//******************************************
		CCommandLine cmdLine;
		nStatus = cmdLine.Parse(argc, argv);
		if (nStatus)
		{
			nStatus = E_FAIL;
			_OLD_DAPIERR((0, DRNPOS, TEXT("cmdLine.Parse"), E_FAIL));
			break;
		}

		apBackupFlowMain.Attach(new CBackupFlowMain(cmdLine));
		if (NULL == (CBackupFlowMain*)apBackupFlowMain)
		{
			nStatus = E_OUTOFMEMORY;
			_OLD_DAPIERR((0, DRNPOS, TEXT("new CBackupFlowMain"), E_OUTOFMEMORY));
			break;
		}

		//******************************************
		nStatus = apBackupFlowMain->Main();
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("Main"), nStatus));
			break;
		}

		//******************************************
	} while (FALSE);

	//******************************************
	if ((CBackupFlowMain*)apBackupFlowMain)
	{
		apBackupFlowMain->DestroyMember();
		apBackupFlowMain.Free();
	}

	//******************************************
	if (bCoInitialized)
	{
		CoUninitialize();
		bCoInitialized = FALSE;
	}

	return nStatus;
}

