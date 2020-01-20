#pragma once

////#include "stdafx.h"////

#include "..\PublicHeader\PublicDTraceLib.h"

#include "SingleVdsDiskInfo.h"
#include "SingleVdsVolumeInfo.h"

struct CAllVdsDiskVolumeInfo
{
	//******************************************
	vector<CSingleVdsDiskInfo*> m_pVdsDiskInfos;
	vector<CSingleVdsVolumeInfo*> m_pVdsVolumeInfos;

	//******************************************
	CAllVdsDiskVolumeInfo();
	~CAllVdsDiskVolumeInfo();
	VOID DestroyMember();

	//******************************************
	INT Retrieve();
	INT WriteVdsDiskToStorage(IStorage * pIStorage);
	INT ReadVdsDiskFromStorage(IStorage * pIStorage);

	INT WriteVdsVolumeToStorage(IStorage * pIStorage);
	INT ReadVdsVolumeFromStorage(IStorage * pIStorage);

	CONST CSingleVdsVolumeInfo* FindVolume(LPCTSTR pszVolGuid);
	CONST CSingleVdsDiskInfo* FindDisk( ULONG ulDiskNumber );

private: //****************************************** not support deep copy
	CAllVdsDiskVolumeInfo(CONST CAllVdsDiskVolumeInfo&);
	CAllVdsDiskVolumeInfo& operator=(CONST CAllVdsDiskVolumeInfo&);
};