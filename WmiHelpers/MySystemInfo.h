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

#include "MyMsftStorage.h"
#include "My_NetworkAdapter_Info.h"

struct My_Win32_OperatingSystem /*: CIM_OperatingSystem*/
{
	SERIALIZE_BEGIN_STRUCT(My_Win32_OperatingSystem, My_Win32_OperatingSystem);
	 CString		__Path;		SERIALIZE_BASIC(__Path);				
	 CString   BootDevice;		SERIALIZE_BASIC(BootDevice);
	 CString   BuildNumber;		SERIALIZE_BASIC(BuildNumber);
	 CString   BuildType;		SERIALIZE_BASIC(BuildType);
	 CString   Caption;		SERIALIZE_BASIC(Caption);
	 CString   CodeSet;		SERIALIZE_BASIC(CodeSet);
	 CString   CountryCode;		SERIALIZE_BASIC(CountryCode);
	 CString   CreationClassName;		SERIALIZE_BASIC(CreationClassName);
	 CString   CSCreationClassName;		SERIALIZE_BASIC(CSCreationClassName);
	 CString   CSDVersion;		SERIALIZE_BASIC(CSDVersion);
	 CString   CSName;		SERIALIZE_BASIC(CSName);
	 INT16   CurrentTimeZone;		SERIALIZE_BASIC(CurrentTimeZone);
	 BOOL  DataExecutionPrevention_Available;		SERIALIZE_BASIC(DataExecutionPrevention_Available);
	 BOOL  DataExecutionPrevention_32BitApplications;		SERIALIZE_BASIC(DataExecutionPrevention_32BitApplications);
	 BOOL  DataExecutionPrevention_Drivers;		SERIALIZE_BASIC(DataExecutionPrevention_Drivers);
	 UINT8    DataExecutionPrevention_SupportPolicy;		SERIALIZE_BASIC(DataExecutionPrevention_SupportPolicy);
	 BOOL  Debug;		SERIALIZE_BASIC(Debug);
	 CString   Description;		SERIALIZE_BASIC(Description);
	 BOOL  Distributed;		SERIALIZE_BASIC(Distributed);
	 UINT32   EncryptionLevel;		SERIALIZE_BASIC(EncryptionLevel);
	 UINT8    ForegroundApplicationBoost;		SERIALIZE_BASIC(ForegroundApplicationBoost);
	 UINT64   FreePhysicalMemory;		SERIALIZE_BASIC(FreePhysicalMemory);
	 UINT64   FreeSpaceInPagingFiles;		SERIALIZE_BASIC(FreeSpaceInPagingFiles);
	 UINT64   FreeVirtualMemory;		SERIALIZE_BASIC(FreeVirtualMemory);
	 SYSTEMTIME InstallDate;		SERIALIZE_BASIC(InstallDate);
	 UINT32   LargeSystemCache;		SERIALIZE_BASIC(LargeSystemCache);
	 SYSTEMTIME LastBootUpTime;		SERIALIZE_BASIC(LastBootUpTime);
	 SYSTEMTIME LocalSYSTEMTIME;		SERIALIZE_BASIC(LocalSYSTEMTIME);
	 CString   Locale;		SERIALIZE_BASIC(Locale);
	 CString   Manufacturer;		SERIALIZE_BASIC(Manufacturer);
	 UINT32   MaxNumberOfProcesses;		SERIALIZE_BASIC(MaxNumberOfProcesses);
	 UINT64   MaxProcessMemorySize;		SERIALIZE_BASIC(MaxProcessMemorySize);
	 std::vector<CString>   MUILanguages; SERIALIZE_VECTOR(MUILanguages)
	 CString   Name;		SERIALIZE_BASIC(Name);
	 UINT32   NumberOfLicensedUsers;		SERIALIZE_BASIC(NumberOfLicensedUsers);
	 UINT32   NumberOfProcesses;		SERIALIZE_BASIC(NumberOfProcesses);
	 UINT32   NumberOfUsers;		SERIALIZE_BASIC(NumberOfUsers);
	 UINT32   OperatingSystemSKU;		SERIALIZE_BASIC(OperatingSystemSKU);
	 CString   Organization;		SERIALIZE_BASIC(Organization);
	 CString   OSArchitecture;		SERIALIZE_BASIC(OSArchitecture);
	 UINT32   OSLanguage;		SERIALIZE_BASIC(OSLanguage);
	 UINT32   OSProductSuite;		SERIALIZE_BASIC(OSProductSuite);
	 UINT16   OSType;		SERIALIZE_BASIC(OSType);
	 CString   OtherTypeDescription;		SERIALIZE_BASIC(OtherTypeDescription);
	 BOOL  PAEEnabled;		SERIALIZE_BASIC(PAEEnabled);
	 CString   PlusProductID;		SERIALIZE_BASIC(PlusProductID);
	 CString   PlusVersionNumber;		SERIALIZE_BASIC(PlusVersionNumber);
	 BOOL  PortableOperatingSystem;		SERIALIZE_BASIC(PortableOperatingSystem);
	 BOOL  Primary;		SERIALIZE_BASIC(Primary);
	 UINT32   ProductType;		SERIALIZE_BASIC(ProductType);
	 CString   RegisteredUser;		SERIALIZE_BASIC(RegisteredUser);
	 CString   SerialNumber;		SERIALIZE_BASIC(SerialNumber);
	 UINT16   ServicePackMajorVersion;		SERIALIZE_BASIC(ServicePackMajorVersion);
	 UINT16   ServicePackMinorVersion;		SERIALIZE_BASIC(ServicePackMinorVersion);
	 UINT64   SizeStoredInPagingFiles;		SERIALIZE_BASIC(SizeStoredInPagingFiles);
	 CString   Status;		SERIALIZE_BASIC(Status);
	 UINT32   SuiteMask;		SERIALIZE_BASIC(SuiteMask);
	 CString   SystemDevice;		SERIALIZE_BASIC(SystemDevice);
	 CString   SystemDirectory;		SERIALIZE_BASIC(SystemDirectory);
	 CString   SystemDrive;		SERIALIZE_BASIC(SystemDrive);
	 UINT64   TotalSwapSpaceSize;		SERIALIZE_BASIC(TotalSwapSpaceSize);
	 UINT64   TotalVirtualMemorySize;		SERIALIZE_BASIC(TotalVirtualMemorySize);
	 UINT64   TotalVisibleMemorySize;		SERIALIZE_BASIC(TotalVisibleMemorySize);
	 CString   Version;		SERIALIZE_BASIC(Version);
	 CString   WindowsDirectory;		SERIALIZE_BASIC(WindowsDirectory);

	 SERIALIZE_END_STRUCT(My_Win32_OperatingSystem, My_Win32_OperatingSystem);
	 STRUCT_XML_SERIALIZER;

	 VOID NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars);
};


struct My_Win32_ComputerSystem/* : CIM_UnitaryComputerSystem*/
{
	SERIALIZE_BEGIN_STRUCT(My_Win32_ComputerSystem, My_Win32_ComputerSystem);
	 CString		__Path;		SERIALIZE_BASIC(__Path);
	 UINT16   AdminPasswordStatus;		SERIALIZE_BASIC(AdminPasswordStatus);
	 BOOL  AutomaticManagedPagefile;		SERIALIZE_BASIC(AutomaticManagedPagefile);
	 BOOL  AutomaticResetBootOption;		SERIALIZE_BASIC(AutomaticResetBootOption);
	 BOOL  AutomaticResetCapability;		SERIALIZE_BASIC(AutomaticResetCapability);
	 UINT16   BootOptionOnLimit;		SERIALIZE_BASIC(BootOptionOnLimit);
	 UINT16   BootOptionOnWatchDog;		SERIALIZE_BASIC(BootOptionOnWatchDog);
	 BOOL  BootROMSupported;		SERIALIZE_BASIC(BootROMSupported);
	 CString   BootupState;		SERIALIZE_BASIC(BootupState);
	 CString   Caption;		SERIALIZE_BASIC(Caption);
	 UINT16   ChassisBootupState;		SERIALIZE_BASIC(ChassisBootupState);
	 CString   CreationClassName;		SERIALIZE_BASIC(CreationClassName);
	 INT16   CurrentTimeZone;		SERIALIZE_BASIC(CurrentTimeZone);
	 BOOL  DaylightInEffect;		SERIALIZE_BASIC(DaylightInEffect);
	 CString   Description;		SERIALIZE_BASIC(Description);
	 CString   DNSHostName;		SERIALIZE_BASIC(DNSHostName);
	 CString   Domain;		SERIALIZE_BASIC(Domain);
	 UINT16   DomainRole;		SERIALIZE_BASIC(DomainRole);
	 BOOL  EnableDaylightSavingsTime;		SERIALIZE_BASIC(EnableDaylightSavingsTime);
	 UINT16   FrontPanelResetStatus;		SERIALIZE_BASIC(FrontPanelResetStatus);
	 BOOL  InfraredSupported;		SERIALIZE_BASIC(InfraredSupported);
	 CString   InitialLoadInfo;		SERIALIZE_BASIC(InitialLoadInfo);
	 SYSTEMTIME InstallDate;		SERIALIZE_BASIC(InstallDate);
	 UINT16   KeyboardPasswordStatus;		SERIALIZE_BASIC(KeyboardPasswordStatus);
	 CString   LastLoadInfo;		SERIALIZE_BASIC(LastLoadInfo);
	 CString   Manufacturer;		SERIALIZE_BASIC(Manufacturer);
	 CString   Model;		SERIALIZE_BASIC(Model);
	 CString   Name;		SERIALIZE_BASIC(Name);
	 CString   NameFormat;		SERIALIZE_BASIC(NameFormat);
	 BOOL  NetworkServerModeEnabled;		SERIALIZE_BASIC(NetworkServerModeEnabled);
	 UINT32   NumberOfLogicalProcessors;		SERIALIZE_BASIC(NumberOfLogicalProcessors);
	 UINT32   NumberOfProcessors;		SERIALIZE_BASIC(NumberOfProcessors);
	 vector<UINT8>    OEMLogoBitmap;		SERIALIZE_BASIC(OEMLogoBitmap);
	 vector<CString>   OEMCStringArray;		SERIALIZE_VECTOR(OEMCStringArray);
	 BOOL  PartOfDomain;		SERIALIZE_BASIC(PartOfDomain);
	 INT64   PauseAfterReset;		SERIALIZE_BASIC(PauseAfterReset);
	 UINT16   PCSystemType;		SERIALIZE_BASIC(PCSystemType);
	 vector<UINT16>   PowerManagementCapabilities; SERIALIZE_VECTOR(PowerManagementCapabilities);
	 BOOL  PowerManagementSupported;		SERIALIZE_BASIC(PowerManagementSupported);
	 UINT16   PowerOnPasswordStatus;		SERIALIZE_BASIC(PowerOnPasswordStatus);
	 UINT16   PowerState;		SERIALIZE_BASIC(PowerState);
	 UINT16   PowerSupplyState;		SERIALIZE_BASIC(PowerSupplyState);
	 CString   PrimaryOwnerContact;		SERIALIZE_BASIC(PrimaryOwnerContact);
	 CString   PrimaryOwnerName;		SERIALIZE_BASIC(PrimaryOwnerName);
	 UINT16   ResetCapability;		SERIALIZE_BASIC(ResetCapability);
	 INT16   ResetCount;		SERIALIZE_BASIC(ResetCount);
	 INT16   ResetLimit;		SERIALIZE_BASIC(ResetLimit);
	 vector<CString>   Roles;	SERIALIZE_VECTOR(Roles);
	 CString   Status;		SERIALIZE_BASIC(Status);
	 vector<CString>   SupportContactDescription; SERIALIZE_VECTOR(SupportContactDescription);
	 UINT16   SystemStartupDelay;		SERIALIZE_BASIC(SystemStartupDelay);
	 vector<CString>   SystemStartupOptions;	SERIALIZE_VECTOR(SystemStartupOptions);
	 UINT8    SystemStartupSetting;		SERIALIZE_BASIC(SystemStartupSetting);
	 CString   SystemType;		SERIALIZE_BASIC(SystemType);
	 UINT16   ThermalState;		SERIALIZE_BASIC(ThermalState);
	 UINT64   TotalPhysicalMemory;		SERIALIZE_BASIC(TotalPhysicalMemory);
	 CString   UserName;		SERIALIZE_BASIC(UserName);
	 UINT16   WakeUpType;		SERIALIZE_BASIC(WakeUpType);
	 CString   Workgroup;		SERIALIZE_BASIC(Workgroup);

	 SERIALIZE_END_STRUCT(My_Win32_ComputerSystem, My_Win32_ComputerSystem);
	 STRUCT_XML_SERIALIZER;

	 VOID NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars);
};

struct My_Win32_Processor /*: CIM_Processor*/
{
	SERIALIZE_BEGIN_STRUCT(My_Win32_Processor, My_Win32_Processor);
	 CString		__Path;		SERIALIZE_BASIC(__Path);
	 UINT16   AddressWidth;		SERIALIZE_BASIC(AddressWidth);
	 UINT16   Architecture;		SERIALIZE_BASIC(Architecture);
	 UINT16   Availability;		SERIALIZE_BASIC(Availability);
	 CString   Caption;		SERIALIZE_BASIC(Caption);
	 UINT32   ConfigManagerErrorCode;		SERIALIZE_BASIC(ConfigManagerErrorCode);
	 BOOL  ConfigManagerUserConfig;		SERIALIZE_BASIC(ConfigManagerUserConfig);
	 UINT16   CpuStatus;		SERIALIZE_BASIC(CpuStatus);
	 CString   CreationClassName;		SERIALIZE_BASIC(CreationClassName);
	 UINT32   CurrentClockSpeed;		SERIALIZE_BASIC(CurrentClockSpeed);
	 UINT16   CurrentVoltage;		SERIALIZE_BASIC(CurrentVoltage);
	 UINT16   DataWidth;		SERIALIZE_BASIC(DataWidth);
	 CString   Description;		SERIALIZE_BASIC(Description);
	 CString   DeviceID;		SERIALIZE_BASIC(DeviceID);
	 BOOL  ErrorCleared;		SERIALIZE_BASIC(ErrorCleared);
	 CString   ErrorDescription;		SERIALIZE_BASIC(ErrorDescription);
	 UINT32   ExtClock;		SERIALIZE_BASIC(ExtClock);
	 UINT16   Family;		SERIALIZE_BASIC(Family);
	 SYSTEMTIME InstallDate;		SERIALIZE_BASIC(InstallDate);
	 UINT32   L2CacheSize;		SERIALIZE_BASIC(L2CacheSize);
	 UINT32   L2CacheSpeed;		SERIALIZE_BASIC(L2CacheSpeed);
	 UINT32   L3CacheSize;		SERIALIZE_BASIC(L3CacheSize);
	 UINT32   L3CacheSpeed;		SERIALIZE_BASIC(L3CacheSpeed);
	 UINT32   LastErrorCode;		SERIALIZE_BASIC(LastErrorCode);
	 UINT16   Level;		SERIALIZE_BASIC(Level);
	 UINT16   LoadPercentage;		SERIALIZE_BASIC(LoadPercentage);
	 CString   Manufacturer;		SERIALIZE_BASIC(Manufacturer);
	 UINT32   MaxClockSpeed;		SERIALIZE_BASIC(MaxClockSpeed);
	 CString   Name;		SERIALIZE_BASIC(Name);
	 UINT32   NumberOfCores;		SERIALIZE_BASIC(NumberOfCores);
	 UINT32   NumberOfLogicalProcessors;		SERIALIZE_BASIC(NumberOfLogicalProcessors);
	 CString   OtherFamilyDescription;		SERIALIZE_BASIC(OtherFamilyDescription);
	 CString   PNPDeviceID;		SERIALIZE_BASIC(PNPDeviceID);
	 vector<UINT16>   PowerManagementCapabilities; SERIALIZE_VECTOR(PowerManagementCapabilities);
	 BOOL  PowerManagementSupported;		SERIALIZE_BASIC(PowerManagementSupported);
	 CString   ProcessorId;		SERIALIZE_BASIC(ProcessorId);
	 UINT16   ProcessorType;		SERIALIZE_BASIC(ProcessorType);
	 UINT16   Revision;		SERIALIZE_BASIC(Revision);
	 CString   Role;		SERIALIZE_BASIC(Role);
	 CString   SocketDesignation;		SERIALIZE_BASIC(SocketDesignation);
	 CString   Status;		SERIALIZE_BASIC(Status);
	 UINT16   StatusInfo;		SERIALIZE_BASIC(StatusInfo);
	 CString   Stepping;		SERIALIZE_BASIC(Stepping);
	 CString   SystemCreationClassName;		SERIALIZE_BASIC(SystemCreationClassName);
	 CString   SystemName;		SERIALIZE_BASIC(SystemName);
	 CString   UniqueId;		SERIALIZE_BASIC(UniqueId);
	 UINT16   UpgradeMethod;		SERIALIZE_BASIC(UpgradeMethod);
	 CString   Version;		SERIALIZE_BASIC(Version);
	 UINT32   VoltageCaps;		SERIALIZE_BASIC(VoltageCaps);

	 SERIALIZE_END_STRUCT(My_Win32_Processor, My_Win32_Processor);
	 STRUCT_XML_SERIALIZER;

	VOID NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars);
};


struct My_Win32_Product /*: CIM_Product*/
{
	SERIALIZE_BEGIN_STRUCT(My_Win32_Product, My_Win32_Product);
	CString __Path;			SERIALIZE_BASIC(__Path);
	UINT16   AssignmentType;			SERIALIZE_BASIC(AssignmentType);
	CString   Caption;			SERIALIZE_BASIC(Caption);
	CString   Description;			SERIALIZE_BASIC(Description);
	CString   IdentifyingNumber;			SERIALIZE_BASIC(IdentifyingNumber);
	CString   InstallDate;			SERIALIZE_BASIC(InstallDate);
	SYSTEMTIME InstallDate2;			SERIALIZE_BASIC(InstallDate2);
	CString   InstallLocation;			SERIALIZE_BASIC(InstallLocation);
	INT16   InstallState;			SERIALIZE_BASIC(InstallState);
	CString   HelpLink;			SERIALIZE_BASIC(HelpLink);
	CString   HelpTelephone;			SERIALIZE_BASIC(HelpTelephone);
	CString   InstallSource;			SERIALIZE_BASIC(InstallSource);
	CString   Language;			SERIALIZE_BASIC(Language);
	CString   LocalPackage;			SERIALIZE_BASIC(LocalPackage);
	CString   Name;			SERIALIZE_BASIC(Name);
	CString   PackageCache;			SERIALIZE_BASIC(PackageCache);
	CString   PackageCode;			SERIALIZE_BASIC(PackageCode);
	CString   PackageName;			SERIALIZE_BASIC(PackageName);
	CString   ProductID;			SERIALIZE_BASIC(ProductID);
	CString   RegOwner;			SERIALIZE_BASIC(RegOwner);
	CString   RegCompany;			SERIALIZE_BASIC(RegCompany);
	CString   SKUNumber;			SERIALIZE_BASIC(SKUNumber);
	CString   Transforms;			SERIALIZE_BASIC(Transforms);
	CString   URLInfoAbout;			SERIALIZE_BASIC(URLInfoAbout);
	CString   URLUpdateInfo;			SERIALIZE_BASIC(URLUpdateInfo);
	CString   Vendor;			SERIALIZE_BASIC(Vendor);
	UINT32   WordCount;			SERIALIZE_BASIC(WordCount);
	CString   Version;			SERIALIZE_BASIC(Version);

	SERIALIZE_END_STRUCT(My_Win32_Product, My_Win32_Product);
	STRUCT_XML_SERIALIZER;

	VOID NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars);
};


struct My_Win32_Service /*: Win32_BaseService*/
{
	SERIALIZE_BEGIN_STRUCT(My_Win32_Service, My_Win32_Service);
	CString __Path;			SERIALIZE_BASIC(__Path);
	BOOL  AcceptPause;			SERIALIZE_BASIC(AcceptPause);
	BOOL  AcceptStop;			SERIALIZE_BASIC(AcceptStop);
	CString   Caption;			SERIALIZE_BASIC(Caption);
	UINT32   CheckPoint;			SERIALIZE_BASIC(CheckPoint);
	CString   CreationClassName;			SERIALIZE_BASIC(CreationClassName);
	CString   Description;			SERIALIZE_BASIC(Description);
	boolean  DesktopInteract;			SERIALIZE_BASIC(DesktopInteract);
	CString   DisplayName;			SERIALIZE_BASIC(DisplayName);
	CString   ErrorControl;			SERIALIZE_BASIC(ErrorControl);
	UINT32   ExitCode;			SERIALIZE_BASIC(ExitCode);
	SYSTEMTIME InstallDate;			SERIALIZE_BASIC(InstallDate);
	CString   Name;			SERIALIZE_BASIC(Name);
	CString   PathName;			SERIALIZE_BASIC(PathName);
	UINT32   ProcessId;			SERIALIZE_BASIC(ProcessId);
	UINT32   ServiceSpecificExitCode;			SERIALIZE_BASIC(ServiceSpecificExitCode);
	CString   ServiceType;			SERIALIZE_BASIC(ServiceType);
	BOOL  Started;			SERIALIZE_BASIC(Started);
	CString   StartMode;			SERIALIZE_BASIC(StartMode);
	CString   StartName;			SERIALIZE_BASIC(StartName);
	CString   State;			SERIALIZE_BASIC(State);
	CString   Status;			SERIALIZE_BASIC(Status);
	CString   SystemCreationClassName;			SERIALIZE_BASIC(SystemCreationClassName);
	CString   SystemName;			SERIALIZE_BASIC(SystemName);
	UINT32   TagId;			SERIALIZE_BASIC(TagId);
	UINT32   WaitHint;			SERIALIZE_BASIC(WaitHint);
	SERIALIZE_END_STRUCT(My_Win32_Service, My_Win32_Service);
	STRUCT_XML_SERIALIZER;

	VOID NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars);
};



struct My_System_Info
{
	SERIALIZE_BEGIN_STRUCT(My_System_Info, My_System_Info);
	vector<My_Win32_OperatingSystem> OperatingSystems;									SERIALIZE_COMPLEX_VECTOR(OperatingSystems);
	vector<My_Win32_ComputerSystem> ComputerSystems;									SERIALIZE_COMPLEX_VECTOR(ComputerSystems);
	vector<My_Win32_Processor> Processors;												SERIALIZE_COMPLEX_VECTOR(Processors);
	vector<My_Win32_NetworkAdapter> NetworkAdapters;									SERIALIZE_COMPLEX_VECTOR(NetworkAdapters);
	vector<My_Win32_NetworkAdapterConfiguration> NetworkAdapterConfigurations;			SERIALIZE_COMPLEX_VECTOR(NetworkAdapterConfigurations);
	vector<My_Win32_NetworkAdapterSetting> NetworkAdapterSettings;						SERIALIZE_COMPLEX_VECTOR(NetworkAdapterSettings);
	vector<My_Win32_Product> Products;													SERIALIZE_COMPLEX_VECTOR(Products);
	vector<My_Win32_Service> Services;													SERIALIZE_COMPLEX_VECTOR(Services);
	SERIALIZE_END_STRUCT(My_System_Info, My_System_Info);
	STRUCT_XML_SERIALIZER;



private:
	CString m_machine;
	CString m_user;
	CString m_domain;
	CString m_password;

	static CONST LPCTSTR s_wmiRes;

public:
	My_System_Info(
		LPCTSTR machine,
		LPCTSTR user,
		LPCTSTR domain,
		LPCTSTR password
		);

	HRESULT Retrieve();
};