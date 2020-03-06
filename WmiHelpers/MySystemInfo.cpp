#include "stdafx.h"
#include "MySystemInfo.h"

#include "MySystemInfo.h.xml.Output.hpp"

VOID My_Win32_OperatingSystem::NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars)
{
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		if (0){}
		WMI_GET_PROP_STRING(__Path) //		SERIALIZE_BASIC(__Path);
		WMI_GET_PROP_STRING(BootDevice) //		SERIALIZE_BASIC(BootDevice);
		WMI_GET_PROP_STRING(BuildNumber) //		SERIALIZE_BASIC(BuildNumber);
		WMI_GET_PROP_STRING(BuildType) //		SERIALIZE_BASIC(BuildType);
		WMI_GET_PROP_STRING(Caption) //		SERIALIZE_BASIC(Caption);
		WMI_GET_PROP_STRING(CodeSet) //		SERIALIZE_BASIC(CodeSet);
		WMI_GET_PROP_STRING(CountryCode) //		SERIALIZE_BASIC(CountryCode);
		WMI_GET_PROP_STRING(CreationClassName) //		SERIALIZE_BASIC(CreationClassName);
		WMI_GET_PROP_STRING(CSCreationClassName) //		SERIALIZE_BASIC(CSCreationClassName);
		WMI_GET_PROP_STRING(CSDVersion) //		SERIALIZE_BASIC(CSDVersion);
		WMI_GET_PROP_STRING(CSName) //		SERIALIZE_BASIC(CSName);
		WMI_GET_PROP_INT(CurrentTimeZone) //		SERIALIZE_BASIC(CurrentTimeZone);
		WMI_GET_PROP_INT(DataExecutionPrevention_Available) //		SERIALIZE_BASIC(DataExecutionPrevention_Available);
		WMI_GET_PROP_INT(DataExecutionPrevention_32BitApplications) //		SERIALIZE_BASIC(DataExecutionPrevention_32BitApplications);
		WMI_GET_PROP_INT(DataExecutionPrevention_Drivers) //		SERIALIZE_BASIC(DataExecutionPrevention_Drivers);
		WMI_GET_PROP_INT(DataExecutionPrevention_SupportPolicy) //		SERIALIZE_BASIC(DataExecutionPrevention_SupportPolicy);
		WMI_GET_PROP_INT(Debug) //		SERIALIZE_BASIC(Debug);
		WMI_GET_PROP_STRING(Description) //		SERIALIZE_BASIC(Description);
		WMI_GET_PROP_INT(Distributed) //		SERIALIZE_BASIC(Distributed);
		WMI_GET_PROP_INT(EncryptionLevel) //		SERIALIZE_BASIC(EncryptionLevel);
		WMI_GET_PROP_INT(ForegroundApplicationBoost) //		SERIALIZE_BASIC(ForegroundApplicationBoost);
		WMI_GET_PROP_INT(FreePhysicalMemory) //		SERIALIZE_BASIC(FreePhysicalMemory);
		WMI_GET_PROP_INT(FreeSpaceInPagingFiles) //		SERIALIZE_BASIC(FreeSpaceInPagingFiles);
		WMI_GET_PROP_INT(FreeVirtualMemory) //		SERIALIZE_BASIC(FreeVirtualMemory);
		WMI_GET_PROP_SYSTEMTIME(InstallDate) //		SERIALIZE_BASIC(InstallDate);
		WMI_GET_PROP_INT(LargeSystemCache) //		SERIALIZE_BASIC(LargeSystemCache);
		WMI_GET_PROP_SYSTEMTIME(LastBootUpTime) //		SERIALIZE_BASIC(LastBootUpTime);
		WMI_GET_PROP_SYSTEMTIME(LocalSYSTEMTIME) //		SERIALIZE_BASIC(LocalSYSTEMTIME);
		WMI_GET_PROP_STRING(Locale) //		SERIALIZE_BASIC(Locale);
		WMI_GET_PROP_STRING(Manufacturer) //		SERIALIZE_BASIC(Manufacturer);
		WMI_GET_PROP_INT(MaxNumberOfProcesses) //		SERIALIZE_BASIC(MaxNumberOfProcesses);
		WMI_GET_PROP_INT(MaxProcessMemorySize) //		SERIALIZE_BASIC(MaxProcessMemorySize);
		WMI_GET_PROP_ARRAY(CString, BSTR, MUILanguages) //std::vector<CString>   MUILanguages; SERIALIZE_VECTOR(MUILanguages)
			WMI_GET_PROP_STRING(Name) //		SERIALIZE_BASIC(Name);
		WMI_GET_PROP_INT(NumberOfLicensedUsers) //		SERIALIZE_BASIC(NumberOfLicensedUsers);
		WMI_GET_PROP_INT(NumberOfProcesses) //		SERIALIZE_BASIC(NumberOfProcesses);
		WMI_GET_PROP_INT(NumberOfUsers) //		SERIALIZE_BASIC(NumberOfUsers);
		WMI_GET_PROP_INT(OperatingSystemSKU) //		SERIALIZE_BASIC(OperatingSystemSKU);
		WMI_GET_PROP_STRING(Organization) //		SERIALIZE_BASIC(Organization);
		WMI_GET_PROP_STRING(OSArchitecture) //		SERIALIZE_BASIC(OSArchitecture);
		WMI_GET_PROP_INT(OSLanguage) //		SERIALIZE_BASIC(OSLanguage);
		WMI_GET_PROP_INT(OSProductSuite) //		SERIALIZE_BASIC(OSProductSuite);
		WMI_GET_PROP_INT(OSType) //		SERIALIZE_BASIC(OSType);
		WMI_GET_PROP_STRING(OtherTypeDescription) //		SERIALIZE_BASIC(OtherTypeDescription);
		WMI_GET_PROP_INT(PAEEnabled) //		SERIALIZE_BASIC(PAEEnabled);
		WMI_GET_PROP_STRING(PlusProductID) //		SERIALIZE_BASIC(PlusProductID);
		WMI_GET_PROP_STRING(PlusVersionNumber) //		SERIALIZE_BASIC(PlusVersionNumber);
		WMI_GET_PROP_INT(PortableOperatingSystem) //		SERIALIZE_BASIC(PortableOperatingSystem);
		WMI_GET_PROP_INT(Primary) //		SERIALIZE_BASIC(Primary);
		WMI_GET_PROP_INT(ProductType) //		SERIALIZE_BASIC(ProductType);
		WMI_GET_PROP_STRING(RegisteredUser) //		SERIALIZE_BASIC(RegisteredUser);
		WMI_GET_PROP_STRING(SerialNumber) //		SERIALIZE_BASIC(SerialNumber);
		WMI_GET_PROP_INT(ServicePackMajorVersion) //		SERIALIZE_BASIC(ServicePackMajorVersion);
		WMI_GET_PROP_INT(ServicePackMinorVersion) //		SERIALIZE_BASIC(ServicePackMinorVersion);
		WMI_GET_PROP_INT(SizeStoredInPagingFiles) //		SERIALIZE_BASIC(SizeStoredInPagingFiles);
		WMI_GET_PROP_STRING(Status) //		SERIALIZE_BASIC(Status);
		WMI_GET_PROP_INT(SuiteMask) //		SERIALIZE_BASIC(SuiteMask);
		WMI_GET_PROP_STRING(SystemDevice) //		SERIALIZE_BASIC(SystemDevice);
		WMI_GET_PROP_STRING(SystemDirectory) //		SERIALIZE_BASIC(SystemDirectory);
		WMI_GET_PROP_STRING(SystemDrive) //		SERIALIZE_BASIC(SystemDrive);
		WMI_GET_PROP_INT(TotalSwapSpaceSize) //		SERIALIZE_BASIC(TotalSwapSpaceSize);
		WMI_GET_PROP_INT(TotalVirtualMemorySize) //		SERIALIZE_BASIC(TotalVirtualMemorySize);
		WMI_GET_PROP_INT(TotalVisibleMemorySize) //		SERIALIZE_BASIC(TotalVisibleMemorySize);
		WMI_GET_PROP_STRING(Version) //		SERIALIZE_BASIC(Version);
		WMI_GET_PROP_STRING(WindowsDirectory) //		SERIALIZE_BASIC(WindowsDirectory);
	}
}

VOID My_Win32_ComputerSystem::NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars)
{
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		if (0){}
		WMI_GET_PROP_STRING(__Path) //		SERIALIZE_BASIC(__Path);
		WMI_GET_PROP_INT(AdminPasswordStatus) //		SERIALIZE_BASIC(AdminPasswordStatus);
		WMI_GET_PROP_INT(AutomaticManagedPagefile) //		SERIALIZE_BASIC(AutomaticManagedPagefile);
		WMI_GET_PROP_INT(AutomaticResetBootOption) //		SERIALIZE_BASIC(AutomaticResetBootOption);
		WMI_GET_PROP_INT(AutomaticResetCapability) //		SERIALIZE_BASIC(AutomaticResetCapability);
		WMI_GET_PROP_INT(BootOptionOnLimit) //		SERIALIZE_BASIC(BootOptionOnLimit);
		WMI_GET_PROP_INT(BootOptionOnWatchDog) //		SERIALIZE_BASIC(BootOptionOnWatchDog);
		WMI_GET_PROP_INT(BootROMSupported) //		SERIALIZE_BASIC(BootROMSupported);
		WMI_GET_PROP_STRING(BootupState) //		SERIALIZE_BASIC(BootupState);
		WMI_GET_PROP_STRING(Caption) //		SERIALIZE_BASIC(Caption);
		WMI_GET_PROP_INT(ChassisBootupState) //		SERIALIZE_BASIC(ChassisBootupState);
		WMI_GET_PROP_STRING(CreationClassName) //		SERIALIZE_BASIC(CreationClassName);
		WMI_GET_PROP_INT(CurrentTimeZone) //		SERIALIZE_BASIC(CurrentTimeZone);
		WMI_GET_PROP_INT(DaylightInEffect) //		SERIALIZE_BASIC(DaylightInEffect);
		WMI_GET_PROP_STRING(Description) //		SERIALIZE_BASIC(Description);
		WMI_GET_PROP_STRING(DNSHostName) //		SERIALIZE_BASIC(DNSHostName);
		WMI_GET_PROP_STRING(Domain) //		SERIALIZE_BASIC(Domain);
		WMI_GET_PROP_INT(DomainRole) //		SERIALIZE_BASIC(DomainRole);
		WMI_GET_PROP_INT(EnableDaylightSavingsTime) //		SERIALIZE_BASIC(EnableDaylightSavingsTime);
		WMI_GET_PROP_INT(FrontPanelResetStatus) //		SERIALIZE_BASIC(FrontPanelResetStatus);
		WMI_GET_PROP_INT(InfraredSupported) //		SERIALIZE_BASIC(InfraredSupported);
		WMI_GET_PROP_STRING(InitialLoadInfo) //		SERIALIZE_BASIC(InitialLoadInfo);
		WMI_GET_PROP_SYSTEMTIME(InstallDate) //		SERIALIZE_BASIC(InstallDate);
		WMI_GET_PROP_INT(KeyboardPasswordStatus) //		SERIALIZE_BASIC(KeyboardPasswordStatus);
		WMI_GET_PROP_STRING(LastLoadInfo) //		SERIALIZE_BASIC(LastLoadInfo);
		WMI_GET_PROP_STRING(Manufacturer) //		SERIALIZE_BASIC(Manufacturer);
		WMI_GET_PROP_STRING(Model) //		SERIALIZE_BASIC(Model);
		WMI_GET_PROP_STRING(Name) //		SERIALIZE_BASIC(Name);
		WMI_GET_PROP_STRING(NameFormat) //		SERIALIZE_BASIC(NameFormat);
		WMI_GET_PROP_INT(NetworkServerModeEnabled) //		SERIALIZE_BASIC(NetworkServerModeEnabled);
		WMI_GET_PROP_INT(NumberOfLogicalProcessors) //		SERIALIZE_BASIC(NumberOfLogicalProcessors);
		WMI_GET_PROP_INT(NumberOfProcessors) //		SERIALIZE_BASIC(NumberOfProcessors);
		WMI_GET_PROP_ARRAY(UINT8, UINT8, OEMLogoBitmap)		//SERIALIZE_BASIC(OEMLogoBitmap);
		WMI_GET_PROP_ARRAY(CString, BSTR, OEMCStringArray)		//SERIALIZE_VECTOR(OEMCStringArray);
		WMI_GET_PROP_INT(PartOfDomain) //		SERIALIZE_BASIC(PartOfDomain);
		WMI_GET_PROP_INT(PauseAfterReset) //		SERIALIZE_BASIC(PauseAfterReset);
		WMI_GET_PROP_INT(PCSystemType) //		SERIALIZE_BASIC(PCSystemType);
		WMI_GET_PROP_ARRAY(UINT16, UINT16, PowerManagementCapabilities) //SERIALIZE_VECTOR(PowerManagementCapabilities);
		WMI_GET_PROP_INT(PowerManagementSupported) //		SERIALIZE_BASIC(PowerManagementSupported);
		WMI_GET_PROP_INT(PowerOnPasswordStatus) //		SERIALIZE_BASIC(PowerOnPasswordStatus);
		WMI_GET_PROP_INT(PowerState) //		SERIALIZE_BASIC(PowerState);
		WMI_GET_PROP_INT(PowerSupplyState) //		SERIALIZE_BASIC(PowerSupplyState);
		WMI_GET_PROP_STRING(PrimaryOwnerContact) //		SERIALIZE_BASIC(PrimaryOwnerContact);
		WMI_GET_PROP_STRING(PrimaryOwnerName) //		SERIALIZE_BASIC(PrimaryOwnerName);
		WMI_GET_PROP_INT(ResetCapability) //		SERIALIZE_BASIC(ResetCapability);
		WMI_GET_PROP_INT(ResetCount) //		SERIALIZE_BASIC(ResetCount);
		WMI_GET_PROP_INT(ResetLimit) //		SERIALIZE_BASIC(ResetLimit);
		WMI_GET_PROP_ARRAY(CString, BSTR, Roles)	//SERIALIZE_VECTOR(Roles);
		WMI_GET_PROP_STRING(Status) //		SERIALIZE_BASIC(Status);
		WMI_GET_PROP_ARRAY(CString, BSTR, SupportContactDescription) //SERIALIZE_VECTOR(SupportContactDescription);
		WMI_GET_PROP_INT(SystemStartupDelay) //		SERIALIZE_BASIC(SystemStartupDelay);
		WMI_GET_PROP_ARRAY(CString, BSTR, SystemStartupOptions)	//SERIALIZE_VECTOR(SystemStartupOptions);
		WMI_GET_PROP_INT(SystemStartupSetting) //		SERIALIZE_BASIC(SystemStartupSetting);
		WMI_GET_PROP_STRING(SystemType) //		SERIALIZE_BASIC(SystemType);
		WMI_GET_PROP_INT(ThermalState) //		SERIALIZE_BASIC(ThermalState);
		WMI_GET_PROP_INT(TotalPhysicalMemory) //		SERIALIZE_BASIC(TotalPhysicalMemory);
		WMI_GET_PROP_STRING(UserName) //		SERIALIZE_BASIC(UserName);
		WMI_GET_PROP_INT(WakeUpType) //		SERIALIZE_BASIC(WakeUpType);
		WMI_GET_PROP_STRING(Workgroup) //		SERIALIZE_BASIC(Workgroup);
	}
}

VOID My_Win32_Processor::NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars)
{
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		if (0){}
		WMI_GET_PROP_STRING(__Path) //		SERIALIZE_BASIC(__Path);
		WMI_GET_PROP_INT(AddressWidth) //		SERIALIZE_BASIC(AddressWidth);
		WMI_GET_PROP_INT(Architecture) //		SERIALIZE_BASIC(Architecture);
		WMI_GET_PROP_INT(Availability) //		SERIALIZE_BASIC(Availability);
		WMI_GET_PROP_STRING(Caption) //		SERIALIZE_BASIC(Caption);
		WMI_GET_PROP_INT(ConfigManagerErrorCode) //		SERIALIZE_BASIC(ConfigManagerErrorCode);
		WMI_GET_PROP_INT(ConfigManagerUserConfig) //		SERIALIZE_BASIC(ConfigManagerUserConfig);
		WMI_GET_PROP_INT(CpuStatus) //		SERIALIZE_BASIC(CpuStatus);
		WMI_GET_PROP_STRING(CreationClassName) //		SERIALIZE_BASIC(CreationClassName);
		WMI_GET_PROP_INT(CurrentClockSpeed) //		SERIALIZE_BASIC(CurrentClockSpeed);
		WMI_GET_PROP_INT(CurrentVoltage) //		SERIALIZE_BASIC(CurrentVoltage);
		WMI_GET_PROP_INT(DataWidth) //		SERIALIZE_BASIC(DataWidth);
		WMI_GET_PROP_STRING(Description) //		SERIALIZE_BASIC(Description);
		WMI_GET_PROP_STRING(DeviceID) //		SERIALIZE_BASIC(DeviceID);
		WMI_GET_PROP_INT(ErrorCleared) //		SERIALIZE_BASIC(ErrorCleared);
		WMI_GET_PROP_STRING(ErrorDescription) //		SERIALIZE_BASIC(ErrorDescription);
		WMI_GET_PROP_INT(ExtClock) //		SERIALIZE_BASIC(ExtClock);
		WMI_GET_PROP_INT(Family) //		SERIALIZE_BASIC(Family);
		WMI_GET_PROP_SYSTEMTIME(InstallDate) //		SERIALIZE_BASIC(InstallDate);
		WMI_GET_PROP_INT(L2CacheSize) //		SERIALIZE_BASIC(L2CacheSize);
		WMI_GET_PROP_INT(L2CacheSpeed) //		SERIALIZE_BASIC(L2CacheSpeed);
		WMI_GET_PROP_INT(L3CacheSize) //		SERIALIZE_BASIC(L3CacheSize);
		WMI_GET_PROP_INT(L3CacheSpeed) //		SERIALIZE_BASIC(L3CacheSpeed);
		WMI_GET_PROP_INT(LastErrorCode) //		SERIALIZE_BASIC(LastErrorCode);
		WMI_GET_PROP_INT(Level) //		SERIALIZE_BASIC(Level);
		WMI_GET_PROP_INT(LoadPercentage) //		SERIALIZE_BASIC(LoadPercentage);
		WMI_GET_PROP_STRING(Manufacturer) //		SERIALIZE_BASIC(Manufacturer);
		WMI_GET_PROP_INT(MaxClockSpeed) //		SERIALIZE_BASIC(MaxClockSpeed);
		WMI_GET_PROP_STRING(Name) //		SERIALIZE_BASIC(Name);
		WMI_GET_PROP_INT(NumberOfCores) //		SERIALIZE_BASIC(NumberOfCores);
		WMI_GET_PROP_INT(NumberOfLogicalProcessors) //		SERIALIZE_BASIC(NumberOfLogicalProcessors);
		WMI_GET_PROP_STRING(OtherFamilyDescription) //		SERIALIZE_BASIC(OtherFamilyDescription);
		WMI_GET_PROP_STRING(PNPDeviceID) //		SERIALIZE_BASIC(PNPDeviceID);
		WMI_GET_PROP_ARRAY(UINT16, UINT16, PowerManagementCapabilities)  // SERIALIZE_VECTOR(PowerManagementCapabilities);
		WMI_GET_PROP_INT(PowerManagementSupported) //		SERIALIZE_BASIC(PowerManagementSupported);
		WMI_GET_PROP_STRING(ProcessorId) //		SERIALIZE_BASIC(ProcessorId);
		WMI_GET_PROP_INT(ProcessorType) //		SERIALIZE_BASIC(ProcessorType);
		WMI_GET_PROP_INT(Revision) //		SERIALIZE_BASIC(Revision);
		WMI_GET_PROP_STRING(Role) //		SERIALIZE_BASIC(Role);
		WMI_GET_PROP_STRING(SocketDesignation) //		SERIALIZE_BASIC(SocketDesignation);
		WMI_GET_PROP_STRING(Status) //		SERIALIZE_BASIC(Status);
		WMI_GET_PROP_INT(StatusInfo) //		SERIALIZE_BASIC(StatusInfo);
		WMI_GET_PROP_STRING(Stepping) //		SERIALIZE_BASIC(Stepping);
		WMI_GET_PROP_STRING(SystemCreationClassName) //		SERIALIZE_BASIC(SystemCreationClassName);
		WMI_GET_PROP_STRING(SystemName) //		SERIALIZE_BASIC(SystemName);
		WMI_GET_PROP_STRING(UniqueId) //		SERIALIZE_BASIC(UniqueId);
		WMI_GET_PROP_INT(UpgradeMethod) //		SERIALIZE_BASIC(UpgradeMethod);
		WMI_GET_PROP_STRING(Version) //		SERIALIZE_BASIC(Version);
		WMI_GET_PROP_INT(VoltageCaps) //		SERIALIZE_BASIC(VoltageCaps);
	}
}


VOID My_Win32_Product::NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars)
{
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		if (0){}
		WMI_GET_PROP_STRING(__Path) //			SERIALIZE_BASIC(__Path);
		WMI_GET_PROP_INT(AssignmentType) //			SERIALIZE_BASIC(AssignmentType);
		WMI_GET_PROP_STRING(Caption) //			SERIALIZE_BASIC(Caption);
		WMI_GET_PROP_STRING(Description) //			SERIALIZE_BASIC(Description);
		WMI_GET_PROP_STRING(IdentifyingNumber) //			SERIALIZE_BASIC(IdentifyingNumber);
		WMI_GET_PROP_STRING(InstallDate) //			SERIALIZE_BASIC(InstallDate);
		WMI_GET_PROP_SYSTEMTIME(InstallDate2) //			SERIALIZE_BASIC(InstallDate2);
		WMI_GET_PROP_STRING(InstallLocation) //			SERIALIZE_BASIC(InstallLocation);
		WMI_GET_PROP_INT(InstallState) //			SERIALIZE_BASIC(InstallState);
		WMI_GET_PROP_STRING(HelpLink) //			SERIALIZE_BASIC(HelpLink);
		WMI_GET_PROP_STRING(HelpTelephone) //			SERIALIZE_BASIC(HelpTelephone);
		WMI_GET_PROP_STRING(InstallSource) //			SERIALIZE_BASIC(InstallSource);
		WMI_GET_PROP_STRING(Language) //			SERIALIZE_BASIC(Language);
		WMI_GET_PROP_STRING(LocalPackage) //			SERIALIZE_BASIC(LocalPackage);
		WMI_GET_PROP_STRING(Name) //			SERIALIZE_BASIC(Name);
		WMI_GET_PROP_STRING(PackageCache) //			SERIALIZE_BASIC(PackageCache);
		WMI_GET_PROP_STRING(PackageCode) //			SERIALIZE_BASIC(PackageCode);
		WMI_GET_PROP_STRING(PackageName) //			SERIALIZE_BASIC(PackageName);
		WMI_GET_PROP_STRING(ProductID) //			SERIALIZE_BASIC(ProductID);
		WMI_GET_PROP_STRING(RegOwner) //			SERIALIZE_BASIC(RegOwner);
		WMI_GET_PROP_STRING(RegCompany) //			SERIALIZE_BASIC(RegCompany);
		WMI_GET_PROP_STRING(SKUNumber) //			SERIALIZE_BASIC(SKUNumber);
		WMI_GET_PROP_STRING(Transforms) //			SERIALIZE_BASIC(Transforms);
		WMI_GET_PROP_STRING(URLInfoAbout) //			SERIALIZE_BASIC(URLInfoAbout);
		WMI_GET_PROP_STRING(URLUpdateInfo) //			SERIALIZE_BASIC(URLUpdateInfo);
		WMI_GET_PROP_STRING(Vendor) //			SERIALIZE_BASIC(Vendor);
		WMI_GET_PROP_INT(WordCount) //			SERIALIZE_BASIC(WordCount);
		WMI_GET_PROP_STRING(Version) //			SERIALIZE_BASIC(Version);
	}
}

VOID My_Win32_Service::NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars)
{
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		if (0){}
		WMI_GET_PROP_STRING(__Path) //			SERIALIZE_BASIC(__Path);
			WMI_GET_PROP_INT(AcceptPause)	//		SERIALIZE_BASIC(AcceptPause);
		WMI_GET_PROP_INT(AcceptStop) //			SERIALIZE_BASIC(AcceptStop);
		WMI_GET_PROP_STRING(Caption) //			SERIALIZE_BASIC(Caption);
		WMI_GET_PROP_INT(CheckPoint) //			SERIALIZE_BASIC(CheckPoint);
		WMI_GET_PROP_STRING(CreationClassName) //			SERIALIZE_BASIC(CreationClassName);
		WMI_GET_PROP_STRING(Description) //			SERIALIZE_BASIC(Description);
		WMI_GET_PROP_INT(DesktopInteract) //			SERIALIZE_BASIC(DesktopInteract);
		WMI_GET_PROP_STRING(DisplayName) //			SERIALIZE_BASIC(DisplayName);
		WMI_GET_PROP_STRING(ErrorControl) //			SERIALIZE_BASIC(ErrorControl);
		WMI_GET_PROP_INT(ExitCode) //			SERIALIZE_BASIC(ExitCode);
		WMI_GET_PROP_SYSTEMTIME(InstallDate) //			SERIALIZE_BASIC(InstallDate);
		WMI_GET_PROP_STRING(Name) //			SERIALIZE_BASIC(Name);
		WMI_GET_PROP_STRING(PathName) //			SERIALIZE_BASIC(PathName);
		WMI_GET_PROP_INT(ProcessId) //			SERIALIZE_BASIC(ProcessId);
		WMI_GET_PROP_INT(ServiceSpecificExitCode) //			SERIALIZE_BASIC(ServiceSpecificExitCode);
		WMI_GET_PROP_STRING(ServiceType) //			SERIALIZE_BASIC(ServiceType);
		WMI_GET_PROP_INT(Started) //			SERIALIZE_BASIC(Started);
		WMI_GET_PROP_STRING(StartMode) //			SERIALIZE_BASIC(StartMode);
		WMI_GET_PROP_STRING(StartName) //			SERIALIZE_BASIC(StartName);
		WMI_GET_PROP_STRING(State) //			SERIALIZE_BASIC(State);
		WMI_GET_PROP_STRING(Status) //			SERIALIZE_BASIC(Status);
		WMI_GET_PROP_STRING(SystemCreationClassName) //			SERIALIZE_BASIC(SystemCreationClassName);
		WMI_GET_PROP_STRING(SystemName) //			SERIALIZE_BASIC(SystemName);
		WMI_GET_PROP_INT(TagId) //			SERIALIZE_BASIC(TagId);
		WMI_GET_PROP_INT(WaitHint) //			SERIALIZE_BASIC(WaitHint);
	}
}


CONST LPCTSTR My_System_Info::s_wmiRes = TEXT("Root\\CIMV2");

My_System_Info::My_System_Info(LPCTSTR machine, LPCTSTR user, LPCTSTR domain, LPCTSTR password) :
m_machine(machine),
m_user(user),
m_domain(domain),
m_password(password)
{

}

HRESULT My_System_Info::Retrieve()
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
		TEXT("Win32_OperatingSystem"),
		TEXT("Win32_ComputerSystem"),
		TEXT("Win32_Processor"),
		TEXT("Win32_NetworkAdapter"),
		TEXT("Win32_NetworkAdapterConfiguration"),
		TEXT("Win32_NetworkAdapterSetting"),
		TEXT("Win32_Product"),
		TEXT("Win32_Service")
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
					My_Win32_OperatingSystem obj;
					obj.NameValuesToFields(names, vars);
					OperatingSystems.push_back(obj);
				}
				break;

				case 1:
				{
					My_Win32_ComputerSystem obj;
					obj.NameValuesToFields(names, vars);
					ComputerSystems.push_back(obj);
				}
				break;

				case 2:
				{
					My_Win32_Processor obj;
					obj.NameValuesToFields(names, vars);
					Processors.push_back(obj);
				}
				break;

				case 3:
				{
					My_Win32_NetworkAdapter obj;
					obj.NameValuesToFields(names, vars);
					NetworkAdapters.push_back(obj);
				}
				break;

				case 4:
				{
					My_Win32_NetworkAdapterConfiguration obj;
					obj.NameValuesToFields(names, vars);
					NetworkAdapterConfigurations.push_back(obj);
				}
				break;

				case 5:
				{
					My_Win32_NetworkAdapterSetting obj;
					obj.NameValuesToFields(names, vars);
					NetworkAdapterSettings.push_back(obj);
				}
				break;

				case 6:
				{
					My_Win32_Product obj;
					obj.NameValuesToFields(names, vars);
					Products.push_back(obj);
				}
				break;

				case 7:
				{
					My_Win32_Service obj;
					obj.NameValuesToFields(names, vars);
					Services.push_back(obj);
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

