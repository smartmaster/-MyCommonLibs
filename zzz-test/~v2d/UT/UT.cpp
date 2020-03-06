// UT.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


//******************************************
extern INT FileDeviceTest(LPCTSTR szFile);
extern INT SingleVolumeInfoTest();
extern INT SingleDiskInfoTest();
extern INT VShadowClientTest(INT argc, TCHAR ** argv);
extern VOID TestAllVolumeInfo();
extern VOID TestAllDiskInfo();
extern VOID AllVdsDiskVolumeInfoTest();
extern INT VShadowClientTest();
extern VOID TestShadowCopy(LPCTSTR szShadowCopy);
extern INT AsyncIoCopyTest(LPCTSTR szSource, LPCTSTR szDest);
extern HRESULT AllDiskVolumeInformationTest();
extern INT AppVirtualDiskMain(INT argc, _TCHAR* argv[]);
extern INT VirtualDiskTest( LPCTSTR szVHDPath, ULONG nSizeGB );
extern INT VirtualDiskChildTest( LPCTSTR szVHDPath, LPCTSTR szParent );
extern INT VirtualDiskTestVds( LPCTSTR szVHDPath, ULONG nSizeGB );
extern INT CrcTest();
extern INT GptDiskLayoutTest(ULONG ulDiskNumber);
extern VOID TestGUID();
extern INT AsyncIoCopyVolumeDiskTest(LPCTSTR szSource, LPCTSTR szDest);
extern INT VolumeToDiskCopyTest(LPCTSTR szSource, LPCTSTR szVHDPath, ULONG nSizeGB );
extern VOID NtfsAttrTest(LPCTSTR szFileName, BOOL m_dir);
extern VOID EnumFileTest(LPCTSTR pszRoot, LPCTSTR pszFileSpec, BOOL b);
extern VOID MyNtfsAttrTest(LPCTSTR pszFullPath, BOOL m_dir);
extern VOID NtfsEnumDir(LPCTSTR pszFullPath, BOOL bShowFullPath);
extern VOID NtfsEnumDirEx(LPCTSTR pszFullPath, BOOL bShowFullPath, LPCTSTR pszIs, LPCTSTR pszIsNot);
extern VOID EnumFindFilesTest(LPCTSTR pszFullPath,LPCTSTR pszIs, LPCTSTR pszIsNot);
extern VOID LibVhdTest(LPCTSTR pszVhdFileName);
#include "..\CommonFunctions\XmlHelper.h"
#include "..\CommonFunctions\IocpAsyncCopy.h"
#include "..\CommonFunctions\AsyncIOCompleteRoutine.h"

extern INT ut_pipe_main(int argc, _TCHAR* argv[]);
extern INT ut_socket_main(int argc, _TCHAR* argv[]);
//******************************************
#include "..\CommonFunctions\MTCopy.h"


int _tmain_0001(int argc, _TCHAR* argv[])
{

	if (argc == 1)
	{
		_ftprintf_s(stdout, TEXT("Usage:\t this.exe <dir> <full|notFull> <ToMatch> <NotToMatch>") TEXT("\r\n"));
		_ftprintf_s(stdout, TEXT("E.g:\t this.exe C:\\ full *.txt ???.txt") TEXT("\r\n"));
		return 0;
	}

	BOOL bShowFullPath = FALSE;
	if (argc >= 3)
	{
		bShowFullPath = (0 == _tcsicmp(argv[2], TEXT("full")));
	}

	LPCTSTR pszIs = NULL;
	if (argc >= 4)
	{
		pszIs = argv[3];
	}

	LPCTSTR pszIsNot = NULL;
	if (argc >= 5)
	{
		pszIsNot = argv[4];
	}

	NtfsEnumDirEx(argv[1], bShowFullPath, pszIs, pszIsNot);
	EnumFindFilesTest(argv[1], pszIs, pszIsNot);
	return 0;
}





int _tmain_0000(int argc, _TCHAR* argv[])
{
	DSTART(999);

	if(FALSE)
	{
		CTestFileCopyCmdLine cmdLine;
		cmdLine.Parse(argc, argv);
		TestFileCopy(cmdLine);
	}
	else
	{
		NS_MTCopy::TestApp::CCmdLine cmdLine;
		cmdLine.Parse(argc, argv);
		NS_MTCopy::TestApp::Main(cmdLine);
	}

	//{
	//	//ut_socket_main(argc, argv);
	//	ut_pipe_main(argc, argv);
	//}

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;
	BOOL bCoInitialized = FALSE;

	do 
	{
		//******************************************
		//hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		//if (FAILED(hr))
		//{
		//	_OLD_DSETLASTSTATUS(hr);
		//	nStatus = hr;
		//	_OLD_DAPIERR((0, DRNPOS, TEXT("CoInitializeEx"), hr));
		//	break;
		//}
		//bCoInitialized = TRUE;

		////******************************************
		//hr = CoInitializeSecurity(
		//	NULL,                           //  Allow *all* VSS writers to communicate back!
		//	-1,                             //  Default COM authentication service
		//	NULL,                           //  Default COM authorization service
		//	NULL,                           //  reserved parameter
		//	RPC_C_AUTHN_LEVEL_NONE,  //  Strongest COM authentication level
		//	RPC_C_IMP_LEVEL_IMPERSONATE,       //  Minimal impersonation abilities 
		//	NULL,                           //  Default COM authentication settings
		//	EOAC_NONE,                      //  No special options
		//	NULL                            //  Reserved parameter
		//	);
		//if (FAILED(hr))
		//{	
		//	_OLD_DSETLASTSTATUS(hr);
		//	nStatus = hr;
		//	_OLD_DAPIERR((0, DRNPOS, TEXT("CoInitializeSecurity"), hr));
		//	//break;
		//}


		//*****************************************
		//{
		//	AsyncIOCompleteRoutineApp::CTestFileCopyCmdLine cmdLine;
		//	cmdLine.Parse(argc, argv);
		//	AsyncIOCompleteRoutineApp::Main(cmdLine);
		//}
		//{
		//	CTestFileCopyCmdLine cmdLine;
		//	cmdLine.Parse(argc, argv);
		//	TestFileCopy(cmdLine/*argv[1], argv[2]*/);
		//}
		//{
		//	LibVhdTest(argv[1]);
		//}
		//{
		//	CComplexXmlUT::XmlHelperUT(argv[1]);
		//}
		//{
		//	LibVhdTest(argv[1]);
		//}
		//{
		//	LONG ll = _tcstol(argv[3], NULL, 10);
		//	EnumFileTest(argv[1], argv[2], ll);
		//}
		//{
		//	VShadowClientTest(argc, argv);
		//}
		{
			if (argc == 1)
			{
				_ftprintf_s(stdout, TEXT("Usage:\t this.exe <dir> <full|notFull> <ToMatch> <NotToMatch>") TEXT("\r\n"));
				_ftprintf_s(stdout, TEXT("E.g:\t this.exe C:\\ full *.txt ???.txt") TEXT("\r\n"));
				return 0;
			}
			
			BOOL bShowFullPath = FALSE;
			if (argc >= 3)
			{
				bShowFullPath = (0 == _tcsicmp(argv[2], TEXT("full")));
			}
			
			LPCTSTR pszIs = NULL;
			if (argc >= 4)
			{
				pszIs = argv[3];
			}

			LPCTSTR pszIsNot = NULL;
			if (argc >= 5)
			{
				pszIsNot = argv[4];
			}

			NtfsEnumDirEx(argv[1], bShowFullPath, pszIs, pszIsNot);
			EnumFindFilesTest(argv[1], pszIs, pszIsNot);
			NtfsEnumDir(argv[1], bShowFullPath);
		}
		//{
		//	BOOL  m_dir = _tcstoul(argv[2], NULL, 10);
		//	NtfsEnumDir(argv[1]);
		//	NtfsAttrTest(argv[1], m_dir);
		//	MyNtfsAttrTest(argv[1], m_dir);
		//}
		//{
		//	ULONG nSizeGB = _tcstoul(argv[3], NULL, 10);
		//	VolumeToDiskCopyTest(argv[1], argv[2], nSizeGB);
		//}
		//GptDiskLayoutTest(1);
		//VirtualDiskTest( TEXT("f:\\1.vhd"), 4);
		//AsyncIoCopyVolumeDiskTest(argv[1], argv[2]);
		//TestGUID();
		//CrcTest();
		//VirtualDiskTestVds( TEXT("f:\\1.vhd"), 2040 );
		//VirtualDiskTest( TEXT("f:\\1.vhd"), 2040);
		//VirtualDiskChildTest( TEXT("f:\\2.vhd"), TEXT("f:\\1.vhd"));
		//AppVirtualDiskMain(argc, argv);
		//AllDiskVolumeInformationTest();
		//AsyncIoCopyTest(argv[1], argv[2]);
		//TestShadowCopy(argv[1]); //argv[1] is shadow copy 
		//FileDeviceTest(argv[2]);
		//VShadowClientTest();
		//AllVdsDiskVolumeInfoTest();
		//TestAllVolumeInfo();
		//TestAllDiskInfo();
		

		//******************************************
	} while (FALSE);


	//******************************************
	if (bCoInitialized)
	{
		CoUninitialize();
		bCoInitialized = FALSE;
	}

	return nStatus;
}


//////////////////////////////////////////////////////////////////////////
#include "..\CommonFunctions\NTFS_Lib.h"
extern VOID EnumFilesDataRun( 
	IN IFileDeviceObject * pFileDeviceVolume, 
	IN LPCTSTR pszRootPath, 
	IN LPCTSTR pszIS, 
	IN LPCTSTR pszISNOT, 
	OUT vector<CString> * pFiles,
	OUT vector<LONGLONG> * pFileRefences,
	OUT vector<vector<DataRun_Entry> > * pDataRun);

/*
-volume \\.\C:
-path "\windows\system32"
-is *.dll
-isnot *e*.dll
*/

void Printusage(int argc, _TCHAR* argv[])
{
	_ftprintf_s(stdout, TEXT("Example:\r\n"));
	_ftprintf_s(stdout, TEXT("%s  -volume \\\\.\\C:  -path  \\windows\\system32  -is  *.dll   -isnot *e*.dll \r\n"), argv[0]);
}

int _tmain_0002(int argc, _TCHAR* argv[])
{

	if (argc < 3)
	{
		Printusage(argc, argv);
		return 0;
	}

	CString cmd_Volume;
	CString cmd_Path;
	CString cmd_is = TEXT("*");
	CString cmd_isnot = TEXT("");



	for (INT ii = 1; ii < argc; ++ ii)
	{
		if (0 == _tcsicmp(TEXT("-volume"), argv[ii]))
		{
			++ii;
			cmd_Volume =  argv[ii];
		}
		else if (0 == _tcsicmp(TEXT("-path"), argv[ii]))
		{
			++ii;
			cmd_Path =  argv[ii];
		}
		else if (0 == _tcsicmp(TEXT("-is"), argv[ii]))
		{
			++ii;
			cmd_is =  argv[ii];
		}
		else if (0 == _tcsicmp(TEXT("-isnot"), argv[ii]))
		{
			++ii;
			cmd_isnot =  argv[ii];
		}
	}

	_OLD_DINFO((0, DRNPOS, TEXT("volume=[%s], path=[%s], file pattern is [%s] but is not [%s]"), 
		cmd_Volume.GetString(), 
		cmd_Path.GetString(), 
		cmd_is.GetString(), 
		cmd_isnot.GetString()));

	CLocalFileDevice FileDeviceVolume;
	CComBSTR bstrFileName = cmd_Volume.GetString();
	FileDeviceVolume.Create(
		bstrFileName,//BSTR bstrFileName, 
		GENERIC_READ,//ULONG ulDesiredAccess, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,//ULONG ulShareMode, 
		OPEN_EXISTING,//ULONG ulCreationDisposition, 
		0//ULONG ulFlagsAndAttributes
		);

	vector<CString>  Files;
	vector<LONGLONG> FileRefences;
	vector<vector<DataRun_Entry> > DataRun;
	EnumFilesDataRun( 
		&FileDeviceVolume,//IN IFileDeviceObject * pFileDeviceVolume, 
		cmd_Path.GetString(),//IN LPCTSTR pszRootPath, 
		cmd_is.GetString(),//IN LPCTSTR pszIS, 
		cmd_isnot.GetString(),//IN LPCTSTR pszISNOT, 
		&Files,//OUT vector<CString> * pFiles,
		&FileRefences,//OUT vector<LONGLONG> * pFileRefences,
		&DataRun//OUT vector<vector<DataRun_Entry> > * pDataRun
		);

	return 0;
}


//////////////////////////////////////////////////////////////////////////


int _tmain(int argc, _TCHAR* argv[])
{
	return _tmain_0002(argc, argv);
	return _tmain_0001(argc, argv);
	return _tmain_0000(argc, argv);
}
