#include "stdafx.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <conio.h>

#include <vector>
#include <string>
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////////
#include "..\..\PublicHeader\PublicWideNarrowChar.h"
#include "..\..\PublicHeader\PublicFileLockInterface.h"
#include "..\..\PublicHeader\PublicMiscHelper.h"
#include "..\..\PublicHeader\PublicDiskImageInterface.h"
#include "..\..\PublicHeader\PublicMTCopy.h"


//////////////////////////////////////////////////////////////////////////
#include "MountParams.h"
#include "RemoteVddkMount.h"
#include "PRINT_LINE.h"

#include "FileDiviceVMDK.h"
#include "VDDK_API.h"


//#pragma comment(lib, "D:/VMware-vix-disklib-5.5.2-1890828.x86_64/lib/vixDiskLib.lib")
//#pragma comment(lib, "D:/VMware-vix-disklib-5.5.2-1890828.x86_64/lib/vixMntapi.lib")

//////////////////////////////////////////////////////////////////////////
static HRESULT CopyVMDKToVHDX(VixDiskLibHandle diskHandle, LONGLONG diskSize, LPCTSTR vmdkName, LPCTSTR dir);

static void
LogFunc(const char *fmt, va_list args)
{
	printf("[VMware] Log: ");
	vprintf(fmt, args);
	printf("\r\n");
}


static void
WarnFunc(const char *fmt, va_list args)
{
	printf("[VMware] Warning: ");
	vprintf(fmt, args);
	printf("\r\n");
}


static void
PanicFunc(const char *fmt, va_list args)
{
	printf("[VMware] Panic: ");
	vprintf(fmt, args);
	printf("\r\n");
	//exit(10);
}


static
const char * str_to_ptr(const CStringA & strA)
{
	return strA.GetLength() ? strA.GetString() : NULL;
}

static
char * str_to_buffer(const CStringA & strA)
{
	return strA.GetLength() ? (char*)strA.GetString() : NULL;
}

//////////////////////////////////////////////////////////////////////////
static HRESULT GetVddkLibPathByModule(CString & strVddkLibPath)
{
	CONST LONG BUFFER_SIZE = 1024;
	LPTSTR pBuffer = strVddkLibPath.GetBuffer(BUFFER_SIZE);
	GetModuleFileName(
		NULL,//_In_opt_  HMODULE hModule,
		pBuffer,//_Out_     LPTSTR lpFilename,
		BUFFER_SIZE//_In_      DWORD nSize
		);
	for (int ii = 0; ii < 2; ++ ii)
	{
		LPTSTR pTemp =_tcsrchr(pBuffer, TEXT('\\'));
		if (pTemp)
		{
			pTemp[0] = 0;
		}
		else
		{
			break;
		}
	}
	strVddkLibPath.ReleaseBuffer();
	return 0;
}



//////////////////////////////////////////////////////////////////////////
#define VIXDISKLIB_VERSION_MAJOR 1
#define VIXDISKLIB_VERSION_MINOR 0

HRESULT RemoteVddkMount(CONST MountParams & params, IVolumeOperationCallback * pVolCallback)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	VixError err = VIX_OK;

	//////////////////////////////////////////////////////////////////////////
	VDDK_API vddkApi;
	vddkApi.Init();

	//////////////////////////////////////////////////////////////////////////
	VixDiskLibConnection connection = NULL; /***CLEANUP***/
	vector<VixDiskLibHandle> diskHandles; /***CLEANUP***/
	VixDiskSetHandle diskSetHandle = NULL; /***CLEANUP***/

	size_t numberOfVolumes = 0;
	VixVolumeHandle *volumeHandles = 0; /***CLEANUP***/
	vector<VixVolumeHandle> mountedVoluneHandle; /***CLEANUP***/

	vector<CString> mountPoints;  /**CLEANUP**/
	vector<CString> SymbolLinks;  /**CLEANUP**/

	//////////////////////////////////////////////////////////////////////////
	CString strLockFile;
	CONST LONG NAME_SIZE = 1024;
	LPTSTR strLockFileBuffer = strLockFile.GetBuffer(NAME_SIZE);
	GetModuleFileName(
		NULL,//_In_opt_  HMODULE hModule,
		strLockFileBuffer,//_Out_     LPTSTR lpFilename,
		NAME_SIZE//_In_      DWORD nSize
		);
	strLockFile.ReleaseBuffer();
	strLockFile += TEXT(".mount.lock");

	CComPtr<IFileLock> spFileLock;
	CreateInstancCFileLock(strLockFile.GetString(), &spFileLock, TRUE);


	VixDiskLibConnectParams * cnxParamsAllocated = nullptr;


	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CStringA libDir; 
	if (params.libDir.GetLength())
	{
		Utf16To8String(params.libDir, libDir);
	}
	else
	{
		CString strVddkLibPath;
		GetVddkLibPathByModule(strVddkLibPath);
		Utf16To8String(strVddkLibPath, libDir);
	}
	CStringA configFile; Utf16To8String(params.configFile, configFile);
	
	err = VixDiskLib_InitEx(
		VIXDISKLIB_VERSION_MAJOR,//uint32 majorVersion,
		VIXDISKLIB_VERSION_MINOR,//uint32 minorVersion,
		&LogFunc,//VixDiskLibGenericLogFunc *log,
		&WarnFunc,//VixDiskLibGenericLogFunc *warn,
		&PanicFunc,//VixDiskLibGenericLogFunc *panic,
		str_to_ptr(libDir),//const char* libDir,
		str_to_ptr(configFile)//const char* configFile
		);
	if (VIX_FAILED(err))
	{
		char * errMsg = VixDiskLib_GetErrorText(err, NULL);
		PRINT_LINE_A("VixDiskLib_InitEx failed with %I64d %s", err, errMsg);
		VixDiskLib_FreeErrorText(errMsg);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	err = VixMntapi_Init(
		VIXMNTAPI_MAJOR_VERSION,//uint32 majorVersion,
		VIXMNTAPI_MINOR_VERSION,//uint32 minorVersion,
		&LogFunc,//VixDiskLibGenericLogFunc *log,
		&WarnFunc,//VixDiskLibGenericLogFunc *warn,
		&PanicFunc,//VixDiskLibGenericLogFunc *panic,
		NULL,//const char* libDir,
		NULL//const char* configFile
		);
	if (VIX_FAILED(err))
	{
		char * errMsg = VixDiskLib_GetErrorText(err, NULL);
		PRINT_LINE_A("VixMntapi_Init failed with %I64d %s", err, errMsg);
		VixDiskLib_FreeErrorText(errMsg);
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	CStringA vmxSpec; Utf16To8String(params.vmxSpec, vmxSpec);
	CStringA serverName; Utf16To8String(params.serverName, serverName);
	CStringA userName; Utf16To8String(params.userName, userName);
	CStringA password; Utf16To8String(params.password, password);
	CStringA thumbPrint; Utf16To8String(params.thumbPrint, thumbPrint);
	VixDiskLibConnectParams cnxParamsLocal = { 0 };
	cnxParamsAllocated = vddkApi.VixDiskLib_AllocateConnectParams? vddkApi.VixDiskLib_AllocateConnectParams() : nullptr;
	VixDiskLibConnectParams& cnxParams = cnxParamsAllocated ? *cnxParamsAllocated : cnxParamsLocal;
	
	cnxParams.vmxSpec = str_to_buffer(vmxSpec);
	cnxParams.serverName = str_to_buffer(serverName);
	cnxParams.credType = VIXDISKLIB_CRED_UID;
	cnxParams.creds.uid.userName = str_to_buffer(userName);
	cnxParams.creds.uid.password = str_to_buffer(password);
	cnxParams.thumbPrint = str_to_buffer(thumbPrint);
	cnxParams.port = params.port;

	CStringA snapshotRef; Utf16To8String(params.snapshotRef, snapshotRef);
	CStringA transportModes; Utf16To8String(params.transportModes, transportModes);
	err = VixDiskLib_ConnectEx(
		&cnxParams,//const VixDiskLibConnectParams *connectParams,
		params.readOnly,//Bool readOnly,
		str_to_ptr(snapshotRef),//const char *snapshotRef,
		str_to_ptr(transportModes),//const char *transportModes,
		&connection//VixDiskLibConnection *connection
		);
	if (VIX_FAILED(err))
	{
		char * errMsg = VixDiskLib_GetErrorText(err, NULL);
		PRINT_LINE_A("VixDiskLib_ConnectEx failed with %I64d %s", err, errMsg);
		VixDiskLib_FreeErrorText(errMsg);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	for (size_t ii = 0; ii < params.diskPath.size(); ++ii)
	{
		CStringA diskPath; Utf16To8String(params.diskPath[ii], diskPath);
		VixDiskLibHandle diskHandleTmp = NULL;
		err = VixDiskLib_Open(
			connection,//const VixDiskLibConnection connection,
			str_to_ptr(diskPath),//const char *path,
			params.readOnly ? VIXDISKLIB_FLAG_OPEN_READ_ONLY : 0,//uint32 flags,
			&diskHandleTmp//VixDiskLibHandle *diskHandle
			);
		if (VIX_FAILED(err))
		{
			char * errMsg = VixDiskLib_GetErrorText(err, NULL);
			PRINT_LINE_A("VixDiskLib_Open failed with %I64d %s %s", err, errMsg, diskPath.GetString());
			VixDiskLib_FreeErrorText(errMsg);
			break;
		}


		VixDiskLibInfo *info = nullptr;
		err = VixDiskLib_GetInfo(
			diskHandleTmp,//VixDiskLibHandle diskHandle,
			&info//,//VixDiskLibInfo **info
			);
		CONST LONG SECTOR_SIZE = 512;
		LONGLONG diskSize = info->capacity * SECTOR_SIZE;
		VixDiskLib_FreeInfo(info); info = nullptr;

		if (params.DumpToDirectory.GetLength())
		{
			CopyVMDKToVHDX(diskHandleTmp, diskSize, params.diskPath[ii], params.DumpToDirectory);
		}
		

		diskHandles.push_back(diskHandleTmp);
	}
	if (VIX_FAILED(err))
	{
		LEAVE_BLOCK(0);
	}


	if (0 == diskHandles.size())
	{
		PRINT_LINE_A("VixDiskLib_Open - no VMDK opened, please check disk urls");
		hr = E_FAIL;
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	err = VixMntapi_OpenDiskSet(
		&diskHandles[0],//VixDiskLibHandle diskHandles[],
		diskHandles.size(),//size_t numberOfDisks,
		params.readOnly ? VIXDISKLIB_FLAG_OPEN_READ_ONLY : 0,//uint32 openMode,
		&diskSetHandle//VixDiskSetHandle *handle
		);
	if (VIX_FAILED(err))
	{
		char * errMsg = VixDiskLib_GetErrorText(err, NULL);
		PRINT_LINE_A("VixMntapi_OpenDiskSet failed with %I64d %s", err, errMsg);
		VixDiskLib_FreeErrorText(errMsg);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	VixDiskSetInfo *diskSetInfo = NULL;
	err =
		VixMntapi_GetDiskSetInfo(
		diskSetHandle,//VixDiskSetHandle handle,
		&diskSetInfo//VixDiskSetInfo **diskSetInfo
		);
	if (VIX_FAILED(err))
	{
		char * errMsg = VixDiskLib_GetErrorText(err, NULL);
		PRINT_LINE_A("VixMntapi_GetDiskSetInfo failed with %I64d %s", err, errMsg);
		VixDiskLib_FreeErrorText(errMsg);
		//LEAVE_BLOCK(0);
	}
	VixMntapi_FreeDiskSetInfo(diskSetInfo);

	//////////////////////////////////////////////////////////////////////////
	if (params.bGetOsInfo)
	{
		VixOsInfo *info = NULL;
		err = VixMntapi_GetOsInfo(
			diskSetHandle,//VixDiskSetHandle diskSet,
			&info//VixOsInfo **info
			);
		if (VIX_FAILED(err))
		{
			char * errMsg = VixDiskLib_GetErrorText(err, NULL);
			PRINT_LINE_A("VixMntapi_GetOsInfo failed with %I64d %s", err, errMsg);
			VixDiskLib_FreeErrorText(errMsg);
			//LEAVE_BLOCK(0);
		}
		VixMntapi_FreeOsInfo(info);
	}

	//////////////////////////////////////////////////////////////////////////
	err = VixMntapi_GetVolumeHandles(
		diskSetHandle,//VixDiskSetHandle diskSet,
		&numberOfVolumes,//size_t *numberOfVolumes,
		&volumeHandles//VixVolumeHandle *volumeHandles[]
		);
	if (VIX_FAILED(err))
	{
		char * errMsg = VixDiskLib_GetErrorText(err, NULL);
		PRINT_LINE_A("VixMntapi_GetVolumeHandles failed with %I64d %s", err, errMsg);
		VixDiskLib_FreeErrorText(errMsg);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	BOOL bFirstTimeMountPoint = TRUE;

	//////////////////////////////////////////////////////////////////////////
	if (bFirstTimeMountPoint)
	{
		if (spFileLock)
		{
			PRINT_LINE_A("Begin to acqurie lock to mount");
			hr = spFileLock->LockEx(3000, (ULONG)(-2));
			if (FAILED(hr))
			{
				PRINT_LINE_A("failed to acquire lock to mount");
			}
			else
			{
				PRINT_LINE_A("Acquired lock to mount");
			}
		}

		bFirstTimeMountPoint = FALSE;
	}

	CString driveletter = params.DriverLetter;
	for (size_t ii = 0; ii < numberOfVolumes; ++ii)
	{
		VixVolumeHandle volumeHandle = volumeHandles[ii];
		VixVolumeInfo *volumeInfo = NULL;

		err = VixMntapi_MountVolume(volumeHandle, params.readOnly);
		if (VIX_FAILED(err) && VIX_E_MNTAPI_VOLUME_ALREADY_MOUNTED != err)
		{
			char * errMsg = VixDiskLib_GetErrorText(err, NULL);
			PRINT_LINE_A("VixMntapi_MountVolume failed with %I64d %s", err, errMsg);
			VixDiskLib_FreeErrorText(errMsg);
		}
		else
		{
			mountedVoluneHandle.push_back(volumeHandle);
			err = VixMntapi_GetVolumeInfo(volumeHandle, &volumeInfo);
			if (VIX_FAILED(err))
			{
				char * errMsg = VixDiskLib_GetErrorText(err, NULL);
				PRINT_LINE_A("VixMntapi_GetVolumeInfo failed with %I64d %s", err, errMsg);
				VixDiskLib_FreeErrorText(errMsg);
				//LEAVE_BLOCK(0);
			}
			else
			{

				CString strSymbolLink(volumeInfo->symbolicLink);
				SymbolLinks.push_back(strSymbolLink);
				strSymbolLink.Replace(TEXT("\\\\.\\"), TEXT("\\Device\\"));
				strSymbolLink.TrimRight(TEXT('\\'));
				//assign_drive_letter(driveletter, strSymbolLink.GetString());

				////////////////////////////////////////////////////////////////////////////
				//if (bFirstTimeMountPoint)
				//{
				//	if (spFileLock)
				//	{
				//		PRINT_LINE_A("Begin to acqurie lock to mount");
				//		hr = spFileLock->LockEx(3000, (ULONG)(-2));
				//		if (FAILED(hr))
				//		{
				//			PRINT_LINE_A("failed to acquire lock to mount");
				//		}
				//		else
				//		{
				//			PRINT_LINE_A("Acquired lock to mount");
				//		}
				//	}

				//	bFirstTimeMountPoint = FALSE;
				//}

				//////////////////////////////////////////////////////////////////////////
				BOOL bRet = DefineDosDevice(
					DDD_RAW_TARGET_PATH,//_In_      DWORD dwFlags,
					driveletter,//_In_      LPCTSTR lpDeviceName,
					strSymbolLink.GetString()//_In_opt_  LPCTSTR lpTargetPath
					);
				if (!bRet)
				{
					LastError = GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					PRINT_LINE_A("DefineDosDeviceA failed with %d", hr);
					mountPoints.push_back(L"");
				}
				else
				{
					mountPoints.push_back(driveletter + TEXT('\\'));
					//cout << "Mounted volume: " << driveletter.GetString() <<  << strSymbolLink.GetString() << endl;
				}
				++ (driveletter.GetBuffer()[0]);
			}
			VixMntapi_FreeVolumeInfo(volumeInfo);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);


	//////////////////////////////////////////////////////////////////////////
	_tprintf_s(TEXT("\r\n") TEXT("\r\n") TEXT("\r\n"));
	//for (CONST auto & var : mountPoints)
	//{
	//	_tprintf_s(TEXT("Mount points: %s") TEXT("\r\n"), var.GetString());
	//}

	for (size_t ii = 0; ii < mountPoints.size(); ++ ii)
	{
		_tprintf_s(TEXT("Mount points: %s") TEXT(" <==> %s") TEXT("\r\n"), mountPoints[ii].GetString(), SymbolLinks[ii].GetString());
	}


	if (pVolCallback)
	{
		if (params.bPressKey)
		{
			for (int ii = 0; ii < 2; ++ii)
			{
				cerr << "Press ENTER key to operating on volumes for " << ii << " times..." << endl;
				_getch();
			}
		}

		_tprintf_s(TEXT("\r\n") TEXT("\r\n") TEXT("Operating on volumes, please wait...") TEXT("\r\n") );
		pVolCallback->Do(mountPoints, SymbolLinks);
		_tprintf_s(TEXT("Operating on volumes finished") TEXT("\r\n"));
	}


	////***CLEANUP***/
	_tprintf_s(TEXT("\r\n") TEXT("\r\n") TEXT("\r\n"));
	if (params.bPressKey)
	{
		for (int ii = 0; ii < 2; ++ ii)
		{
			cerr << "Press ENTER key to dismount volumes and cleanup for " << ii << " times..." << endl;
			_getch();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if (params.bDismount)
	{
		//////////////////////////////////////////////////////////////////////////
		for (size_t ii = 0; ii < mountPoints.size(); ++ii)
		{
			BOOL bDVMP = DeleteVolumeMountPoint(
				mountPoints[ii].GetString()//_In_  LPCTSTR lpszVolumeMountPoint
				);
			if (!bDVMP)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				PRINT_LINE_A("DeleteVolumeMountPoint failed with %d", hr);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		for (size_t ii = 0; ii < mountedVoluneHandle.size(); ++ii)
		{
			err =
				VixMntapi_DismountVolume(
				mountedVoluneHandle[ii],//VixVolumeHandle volumeHandle,
				TRUE//Bool force
				);  mountedVoluneHandle[ii] = NULL;
			if (VIX_FAILED(err))
			{
				char * errMsg = VixDiskLib_GetErrorText(err, NULL);
				PRINT_LINE_A("VixMntapi_DismountVolume failed with %I64d %s", err, errMsg);
				VixDiskLib_FreeErrorText(errMsg);
				//LEAVE_BLOCK(0);
			}
		}
		mountedVoluneHandle.clear();

		//////////////////////////////////////////////////////////////////////////
		for (size_t ii = 0; ii < mountPoints.size(); ++ii)
		{
			BOOL bDVMP = DeleteVolumeMountPoint(
				mountPoints[ii].GetString()//_In_  LPCTSTR lpszVolumeMountPoint
				);
			if (!bDVMP)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				PRINT_LINE_A("DeleteVolumeMountPoint failed with %d", hr);
			}
		}

	}
	
	//////////////////////////////////////////////////////////////////////////
	if (volumeHandles)
	{
		VixMntapi_FreeVolumeHandles(volumeHandles); volumeHandles = NULL;
	}


	//////////////////////////////////////////////////////////////////////////
	if (diskSetHandle)
	{
		err = VixMntapi_CloseDiskSet(diskSetHandle); diskSetHandle = NULL;
		if (VIX_FAILED(err))
		{
			char * errMsg = VixDiskLib_GetErrorText(err, NULL);
			PRINT_LINE_A("VixMntapi_CloseDiskSet failed with %I64d %s", err, errMsg);
			VixDiskLib_FreeErrorText(errMsg);
			//LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if (spFileLock)
	{
		spFileLock->Unlock();
	}

	//////////////////////////////////////////////////////////////////////////
	for (size_t ii = 0; ii < diskHandles.size(); ++ii)
	{
		err = VixDiskLib_Close(diskHandles[ii]); diskHandles[ii] = NULL;
		if (VIX_FAILED(err))
		{
			char * errMsg = VixDiskLib_GetErrorText(err, NULL);
			PRINT_LINE_A("VixDiskLib_Close failed with %I64d %s", err, errMsg);
			VixDiskLib_FreeErrorText(errMsg);
			//LEAVE_BLOCK(0);
		}
	}
	diskHandles.clear();

	//////////////////////////////////////////////////////////////////////////
	if (connection)
	{
		err = VixDiskLib_Disconnect(connection); connection = NULL;
		if (VIX_FAILED(err))
		{
			char * errMsg = VixDiskLib_GetErrorText(err, NULL);
			PRINT_LINE_A("VixDiskLib_Disconnect failed with %I64d %s", err, errMsg);
			VixDiskLib_FreeErrorText(errMsg);
			//LEAVE_BLOCK(0);
		}
	}


	if (cnxParamsAllocated)
	{
		if (vddkApi.VixDiskLib_FreeConnectParams)
		{
			vddkApi.VixDiskLib_FreeConnectParams(cnxParamsAllocated);
		}
		cnxParamsAllocated = nullptr;
	}

	//////////////////////////////////////////////////////////////////////////
	VixMntapi_Exit();
	VixDiskLib_Exit();

	
	return hr;
}


static HRESULT CopyVMDKToVHDX(VixDiskLibHandle diskHandle, LONGLONG diskSize, LPCTSTR vmdkName, LPCTSTR dir)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDeviceFlush> spVMDK;
	hr = SmartLib::CreateInstanceCFileDeviceVMDK(diskHandle, &spVMDK, TRUE);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceCFileDeviceVMDK"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	CString vhdxName = vmdkName;
	CMiscHelper::ReplaceInvalidFileNameChar(vhdxName, TEXT('_'));
	CString fullVhdxname = dir;
	fullVhdxname += TEXT('\\');
	fullVhdxname += vhdxName;
	fullVhdxname += TEXT(".vhdx");


	CONST LONG BLOCK_SIZE = 1024 * 1024 * 2;
	CreateInstanceCDiskImageVhdxParams params =
	{
		nullptr,//LPCTSTR		parentVhdxFileName;
		fullVhdxname.GetString(),//LPCTSTR		vhdxFileName;
		GENERIC_WRITE,//DWORD		DesiredAccess;
		CREATE_NEW,//DWORD		CreationDisposition;
		AtlAlignUp(diskSize, BLOCK_SIZE),//LONGLONG	VirtualDiskSize;
		512,//LONG		LogicalSectorSize;
		4096,//LONG		PhysicalSectorSize;
		BLOCK_SIZE,//LONG		BlockSize;
		VHDX_DISK_TYPE_ENUM::DYNAMIC//LONG		VhdxDiskType; ,//<sonmi01>2014-4-21 ###??? ,//take effect only if create_new
	};
	CComPtr<IDiskImageVirtual> spVHDX;
	hr = CreateInstanceCDiskImageVhdx(params, &spVHDX, TRUE);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceCDiskImageVhdx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	BYTE buffer = 1;
	CSimpleBitArray sba(&buffer, 1);
	CComPtr<IFileCopy> spIFileCopy;
	//////////////////////////////////////////////////////////////////////////
	hr = CreateInstanceCMTCopy(
		spVMDK,//IFileDevice * pSource,
		spVHDX,//IFileDevice * pDest,
		0,//LONGLONG SourceStartOffset,			,// in case 0 == SourceStartOffset,  0 == Diff, nullptr == pSimpleBitArray, it will determine whether rad/write file_seek be called
		0,//LONGLONG Diff,						,//
		&sba,//CSimpleBitArray * pSimpleBitArray, ,//if nullptr, means reading from beginning until no more data
		diskSize,//LONGLONG BlockSize,
		TRUE,//BOOL SeekAlways, ,//<sonmi01>2014-12-12 ###???
		&spIFileCopy,//IFileCopy ** ppObj,
		TRUE//,//BOOL bCreateMemeber
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
		D_API_ERR(0, TEXT("CreateInstanceCMTCopy"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = spIFileCopy->Copy();
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Copy"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	spVHDX->FlushMetadata();
	spVHDX->FlushData();
	spVHDX.Release();
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}