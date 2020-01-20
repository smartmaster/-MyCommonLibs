#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


//////////////////////////////////////////////////////////////////////////
#include "..\PublicHeader\PublicDTraceLib.h"


#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"

#include "..\PublicHeader\PublicSimpleXml.h"
#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\PublicHeader\PublicValueXmlElementConvert.h"
#include "..\PublicHeader\PublicParameterStreamDef.h"
//////////////////////////////////////////////////////////////////////////


#include "SingleVdsVolumeInfo.h"


//id
//type
//status
//health
//TransitionState
//ullSize
//ulStripeSize
//ulNumberOfMembers


struct VDS_VOLUME_PLEX_PROP_Xml
{
	SERIALIZE_BEGIN_STRUCT(VDS_VOLUME_PLEX_PROP_Xml, VDS_VOLUME_PLEX_PROP_Xml);
	/*VDS_OBJECT_ID*/ GUID id;							SERIALIZE_BASIC(id);
	/*VDS_VOLUME_PLEX_TYPE*/ INT type;					SERIALIZE_BASIC(type);
	/*VDS_VOLUME_PLEX_STATUS */ INT status;				SERIALIZE_BASIC(status);
	/*VDS_HEALTH */ INT health;							SERIALIZE_BASIC(health);
	/*VDS_TRANSITION_STATE*/ INT TransitionState;		SERIALIZE_BASIC(TransitionState);
	ULONGLONG ullSize;									SERIALIZE_BASIC(ullSize);
	ULONG ulStripeSize;									SERIALIZE_BASIC(ulStripeSize);
	ULONG ulNumberOfMembers;							SERIALIZE_BASIC(ulNumberOfMembers);
	SERIALIZE_END_STRUCT(VDS_VOLUME_PLEX_PROP_Xml, VDS_VOLUME_PLEX_PROP_Xml);
	STRUCT_XML_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	VDS_VOLUME_PLEX_PROP_Xml()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST VDS_VOLUME_PLEX_PROP & src, VDS_VOLUME_PLEX_PROP_Xml & target)
	{
		target.id = src.id;
		target.type = src.type;
		target.status = src.status;
		target.health = src.health;
		target.TransitionState = src.TransitionState;
		target.ullSize = src.ullSize;
		target.ulStripeSize = src.ulStripeSize;
		target.ulNumberOfMembers = src.ulNumberOfMembers;
	}

	static void Xml2Bin(CONST VDS_VOLUME_PLEX_PROP_Xml & src, VDS_VOLUME_PLEX_PROP & target)
	{
		target.id = src.id;
		target.type = (VDS_VOLUME_PLEX_TYPE)src.type;
		target.status = (VDS_VOLUME_PLEX_STATUS)src.status;
		target.health = (VDS_HEALTH)src.health;
		target.TransitionState = (VDS_TRANSITION_STATE)src.TransitionState;
		target.ullSize = src.ullSize;
		target.ulStripeSize = src.ulStripeSize;
		target.ulNumberOfMembers = src.ulNumberOfMembers;
	}
}



//diskId
//type
//ullOffset
//ullSize
//volumeId
//plexId
//memberIdx



struct VDS_DISK_EXTENT_Xml
{
	SERIALIZE_BEGIN_STRUCT(VDS_DISK_EXTENT_Xml, VDS_DISK_EXTENT_Xml);
	/*VDS_OBJECT_ID*/ GUID diskId;				SERIALIZE_BASIC(diskId);
	/*VDS_DISK_EXTENT_TYPE*/ INT type;			SERIALIZE_BASIC(type);
	ULONGLONG ullOffset;						SERIALIZE_BASIC(ullOffset);
	ULONGLONG ullSize;							SERIALIZE_BASIC(ullSize);
	/*VDS_OBJECT_ID*/ GUID volumeId;			SERIALIZE_BASIC(volumeId);
	/*VDS_OBJECT_ID*/ GUID plexId;				SERIALIZE_BASIC(plexId);
	ULONG memberIdx;							SERIALIZE_BASIC(memberIdx);
	SERIALIZE_END_STRUCT(VDS_DISK_EXTENT_Xml, VDS_DISK_EXTENT_Xml);
	STRUCT_XML_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	VDS_DISK_EXTENT_Xml()
	{
		ZeroMemory(this, sizeof(*this));
	}
} ;


namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST VDS_DISK_EXTENT & src, VDS_DISK_EXTENT_Xml & target)
	{
		target.diskId = src.diskId;
		target.type = src.type;
		target.ullOffset = src.ullOffset;
		target.ullSize = src.ullSize;
		target.volumeId = src.volumeId;
		target.plexId = src.plexId;
		target.memberIdx = src.memberIdx;
	}

	static void Xml2Bin(CONST VDS_DISK_EXTENT_Xml & src, VDS_DISK_EXTENT & target)
	{
		target.diskId = src.diskId;
		target.type = (VDS_DISK_EXTENT_TYPE)src.type;
		target.ullOffset = src.ullOffset;
		target.ullSize = src.ullSize;
		target.volumeId = src.volumeId;
		target.plexId = src.plexId;
		target.memberIdx = src.memberIdx;
	}
}


//m_VDSVolumePlexProp
//m_VDSDiskExtents


struct CSingleVdsVolumePlexXml
{
	SERIALIZE_BEGIN_STRUCT(CSingleVdsVolumePlexXml, CSingleVdsVolumePlexXml);
	VDS_VOLUME_PLEX_PROP_Xml m_VDSVolumePlexProp;	SERIALIZE_COMPLEX(m_VDSVolumePlexProp);
	vector<VDS_DISK_EXTENT_Xml> m_VDSDiskExtents;	SERIALIZE_COMPLEX_VECTOR(m_VDSDiskExtents);
	SERIALIZE_END_STRUCT(CSingleVdsVolumePlexXml, CSingleVdsVolumePlexXml);
	STRUCT_XML_SERIALIZER;


	CSingleVdsVolumePlexXml()
	{

	}
};


namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST CSingleVdsVolumePlex & src, CSingleVdsVolumePlexXml & target)
	{
		Bin2Xml(src.m_VDSVolumePlexProp, target.m_VDSVolumePlexProp);
		target.m_VDSDiskExtents.resize(src.m_VDSDiskExtents.size());
		for (size_t ii = 0; ii < target.m_VDSDiskExtents.size(); ++ ii)
		{
			Bin2Xml(src.m_VDSDiskExtents[ii], target.m_VDSDiskExtents[ii]);
		}
	}

	static void Xml2Bin(CONST CSingleVdsVolumePlexXml & src, CSingleVdsVolumePlex & target)
	{
		Xml2Bin(src.m_VDSVolumePlexProp, target.m_VDSVolumePlexProp);
		target.m_VDSDiskExtents.resize(src.m_VDSDiskExtents.size());
		for (size_t ii = 0; ii < target.m_VDSDiskExtents.size(); ++ii)
		{
			Xml2Bin(src.m_VDSDiskExtents[ii], target.m_VDSDiskExtents[ii]);
		}
	}
}


//m_id
//m_type
//m_status
//m_health
//m_TransitionState
//m_ullSize
//m_ulFlags
//m_RecommendedFileSystemType
//m_strName




struct MY_VDS_VOLUME_PROP_Xml
{
	SERIALIZE_BEGIN_STRUCT(MY_VDS_VOLUME_PROP_Xml, MY_VDS_VOLUME_PROP_Xml);
	/*VDS_OBJECT_ID*/ GUID m_id;									SERIALIZE_BASIC(m_id);
	/*VDS_VOLUME_TYPE*/ INT m_type;									SERIALIZE_BASIC(m_type);
	/*VDS_VOLUME_STATUS*/ INT  m_status;							SERIALIZE_BASIC(m_status);
	/*VDS_HEALTH*/ INT  m_health;									SERIALIZE_BASIC(m_health);
	/*VDS_TRANSITION_STATE*/ INT  m_TransitionState;				SERIALIZE_BASIC(m_TransitionState);
	ULONGLONG m_ullSize;											SERIALIZE_BASIC(m_ullSize);
	ULONG m_ulFlags;												SERIALIZE_BASIC(m_ulFlags);
	/*VDS_FILE_SYSTEM_TYPE*/ INT m_RecommendedFileSystemType;		SERIALIZE_BASIC(m_RecommendedFileSystemType);
	CString m_strName;												SERIALIZE_BASIC(m_strName);
	SERIALIZE_END_STRUCT(MY_VDS_VOLUME_PROP_Xml, MY_VDS_VOLUME_PROP_Xml);
	STRUCT_XML_SERIALIZER;


	MY_VDS_VOLUME_PROP_Xml()
	{
		ZeroMemory(this, FIELD_OFFSET(MY_VDS_VOLUME_PROP_Xml, m_strName));
	}
};


namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST MY_VDS_VOLUME_PROP & src, MY_VDS_VOLUME_PROP_Xml & target)
	{
		target.m_id = src.m_id;
		target.m_type = src.m_type;
		target.m_status = src.m_status;
		target.m_health = src.m_health;
		target.m_TransitionState = src.m_TransitionState;
		target.m_ullSize = src.m_ullSize;
		target.m_ulFlags = src.m_ulFlags;
		target.m_RecommendedFileSystemType = src.m_RecommendedFileSystemType;
		target.m_strName = src.m_strName;
	}

	static void Xml2Bin(CONST MY_VDS_VOLUME_PROP_Xml & src, MY_VDS_VOLUME_PROP & target)
	{
		target.m_id = src.m_id;
		target.m_type = (VDS_VOLUME_TYPE)src.m_type;
		target.m_status = (VDS_VOLUME_STATUS)src.m_status;
		target.m_health = (VDS_HEALTH)src.m_health;
		target.m_TransitionState = (VDS_TRANSITION_STATE)src.m_TransitionState;
		target.m_ullSize = src.m_ullSize;
		target.m_ulFlags = src.m_ulFlags;
		target.m_RecommendedFileSystemType = (VDS_FILE_SYSTEM_TYPE)src.m_RecommendedFileSystemType;
		target.m_strName = src.m_strName;
	}
}



//m_MyVDSVolumeProp
//m_strVolumeGUID
//m_VolumePlexes

struct CSingleVdsVolumeInfoXml
{
	SERIALIZE_BEGIN_STRUCT(CSingleVdsVolumeInfoXml, CSingleVdsVolumeInfoXml);
	MY_VDS_VOLUME_PROP_Xml m_MyVDSVolumeProp;				SERIALIZE_COMPLEX(m_MyVDSVolumeProp);
	CString m_strVolumeGUID;								SERIALIZE_BASIC(m_strVolumeGUID);
	vector<CSingleVdsVolumePlexXml> m_VolumePlexes;			SERIALIZE_COMPLEX_VECTOR(m_VolumePlexes);
	SERIALIZE_END_STRUCT(CSingleVdsVolumeInfoXml, CSingleVdsVolumeInfoXml);
	STRUCT_XML_SERIALIZER;

	CSingleVdsVolumeInfoXml()
	{
	}
};


namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST CSingleVdsVolumeInfo & src, CSingleVdsVolumeInfoXml & target)
	{
		Bin2Xml(src.m_MyVDSVolumeProp, target.m_MyVDSVolumeProp);
		target.m_strVolumeGUID = src.m_strVolumeGUID;
		target.m_VolumePlexes.resize(src.m_VolumePlexes.size());
		for (size_t ii = 0; ii < target.m_VolumePlexes.size(); ++ ii)
		{
			Bin2Xml(src.m_VolumePlexes[ii], target.m_VolumePlexes[ii]);
		}
	}

	static void Xml2Bin(CONST CSingleVdsVolumeInfoXml & src, CSingleVdsVolumeInfo & target)
	{
		Xml2Bin(src.m_MyVDSVolumeProp, target.m_MyVDSVolumeProp);
		target.m_strVolumeGUID = src.m_strVolumeGUID;
		target.m_VolumePlexes.resize(src.m_VolumePlexes.size());
		for (size_t ii = 0; ii < target.m_VolumePlexes.size(); ++ii)
		{
			Xml2Bin(src.m_VolumePlexes[ii], target.m_VolumePlexes[ii]);
		}
	}
}
