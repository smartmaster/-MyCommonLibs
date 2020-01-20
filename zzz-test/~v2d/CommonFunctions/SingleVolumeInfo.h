#pragma once 

#include "stdafx.h"
#include "VolumeHelper.h"

//******************************************
struct CGetVolumeInformation
{
	TCHAR m_VolumeNameBuffer[128];
	DWORD m_VolumeSerialNumber;
	DWORD m_MaximumComponentLength;
	DWORD m_FileSystemFlags;
	TCHAR m_FileSystemNameBuffer[64];
};

struct CSingleVolumeInfo
{
	//******************************************
	CString m_strVolumeGUID;
	vector<CString> m_Paths;
	vector<CString> m_MountPoint;
	CGetVolumeInformation m_GetVolumeInformation;
	NTFS_VOLUME_DATA_BUFFER m_NtfsVolumeDataBuffer;
	VOLUME_DISK_EXTENTS * m_pVolumeDiskExtents;
	VOLUME_BITMAP_BUFFER * m_pVolumeBitmapBuffer;
	GET_LENGTH_INFORMATION m_GetLengthInformation;

	//******************************************
	CSingleVolumeInfo();
	~CSingleVolumeInfo();

	INT WriteToStorage(IStorage * pIStorage);
	INT ReadFromStorage(IStorage * pIStorage);
	INT Retrieve(LPCTSTR szVolumeGUID, BOOL bGetVolumeBitmap, BOOL bBreakOnError);

private: //******************************************
	CSingleVolumeInfo(CONST CSingleVolumeInfo&);
	CSingleVolumeInfo& operator=(CONST CSingleVolumeInfo&);
};

struct CAllVolumeInfo
{
	//******************************************
	vector<CSingleVolumeInfo*> m_pVolumeInfos;

	//******************************************
	CAllVolumeInfo();
	~CAllVolumeInfo();
	VOID DestroyMember();

	//******************************************
	INT Retrieve(BOOL bGetVolumeBitmap);
	INT WriteToStorage(IStorage * pIStorage);
	INT ReadFromStorage(IStorage * pIStorage);

	CONST CSingleVolumeInfo* FindVolumeByGuid(LPCTSTR pszVolGuid);
	CONST CSingleVolumeInfo * FindVolume(LPCTSTR pszName);

private: //****************************************** not support deep copy
	CAllVolumeInfo(CONST CAllVolumeInfo&);
	CAllVolumeInfo& operator=(CONST CAllVolumeInfo&);
};