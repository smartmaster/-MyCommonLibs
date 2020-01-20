#pragma once

#include "CmdLine.h"
#include "..\CommonFunctions\MiscHelper.h"

#include "..\ServerDiskVolumeInfo\ServerDiskVolumeInfo_i.h"
#include "..\ServerFileDevice\ServerFileDevice_i.h"
#include "..\ServerVssLayer\ServerVssLayer_i.h"

#include "..\CommonFunctions\SingleVolumeInfo.h"
#include "..\CommonFunctions\SingleDiskInfo.h"
#include "..\CommonFunctions\AllVdsDiskVolumeInfo.h"
#include "..\CommonFunctions\MyStgHelper.h"

#include "..\ServerVssLayer\MyVssSnapshotProp.h"

#include "..\CommonFunctions\NTFS_Lib_Helper.h"
#include "..\CommonFunctions\SimpleBitArray.h"

static CONST TCHAR SZ_META_DATA_FOLDER[] = TEXT("MetaData");
static CONST TCHAR SZ_META_DATA_VOLUMES[] = TEXT("000-volumes.stg");
static CONST TCHAR SZ_META_DATA_DISKS[] = TEXT("001-disks.stg");
static CONST TCHAR SZ_META_DATA_VDS_VOLUMES[] = TEXT("002-vdsVolumes.stg");
static CONST TCHAR SZ_META_DATA_VDS_DISKS[] = TEXT("003-vdsDisks.stg");
static CONST TCHAR SZ_META_DATA_WM_XMLS[] = TEXT("004-vssWmXmls.stg");
static CONST TCHAR SZ_META_DATA_VSS_RESULTS[] = TEXT("005-vssResults.stg");
static CONST TCHAR SZ_META_DATA_SHADOW_VOLUME[] = TEXT("006-shadowVolume-%s.stg");
static CONST TCHAR SZ_META_DATA_VHD_LAYOUT[] = TEXT("007-vhdLayout-%s.stg");
static CONST TCHAR SZ_META_DATA_PHYSICAL_DISK_METADATA[] = TEXT("008-DiskMetadata-%s.VHD");


class CBackupFlowMain
{
private:
	CONST CCommandLine & m_CmdLine; //external object

	CAllVolumeInfo m_AllVolumeInfoRemote;
	CAllDiskInfo m_AllDiskInfoRemote;
	CAllVdsDiskVolumeInfo m_AllVdsDiskVolumeInfoRemote;
	vector<MY_VSS_SNAPSHOT_PROP> m_MyVssSnapshotPropsRemote;

	CComPtr<IDiskVolumeInfoObject> m_spDiskVolumeInfoObject;
	CComPtr<IVssLayerObject> m_spVssLayerObject;
	CComPtr<IFileDeviceObject> m_spFileDeviceObject;

	/**********************************************/
	vector<DWORD> m_ExistingLocalDisks;
	vector<CString> m_volumeGuidRemote;

private:
	INT CreateRemoteInterface();
	INT CreateLocalInterface();
	VOID GetMetaDataFolder(CString & strMetetaDataFolder);
	VOID GetShadowVolumeStgFileName(LPCTSTR pszVolume, CString & strShadowVolumeStg);
	VOID GetVhdLayoutStgFileName(LPCTSTR pszVolume, CString & strVhdLayoutStg);
	VOID GetVhdFileName(LPCTSTR pszVolume, CString & strVhd);
	VOID GetDiskMetaDataVhdFileName(PDRIVE_LAYOUT_INFORMATION_EX pDriveLayoutInformationEx, CString & strVhd);
	BOOL IsNewlyAddedDisk(DWORD dwDiskNumber);
	size_t GetSourceVolumesGuid();
	CONST CSingleVolumeInfo * FindVolumeInfo(LPCTSTR pszName);
	CONST CSingleDiskInfo * FindDiskInfo(ULONG ulDiskNumber);
	CONST CSingleVdsVolumeInfo* FindVdsVolumeInfo(LPCTSTR pszName);
	CONST CSingleVdsDiskInfo * FindVdsDiskInfo(ULONG ulDiskNumber);

	BOOL GetVdsVolumeFlag(LPCTSTR pszName, ULONG & ulFlags);
	BOOL IsPageFileVolume(LPCTSTR pszName);
	BOOL IsHibernationFileVolume(LPCTSTR pszName);

	static VOID GetVssStorgeFiles(IFileDeviceObject * pShadowVolume, vector<CString> & vssFiles);
	VOID GetLargeSystemFiles(LPCTSTR pszVolume, vector<CString> & Files);

	static VOID ClearNtfsFileRecordBitmap(CFileRecord * pFileRecord, CSimpleBitArray & SimpleBitArray, LONGLONG & excludedClusterCount);
	static VOID ClearExcludedFilesBitMap(
		IFileDeviceObject * pFileDeviceVolume, 
		CONST vector<CString> & excludedFiles,
		CSimpleBitArray & sba,
		LONGLONG & excludedClusterCount
		);
	static VOID ClearExcludedFilesBitMapByRefercenceNumber(
		IFileDeviceObject * pFileDeviceVolume, 
		CONST vector<LONGLONG> & excludedFileRefers,
		CSimpleBitArray & sba,
		LONGLONG & excludedClusterCount
		);

public:
	CBackupFlowMain(CONST CCommandLine & CmdLine);
	INT CreateMember();
	INT DestroyMember();
	virtual ~CBackupFlowMain();

	INT GetAllVolumeDiskInfo();

	INT BackupDiskMetaData();
	INT BackupSingleDiskMetaData(CSingleDiskInfo * pSingleDiskInfo);

	INT CreateVssSnapShot();
	INT VssSnapShotCleanup();

	INT BackupVolumes();
	INT BackupSingleVolume(CONST MY_VSS_SNAPSHOT_PROP & MyVssSnapshotProp);


	INT Main();
};