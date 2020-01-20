#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include "SingleAndAllVolumeInfoXml.h"
#include "SingleAndAllDiskInfoXml.h"
#include "AllVdsDiskVolumeInfoXml.h"



struct CAllVolumeDiskInfoXml
{
	SERIALIZE_BEGIN_STRUCT(CAllVolumeDiskInfoXml, CAllVolumeDiskInfoXml);
	CAllVolumeInfoXml			AllVolumeInfoXml;			SERIALIZE_COMPLEX(AllVolumeInfoXml);
	CAllDiskInfoXml				AllDiskInfoXml;				SERIALIZE_COMPLEX(AllDiskInfoXml);
	CAllVdsDiskVolumeInfoXml	AllVdsDiskVolumeInfoXml;	SERIALIZE_COMPLEX(AllVdsDiskVolumeInfoXml);
	SERIALIZE_END_STRUCT(CAllVolumeDiskInfoXml, CAllVolumeDiskInfoXml);
	STRUCT_XML_SERIALIZER;
};


HRESULT GetAllVolumeDiskInfoXml(CAllVolumeDiskInfoXml & AllVolumeDiskInfoXml, BOOL /*bGetVolumeBitmap*/);