#include "stdafx.h"
#include "BackupFlowMain.h"
#include "..\CommonFunctions\VhdHelper.h"
#include "..\CommonFunctions\GPTDiskLayout.h"
#include "..\CommonFunctions\LocalFileDevice.h"
#include "..\CommonFunctions\SimpleBitArray.h"
#include "..\CommonFunctions\AsyncIoCopy.h"
#include "..\CommonFunctions\PerformanceTiming.h"
#include "..\CommonFunctions\SingleDiskInfo.h"
#include "..\CommonFunctions\NTFS_Lib_Helper.h"
#include "VhdAdapter.h"


CBackupFlowMain::CBackupFlowMain( CONST CCommandLine & CmdLine ) :
m_CmdLine(CmdLine)
{

}

INT CBackupFlowMain::CreateMember()
{
	INT nStatus = 0;
	DWORD dwLasterror = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		if (m_CmdLine.m_strMachine.GetLength())
		{
			nStatus = CreateRemoteInterface();
		}
		else
		{
			nStatus = CreateLocalInterface();
		}
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("Create Interface"), nStatus));
			break;
		}

		/**********************************************/
		CONST ULONG MAX_DISK_NUMBER = 256;
		CAllDiskInfo::GetAllDiskNumbers(MAX_DISK_NUMBER, m_ExistingLocalDisks);

		//******************************************
	} while (FALSE);

	return nStatus;
}

INT CBackupFlowMain::CreateRemoteInterface()
{
	INT nStatus = 0;
	DWORD dwLasterror = 0;
	HRESULT hr = S_OK;

	do 
	{
		hr = CoInitializeSecurity(
			NULL,//PSECURITY_DESCRIPTOR pVoid,
			-1,//LONG cAuthSvc,
			NULL,//SOLE_AUTHENTICATION_SERVICE * asAuthSvc, 
			NULL,//void * pReserved1,
			RPC_C_AUTHN_LEVEL_NONE,//DWORD dwAuthnLevel,
			RPC_C_IMP_LEVEL_IMPERSONATE,//DWORD dwImpLevel,
			NULL,//SOLE_AUTHENTICATION_LIST * pAuthList,
			EOAC_NONE,//DWORD dwCapabilities,
			NULL//void * pReserved3
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("CoInitializeSecurity"), hr));
			break;
		}

		//******************************************
		COAUTHIDENTITY AuthIdentity = {0};
		CRemoterInstanceHelper::InitializeCOAUTHIDENTITY(
			AuthIdentity, 
			const_cast<LPTSTR>(m_CmdLine.m_strUser.GetString()), 
			const_cast<LPTSTR>(m_CmdLine.m_strDomain.GetString()), 
			const_cast<LPTSTR>(m_CmdLine.m_strPassword.GetString())
			);
		COAUTHINFO AuthInfo = {0};
		CRemoterInstanceHelper::InitializeCOAUTHINFO(AuthInfo, &AuthIdentity);
		COSERVERINFO ServerInfo = {0};
		CRemoterInstanceHelper::InitializeCOSERVERINFO(ServerInfo, &AuthInfo, const_cast<LPTSTR>(m_CmdLine.m_strMachine.GetString()));

		//******************************************
		hr = CRemoterInstanceHelper::MyCoCreateInstance<IDiskVolumeInfoObject>(
			__uuidof(DiskVolumeInfoObject),//REFCLSID rclsid,
			NULL,//IUnknown * punkOuter,
			CLSCTX_ALL,//CLSCTX_ALL,//DWORD dwClsCtx,
			&ServerInfo,//LPTSTR pszServer,
			&m_spDiskVolumeInfoObject//QI ** ppQI
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("MyCreateInstance IDiskVolumeInfoObject"), hr));
			break;
		}


		//******************************************
		hr = CRemoterInstanceHelper::MyCoCreateInstance<IVssLayerObject>(
			__uuidof(VssLayerObject),//REFCLSID rclsid,
			NULL,//IUnknown * punkOuter,
			CLSCTX_ALL,//DWORD dwClsCtx,
			&ServerInfo,//LPTSTR pszServer,
			&m_spVssLayerObject//QI ** ppQI
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("MyCreateInstance IVssLayerObject"), hr));
			break;
		}

		//******************************************
		hr = CRemoterInstanceHelper::MyCoCreateInstance<IFileDeviceObject>(
			__uuidof(FileDeviceObject),//REFCLSID rclsid,
			NULL,//IUnknown * punkOuter,
			CLSCTX_ALL,//DWORD dwClsCtx,
			&ServerInfo,//LPTSTR pszServer,
			&m_spFileDeviceObject//QI ** ppQI
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("MyCreateInstance IFileDeviceObject"), hr));
			break;
		}


	} while (FALSE);

	return nStatus;
}

INT CBackupFlowMain::CreateLocalInterface()
{
	INT nStatus = 0;
	DWORD dwLasterror = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		hr = m_spDiskVolumeInfoObject.CoCreateInstance(__uuidof(DiskVolumeInfoObject));
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("MyCreateInstance IDiskVolumeInfoObject"), hr));
			break;
		}


		//******************************************
		hr = m_spVssLayerObject.CoCreateInstance(__uuidof(VssLayerObject));
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("MyCreateInstance IVssLayerObject"), hr));
			break;
		}

		//******************************************
		hr = m_spFileDeviceObject.CoCreateInstance(__uuidof(FileDeviceObject));
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("MyCreateInstance IFileDeviceObject"), hr));
			break;
		}


	} while (FALSE);

	return nStatus;
}

INT CBackupFlowMain::DestroyMember()
{
	m_AllVolumeInfoRemote.DestroyMember();
	m_AllDiskInfoRemote.DestroyMember();
	m_AllVdsDiskVolumeInfoRemote.DestroyMember();

	m_spDiskVolumeInfoObject.Release();
	m_spVssLayerObject.Release();
	m_spFileDeviceObject.Release();

	return 0;
}

CBackupFlowMain::~CBackupFlowMain()
{
	DestroyMember();
}

INT CBackupFlowMain::Main()
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		nStatus = CreateMember();
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateMember"), nStatus));
			break;
		}

		//******************************************
		nStatus =GetAllVolumeDiskInfo();
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("GetAllVolumeDiskInfo"), nStatus));
			break;
		}

		//******************************************
		nStatus = BackupDiskMetaData();
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("BackupDiskMetaData"), nStatus));
			break;
		}

		//******************************************
		nStatus = CreateVssSnapShot();
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateVssSnapShot"), nStatus));
			break;
		}

		//******************************************
		nStatus = BackupVolumes();
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("BackupVolumes"), nStatus));
			break;
		}


		//******************************************
	} while (FALSE);


	//******************************************
	nStatus = VssSnapShotCleanup();
	if (nStatus)
	{
		_OLD_DAPIERR((0, DRNPOS, TEXT("VssSnapShotCleanup"), nStatus));
	}


	return nStatus;
}

INT CBackupFlowMain::GetAllVolumeDiskInfo()
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		CString strMetaDetaFolder;
		GetMetaDataFolder(strMetaDetaFolder);
		BOOL bRet = ::CreateDirectory(
			strMetaDetaFolder,//__in_opt  LPCTSTR lpPathName,
			NULL//__in      LPSECURITY_ATTRIBUTES lpSecurityAttributes
			);
		if (!bRet)
		{
			dwLastError = GetLastError();
			if (ERROR_ALREADY_EXISTS != dwLastError)
			{
				_OLD_DSETLASTSTATUS(dwLastError);
				nStatus = dwLastError;
				_OLD_DAPIERR((0, DRNPOS, TEXT("CreateDirectory"), dwLastError, TEXT("[Folder=%s]"), strMetaDetaFolder.GetString()));
				break;
			}
		}


		//******************************************
		{
			CString strStgFile = strMetaDetaFolder;
			CMiscHelper::AppendTail(strStgFile, TEXT('\\'));
			strStgFile += SZ_META_DATA_DISKS;
			CComPtr<IStorage> spIStorage;
			DWORD grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE;
			hr = CMyStgHelper::CreateFileStorage(strStgFile, &spIStorage,  grfMode);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("CreateFileStorage"), hr));
				break;
			}

			hr = m_spDiskVolumeInfoObject->WriteDiskInfo(spIStorage);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteDiskInfo"), hr));
				break;
			}

			nStatus = m_AllDiskInfoRemote.ReadFromStorage(spIStorage);
			if (nStatus)
			{
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStorage"), nStatus));
				break;
			}
		}

		{
			CString strStgFile = strMetaDetaFolder;
			CMiscHelper::AppendTail(strStgFile, TEXT('\\'));
			strStgFile += SZ_META_DATA_VOLUMES;
			CComPtr<IStorage> spIStorage;
			DWORD grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE;
			hr = CMyStgHelper::CreateFileStorage(strStgFile, &spIStorage,  grfMode);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("CreateFileStorage"), hr));
				break;
			}

			hr = m_spDiskVolumeInfoObject->WriteVolumeInfo(spIStorage, FALSE);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteVolumeInfo"), hr));
				break;
			}

			nStatus = m_AllVolumeInfoRemote.ReadFromStorage(spIStorage);
			if (nStatus)
			{
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStorage"), nStatus));
				break;
			}

			/**********************************************/
			size_t nVolumeCount = GetSourceVolumesGuid();
			if (0 == nVolumeCount)
			{
				nStatus = E_INVALIDARG;
				_OLD_DAPIERR((0, DRNPOS, TEXT("GetSourceVolumesGuid"), nStatus, TEXT("[VolumeCount=%s]"), nVolumeCount));
				break;
			}
		}

		{
			CString strStgFileVolume = strMetaDetaFolder;
			CMiscHelper::AppendTail(strStgFileVolume, TEXT('\\'));
			strStgFileVolume += SZ_META_DATA_VDS_VOLUMES;
			CComPtr<IStorage> spIStorageVolume;
			DWORD grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE;
			hr = CMyStgHelper::CreateFileStorage(strStgFileVolume, &spIStorageVolume,  grfMode);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("CreateFileStorage"), hr));
				break;
			}

			CString strStgFileDisk = strMetaDetaFolder;
			CMiscHelper::AppendTail(strStgFileDisk, TEXT('\\'));
			strStgFileDisk += SZ_META_DATA_VDS_DISKS;
			CComPtr<IStorage> spIStorageDisk;
			hr = CMyStgHelper::CreateFileStorage(strStgFileDisk, &spIStorageDisk,  grfMode);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("CreateFileStorage"), hr));
				break;
			}

			hr = m_spDiskVolumeInfoObject->WriteVdsDiskVolumeInfo(spIStorageDisk, spIStorageVolume);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteVdsDiskVolumeInfo"), hr));
				break;
			}

			nStatus = m_AllVdsDiskVolumeInfoRemote.ReadVdsDiskFromStorage(spIStorageDisk);
			if (nStatus)
			{
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadVdsDiskFromStorage"), nStatus));
				break;
			}

			nStatus = m_AllVdsDiskVolumeInfoRemote.ReadVdsVolumeFromStorage(spIStorageVolume);
			if (nStatus)
			{
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadVdsVolumeFromStorage"), nStatus));
				break;
			}
		}

	} while (FALSE);

	return nStatus;
}

INT CBackupFlowMain::CreateVssSnapShot()
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		CComSafeArray<BSTR> spsa;
		for (size_t ii=0; ii<m_volumeGuidRemote.size(); ++ii)
		{
			CONST CString & strVolume = m_volumeGuidRemote[ii];
			spsa.Add(strVolume.AllocSysString(), FALSE);
		}

		//******************************************
		CString strMetaDetaFolder;
		GetMetaDataFolder(strMetaDetaFolder);

		CString strStgFileVssResults = strMetaDetaFolder;
		CMiscHelper::AppendTail(strStgFileVssResults, TEXT('\\'));
		strStgFileVssResults += SZ_META_DATA_VSS_RESULTS;
		CComPtr<IStorage> spIStorageVssResults;
		DWORD grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE;
		hr = CMyStgHelper::CreateFileStorage(strStgFileVssResults, &spIStorageVssResults,  grfMode);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateFileStorage"), hr));
			break;
		}

		CString strStgFileWmXmls = strMetaDetaFolder;
		CMiscHelper::AppendTail(strStgFileWmXmls, TEXT('\\'));
		strStgFileWmXmls += SZ_META_DATA_WM_XMLS;
		CComPtr<IStorage> spIStorageWmXmls;
		hr = CMyStgHelper::CreateFileStorage(strStgFileWmXmls, &spIStorageWmXmls,  grfMode);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateFileStorage"), hr));
			break;
		}

		hr = m_spVssLayerObject->ShadowCopy(
			spsa,//SAFEARRAY * psaArgvs, 
			spIStorageWmXmls,//IStorage * pISorageXmls, 
			spIStorageVssResults//IStorage * pISorageVssResults
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("ShadowCopy"), hr));
			break;
		}

		nStatus = MY_VSS_SNAPSHOT_PROP::ReadFromVectorRootStorage(spIStorageVssResults, m_MyVssSnapshotPropsRemote);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromVectorRootStorage"), nStatus));
			break;
		}


	} while (FALSE);

	return nStatus;
}

INT CBackupFlowMain::VssSnapShotCleanup()
{
	INT nStatus = 0;

	do 
	{
		if (NULL == (IVssLayerObject*)m_spVssLayerObject)
		{
			break;
		}
		
		HRESULT hr = m_spVssLayerObject->VssBackupComponentsRelease();
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("VssBackupComponentsRelease"), hr));
			//break;
		}

	} while (FALSE);
	
	return nStatus;
}

VOID CBackupFlowMain::GetMetaDataFolder( CString & strMetetaDataFolder )
{
	strMetetaDataFolder = m_CmdLine.m_strDestination;
	CMiscHelper::AppendTail(strMetetaDataFolder, TEXT('\\'));
	strMetetaDataFolder += SZ_META_DATA_FOLDER;
}

INT CBackupFlowMain::BackupVolumes()
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	for (size_t ii=0; ii<m_MyVssSnapshotPropsRemote.size(); ++ii)
	{
		nStatus = BackupSingleVolume(m_MyVssSnapshotPropsRemote[ii]);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("BackupSingleVolume"), nStatus, TEXT("[%s, %s]"), m_MyVssSnapshotPropsRemote[ii].m_pwszOriginalVolumeName, m_MyVssSnapshotPropsRemote[ii].m_pwszSnapshotDeviceObject));
			break;
		}
	}

	return nStatus;
}

static VOID AdjustVhdSizeHelper(LONGLONG llVolumeSize, LONGLONG & llVhdSize)
{
	CONST LONGLONG RESERVE_512M = ((LONGLONG)512)*1024*1024;
	CONST LONGLONG ALIGN_AT_1024M = ((LONGLONG)1024)*1024*1024;
	llVhdSize = AtlAlignUp(llVolumeSize + RESERVE_512M, ALIGN_AT_1024M);
}

INT CBackupFlowMain::BackupSingleVolume( CONST MY_VSS_SNAPSHOT_PROP & MyVssSnapshotProp )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	BOOL bIFileDeviceCreated = FALSE;

#if 0
	BOOL bAttached = FALSE;
	CVhdHelper vhdHelper;
	CLocalFileDevice LocalFileDeviceVhd;
#else
	CVHDAdapter vhdWriter;
#endif

	do 
	{
		//******************************************
		CString strShadowVolumeStg;
		GetShadowVolumeStgFileName(MyVssSnapshotProp.m_pwszOriginalVolumeName, strShadowVolumeStg);

		CComPtr<IStorage> spIStorageShadowVolume;
		DWORD grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE;
		hr = CMyStgHelper::CreateFileStorage(strShadowVolumeStg, &spIStorageShadowVolume, grfMode);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateFileStorage"), hr));
			break;
		}

		CComBSTR bstrShadowVolumeRemote = MyVssSnapshotProp.m_pwszSnapshotDeviceObject;
		hr = m_spDiskVolumeInfoObject->WriteSingleVolumeInfo(bstrShadowVolumeRemote, TRUE, spIStorageShadowVolume, FALSE);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("WriteSingleVolumeInfo"), hr));
			break;
		}

		CSingleVolumeInfo SingleShadowVolumeInfo;
		nStatus =  SingleShadowVolumeInfo.ReadFromStorage(spIStorageShadowVolume);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStorage"), nStatus));
			break;
		}
		spIStorageShadowVolume.Release();


		//******************************************
		LONGLONG llDiskSize = 0;
		AdjustVhdSizeHelper(SingleShadowVolumeInfo.m_GetLengthInformation.Length.QuadPart, llDiskSize);
		CString strVhdLocal;
		GetVhdFileName(MyVssSnapshotProp.m_pwszOriginalVolumeName, strVhdLocal );
#if 0
		nStatus = vhdHelper.Create(
			strVhdLocal,//LPCTSTR szPath, 
			VIRTUAL_DISK_ACCESS_ALL,//VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask, 
			llDiskSize,//ULONGLONG MaximumSize, 
			NULL//LPCTSTR ParentPath
			);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("CVirtualDiskHelper::Create"), nStatus));
			break;
		}

		//********************************************************************
		ATTACH_VIRTUAL_DISK_FLAG AttachFlags = ATTACH_VIRTUAL_DISK_FLAG_NO_DRIVE_LETTER /*| ATTACH_VIRTUAL_DISK_FLAG_PERMANENT_LIFETIME*/;
		nStatus = vhdHelper.Attach(AttachFlags);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("CVirtualDiskHelper::Attach"), nStatus));
			break;
		}
		bAttached = TRUE;

		//********************************************************************
		DWORD dwDiskNumber = vhdHelper.FindUninitalizedVHD();
		if ((DWORD)(-1) == dwDiskNumber || 0 == dwDiskNumber || !IsNewlyAddedDisk(dwDiskNumber))
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("FindUninitalizedVHD"), E_FAIL, TEXT("[DiskNumber=%u]"), dwDiskNumber));
			nStatus = E_FAIL;
			break;
		}
		_OLD_DINFO((0, DRNPOS, TEXT("[VHDPath, Size, DiskNumber]=[%s, %I64d, %u]"), strVhdLocal, llDiskSize, dwDiskNumber));

		//******************************************
		{
			CSingleDiskInfo SingleVhdDiskInfo;
			SingleVhdDiskInfo.Retrieve(dwDiskNumber, FALSE);

			CString strVhdLayoutStg;
			GetVhdLayoutStgFileName( MyVssSnapshotProp.m_pwszOriginalVolumeName, strVhdLayoutStg );
			
			CComPtr<IStorage> spIStorageVhdLayoutStg;
			DWORD grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE;
			INT nStatusCreateFileStorage = CMyStgHelper::CreateFileStorage(strVhdLayoutStg, &spIStorageVhdLayoutStg, grfMode);
			if (0 == nStatusCreateFileStorage)
			{
				nStatusCreateFileStorage = SingleVhdDiskInfo.WriteToStorage(spIStorageVhdLayoutStg);
			}
		}

		//******************************************
		CString strVhdNameLocal;
		strVhdNameLocal.Format(TEXT("\\\\?\\PhysicalDrive%u"), dwDiskNumber);
		CComBSTR bstrVhdNameLocal = strVhdNameLocal.GetString();
		hr = LocalFileDeviceVhd.Create(
			bstrVhdNameLocal,//BSTR bstrFileName, 
			GENERIC_WRITE,//ULONG ulDesiredAccess, 
			FILE_SHARE_READ|FILE_SHARE_WRITE,//ULONG ulShareMode, 
			OPEN_EXISTING,//ULONG ulCreationDisposition, 
			0//FILE_FLAG_NO_BUFFERING//ULONG ulFlagsAndAttributes
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT(" LocalFileDeviceVhd.Create"), hr, TEXT("[%s]"), strVhdNameLocal.GetString()));
			break;
		}
		IFileDeviceObject * pVHDWriter = &LocalFileDeviceVhd;
#else
		HaVhdUtility::IVHDFileW * pVHD= NULL;
		HaVhdUtility::ST_CREATE_PARMS CreateParms = {0};
		CreateParms.llDiskSize = llDiskSize;
		CreateParms.eType = VHD_Dynamic;
		CreateParms.dwCreateDisp = /*HaVhdUtility::VHD_DISP_*/CREATE_ALWAYS;
		nStatus = HaVhdUtility::CreateVHDFileW(
			strVhdLocal.GetString(),//const wchar_t*      pwszFilePath, 
			&CreateParms,//const ST_CREATE_PARMS*    pstCreateParms,
			&pVHD//IVHDFileW** ppVhdW
			);
		vhdWriter.Attach(pVHD);
		if (nStatus || NULL == pVHD)
		{
			hr = E_FAIL;
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateVHDFileW"), hr, TEXT("%s, statuc=%u, pvhd=0x%p"), strVhdLocal.GetString(), nStatus, pVHD));
			break;
		}
		pVHD = NULL;
		IFileDeviceObject * pVHDWriter = &vhdWriter;
#endif

		//******************************************
		CGptDiskPartition GptDiskPartition(llDiskSize, pVHDWriter);
		nStatus = GptDiskPartition.Partition(FALSE);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("GptDiskPartition.Partition"), nStatus));
			break;
		}

		GUID UniquePartitionGUID = {0};
		GUID DiskGUID = {0};
		LONGLONG llDiskStartLba = 0;
		LONGLONG llDiskEndLba = 0;
		LONGLONG llPartitionStartLba = 0;
		LONGLONG llPartitionEndLba = 0;
		GptDiskPartition.GetPartitionInfo(UniquePartitionGUID, DiskGUID, llDiskStartLba, llDiskEndLba, llPartitionStartLba, llPartitionEndLba);
		{
			CString strUniquePartitionGUID;
			CMiscHelper::StringFromGuid( strUniquePartitionGUID, UniquePartitionGUID );
			CString strDiskGUID;
			CMiscHelper::StringFromGuid( strDiskGUID, DiskGUID);
			_OLD_DINFO((0, DRNPOS, TEXT("[UniquePartitionGUID=%s, DiskGUID=%s, DiskStartLba=%I64d, DiskEndLba=%I64d, PartitionStartLba=%I64d, PartitionEndLba=%I64d]"), 
				strUniquePartitionGUID, strDiskGUID, llDiskStartLba, llDiskEndLba, llPartitionStartLba, llPartitionEndLba ));
		}

		//******************************************
		CComBSTR bstrRemoteSource = MyVssSnapshotProp.m_pwszSnapshotDeviceObject;
		hr = m_spFileDeviceObject->Create(
			bstrRemoteSource,//BSTR bstrFileName, 
			GENERIC_READ,//ULONG ulDesiredAccess, 
			FILE_SHARE_READ|FILE_SHARE_WRITE,//ULONG ulShareMode, 
			OPEN_EXISTING,//ULONG ulCreationDisposition, 
			0//ULONG ulFlagsAndAttributes
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("Create"), hr, TEXT("[%s]"), MyVssSnapshotProp.m_pwszSnapshotDeviceObject));
			break;
		}
		bIFileDeviceCreated= TRUE;

		//******************************************
		CSimpleBitArray sba(SingleShadowVolumeInfo.m_pVolumeBitmapBuffer->Buffer, SingleShadowVolumeInfo.m_pVolumeBitmapBuffer->BitmapSize.QuadPart);
		
		vector<CString> vecExcludedFiles;
		GetVssStorgeFiles(m_spFileDeviceObject, vecExcludedFiles);
		GetLargeSystemFiles(MyVssSnapshotProp.m_pwszOriginalVolumeName, vecExcludedFiles);

		LONGLONG excludedClusterCount = 0;
		ClearExcludedFilesBitMap( m_spFileDeviceObject, vecExcludedFiles, sba, excludedClusterCount);
		
		ULONG ulBlockSize = SingleShadowVolumeInfo.m_NtfsVolumeDataBuffer.BytesPerCluster;
		LONGLONG llFileSizeDebug = SingleShadowVolumeInfo.m_NtfsVolumeDataBuffer.TotalClusters.QuadPart - SingleShadowVolumeInfo.m_NtfsVolumeDataBuffer.FreeClusters.QuadPart - excludedClusterCount;
		LONGLONG llFileSize = sba.CountBit1();
		llFileSize *= ulBlockSize;

		//******************************************
		LONGLONG llOffsetDiff = llPartitionStartLba;
		llOffsetDiff *= GPT_LBA_BLOCK_SIZE;
		//m_spIFileDevice.SetFilePointer(0, FILE_BEGIN); //no need but more readable
		//LocalFileDeviceVhd.SetFilePointer(0, FILE_BEGIN);
		CAsyncIoCopy AsyncIoCopy(	
			m_spFileDeviceObject,//IFileDeviceObject * pSource, //external object
			pVHDWriter,//ILocalFileDevice * pDest, //external object
			&sba,//CSimpleBitArray * pSimpleBitArray, //external object
			llOffsetDiff,//LONGLONG llOffsetDiff,
			ulBlockSize,//ULONG ulBlockSize,
			llFileSize,//LONGLONG liTotalData
			0
			);

		nStatus = AsyncIoCopy.CreateMember();
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("AsyncIoCopy.CreateMember"), nStatus));
			break;
		}


		CStopTimer perfTiming(TRUE);
		perfTiming.Start();

		nStatus = AsyncIoCopy.Copy();
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("AsyncIoCopy.CreateMember"), nStatus));
			break;
		}

		perfTiming.End();
		LONGLONG llElapsedMilSec = perfTiming.Timing();
		LONGLONG llSpeed = llFileSize*1000/llElapsedMilSec;
		{
			CString strElapsedMilSec;
			CMiscHelper::NumberToString(llElapsedMilSec, strElapsedMilSec);

			CString strFileSize;
			CMiscHelper::NumberToString(llFileSize, strFileSize);

			CString strSpeed;
			CMiscHelper::NumberToString(llSpeed, strSpeed);

			_OLD_DINFO((0, DRNPOS, TEXT("[FileSize=%s Bytes, ElapsedMilSec=%s MilSec, Speed=%s Bytes/Sec]"), strFileSize, strElapsedMilSec, strSpeed));
		}

		//******************************************
	} while (FALSE);


	//******************************************
#if 0
	LocalFileDeviceVhd.Close();
	if (bAttached)
	{
		vhdHelper.Dettach();
		bAttached = FALSE;
	}
#else
	vhdWriter.Close();
#endif

	//******************************************
	if (bIFileDeviceCreated)
	{
		m_spFileDeviceObject->Close();
		bIFileDeviceCreated = FALSE;
	}

	return nStatus;
}

VOID CBackupFlowMain::GetShadowVolumeStgFileName( LPCTSTR pszVolume, CString & strShadowVolumeStg )
{
	GetMetaDataFolder(strShadowVolumeStg);
	CMiscHelper::AppendTail(strShadowVolumeStg, TEXT('\\'));

	CString strVolume = pszVolume;
	CMiscHelper::ReplaceInvalidFileNameChar(strVolume, TEXT('#'));

	strShadowVolumeStg.AppendFormat(SZ_META_DATA_SHADOW_VOLUME, strVolume);
}

VOID CBackupFlowMain::GetVhdFileName( LPCTSTR pszVolume, CString & strVhd )
{
	strVhd = m_CmdLine.m_strDestination;
	CMiscHelper::AppendTail(strVhd, TEXT('\\'));

	CString strVolume = pszVolume;
	strVolume.Trim(TEXT("\\/?."));

	strVhd.AppendFormat(TEXT("%s.VHD"), strVolume);
}

VOID CBackupFlowMain::GetVhdLayoutStgFileName( LPCTSTR pszVolume, CString & strVhdLayoutStg )
{
	GetMetaDataFolder(strVhdLayoutStg);
	CMiscHelper::AppendTail(strVhdLayoutStg, TEXT('\\'));

	CString strVolume = pszVolume;
	CMiscHelper::ReplaceInvalidFileNameChar(strVolume, TEXT('#'));

	strVhdLayoutStg.AppendFormat(SZ_META_DATA_VHD_LAYOUT, strVolume);
}

INT CBackupFlowMain::BackupDiskMetaData()
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	for (size_t ii=0; ii<m_AllDiskInfoRemote.m_pDiskInfos.size(); ++ii)
	{
		CSingleDiskInfo * pSingleDiskInfo = m_AllDiskInfoRemote.m_pDiskInfos[ii];
		nStatus = BackupSingleDiskMetaData(pSingleDiskInfo);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("BackupsingleDiskMetaData"), nStatus, TEXT("[DiskNumber=%u]"), pSingleDiskInfo->m_DiskNumber));
			break;
		}
	}

	return nStatus;
}

INT CBackupFlowMain::BackupSingleDiskMetaData( CSingleDiskInfo * pSingleDiskInfo )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	BOOL bIFileDeviceCreated = FALSE;

#if 0
	BOOL bAttached = FALSE;
	CVhdHelper vhdHelper;
	CLocalFileDevice LocalFileDeviceVhd;
#else
	CVHDAdapter vhdWriter;
#endif

	do 
	{
		map<LONGLONG, LONGLONG> DiskMetaDataRange;
		CInvertDataRange::GetDiskNonDataRange(
			pSingleDiskInfo->m_pDriveLayoutInformationEx, 
			pSingleDiskInfo->m_GetLengthInformation.Length.QuadPart, 
			DiskMetaDataRange
			);

		CONST LONGLONG LL_DISK_METADATA_LENGTH_LIMIT = 16*1024*1024;
		map<LONGLONG, LONGLONG> AdjustedDiskMetaDataRange;
		CInvertDataRange::AdjustDataRangeLengthLimit(DiskMetaDataRange, AdjustedDiskMetaDataRange, LL_DISK_METADATA_LENGTH_LIMIT);
		DiskMetaDataRange.clear();

		CString strPhysicalDriveRemote;
		strPhysicalDriveRemote.Format(TEXT("\\\\?\\PhysicalDrive%u"), pSingleDiskInfo->m_DiskNumber);
		CComBSTR bstrPhysicalDriveRemote = strPhysicalDriveRemote.GetString();
		hr = m_spFileDeviceObject->Create(
			bstrPhysicalDriveRemote,//BSTR bstrFileName, 
			GENERIC_READ,//ULONG ulDesiredAccess, 
			FILE_SHARE_READ|FILE_SHARE_WRITE,//ULONG ulShareMode, 
			OPEN_EXISTING,//ULONG ulCreationDisposition, 
			0//FILE_FLAG_NO_BUFFERING//ULONG ulFlagsAndAttributes
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT(" m_spIFileDevice->Create"), hr, TEXT("[%s]"), strPhysicalDriveRemote.GetString()));
			break;
		}
		bIFileDeviceCreated= TRUE;

		//******************************************
		CString strVhdLocal;
		GetDiskMetaDataVhdFileName(pSingleDiskInfo->m_pDriveLayoutInformationEx, strVhdLocal);
#if 0
		nStatus = vhdHelper.Create(
			strVhdLocal,//LPCTSTR szPath, 
			VIRTUAL_DISK_ACCESS_ALL,//VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask, 
			pSingleDiskInfo->m_GetLengthInformation.Length.QuadPart,//ULONGLONG MaximumSize, 
			NULL//LPCTSTR ParentPath
			);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("CVirtualDiskHelper::Create"), nStatus));
			break;
		}
#else 
		HaVhdUtility::IVHDFileW * pVHD= NULL;
		HaVhdUtility::ST_CREATE_PARMS CreateParms = {0};
		CreateParms.llDiskSize = pSingleDiskInfo->m_GetLengthInformation.Length.QuadPart;
		CreateParms.eType = VHD_Dynamic;
		CreateParms.dwCreateDisp = /*HaVhdUtility::VHD_DISP_*/CREATE_ALWAYS;
		nStatus = HaVhdUtility::CreateVHDFileW(
			strVhdLocal.GetString(),//const wchar_t*      pwszFilePath, 
			&CreateParms,//const ST_CREATE_PARMS*    pstCreateParms,
			&pVHD//IVHDFileW** ppVhdW
			);
		vhdWriter.Attach(pVHD);
		if (nStatus || NULL == pVHD)
		{
			hr = E_FAIL;
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateVHDFileW"), hr, TEXT("%s, statuc=%u, pvhd=0x%p"), strVhdLocal.GetString(), nStatus, pVHD));
			break;
		}
		pVHD = NULL;
#endif

#if 0
		//********************************************************************
		ATTACH_VIRTUAL_DISK_FLAG AttachFlags = ATTACH_VIRTUAL_DISK_FLAG_NO_DRIVE_LETTER /*| ATTACH_VIRTUAL_DISK_FLAG_PERMANENT_LIFETIME*/;
		nStatus = vhdHelper.Attach(AttachFlags);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("CVirtualDiskHelper::Attach"), nStatus));
			break;
		}
		bAttached = TRUE;

		//******************************************
		DWORD dwDiskNumber = vhdHelper.FindUninitalizedVHD();
		if ((DWORD)(-1) == dwDiskNumber || 0 == dwDiskNumber || !IsNewlyAddedDisk(dwDiskNumber))
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("FindUninitalizedVHD"), E_FAIL, TEXT("[DiskNumber=%u]"), dwDiskNumber));
			nStatus = E_FAIL;
			break;
		}
		_OLD_DINFO((0, DRNPOS, TEXT("[VHDPath, Size, DiskNumber]=[%s, %I64d, %u]"), strVhdLocal, pSingleDiskInfo->m_GetLengthInformation.Length.QuadPart, dwDiskNumber));

		//******************************************
		CString strVhdNameLocal;
		strVhdNameLocal.Format(TEXT("\\\\?\\PhysicalDrive%u"), dwDiskNumber);
		CComBSTR bstrVhdNameLocal = strVhdNameLocal.GetString();
		hr = LocalFileDeviceVhd.Create(
			bstrVhdNameLocal,//BSTR bstrFileName, 
			GENERIC_WRITE,//ULONG ulDesiredAccess, 
			FILE_SHARE_READ|FILE_SHARE_WRITE,//ULONG ulShareMode, 
			OPEN_EXISTING,//ULONG ulCreationDisposition, 
			0//FILE_FLAG_NO_BUFFERING//ULONG ulFlagsAndAttributes
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT(" LocalFileDeviceVhd.Create"), hr, TEXT("[%s]"), strVhdNameLocal.GetString()));
			break;
		}
		IFileDeviceObject * pVHDWriter = &LocalFileDeviceVhd;
#else
		IFileDeviceObject * pVHDWriter = &vhdWriter;
#endif

		//******************************************
		CSyncIoCopy SyncIoCopy(m_spFileDeviceObject, pVHDWriter, AdjustedDiskMetaDataRange, 0);
		nStatus = SyncIoCopy.Copy();
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("SyncIoCopyData"), nStatus));
			break;
		}

		//******************************************
	} while (FALSE);

	//******************************************
#if 0
	LocalFileDeviceVhd.Close();
	if (bAttached)
	{
		vhdHelper.Dettach();
		bAttached = FALSE;
	}
#else
	vhdWriter.Close();
#endif

	//******************************************
	if (bIFileDeviceCreated)
	{
		m_spFileDeviceObject->Close();
		bIFileDeviceCreated = FALSE;
	}

	return nStatus;
}

VOID CBackupFlowMain::GetDiskMetaDataVhdFileName( PDRIVE_LAYOUT_INFORMATION_EX pDriveLayoutInformationEx, CString & strVhd )
{
	GetMetaDataFolder(strVhd);
	CMiscHelper::AppendTail(strVhd, TEXT('\\'));

	CString strDisk;
	if (PARTITION_STYLE_MBR == pDriveLayoutInformationEx->PartitionStyle)
	{
		strDisk.Format(TEXT("PhysicalDisk%08X"), pDriveLayoutInformationEx->Mbr.Signature);
	}
	else
	{
		CString strDiskGuid;
		CMiscHelper::StringFromGuid(strDiskGuid, pDriveLayoutInformationEx->Gpt.DiskId);
		strDisk = TEXT("PhysicalDisk");
		strDisk += strDiskGuid;
	}

	strVhd.AppendFormat(SZ_META_DATA_PHYSICAL_DISK_METADATA, strDisk);
}

BOOL CBackupFlowMain::IsNewlyAddedDisk( DWORD dwDiskNumber )
{
	BOOL bRet = TRUE;
	for (size_t ii=0; ii<m_ExistingLocalDisks.size(); ++ii)
	{
		if (dwDiskNumber == m_ExistingLocalDisks[ii])
		{
			bRet = FALSE;
			break;
		}
	}
	return bRet;
}

CONST CSingleVolumeInfo * CBackupFlowMain::FindVolumeInfo(LPCTSTR pszName)
{
	return m_AllVolumeInfoRemote.FindVolume(pszName);

#if 0
	CSingleVolumeInfo * pRetVolInfo = NULL;;
	for (size_t ii=0; ii<m_AllVolumeInfoRemote.m_pVolumeInfos.size(); ++ii)
	{
		CSingleVolumeInfo * pSingleVolumeInfo = m_AllVolumeInfoRemote.m_pVolumeInfos[ii];
		if (CMiscHelper::IsVolumeGuidMatch(pszName, pSingleVolumeInfo->m_strVolumeGUID))
		{
			pRetVolInfo = pSingleVolumeInfo;
			break;
		}
		else
		{
			CString strName = pszName;
			CMiscHelper::AppendTail(strName, TEXT('\\'));
			for (size_t jj=0; jj<pSingleVolumeInfo->m_Paths.size(); ++jj)
			{
				if (0 == strName.CompareNoCase(pSingleVolumeInfo->m_Paths[jj]))
				{
					pRetVolInfo = pSingleVolumeInfo;
					break;
				}
			}
			if (pRetVolInfo)
			{
				break;
			}
		}
	}

	return pRetVolInfo;
#endif
}

CONST CSingleDiskInfo * CBackupFlowMain::FindDiskInfo( ULONG ulDiskNumber )
{
	return m_AllDiskInfoRemote.FindDisk(ulDiskNumber);

#if 0
	CSingleDiskInfo * pRetDiskInfo = NULL;
	for (size_t ii=0; ii<m_AllDiskInfoRemote.m_pDiskInfos.size(); ++ii)
	{
		if (ulDiskNumber == m_AllDiskInfoRemote.m_pDiskInfos[ii]->m_DiskNumber)
		{
			pRetDiskInfo = m_AllDiskInfoRemote.m_pDiskInfos[ii];
			break;
		}
	}

	return pRetDiskInfo;
#endif
}

size_t CBackupFlowMain::GetSourceVolumesGuid()
{
	for (size_t ii=0; ii<m_CmdLine.m_strVolumes.size(); ++ii)
	{
		CONST CSingleVolumeInfo * pSingleVolumeInfo = FindVolumeInfo(m_CmdLine.m_strVolumes[ii]);
		if (pSingleVolumeInfo)
		{
			m_volumeGuidRemote.push_back(pSingleVolumeInfo->m_strVolumeGUID);
		}
		else
		{
			_OLD_DINFO((0, DRNPOS, TEXT("%s not found"), m_CmdLine.m_strVolumes[ii].GetString()));
		}
	}
	return m_volumeGuidRemote.size();
}

VOID CBackupFlowMain::GetVssStorgeFiles( IFileDeviceObject * pShadowVolume, vector<CString> & vssFiles )
{
	LONGLONG llNewFilePointer = 0;
	HRESULT hr = pShadowVolume-> SetPointerEx( 
		0,///* [in] */ LONGLONG llDistanceToMove,
		&llNewFilePointer,///* [out] */ LONGLONG *lpNewFilePointer,
		FILE_BEGIN///* [in] */ ULONG ulMoveMethod
		);
	if (FAILED(hr))
	{
		return;
	}

	CONST TCHAR SZ_PATH[] = TEXT("\\System Volume Information");
	CONST TCHAR SZ_PATTERN[] = TEXT("*") TEXT("{") TEXT("????????") TEXT("-") TEXT("????") TEXT("-") TEXT("????") TEXT("-") TEXT("????") TEXT("-") TEXT("????????????") TEXT("}");
	CNtfsParserHelper::EnumFindFiles(pShadowVolume, SZ_PATH, SZ_PATTERN, NULL, &vssFiles, NULL);

	for (size_t ii=0; ii<vssFiles.size(); ++ii)
	{
		vssFiles[ii].Insert(0, SZ_PATH);
	}
}

CONST CSingleVdsVolumeInfo* CBackupFlowMain::FindVdsVolumeInfo( LPCTSTR pszName )
{
	return m_AllVdsDiskVolumeInfoRemote.FindVolume(pszName);

#if 0
	CONST CSingleVdsVolumeInfo * pRetVdsVolInfo = NULL;;
	for (size_t ii=0; ii<m_AllVdsDiskVolumeInfoRemote.m_pVdsVolumeInfos.size(); ++ii)
	{
		CONST CSingleVdsVolumeInfo * pSingleVdsVolumeInfo = m_AllVdsDiskVolumeInfoRemote.m_pVdsVolumeInfos[ii];
		if (CMiscHelper::IsVolumeGuidMatch(pszName, pSingleVdsVolumeInfo->m_strVolumeGUID))
		{
			pRetVdsVolInfo = pSingleVdsVolumeInfo;
			break;
		}
	}

	return pRetVdsVolInfo;
#endif
}

BOOL CBackupFlowMain::GetVdsVolumeFlag( LPCTSTR pszName, ULONG & ulFlags )
{
	BOOL bRet = FALSE;
	ulFlags = -1;
	CONST CSingleVdsVolumeInfo*  pInfo = FindVdsVolumeInfo(pszName);
	if (pInfo)
	{
		ulFlags = pInfo->m_MyVDSVolumeProp.m_ulFlags;
		bRet = TRUE;
	}

	return bRet;
}

BOOL CBackupFlowMain::IsPageFileVolume( LPCTSTR pszName )
{
	ULONG ulFlags = -1;
	BOOL bRet =  GetVdsVolumeFlag(pszName, ulFlags);
	if (bRet)
	{
		bRet = (VDS_VF_PAGEFILE == (ulFlags & VDS_VF_PAGEFILE));
	}

	return bRet;
}

BOOL CBackupFlowMain::IsHibernationFileVolume( LPCTSTR pszName )
{
	ULONG ulFlags = -1;
	BOOL bRet =  GetVdsVolumeFlag(pszName, ulFlags);
	if (bRet)
	{
		bRet = (VDS_VF_HIBERNATION == (ulFlags & VDS_VF_HIBERNATION));
	}

	return bRet;
}

VOID CBackupFlowMain::GetLargeSystemFiles( LPCTSTR pszVolume, vector<CString> & Files )
{
	if (IsPageFileVolume(pszVolume))
	{
		CONST TCHAR SZ_PAGE_FILE[] = TEXT("\\pagefile.sys");
		Files.push_back(SZ_PAGE_FILE);
	}

	if (IsHibernationFileVolume(pszVolume))
	{
		CONST TCHAR SZ_HIBER_FILE[] = TEXT("\\hiberfil.sys");
		Files.push_back(SZ_HIBER_FILE);
	}
}

VOID CBackupFlowMain::ClearNtfsFileRecordBitmap(CFileRecord * pFileRecord, CSimpleBitArray & SimpleBitArray, LONGLONG & excludedClusterCount)
{
	CONST CAttrBase * pAttrData = pFileRecord->FindFirstAttr(ATTR_TYPE_DATA);
	while (pAttrData)
	{
		if (pAttrData->IsNonResident() && pAttrData->IsDataRunOK())
		{
			CONST CAttrNonResident * pnr = static_cast<CONST CAttrNonResident *>(pAttrData);
			CONST CDataRunList & drlst = pnr->GetDataRunList();

			CONST DataRun_Entry *dr = drlst.FindFirstEntry();
			while (dr)
			{
				if (dr->m_LCN > 0 && dr->m_Clusters > 0)
				{
					SimpleBitArray.ClearRange(dr->m_LCN, dr->m_LCN + dr->m_Clusters);
					excludedClusterCount += dr->m_Clusters;
				}

				dr = drlst.FindNextEntry();
			}
		}

		pAttrData = pFileRecord->FindNextAttr(ATTR_TYPE_DATA);
	}
}

VOID CBackupFlowMain::ClearExcludedFilesBitMap( IFileDeviceObject * pFileDeviceVolume, CONST vector<CString> & excludedFiles, CSimpleBitArray & sba, LONGLONG & excludedClusterCount)
{
	LONGLONG llNewFilePointer = 0;
	HRESULT hr = pFileDeviceVolume->SetPointerEx( 
		0,///* [in] */ LONGLONG llDistanceToMove,
		&llNewFilePointer,///* [out] */ LONGLONG *lpNewFilePointer,
		FILE_BEGIN///* [in] */ ULONG ulMoveMethod
		);
	if (FAILED(hr))
	{
		return;
	}

	CAutoPtr<CNTFSVolume> apNTFSVolume;
	INT nStatus = CNtfsParserHelper::InitVolume(pFileDeviceVolume, &apNTFSVolume.m_p);
	if (nStatus || NULL == apNTFSVolume.m_p)
	{
		return;
	}

	for (size_t ii=0; ii<excludedFiles.size(); ++ii)
	{
		CAutoPtr<CFileRecord> apFileRecord;
		nStatus = CNtfsParserHelper::FindAndParseFileRecord(apNTFSVolume, excludedFiles[ii], &apFileRecord.m_p);
		if (nStatus || NULL == apFileRecord.m_p)
		{
			continue;
		}

		ClearNtfsFileRecordBitmap(apFileRecord, sba, excludedClusterCount);
	}
}

VOID CBackupFlowMain::ClearExcludedFilesBitMapByRefercenceNumber( IFileDeviceObject * pFileDeviceVolume, CONST vector<LONGLONG> & excludedFileRefers, CSimpleBitArray & sba, LONGLONG & excludedClusterCount )
{
	LONGLONG llNewFilePointer = 0;
	HRESULT hr = pFileDeviceVolume->SetPointerEx( 
		0,///* [in] */ LONGLONG llDistanceToMove,
		&llNewFilePointer,///* [out] */ LONGLONG *lpNewFilePointer,
		FILE_BEGIN///* [in] */ ULONG ulMoveMethod
		);
	if (FAILED(hr))
	{
		return;
	}

	CAutoPtr<CNTFSVolume> apNTFSVolume;
	INT nStatus = CNtfsParserHelper::InitVolume(pFileDeviceVolume, &apNTFSVolume.m_p);
	if (nStatus || NULL == apNTFSVolume.m_p)
	{
		return;
	}

	for (size_t ii=0; ii<excludedFileRefers.size(); ++ii)
	{
		CAutoPtr<CFileRecord> apFileRecord;
		nStatus = CNtfsParserHelper::ParseFileRecordByRefecenceNumber(apNTFSVolume, excludedFileRefers[ii], &apFileRecord.m_p);
		if (nStatus || NULL == apFileRecord.m_p)
		{
			continue;
		}

		ClearNtfsFileRecordBitmap(apFileRecord, sba, excludedClusterCount);
	}
}

CONST CSingleVdsDiskInfo * CBackupFlowMain::FindVdsDiskInfo( ULONG ulDiskNumber )
{
	return m_AllVdsDiskVolumeInfoRemote.FindDisk(ulDiskNumber);

#if 0
	CONST CSingleVdsDiskInfo * pRetVdsDiskInfo = NULL;;
	for (size_t ii=0; ii<m_AllVdsDiskVolumeInfoRemote.m_pVdsDiskInfos.size(); ++ii)
	{
		CONST CSingleVdsDiskInfo * pSingleVdsDiskInfo = m_AllVdsDiskVolumeInfoRemote.m_pVdsDiskInfos[ii];
		if (CMiscHelper::IsDiskNumberMatch(ulDiskNumber, pSingleVdsDiskInfo->m_MyVDSDiskProp.m_strName))
		{
			pRetVdsDiskInfo = pSingleVdsDiskInfo;
			break;
		}
	}

	return pRetVdsDiskInfo;
#endif
}