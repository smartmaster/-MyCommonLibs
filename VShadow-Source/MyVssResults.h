#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <vector>
#include <vss.h>


#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "..\PublicHeader\PublicSimpleXml.h"
#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\PublicHeader\PublicValueXmlElementConvert.h"
#include "..\PublicHeader\PublicParameterStreamDef.h"


struct My_VSS_SNAPSHOT_PROP
{
	SERIALIZE_BEGIN_STRUCT(My_VSS_SNAPSHOT_PROP, My_VSS_SNAPSHOT_PROP);
	GUID m_SnapshotId;											SERIALIZE_BASIC(m_SnapshotId);
	GUID m_SnapshotSetId;										SERIALIZE_BASIC(m_SnapshotSetId);
	LONG m_lSnapshotsCount;										SERIALIZE_BASIC(m_lSnapshotsCount);
	CString m_pwszSnapshotDeviceObject;							SERIALIZE_BASIC(m_pwszSnapshotDeviceObject);
	CString m_pwszOriginalVolumeName;							SERIALIZE_BASIC(m_pwszOriginalVolumeName);
	CString m_pwszOriginatingMachine;							SERIALIZE_BASIC(m_pwszOriginatingMachine);
	CString m_pwszServiceMachine;								SERIALIZE_BASIC(m_pwszServiceMachine);
	CString m_pwszExposedName;									SERIALIZE_BASIC(m_pwszExposedName);
	CString m_pwszExposedPath;									SERIALIZE_BASIC(m_pwszExposedPath);
	GUID m_ProviderId;											SERIALIZE_BASIC(m_ProviderId);
	LONG m_lSnapshotAttributes;									SERIALIZE_BASIC(m_lSnapshotAttributes);
	LONGLONG m_tsCreationTimestamp;								SERIALIZE_BASIC(m_tsCreationTimestamp);
	INT m_eStatus;												SERIALIZE_BASIC(m_eStatus);
	SERIALIZE_END_STRUCT(My_VSS_SNAPSHOT_PROP, My_VSS_SNAPSHOT_PROP);
	STRUCT_XML_SERIALIZER;

	VOID FromVSS_SNAPSHOT_PROP(CONST VSS_SNAPSHOT_PROP & prop)
	{
		m_SnapshotId = prop.m_SnapshotId;
		m_SnapshotSetId = prop.m_SnapshotSetId;
		m_lSnapshotsCount = prop.m_lSnapshotsCount;
		m_pwszSnapshotDeviceObject = prop.m_pwszSnapshotDeviceObject;
		m_pwszOriginalVolumeName = prop.m_pwszOriginalVolumeName;
		m_pwszOriginatingMachine = prop.m_pwszOriginatingMachine;
		m_pwszServiceMachine = prop.m_pwszServiceMachine;
		m_pwszExposedName = prop.m_pwszExposedName;
		m_pwszExposedPath = prop.m_pwszExposedPath;
		m_ProviderId = prop.m_ProviderId;
		m_lSnapshotAttributes = prop.m_lSnapshotAttributes;
		m_tsCreationTimestamp = prop.m_tsCreationTimestamp;
		m_eStatus = prop.m_eStatus;
	}
};


struct  My_VSS_Results
{
	SERIALIZE_BEGIN_STRUCT(My_VSS_Results, My_VSS_Results)
	GUID								SnapshotSetID;			SERIALIZE_BASIC(SnapshotSetID);
	std::vector<My_VSS_SNAPSHOT_PROP>	SnapshotProps;			SERIALIZE_COMPLEX_VECTOR(SnapshotProps);
	SERIALIZE_END_STRUCT(My_VSS_Results, My_VSS_Results);
	STRUCT_XML_SERIALIZER;
};