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

struct My_Win32_NetworkAdapter/* : CIM_NetworkAdapter*/
{
	SERIALIZE_BEGIN_STRUCT(My_Win32_NetworkAdapter, My_Win32_NetworkAdapter);
	CString __Path;			SERIALIZE_BASIC(__Path);
	CString   AdapterType;			SERIALIZE_BASIC(AdapterType);
	UINT16   AdapterTypeID;			SERIALIZE_BASIC(AdapterTypeID);
	BOOL  AutoSense;			SERIALIZE_BASIC(AutoSense);
	UINT16   Availability;			SERIALIZE_BASIC(Availability);
	CString   Caption;			SERIALIZE_BASIC(Caption);
	UINT32   ConfigManagerErrorCode;			SERIALIZE_BASIC(ConfigManagerErrorCode);
	BOOL  ConfigManagerUserConfig;			SERIALIZE_BASIC(ConfigManagerUserConfig);
	CString   CreationClassName;			SERIALIZE_BASIC(CreationClassName);
	CString   Description;			SERIALIZE_BASIC(Description);
	CString   DeviceID;			SERIALIZE_BASIC(DeviceID);
	BOOL  ErrorCleared;			SERIALIZE_BASIC(ErrorCleared);
	CString   ErrorDescription;			SERIALIZE_BASIC(ErrorDescription);
	CString   GUID;			SERIALIZE_BASIC(GUID);
	UINT32   Index;			SERIALIZE_BASIC(Index);
	SYSTEMTIME InstallDate;			SERIALIZE_BASIC(InstallDate);
	BOOL  Installed;			SERIALIZE_BASIC(Installed);
	UINT32   InterfaceIndex;			SERIALIZE_BASIC(InterfaceIndex);
	UINT32   LastErrorCode;			SERIALIZE_BASIC(LastErrorCode);
	CString   MACAddress;			SERIALIZE_BASIC(MACAddress);
	CString   Manufacturer;			SERIALIZE_BASIC(Manufacturer);
	UINT32   MaxNumberControlled;			SERIALIZE_BASIC(MaxNumberControlled);
	UINT64   MaxSpeed;			SERIALIZE_BASIC(MaxSpeed);
	CString   Name;			SERIALIZE_BASIC(Name);
	CString   NetConnectionID;			SERIALIZE_BASIC(NetConnectionID);
	UINT16   NetConnectionStatus;			SERIALIZE_BASIC(NetConnectionStatus);
	BOOL  NetEnabled;			SERIALIZE_BASIC(NetEnabled);
	std::vector<CString>   NetworkAddresses;			SERIALIZE_VECTOR(NetworkAddresses);
	CString   PermanentAddress;			SERIALIZE_BASIC(PermanentAddress);
	BOOL  PhysicalAdapter;			SERIALIZE_BASIC(PhysicalAdapter);
	CString   PNPDeviceID;			SERIALIZE_BASIC(PNPDeviceID);
	std::vector<UINT16>   PowerManagementCapabilities;			SERIALIZE_VECTOR(PowerManagementCapabilities);
	BOOL  PowerManagementSupported;			SERIALIZE_BASIC(PowerManagementSupported);
	CString   ProductName;			SERIALIZE_BASIC(ProductName);
	CString   ServiceName;			SERIALIZE_BASIC(ServiceName);
	UINT64   Speed;			SERIALIZE_BASIC(Speed);
	CString   Status;			SERIALIZE_BASIC(Status);
	UINT16   StatusInfo;			SERIALIZE_BASIC(StatusInfo);
	CString   SystemCreationClassName;			SERIALIZE_BASIC(SystemCreationClassName);
	CString   SystemName;			SERIALIZE_BASIC(SystemName);
	SYSTEMTIME TimeOfLastReset;			SERIALIZE_BASIC(TimeOfLastReset);
	SERIALIZE_END_STRUCT(My_Win32_NetworkAdapter, My_Win32_NetworkAdapter);
	STRUCT_XML_SERIALIZER;

	VOID NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars);
};

struct My_Win32_NetworkAdapterConfiguration /*: CIM_Setting*/
{
	SERIALIZE_BEGIN_STRUCT(My_Win32_NetworkAdapterConfiguration, My_Win32_NetworkAdapterConfiguration);
	CString __Path;			SERIALIZE_BASIC(__Path);
	BOOL  ArpAlwaysSourceRoute;			SERIALIZE_BASIC(ArpAlwaysSourceRoute);
	BOOL  ArpUseEtherSNAP;			SERIALIZE_BASIC(ArpUseEtherSNAP);
	CString   Caption;			SERIALIZE_BASIC(Caption);
	CString   DatabasePath;			SERIALIZE_BASIC(DatabasePath);
	BOOL  DeadGWDetectEnabled;			SERIALIZE_BASIC(DeadGWDetectEnabled);
	std::vector<CString>   DefaultIPGateway;			SERIALIZE_VECTOR(DefaultIPGateway);
	UINT8    DefaultTOS;			SERIALIZE_BASIC(DefaultTOS);
	UINT8    DefaultTTL;			SERIALIZE_BASIC(DefaultTTL);
	CString   Description;			SERIALIZE_BASIC(Description);
	BOOL  DHCPEnabled;			SERIALIZE_BASIC(DHCPEnabled);
	SYSTEMTIME DHCPLeaseExpires;			SERIALIZE_BASIC(DHCPLeaseExpires);
	SYSTEMTIME DHCPLeaseObtained;			SERIALIZE_BASIC(DHCPLeaseObtained);
	CString   DHCPServer;			SERIALIZE_BASIC(DHCPServer);
	CString   DNSDomain;			SERIALIZE_BASIC(DNSDomain);
	std::vector<CString>   DNSDomainSuffixSearchOrder;			SERIALIZE_VECTOR(DNSDomainSuffixSearchOrder);
	BOOL  DNSEnabledForWINSResolution;			SERIALIZE_BASIC(DNSEnabledForWINSResolution);
	CString   DNSHostName;			SERIALIZE_BASIC(DNSHostName);
	std::vector<CString>   DNSServerSearchOrder;			SERIALIZE_VECTOR(DNSServerSearchOrder);
	BOOL  DomainDNSRegistrationEnabled;			SERIALIZE_BASIC(DomainDNSRegistrationEnabled);
	UINT32   ForwardBufferMemory;			SERIALIZE_BASIC(ForwardBufferMemory);
	BOOL  FullDNSRegistrationEnabled;			SERIALIZE_BASIC(FullDNSRegistrationEnabled);
	std::vector<INT>   GatewayCostMetric;			SERIALIZE_VECTOR(GatewayCostMetric);
	UINT8    IGMPLevel;			SERIALIZE_BASIC(IGMPLevel);
	UINT32   Index;			SERIALIZE_BASIC(Index);
	UINT32   InterfaceIndex;			SERIALIZE_BASIC(InterfaceIndex);
	std::vector<CString>   IPAddress;			SERIALIZE_VECTOR(IPAddress);
	UINT32   IPConnectionMetric;			SERIALIZE_BASIC(IPConnectionMetric);
	BOOL  IPEnabled;			SERIALIZE_BASIC(IPEnabled);
	BOOL  IPFilterSecurityEnabled;			SERIALIZE_BASIC(IPFilterSecurityEnabled);
	BOOL  IPPortSecurityEnabled;			SERIALIZE_BASIC(IPPortSecurityEnabled);
	std::vector<CString>   IPSecPermitIPProtocols;			SERIALIZE_VECTOR(IPSecPermitIPProtocols);
	std::vector<CString>   IPSecPermitTCPPorts;			SERIALIZE_VECTOR(IPSecPermitTCPPorts);
	std::vector<CString>  IPSecPermitUDPPorts;			SERIALIZE_VECTOR(IPSecPermitUDPPorts);
	std::vector<CString>   IPSubnet;			SERIALIZE_VECTOR(IPSubnet);
	BOOL  IPUseZeroBroadcast;			SERIALIZE_BASIC(IPUseZeroBroadcast);
	CString   IPXAddress;			SERIALIZE_BASIC(IPXAddress);
	BOOL  IPXEnabled;			SERIALIZE_BASIC(IPXEnabled);
	std::vector<UINT32>   IPXFrameType;			SERIALIZE_VECTOR(IPXFrameType);
	UINT32   IPXMediaType;			SERIALIZE_BASIC(IPXMediaType);
	std::vector<CString>   IPXNetworkNumber;			SERIALIZE_VECTOR(IPXNetworkNumber);
	CString   IPXVirtualNetNumber;			SERIALIZE_BASIC(IPXVirtualNetNumber);
	UINT32   KeepAliveInterval;			SERIALIZE_BASIC(KeepAliveInterval);
	UINT32   KeepAliveTime;			SERIALIZE_BASIC(KeepAliveTime);
	CString   MACAddress;			SERIALIZE_BASIC(MACAddress);
	UINT32   MTU;			SERIALIZE_BASIC(MTU);
	UINT32   NumForwardPackets;			SERIALIZE_BASIC(NumForwardPackets);
	BOOL  PMTUBHDetectEnabled;			SERIALIZE_BASIC(PMTUBHDetectEnabled);
	BOOL  PMTUDiscoveryEnabled;			SERIALIZE_BASIC(PMTUDiscoveryEnabled);
	CString   ServiceName;			SERIALIZE_BASIC(ServiceName);
	CString   SettingID;			SERIALIZE_BASIC(SettingID);
	UINT32   TcpipNetbiosOptions;			SERIALIZE_BASIC(TcpipNetbiosOptions);
	UINT32   TcpMaxConnectRetransmissions;			SERIALIZE_BASIC(TcpMaxConnectRetransmissions);
	UINT32   TcpMaxDataRetransmissions;			SERIALIZE_BASIC(TcpMaxDataRetransmissions);
	UINT32   TcpNumConnections;			SERIALIZE_BASIC(TcpNumConnections);
	BOOL  TcpUseRFC1122UrgentPointer;			SERIALIZE_BASIC(TcpUseRFC1122UrgentPointer);
	UINT16   TcpWindowSize;			SERIALIZE_BASIC(TcpWindowSize);
	BOOL  WINSEnableLMHostsLookup;			SERIALIZE_BASIC(WINSEnableLMHostsLookup);
	CString   WINSHostLookupFile;			SERIALIZE_BASIC(WINSHostLookupFile);
	CString   WINSPrimaryServer;			SERIALIZE_BASIC(WINSPrimaryServer);
	CString   WINSScopeID;			SERIALIZE_BASIC(WINSScopeID);
	CString   WINSSecondaryServer;			SERIALIZE_BASIC(WINSSecondaryServer);
	SERIALIZE_END_STRUCT(My_Win32_NetworkAdapterConfiguration, My_Win32_NetworkAdapterConfiguration);
	STRUCT_XML_SERIALIZER;

	VOID NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars);
};


struct My_Win32_NetworkAdapterSetting /*: Win32_DeviceSettings*/
{
	SERIALIZE_BEGIN_STRUCT(My_Win32_NetworkAdapterSetting, My_Win32_NetworkAdapterSetting);
	CString __Path;			SERIALIZE_BASIC(__Path);
	CString Element;			SERIALIZE_BASIC(Element);
	CString Setting;			SERIALIZE_BASIC(Setting);
	SERIALIZE_END_STRUCT(My_Win32_NetworkAdapterSetting, My_Win32_NetworkAdapterSetting);
	STRUCT_XML_SERIALIZER;

	VOID NameValuesToFields(CONST vector<CString> & names, CONST vector<CComVariant> & vars);
};