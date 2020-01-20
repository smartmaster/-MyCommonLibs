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

#include "SingleAndAllVolumeInfoXml.h"
#include "SingleAndAllDiskInfo.h"

struct DISK_GEOMETRY_Xml 
{
	SERIALIZE_BEGIN_STRUCT(DISK_GEOMETRY_Xml, DISK_GEOMETRY_Xml);
	LONGLONG Cylinders;				SERIALIZE_BASIC(Cylinders);
	/*MEDIA_TYPE*/ INT MediaType;			SERIALIZE_BASIC(MediaType);
	DWORD TracksPerCylinder;		SERIALIZE_BASIC(TracksPerCylinder);
	DWORD SectorsPerTrack;			SERIALIZE_BASIC(SectorsPerTrack);
	DWORD BytesPerSector;			SERIALIZE_BASIC(BytesPerSector);
	SERIALIZE_END_STRUCT(DISK_GEOMETRY_Xml, DISK_GEOMETRY_Xml);
	STRUCT_XML_SERIALIZER;

	DISK_GEOMETRY_Xml()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST DISK_GEOMETRY & src, DISK_GEOMETRY_Xml & target)
	{
		target.Cylinders = src.Cylinders.QuadPart;
		target.MediaType = src.MediaType;
		target.TracksPerCylinder = src.TracksPerCylinder;
		target.SectorsPerTrack = src.SectorsPerTrack;
		target.BytesPerSector = src.BytesPerSector;
	}

	static void Xml2Bin(CONST DISK_GEOMETRY_Xml & src, DISK_GEOMETRY & target)
	{
		target.Cylinders.QuadPart = src.Cylinders;
		target.MediaType = (MEDIA_TYPE)src.MediaType;
		target.TracksPerCylinder = src.TracksPerCylinder;
		target.SectorsPerTrack = src.SectorsPerTrack;
		target.BytesPerSector = src.BytesPerSector;
	}
}

struct DISK_GEOMETRY_EX_Xml
{
	SERIALIZE_BEGIN_STRUCT(DISK_GEOMETRY_EX_Xml, DISK_GEOMETRY_EX_Xml);
	DISK_GEOMETRY_Xml Geometry;		SERIALIZE_COMPLEX(Geometry)
	LONGLONG DiskSize;				SERIALIZE_BASIC(DiskSize);
	SERIALIZE_END_STRUCT(DISK_GEOMETRY_EX_Xml, DISK_GEOMETRY_EX_Xml);
	STRUCT_XML_SERIALIZER;

	DISK_GEOMETRY_EX_Xml()
	{
		DiskSize = 0;
	}
};


namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST DISK_GEOMETRY_EX & src, DISK_GEOMETRY_EX_Xml & target)
	{
		Bin2Xml(src.Geometry, target.Geometry);
		target.DiskSize = src.DiskSize.QuadPart;
	}

	static void Xml2Bin(CONST DISK_GEOMETRY_EX_Xml & src, DISK_GEOMETRY_EX & target)
	{
		Xml2Bin(src.Geometry, target.Geometry);
		target.DiskSize.QuadPart = src.DiskSize;
	}
}



struct DRIVE_LAYOUT_INFORMATION_GPT_Xml
{
	SERIALIZE_BEGIN_STRUCT(DRIVE_LAYOUT_INFORMATION_GPT_Xml, DRIVE_LAYOUT_INFORMATION_GPT_Xml);
	GUID DiskId;						SERIALIZE_BASIC(DiskId);
	LONGLONG StartingUsableOffset;		SERIALIZE_BASIC(StartingUsableOffset);
	LONGLONG UsableLength;				SERIALIZE_BASIC(UsableLength);
	DWORD MaxPartitionCount;			SERIALIZE_BASIC(MaxPartitionCount);
	SERIALIZE_END_STRUCT(DRIVE_LAYOUT_INFORMATION_GPT_Xml, DRIVE_LAYOUT_INFORMATION_GPT_Xml);
	STRUCT_XML_SERIALIZER;


	DRIVE_LAYOUT_INFORMATION_GPT_Xml()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST DRIVE_LAYOUT_INFORMATION_GPT & src, DRIVE_LAYOUT_INFORMATION_GPT_Xml & target)
	{
		target.DiskId = src.DiskId;
		target.StartingUsableOffset = src.StartingUsableOffset.QuadPart;
		target.UsableLength = src.UsableLength.QuadPart;
		target.MaxPartitionCount = src.MaxPartitionCount;
	}

	static void Xml2Bin(CONST DRIVE_LAYOUT_INFORMATION_GPT_Xml & src, DRIVE_LAYOUT_INFORMATION_GPT & target)
	{
		target.DiskId = src.DiskId;
		target.StartingUsableOffset.QuadPart = src.StartingUsableOffset;
		target.UsableLength.QuadPart = src.UsableLength;
		target.MaxPartitionCount = src.MaxPartitionCount;
	}
}



struct PARTITION_INFORMATION_GPT_Xml
{
	SERIALIZE_BEGIN_STRUCT(PARTITION_INFORMATION_GPT_Xml, PARTITION_INFORMATION_GPT_Xml);
	GUID PartitionType;            SERIALIZE_BASIC(PartitionType);
	GUID PartitionId;              SERIALIZE_BASIC(PartitionId);
	DWORD64 Attributes;            SERIALIZE_BASIC(Attributes);
	VectorBYTE Name;             SERIALIZE_BASIC(Name);
	SERIALIZE_END_STRUCT(PARTITION_INFORMATION_GPT_Xml, PARTITION_INFORMATION_GPT_Xml);
	STRUCT_XML_SERIALIZER;


	PARTITION_INFORMATION_GPT_Xml()
	{
		ZeroMemory(this, FIELD_OFFSET(PARTITION_INFORMATION_GPT_Xml, Name));
	}
};


namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST PARTITION_INFORMATION_GPT & src, PARTITION_INFORMATION_GPT_Xml & target)
	{
		target.PartitionType = src.PartitionType;
		target.PartitionId = src.PartitionId;
		target.Attributes = src.Attributes;

		//CONST LONG CHAR_COUNT = 36;
		//LPTSTR pBuffer = target.Name.GetBuffer(CHAR_COUNT);
		//CopyMemory(pBuffer, src.Name, CHAR_COUNT * sizeof(WCHAR));
		//pBuffer[CHAR_COUNT - 1] = 0;
		//target.Name.ReleaseBuffer();
		BufferToVectorHelper(src.Name, sizeof(src.Name), target.Name/*.Buffer*/);

	}

	static void Xml2Bin(CONST PARTITION_INFORMATION_GPT_Xml & src, PARTITION_INFORMATION_GPT & target)
	{
		target.PartitionType = src.PartitionType;
		target.PartitionId = src.PartitionId;
		target.Attributes = src.Attributes;

		//CONST LONG CHAR_COUNT = 36;
		//ATLASSERT(src.Name.GetLength() < CHAR_COUNT);
		//_tcscpy_s(target.Name, src.Name);
		VectorToBufferHelper(target.Name, sizeof(target.Name), src.Name/*.Buffer*/);
	}
}



struct PARTITION_INFORMATION_EX_Xml
{
	SERIALIZE_BEGIN_STRUCT(PARTITION_INFORMATION_EX_Xml, PARTITION_INFORMATION_EX_Xml);
	/*PARTITION_STYLE*/ INT PartitionStyle;				SERIALIZE_BASIC(PartitionStyle);
	LONGLONG StartingOffset;					SERIALIZE_BASIC(StartingOffset);
	LONGLONG PartitionLength;					SERIALIZE_BASIC(PartitionLength);
	DWORD PartitionNumber;						SERIALIZE_BASIC(PartitionNumber);
	BOOLEAN RewritePartition;					SERIALIZE_BASIC(RewritePartition);
	PARTITION_INFORMATION_GPT_Xml Gpt;			SERIALIZE_COMPLEX(Gpt);
	SERIALIZE_END_STRUCT(PARTITION_INFORMATION_EX_Xml, PARTITION_INFORMATION_EX_Xml);
	STRUCT_XML_SERIALIZER;

	PARTITION_INFORMATION_EX_Xml()
	{
		ZeroMemory(this, FIELD_OFFSET(PARTITION_INFORMATION_EX_Xml, Gpt));
	}
};

namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST PARTITION_INFORMATION_EX & src, PARTITION_INFORMATION_EX_Xml & target)
	{
		target.PartitionStyle = src.PartitionStyle;
		target.StartingOffset = src.StartingOffset.QuadPart;
		target.PartitionLength = src.PartitionLength.QuadPart;
		target.PartitionNumber = src.PartitionNumber;
		target.RewritePartition = src.RewritePartition;
		Bin2Xml(src.Gpt, target.Gpt);
	}

	static void Xml2Bin(CONST PARTITION_INFORMATION_EX_Xml & src, PARTITION_INFORMATION_EX & target)
	{
		target.PartitionStyle = (PARTITION_STYLE)src.PartitionStyle;
		target.StartingOffset.QuadPart = src.StartingOffset;
		target.PartitionLength.QuadPart = src.PartitionLength;
		target.PartitionNumber = src.PartitionNumber;
		target.RewritePartition = src.RewritePartition;
		Xml2Bin(src.Gpt, target.Gpt);
	}
}


struct DRIVE_LAYOUT_INFORMATION_EX_Xml
{
	SERIALIZE_BEGIN_STRUCT(DRIVE_LAYOUT_INFORMATION_EX_Xml, DRIVE_LAYOUT_INFORMATION_EX_Xml);
	DWORD PartitionStyle;										SERIALIZE_BASIC(PartitionStyle);
	DWORD PartitionCount;										SERIALIZE_BASIC(PartitionCount);
	DRIVE_LAYOUT_INFORMATION_GPT_Xml Gpt;						SERIALIZE_COMPLEX(Gpt);
	vector<PARTITION_INFORMATION_EX_Xml> PartitionEntry;		SERIALIZE_COMPLEX_VECTOR(PartitionEntry);
	SERIALIZE_END_STRUCT(DRIVE_LAYOUT_INFORMATION_EX_Xml, DRIVE_LAYOUT_INFORMATION_EX_Xml);
	STRUCT_XML_SERIALIZER;

	DRIVE_LAYOUT_INFORMATION_EX_Xml()
	{
		PartitionStyle = 0;
		PartitionCount = 0;
	}
};

namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST DRIVE_LAYOUT_INFORMATION_EX & src, DRIVE_LAYOUT_INFORMATION_EX_Xml & target)
	{
		target.PartitionStyle = src.PartitionStyle;
		target.PartitionCount = src.PartitionCount;
		Bin2Xml(src.Gpt, target.Gpt);
		target.PartitionEntry.resize(target.PartitionCount);
		for (size_t ii = 0; ii < target.PartitionEntry.size(); ++ ii)
		{
			Bin2Xml(src.PartitionEntry[ii], target.PartitionEntry[ii]);
		}
	}

	static void Xml2Bin(CONST DRIVE_LAYOUT_INFORMATION_EX_Xml & src, DRIVE_LAYOUT_INFORMATION_EX & target)
	{
		target.PartitionStyle = src.PartitionStyle;
		target.PartitionCount = src.PartitionCount;
		Xml2Bin(src.Gpt, target.Gpt);
		for (size_t ii = 0; ii < src.PartitionEntry.size(); ++ii)
		{
			Xml2Bin(src.PartitionEntry[ii], target.PartitionEntry[ii]);
		}
	}
}

struct STORAGE_DEVICE_DESCRIPTOR_Xml
{
	SERIALIZE_BEGIN_STRUCT(STORAGE_DEVICE_DESCRIPTOR_Xml, STORAGE_DEVICE_DESCRIPTOR_Xml);
	DWORD Version;								SERIALIZE_BASIC(Version);
	DWORD Size;									SERIALIZE_BASIC(Size);
	BYTE  DeviceType;							SERIALIZE_BASIC(DeviceType);
	BYTE  DeviceTypeModifier;					SERIALIZE_BASIC(DeviceTypeModifier);
	BOOLEAN RemovableMedia;						SERIALIZE_BASIC(RemovableMedia);
	BOOLEAN CommandQueueing;					SERIALIZE_BASIC(CommandQueueing);
	DWORD VendorIdOffset;						SERIALIZE_BASIC(VendorIdOffset);
	DWORD ProductIdOffset;						SERIALIZE_BASIC(ProductIdOffset);
	DWORD ProductRevisionOffset;				SERIALIZE_BASIC(ProductRevisionOffset);
	DWORD SerialNumberOffset;					SERIALIZE_BASIC(SerialNumberOffset);
	/*STORAGE_BUS_TYPE*/ INT BusType;					SERIALIZE_BASIC(BusType);
	DWORD RawPropertiesLength;					SERIALIZE_BASIC(RawPropertiesLength);
	VectorBYTE  RawDeviceProperties;			SERIALIZE_BASIC(RawDeviceProperties);
	SERIALIZE_END_STRUCT(STORAGE_DEVICE_DESCRIPTOR_Xml, STORAGE_DEVICE_DESCRIPTOR_Xml);
	STRUCT_XML_SERIALIZER;

	STORAGE_DEVICE_DESCRIPTOR_Xml()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST STORAGE_DEVICE_DESCRIPTOR & src, STORAGE_DEVICE_DESCRIPTOR_Xml & target)
	{
		target.Version = src.Version;
		target.Size = src.Size;
		target.DeviceType = src.DeviceType;
		target.DeviceTypeModifier = src.DeviceTypeModifier;
		target.RemovableMedia = src.RemovableMedia;
		target.CommandQueueing = src.CommandQueueing;
		target.VendorIdOffset = src.VendorIdOffset;
		target.ProductIdOffset = src.ProductIdOffset;
		target.ProductRevisionOffset = src.ProductRevisionOffset;
		target.SerialNumberOffset = src.SerialNumberOffset;
		target.BusType = src.BusType;
		target.RawPropertiesLength = src.RawPropertiesLength;
		target.RawDeviceProperties/*.Buffer*/.resize(src.Size - FIELD_OFFSET(STORAGE_DEVICE_DESCRIPTOR, RawDeviceProperties));
		BufferToVectorHelper(src.RawDeviceProperties, target.RawDeviceProperties/*.Buffer*/.size(), target.RawDeviceProperties/*.Buffer*/);
	}

	static void Xml2Bin(CONST STORAGE_DEVICE_DESCRIPTOR_Xml & src, STORAGE_DEVICE_DESCRIPTOR & target)
	{
		target.Version = src.Version;
		target.Size = src.Size;
		target.DeviceType = src.DeviceType;
		target.DeviceTypeModifier = src.DeviceTypeModifier;
		target.RemovableMedia = src.RemovableMedia;
		target.CommandQueueing = src.CommandQueueing;
		target.VendorIdOffset = src.VendorIdOffset;
		target.ProductIdOffset = src.ProductIdOffset;
		target.ProductRevisionOffset = src.ProductRevisionOffset;
		target.SerialNumberOffset = src.SerialNumberOffset;
		target.BusType = (STORAGE_BUS_TYPE)src.BusType;
		target.RawPropertiesLength = src.RawPropertiesLength;
		VectorToBufferHelper(target.RawDeviceProperties, src.RawDeviceProperties/*.Buffer*/.size(), src.RawDeviceProperties/*.Buffer*/);
	}
}

struct STORAGE_ADAPTER_DESCRIPTOR_Xml 
{
	SERIALIZE_BEGIN_STRUCT(STORAGE_ADAPTER_DESCRIPTOR_Xml, STORAGE_ADAPTER_DESCRIPTOR_Xml);
	DWORD Version;						SERIALIZE_BASIC(Version);
	DWORD Size;							SERIALIZE_BASIC(Size);
	DWORD MaximumTransferLength;		SERIALIZE_BASIC(MaximumTransferLength);
	DWORD MaximumPhysicalPages;			SERIALIZE_BASIC(MaximumPhysicalPages);
	DWORD AlignmentMask;				SERIALIZE_BASIC(AlignmentMask);
	BOOLEAN AdapterUsesPio;				SERIALIZE_BASIC(AdapterUsesPio);
	BOOLEAN AdapterScansDown;			SERIALIZE_BASIC(AdapterScansDown);
	BOOLEAN CommandQueueing;			SERIALIZE_BASIC(CommandQueueing);
	BOOLEAN AcceleratedTransfer;		SERIALIZE_BASIC(AcceleratedTransfer);
	
#if (NTDDI_VERSION < NTDDI_WINXP)		
	BOOLEAN BusType;					SERIALIZE_BASIC(BusType);
#else									
	BYTE  BusType;						SERIALIZE_BASIC(BusType);
#endif									

	WORD   BusMajorVersion;				SERIALIZE_BASIC(BusMajorVersion);
	WORD   BusMinorVersion;				SERIALIZE_BASIC(BusMinorVersion);

#if (NTDDI_VERSION >= NTDDI_WIN8)
	BYTE  SrbType;						SERIALIZE_BASIC(SrbType);
	BYTE  AddressType;					SERIALIZE_BASIC(AddressType);
#endif

	SERIALIZE_END_STRUCT(STORAGE_ADAPTER_DESCRIPTOR_Xml, STORAGE_ADAPTER_DESCRIPTOR_Xml);
	STRUCT_XML_SERIALIZER;

	STORAGE_ADAPTER_DESCRIPTOR_Xml()
	{
		ZeroMemory(this, sizeof(*this));
	}
};


namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST STORAGE_ADAPTER_DESCRIPTOR & src, STORAGE_ADAPTER_DESCRIPTOR_Xml & target)
	{
		target.Version = src.Version;
		target.Size = src.Size;
		target.MaximumTransferLength = src.MaximumTransferLength;
		target.MaximumPhysicalPages = src.MaximumPhysicalPages;
		target.AlignmentMask = src.AlignmentMask;
		target.AdapterUsesPio = src.AdapterUsesPio;
		target.AdapterScansDown = src.AdapterScansDown;
		target.CommandQueueing = src.CommandQueueing;
		target.AcceleratedTransfer = src.AcceleratedTransfer;

#if (NTDDI_VERSION < NTDDI_WINXP)
		target.BusType = src.BusType;
#else
		target.BusType = src.BusType;
#endif

		target.BusMajorVersion = src.BusMajorVersion;
		target.BusMinorVersion = src.BusMinorVersion;

#if (NTDDI_VERSION >= NTDDI_WIN8)
		target.SrbType = src.SrbType;
		target.AddressType = src.AddressType;
#endif

	}

	static void Xml2Bin(CONST STORAGE_ADAPTER_DESCRIPTOR_Xml & src, STORAGE_ADAPTER_DESCRIPTOR & target)
	{
		target.Version = src.Version;
		target.Size = src.Size;
		target.MaximumTransferLength = src.MaximumTransferLength;
		target.MaximumPhysicalPages = src.MaximumPhysicalPages;
		target.AlignmentMask = src.AlignmentMask;
		target.AdapterUsesPio = src.AdapterUsesPio;
		target.AdapterScansDown = src.AdapterScansDown;
		target.CommandQueueing = src.CommandQueueing;
		target.AcceleratedTransfer = src.AcceleratedTransfer;

#if (NTDDI_VERSION < NTDDI_WINXP)
		target.BusType = src.BusType;
#else
		target.BusType = src.BusType;
#endif

		target.BusMajorVersion = src.BusMajorVersion;
		target.BusMinorVersion = src.BusMinorVersion;

#if (NTDDI_VERSION >= NTDDI_WIN8)
		target.SrbType = src.SrbType;
		target.AddressType = src.AddressType;
#endif
	}
}


struct STORAGE_DEVICE_ID_DESCRIPTOR_Xml
{
	SERIALIZE_BEGIN_STRUCT(STORAGE_DEVICE_ID_DESCRIPTOR_Xml, STORAGE_DEVICE_ID_DESCRIPTOR_Xml);
	DWORD Version;					SERIALIZE_BASIC(Version);
	DWORD Size;						SERIALIZE_BASIC(Size);
	DWORD NumberOfIdentifiers;		SERIALIZE_BASIC(NumberOfIdentifiers);
	VectorBYTE  Identifiers;		SERIALIZE_BASIC(Identifiers);
	SERIALIZE_END_STRUCT(STORAGE_DEVICE_ID_DESCRIPTOR_Xml, STORAGE_DEVICE_ID_DESCRIPTOR_Xml);
	STRUCT_XML_SERIALIZER;

	STORAGE_DEVICE_ID_DESCRIPTOR_Xml()
	{
		ZeroMemory(this, FIELD_OFFSET(STORAGE_DEVICE_ID_DESCRIPTOR_Xml, Identifiers));
	}
};

namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST STORAGE_DEVICE_ID_DESCRIPTOR & src, STORAGE_DEVICE_ID_DESCRIPTOR_Xml & target)
	{
		target.Version = src.Version;
		target.Size = src.Size;
		target.NumberOfIdentifiers = src.NumberOfIdentifiers;
		target.Identifiers/*.Buffer*/.resize(src.Size - FIELD_OFFSET(STORAGE_DEVICE_ID_DESCRIPTOR, Identifiers));
		BufferToVectorHelper(src.Identifiers, target.Identifiers/*.Buffer*/.size(), target.Identifiers/*.Buffer*/);
	}

	static void Xml2Bin(CONST STORAGE_DEVICE_ID_DESCRIPTOR_Xml & src, STORAGE_DEVICE_ID_DESCRIPTOR & target)
	{
		target.Version = src.Version;
		target.Size = src.Size;
		target.NumberOfIdentifiers = src.NumberOfIdentifiers;
		VectorToBufferHelper(target.Identifiers, src.Identifiers/*.Buffer*/.size(), src.Identifiers/*.Buffer*/);
	}
}


struct STORAGE_DEVICE_NUMBER_Xml
{
	SERIALIZE_BEGIN_STRUCT(STORAGE_DEVICE_NUMBER_Xml, STORAGE_DEVICE_NUMBER_Xml);
	DEVICE_TYPE DeviceType;				SERIALIZE_BASIC(DeviceType);
	DWORD       DeviceNumber;			SERIALIZE_BASIC(DeviceNumber);
	DWORD       PartitionNumber;		SERIALIZE_BASIC(PartitionNumber);
	SERIALIZE_END_STRUCT(STORAGE_DEVICE_NUMBER_Xml, STORAGE_DEVICE_NUMBER_Xml);
	STRUCT_XML_SERIALIZER;

	STORAGE_DEVICE_NUMBER_Xml()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST STORAGE_DEVICE_NUMBER & src, STORAGE_DEVICE_NUMBER_Xml & target)
	{
		target.DeviceType = src.DeviceType;;
		target.DeviceNumber = src.DeviceNumber;;
		target.PartitionNumber = src.PartitionNumber;;
	}

	static void Xml2Bin(CONST STORAGE_DEVICE_NUMBER_Xml & src, STORAGE_DEVICE_NUMBER & target)
	{
		target.DeviceType = src.DeviceType;;
		target.DeviceNumber = src.DeviceNumber;;
		target.PartitionNumber = src.PartitionNumber;;
	}
}


struct STORAGE_HOTPLUG_INFO_Xml
{
	SERIALIZE_BEGIN_STRUCT(STORAGE_HOTPLUG_INFO_Xml, STORAGE_HOTPLUG_INFO_Xml);
	DWORD Size;								SERIALIZE_BASIC(Size);
	BOOLEAN MediaRemovable; 				SERIALIZE_BASIC(MediaRemovable);
	BOOLEAN MediaHotplug;   				SERIALIZE_BASIC(MediaHotplug);
	BOOLEAN DeviceHotplug;  				SERIALIZE_BASIC(DeviceHotplug);
	BOOLEAN WriteCacheEnableOverride; 		SERIALIZE_BASIC(WriteCacheEnableOverride);
	SERIALIZE_END_STRUCT(STORAGE_HOTPLUG_INFO_Xml, STORAGE_HOTPLUG_INFO_Xml);
	STRUCT_XML_SERIALIZER;

	STORAGE_HOTPLUG_INFO_Xml()
	{
		ZeroMemory(this, sizeof(*this));
	}
};



namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST STORAGE_HOTPLUG_INFO & src, STORAGE_HOTPLUG_INFO_Xml & target)
	{
		target.Size = src.Size;;
		target.MediaRemovable = src.MediaRemovable;;
		target.MediaHotplug = src.MediaHotplug;;
		target.DeviceHotplug = src.DeviceHotplug;;
		target.WriteCacheEnableOverride = src.WriteCacheEnableOverride;;
	}

	static void Xml2Bin(CONST STORAGE_HOTPLUG_INFO_Xml & src, STORAGE_HOTPLUG_INFO & target)
	{
		target.Size = src.Size;;
		target.MediaRemovable = src.MediaRemovable;;
		target.MediaHotplug = src.MediaHotplug;;
		target.DeviceHotplug = src.DeviceHotplug;;
		target.WriteCacheEnableOverride = src.WriteCacheEnableOverride;;
	}
}





struct DEVICE_MEDIA_INFO_Xml
{
	SERIALIZE_BEGIN_STRUCT(DEVICE_MEDIA_INFO_Xml, DEVICE_MEDIA_INFO_Xml);
	struct
	{
		LONGLONG Cylinders;						SERIALIZE_BASIC(DiskInfo.Cylinders);
		/*STORAGE_MEDIA_TYPE*/ INT MediaType;			SERIALIZE_BASIC(DiskInfo.MediaType);
		DWORD TracksPerCylinder;				SERIALIZE_BASIC(DiskInfo.TracksPerCylinder);
		DWORD SectorsPerTrack;					SERIALIZE_BASIC(DiskInfo.SectorsPerTrack);
		DWORD BytesPerSector;					SERIALIZE_BASIC(DiskInfo.BytesPerSector);
		DWORD NumberMediaSides;					SERIALIZE_BASIC(DiskInfo.NumberMediaSides);
		DWORD MediaCharacteristics;				SERIALIZE_BASIC(DiskInfo.MediaCharacteristics);
	} DiskInfo;
	SERIALIZE_END_STRUCT(DEVICE_MEDIA_INFO_Xml, DEVICE_MEDIA_INFO_Xml);
	STRUCT_XML_SERIALIZER;


	DEVICE_MEDIA_INFO_Xml()
	{
		ZeroMemory(this, sizeof(*this));
	}
};


namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST DEVICE_MEDIA_INFO & src, DEVICE_MEDIA_INFO_Xml & target)
	{
		target.DiskInfo.Cylinders = src.DeviceSpecific.DiskInfo.Cylinders.QuadPart;
		target.DiskInfo.MediaType = src.DeviceSpecific.DiskInfo.MediaType;
		target.DiskInfo.TracksPerCylinder = src.DeviceSpecific.DiskInfo.TracksPerCylinder;
		target.DiskInfo.SectorsPerTrack = src.DeviceSpecific.DiskInfo.SectorsPerTrack;
		target.DiskInfo.BytesPerSector = src.DeviceSpecific.DiskInfo.BytesPerSector;
		target.DiskInfo.NumberMediaSides = src.DeviceSpecific.DiskInfo.NumberMediaSides;
		target.DiskInfo.MediaCharacteristics = src.DeviceSpecific.DiskInfo.MediaCharacteristics;
	}

	static void Xml2Bin(CONST DEVICE_MEDIA_INFO_Xml & src, DEVICE_MEDIA_INFO & target)
	{
		target.DeviceSpecific.DiskInfo.Cylinders.QuadPart = src.DiskInfo.Cylinders;
		target.DeviceSpecific.DiskInfo.MediaType = (STORAGE_MEDIA_TYPE)src.DiskInfo.MediaType;
		target.DeviceSpecific.DiskInfo.TracksPerCylinder = src.DiskInfo.TracksPerCylinder;
		target.DeviceSpecific.DiskInfo.SectorsPerTrack = src.DiskInfo.SectorsPerTrack;
		target.DeviceSpecific.DiskInfo.BytesPerSector = src.DiskInfo.BytesPerSector;
		target.DeviceSpecific.DiskInfo.NumberMediaSides = src.DiskInfo.NumberMediaSides;
		target.DeviceSpecific.DiskInfo.MediaCharacteristics = src.DiskInfo.MediaCharacteristics;
	}
}

struct GET_MEDIA_TYPES_Xml
{
	SERIALIZE_BEGIN_STRUCT(GET_MEDIA_TYPES_Xml, GET_MEDIA_TYPES_Xml);
	DWORD DeviceType;							SERIALIZE_BASIC(DeviceType);
	DWORD MediaInfoCount;						SERIALIZE_BASIC(MediaInfoCount);
	vector<DEVICE_MEDIA_INFO_Xml> MediaInfo;	SERIALIZE_COMPLEX_VECTOR(MediaInfo);
	SERIALIZE_END_STRUCT(GET_MEDIA_TYPES_Xml, GET_MEDIA_TYPES_Xml);
	STRUCT_XML_SERIALIZER;

	GET_MEDIA_TYPES_Xml()
	{
		DeviceType = 0;
		MediaInfoCount = 0;
	}
};

namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST GET_MEDIA_TYPES & src, GET_MEDIA_TYPES_Xml & target)
	{
		target.DeviceType = src.DeviceType;
		target.MediaInfoCount = src.MediaInfoCount;
		target.MediaInfo.resize(src.MediaInfoCount);
		for (size_t ii = 0; ii < target.MediaInfo.size(); ++ ii)
		{
			Bin2Xml(src.MediaInfo[ii], target.MediaInfo[ii]);
		}
	}

	static void Xml2Bin(CONST GET_MEDIA_TYPES_Xml & src, GET_MEDIA_TYPES & target)
	{
		target.DeviceType = src.DeviceType;
		target.MediaInfoCount = src.MediaInfoCount;
		for (size_t ii = 0; ii < src.MediaInfo.size(); ++ii)
		{
			Xml2Bin(src.MediaInfo[ii], target.MediaInfo[ii]);
		}
	}
}


struct CSingleDiskInfoXml
{
	SERIALIZE_BEGIN_STRUCT(CSingleDiskInfoXml, CSingleDiskInfoXml);
	DWORD m_DiskNumber;															SERIALIZE_BASIC(m_DiskNumber);
	DISK_GEOMETRY_EX_Xml  m_pDiskGeometryEx;									SERIALIZE_COMPLEX(m_pDiskGeometryEx);
	DRIVE_LAYOUT_INFORMATION_EX_Xml  m_pDriveLayoutInformationEx;				SERIALIZE_COMPLEX(m_pDriveLayoutInformationEx);
	PARTITION_INFORMATION_EX_Xml m_PartitionInformationEx;						SERIALIZE_COMPLEX(m_PartitionInformationEx);
	GET_LENGTH_INFORMATION_Xml m_GetLengthInformation;							SERIALIZE_COMPLEX(m_GetLengthInformation);
	STORAGE_DEVICE_DESCRIPTOR_Xml  m_pStorageDeviceDescriptor;					SERIALIZE_COMPLEX(m_pStorageDeviceDescriptor);
	STORAGE_ADAPTER_DESCRIPTOR_Xml  m_pStorageAdapterDescriptor;				SERIALIZE_COMPLEX(m_pStorageAdapterDescriptor);
	STORAGE_DEVICE_ID_DESCRIPTOR_Xml  m_pStorageDeviceIDDescriptor;				SERIALIZE_COMPLEX(m_pStorageDeviceIDDescriptor);
	STORAGE_DEVICE_ID_DESCRIPTOR_Xml  m_pStorageDeviceUniqueIDDescriptor;		SERIALIZE_COMPLEX(m_pStorageDeviceUniqueIDDescriptor);
	STORAGE_DEVICE_NUMBER_Xml m_StorageDeviceNumber;							SERIALIZE_COMPLEX(m_StorageDeviceNumber);
	STORAGE_HOTPLUG_INFO_Xml m_StorageHotplugInfo;								SERIALIZE_COMPLEX(m_StorageHotplugInfo);
	GET_MEDIA_TYPES_Xml  m_pMEDIA_TYPES;										SERIALIZE_COMPLEX(m_pMEDIA_TYPES);
	SERIALIZE_END_STRUCT(CSingleDiskInfoXml, CSingleDiskInfoXml);
	STRUCT_XML_SERIALIZER;

	CSingleDiskInfoXml()
	{
		m_DiskNumber = 0;
	}
};

namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST CSingleDiskInfo & src, CSingleDiskInfoXml & target)
	{
		target.m_DiskNumber = src.m_DiskNumber;
		if (src.m_pDiskGeometryEx) Bin2Xml(*src.m_pDiskGeometryEx, target.m_pDiskGeometryEx);
		if (src.m_pDriveLayoutInformationEx) Bin2Xml(*src.m_pDriveLayoutInformationEx, target.m_pDriveLayoutInformationEx);
		Bin2Xml(src.m_PartitionInformationEx, target.m_PartitionInformationEx);
		Bin2Xml(src.m_GetLengthInformation, target.m_GetLengthInformation);
		if (src.m_pStorageDeviceDescriptor) Bin2Xml(*src.m_pStorageDeviceDescriptor, target.m_pStorageDeviceDescriptor);
		if (src.m_pStorageAdapterDescriptor) Bin2Xml(*src.m_pStorageAdapterDescriptor, target.m_pStorageAdapterDescriptor);
		if (src.m_pStorageDeviceIDDescriptor) Bin2Xml(*src.m_pStorageDeviceIDDescriptor, target.m_pStorageDeviceIDDescriptor);
		if (src.m_pStorageDeviceUniqueIDDescriptor) Bin2Xml(*src.m_pStorageDeviceUniqueIDDescriptor, target.m_pStorageDeviceUniqueIDDescriptor);
		Bin2Xml(src.m_StorageDeviceNumber, target.m_StorageDeviceNumber);
		Bin2Xml(src.m_StorageHotplugInfo, target.m_StorageHotplugInfo);
		if (src.m_pMEDIA_TYPES) Bin2Xml(*src.m_pMEDIA_TYPES, target.m_pMEDIA_TYPES);
	}

	
	static void Xml2Bin(CONST CSingleDiskInfoXml & src, CSingleDiskInfo & target)
	{
		target.m_DiskNumber = src.m_DiskNumber;

		//if (src.m_pDiskGeometryEx) Bin2Xml(*src.m_pDiskGeometryEx, target.m_pDiskGeometryEx);
		{
			CHeapPtr<BYTE> apBuffer;
			size_t allocSize = sizeof(DISK_GEOMETRY_EX);
			apBuffer.Allocate(allocSize);
			ZeroMemory(apBuffer.m_pData, allocSize);
			target.m_pDiskGeometryEx = (DISK_GEOMETRY_EX *)apBuffer.Detach();
		}
		Xml2Bin(src.m_pDiskGeometryEx, *target.m_pDiskGeometryEx);

		//if (src.m_pDriveLayoutInformationEx) Bin2Xml(*src.m_pDriveLayoutInformationEx, target.m_pDriveLayoutInformationEx);
		{
			CHeapPtr<BYTE> apBuffer;
			size_t allocSize = sizeof(DRIVE_LAYOUT_INFORMATION_EX) + src.m_pDriveLayoutInformationEx.PartitionCount * sizeof(PARTITION_INFORMATION_EX);
			apBuffer.Allocate(allocSize);
			ZeroMemory(apBuffer.m_pData, allocSize);
			target.m_pDriveLayoutInformationEx = (DRIVE_LAYOUT_INFORMATION_EX *)apBuffer.Detach();
		}
		Xml2Bin(src.m_pDriveLayoutInformationEx, *target.m_pDriveLayoutInformationEx);


		//Bin2Xml(src.m_PartitionInformationEx, target.m_PartitionInformationEx);
		Xml2Bin(src.m_PartitionInformationEx, target.m_PartitionInformationEx);

		//Bin2Xml(src.m_GetLengthInformation, target.m_GetLengthInformation);
		Xml2Bin(src.m_GetLengthInformation, target.m_GetLengthInformation);

		//if (src.m_pStorageDeviceDescriptor) Bin2Xml(*src.m_pStorageDeviceDescriptor, target.m_pStorageDeviceDescriptor);
		{
			CHeapPtr<BYTE> apBuffer;
			size_t allocSize = sizeof(STORAGE_DEVICE_DESCRIPTOR) + src.m_pStorageDeviceDescriptor.RawDeviceProperties/*.Buffer*/.size() * sizeof(BYTE);
			apBuffer.Allocate(allocSize);
			ZeroMemory(apBuffer.m_pData, allocSize);
			target.m_pStorageDeviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR *)apBuffer.Detach();
		}
		Xml2Bin(src.m_pStorageDeviceDescriptor, *target.m_pStorageDeviceDescriptor);


		//if (src.m_pStorageAdapterDescriptor) Bin2Xml(*src.m_pStorageAdapterDescriptor, target.m_pStorageAdapterDescriptor);
		{
			CHeapPtr<BYTE> apBuffer;
			size_t allocSize = sizeof(STORAGE_ADAPTER_DESCRIPTOR);
			apBuffer.Allocate(allocSize);
			ZeroMemory(apBuffer.m_pData, allocSize);
			target.m_pStorageAdapterDescriptor = (STORAGE_ADAPTER_DESCRIPTOR *)apBuffer.Detach();
		}
		Xml2Bin(src.m_pStorageAdapterDescriptor, *target.m_pStorageAdapterDescriptor);


		//if (src.m_pStorageDeviceIDDescriptor) Bin2Xml(*src.m_pStorageDeviceIDDescriptor, target.m_pStorageDeviceIDDescriptor);
		{
			CHeapPtr<BYTE> apBuffer;
			size_t allocSize = sizeof(STORAGE_DEVICE_ID_DESCRIPTOR) + src.m_pStorageDeviceIDDescriptor.Identifiers/*.Buffer*/.size() * sizeof(BYTE);
			apBuffer.Allocate(allocSize);
			ZeroMemory(apBuffer.m_pData, allocSize);
			target.m_pStorageDeviceIDDescriptor = (STORAGE_DEVICE_ID_DESCRIPTOR *)apBuffer.Detach();
		}
		Xml2Bin(src.m_pStorageDeviceIDDescriptor, *target.m_pStorageDeviceIDDescriptor);

		//if (src.m_pStorageDeviceUniqueIDDescriptor) Bin2Xml(*src.m_pStorageDeviceUniqueIDDescriptor, target.m_pStorageDeviceUniqueIDDescriptor);
		{
			CHeapPtr<BYTE> apBuffer;
			size_t allocSize = sizeof(STORAGE_DEVICE_ID_DESCRIPTOR) + src.m_pStorageDeviceUniqueIDDescriptor.Identifiers/*.Buffer*/.size() * sizeof(BYTE);
			apBuffer.Allocate(allocSize);
			ZeroMemory(apBuffer.m_pData, allocSize);
			target.m_pStorageDeviceUniqueIDDescriptor = (STORAGE_DEVICE_ID_DESCRIPTOR *)apBuffer.Detach();
		}
		Xml2Bin(src.m_pStorageDeviceUniqueIDDescriptor, *target.m_pStorageDeviceUniqueIDDescriptor);


		//Bin2Xml(src.m_StorageDeviceNumber, target.m_StorageDeviceNumber);
		Xml2Bin(src.m_StorageDeviceNumber, target.m_StorageDeviceNumber);

		//Bin2Xml(src.m_StorageHotplugInfo, target.m_StorageHotplugInfo);
		Xml2Bin(src.m_StorageHotplugInfo, target.m_StorageHotplugInfo);

		//if (src.m_pMEDIA_TYPES) Bin2Xml(*src.m_pMEDIA_TYPES, target.m_pMEDIA_TYPES);
		{
			CHeapPtr<BYTE> apBuffer;
			size_t allocSize = sizeof(GET_MEDIA_TYPES) + src.m_pMEDIA_TYPES.MediaInfoCount * sizeof(DEVICE_MEDIA_INFO);
			apBuffer.Allocate(allocSize);
			ZeroMemory(apBuffer.m_pData, allocSize);
			target.m_pMEDIA_TYPES = (GET_MEDIA_TYPES *)apBuffer.Detach();
		}
		Xml2Bin(src.m_pMEDIA_TYPES, *target.m_pMEDIA_TYPES);
	}
}



/*
{
CHeapPtr<BYTE> apBuffer;
size_t allocSize = sizeof(xxx) + xxxx * sizeof(xxxx);
apBuffer.Allocate(allocSize);
ZeroMemory(apBuffer.m_pData, allocSize);
target.xxx = (xxxx *)apBuffer.Detach();
}
Xml2Bin(src.xxx, *target.xxx);
*/

struct CAllDiskInfoXml
{
	SERIALIZE_BEGIN_STRUCT(CAllDiskInfoXml, CAllDiskInfoXml);
	vector<CSingleDiskInfoXml> m_pDiskInfos; SERIALIZE_COMPLEX_VECTOR(m_pDiskInfos);
	SERIALIZE_END_STRUCT(CAllDiskInfoXml, CAllDiskInfoXml);
	STRUCT_XML_SERIALIZER;
};

namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST CAllDiskInfo & src, CAllDiskInfoXml & target)
	{
		target.m_pDiskInfos.resize(src.m_pDiskInfos.size());
		for (size_t ii = 0; ii < target.m_pDiskInfos.size(); ++ ii)
		{
			if (src.m_pDiskInfos[ii])
			{
				Bin2Xml(*src.m_pDiskInfos[ii], target.m_pDiskInfos[ii]);
			}
		}
	}

	static void Xml2Bin(CONST CAllDiskInfoXml & src, CAllDiskInfo & target)
	{
		target.m_pDiskInfos.resize(src.m_pDiskInfos.size());
		for (size_t ii = 0; ii < target.m_pDiskInfos.size(); ++ii)
		{
			target.m_pDiskInfos[ii] = new CSingleDiskInfo;
			Xml2Bin(src.m_pDiskInfos[ii], *target.m_pDiskInfos[ii]);
		}
	}
}

