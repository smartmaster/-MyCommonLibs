#include "stdafx.h"
#include "My_NetworkAdapter_Info.h"
#include "My_NetworkAdapter_Info.h.xml.Output.hpp"

VOID My_Win32_NetworkAdapter::NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars)
{
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		if (0){}
		WMI_GET_PROP_STRING(__Path) //			SERIALIZE_BASIC(__Path);
			WMI_GET_PROP_STRING(AdapterType) //			SERIALIZE_BASIC(AdapterType);
			WMI_GET_PROP_INT(AdapterTypeID) //			SERIALIZE_BASIC(AdapterTypeID);
			WMI_GET_PROP_INT(AutoSense) //			SERIALIZE_BASIC(AutoSense);
			WMI_GET_PROP_INT(Availability) //			SERIALIZE_BASIC(Availability);
			WMI_GET_PROP_STRING(Caption) //			SERIALIZE_BASIC(Caption);
			WMI_GET_PROP_INT(ConfigManagerErrorCode) //			SERIALIZE_BASIC(ConfigManagerErrorCode);
			WMI_GET_PROP_INT(ConfigManagerUserConfig) //			SERIALIZE_BASIC(ConfigManagerUserConfig);
			WMI_GET_PROP_STRING(CreationClassName) //			SERIALIZE_BASIC(CreationClassName);
			WMI_GET_PROP_STRING(Description) //			SERIALIZE_BASIC(Description);
			WMI_GET_PROP_STRING(DeviceID) //			SERIALIZE_BASIC(DeviceID);
			WMI_GET_PROP_INT(ErrorCleared) //			SERIALIZE_BASIC(ErrorCleared);
			WMI_GET_PROP_STRING(ErrorDescription) //			SERIALIZE_BASIC(ErrorDescription);
			WMI_GET_PROP_STRING(GUID) //			SERIALIZE_BASIC(GUID);
			WMI_GET_PROP_INT(Index) //			SERIALIZE_BASIC(Index);
			WMI_GET_PROP_SYSTEMTIME(InstallDate) //			SERIALIZE_BASIC(InstallDate);
			WMI_GET_PROP_INT(Installed) //			SERIALIZE_BASIC(Installed);
			WMI_GET_PROP_INT(InterfaceIndex) //			SERIALIZE_BASIC(InterfaceIndex);
			WMI_GET_PROP_INT(LastErrorCode) //			SERIALIZE_BASIC(LastErrorCode);
			WMI_GET_PROP_STRING(MACAddress) //			SERIALIZE_BASIC(MACAddress);
			WMI_GET_PROP_STRING(Manufacturer) //			SERIALIZE_BASIC(Manufacturer);
			WMI_GET_PROP_INT(MaxNumberControlled) //			SERIALIZE_BASIC(MaxNumberControlled);
			WMI_GET_PROP_INT(MaxSpeed) //			SERIALIZE_BASIC(MaxSpeed);
			WMI_GET_PROP_STRING(Name) //			SERIALIZE_BASIC(Name);
			WMI_GET_PROP_STRING(NetConnectionID) //			SERIALIZE_BASIC(NetConnectionID);
			WMI_GET_PROP_INT(NetConnectionStatus) //			SERIALIZE_BASIC(NetConnectionStatus);
			WMI_GET_PROP_INT(NetEnabled) //			SERIALIZE_BASIC(NetEnabled);
			WMI_GET_PROP_ARRAY(CString, BSTR, NetworkAddresses)  //			SERIALIZE_VECTOR(NetworkAddresses);
			WMI_GET_PROP_STRING(PermanentAddress) //			SERIALIZE_BASIC(PermanentAddress);
			WMI_GET_PROP_INT(PhysicalAdapter) //			SERIALIZE_BASIC(PhysicalAdapter);
			WMI_GET_PROP_STRING(PNPDeviceID) //			SERIALIZE_BASIC(PNPDeviceID);
			WMI_GET_PROP_ARRAY(UINT16, UINT16, PowerManagementCapabilities)  //			SERIALIZE_VECTOR(PowerManagementCapabilities);
			WMI_GET_PROP_INT(PowerManagementSupported) //			SERIALIZE_BASIC(PowerManagementSupported);
			WMI_GET_PROP_STRING(ProductName) //			SERIALIZE_BASIC(ProductName);
			WMI_GET_PROP_STRING(ServiceName) //			SERIALIZE_BASIC(ServiceName);
			WMI_GET_PROP_INT(Speed) //			SERIALIZE_BASIC(Speed);
			WMI_GET_PROP_STRING(Status) //			SERIALIZE_BASIC(Status);
			WMI_GET_PROP_INT(StatusInfo) //			SERIALIZE_BASIC(StatusInfo);
			WMI_GET_PROP_STRING(SystemCreationClassName) //			SERIALIZE_BASIC(SystemCreationClassName);
			WMI_GET_PROP_STRING(SystemName) //			SERIALIZE_BASIC(SystemName);
			WMI_GET_PROP_SYSTEMTIME(TimeOfLastReset) //			SERIALIZE_BASIC(TimeOfLastReset);
	}
}

VOID My_Win32_NetworkAdapterConfiguration::NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars)
{
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		if (0){}
		WMI_GET_PROP_STRING(__Path) //			SERIALIZE_BASIC(__Path);
			WMI_GET_PROP_INT(ArpAlwaysSourceRoute) //			SERIALIZE_BASIC(ArpAlwaysSourceRoute);
			WMI_GET_PROP_INT(ArpUseEtherSNAP) //			SERIALIZE_BASIC(ArpUseEtherSNAP);
			WMI_GET_PROP_STRING(Caption) //			SERIALIZE_BASIC(Caption);
			WMI_GET_PROP_STRING(DatabasePath) //			SERIALIZE_BASIC(DatabasePath);
			WMI_GET_PROP_INT(DeadGWDetectEnabled) //			SERIALIZE_BASIC(DeadGWDetectEnabled);
			WMI_GET_PROP_ARRAY(CString, BSTR, DefaultIPGateway)  //			SERIALIZE_VECTOR(DefaultIPGateway);
			WMI_GET_PROP_INT(DefaultTOS) //			SERIALIZE_BASIC(DefaultTOS);
			WMI_GET_PROP_INT(DefaultTTL) //			SERIALIZE_BASIC(DefaultTTL);
			WMI_GET_PROP_STRING(Description) //			SERIALIZE_BASIC(Description);
			WMI_GET_PROP_INT(DHCPEnabled) //			SERIALIZE_BASIC(DHCPEnabled);
			WMI_GET_PROP_SYSTEMTIME(DHCPLeaseExpires) //			SERIALIZE_BASIC(DHCPLeaseExpires);
			WMI_GET_PROP_SYSTEMTIME(DHCPLeaseObtained) //			SERIALIZE_BASIC(DHCPLeaseObtained);
			WMI_GET_PROP_STRING(DHCPServer) //			SERIALIZE_BASIC(DHCPServer);
			WMI_GET_PROP_STRING(DNSDomain) //			SERIALIZE_BASIC(DNSDomain);
			WMI_GET_PROP_ARRAY(CString, BSTR, DNSDomainSuffixSearchOrder)  //			SERIALIZE_VECTOR(DNSDomainSuffixSearchOrder);
			WMI_GET_PROP_INT(DNSEnabledForWINSResolution) //			SERIALIZE_BASIC(DNSEnabledForWINSResolution);
			WMI_GET_PROP_STRING(DNSHostName) //			SERIALIZE_BASIC(DNSHostName);
			WMI_GET_PROP_ARRAY(CString, BSTR, DNSServerSearchOrder)  //			SERIALIZE_VECTOR(DNSServerSearchOrder);
			WMI_GET_PROP_INT(DomainDNSRegistrationEnabled) //			SERIALIZE_BASIC(DomainDNSRegistrationEnabled);
			WMI_GET_PROP_INT(ForwardBufferMemory) //			SERIALIZE_BASIC(ForwardBufferMemory);
			WMI_GET_PROP_INT(FullDNSRegistrationEnabled) //			SERIALIZE_BASIC(FullDNSRegistrationEnabled);
			WMI_GET_PROP_ARRAY(INT, INT, GatewayCostMetric)  //			SERIALIZE_VECTOR(GatewayCostMetric);
			WMI_GET_PROP_INT(IGMPLevel) //			SERIALIZE_BASIC(IGMPLevel);
			WMI_GET_PROP_INT(Index) //			SERIALIZE_BASIC(Index);
			WMI_GET_PROP_INT(InterfaceIndex) //			SERIALIZE_BASIC(InterfaceIndex);
			WMI_GET_PROP_ARRAY(CString, BSTR, IPAddress)  //			SERIALIZE_VECTOR(IPAddress);
			WMI_GET_PROP_INT(IPConnectionMetric) //			SERIALIZE_BASIC(IPConnectionMetric);
			WMI_GET_PROP_INT(IPEnabled) //			SERIALIZE_BASIC(IPEnabled);
			WMI_GET_PROP_INT(IPFilterSecurityEnabled) //			SERIALIZE_BASIC(IPFilterSecurityEnabled);
			WMI_GET_PROP_INT(IPPortSecurityEnabled) //			SERIALIZE_BASIC(IPPortSecurityEnabled);
			WMI_GET_PROP_ARRAY(CString, BSTR, IPSecPermitIPProtocols)  //			SERIALIZE_VECTOR(IPSecPermitIPProtocols);
			WMI_GET_PROP_ARRAY(CString, BSTR, IPSecPermitTCPPorts)  //			SERIALIZE_VECTOR(IPSecPermitTCPPorts);
			WMI_GET_PROP_ARRAY(CString, BSTR, IPSecPermitUDPPorts)  //			SERIALIZE_VECTOR(IPSecPermitUDPPorts);
			WMI_GET_PROP_ARRAY(CString, BSTR, IPSubnet)  //			SERIALIZE_VECTOR(IPSubnet);
			WMI_GET_PROP_INT(IPUseZeroBroadcast) //			SERIALIZE_BASIC(IPUseZeroBroadcast);
			WMI_GET_PROP_STRING(IPXAddress) //			SERIALIZE_BASIC(IPXAddress);
			WMI_GET_PROP_INT(IPXEnabled) //			SERIALIZE_BASIC(IPXEnabled);
			WMI_GET_PROP_ARRAY(UINT32, UINT32, IPXFrameType)  //			SERIALIZE_VECTOR(IPXFrameType);
			WMI_GET_PROP_INT(IPXMediaType) //			SERIALIZE_BASIC(IPXMediaType);
			WMI_GET_PROP_ARRAY(CString, BSTR, IPXNetworkNumber)  //			SERIALIZE_VECTOR(IPXNetworkNumber);
			WMI_GET_PROP_STRING(IPXVirtualNetNumber) //			SERIALIZE_BASIC(IPXVirtualNetNumber);
			WMI_GET_PROP_INT(KeepAliveInterval) //			SERIALIZE_BASIC(KeepAliveInterval);
			WMI_GET_PROP_INT(KeepAliveTime) //			SERIALIZE_BASIC(KeepAliveTime);
			WMI_GET_PROP_STRING(MACAddress) //			SERIALIZE_BASIC(MACAddress);
			WMI_GET_PROP_INT(MTU) //			SERIALIZE_BASIC(MTU);
			WMI_GET_PROP_INT(NumForwardPackets) //			SERIALIZE_BASIC(NumForwardPackets);
			WMI_GET_PROP_INT(PMTUBHDetectEnabled) //			SERIALIZE_BASIC(PMTUBHDetectEnabled);
			WMI_GET_PROP_INT(PMTUDiscoveryEnabled) //			SERIALIZE_BASIC(PMTUDiscoveryEnabled);
			WMI_GET_PROP_STRING(ServiceName) //			SERIALIZE_BASIC(ServiceName);
			WMI_GET_PROP_STRING(SettingID) //			SERIALIZE_BASIC(SettingID);
			WMI_GET_PROP_INT(TcpipNetbiosOptions) //			SERIALIZE_BASIC(TcpipNetbiosOptions);
			WMI_GET_PROP_INT(TcpMaxConnectRetransmissions) //			SERIALIZE_BASIC(TcpMaxConnectRetransmissions);
			WMI_GET_PROP_INT(TcpMaxDataRetransmissions) //			SERIALIZE_BASIC(TcpMaxDataRetransmissions);
			WMI_GET_PROP_INT(TcpNumConnections) //			SERIALIZE_BASIC(TcpNumConnections);
			WMI_GET_PROP_INT(TcpUseRFC1122UrgentPointer) //			SERIALIZE_BASIC(TcpUseRFC1122UrgentPointer);
			WMI_GET_PROP_INT(TcpWindowSize) //			SERIALIZE_BASIC(TcpWindowSize);
			WMI_GET_PROP_INT(WINSEnableLMHostsLookup) //			SERIALIZE_BASIC(WINSEnableLMHostsLookup);
			WMI_GET_PROP_STRING(WINSHostLookupFile) //			SERIALIZE_BASIC(WINSHostLookupFile);
			WMI_GET_PROP_STRING(WINSPrimaryServer) //			SERIALIZE_BASIC(WINSPrimaryServer);
			WMI_GET_PROP_STRING(WINSScopeID) //			SERIALIZE_BASIC(WINSScopeID);
			WMI_GET_PROP_STRING(WINSSecondaryServer) //			SERIALIZE_BASIC(WINSSecondaryServer);
	}
}

VOID My_Win32_NetworkAdapterSetting::NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars)
{
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		if (0){}
		WMI_GET_PROP_STRING(__Path) //			SERIALIZE_BASIC(__Path);
		WMI_GET_PROP_STRING(Element) //			SERIALIZE_BASIC(Element);
		WMI_GET_PROP_STRING(Setting) //			SERIALIZE_BASIC(Setting);

	}
}
