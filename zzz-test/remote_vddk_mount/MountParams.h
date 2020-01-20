#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include <vector>
#include <string>

using namespace std;
//////////////////////////////////////////////////////////////////////////

#include "..\..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\..\PublicHeader\PublicDTraceLib.h"

#include "..\..\PublicHeader\PublicSimpleXml.h"
#include "..\..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\..\PublicHeader\PublicValueXmlElementConvert.h"
#include "..\..\PublicHeader\PublicParameterStreamDef.h"

//////////////////////////////////////////////////////////////////////////
#include "vm_basic_types.h"
#include "vixDiskLib.h"
#include "vixMntapi.h"



#if 0

CString vmxSpec,
CString serverName,
CString userName,
CString password,
const vector<string> & diskPath,
uint32 port = 0,
CString thumbPrint = NULL,
Bool readOnly = TRUE,
Cstring snapshotRef = NULL,
Cstring transportModes = NULL,
const char* configFile = NULL,
const char* libDir = "D:\\VMware-vix-disklib-5.5.2-1890828.x86_64",
BOOL bPressKey = FALSE

#endif


struct MountParams
{
	SERIALIZE_BEGIN_STRUCT(MountParams, MountParams);
	CString vmxSpec;			SERIALIZE_BASIC(vmxSpec);
	CString serverName;			SERIALIZE_BASIC(serverName);
	CString userName;			SERIALIZE_BASIC(userName);
	CString password;			SERIALIZE_BASIC(password);
	vector<CString> diskPath;	SERIALIZE_VECTOR(diskPath);
	uint32 port;				SERIALIZE_BASIC(port);
	CString thumbPrint;			SERIALIZE_BASIC(thumbPrint);
	BOOL readOnly;				SERIALIZE_BASIC(readOnly);
	CString snapshotRef;		SERIALIZE_BASIC(snapshotRef);
	CString transportModes;		SERIALIZE_BASIC(transportModes);
	CString configFile;			SERIALIZE_BASIC(configFile);
	CString libDir;				SERIALIZE_BASIC(libDir);
	BOOL bPressKey;				SERIALIZE_BASIC(bPressKey);
	BOOL bDismount;				SERIALIZE_BASIC(bDismount);
	CString DriverLetter;		SERIALIZE_BASIC(DriverLetter);
	BOOL bGetOsInfo;			SERIALIZE_BASIC(bGetOsInfo);
	CString DumpToDirectory;	SERIALIZE_BASIC(DumpToDirectory);
	SERIALIZE_END_STRUCT(MountParams, MountParams);

	STRUCT_XML_SERIALIZER;


	void Init()
	{
		vmxSpec = TEXT("[datastore2-9T-raid0] sonmi01-004-rps2-veeam/sonmi01-004-rps2-veeam.vmx");
		serverName = TEXT("155.35.102.249");
		userName = TEXT("root");
		password = TEXT("1qaz@WSX3edc$RFV");
		diskPath.push_back(TEXT("[datastore2-9T-raid0] sonmi01-004-rps2-veeam/sonmi01-004-rps2-veeam-000004.vmdk"));
		diskPath.push_back(TEXT("[datastore2-9T-raid0] sonmi01-004-rps2-veeam/sonmi01-004-rps2-veeam_1-000004.vmdk"));

		port = 0;
		thumbPrint = TEXT("");
		readOnly = TRUE;
		snapshotRef = TEXT("");
		transportModes = TEXT("");
		configFile = TEXT("");
		libDir = TEXT("D:\\VMware-vix-disklib-5.5.2-1890828.x86_64");
		bPressKey = FALSE;
		bDismount = FALSE;

		DriverLetter = TEXT("M:");
		bGetOsInfo = FALSE;
	}

	static void unit_test()
	{
		//////////////////////////////////////////////////////////////////////////
		MountParams var1;
		var1.Init();

		LPCTSTR fileName = TEXT("e:\\remote_vddk_mount_sample.xml");
		var1.ToFile(FIELD_NAME(MountParams), fileName);

		//////////////////////////////////////////////////////////////////////////
		MountParams var2;
		var2.FromFile(FIELD_NAME(MountParams), fileName);

	}
};

