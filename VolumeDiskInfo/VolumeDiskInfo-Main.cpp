// VolumeDiskInfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "..\PublicHeader\PublicCodeBlock.h"

#include "MyStgHelper.h"
#include "AllVdsDiskVolumeInfo.h"
#include "SingleAndAllDiskInfo.h"
#include "SingleAndAllVolumeInfo.h"
#include "ComInit.h"
#include "MiscHelper.h"
#include "SingleAndAllVolumeInfoXml.h"
#include "SingleAndAllDiskInfoXml.h"
#include "AllVdsDiskVolumeInfoXml.h"
#include "ServiceManagerHelper.h"
#include "ModifyFileForVSS.h"
#include "MyVersionHelpers.h"
#include "DeployVMwareVSS.h"
#include "FileVerInfo.h"

extern INT TestVolumeBlockOffset(LPCTSTR volumeName, LONG blockSizeKB, LONG count); //no closeHandle or feeMem, do not use for production

HRESULT AllDiskVolumeInformationTest()
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);

	
	

	BEGIN_BLOCK(1);
	CComPtr<ILockBytes> spILockBytes;
	CComPtr<IStorage> spIStorage;
	DWORD grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE;
	hr = CMyStgHelper::CreateMemoryStorage(&spILockBytes, &spIStorage,  grfMode);
	if (SUCCEEDED(hr))
	{
		CAllDiskInfo MyAllDiskInfo;
		CONST DWORD MAX_DISK_NUMBER = 256;
		MyAllDiskInfo.Retrieve(MAX_DISK_NUMBER);

		hr = MyAllDiskInfo.WriteToStorage(spIStorage);
	}

	if (SUCCEEDED(hr))
	{
		CAllDiskInfo MyAllDiskInfo;
		MyAllDiskInfo.ReadFromStorage(spIStorage);
	}
	END_BLOCK(1);

	BEGIN_BLOCK(2);
	TCHAR szFileName[] = TEXT("bbb.stg");
	CComPtr<IStorage> spIStorage;
	DWORD grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE;
	hr = CMyStgHelper::CreateFileStorage(szFileName, &spIStorage,  grfMode);
	if (SUCCEEDED(hr))
	{
		CAllVolumeInfo MyAllVolumeInfo;
		MyAllVolumeInfo.Retrieve(TRUE);

		hr = MyAllVolumeInfo.WriteToStorage(spIStorage);
	}

	if (SUCCEEDED(hr))
	{
		CAllVolumeInfo MyAllVolumeInfo;
		MyAllVolumeInfo.ReadFromStorage(spIStorage);
	}
	END_BLOCK(2);

	BEGIN_BLOCK(3);
	TCHAR szFileName[] = TEXT("bbb.stg");
	CComPtr<IStorage> spIStorage;
	DWORD grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE;
	hr = CMyStgHelper::OpenFileStorage(szFileName, &spIStorage,  grfMode);
	if (SUCCEEDED(hr))
	{
		CAllVolumeInfo MyAllVolumeInfo;
		MyAllVolumeInfo.ReadFromStorage(spIStorage);
	}
	END_BLOCK(3);

	BEGIN_BLOCK(4);
	TCHAR szFileNameDisk[] = TEXT("ccc.stg");
	CComPtr<IStorage> spIStorageDisk;
	DWORD grfModeDisk = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_DELETEONRELEASE;
	hr = CMyStgHelper::CreateFileStorage(szFileNameDisk, &spIStorageDisk,  grfModeDisk);

	TCHAR szFileName[] = TEXT("ddd.stg");
	CComPtr<IStorage> spIStorageVolume;
	DWORD grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_DELETEONRELEASE;
	hr = CMyStgHelper::CreateFileStorage(szFileName, &spIStorageVolume,  grfMode);

	if (SUCCEEDED(hr))
	{


		CAllVdsDiskVolumeInfo MyAllVdsDiskVolumeInfo;
		MyAllVdsDiskVolumeInfo.Retrieve();

		hr = MyAllVdsDiskVolumeInfo.WriteVdsDiskToStorage(spIStorageDisk);

		hr = MyAllVdsDiskVolumeInfo.WriteVdsVolumeToStorage(spIStorageVolume);
	}

	if (SUCCEEDED(hr))
	{
		CAllVdsDiskVolumeInfo MyAllVdsDiskVolumeInfo;
		MyAllVdsDiskVolumeInfo.ReadVdsDiskFromStorage(spIStorageDisk);
		MyAllVdsDiskVolumeInfo.ReadVdsVolumeFromStorage(spIStorageVolume);
	}
	END_BLOCK(4);

	END_BLOCK(0);

	return hr;
}

int GetStringParentByLevel_test()
{

	{
		CString strParent = TEXT("d:\\zzz-ds-0714\\zzz\\test-vapp@155.35.93.144[0aae22b6-3db1-49c1-a3e3-24f5794e36f8]");
		CString strChild = TEXT("d:\\zzz-ds-0714\\zzz\\0aae22b6-3db1-49c1-a3e3-24f5794e36f8@liuwe05-vc55[503528fe-f973-be4f-4cce-c33b9717ff2f]\\");
		CString strRelative;
		CMiscHelper::GetParentRelativePath(strChild, strParent, strRelative);
	}

	{
		CString strParent = TEXT("D:\\zzz-ds-0714\\zzz\\test-vapp@155.35.93.144[0aae22b6-3db1-49c1-a3e3-24f5794e36f8]");
		CString strChild = TEXT("D:\\zzz-ds-0714\\zzz\\0aae22b6-3db1-49c1-a3e3-24f5794e36f8@liuwe05-vc55[503528fe-f973-be4f-4cce-c33b9717ff2f]\\VStore\\S0000000007\\");
		CString strRelative;
		CMiscHelper::GetParentRelativePath(strChild, strParent, strRelative);
	}

	{
		CString absPath1 = TEXT("q:\\path1\\sub2\\file.vhdx");
		CString relPath = TEXT("..\\..\\path2\\sub3\\parent.vhdx");
		CString absPath2;
		CMiscHelper::ConvertRelativeToAbsolutePath(absPath1, relPath, absPath2);

		CString strRelative;
		CMiscHelper::GetParentRelativePath(absPath2, absPath1, strRelative);

		ATLASSERT(TEXT("q:\\path2\\sub3\\parent.vhdx") == absPath2);

	}


	{
		CString absPath1 = TEXT("q:\\path2\\sub3\\file.vhdx");
		CString relPath = TEXT(".\\parent.vhdx");
		CString absPath2;
		CMiscHelper::ConvertRelativeToAbsolutePath(absPath1, relPath, absPath2);

		CString strRelative;
		CMiscHelper::GetParentRelativePath(absPath2, absPath1, strRelative);


		ATLASSERT(TEXT("q:\\path2\\sub3\\parent.vhdx") == absPath2);
	}


	{
		CString absPath1 = TEXT("xxxxxx");
		CString relPath = TEXT("q:\\path2\\sub3\\parent.vhdx");
		CString absPath2;
		CMiscHelper::ConvertRelativeToAbsolutePath(absPath1, relPath, absPath2);

		CString strRelative;
		CMiscHelper::GetParentRelativePath(absPath2, absPath1, strRelative);


		ATLASSERT(TEXT("q:\\path2\\sub3\\parent.vhdx") == absPath2);
	}


	{
		CString absPath1 = TEXT("q:\\path1\\sub2\\file.vhdx");
		CString relPath = TEXT(".\\.\\..\\..\\path2\\sub3\\parent.vhdx");
		CString absPath2;
		CMiscHelper::ConvertRelativeToAbsolutePath(absPath1, relPath, absPath2);

		CString strRelative;
		CMiscHelper::GetParentRelativePath(absPath2, absPath1, strRelative);


		ATLASSERT(TEXT("q:\\path2\\sub3\\parent.vhdx") == absPath2);
	}


	{
		CString absPath1 = TEXT("q:\\path2\\sub3\\file.vhdx");
		CString relPath = TEXT(".\\.\\.\\.\\.\\.\\parent.vhdx");
		CString absPath2;
		CMiscHelper::ConvertRelativeToAbsolutePath(absPath1, relPath, absPath2);

		CString strRelative;
		CMiscHelper::GetParentRelativePath(absPath2, absPath1, strRelative);

		ATLASSERT(TEXT("q:\\path2\\sub3\\parent.vhdx") == absPath2);
	}



	LPCTSTR str = TEXT("\\\\\\\\c:\\\\111\\222\\33\\");

	for (int level = 1; level < 6; ++ level)
	{
		CString strParent;
		CMiscHelper::GetStringParentByLevel(strParent, str, level, TEXT('\\'));

		int x = 0;
	}

	return 0;
}

int test_volume_path_name()
{
	LPCTSTR path[] = 
	{
		TEXT("\\\\?\\F:\\MyProjects\\~MyCommonLibs\\~myfiles\\790"),
		TEXT("\\\\?\\F:\\MyProjects\\~MyCommonLibs-xxxx\\~myfiles\\790"),
		TEXT("F:\\"),
		TEXT("F:"),
		TEXT("\\\\?\\Volume{1BE5E1C3-38D7-11E1-9098-806E6F6E6963}\\MyProjects\\~MyCommonLibs\\~myfiles\\790"),
		TEXT("\\\\?\\Volume{1BE5E1C3-38D7-11E1-9098-806E6F6E6963}\\MyProjects\\~MyCommonLibs-xxxx\\~myfiles\\790"),
		TEXT("\\\\?\\Volume{1BE5E1C3-38D7-11E1-9098-806E6F6E6963}\\"),
		TEXT("\\\\?\\Volume{1BE5E1C3-38D7-11E1-9098-806E6F6E6963}"),
		TEXT("S:\\MyProjects\\~MyCommonLibs\\~myfiles\\790"),
		TEXT("S:\\MyProjects\\~MyCommonLibs-xxxx\\~myfiles\\790"),
		TEXT("S:\\"),
		TEXT("S:"),
		TEXT("\\\\lijyo02-9010\\f$\\sonmi01\\Debug"),
		TEXT("\\\\lijyo02-9010\\f$\\sonmi01-xxx\\Debug"),
		TEXT("\\\\lijyo02-9010\\s$\\sonmi01\\Debug"),
		TEXT("\\\\lijyo02-9010\\s$\\sonmi01-xxx\\Debug"),
		TEXT("\\\\lijyo02-9010\\s$\\"),
		TEXT("\\\\lijyo02-9010\\s$"),
		TEXT("\\\\lijyo02-9010-xxx\\s$\\"),
		TEXT("\\\\lijyo02-9010-xxx\\s$"),
		TEXT("\\\\lijyo02-9010"),
		TEXT("\\\\lijyo02-9010-xx"),
	};

	for (LONG ii = 0; ii < _countof(path); ++ ii)
	{
		CString strGuidPath;
		CMiscHelper::AbsoluteWin32PathToVolumepath(path[ii], strGuidPath);
		0 == 0;
	}
	
	return 0;
}

HRESULT test_AllDiskVolumeInformationTest_xml()
{

	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CAllVolumeInfo avi;
	avi.Retrieve(FALSE);

	CAllVolumeInfoXml aviXml;
	BinXmlConv_NS::Bin2Xml(avi, aviXml);

	LPCTSTR xml1 = TEXT("CAllVolumeInfoXml1.xml");
	aviXml.ToFile(FIELD_NAME(CAllVolumeInfoXml), xml1);

	CAllVolumeInfoXml aviXml2;
	aviXml2.FromFile(FIELD_NAME(CAllVolumeInfoXml), xml1);

	CAllVolumeInfo avi2;
	BinXmlConv_NS::Xml2Bin(aviXml2, avi2);
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

HRESULT test_AllDiskInformationTest_xml()
{


	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CAllDiskInfo adi;
	adi.Retrieve(16);

	CAllDiskInfoXml adiXml;
	BinXmlConv_NS::Bin2Xml(adi, adiXml);

	LPCTSTR xml1 = TEXT("CAllDiskInfoXml1.xml");
	adiXml.ToFile(FIELD_NAME(CAllDiskInfo), xml1);

	CAllDiskInfoXml adiXml2;
	adiXml2.FromFile(FIELD_NAME(CAllDiskInfo), xml1);

	CAllDiskInfo avi2;
	BinXmlConv_NS::Xml2Bin(adiXml2, avi2);
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;


}


HRESULT test_AllVdsDiskVolInformationTest_xml()
{


	HRESULT hr = S_OK;
	DWORD LastError = 0;

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CAllVdsDiskVolumeInfo var;
	var.Retrieve();

	CAllVdsDiskVolumeInfoXml varXml;
	BinXmlConv_NS::Bin2Xml(var, varXml);

	LPCTSTR xml1 = TEXT("CAllVdsDiskVolumeInfo") TEXT("1.xml");
	varXml.ToFile(FIELD_NAME(CAllVdsDiskVolumeInfo), xml1);

	CAllVdsDiskVolumeInfoXml varXml2;
	varXml2.FromFile(FIELD_NAME(CAllVdsDiskVolumeInfo), xml1);

	CAllVdsDiskVolumeInfo var2;
	BinXmlConv_NS::Xml2Bin(varXml2, var2);
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	CoUninitialize();

	return hr;


}

int _tmain_test_copy_file_tree(int argc, _TCHAR* argv[])
{
	CMiscHelper::CopyFileTreeEx(argv[2], argv[3], argv[4]);
	return 0;
}


int _tmain_test_start_service(int argc, _TCHAR* argv[])
{
	MyServiceConfig sconf;
	MyQueryService(argv[2], sconf);
	MyStopService(argv[2]);
	MyQueryService(argv[2], sconf);
	MyStartService(argv[2], 0, nullptr);
	MyQueryService(argv[2], sconf);

	MyQueryService(argv[2], sconf);
	MyStopService(argv[2]);
	MyQueryService(argv[2], sconf);
	MyStartService(argv[2], 0, nullptr);
	MyQueryService(argv[2], sconf);

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	D_SETTINGS(99999, FALSE, TEXT(""), TEXT(""));

	CComInit coInit(COINIT_APARTMENTTHREADED);

	int testcase = 0;
	if (argc > 1)
	{
		testcase = _tcstol(argv[1], nullptr, 0);
	}

	switch (testcase)
	{
	case 16:
	{
		TestVolumeBlockOffset(argv[1], 64, 32);
		//no closeHandle or feeMem, do not use for production
	}
	break;

	case 15:
	{
		USHORT usImageType = 0;
		VS_FIXEDFILEINFO ffi = { 0 };
		vector<CString> vecstr;
		CFileVerInfo::GetImageFileVersion(argv[1], usImageType, ffi, vecstr);
	}
	break;
	case 14:
	{
		LPCTSTR psz = TEXT("123-abc-123-abc-123-abc-123-abc---");

		std::vector<LONGLONG> n1;
		CMiscHelper::ExtractDecNumbers(psz, n1, TRUE);

		std::vector<LONGLONG> n2;
		CMiscHelper::ExtractDecNumbers(psz, n2, FALSE);

		std::vector<LONGLONG> n3;
		CMiscHelper::ExtractHexNumbers(psz, n3);
	}
	break;
	case 13:
	{
		CMiscHelper::CreateDirectoryRecursively(argv[2]);
	}
	break;
	case 0:
		return test_AllVdsDiskVolInformationTest_xml();
		break;
	case 1:
		return test_AllDiskInformationTest_xml();
		break;
	case 2:
		return test_AllDiskVolumeInformationTest_xml();
		break;
	case 3:
		return GetStringParentByLevel_test();
		break;
	case 4:
		return test_volume_path_name();
		break;
	case 5:
		AllDiskVolumeInformationTest();
		break;
	case  6:
		_tmain_test_copy_file_tree(argc, argv);
		break;
	case  7:
		_tmain_test_start_service(argc, argv);
		break;
	case  8:
	{
		ModifyFileForDiskShadow(argv[2]);
		CString str = argv[2];
		str += TEXT(".cmd");
		ModifyFileForVShadow(str.GetString());
	}
	break;
	case 9:
	{
		bool b1 = WINVER_HELPER::IsWindowsVistaOrGreater();
		bool b2 = WINVER_HELPER::IsWindows7OrGreater();
		bool b3 = WINVER_HELPER::IsWindows8OrGreater();
		bool b4 = WINVER_HELPER::Is32BitWindows();
		bool b5 = WINVER_HELPER::Is64BitWindows();
		int x = 0;
	}
	break;
	case 10:
		DeployVMwareVSS();
		CheckVMwareVssFiles();
		break;
	case 11:
		UndeployVMwareVSS();
		CheckVMwareVssFiles();
		break;
	case 12:
	{
		if (argc < 4)
		{
			D_INFO(0, TEXT("Commandline is: \t this.exe 12 <path1> <path2>"));
			PRINT_LINE(L"Commandline is: \t this.exe 12 <path1> <path2>");
			break;
		}
		CString strRelative;
		CMiscHelper::GetParentRelativePath(argv[2], argv[3], strRelative);

		D_INFO(0, TEXT("Paht1: \t %s"), argv[2]);
		D_INFO(0, TEXT("Paht2: \t %s"), argv[3]);
		D_INFO(0, TEXT("Relative: \t %s"), strRelative.GetString());

		PRINT_LINE(TEXT("Paht1: \t %s"), argv[2]);
		PRINT_LINE(TEXT("Paht2: \t %s"), argv[3]);
		PRINT_LINE(TEXT("Relative path: \t %s"), strRelative.GetString());
		
	}
	break;

	}

	return 0;
}

