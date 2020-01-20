#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <vector>



#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "..\PublicHeader\PublicSimpleXml.h"
#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\PublicHeader\PublicValueXmlElementConvert.h"
#include "..\PublicHeader\PublicParameterStreamDef.h"

#include "WmiHelper.h"


struct My_MSFT_Disk
{
	SERIALIZE_BEGIN_STRUCT(My_MSFT_Disk, My_MSFT_Disk);
	CString __Path;					SERIALIZE_BASIC(__Path);
	CString  ObjectId;				SERIALIZE_BASIC(ObjectId);
	CString  Path;					SERIALIZE_BASIC(Path);
	CString  Location;				SERIALIZE_BASIC(Location);
	CString  FriendlyName;			SERIALIZE_BASIC(FriendlyName);
	CString  UniqueId;				SERIALIZE_BASIC(UniqueId);
	UINT16  UniqueIdFormat;			SERIALIZE_BASIC(UniqueIdFormat);
	UINT32  Number;					SERIALIZE_BASIC(Number);
	CString  SerialNumber;			SERIALIZE_BASIC(SerialNumber);
	CString  FirmwareVersion;       SERIALIZE_BASIC(FirmwareVersion);
	CString  Manufacturer;			SERIALIZE_BASIC(Manufacturer);
	CString  Model;					SERIALIZE_BASIC(Model);
	UINT64  TotalSize;				SERIALIZE_BASIC(TotalSize);
	UINT64  AllocatedSize;			SERIALIZE_BASIC(AllocatedSize);
	UINT32  LogicalSectorSize;      SERIALIZE_BASIC(LogicalSectorSize);
	UINT32  PhysicalSectorSize;     SERIALIZE_BASIC(PhysicalSectorSize);
	UINT64  LargestFreeExtent;      SERIALIZE_BASIC(LargestFreeExtent);
	UINT32  NumberOfPartitions;     SERIALIZE_BASIC(NumberOfPartitions);
	UINT16  ProvisioningType;       SERIALIZE_BASIC(ProvisioningType);
	UINT16  OperationalStatus;      SERIALIZE_BASIC(OperationalStatus);
	UINT16  HealthStatus;			SERIALIZE_BASIC(HealthStatus);
	UINT16  BusType;				SERIALIZE_BASIC(BusType);
	UINT16  PartitionStyle;			SERIALIZE_BASIC(PartitionStyle);
	UINT32  Signature;				SERIALIZE_BASIC(Signature);
	CString  Guid;					SERIALIZE_BASIC(Guid);
	BOOL IsOffline;					SERIALIZE_BASIC(IsOffline);
	UINT16  OfflineReason;			SERIALIZE_BASIC(OfflineReason);
	BOOL IsReadOnly;				SERIALIZE_BASIC(IsReadOnly);
	BOOL IsSystem;					SERIALIZE_BASIC(IsSystem);
	BOOL IsClustered;				SERIALIZE_BASIC(IsClustered);
	BOOL IsBoot;					SERIALIZE_BASIC(IsBoot);
	BOOL BootFromDisk;				SERIALIZE_BASIC(BootFromDisk);
	SERIALIZE_END_STRUCT(My_MSFT_Disk, My_MSFT_Disk);
	STRUCT_XML_SERIALIZER;


	VOID NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars);
};

struct My_MSFT_Partition
{
	SERIALIZE_BEGIN_STRUCT(My_MSFT_Partition, My_MSFT_Partition);
	CString __Path;								SERIALIZE_BASIC(__Path);
	CString  DiskId;							SERIALIZE_BASIC(DiskId);
	UINT32  DiskNumber;							SERIALIZE_BASIC(DiskNumber);
	UINT32  PartitionNumber;					SERIALIZE_BASIC(PartitionNumber);
	SHORT  DriveLetter;							SERIALIZE_BASIC(DriveLetter);
	std::vector<CString>  AccessPaths;			SERIALIZE_VECTOR(AccessPaths);
	UINT16  OperationalStatus;					SERIALIZE_BASIC(OperationalStatus);
	UINT16  TransitionState;					SERIALIZE_BASIC(TransitionState);
	UINT64  Offset;								SERIALIZE_BASIC(Offset);
	UINT64  Size;								SERIALIZE_BASIC(Size);
	UINT16  MbrType;							SERIALIZE_BASIC(MbrType);
	CString  GptType;							SERIALIZE_BASIC(GptType);
	CString  Guid;								SERIALIZE_BASIC(Guid);
	BOOL IsReadOnly;							SERIALIZE_BASIC(IsReadOnly);
	BOOL IsOffline;								SERIALIZE_BASIC(IsOffline);
	BOOL IsSystem;								SERIALIZE_BASIC(IsSystem);
	BOOL IsBoot;								SERIALIZE_BASIC(IsBoot);
	BOOL IsActive;								SERIALIZE_BASIC(IsActive);
	BOOL IsHidden;								SERIALIZE_BASIC(IsHidden);
	BOOL IsShadowCopy;							SERIALIZE_BASIC(IsShadowCopy);
	BOOL NoDefaultDriveLetter;					SERIALIZE_BASIC(NoDefaultDriveLetter);
	SERIALIZE_END_STRUCT(My_MSFT_Partition, My_MSFT_Partition);
	STRUCT_XML_SERIALIZER;

	VOID NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars);
};

struct My_MSFT_Volume
{
	SERIALIZE_BEGIN_STRUCT(My_MSFT_Volume, My_MSFT_Volume);
	CString __Path;					SERIALIZE_BASIC(__Path);
	CString ObjectId;				SERIALIZE_BASIC(ObjectId);
	SHORT DriveLetter;				SERIALIZE_BASIC(DriveLetter);
	CString Path;					SERIALIZE_BASIC(Path);
	UINT16 HealthStatus;			SERIALIZE_BASIC(HealthStatus);
	CString FileSystem;				SERIALIZE_BASIC(FileSystem);
	CString FileSystemLabel;		SERIALIZE_BASIC(FileSystemLabel);
	UINT64 Size;					SERIALIZE_BASIC(Size);
	UINT64 SizeRemaining;			SERIALIZE_BASIC(SizeRemaining);
	UINT32 DriveType;				SERIALIZE_BASIC(DriveType);
	SERIALIZE_END_STRUCT(My_MSFT_Volume, My_MSFT_Volume);
	STRUCT_XML_SERIALIZER;


	VOID NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars);
};

struct My_MSFT_DiskToPartition
{
	SERIALIZE_BEGIN_STRUCT(My_MSFT_DiskToPartition, My_MSFT_DiskToPartition);
	CString __Path;											SERIALIZE_BASIC(__Path);
	CString /*My_MSFT_Disk*/      /*REF*/ Disk;				SERIALIZE_BASIC(Disk);
	CString /*My_MSFT_Partition*/ /*REF*/ Partition;		SERIALIZE_BASIC(Partition);
	SERIALIZE_END_STRUCT(My_MSFT_DiskToPartition, My_MSFT_DiskToPartition);
	STRUCT_XML_SERIALIZER;

	VOID NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars);
};


struct My_MSFT_PartitionToVolume
{
	SERIALIZE_BEGIN_STRUCT(My_MSFT_PartitionToVolume, My_MSFT_PartitionToVolume);
	CString __Path;											SERIALIZE_BASIC(__Path);
	CString /*My_MSFT_Partition*/ /*REF*/ Partition;		SERIALIZE_BASIC(Partition);
	CString /*My_MSFT_Volume*/    /*REF*/ Volume;			SERIALIZE_BASIC(Volume);
	SERIALIZE_END_STRUCT(My_MSFT_PartitionToVolume, My_MSFT_PartitionToVolume);
	STRUCT_XML_SERIALIZER;

	VOID NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars);
};

struct My_MSFT_PhysicalDisk /*: MSFT_StorageObject*/
{
	SERIALIZE_BEGIN_STRUCT(My_MSFT_PhysicalDisk, My_MSFT_PhysicalDisk);
	CString	__Path;			SERIALIZE_BASIC(__Path);
	CString  DeviceId;			SERIALIZE_BASIC(DeviceId);
	CString  FriendlyName;			SERIALIZE_BASIC(FriendlyName);
	UINT16  Usage;			SERIALIZE_BASIC(Usage);
	std::vector<INT>  SupportedUsages;			SERIALIZE_VECTOR(SupportedUsages);
	CString  Description;			SERIALIZE_BASIC(Description);
	CString  Manufacturer;			SERIALIZE_BASIC(Manufacturer);
	CString  Model;			SERIALIZE_BASIC(Model);
	CString  SerialNumber;			SERIALIZE_BASIC(SerialNumber);
	CString  PartNumber;			SERIALIZE_BASIC(PartNumber);
	CString  FirmwareVersion;			SERIALIZE_BASIC(FirmwareVersion);
	CString  SoftwareVersion;			SERIALIZE_BASIC(SoftwareVersion);
	std::vector<INT>  OperationalStatus;			SERIALIZE_VECTOR(OperationalStatus);
	UINT16  HealthStatus;			SERIALIZE_BASIC(HealthStatus);
	UINT64  Size;			SERIALIZE_BASIC(Size);
	UINT64  AllocatedSize;			SERIALIZE_BASIC(AllocatedSize);
	UINT16  BusType;			SERIALIZE_BASIC(BusType);
	UINT64  PhysicalSectorSize;			SERIALIZE_BASIC(PhysicalSectorSize);
	UINT64  LogicalSectorSize;			SERIALIZE_BASIC(LogicalSectorSize);
	UINT32  SpindleSpeed;			SERIALIZE_BASIC(SpindleSpeed);
	BOOL IsIndicationEnabled;			SERIALIZE_BASIC(IsIndicationEnabled);
	CString  PhysicalLocation;			SERIALIZE_BASIC(PhysicalLocation);
	UINT16  EnclosureNumber;			SERIALIZE_BASIC(EnclosureNumber);
	UINT16  SlotNumber;			SERIALIZE_BASIC(SlotNumber);
	BOOL CanPool;			SERIALIZE_BASIC(CanPool);
	std::vector<INT>  CannotPoolReason;			SERIALIZE_VECTOR(CannotPoolReason);
	CString  OtherCannotPoolReasonDescription;			SERIALIZE_BASIC(OtherCannotPoolReasonDescription);
	BOOL IsPartial;			SERIALIZE_BASIC(IsPartial);
	SERIALIZE_END_STRUCT(My_MSFT_PhysicalDisk, My_MSFT_PhysicalDisk);
	STRUCT_XML_SERIALIZER;

	VOID NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars);
};


struct My_MSFT_VirtualDisk /*: MSFT_StorageObject*/
{
	SERIALIZE_BEGIN_STRUCT(My_MSFT_VirtualDisk, My_MSFT_VirtualDisk);
	CString __Path;			SERIALIZE_BASIC(__Path);
	CString  FriendlyName;			SERIALIZE_BASIC(FriendlyName);
	CString  Name;			SERIALIZE_BASIC(Name);
	UINT16  NameFormat;			SERIALIZE_BASIC(NameFormat);
	UINT16  UniqueIdFormat;			SERIALIZE_BASIC(UniqueIdFormat);
	CString  UniqueIdFormatDescription;			SERIALIZE_BASIC(UniqueIdFormatDescription);
	UINT16  Usage;			SERIALIZE_BASIC(Usage);
	CString  OtherUsageDescription;			SERIALIZE_BASIC(OtherUsageDescription);
	UINT16  HealthStatus;			SERIALIZE_BASIC(HealthStatus);
	std::vector<INT>  OperationalStatus;			SERIALIZE_VECTOR(OperationalStatus);
	CString  OtherOperationalStatusDescription;			SERIALIZE_BASIC(OtherOperationalStatusDescription);
	CString  ResiliencySettingName;			SERIALIZE_BASIC(ResiliencySettingName);
	UINT64  Size;			SERIALIZE_BASIC(Size);
	UINT64  AllocatedSize;			SERIALIZE_BASIC(AllocatedSize);
	UINT64  LogicalSectorSize;			SERIALIZE_BASIC(LogicalSectorSize);
	UINT64  PhysicalSectorSize;			SERIALIZE_BASIC(PhysicalSectorSize);
	UINT64  FootprintOnPool;			SERIALIZE_BASIC(FootprintOnPool);
	UINT16  ProvisioningType;			SERIALIZE_BASIC(ProvisioningType);
	UINT16  NumberOfDataCopies;			SERIALIZE_BASIC(NumberOfDataCopies);
	UINT16  PhysicalDiskRedundancy;			SERIALIZE_BASIC(PhysicalDiskRedundancy);
	UINT16  ParityLayout;			SERIALIZE_BASIC(ParityLayout);
	UINT16  NumberOfColumns;			SERIALIZE_BASIC(NumberOfColumns);
	UINT64  Interleave;			SERIALIZE_BASIC(Interleave);
	BOOL RequestNoSinglePointOfFailure;			SERIALIZE_BASIC(RequestNoSinglePointOfFailure);
	UINT16  Access;			SERIALIZE_BASIC(Access);
	BOOL IsSnapshot;			SERIALIZE_BASIC(IsSnapshot);
	BOOL IsManualAttach;			SERIALIZE_BASIC(IsManualAttach);
	BOOL IsDeduplicationEnabled;			SERIALIZE_BASIC(IsDeduplicationEnabled);
	BOOL IsEnclosureAware;			SERIALIZE_BASIC(IsEnclosureAware);
	UINT16  NumberOfAvailableCopies;			SERIALIZE_BASIC(NumberOfAvailableCopies);
	UINT16  DetachedReason;			SERIALIZE_BASIC(DetachedReason);
	SERIALIZE_END_STRUCT(My_MSFT_VirtualDisk, My_MSFT_VirtualDisk);
	STRUCT_XML_SERIALIZER;

	VOID NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars);
};

struct My_MSFT_VirtualDiskToPhysicalDisk
{
	SERIALIZE_BEGIN_STRUCT(My_MSFT_VirtualDiskToPhysicalDisk, My_MSFT_VirtualDiskToPhysicalDisk)
	CString __Path;			SERIALIZE_BASIC(__Path);
	CString VirtualDisk;			SERIALIZE_BASIC(VirtualDisk);
	CString PhysicalDisk;			SERIALIZE_BASIC(PhysicalDisk);
	SERIALIZE_END_STRUCT(My_MSFT_VirtualDiskToPhysicalDisk, My_MSFT_VirtualDiskToPhysicalDisk);
	STRUCT_XML_SERIALIZER;

	VOID NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars);
};


struct My_MSFT_VirtualDiskToPartition
{
	SERIALIZE_BEGIN_STRUCT(My_MSFT_VirtualDiskToPartition, My_MSFT_VirtualDiskToPartition)
	CString __Path;				SERIALIZE_BASIC(__Path);
	CString VirtualDisk;		SERIALIZE_BASIC(VirtualDisk);
	CString Partition;			SERIALIZE_BASIC(Partition);
	SERIALIZE_END_STRUCT(My_MSFT_VirtualDiskToPartition, My_MSFT_VirtualDiskToPartition);
	STRUCT_XML_SERIALIZER;

	VOID NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars);
};


struct My_MSFT_DisksPartitionsVolumes
{
	SERIALIZE_BEGIN_STRUCT(My_MSFT_DisksPartitionsVolumes, My_MSFT_DisksPartitionsVolumes);
	std::vector<My_MSFT_Disk> Disks;														SERIALIZE_COMPLEX_VECTOR(Disks);
	std::vector<My_MSFT_Partition> Partitions;												SERIALIZE_COMPLEX_VECTOR(Partitions);
	std::vector<My_MSFT_Volume> Volumes;													SERIALIZE_COMPLEX_VECTOR(Volumes);
	std::vector<My_MSFT_DiskToPartition> DiskToPartitions;									SERIALIZE_COMPLEX_VECTOR(DiskToPartitions);
	std::vector<My_MSFT_PartitionToVolume> PartitionToVolumes;								SERIALIZE_COMPLEX_VECTOR(PartitionToVolumes);
	std::vector<My_MSFT_PhysicalDisk> PhysicalDisks;										SERIALIZE_COMPLEX_VECTOR(PhysicalDisks);
	std::vector<My_MSFT_VirtualDisk> VirtualDisks;											SERIALIZE_COMPLEX_VECTOR(VirtualDisks);
	std::vector<My_MSFT_VirtualDiskToPhysicalDisk> VirtualDiskToPhysicalDisks;				SERIALIZE_COMPLEX_VECTOR(VirtualDiskToPhysicalDisks);
	std::vector<My_MSFT_VirtualDiskToPartition> VirtualDiskToPartitions;					SERIALIZE_COMPLEX_VECTOR(VirtualDiskToPartitions);
	SERIALIZE_END_STRUCT(My_MSFT_DisksPartitionsVolumes, My_MSFT_DisksPartitionsVolumes);
	STRUCT_XML_SERIALIZER;

private:
	CString m_machine;
	CString m_user;
	CString m_domain; 
	CString m_password;

	static CONST LPCTSTR s_wmiRes;

public:
	My_MSFT_DisksPartitionsVolumes(
		LPCTSTR machine,
		LPCTSTR user,
		LPCTSTR domain,
		LPCTSTR password
	);

	HRESULT Retrieve();
};
