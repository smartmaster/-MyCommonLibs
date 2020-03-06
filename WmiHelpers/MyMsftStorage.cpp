#include "stdafx.h"

#include "MyMsftStorage.h"
#include "MyMsftStorage.h.xml.Output.hpp"


const LPCTSTR My_MSFT_DisksPartitionsVolumes::s_wmiRes = TEXT("Root\\Microsoft\\Windows\\Storage");

My_MSFT_DisksPartitionsVolumes::My_MSFT_DisksPartitionsVolumes(LPCTSTR machine, LPCTSTR user, LPCTSTR domain, LPCTSTR password) :
m_machine(machine),
m_user(user),
m_domain(domain),
m_password(password)
{

}

HRESULT My_MSFT_DisksPartitionsVolumes::Retrieve()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	COAUTHIDENTITY authid = { 0 };
	WMiHelper::__ComposeAuthId(authid, m_user, m_domain, m_password);

	CComPtr<IWbemServices> spService;
	hr = WMiHelper::ApiConnectService(
		m_machine,//IN LPCTSTR Machine,
		m_user,//IN LPCTSTR User,
		m_domain,//IN LPCTSTR Domain,
		m_password,//IN LPCTSTR Password,
		s_wmiRes,//IN LPCTSTR WmiNamespace,
		&spService//OUT IWbemServices ** ppService
		);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("ApiConnectService"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR classNames[] =
	{
		TEXT("MSFT_DISK"),
		TEXT("MSFT_PARTITION"),
		TEXT("MSFT_VOLUME"),
		TEXT("MSFT_DISKTOPARTITION"),
		TEXT("MSFT_PARTITIONTOVOLUME"),
		TEXT("MSFT_PhysicalDisk"),
		TEXT("MSFT_VirtualDisk"),
		TEXT("MSFT_VirtualDiskToPhysicalDisk"),
		TEXT("MSFT_VirtualDiskToPartition")
	};


	for (INT ii = 0; ii < _countof(classNames); ++ii)
	{
		vector<CComPtr<IWbemClassObject> > vecInstances;
		hr = WMiHelper::ApiEnumInstances(
			spService,//IN IWbemServices * pService,
			classNames[ii],//IN LPCTSTR className,
			nullptr,//IN LPCTSTR whereClause,
			m_machine.GetLength() ? &authid : nullptr,//IN COAUTHIDENTITY * AuthId,
			vecInstances//OUT vector<CComPtr<IWbemClassObject> > & vecInstances
			);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			//LEAVE_BLOCK(0);
			D_API_ERR(0, TEXT("ApiEnumInstances"), hr, TEXT("className=%s"), classNames[ii]);
			continue;
		}

		for (size_t jj = 0; jj < vecInstances.size(); ++jj)
		{
			vector<CString>  names;
			vector<CComVariant>  vars;
			hr = WMiHelper::ApiGetAllProps(
				vecInstances[jj],//IN IWbemClassObject * pInstance,
				names,//OUT vector<CString> & names,
				vars//OUT vector<CComVariant> & vars
				);

			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("ApiGetAllProps"), hr, TEXT(""));
				//LEAVE_BLOCK(0);
			}
			else
			{
				switch (ii)
				{
				case 0:
				{
					My_MSFT_Disk obj;
					obj.NameValuesToFields(names, vars);
					Disks.push_back(obj);
				}
				break;

				case 1:
				{
					My_MSFT_Partition obj;
					obj.NameValuesToFields(names, vars);
					Partitions.push_back(obj);
				}
				break;

				case 2:
				{
					My_MSFT_Volume obj;
					obj.NameValuesToFields(names, vars);
					Volumes.push_back(obj);
				}
				break;

				case 3:
				{
					My_MSFT_DiskToPartition obj;
					obj.NameValuesToFields(names, vars);
					DiskToPartitions.push_back(obj);
				}
				break;

				case 4:
				{
					My_MSFT_PartitionToVolume obj;
					obj.NameValuesToFields(names, vars);
					PartitionToVolumes.push_back(obj);
				}
				break;

				case 5:
				{
					My_MSFT_PhysicalDisk obj;
					obj.NameValuesToFields(names, vars);
					PhysicalDisks.push_back(obj);
				}
				break;

				case 6:
				{
					My_MSFT_VirtualDisk obj;
					obj.NameValuesToFields(names, vars);
					VirtualDisks.push_back(obj);
				}
				break;

				case 7:
				{
					My_MSFT_VirtualDiskToPhysicalDisk obj;
					obj.NameValuesToFields(names, vars);
					VirtualDiskToPhysicalDisks.push_back(obj);
				}
				break;


				case 8:
				{
					My_MSFT_VirtualDiskToPartition obj;
					obj.NameValuesToFields(names, vars);
					VirtualDiskToPartitions.push_back(obj);
				}
				break;


				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

VOID My_MSFT_Disk::NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars)
{
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		if (0){}
		WMI_GET_PROP_STRING(__Path)
			WMI_GET_PROP_STRING(ObjectId)
			WMI_GET_PROP_STRING(Path)
			WMI_GET_PROP_STRING(Location)
			WMI_GET_PROP_STRING(FriendlyName)
			WMI_GET_PROP_STRING(UniqueId)
			WMI_GET_PROP_INT(UniqueIdFormat)
			WMI_GET_PROP_INT(Number)
			WMI_GET_PROP_STRING(SerialNumber)
			WMI_GET_PROP_STRING(FirmwareVersion)
			WMI_GET_PROP_STRING(Manufacturer)
			WMI_GET_PROP_STRING(Model)
			WMI_GET_PROP_INT(TotalSize)
			WMI_GET_PROP_INT(AllocatedSize)
			WMI_GET_PROP_INT(LogicalSectorSize)
			WMI_GET_PROP_INT(PhysicalSectorSize)
			WMI_GET_PROP_INT(LargestFreeExtent)
			WMI_GET_PROP_INT(NumberOfPartitions)
			WMI_GET_PROP_INT(ProvisioningType)
			WMI_GET_PROP_INT(OperationalStatus)
			WMI_GET_PROP_INT(HealthStatus)
			WMI_GET_PROP_INT(BusType)
			WMI_GET_PROP_INT(PartitionStyle)
			WMI_GET_PROP_INT(Signature)
			WMI_GET_PROP_STRING(Guid)
			WMI_GET_PROP_INT(IsOffline)
			WMI_GET_PROP_INT(OfflineReason)
			WMI_GET_PROP_INT(IsReadOnly)
			WMI_GET_PROP_INT(IsSystem)
			WMI_GET_PROP_INT(IsClustered)
			WMI_GET_PROP_INT(IsBoot)
			WMI_GET_PROP_INT(BootFromDisk)
	}
}

VOID My_MSFT_Partition::NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars)
{
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		if (0){}
		WMI_GET_PROP_STRING(__Path)
			WMI_GET_PROP_STRING(DiskId)
			WMI_GET_PROP_INT(DiskNumber)
			WMI_GET_PROP_INT(PartitionNumber)
			WMI_GET_PROP_INT(DriveLetter)
			WMI_GET_PROP_ARRAY(CString, BSTR, AccessPaths)
			WMI_GET_PROP_INT(OperationalStatus)
			WMI_GET_PROP_INT(TransitionState)
			WMI_GET_PROP_INT(Offset)
			WMI_GET_PROP_INT(Size)
			WMI_GET_PROP_INT(MbrType)
			WMI_GET_PROP_STRING(GptType)
			WMI_GET_PROP_STRING(Guid)
			WMI_GET_PROP_INT(IsReadOnly)
			WMI_GET_PROP_INT(IsOffline)
			WMI_GET_PROP_INT(IsSystem)
			WMI_GET_PROP_INT(IsBoot)
			WMI_GET_PROP_INT(IsActive)
			WMI_GET_PROP_INT(IsHidden)
			WMI_GET_PROP_INT(IsShadowCopy)
			WMI_GET_PROP_INT(NoDefaultDriveLetter)
	}
}

VOID My_MSFT_Volume::NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars)
{
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		if (0){}
		WMI_GET_PROP_STRING(__Path)
			WMI_GET_PROP_STRING(ObjectId)
			WMI_GET_PROP_INT(DriveLetter)
			WMI_GET_PROP_STRING(Path)
			WMI_GET_PROP_INT(HealthStatus)
			WMI_GET_PROP_STRING(FileSystem)
			WMI_GET_PROP_STRING(FileSystemLabel)
			WMI_GET_PROP_INT(Size)
			WMI_GET_PROP_INT(SizeRemaining)
			WMI_GET_PROP_INT(DriveType)
	}
}

VOID My_MSFT_DiskToPartition::NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars)
{
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		if (0){}
		WMI_GET_PROP_STRING(__Path)
			WMI_GET_PROP_STRING(Disk)
			WMI_GET_PROP_STRING(Partition)
	}
}

VOID My_MSFT_PartitionToVolume::NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars)
{
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		if (0){}
		WMI_GET_PROP_STRING(__Path)
			WMI_GET_PROP_STRING(Partition)
			WMI_GET_PROP_STRING(Volume)
	}
}

VOID My_MSFT_PhysicalDisk::NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars)
{
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		if (0){}
		WMI_GET_PROP_STRING(__Path) //			SERIALIZE_BASIC(__Path);
		WMI_GET_PROP_STRING(DeviceId) //			SERIALIZE_BASIC(DeviceId);
		WMI_GET_PROP_STRING(FriendlyName) //			SERIALIZE_BASIC(FriendlyName);
		WMI_GET_PROP_INT(Usage) //			SERIALIZE_BASIC(Usage);
		WMI_GET_PROP_ARRAY(INT, INT, SupportedUsages)  //			SERIALIZE_VECTOR(SupportedUsages);
		WMI_GET_PROP_STRING(Description) //			SERIALIZE_BASIC(Description);
		WMI_GET_PROP_STRING(Manufacturer) //			SERIALIZE_BASIC(Manufacturer);
		WMI_GET_PROP_STRING(Model) //			SERIALIZE_BASIC(Model);
		WMI_GET_PROP_STRING(SerialNumber) //			SERIALIZE_BASIC(SerialNumber);
		WMI_GET_PROP_STRING(PartNumber) //			SERIALIZE_BASIC(PartNumber);
		WMI_GET_PROP_STRING(FirmwareVersion) //			SERIALIZE_BASIC(FirmwareVersion);
		WMI_GET_PROP_STRING(SoftwareVersion) //			SERIALIZE_BASIC(SoftwareVersion);
		WMI_GET_PROP_ARRAY(INT, INT, OperationalStatus)  //			SERIALIZE_VECTOR(OperationalStatus);
		WMI_GET_PROP_INT(HealthStatus) //			SERIALIZE_BASIC(HealthStatus);
		WMI_GET_PROP_INT(Size) //			SERIALIZE_BASIC(Size);
		WMI_GET_PROP_INT(AllocatedSize) //			SERIALIZE_BASIC(AllocatedSize);
		WMI_GET_PROP_INT(BusType) //			SERIALIZE_BASIC(BusType);
		WMI_GET_PROP_INT(PhysicalSectorSize) //			SERIALIZE_BASIC(PhysicalSectorSize);
		WMI_GET_PROP_INT(LogicalSectorSize) //			SERIALIZE_BASIC(LogicalSectorSize);
		WMI_GET_PROP_INT(SpindleSpeed) //			SERIALIZE_BASIC(SpindleSpeed);
		WMI_GET_PROP_INT(IsIndicationEnabled) //			SERIALIZE_BASIC(IsIndicationEnabled);
		WMI_GET_PROP_STRING(PhysicalLocation) //			SERIALIZE_BASIC(PhysicalLocation);
		WMI_GET_PROP_INT(EnclosureNumber) //			SERIALIZE_BASIC(EnclosureNumber);
		WMI_GET_PROP_INT(SlotNumber) //			SERIALIZE_BASIC(SlotNumber);
		WMI_GET_PROP_INT(CanPool) //			SERIALIZE_BASIC(CanPool);
		WMI_GET_PROP_ARRAY(INT, INT, CannotPoolReason)  //			SERIALIZE_VECTOR(CannotPoolReason);
		WMI_GET_PROP_STRING(OtherCannotPoolReasonDescription) //			SERIALIZE_BASIC(OtherCannotPoolReasonDescription);
		WMI_GET_PROP_INT(IsPartial) //			SERIALIZE_BASIC(IsPartial);
	}
}

VOID My_MSFT_VirtualDisk::NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars)
{
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		if (0){}
		WMI_GET_PROP_STRING(__Path) //			SERIALIZE_BASIC(__Path);
		WMI_GET_PROP_STRING(FriendlyName) //			SERIALIZE_BASIC(FriendlyName);
		WMI_GET_PROP_STRING(Name) //			SERIALIZE_BASIC(Name);
		WMI_GET_PROP_INT(NameFormat) //			SERIALIZE_BASIC(NameFormat);
		WMI_GET_PROP_INT(UniqueIdFormat) //			SERIALIZE_BASIC(UniqueIdFormat);
		WMI_GET_PROP_STRING(UniqueIdFormatDescription) //			SERIALIZE_BASIC(UniqueIdFormatDescription);
		WMI_GET_PROP_INT(Usage) //			SERIALIZE_BASIC(Usage);
		WMI_GET_PROP_STRING(OtherUsageDescription) //			SERIALIZE_BASIC(OtherUsageDescription);
		WMI_GET_PROP_INT(HealthStatus) //			SERIALIZE_BASIC(HealthStatus);
		WMI_GET_PROP_ARRAY(INT, INT, OperationalStatus)  //			SERIALIZE_VECTOR(OperationalStatus);
		WMI_GET_PROP_STRING(OtherOperationalStatusDescription) //			SERIALIZE_BASIC(OtherOperationalStatusDescription);
		WMI_GET_PROP_STRING(ResiliencySettingName) //			SERIALIZE_BASIC(ResiliencySettingName);
		WMI_GET_PROP_INT(Size) //			SERIALIZE_BASIC(Size);
		WMI_GET_PROP_INT(AllocatedSize) //			SERIALIZE_BASIC(AllocatedSize);
		WMI_GET_PROP_INT(LogicalSectorSize) //			SERIALIZE_BASIC(LogicalSectorSize);
		WMI_GET_PROP_INT(PhysicalSectorSize) //			SERIALIZE_BASIC(PhysicalSectorSize);
		WMI_GET_PROP_INT(FootprintOnPool) //			SERIALIZE_BASIC(FootprintOnPool);
		WMI_GET_PROP_INT(ProvisioningType) //			SERIALIZE_BASIC(ProvisioningType);
		WMI_GET_PROP_INT(NumberOfDataCopies) //			SERIALIZE_BASIC(NumberOfDataCopies);
		WMI_GET_PROP_INT(PhysicalDiskRedundancy) //			SERIALIZE_BASIC(PhysicalDiskRedundancy);
		WMI_GET_PROP_INT(ParityLayout) //			SERIALIZE_BASIC(ParityLayout);
		WMI_GET_PROP_INT(NumberOfColumns) //			SERIALIZE_BASIC(NumberOfColumns);
		WMI_GET_PROP_INT(Interleave) //			SERIALIZE_BASIC(Interleave);
		WMI_GET_PROP_INT(RequestNoSinglePointOfFailure) //			SERIALIZE_BASIC(RequestNoSinglePointOfFailure);
		WMI_GET_PROP_INT(Access) //			SERIALIZE_BASIC(Access);
		WMI_GET_PROP_INT(IsSnapshot) //			SERIALIZE_BASIC(IsSnapshot);
		WMI_GET_PROP_INT(IsManualAttach) //			SERIALIZE_BASIC(IsManualAttach);
		WMI_GET_PROP_INT(IsDeduplicationEnabled) //			SERIALIZE_BASIC(IsDeduplicationEnabled);
		WMI_GET_PROP_INT(IsEnclosureAware) //			SERIALIZE_BASIC(IsEnclosureAware);
		WMI_GET_PROP_INT(NumberOfAvailableCopies) //			SERIALIZE_BASIC(NumberOfAvailableCopies);
		WMI_GET_PROP_INT(DetachedReason) //			SERIALIZE_BASIC(DetachedReason);
	}
}

VOID My_MSFT_VirtualDiskToPhysicalDisk::NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars)
{
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		if (0){}
		WMI_GET_PROP_STRING(__Path) //			SERIALIZE_BASIC(__Path);
			WMI_GET_PROP_STRING(VirtualDisk) //			SERIALIZE_BASIC(VirtualDisk);
			WMI_GET_PROP_STRING(PhysicalDisk) //			SERIALIZE_BASIC(PhysicalDisk);
	}
}

VOID My_MSFT_VirtualDiskToPartition::NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars)
{
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		if (0){}
		WMI_GET_PROP_STRING(__Path) //				SERIALIZE_BASIC(__Path);
			WMI_GET_PROP_STRING(VirtualDisk) //		SERIALIZE_BASIC(VirtualDisk);
			WMI_GET_PROP_STRING(Partition) //			SERIALIZE_BASIC(Partition);
	}
}
