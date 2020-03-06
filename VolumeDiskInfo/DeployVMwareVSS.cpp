#include "stdafx.h"

#include <Shlobj.h>
#include <fstream>

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicFileTreeOp.h"

#include "DeployVMwareVSS.h"
#include "MyVersionHelpers.h"
#include "ServiceManagerHelper.h"
#include "MiscHelper.h"
#include "ModifyFileForVSS.h"



GUEST_VSS_USE_METHOD DetectVssMethod()
{
	using namespace WINVER_HELPER;

	GUEST_VSS_USE_METHOD vssMethod = GUEST_VSS_USE_METHOD::UNKNOWN;

	if (IsWindowsServer())
	{
		D_INFO(0, TEXT("%s"), TEXT("IsWindowsServer"));
		if (IsWindowsVistaOrGreater())
		{
			D_INFO(0, TEXT("%s"), TEXT("IsWindowsVistaOrGreater"));
			vssMethod = GUEST_VSS_USE_METHOD::DISKSHADOW;
		}
		else
		{
			D_INFO(0, TEXT("%s"), TEXT("Not IsWindowsVistaOrGreater"));
			if (Is32BitWindows())
			{
				D_INFO(0, TEXT("%s"), TEXT("Is32BitWindows"));
				vssMethod = GUEST_VSS_USE_METHOD::VSHADOW_32_WIN03;
			}
			else
			{
				D_INFO(0, TEXT("%s"), TEXT("Not Is32BitWindows"));
				vssMethod = GUEST_VSS_USE_METHOD::VSHADOW_64_WIN03;
			}
		}
	}
	else
	{
		D_INFO(0, TEXT("%s"), TEXT("Not IsWindowsServer"));
		if (IsWindowsVistaOrGreater())
		{
			D_INFO(0, TEXT("%s"), TEXT("IsWindowsVistaOrGreater"));
			if (Is32BitWindows())
			{
				D_INFO(0, TEXT("%s"), TEXT("Is32BitWindows"));
				vssMethod = GUEST_VSS_USE_METHOD::VSHADOW_32_VISTA;
			}
			else
			{
				D_INFO(0, TEXT("%s"), TEXT("Not Is32BitWindows"));
				vssMethod = GUEST_VSS_USE_METHOD::VSHADOW_64_VISTA;
			}
		}
		else
		{
			D_INFO(0, TEXT("%s"), TEXT("Not IsWindowsVistaOrGreater"));
			if (Is32BitWindows())
			{
				D_INFO(0, TEXT("%s"), TEXT("Is32BitWindows"));
				vssMethod = GUEST_VSS_USE_METHOD::VSHADOW_32_WIN03;
			}
			else
			{
				D_INFO(0, TEXT("%s"), TEXT("Not Is32BitWindows"));
				vssMethod = GUEST_VSS_USE_METHOD::VSHADOW_64_WIN03;
			}
		}
	}

	return vssMethod;
}

//C:\Program Files\VMware\VMware Tools\vmtoolsd.exe
//return C:\Program Files\VMware\VMware Tools
HRESULT GetVMwareToolsInstallationFolder(CString & strVMwareTools)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	static const TCHAR SERVICE_NAME[] = TEXT("VMTools");
	MyServiceConfig serviceConf;
	hr = MyQueryService(SERVICE_NAME, serviceConf);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("MyQueryService"), hr, TEXT("%s"), SERVICE_NAME);
		LEAVE_BLOCK(0);
	}
	D_INFO(0, TEXT("VMTools image path=%s"), serviceConf.lpBinaryPathName.GetString());

	//////////////////////////////////////////////////////////////////////////
	strVMwareTools = serviceConf.lpBinaryPathName;
	strVMwareTools.Trim(TEXT("\""));
	LPTSTR strBuffer = strVMwareTools.GetBuffer();
	LPTSTR pTemp = _tcsrchr(strBuffer, TEXT('\\'));
	if (pTemp)
	{
		pTemp[0] = 0;
	}
	strVMwareTools.ReleaseBuffer();

	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}


HRESULT GetPath_backupscripts_d(CString & strbackupscripts_d)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = GetVMwareToolsInstallationFolder(strbackupscripts_d);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetVMwareToolsInstallationFolder"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	static CONST TCHAR backupscripts_d[] = TEXT("\\backupscripts.d");
	strbackupscripts_d += backupscripts_d;
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}


HRESULT GetFilePath_tools_conf(CString & strtools_conf)
{
	static CONST TCHAR TOOLS_CONF[] = TEXT("\\VMware\\VMware Tools\\tools.conf");

	CONST INT BUFFER_SIZE = 512;
	LPTSTR Buffer = strtools_conf.GetBuffer(BUFFER_SIZE);
	HRESULT hr = SHGetFolderPath(
		nullptr,//_In_   HWND hwndOwner,
		CSIDL_COMMON_APPDATA,//_In_   int nFolder,
		nullptr,//_In_   HANDLE hToken,
		0,//_In_   DWORD dwFlags,
		Buffer//_Out_  LPTSTR pszPath
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
		D_API_ERR(0, TEXT("SHGetFolderPath"), hr, TEXT("CSIDL_COMMON_APPDATA"));
		//LEAVE_BLOCK(0);
	}
	else
	{
		strtools_conf.ReleaseBuffer();
		strtools_conf += TOOLS_CONF;
	}
	return hr;
}



HRESULT DeployVMwareVSS()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CONST LONG BUFFER_SIZE = 512;
	CString strDeploySource;
	LPTSTR pBuffer = strDeploySource.GetBuffer(BUFFER_SIZE);
	DWORD dwGMFN = GetModuleFileName(
		nullptr,//_In_opt_  HMODULE hModule,
		pBuffer,//_Out_     LPTSTR lpFilename,
		BUFFER_SIZE//_In_      DWORD nSize
		);
	if (0 == dwGMFN)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetModuleFileName"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	LPTSTR pTemp = _tcsrchr(pBuffer, TEXT('\\'));
	if (pTemp)
	{
		pTemp[0] = 0;
		pTemp = _tcsrchr(pBuffer, TEXT('\\'));
		if (pTemp)
		{
			pTemp[0] = 0;
		}
	}
	strDeploySource.ReleaseBuffer();

	//////////////////////////////////////////////////////////////////////////
	static CONST TCHAR target_root_dir[] = TEXT("c:\\");
	static CONST TCHAR for_windows_client[] = TEXT("for-windows-client");
	static CONST TCHAR for_windows_server[] = TEXT("for-windows-server");
	static CONST TCHAR as_hbbu_vmwarebackup[] = TEXT("as-hbbu-vmwarebackup");
	static CONST TCHAR backupscripts_d[] = TEXT("backupscripts.d");
	static CONST TCHAR tools_conf[] = TEXT("tools.conf");

	GUEST_VSS_USE_METHOD vssMethod = DetectVssMethod();
	if (GUEST_VSS_USE_METHOD::DISKSHADOW == vssMethod)
	{
		strDeploySource += TEXT("\\");
		strDeploySource += for_windows_server;
	}
	else
	{
		strDeploySource += TEXT("\\");
		strDeploySource += for_windows_client;
	}

	//////////////////////////////////////////////////////////////////////////
	BOOL bSupportedGuest = (
		GUEST_VSS_USE_METHOD::DISKSHADOW == vssMethod || 
		GUEST_VSS_USE_METHOD::VSHADOW_64_VISTA == vssMethod
		);
	if (!bSupportedGuest)
	{
		D_INFO(0, TEXT("Auto-deployment does not support the guest OS, please use mannual deployment as in the documentation"));
		D_INFO(0, TEXT("The supported guest OS are [%s], [%s]"),
			TEXT("Windows Server - 2008 and later - 32 and 64 bit"),
			TEXT("Windows client - Vista and later - 64bit only")
			);
		hr = HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
		LEAVE_BLOCK(0);
	}
	

	//////////////////////////////////////////////////////////////////////////
	//copy
	//G:\~~~workspace\ad2d-r17-oolong\!my\remote_vddk_mount\how-to-check-and-fix-vm-snapshot-data-corrunptions\custom-freeze-vmware-snapshot\for-windows-client\as-hbbu-vmwarebackup
	//or
	//G:\~~~workspace\ad2d-r17-oolong\!my\remote_vddk_mount\how-to-check-and-fix-vm-snapshot-data-corrunptions\custom-freeze-vmware-snapshot\for-windows-server\as-hbbu-vmwarebackup
	//to 
	//c:\as-hbbu-vmwarebackup
	CString pSourceDir = strDeploySource;
	pSourceDir.AppendFormat(TEXT("\\%s"), as_hbbu_vmwarebackup);
	CString pDestDir = target_root_dir;
	pDestDir += as_hbbu_vmwarebackup;
	hr = CMiscHelper::CopyFileTreeEx(pSourceDir, pDestDir, TEXT("*.*"));
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CopyFileTreeEx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	RemoveFileAttributesInFolder(pDestDir, FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN, TEXT("*.*"), TRUE);
	

	//////////////////////////////////////////////////////////////////////////
	//modify volumes in diskshadowfinish.vmr
	//or
	//modify volumes in backup.cmd
	static CONST TCHAR diskshadowfinish_vmr[] = TEXT("diskshadow.vmr");
	static CONST TCHAR backup_cmd[] = TEXT("backup.cmd");
	CString vssFileScript = pDestDir;
	if (GUEST_VSS_USE_METHOD::DISKSHADOW == vssMethod)
	{
		vssFileScript.AppendFormat(TEXT("\\%s"), diskshadowfinish_vmr);
		hr = ModifyFileForDiskShadow(vssFileScript);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("ModifyFileForDiskShadow"), hr, TEXT("%s"), vssFileScript.GetString());
			LEAVE_BLOCK(0);
		}
	}
	else
	{
		vssFileScript.AppendFormat(TEXT("\\%s"), backup_cmd);
		hr = ModifyFileForVShadow(vssFileScript);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("ModifyFileForVShadow"), hr, TEXT("%s"), vssFileScript.GetString());
			LEAVE_BLOCK(0);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	//copy
	//G:\~~~workspace\ad2d-r17-oolong\!my\remote_vddk_mount\how-to-check-and-fix-vm-snapshot-data-corrunptions\custom-freeze-vmware-snapshot\for-windows-client\backupscripts.d
	//G:\~~~workspace\ad2d-r17-oolong\!my\remote_vddk_mount\how-to-check-and-fix-vm-snapshot-data-corrunptions\custom-freeze-vmware-snapshot\for-windows-server\backupscripts.d
	//to
	//C:\Program Files\VMware\VMware Tools\backupscripts.d
	CString strSource_backupscripts_d;
	strSource_backupscripts_d = strDeploySource; 
	strSource_backupscripts_d.AppendFormat(TEXT("\\%s"), backupscripts_d);

	CString strTarget_backupscripts_d;
	hr = GetVMwareToolsInstallationFolder(strTarget_backupscripts_d);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetVMwareToolsInstallationFolder"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	strTarget_backupscripts_d.AppendFormat(TEXT("\\%s"), backupscripts_d);
	hr = CMiscHelper::CopyFileTreeEx(strSource_backupscripts_d, strTarget_backupscripts_d, TEXT("*.*"));
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CopyFileTreeEx"), hr, TEXT("%s --> %s"), strSource_backupscripts_d.GetString(), strTarget_backupscripts_d.GetString());
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	//copy
	//G:\~~~workspace\ad2d-r17-oolong\!my\remote_vddk_mount\how-to-check-and-fix-vm-snapshot-data-corrunptions\custom-freeze-vmware-snapshot\for-windows-client\tools.conf
	//G:\~~~workspace\ad2d-r17-oolong\!my\remote_vddk_mount\how-to-check-and-fix-vm-snapshot-data-corrunptions\custom-freeze-vmware-snapshot\for-windows-server\tools.conf
	//to
	//C:\ProgramData\VMware\VMware Tools\tools.conf
	CString strSource_tools_conf;
	strSource_tools_conf = strDeploySource;
	strSource_tools_conf.AppendFormat(TEXT("\\%s"), tools_conf);

	CString strTarget_tools_conf;
	hr = GetFilePath_tools_conf(strTarget_tools_conf);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetPath_backupscripts_d"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	CMiscHelper::__RemoveFileAttrs(strTarget_tools_conf.GetString(), FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN);
	BOOL bCF = CopyFile(
		strSource_tools_conf.GetString(),//_In_  LPCTSTR lpExistingFileName,
		strTarget_tools_conf.GetString(),//_In_  LPCTSTR lpNewFileName,
		FALSE//_In_  BOOL bFailIfExists
		);
	if (!bCF)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("CopyFile"), hr, TEXT("%s --> %s"), strSource_tools_conf.GetString(), strTarget_tools_conf.GetString());
		LEAVE_BLOCK(0);
	}
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);


	//////////////////////////////////////////////////////////////////////////
	//restart VMware tools service
	static CONST TCHAR SVC_NAME[] = TEXT("VMTools");
	CONST INT RETRY_TIMES = 5;
	CONST INT RETRY_INTERVALS = 3000;
	for (int ii = 0; ii < RETRY_TIMES; ++ ii)
	{
		hr = MyStopService(SVC_NAME);
		if (FAILED(hr))
		{
			Sleep(RETRY_INTERVALS);
			D_API_ERR(0, TEXT("MyStopService"), hr, TEXT("%s"), SVC_NAME);
			break;
		}
		else
		{
			Sleep(RETRY_INTERVALS);
			break;
		}
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}
	
	for (int ii = 0; ii < RETRY_TIMES; ++ii)
	{
		hr = MyStartService(SVC_NAME, 0, nullptr);
		if (FAILED(hr))
		{
			Sleep(RETRY_INTERVALS);
			D_API_ERR(0, TEXT("MyStartService"), hr, TEXT("%s"), SVC_NAME);
			break;
		}
		else
		{
			break;
		}
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	if (FAILED(hr))
	{
		D_INFO(0, TEXT("Failed to deploy VMware VSS customization, reverse opeations"));
		UndeployVMwareVSS();
		D_API_ERR(0, TEXT("Deploy failed"), hr, TEXT("")); //<sonmi01>2014-12-4 ###???
	}
	else
	{
		D_INFO(0, TEXT("Deploy success"), hr, TEXT(""));
	}
	
	return hr;
}

HRESULT UndeployVMwareVSS()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	//delete C:\ProgramData\VMware\VMware Tools\tools.conf
	CString strtools_conf;
	hr = GetFilePath_tools_conf(strtools_conf);
	if (SUCCEEDED(hr) && strtools_conf.GetLength())
	{
		hr = CMiscHelper::__DeleteFile(strtools_conf.GetString());
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("DeleteFile"), hr, TEXT("%s"), strtools_conf.GetString());
			//LEAVE_BLOCK(0);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	//delete C:\Program Files\VMware\VMware Tools\backupscripts.d\arcserve.bat
	CString str_arcserve_bat;
	static CONST TCHAR arcserve_bat[] = TEXT("\\backupscripts.d\\arcserve.bat");
	hr = GetVMwareToolsInstallationFolder(str_arcserve_bat);
	if (SUCCEEDED(hr))
	{
		str_arcserve_bat += arcserve_bat;
		hr = CMiscHelper::__DeleteFile(str_arcserve_bat.GetString());
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("DeleteFile"), hr, TEXT("%s"), str_arcserve_bat.GetString());
			//LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//restart VMware tools service
	static CONST TCHAR SVC_NAME[] = TEXT("VMTools");
	CONST INT RETRY_TIMES = 3;
	CONST INT RETRY_INTERVALS = 3000;
	for (int ii = 0; ii < RETRY_TIMES; ++ii)
	{
		hr = MyStopService(SVC_NAME);
		if (FAILED(hr))
		{
			Sleep(RETRY_INTERVALS);
			D_API_ERR(0, TEXT("MyStopService"), hr, TEXT("%s"), SVC_NAME);
			//break;
		}
		else
		{
			Sleep(RETRY_INTERVALS);
			break;
		}
	}

	for (int ii = 0; ii < RETRY_TIMES; ++ii)
	{
		hr = MyStartService(SVC_NAME, 0, nullptr);
		if (FAILED(hr))
		{
			Sleep(RETRY_INTERVALS);
			D_API_ERR(0, TEXT("MyStartService"), hr, TEXT("%s"), SVC_NAME);
			//break;
		}
		else
		{
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	static CONST TCHAR as_hbbu_vmwarebackup[] = TEXT("C:\\as-hbbu-vmwarebackup");
	hr = RemoveFileInFolder(as_hbbu_vmwarebackup, TEXT("*.*"), TRUE);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("RemoveFileInFolder"), hr, TEXT("%s"), as_hbbu_vmwarebackup);
		LEAVE_BLOCK(0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}


/*
C:\Program Files\ARCServe\ASVMOperationTools\custom-freeze-vmware-snapshot\auto-deploy\ASVMOperation.exe
C:\Program Files\ARCServe\as-hbbu-vmwarebackup\UDPVMSnapshotMethod.Sig
*/

//<sonmi01>2015-5-18 ###???
HRESULT CheckVMwareVssFiles()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BOOL fileExist = TRUE;

	CString strUDPVMSnapshotMethod_Sig;
	CString strCheckVMwareVssFiles;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	//delete C:\Program Files\ARCServe\ASVMOperationTools\custom-freeze-vmware-snapshot\auto-deploy\ASVMOperation.exe.CheckVMwareVssFiles.txt
	CONST LONG SIZE_LEN = 1024;
	CString strExe;
	LPTSTR strExeBuffer = strExe.GetBuffer(SIZE_LEN);
	DWORD dwGMFN = GetModuleFileName(
		nullptr,//_In_opt_  HMODULE hModule,
		strExeBuffer,//_Out_     LPTSTR lpFilename,
		SIZE_LEN//_In_      DWORD nSize
		);
	strExe.ReleaseBuffer();
	if (0 == dwGMFN)
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetModuleFileName"), hr, TEXT("%s"), strExe.GetString());
		LEAVE_BLOCK(0);
	}

	static CONST TCHAR CHECKVMWAREVSSFILES[] = TEXT(".CheckVMwareVssFiles.txt");
	strCheckVMwareVssFiles = strExe + CHECKVMWAREVSSFILES;
	CMiscHelper::__DeleteFile(strCheckVMwareVssFiles.GetString());

	//////////////////////////////////////////////////////////////////////////
	/*
	C:\Program Files\ARCServe\ASVMOperationTools\custom-freeze-vmware-snapshot\auto-deploy\ASVMOperation.exe 
	=>
	C:\Program Files\ARCServe
	*/
	CString strRoot;
	CMiscHelper::GetStringParentByLevel(strRoot, strExe, 4, TEXT('\\'));

	//////////////////////////////////////////////////////////////////////////
	//C:\Program Files\ARCServe\as-hbbu-vmwarebackup\UDPVMSnapshotMethod.Sig
	static CONST TCHAR UDPVMSNAPSHOTMETHOD_SIG[] = TEXT("\\as-hbbu-vmwarebackup\\UDPVMSnapshotMethod.Sig");
	strUDPVMSnapshotMethod_Sig = strRoot + UDPVMSNAPSHOTMETHOD_SIG;
	if (!CMiscHelper::IsFileExists(strUDPVMSnapshotMethod_Sig.GetString()))
	{
		D_INFO(0, TEXT("Sig file %s does not exist, no need to check more."), strUDPVMSnapshotMethod_Sig.GetString());
		LEAVE_BLOCK(0);
	}


	//delete C:\ProgramData\VMware\VMware Tools\tools.conf
	CString strtools_conf;
	hr = GetFilePath_tools_conf(strtools_conf);
	if (SUCCEEDED(hr) && strtools_conf.GetLength())
	{
		fileExist = CMiscHelper::IsFileExists(strtools_conf.GetString());
		if (!fileExist)
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("File does not Exists"), hr, TEXT("%s"), strtools_conf.GetString());
			LEAVE_BLOCK(0);
		}
	}

	/*
	[vmbackup]
	vss.disableAppQuiescing = true
	*/

	TCHAR szdisableAppQuiescing[64] = { 0 };
	DWORD dwGPPS = GetPrivateProfileString(
		L"vmbackup",//_In_   LPCTSTR lpAppName,
		L"vss.disableAppQuiescing",//_In_   LPCTSTR lpKeyName,
		L"false",//_In_   LPCTSTR lpDefault,
		szdisableAppQuiescing,//_Out_  LPTSTR lpReturnedString,
		_countof(szdisableAppQuiescing),//_In_   DWORD nSize,
		strtools_conf.GetString()//_In_   LPCTSTR lpFileName
		);
	if (_tcscmp(L"true", szdisableAppQuiescing)) // szdisableAppQuiescing is not true
	{
		D_INFO(0, TEXT("[vmbackup] vss.disableAppQuiescing is not set to true. [%s]"), strtools_conf.GetString());
		fileExist = FALSE;
		LEAVE_BLOCK(0);
	}




	//////////////////////////////////////////////////////////////////////////
	//delete C:\Program Files\VMware\VMware Tools\backupscripts.d\arcserve.bat
	CString str_arcserve_bat;
	static CONST TCHAR arcserve_bat[] = TEXT("\\backupscripts.d\\arcserve.bat");
	hr = GetVMwareToolsInstallationFolder(str_arcserve_bat);
	if (SUCCEEDED(hr))
	{
		str_arcserve_bat += arcserve_bat;
		fileExist = CMiscHelper::IsFileExists(str_arcserve_bat.GetString());
		if (!fileExist)
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("File does not Exists"), hr, TEXT("%s"), str_arcserve_bat.GetString());
			LEAVE_BLOCK(0);
		}
	}
		
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	if (!fileExist)
	{
		//delete File does not Exists
		//delete sig file so as to deploy again
		D_INFO(0, TEXT("Some UDP file for VMware VSS are missing inside guest, delete sig file so as to deploy again [%s]"), strUDPVMSnapshotMethod_Sig.GetString());
		CMiscHelper::__DeleteFile(strUDPVMSnapshotMethod_Sig.GetString());

		wofstream ofs(strCheckVMwareVssFiles.GetString());
		ofs << L"fileMissing" << endl;
	}
	else
	{
		wofstream ofs(strCheckVMwareVssFiles.GetString());
		ofs << L"fileOk" << endl;
	}

	return hr;
}