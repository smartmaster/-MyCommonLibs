#pragma once

////#include "stdafx.h"////
#include "..\PublicHeader\PublicDTraceLib.h"

#include "VDSVolumeDiskHelper.h"

struct CSingleVdsDiskInfo
{
	MY_VDS_DISK_PROP m_MyVDSDiskProp;
	vector<VDS_DISK_EXTENT> m_VDSDiskExtents;

	INT WriteToStorage(IStorage * pIStorage);
	INT ReadFromStorage(IStorage * pIStorage);
};