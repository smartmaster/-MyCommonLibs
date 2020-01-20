#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>


#include "..\PublicHeader\PublicFileDeviceInterface.h"

//////////////////////////////////////////////////////////////////////////
#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"

#include "..\PublicHeader\PublicSimpleXml.h"
#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\PublicHeader\PublicValueXmlElementConvert.h"
#include "..\PublicHeader\PublicParameterStreamDef.h"
//////////////////////////////////////////////////////////////////////////

namespace SmartLib
{
	struct DiskExtent
	{
		IFileDevice * m_disk;
		LONGLONG m_offset;
		LONGLONG m_length;
		LONGLONG m_volumeOffset; //used by spanned volume or spanned simple volume
	};

	enum class VolumeType
	{
		VtUnknown,
		VtSimple,
		VtSpan,
		VtStripe,
		VtMirror,
		VtRaid5
	};

	enum class FsType
	{
		FsRawOrFat16 = 0x6,
		FsNtfs = 0x7,
		FsFat32 = 0xB
	};

	struct PartitionInfo
	{
		SERIALIZE_BEGIN_STRUCT(PartitionInfo, PartitionInfo);
		IFileDevice * m_path;
		CString m_diskName;					SERIALIZE_BASIC(m_diskName);

		LONGLONG m_id;						SERIALIZE_BASIC(m_id);
		LONGLONG m_diskOffsetLba;			SERIALIZE_BASIC(m_diskOffsetLba);
		LONGLONG m_diskOffset;				SERIALIZE_BASIC(m_diskOffset);
		LONGLONG m_sizeLba;					SERIALIZE_BASIC(m_sizeLba);
		LONGLONG m_size;					SERIALIZE_BASIC(m_size);
		LONGLONG m_volumeOffsetLba;			SERIALIZE_BASIC(m_volumeOffsetLba);
		LONGLONG m_volumeOffset;			SERIALIZE_BASIC(m_volumeOffset);
		LONGLONG m_diskId;					SERIALIZE_BASIC(m_diskId);

		CString m_name;						SERIALIZE_BASIC(m_name);
		ULONG m_flags;						SERIALIZE_BASIC(m_flags);
		LONGLONG m_commitTransactionId;		SERIALIZE_BASIC(m_commitTransactionId);
		LONGLONG m_componentId;				SERIALIZE_BASIC(m_componentId);
		SERIALIZE_END_STRUCT(PartitionInfo, PartitionInfo);
		STRUCT_XML_SERIALIZER;

	};
	

	struct VolumeInfo
	{
		SERIALIZE_BEGIN_STRUCT(VolumeInfo, VolumeInfo);
		LONGLONG m_id;					SERIALIZE_BASIC(m_id);
		INT/*VolumeType*/ m_type;				SERIALIZE_BASIC(m_type);
		CString m_typeS;				SERIALIZE_BASIC(m_typeS);
		INT /*FsType*/ m_fileSystem;				SERIALIZE_BASIC(m_fileSystem);
		CString m_fileSystemS;			SERIALIZE_BASIC(m_fileSystemS);
		uint64_t m_sizeLba;			SERIALIZE_BASIC(m_sizeLba);

		CString m_name;			SERIALIZE_BASIC(m_name);
		CString m_typeString;			SERIALIZE_BASIC(m_typeString);
		CString m_disableDriveLetterAssignment;			SERIALIZE_BASIC(m_disableDriveLetterAssignment);
		CString m_state;			SERIALIZE_BASIC(m_state);
		BYTE m_readPolicy;			SERIALIZE_BASIC(m_readPolicy);
		LONGLONG m_volumeNumber;			SERIALIZE_BASIC(m_volumeNumber);
		ULONG m_flags;			SERIALIZE_BASIC(m_flags);
		LONGLONG m_numberOfComponents;			SERIALIZE_BASIC(m_numberOfComponents);
		LONGLONG m_commitTransactionId;			SERIALIZE_BASIC(m_commitTransactionId);
		LONGLONG m_unknownTransactionId;			SERIALIZE_BASIC(m_unknownTransactionId);
		GUID m_volumeGuid;			SERIALIZE_BASIC(m_volumeGuid);
		LONG m_numberOfPartitions;			SERIALIZE_BASIC(m_numberOfPartitions);

		std::vector<PartitionInfo> m_partitions;			SERIALIZE_VECTOR(m_partitions);
		SERIALIZE_END_STRUCT(VolumeInfo, VolumeInfo);
		STRUCT_XML_SERIALIZER;
	};

	struct  DiskInfo
	{
		SERIALIZE_BEGIN_STRUCT(DiskInfo, DiskInfo);
		LONGLONG m_id;			SERIALIZE_BASIC(m_id);
		CString m_guid;			SERIALIZE_BASIC(m_guid);
		IFileDevice * m_path;
		LONG m_sectorSize;			SERIALIZE_BASIC(m_sectorSize);
		LONGLONG m_logicalStartLba;			SERIALIZE_BASIC(m_logicalStartLba);

		CString m_name;			SERIALIZE_BASIC(m_name);
		CString m_lastDevName;			SERIALIZE_BASIC(m_lastDevName);
		ULONG m_flags;			SERIALIZE_BASIC(m_flags);
		LONGLONG m_commitTransactionId;			SERIALIZE_BASIC(m_commitTransactionId);
		LONG m_numberOfPartitions;			SERIALIZE_BASIC(m_numberOfPartitions);

		std::vector<PartitionInfo> m_partions;			SERIALIZE_VECTOR(m_partions);
		SERIALIZE_END_STRUCT(DiskInfo, DiskInfo);
		STRUCT_XML_SERIALIZER;
	};

	struct DynamicVolumesDisksInfos
	{
		SERIALIZE_BEGIN_STRUCT(DynamicVolumesDisksInfos, DynamicVolumesDisksInfos);
		std::vector<VolumeInfo> m_VolumeInfos;		SERIALIZE_VECTOR(m_VolumeInfos);
		std::vector<DiskInfo> m_DiskInfos;			SERIALIZE_VECTOR(m_DiskInfos);
		SERIALIZE_END_STRUCT(DynamicVolumesDisksInfos, DynamicVolumesDisksInfos);
		STRUCT_XML_SERIALIZER;
	};

} //namespace SmartLib