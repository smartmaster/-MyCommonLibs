#pragma once

////#include "stdafx.h"////

#include "..\PublicHeader\PublicDTraceLib.h"

#include "VDSVolumeDiskHelper.h"

struct CSingleVdsVolumePlex
{
	VDS_VOLUME_PLEX_PROP m_VDSVolumePlexProp;
	vector<VDS_DISK_EXTENT> m_VDSDiskExtents;

	INT WriteToStorage(IStorage * pIStorage);
	INT ReadFromStorage(IStorage * pIStorage);
};


struct CSingleVdsVolumeInfo
{
	MY_VDS_VOLUME_PROP m_MyVDSVolumeProp;
	CString m_strVolumeGUID;
	vector<CSingleVdsVolumePlex> m_VolumePlexes;

	INT WriteToStorage(IStorage * pIStorage);
	INT ReadFromStorage(IStorage * pIStorage);

	static INT WriteVolumePlexesHelper(IStorage * pIStorage, vector<CSingleVdsVolumePlex> & VolumePlexes);
	static INT ReadVolumePlexesHelper(IStorage * pIStorage, vector<CSingleVdsVolumePlex> & VolumePlexes);
};