////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID My_Win32_NetworkAdapter::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(__Path), __Path, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(AdapterType), AdapterType, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(AdapterTypeID), AdapterTypeID, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(AutoSense), AutoSense, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Availability), Availability, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Caption), Caption, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ConfigManagerErrorCode), ConfigManagerErrorCode, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ConfigManagerUserConfig), ConfigManagerUserConfig, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CreationClassName), CreationClassName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Description), Description, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DeviceID), DeviceID, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ErrorCleared), ErrorCleared, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ErrorDescription), ErrorDescription, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(GUID), GUID, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Index), Index, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(InstallDate), InstallDate, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Installed), Installed, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(InterfaceIndex), InterfaceIndex, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(LastErrorCode), LastErrorCode, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(MACAddress), MACAddress, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Manufacturer), Manufacturer, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(MaxNumberControlled), MaxNumberControlled, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(MaxSpeed), MaxSpeed, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Name), Name, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(NetConnectionID), NetConnectionID, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(NetConnectionStatus), NetConnectionStatus, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(NetEnabled), NetEnabled, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(NetworkAddresses), NetworkAddresses, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PermanentAddress), PermanentAddress, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PhysicalAdapter), PhysicalAdapter, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PNPDeviceID), PNPDeviceID, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(PowerManagementCapabilities), PowerManagementCapabilities, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PowerManagementSupported), PowerManagementSupported, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ProductName), ProductName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ServiceName), ServiceName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Speed), Speed, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Status), Status, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(StatusInfo), StatusInfo, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SystemCreationClassName), SystemCreationClassName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SystemName), SystemName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(TimeOfLastReset), TimeOfLastReset, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID My_Win32_NetworkAdapterConfiguration::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(__Path), __Path, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ArpAlwaysSourceRoute), ArpAlwaysSourceRoute, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ArpUseEtherSNAP), ArpUseEtherSNAP, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Caption), Caption, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DatabasePath), DatabasePath, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DeadGWDetectEnabled), DeadGWDetectEnabled, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(DefaultIPGateway), DefaultIPGateway, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DefaultTOS), DefaultTOS, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DefaultTTL), DefaultTTL, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Description), Description, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DHCPEnabled), DHCPEnabled, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DHCPLeaseExpires), DHCPLeaseExpires, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DHCPLeaseObtained), DHCPLeaseObtained, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DHCPServer), DHCPServer, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DNSDomain), DNSDomain, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(DNSDomainSuffixSearchOrder), DNSDomainSuffixSearchOrder, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DNSEnabledForWINSResolution), DNSEnabledForWINSResolution, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DNSHostName), DNSHostName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(DNSServerSearchOrder), DNSServerSearchOrder, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DomainDNSRegistrationEnabled), DomainDNSRegistrationEnabled, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ForwardBufferMemory), ForwardBufferMemory, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(FullDNSRegistrationEnabled), FullDNSRegistrationEnabled, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(GatewayCostMetric), GatewayCostMetric, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(IGMPLevel), IGMPLevel, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Index), Index, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(InterfaceIndex), InterfaceIndex, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(IPAddress), IPAddress, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(IPConnectionMetric), IPConnectionMetric, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(IPEnabled), IPEnabled, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(IPFilterSecurityEnabled), IPFilterSecurityEnabled, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(IPPortSecurityEnabled), IPPortSecurityEnabled, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(IPSecPermitIPProtocols), IPSecPermitIPProtocols, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(IPSecPermitTCPPorts), IPSecPermitTCPPorts, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(IPSecPermitUDPPorts), IPSecPermitUDPPorts, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(IPSubnet), IPSubnet, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(IPUseZeroBroadcast), IPUseZeroBroadcast, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(IPXAddress), IPXAddress, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(IPXEnabled), IPXEnabled, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(IPXFrameType), IPXFrameType, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(IPXMediaType), IPXMediaType, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(IPXNetworkNumber), IPXNetworkNumber, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(IPXVirtualNetNumber), IPXVirtualNetNumber, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(KeepAliveInterval), KeepAliveInterval, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(KeepAliveTime), KeepAliveTime, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(MACAddress), MACAddress, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(MTU), MTU, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(NumForwardPackets), NumForwardPackets, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PMTUBHDetectEnabled), PMTUBHDetectEnabled, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(PMTUDiscoveryEnabled), PMTUDiscoveryEnabled, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ServiceName), ServiceName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SettingID), SettingID, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(TcpipNetbiosOptions), TcpipNetbiosOptions, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(TcpMaxConnectRetransmissions), TcpMaxConnectRetransmissions, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(TcpMaxDataRetransmissions), TcpMaxDataRetransmissions, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(TcpNumConnections), TcpNumConnections, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(TcpUseRFC1122UrgentPointer), TcpUseRFC1122UrgentPointer, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(TcpWindowSize), TcpWindowSize, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(WINSEnableLMHostsLookup), WINSEnableLMHostsLookup, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(WINSHostLookupFile), WINSHostLookupFile, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(WINSPrimaryServer), WINSPrimaryServer, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(WINSScopeID), WINSScopeID, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(WINSSecondaryServer), WINSSecondaryServer, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID My_Win32_NetworkAdapterSetting::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(__Path), __Path, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Element), Element, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Setting), Setting, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
VOID My_Win32_NetworkAdapter::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
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
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(AdapterType)))
		{
			ElementToValue(FIELD_NAME(AdapterType), AdapterType, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(AdapterTypeID)))
		{
			ElementToValue(FIELD_NAME(AdapterTypeID), AdapterTypeID, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(AutoSense)))
		{
			ElementToValue(FIELD_NAME(AutoSense), AutoSense, Child_jfs654);
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
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CreationClassName)))
		{
			ElementToValue(FIELD_NAME(CreationClassName), CreationClassName, Child_jfs654);
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
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(GUID)))
		{
			ElementToValue(FIELD_NAME(GUID), GUID, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Index)))
		{
			ElementToValue(FIELD_NAME(Index), Index, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(InstallDate)))
		{
			ElementToValue(FIELD_NAME(InstallDate), InstallDate, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Installed)))
		{
			ElementToValue(FIELD_NAME(Installed), Installed, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(InterfaceIndex)))
		{
			ElementToValue(FIELD_NAME(InterfaceIndex), InterfaceIndex, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(LastErrorCode)))
		{
			ElementToValue(FIELD_NAME(LastErrorCode), LastErrorCode, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(MACAddress)))
		{
			ElementToValue(FIELD_NAME(MACAddress), MACAddress, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Manufacturer)))
		{
			ElementToValue(FIELD_NAME(Manufacturer), Manufacturer, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(MaxNumberControlled)))
		{
			ElementToValue(FIELD_NAME(MaxNumberControlled), MaxNumberControlled, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(MaxSpeed)))
		{
			ElementToValue(FIELD_NAME(MaxSpeed), MaxSpeed, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Name)))
		{
			ElementToValue(FIELD_NAME(Name), Name, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(NetConnectionID)))
		{
			ElementToValue(FIELD_NAME(NetConnectionID), NetConnectionID, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(NetConnectionStatus)))
		{
			ElementToValue(FIELD_NAME(NetConnectionStatus), NetConnectionStatus, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(NetEnabled)))
		{
			ElementToValue(FIELD_NAME(NetEnabled), NetEnabled, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(NetworkAddresses)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(NetworkAddresses), NetworkAddresses, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PermanentAddress)))
		{
			ElementToValue(FIELD_NAME(PermanentAddress), PermanentAddress, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PhysicalAdapter)))
		{
			ElementToValue(FIELD_NAME(PhysicalAdapter), PhysicalAdapter, Child_jfs654);
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
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ProductName)))
		{
			ElementToValue(FIELD_NAME(ProductName), ProductName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ServiceName)))
		{
			ElementToValue(FIELD_NAME(ServiceName), ServiceName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Speed)))
		{
			ElementToValue(FIELD_NAME(Speed), Speed, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Status)))
		{
			ElementToValue(FIELD_NAME(Status), Status, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(StatusInfo)))
		{
			ElementToValue(FIELD_NAME(StatusInfo), StatusInfo, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SystemCreationClassName)))
		{
			ElementToValue(FIELD_NAME(SystemCreationClassName), SystemCreationClassName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SystemName)))
		{
			ElementToValue(FIELD_NAME(SystemName), SystemName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(TimeOfLastReset)))
		{
			ElementToValue(FIELD_NAME(TimeOfLastReset), TimeOfLastReset, Child_jfs654);
		}
	}
}
VOID My_Win32_NetworkAdapter::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT My_Win32_NetworkAdapter::FromString( LPCTSTR RootName, CONST CString & strXml )
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

HRESULT My_Win32_NetworkAdapter::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
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

HRESULT My_Win32_NetworkAdapter::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
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

HRESULT My_Win32_NetworkAdapter::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
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

HRESULT My_Win32_NetworkAdapter::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
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
VOID My_Win32_NetworkAdapterConfiguration::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
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
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ArpAlwaysSourceRoute)))
		{
			ElementToValue(FIELD_NAME(ArpAlwaysSourceRoute), ArpAlwaysSourceRoute, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ArpUseEtherSNAP)))
		{
			ElementToValue(FIELD_NAME(ArpUseEtherSNAP), ArpUseEtherSNAP, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Caption)))
		{
			ElementToValue(FIELD_NAME(Caption), Caption, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DatabasePath)))
		{
			ElementToValue(FIELD_NAME(DatabasePath), DatabasePath, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DeadGWDetectEnabled)))
		{
			ElementToValue(FIELD_NAME(DeadGWDetectEnabled), DeadGWDetectEnabled, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DefaultIPGateway)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(DefaultIPGateway), DefaultIPGateway, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DefaultTOS)))
		{
			ElementToValue(FIELD_NAME(DefaultTOS), DefaultTOS, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DefaultTTL)))
		{
			ElementToValue(FIELD_NAME(DefaultTTL), DefaultTTL, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Description)))
		{
			ElementToValue(FIELD_NAME(Description), Description, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DHCPEnabled)))
		{
			ElementToValue(FIELD_NAME(DHCPEnabled), DHCPEnabled, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DHCPLeaseExpires)))
		{
			ElementToValue(FIELD_NAME(DHCPLeaseExpires), DHCPLeaseExpires, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DHCPLeaseObtained)))
		{
			ElementToValue(FIELD_NAME(DHCPLeaseObtained), DHCPLeaseObtained, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DHCPServer)))
		{
			ElementToValue(FIELD_NAME(DHCPServer), DHCPServer, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DNSDomain)))
		{
			ElementToValue(FIELD_NAME(DNSDomain), DNSDomain, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DNSDomainSuffixSearchOrder)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(DNSDomainSuffixSearchOrder), DNSDomainSuffixSearchOrder, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DNSEnabledForWINSResolution)))
		{
			ElementToValue(FIELD_NAME(DNSEnabledForWINSResolution), DNSEnabledForWINSResolution, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DNSHostName)))
		{
			ElementToValue(FIELD_NAME(DNSHostName), DNSHostName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DNSServerSearchOrder)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(DNSServerSearchOrder), DNSServerSearchOrder, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DomainDNSRegistrationEnabled)))
		{
			ElementToValue(FIELD_NAME(DomainDNSRegistrationEnabled), DomainDNSRegistrationEnabled, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ForwardBufferMemory)))
		{
			ElementToValue(FIELD_NAME(ForwardBufferMemory), ForwardBufferMemory, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(FullDNSRegistrationEnabled)))
		{
			ElementToValue(FIELD_NAME(FullDNSRegistrationEnabled), FullDNSRegistrationEnabled, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(GatewayCostMetric)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(GatewayCostMetric), GatewayCostMetric, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(IGMPLevel)))
		{
			ElementToValue(FIELD_NAME(IGMPLevel), IGMPLevel, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Index)))
		{
			ElementToValue(FIELD_NAME(Index), Index, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(InterfaceIndex)))
		{
			ElementToValue(FIELD_NAME(InterfaceIndex), InterfaceIndex, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(IPAddress)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(IPAddress), IPAddress, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(IPConnectionMetric)))
		{
			ElementToValue(FIELD_NAME(IPConnectionMetric), IPConnectionMetric, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(IPEnabled)))
		{
			ElementToValue(FIELD_NAME(IPEnabled), IPEnabled, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(IPFilterSecurityEnabled)))
		{
			ElementToValue(FIELD_NAME(IPFilterSecurityEnabled), IPFilterSecurityEnabled, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(IPPortSecurityEnabled)))
		{
			ElementToValue(FIELD_NAME(IPPortSecurityEnabled), IPPortSecurityEnabled, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(IPSecPermitIPProtocols)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(IPSecPermitIPProtocols), IPSecPermitIPProtocols, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(IPSecPermitTCPPorts)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(IPSecPermitTCPPorts), IPSecPermitTCPPorts, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(IPSecPermitUDPPorts)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(IPSecPermitUDPPorts), IPSecPermitUDPPorts, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(IPSubnet)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(IPSubnet), IPSubnet, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(IPUseZeroBroadcast)))
		{
			ElementToValue(FIELD_NAME(IPUseZeroBroadcast), IPUseZeroBroadcast, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(IPXAddress)))
		{
			ElementToValue(FIELD_NAME(IPXAddress), IPXAddress, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(IPXEnabled)))
		{
			ElementToValue(FIELD_NAME(IPXEnabled), IPXEnabled, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(IPXFrameType)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(IPXFrameType), IPXFrameType, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(IPXMediaType)))
		{
			ElementToValue(FIELD_NAME(IPXMediaType), IPXMediaType, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(IPXNetworkNumber)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(IPXNetworkNumber), IPXNetworkNumber, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(IPXVirtualNetNumber)))
		{
			ElementToValue(FIELD_NAME(IPXVirtualNetNumber), IPXVirtualNetNumber, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(KeepAliveInterval)))
		{
			ElementToValue(FIELD_NAME(KeepAliveInterval), KeepAliveInterval, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(KeepAliveTime)))
		{
			ElementToValue(FIELD_NAME(KeepAliveTime), KeepAliveTime, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(MACAddress)))
		{
			ElementToValue(FIELD_NAME(MACAddress), MACAddress, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(MTU)))
		{
			ElementToValue(FIELD_NAME(MTU), MTU, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(NumForwardPackets)))
		{
			ElementToValue(FIELD_NAME(NumForwardPackets), NumForwardPackets, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PMTUBHDetectEnabled)))
		{
			ElementToValue(FIELD_NAME(PMTUBHDetectEnabled), PMTUBHDetectEnabled, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(PMTUDiscoveryEnabled)))
		{
			ElementToValue(FIELD_NAME(PMTUDiscoveryEnabled), PMTUDiscoveryEnabled, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ServiceName)))
		{
			ElementToValue(FIELD_NAME(ServiceName), ServiceName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SettingID)))
		{
			ElementToValue(FIELD_NAME(SettingID), SettingID, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(TcpipNetbiosOptions)))
		{
			ElementToValue(FIELD_NAME(TcpipNetbiosOptions), TcpipNetbiosOptions, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(TcpMaxConnectRetransmissions)))
		{
			ElementToValue(FIELD_NAME(TcpMaxConnectRetransmissions), TcpMaxConnectRetransmissions, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(TcpMaxDataRetransmissions)))
		{
			ElementToValue(FIELD_NAME(TcpMaxDataRetransmissions), TcpMaxDataRetransmissions, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(TcpNumConnections)))
		{
			ElementToValue(FIELD_NAME(TcpNumConnections), TcpNumConnections, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(TcpUseRFC1122UrgentPointer)))
		{
			ElementToValue(FIELD_NAME(TcpUseRFC1122UrgentPointer), TcpUseRFC1122UrgentPointer, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(TcpWindowSize)))
		{
			ElementToValue(FIELD_NAME(TcpWindowSize), TcpWindowSize, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(WINSEnableLMHostsLookup)))
		{
			ElementToValue(FIELD_NAME(WINSEnableLMHostsLookup), WINSEnableLMHostsLookup, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(WINSHostLookupFile)))
		{
			ElementToValue(FIELD_NAME(WINSHostLookupFile), WINSHostLookupFile, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(WINSPrimaryServer)))
		{
			ElementToValue(FIELD_NAME(WINSPrimaryServer), WINSPrimaryServer, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(WINSScopeID)))
		{
			ElementToValue(FIELD_NAME(WINSScopeID), WINSScopeID, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(WINSSecondaryServer)))
		{
			ElementToValue(FIELD_NAME(WINSSecondaryServer), WINSSecondaryServer, Child_jfs654);
		}
	}
}
VOID My_Win32_NetworkAdapterConfiguration::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT My_Win32_NetworkAdapterConfiguration::FromString( LPCTSTR RootName, CONST CString & strXml )
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

HRESULT My_Win32_NetworkAdapterConfiguration::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
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

HRESULT My_Win32_NetworkAdapterConfiguration::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
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

HRESULT My_Win32_NetworkAdapterConfiguration::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
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

HRESULT My_Win32_NetworkAdapterConfiguration::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
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
VOID My_Win32_NetworkAdapterSetting::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
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
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Element)))
		{
			ElementToValue(FIELD_NAME(Element), Element, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Setting)))
		{
			ElementToValue(FIELD_NAME(Setting), Setting, Child_jfs654);
		}
	}
}
VOID My_Win32_NetworkAdapterSetting::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT My_Win32_NetworkAdapterSetting::FromString( LPCTSTR RootName, CONST CString & strXml )
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

HRESULT My_Win32_NetworkAdapterSetting::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
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

HRESULT My_Win32_NetworkAdapterSetting::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
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

HRESULT My_Win32_NetworkAdapterSetting::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
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

HRESULT My_Win32_NetworkAdapterSetting::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
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

