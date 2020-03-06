// remote_vddk_mount.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <fstream>

#include "RemoteVddkMount.h"



static void print_help()
{
	printf_s("%s" "\r\n", "this.exe 1 [script.xml]");
	printf_s("%s" "\r\n", "Example:\t" "this.exe 1 script.xml");
	printf_s("%s" "\r\n", "");
	printf_s("%s" "\r\n", "this.exe 2 <vmxSpec> <serverName> <userName> <password> <DriverLetter> <diskPath1> <diskPath2>...");
	printf_s("%s" "\r\n", "Example:\t" "this.exe 2 moref=vm-5900 shuli02-vc55 administrator c@123456 M: \"[datastore2-9T-raid0] sonmi01-004-rps2-veeam/sonmi01-004-rps2-veeam.vmdk\" \"[datastore2-9T-raid0] sonmi01-004-rps2-veeam/sonmi01-004-rps2-veeam_1.vmdk\" ");
	printf_s("%s" "\r\n", "");
}

//this.exe 1 [script.xml]
//this.exe 2 <vmxSpec> <serverName> <userName> <password> <DriverLetter> <diskPath1> <diskPath2>...
int _tmain(int argc, _TCHAR* argv[])
{
	//for (;;)
	//{
	//	Sleep(1000);
	//}
	if(false)
	{
		CString str = argv[0];
		str += TEXT(".cmdline.txt");
		std::wofstream fout(str);
		for (int ii = 0; ii < argc; ++ii)
		{
			fout << TEXT("\"") << argv[ii] << TEXT("\" ");
		}
		fout << std::endl;
		fout.close();
	}

	//////////////////////////////////////////////////////////////////////////
	int test_case = 0;
	if (argc > 1)
	{
		test_case = _tcstol(argv[1], NULL, 0);
	}
	else
	{
		print_help();
		return E_INVALIDARG;
	}

	//////////////////////////////////////////////////////////////////////////
	switch (test_case)
	{
	case 0:
	{
		MountParams::unit_test();
	}
		break;
	case 1:
	case 4:
	{
		MountParams params;
		if (argc > 2)
		{
			params.FromFile(FIELD_NAME(MountParams), argv[2]);
		}
		else
		{
			params.Init();
		}
		if (1 == test_case)
		{
			RemoteVddkMount(params, nullptr);
		}
		else
		{
			CAutoPtr<CVolumeChkdsk> apVolCallback(new CVolumeChkdsk);
			RemoteVddkMount(params, apVolCallback.m_p);
		}
		
	}
		break;
	case 2:
	{
		MountParams params;
		if (argc <= 9)
		{
			print_help();
		}
		else
		{
			int argvIndex = 2;
			params.vmxSpec = argv[2];
			params.serverName = argv[3];
			params.userName = argv[4];
			params.password = argv[5];
			params.DriverLetter = argv[6];
			params.thumbPrint = argv[7];
			params.snapshotRef = argv[8];
			for (int ii = 9; ii < argc; ++ ii)
			{
				params.diskPath.push_back(argv[ii]);
			}
			params.port = 0;
			//params.thumbPrint = TEXT("");
			params.readOnly = 1;
			//params.snapshotRef = TEXT("");
			params.transportModes = TEXT("nbd:nbdssl");
			params.configFile = TEXT("");
			params.libDir = TEXT("");
			params.bPressKey = 0;
			params.bDismount = 1;
			params.bGetOsInfo = 0;
			CAutoPtr<CVolumeChkdsk> apVolCallback(new CVolumeChkdsk);
			RemoteVddkMount(params, apVolCallback.m_p);
		}
	}
		break;
	case 3:
		CVolumeChkdsk::CHKDSK(TEXT("E:"));
		break;
	}


	return 0;
}

