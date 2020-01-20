////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID My_Win32_OperatingSystem::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(__Path), __Path, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(BootDevice), BootDevice, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(BuildNumber), BuildNumber, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(BuildType), BuildType, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Caption), Caption, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CodeSet), CodeSet, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CountryCode), CountryCode, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CreationClassName), CreationClassName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CSCreationClassName), CSCreationClassName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CSDVersion), CSDVersion, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CSName), CSName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CurrentTimeZone), CurrentTimeZone, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DataExecutionPrevention_Available), DataExecutionPrevention_Available, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DataExecutionPrevention_32BitApplications), DataExecutionPrevention_32BitApplications, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DataExecutionPrevention_Drivers), DataExecutionPrevention_Drivers, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DataExecutionPrevention_SupportPolicy), DataExecutionPrevention_SupportPolicy, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Debug), Debug, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Description), Description, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Distributed), Distributed, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(EncryptionLevel), EncryptionLevel, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ForegroundApplicationBoost), ForegroundApplicationBoost, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(FreePhysicalMemory), FreePhysicalMemory, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(FreeSpaceInPagingFiles), FreeSpaceInPagingFiles, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(FreeVirtualMemory), FreeVirtualMemory, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(InstallDate), InstallDate, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(LargeSystemCache), LargeSystemCache, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(LastBootUpTime), LastBootUpTime, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(LocalSYSTEMTIME), LocalSYSTEMTIME, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Locale), Locale, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Manufacturer), Manufacturer, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(MaxNumberOfProcesses), MaxNumberOfProcesses, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(MaxProcessMemorySize), MaxProcessMemorySize, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(MUILanguages), MUILanguages, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Name), Name, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(NumberOfLicensedUsers), NumberOfLicensedUsers, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(NumberOfProcesses), NumberOfProcesses, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(NumberOfUsers), NumberOfUsers, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(OperatingSystemSKU), OperatingSystemSKU, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Organization), Organization, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(OSArchitecture), OSArchitecture, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(OSLanguage), OSLanguage, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(OSProductSuite), OSProductSuite, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(OSType), OSType, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(OtherTypeDescription), OtherTypeDescription, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PAEEnabled), PAEEnabled, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PlusProductID), PlusProductID, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PlusVersionNumber), PlusVersionNumber, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PortableOperatingSystem), PortableOperatingSystem, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Primary), Primary, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ProductType), ProductType, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(RegisteredUser), RegisteredUser, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SerialNumber), SerialNumber, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ServicePackMajorVersion), ServicePackMajorVersion, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ServicePackMinorVersion), ServicePackMinorVersion, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SizeStoredInPagingFiles), SizeStoredInPagingFiles, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Status), Status, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SuiteMask), SuiteMask, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SystemDevice), SystemDevice, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SystemDirectory), SystemDirectory, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SystemDrive), SystemDrive, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(TotalSwapSpaceSize), TotalSwapSpaceSize, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(TotalVirtualMemorySize), TotalVirtualMemorySize, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(TotalVisibleMemorySize), TotalVisibleMemorySize, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Version), Version, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(WindowsDirectory), WindowsDirectory, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID My_Win32_ComputerSystem::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(__Path), __Path, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(AdminPasswordStatus), AdminPasswordStatus, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(AutomaticManagedPagefile), AutomaticManagedPagefile, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(AutomaticResetBootOption), AutomaticResetBootOption, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(AutomaticResetCapability), AutomaticResetCapability, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(BootOptionOnLimit), BootOptionOnLimit, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(BootOptionOnWatchDog), BootOptionOnWatchDog, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(BootROMSupported), BootROMSupported, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(BootupState), BootupState, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Caption), Caption, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ChassisBootupState), ChassisBootupState, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CreationClassName), CreationClassName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CurrentTimeZone), CurrentTimeZone, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DaylightInEffect), DaylightInEffect, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Description), Description, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DNSHostName), DNSHostName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Domain), Domain, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DomainRole), DomainRole, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(EnableDaylightSavingsTime), EnableDaylightSavingsTime, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(FrontPanelResetStatus), FrontPanelResetStatus, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(InfraredSupported), InfraredSupported, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(InitialLoadInfo), InitialLoadInfo, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(InstallDate), InstallDate, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(KeyboardPasswordStatus), KeyboardPasswordStatus, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(LastLoadInfo), LastLoadInfo, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Manufacturer), Manufacturer, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Model), Model, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Name), Name, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(NameFormat), NameFormat, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(NetworkServerModeEnabled), NetworkServerModeEnabled, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(NumberOfLogicalProcessors), NumberOfLogicalProcessors, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(NumberOfProcessors), NumberOfProcessors, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(OEMLogoBitmap), OEMLogoBitmap, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(OEMCStringArray), OEMCStringArray, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PartOfDomain), PartOfDomain, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PauseAfterReset), PauseAfterReset, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PCSystemType), PCSystemType, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(PowerManagementCapabilities), PowerManagementCapabilities, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PowerManagementSupported), PowerManagementSupported, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PowerOnPasswordStatus), PowerOnPasswordStatus, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PowerState), PowerState, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PowerSupplyState), PowerSupplyState, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PrimaryOwnerContact), PrimaryOwnerContact, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PrimaryOwnerName), PrimaryOwnerName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ResetCapability), ResetCapability, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ResetCount), ResetCount, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ResetLimit), ResetLimit, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(Roles), Roles, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Status), Status, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(SupportContactDescription), SupportContactDescription, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SystemStartupDelay), SystemStartupDelay, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(SystemStartupOptions), SystemStartupOptions, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SystemStartupSetting), SystemStartupSetting, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SystemType), SystemType, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ThermalState), ThermalState, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(TotalPhysicalMemory), TotalPhysicalMemory, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(UserName), UserName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(WakeUpType), WakeUpType, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Workgroup), Workgroup, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID My_Win32_Processor::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(__Path), __Path, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(AddressWidth), AddressWidth, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Architecture), Architecture, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Availability), Availability, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Caption), Caption, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ConfigManagerErrorCode), ConfigManagerErrorCode, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ConfigManagerUserConfig), ConfigManagerUserConfig, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CpuStatus), CpuStatus, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CreationClassName), CreationClassName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CurrentClockSpeed), CurrentClockSpeed, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CurrentVoltage), CurrentVoltage, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DataWidth), DataWidth, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Description), Description, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DeviceID), DeviceID, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ErrorCleared), ErrorCleared, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ErrorDescription), ErrorDescription, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ExtClock), ExtClock, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Family), Family, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(InstallDate), InstallDate, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(L2CacheSize), L2CacheSize, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(L2CacheSpeed), L2CacheSpeed, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(L3CacheSize), L3CacheSize, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(L3CacheSpeed), L3CacheSpeed, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(LastErrorCode), LastErrorCode, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Level), Level, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(LoadPercentage), LoadPercentage, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Manufacturer), Manufacturer, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(MaxClockSpeed), MaxClockSpeed, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Name), Name, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(NumberOfCores), NumberOfCores, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(NumberOfLogicalProcessors), NumberOfLogicalProcessors, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(OtherFamilyDescription), OtherFamilyDescription, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PNPDeviceID), PNPDeviceID, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(PowerManagementCapabilities), PowerManagementCapabilities, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PowerManagementSupported), PowerManagementSupported, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ProcessorId), ProcessorId, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ProcessorType), ProcessorType, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Revision), Revision, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Role), Role, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SocketDesignation), SocketDesignation, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Status), Status, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(StatusInfo), StatusInfo, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Stepping), Stepping, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SystemCreationClassName), SystemCreationClassName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SystemName), SystemName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(UniqueId), UniqueId, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(UpgradeMethod), UpgradeMethod, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Version), Version, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(VoltageCaps), VoltageCaps, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID My_Win32_Product::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(__Path), __Path, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(AssignmentType), AssignmentType, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Caption), Caption, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Description), Description, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(IdentifyingNumber), IdentifyingNumber, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(InstallDate), InstallDate, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(InstallDate2), InstallDate2, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(InstallLocation), InstallLocation, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(InstallState), InstallState, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(HelpLink), HelpLink, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(HelpTelephone), HelpTelephone, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(InstallSource), InstallSource, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Language), Language, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(LocalPackage), LocalPackage, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Name), Name, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PackageCache), PackageCache, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PackageCode), PackageCode, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PackageName), PackageName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ProductID), ProductID, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(RegOwner), RegOwner, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(RegCompany), RegCompany, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SKUNumber), SKUNumber, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Transforms), Transforms, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(URLInfoAbout), URLInfoAbout, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(URLUpdateInfo), URLUpdateInfo, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Vendor), Vendor, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(WordCount), WordCount, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Version), Version, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID My_Win32_Service::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(__Path), __Path, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(AcceptPause), AcceptPause, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(AcceptStop), AcceptStop, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Caption), Caption, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CheckPoint), CheckPoint, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CreationClassName), CreationClassName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Description), Description, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DesktopInteract), DesktopInteract, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DisplayName), DisplayName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ErrorControl), ErrorControl, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ExitCode), ExitCode, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(InstallDate), InstallDate, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Name), Name, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PathName), PathName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ProcessId), ProcessId, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ServiceSpecificExitCode), ServiceSpecificExitCode, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ServiceType), ServiceType, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Started), Started, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(StartMode), StartMode, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(StartName), StartName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(State), State, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Status), Status, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SystemCreationClassName), SystemCreationClassName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SystemName), SystemName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(TagId), TagId, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(WaitHint), WaitHint, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID My_System_Info::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	/*VectorStructToElement*/ContainerStructToElement(FIELD_NAME(OperatingSystems), OperatingSystems, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorStructToElement*/ContainerStructToElement(FIELD_NAME(ComputerSystems), ComputerSystems, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorStructToElement*/ContainerStructToElement(FIELD_NAME(Processors), Processors, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorStructToElement*/ContainerStructToElement(FIELD_NAME(NetworkAdapters), NetworkAdapters, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorStructToElement*/ContainerStructToElement(FIELD_NAME(NetworkAdapterConfigurations), NetworkAdapterConfigurations, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorStructToElement*/ContainerStructToElement(FIELD_NAME(NetworkAdapterSettings), NetworkAdapterSettings, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorStructToElement*/ContainerStructToElement(FIELD_NAME(Products), Products, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorStructToElement*/ContainerStructToElement(FIELD_NAME(Services), Services, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
VOID My_Win32_OperatingSystem::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(__Path)))
		{
			ElementToValue(FIELD_NAME(__Path), __Path, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(BootDevice)))
		{
			ElementToValue(FIELD_NAME(BootDevice), BootDevice, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(BuildNumber)))
		{
			ElementToValue(FIELD_NAME(BuildNumber), BuildNumber, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(BuildType)))
		{
			ElementToValue(FIELD_NAME(BuildType), BuildType, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Caption)))
		{
			ElementToValue(FIELD_NAME(Caption), Caption, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CodeSet)))
		{
			ElementToValue(FIELD_NAME(CodeSet), CodeSet, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CountryCode)))
		{
			ElementToValue(FIELD_NAME(CountryCode), CountryCode, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CreationClassName)))
		{
			ElementToValue(FIELD_NAME(CreationClassName), CreationClassName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CSCreationClassName)))
		{
			ElementToValue(FIELD_NAME(CSCreationClassName), CSCreationClassName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CSDVersion)))
		{
			ElementToValue(FIELD_NAME(CSDVersion), CSDVersion, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CSName)))
		{
			ElementToValue(FIELD_NAME(CSName), CSName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CurrentTimeZone)))
		{
			ElementToValue(FIELD_NAME(CurrentTimeZone), CurrentTimeZone, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DataExecutionPrevention_Available)))
		{
			ElementToValue(FIELD_NAME(DataExecutionPrevention_Available), DataExecutionPrevention_Available, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DataExecutionPrevention_32BitApplications)))
		{
			ElementToValue(FIELD_NAME(DataExecutionPrevention_32BitApplications), DataExecutionPrevention_32BitApplications, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DataExecutionPrevention_Drivers)))
		{
			ElementToValue(FIELD_NAME(DataExecutionPrevention_Drivers), DataExecutionPrevention_Drivers, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DataExecutionPrevention_SupportPolicy)))
		{
			ElementToValue(FIELD_NAME(DataExecutionPrevention_SupportPolicy), DataExecutionPrevention_SupportPolicy, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Debug)))
		{
			ElementToValue(FIELD_NAME(Debug), Debug, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Description)))
		{
			ElementToValue(FIELD_NAME(Description), Description, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Distributed)))
		{
			ElementToValue(FIELD_NAME(Distributed), Distributed, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(EncryptionLevel)))
		{
			ElementToValue(FIELD_NAME(EncryptionLevel), EncryptionLevel, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ForegroundApplicationBoost)))
		{
			ElementToValue(FIELD_NAME(ForegroundApplicationBoost), ForegroundApplicationBoost, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(FreePhysicalMemory)))
		{
			ElementToValue(FIELD_NAME(FreePhysicalMemory), FreePhysicalMemory, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(FreeSpaceInPagingFiles)))
		{
			ElementToValue(FIELD_NAME(FreeSpaceInPagingFiles), FreeSpaceInPagingFiles, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(FreeVirtualMemory)))
		{
			ElementToValue(FIELD_NAME(FreeVirtualMemory), FreeVirtualMemory, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(InstallDate)))
		{
			ElementToValue(FIELD_NAME(InstallDate), InstallDate, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(LargeSystemCache)))
		{
			ElementToValue(FIELD_NAME(LargeSystemCache), LargeSystemCache, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(LastBootUpTime)))
		{
			ElementToValue(FIELD_NAME(LastBootUpTime), LastBootUpTime, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(LocalSYSTEMTIME)))
		{
			ElementToValue(FIELD_NAME(LocalSYSTEMTIME), LocalSYSTEMTIME, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Locale)))
		{
			ElementToValue(FIELD_NAME(Locale), Locale, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Manufacturer)))
		{
			ElementToValue(FIELD_NAME(Manufacturer), Manufacturer, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(MaxNumberOfProcesses)))
		{
			ElementToValue(FIELD_NAME(MaxNumberOfProcesses), MaxNumberOfProcesses, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(MaxProcessMemorySize)))
		{
			ElementToValue(FIELD_NAME(MaxProcessMemorySize), MaxProcessMemorySize, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(MUILanguages)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(MUILanguages), MUILanguages, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Name)))
		{
			ElementToValue(FIELD_NAME(Name), Name, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(NumberOfLicensedUsers)))
		{
			ElementToValue(FIELD_NAME(NumberOfLicensedUsers), NumberOfLicensedUsers, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(NumberOfProcesses)))
		{
			ElementToValue(FIELD_NAME(NumberOfProcesses), NumberOfProcesses, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(NumberOfUsers)))
		{
			ElementToValue(FIELD_NAME(NumberOfUsers), NumberOfUsers, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(OperatingSystemSKU)))
		{
			ElementToValue(FIELD_NAME(OperatingSystemSKU), OperatingSystemSKU, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Organization)))
		{
			ElementToValue(FIELD_NAME(Organization), Organization, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(OSArchitecture)))
		{
			ElementToValue(FIELD_NAME(OSArchitecture), OSArchitecture, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(OSLanguage)))
		{
			ElementToValue(FIELD_NAME(OSLanguage), OSLanguage, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(OSProductSuite)))
		{
			ElementToValue(FIELD_NAME(OSProductSuite), OSProductSuite, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(OSType)))
		{
			ElementToValue(FIELD_NAME(OSType), OSType, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(OtherTypeDescription)))
		{
			ElementToValue(FIELD_NAME(OtherTypeDescription), OtherTypeDescription, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PAEEnabled)))
		{
			ElementToValue(FIELD_NAME(PAEEnabled), PAEEnabled, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PlusProductID)))
		{
			ElementToValue(FIELD_NAME(PlusProductID), PlusProductID, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PlusVersionNumber)))
		{
			ElementToValue(FIELD_NAME(PlusVersionNumber), PlusVersionNumber, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PortableOperatingSystem)))
		{
			ElementToValue(FIELD_NAME(PortableOperatingSystem), PortableOperatingSystem, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Primary)))
		{
			ElementToValue(FIELD_NAME(Primary), Primary, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ProductType)))
		{
			ElementToValue(FIELD_NAME(ProductType), ProductType, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(RegisteredUser)))
		{
			ElementToValue(FIELD_NAME(RegisteredUser), RegisteredUser, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SerialNumber)))
		{
			ElementToValue(FIELD_NAME(SerialNumber), SerialNumber, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ServicePackMajorVersion)))
		{
			ElementToValue(FIELD_NAME(ServicePackMajorVersion), ServicePackMajorVersion, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ServicePackMinorVersion)))
		{
			ElementToValue(FIELD_NAME(ServicePackMinorVersion), ServicePackMinorVersion, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SizeStoredInPagingFiles)))
		{
			ElementToValue(FIELD_NAME(SizeStoredInPagingFiles), SizeStoredInPagingFiles, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Status)))
		{
			ElementToValue(FIELD_NAME(Status), Status, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SuiteMask)))
		{
			ElementToValue(FIELD_NAME(SuiteMask), SuiteMask, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SystemDevice)))
		{
			ElementToValue(FIELD_NAME(SystemDevice), SystemDevice, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SystemDirectory)))
		{
			ElementToValue(FIELD_NAME(SystemDirectory), SystemDirectory, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SystemDrive)))
		{
			ElementToValue(FIELD_NAME(SystemDrive), SystemDrive, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(TotalSwapSpaceSize)))
		{
			ElementToValue(FIELD_NAME(TotalSwapSpaceSize), TotalSwapSpaceSize, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(TotalVirtualMemorySize)))
		{
			ElementToValue(FIELD_NAME(TotalVirtualMemorySize), TotalVirtualMemorySize, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(TotalVisibleMemorySize)))
		{
			ElementToValue(FIELD_NAME(TotalVisibleMemorySize), TotalVisibleMemorySize, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Version)))
		{
			ElementToValue(FIELD_NAME(Version), Version, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(WindowsDirectory)))
		{
			ElementToValue(FIELD_NAME(WindowsDirectory), WindowsDirectory, Child_jfs654);
		}
	}
}
VOID My_Win32_OperatingSystem::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT My_Win32_OperatingSystem::FromString( LPCTSTR RootName, CONST CString & strXml )
{
	XmlElement xmlElement;
	INT Start = 0;
	HRESULT hr = S_OK;
	LONG xml_ele_count = xmlElement.FromString(strXml, Start);
	if (xml_ele_count > 0)
	{
		FromElement(RootName, xmlElement);
	}
	else
	{
		hr = HRESULT_FROM_WIN32(ERROR_BAD_FORMAT);
	}
	return hr;
}

HRESULT My_Win32_OperatingSystem::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CString strXml;
	ToString(RootName, strXml);
	static CONST BYTE UTF16_TAG[] = {0xff, 0xfe};
	ULONG NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(sizeof(UTF16_TAG), &NumberOfBytesWritten, UTF16_TAG);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(strXml.GetLength() * sizeof(TCHAR), &NumberOfBytesWritten, (LPCBYTE)strXml.GetString());
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT My_Win32_OperatingSystem::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	LONGLONG FileSize = 0;
	hr = pIFileDevice->GetFileSize(&FileSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	CString strXML;
	INT CharCount = (INT)FileSize / sizeof(TCHAR) + 2;
	LPTSTR pBuffer = strXML.GetBuffer(CharCount);
	pBuffer[CharCount - 1] = 0;
	pBuffer[CharCount - 2] = 0;
	ULONG NumberOfBytesRead = 0;
	hr = pIFileDevice->Read((ULONG)FileSize, &NumberOfBytesRead, (LPBYTE)pBuffer);
	strXML.ReleaseBuffer();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = FromString(RootName, strXML);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FromString"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT My_Win32_OperatingSystem::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_WRITE, 0, CREATE_ALWAYS, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = ToStream(RootName, spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ToStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT My_Win32_OperatingSystem::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = FromStream(RootName, spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FromStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
VOID My_Win32_ComputerSystem::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(__Path)))
		{
			ElementToValue(FIELD_NAME(__Path), __Path, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(AdminPasswordStatus)))
		{
			ElementToValue(FIELD_NAME(AdminPasswordStatus), AdminPasswordStatus, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(AutomaticManagedPagefile)))
		{
			ElementToValue(FIELD_NAME(AutomaticManagedPagefile), AutomaticManagedPagefile, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(AutomaticResetBootOption)))
		{
			ElementToValue(FIELD_NAME(AutomaticResetBootOption), AutomaticResetBootOption, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(AutomaticResetCapability)))
		{
			ElementToValue(FIELD_NAME(AutomaticResetCapability), AutomaticResetCapability, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(BootOptionOnLimit)))
		{
			ElementToValue(FIELD_NAME(BootOptionOnLimit), BootOptionOnLimit, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(BootOptionOnWatchDog)))
		{
			ElementToValue(FIELD_NAME(BootOptionOnWatchDog), BootOptionOnWatchDog, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(BootROMSupported)))
		{
			ElementToValue(FIELD_NAME(BootROMSupported), BootROMSupported, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(BootupState)))
		{
			ElementToValue(FIELD_NAME(BootupState), BootupState, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Caption)))
		{
			ElementToValue(FIELD_NAME(Caption), Caption, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ChassisBootupState)))
		{
			ElementToValue(FIELD_NAME(ChassisBootupState), ChassisBootupState, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CreationClassName)))
		{
			ElementToValue(FIELD_NAME(CreationClassName), CreationClassName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CurrentTimeZone)))
		{
			ElementToValue(FIELD_NAME(CurrentTimeZone), CurrentTimeZone, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DaylightInEffect)))
		{
			ElementToValue(FIELD_NAME(DaylightInEffect), DaylightInEffect, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Description)))
		{
			ElementToValue(FIELD_NAME(Description), Description, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DNSHostName)))
		{
			ElementToValue(FIELD_NAME(DNSHostName), DNSHostName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Domain)))
		{
			ElementToValue(FIELD_NAME(Domain), Domain, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DomainRole)))
		{
			ElementToValue(FIELD_NAME(DomainRole), DomainRole, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(EnableDaylightSavingsTime)))
		{
			ElementToValue(FIELD_NAME(EnableDaylightSavingsTime), EnableDaylightSavingsTime, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(FrontPanelResetStatus)))
		{
			ElementToValue(FIELD_NAME(FrontPanelResetStatus), FrontPanelResetStatus, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(InfraredSupported)))
		{
			ElementToValue(FIELD_NAME(InfraredSupported), InfraredSupported, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(InitialLoadInfo)))
		{
			ElementToValue(FIELD_NAME(InitialLoadInfo), InitialLoadInfo, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(InstallDate)))
		{
			ElementToValue(FIELD_NAME(InstallDate), InstallDate, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(KeyboardPasswordStatus)))
		{
			ElementToValue(FIELD_NAME(KeyboardPasswordStatus), KeyboardPasswordStatus, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(LastLoadInfo)))
		{
			ElementToValue(FIELD_NAME(LastLoadInfo), LastLoadInfo, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Manufacturer)))
		{
			ElementToValue(FIELD_NAME(Manufacturer), Manufacturer, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Model)))
		{
			ElementToValue(FIELD_NAME(Model), Model, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Name)))
		{
			ElementToValue(FIELD_NAME(Name), Name, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(NameFormat)))
		{
			ElementToValue(FIELD_NAME(NameFormat), NameFormat, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(NetworkServerModeEnabled)))
		{
			ElementToValue(FIELD_NAME(NetworkServerModeEnabled), NetworkServerModeEnabled, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(NumberOfLogicalProcessors)))
		{
			ElementToValue(FIELD_NAME(NumberOfLogicalProcessors), NumberOfLogicalProcessors, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(NumberOfProcessors)))
		{
			ElementToValue(FIELD_NAME(NumberOfProcessors), NumberOfProcessors, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(OEMLogoBitmap)))
		{
			ElementToValue(FIELD_NAME(OEMLogoBitmap), OEMLogoBitmap, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(OEMCStringArray)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(OEMCStringArray), OEMCStringArray, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PartOfDomain)))
		{
			ElementToValue(FIELD_NAME(PartOfDomain), PartOfDomain, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PauseAfterReset)))
		{
			ElementToValue(FIELD_NAME(PauseAfterReset), PauseAfterReset, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PCSystemType)))
		{
			ElementToValue(FIELD_NAME(PCSystemType), PCSystemType, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PowerManagementCapabilities)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(PowerManagementCapabilities), PowerManagementCapabilities, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PowerManagementSupported)))
		{
			ElementToValue(FIELD_NAME(PowerManagementSupported), PowerManagementSupported, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PowerOnPasswordStatus)))
		{
			ElementToValue(FIELD_NAME(PowerOnPasswordStatus), PowerOnPasswordStatus, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PowerState)))
		{
			ElementToValue(FIELD_NAME(PowerState), PowerState, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PowerSupplyState)))
		{
			ElementToValue(FIELD_NAME(PowerSupplyState), PowerSupplyState, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PrimaryOwnerContact)))
		{
			ElementToValue(FIELD_NAME(PrimaryOwnerContact), PrimaryOwnerContact, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PrimaryOwnerName)))
		{
			ElementToValue(FIELD_NAME(PrimaryOwnerName), PrimaryOwnerName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ResetCapability)))
		{
			ElementToValue(FIELD_NAME(ResetCapability), ResetCapability, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ResetCount)))
		{
			ElementToValue(FIELD_NAME(ResetCount), ResetCount, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ResetLimit)))
		{
			ElementToValue(FIELD_NAME(ResetLimit), ResetLimit, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Roles)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(Roles), Roles, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Status)))
		{
			ElementToValue(FIELD_NAME(Status), Status, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SupportContactDescription)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(SupportContactDescription), SupportContactDescription, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SystemStartupDelay)))
		{
			ElementToValue(FIELD_NAME(SystemStartupDelay), SystemStartupDelay, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SystemStartupOptions)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(SystemStartupOptions), SystemStartupOptions, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SystemStartupSetting)))
		{
			ElementToValue(FIELD_NAME(SystemStartupSetting), SystemStartupSetting, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SystemType)))
		{
			ElementToValue(FIELD_NAME(SystemType), SystemType, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ThermalState)))
		{
			ElementToValue(FIELD_NAME(ThermalState), ThermalState, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(TotalPhysicalMemory)))
		{
			ElementToValue(FIELD_NAME(TotalPhysicalMemory), TotalPhysicalMemory, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(UserName)))
		{
			ElementToValue(FIELD_NAME(UserName), UserName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(WakeUpType)))
		{
			ElementToValue(FIELD_NAME(WakeUpType), WakeUpType, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Workgroup)))
		{
			ElementToValue(FIELD_NAME(Workgroup), Workgroup, Child_jfs654);
		}
	}
}
VOID My_Win32_ComputerSystem::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT My_Win32_ComputerSystem::FromString( LPCTSTR RootName, CONST CString & strXml )
{
	XmlElement xmlElement;
	INT Start = 0;
	HRESULT hr = S_OK;
	LONG xml_ele_count = xmlElement.FromString(strXml, Start);
	if (xml_ele_count > 0)
	{
		FromElement(RootName, xmlElement);
	}
	else
	{
		hr = HRESULT_FROM_WIN32(ERROR_BAD_FORMAT);
	}
	return hr;
}

HRESULT My_Win32_ComputerSystem::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CString strXml;
	ToString(RootName, strXml);
	static CONST BYTE UTF16_TAG[] = {0xff, 0xfe};
	ULONG NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(sizeof(UTF16_TAG), &NumberOfBytesWritten, UTF16_TAG);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(strXml.GetLength() * sizeof(TCHAR), &NumberOfBytesWritten, (LPCBYTE)strXml.GetString());
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT My_Win32_ComputerSystem::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	LONGLONG FileSize = 0;
	hr = pIFileDevice->GetFileSize(&FileSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	CString strXML;
	INT CharCount = (INT)FileSize / sizeof(TCHAR) + 2;
	LPTSTR pBuffer = strXML.GetBuffer(CharCount);
	pBuffer[CharCount - 1] = 0;
	pBuffer[CharCount - 2] = 0;
	ULONG NumberOfBytesRead = 0;
	hr = pIFileDevice->Read((ULONG)FileSize, &NumberOfBytesRead, (LPBYTE)pBuffer);
	strXML.ReleaseBuffer();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = FromString(RootName, strXML);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FromString"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT My_Win32_ComputerSystem::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_WRITE, 0, CREATE_ALWAYS, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = ToStream(RootName, spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ToStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT My_Win32_ComputerSystem::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = FromStream(RootName, spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FromStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
VOID My_Win32_Processor::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(__Path)))
		{
			ElementToValue(FIELD_NAME(__Path), __Path, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(AddressWidth)))
		{
			ElementToValue(FIELD_NAME(AddressWidth), AddressWidth, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Architecture)))
		{
			ElementToValue(FIELD_NAME(Architecture), Architecture, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Availability)))
		{
			ElementToValue(FIELD_NAME(Availability), Availability, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Caption)))
		{
			ElementToValue(FIELD_NAME(Caption), Caption, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ConfigManagerErrorCode)))
		{
			ElementToValue(FIELD_NAME(ConfigManagerErrorCode), ConfigManagerErrorCode, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ConfigManagerUserConfig)))
		{
			ElementToValue(FIELD_NAME(ConfigManagerUserConfig), ConfigManagerUserConfig, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CpuStatus)))
		{
			ElementToValue(FIELD_NAME(CpuStatus), CpuStatus, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CreationClassName)))
		{
			ElementToValue(FIELD_NAME(CreationClassName), CreationClassName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CurrentClockSpeed)))
		{
			ElementToValue(FIELD_NAME(CurrentClockSpeed), CurrentClockSpeed, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CurrentVoltage)))
		{
			ElementToValue(FIELD_NAME(CurrentVoltage), CurrentVoltage, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DataWidth)))
		{
			ElementToValue(FIELD_NAME(DataWidth), DataWidth, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Description)))
		{
			ElementToValue(FIELD_NAME(Description), Description, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DeviceID)))
		{
			ElementToValue(FIELD_NAME(DeviceID), DeviceID, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ErrorCleared)))
		{
			ElementToValue(FIELD_NAME(ErrorCleared), ErrorCleared, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ErrorDescription)))
		{
			ElementToValue(FIELD_NAME(ErrorDescription), ErrorDescription, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ExtClock)))
		{
			ElementToValue(FIELD_NAME(ExtClock), ExtClock, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Family)))
		{
			ElementToValue(FIELD_NAME(Family), Family, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(InstallDate)))
		{
			ElementToValue(FIELD_NAME(InstallDate), InstallDate, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(L2CacheSize)))
		{
			ElementToValue(FIELD_NAME(L2CacheSize), L2CacheSize, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(L2CacheSpeed)))
		{
			ElementToValue(FIELD_NAME(L2CacheSpeed), L2CacheSpeed, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(L3CacheSize)))
		{
			ElementToValue(FIELD_NAME(L3CacheSize), L3CacheSize, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(L3CacheSpeed)))
		{
			ElementToValue(FIELD_NAME(L3CacheSpeed), L3CacheSpeed, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(LastErrorCode)))
		{
			ElementToValue(FIELD_NAME(LastErrorCode), LastErrorCode, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Level)))
		{
			ElementToValue(FIELD_NAME(Level), Level, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(LoadPercentage)))
		{
			ElementToValue(FIELD_NAME(LoadPercentage), LoadPercentage, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Manufacturer)))
		{
			ElementToValue(FIELD_NAME(Manufacturer), Manufacturer, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(MaxClockSpeed)))
		{
			ElementToValue(FIELD_NAME(MaxClockSpeed), MaxClockSpeed, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Name)))
		{
			ElementToValue(FIELD_NAME(Name), Name, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(NumberOfCores)))
		{
			ElementToValue(FIELD_NAME(NumberOfCores), NumberOfCores, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(NumberOfLogicalProcessors)))
		{
			ElementToValue(FIELD_NAME(NumberOfLogicalProcessors), NumberOfLogicalProcessors, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(OtherFamilyDescription)))
		{
			ElementToValue(FIELD_NAME(OtherFamilyDescription), OtherFamilyDescription, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PNPDeviceID)))
		{
			ElementToValue(FIELD_NAME(PNPDeviceID), PNPDeviceID, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PowerManagementCapabilities)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(PowerManagementCapabilities), PowerManagementCapabilities, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PowerManagementSupported)))
		{
			ElementToValue(FIELD_NAME(PowerManagementSupported), PowerManagementSupported, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ProcessorId)))
		{
			ElementToValue(FIELD_NAME(ProcessorId), ProcessorId, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ProcessorType)))
		{
			ElementToValue(FIELD_NAME(ProcessorType), ProcessorType, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Revision)))
		{
			ElementToValue(FIELD_NAME(Revision), Revision, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Role)))
		{
			ElementToValue(FIELD_NAME(Role), Role, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SocketDesignation)))
		{
			ElementToValue(FIELD_NAME(SocketDesignation), SocketDesignation, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Status)))
		{
			ElementToValue(FIELD_NAME(Status), Status, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(StatusInfo)))
		{
			ElementToValue(FIELD_NAME(StatusInfo), StatusInfo, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Stepping)))
		{
			ElementToValue(FIELD_NAME(Stepping), Stepping, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SystemCreationClassName)))
		{
			ElementToValue(FIELD_NAME(SystemCreationClassName), SystemCreationClassName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SystemName)))
		{
			ElementToValue(FIELD_NAME(SystemName), SystemName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(UniqueId)))
		{
			ElementToValue(FIELD_NAME(UniqueId), UniqueId, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(UpgradeMethod)))
		{
			ElementToValue(FIELD_NAME(UpgradeMethod), UpgradeMethod, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Version)))
		{
			ElementToValue(FIELD_NAME(Version), Version, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(VoltageCaps)))
		{
			ElementToValue(FIELD_NAME(VoltageCaps), VoltageCaps, Child_jfs654);
		}
	}
}
VOID My_Win32_Processor::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT My_Win32_Processor::FromString( LPCTSTR RootName, CONST CString & strXml )
{
	XmlElement xmlElement;
	INT Start = 0;
	HRESULT hr = S_OK;
	LONG xml_ele_count = xmlElement.FromString(strXml, Start);
	if (xml_ele_count > 0)
	{
		FromElement(RootName, xmlElement);
	}
	else
	{
		hr = HRESULT_FROM_WIN32(ERROR_BAD_FORMAT);
	}
	return hr;
}

HRESULT My_Win32_Processor::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CString strXml;
	ToString(RootName, strXml);
	static CONST BYTE UTF16_TAG[] = {0xff, 0xfe};
	ULONG NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(sizeof(UTF16_TAG), &NumberOfBytesWritten, UTF16_TAG);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(strXml.GetLength() * sizeof(TCHAR), &NumberOfBytesWritten, (LPCBYTE)strXml.GetString());
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT My_Win32_Processor::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	LONGLONG FileSize = 0;
	hr = pIFileDevice->GetFileSize(&FileSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	CString strXML;
	INT CharCount = (INT)FileSize / sizeof(TCHAR) + 2;
	LPTSTR pBuffer = strXML.GetBuffer(CharCount);
	pBuffer[CharCount - 1] = 0;
	pBuffer[CharCount - 2] = 0;
	ULONG NumberOfBytesRead = 0;
	hr = pIFileDevice->Read((ULONG)FileSize, &NumberOfBytesRead, (LPBYTE)pBuffer);
	strXML.ReleaseBuffer();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = FromString(RootName, strXML);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FromString"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT My_Win32_Processor::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_WRITE, 0, CREATE_ALWAYS, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = ToStream(RootName, spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ToStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT My_Win32_Processor::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = FromStream(RootName, spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FromStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
VOID My_Win32_Product::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(__Path)))
		{
			ElementToValue(FIELD_NAME(__Path), __Path, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(AssignmentType)))
		{
			ElementToValue(FIELD_NAME(AssignmentType), AssignmentType, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Caption)))
		{
			ElementToValue(FIELD_NAME(Caption), Caption, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Description)))
		{
			ElementToValue(FIELD_NAME(Description), Description, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(IdentifyingNumber)))
		{
			ElementToValue(FIELD_NAME(IdentifyingNumber), IdentifyingNumber, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(InstallDate)))
		{
			ElementToValue(FIELD_NAME(InstallDate), InstallDate, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(InstallDate2)))
		{
			ElementToValue(FIELD_NAME(InstallDate2), InstallDate2, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(InstallLocation)))
		{
			ElementToValue(FIELD_NAME(InstallLocation), InstallLocation, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(InstallState)))
		{
			ElementToValue(FIELD_NAME(InstallState), InstallState, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(HelpLink)))
		{
			ElementToValue(FIELD_NAME(HelpLink), HelpLink, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(HelpTelephone)))
		{
			ElementToValue(FIELD_NAME(HelpTelephone), HelpTelephone, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(InstallSource)))
		{
			ElementToValue(FIELD_NAME(InstallSource), InstallSource, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Language)))
		{
			ElementToValue(FIELD_NAME(Language), Language, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(LocalPackage)))
		{
			ElementToValue(FIELD_NAME(LocalPackage), LocalPackage, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Name)))
		{
			ElementToValue(FIELD_NAME(Name), Name, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PackageCache)))
		{
			ElementToValue(FIELD_NAME(PackageCache), PackageCache, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PackageCode)))
		{
			ElementToValue(FIELD_NAME(PackageCode), PackageCode, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PackageName)))
		{
			ElementToValue(FIELD_NAME(PackageName), PackageName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ProductID)))
		{
			ElementToValue(FIELD_NAME(ProductID), ProductID, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(RegOwner)))
		{
			ElementToValue(FIELD_NAME(RegOwner), RegOwner, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(RegCompany)))
		{
			ElementToValue(FIELD_NAME(RegCompany), RegCompany, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SKUNumber)))
		{
			ElementToValue(FIELD_NAME(SKUNumber), SKUNumber, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Transforms)))
		{
			ElementToValue(FIELD_NAME(Transforms), Transforms, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(URLInfoAbout)))
		{
			ElementToValue(FIELD_NAME(URLInfoAbout), URLInfoAbout, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(URLUpdateInfo)))
		{
			ElementToValue(FIELD_NAME(URLUpdateInfo), URLUpdateInfo, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Vendor)))
		{
			ElementToValue(FIELD_NAME(Vendor), Vendor, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(WordCount)))
		{
			ElementToValue(FIELD_NAME(WordCount), WordCount, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Version)))
		{
			ElementToValue(FIELD_NAME(Version), Version, Child_jfs654);
		}
	}
}
VOID My_Win32_Product::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT My_Win32_Product::FromString( LPCTSTR RootName, CONST CString & strXml )
{
	XmlElement xmlElement;
	INT Start = 0;
	HRESULT hr = S_OK;
	LONG xml_ele_count = xmlElement.FromString(strXml, Start);
	if (xml_ele_count > 0)
	{
		FromElement(RootName, xmlElement);
	}
	else
	{
		hr = HRESULT_FROM_WIN32(ERROR_BAD_FORMAT);
	}
	return hr;
}

HRESULT My_Win32_Product::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CString strXml;
	ToString(RootName, strXml);
	static CONST BYTE UTF16_TAG[] = {0xff, 0xfe};
	ULONG NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(sizeof(UTF16_TAG), &NumberOfBytesWritten, UTF16_TAG);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(strXml.GetLength() * sizeof(TCHAR), &NumberOfBytesWritten, (LPCBYTE)strXml.GetString());
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT My_Win32_Product::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	LONGLONG FileSize = 0;
	hr = pIFileDevice->GetFileSize(&FileSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	CString strXML;
	INT CharCount = (INT)FileSize / sizeof(TCHAR) + 2;
	LPTSTR pBuffer = strXML.GetBuffer(CharCount);
	pBuffer[CharCount - 1] = 0;
	pBuffer[CharCount - 2] = 0;
	ULONG NumberOfBytesRead = 0;
	hr = pIFileDevice->Read((ULONG)FileSize, &NumberOfBytesRead, (LPBYTE)pBuffer);
	strXML.ReleaseBuffer();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = FromString(RootName, strXML);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FromString"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT My_Win32_Product::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_WRITE, 0, CREATE_ALWAYS, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = ToStream(RootName, spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ToStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT My_Win32_Product::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = FromStream(RootName, spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FromStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
VOID My_Win32_Service::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(__Path)))
		{
			ElementToValue(FIELD_NAME(__Path), __Path, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(AcceptPause)))
		{
			ElementToValue(FIELD_NAME(AcceptPause), AcceptPause, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(AcceptStop)))
		{
			ElementToValue(FIELD_NAME(AcceptStop), AcceptStop, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Caption)))
		{
			ElementToValue(FIELD_NAME(Caption), Caption, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CheckPoint)))
		{
			ElementToValue(FIELD_NAME(CheckPoint), CheckPoint, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CreationClassName)))
		{
			ElementToValue(FIELD_NAME(CreationClassName), CreationClassName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Description)))
		{
			ElementToValue(FIELD_NAME(Description), Description, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DesktopInteract)))
		{
			ElementToValue(FIELD_NAME(DesktopInteract), DesktopInteract, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DisplayName)))
		{
			ElementToValue(FIELD_NAME(DisplayName), DisplayName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ErrorControl)))
		{
			ElementToValue(FIELD_NAME(ErrorControl), ErrorControl, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ExitCode)))
		{
			ElementToValue(FIELD_NAME(ExitCode), ExitCode, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(InstallDate)))
		{
			ElementToValue(FIELD_NAME(InstallDate), InstallDate, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Name)))
		{
			ElementToValue(FIELD_NAME(Name), Name, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PathName)))
		{
			ElementToValue(FIELD_NAME(PathName), PathName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ProcessId)))
		{
			ElementToValue(FIELD_NAME(ProcessId), ProcessId, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ServiceSpecificExitCode)))
		{
			ElementToValue(FIELD_NAME(ServiceSpecificExitCode), ServiceSpecificExitCode, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ServiceType)))
		{
			ElementToValue(FIELD_NAME(ServiceType), ServiceType, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Started)))
		{
			ElementToValue(FIELD_NAME(Started), Started, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(StartMode)))
		{
			ElementToValue(FIELD_NAME(StartMode), StartMode, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(StartName)))
		{
			ElementToValue(FIELD_NAME(StartName), StartName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(State)))
		{
			ElementToValue(FIELD_NAME(State), State, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Status)))
		{
			ElementToValue(FIELD_NAME(Status), Status, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SystemCreationClassName)))
		{
			ElementToValue(FIELD_NAME(SystemCreationClassName), SystemCreationClassName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SystemName)))
		{
			ElementToValue(FIELD_NAME(SystemName), SystemName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(TagId)))
		{
			ElementToValue(FIELD_NAME(TagId), TagId, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(WaitHint)))
		{
			ElementToValue(FIELD_NAME(WaitHint), WaitHint, Child_jfs654);
		}
	}
}
VOID My_Win32_Service::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT My_Win32_Service::FromString( LPCTSTR RootName, CONST CString & strXml )
{
	XmlElement xmlElement;
	INT Start = 0;
	HRESULT hr = S_OK;
	LONG xml_ele_count = xmlElement.FromString(strXml, Start);
	if (xml_ele_count > 0)
	{
		FromElement(RootName, xmlElement);
	}
	else
	{
		hr = HRESULT_FROM_WIN32(ERROR_BAD_FORMAT);
	}
	return hr;
}

HRESULT My_Win32_Service::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CString strXml;
	ToString(RootName, strXml);
	static CONST BYTE UTF16_TAG[] = {0xff, 0xfe};
	ULONG NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(sizeof(UTF16_TAG), &NumberOfBytesWritten, UTF16_TAG);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(strXml.GetLength() * sizeof(TCHAR), &NumberOfBytesWritten, (LPCBYTE)strXml.GetString());
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT My_Win32_Service::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	LONGLONG FileSize = 0;
	hr = pIFileDevice->GetFileSize(&FileSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	CString strXML;
	INT CharCount = (INT)FileSize / sizeof(TCHAR) + 2;
	LPTSTR pBuffer = strXML.GetBuffer(CharCount);
	pBuffer[CharCount - 1] = 0;
	pBuffer[CharCount - 2] = 0;
	ULONG NumberOfBytesRead = 0;
	hr = pIFileDevice->Read((ULONG)FileSize, &NumberOfBytesRead, (LPBYTE)pBuffer);
	strXML.ReleaseBuffer();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = FromString(RootName, strXML);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FromString"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT My_Win32_Service::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_WRITE, 0, CREATE_ALWAYS, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = ToStream(RootName, spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ToStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT My_Win32_Service::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = FromStream(RootName, spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FromStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
VOID My_System_Info::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(OperatingSystems)))
		{
			/*ElementToVectorStruct*/ElementToContainerStruct(FIELD_NAME(OperatingSystems), OperatingSystems, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ComputerSystems)))
		{
			/*ElementToVectorStruct*/ElementToContainerStruct(FIELD_NAME(ComputerSystems), ComputerSystems, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Processors)))
		{
			/*ElementToVectorStruct*/ElementToContainerStruct(FIELD_NAME(Processors), Processors, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(NetworkAdapters)))
		{
			/*ElementToVectorStruct*/ElementToContainerStruct(FIELD_NAME(NetworkAdapters), NetworkAdapters, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(NetworkAdapterConfigurations)))
		{
			/*ElementToVectorStruct*/ElementToContainerStruct(FIELD_NAME(NetworkAdapterConfigurations), NetworkAdapterConfigurations, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(NetworkAdapterSettings)))
		{
			/*ElementToVectorStruct*/ElementToContainerStruct(FIELD_NAME(NetworkAdapterSettings), NetworkAdapterSettings, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Products)))
		{
			/*ElementToVectorStruct*/ElementToContainerStruct(FIELD_NAME(Products), Products, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Services)))
		{
			/*ElementToVectorStruct*/ElementToContainerStruct(FIELD_NAME(Services), Services, Child_jfs654);
		}
	}
}
VOID My_System_Info::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT My_System_Info::FromString( LPCTSTR RootName, CONST CString & strXml )
{
	XmlElement xmlElement;
	INT Start = 0;
	HRESULT hr = S_OK;
	LONG xml_ele_count = xmlElement.FromString(strXml, Start);
	if (xml_ele_count > 0)
	{
		FromElement(RootName, xmlElement);
	}
	else
	{
		hr = HRESULT_FROM_WIN32(ERROR_BAD_FORMAT);
	}
	return hr;
}

HRESULT My_System_Info::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CString strXml;
	ToString(RootName, strXml);
	static CONST BYTE UTF16_TAG[] = {0xff, 0xfe};
	ULONG NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(sizeof(UTF16_TAG), &NumberOfBytesWritten, UTF16_TAG);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(strXml.GetLength() * sizeof(TCHAR), &NumberOfBytesWritten, (LPCBYTE)strXml.GetString());
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT My_System_Info::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	LONGLONG FileSize = 0;
	hr = pIFileDevice->GetFileSize(&FileSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	CString strXML;
	INT CharCount = (INT)FileSize / sizeof(TCHAR) + 2;
	LPTSTR pBuffer = strXML.GetBuffer(CharCount);
	pBuffer[CharCount - 1] = 0;
	pBuffer[CharCount - 2] = 0;
	ULONG NumberOfBytesRead = 0;
	hr = pIFileDevice->Read((ULONG)FileSize, &NumberOfBytesRead, (LPBYTE)pBuffer);
	strXML.ReleaseBuffer();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = FromString(RootName, strXML);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FromString"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT My_System_Info::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_WRITE, 0, CREATE_ALWAYS, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = ToStream(RootName, spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ToStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT My_System_Info::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = FromStream(RootName, spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FromStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

////////////////////////////RRRRRR END//////////////////////////////////////////////

