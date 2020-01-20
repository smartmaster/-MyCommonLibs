#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "..\PublicHeader\PublicValueXmlElementConvert.h"
#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\PublicHeader\PublicSimpleXml.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicParameterStreamDef.h"

//////////////////////////////////////////////////////////////////////////
struct PluginFileFactoryConfig
{
	SERIALIZE_BEGIN_STRUCT(PluginFileFactoryConfig, PluginFileFactoryConfig);
	LONG FileType;						SERIALIZE_BASIC(FileType);
	CString FileTypeString;				SERIALIZE_CSTRING(FileTypeString);
	BOOL IsRelativePath;				SERIALIZE_BASIC(IsRelativePath);
	CString ModulePath;					SERIALIZE_CSTRING(ModulePath);
	CString ApiName;					SERIALIZE_CSTRING(ApiName);
	SERIALIZE_END_STRUCT(PluginFileFactoryConfig, PluginFileFactoryConfig);

	STRUCT_XML_SERIALIZER;

	PluginFileFactoryConfig() :
		FileType(0),
		IsRelativePath(0)
	{
	}

	PluginFileFactoryConfig(LONG FileTypeTmp, LPCTSTR FileTypeStringTmp, BOOL IsRelativePathTmp, LPCTSTR ModulePathTmp, LPCTSTR ApiNameTmp) :
		FileType(FileTypeTmp),
		FileTypeString(FileTypeStringTmp),
		IsRelativePath(IsRelativePathTmp),
		ModulePath(ModulePathTmp),
		ApiName(ApiNameTmp)
	{
	}
};

struct PluginFileFactoryConfigs
{
	SERIALIZE_BEGIN_STRUCT(PluginFileFactoryConfigs, PluginFileFactoryConfigs);
	vector<PluginFileFactoryConfig> Configs;		SERIALIZE_COMPLEX_VECTOR(Configs);
	SERIALIZE_END_STRUCT(PluginFileFactoryConfigs, PluginFileFactoryConfigs);

	STRUCT_XML_SERIALIZER;
};

//////////////////////////////////////////////////////////////////////////
namespace PluginFileFactoryConfig_NS
{
	//////////////////////////////////////////////////////////////////////////
	static CONST PluginFileFactoryConfig fileTest =
	{
		-1,
		TEXT(".fileTest"),
		TRUE,
		TEXT("PluginFileFactoryFileTest.dll"),
		TEXT("CreateInstanceIPluginFileFactory")
	};

	//////////////////////////////////////////////////////////////////////////
	static CONST PluginFileFactoryConfig vhdx = 
	{
		1,
		TEXT(".vhdx"),
		TRUE,
		TEXT("PluginFileFactoryVHDX.dll"),
		TEXT("CreateInstanceIPluginFileFactory")
	};

	static CONST PluginFileFactoryConfig vhd =
	{
		2,
		TEXT(".vhd"),
		TRUE,
		TEXT("PluginFileFactoryVHD.dll"),
		TEXT("CreateInstanceIPluginFileFactory")
	};

	static CONST PluginFileFactoryConfig rawIFileDevice =
	{
		3,
		TEXT(".rawIFileDevice"),
		TRUE,
		TEXT("PluginFileFactoryFileTest.dll"),
		TEXT("CreateInstanceIPluginFileFactoryRawIFileDevice")
	};


	//////////////////////////////////////////////////////////////////////////
	static CONST PluginFileFactoryConfig fileCommandDiskVolumeInfo =
	{
		1001,
		TEXT(".fileCommandDiskVolumeInfo"),
		TRUE,
		TEXT("PluginFileFactoryFileCommand.dll"),
		TEXT("CreateInstanceIPluginFileFactory")
	};
	
}


//////////////////////////////////////////////////////////////////////////
struct CreateInstanceCDiskImageVhdxParamsXml
{
	SERIALIZE_BEGIN_STRUCT(CreateInstanceCDiskImageVhdxParamsXml, CreateInstanceCDiskImageVhdxParamsXml);
	CString		parentVhdxFileName;		SERIALIZE_CSTRING(parentVhdxFileName);
	CString		vhdxFileName;			SERIALIZE_CSTRING(vhdxFileName);
	DWORD		DesiredAccess;			SERIALIZE_BASIC(DesiredAccess);
	DWORD		CreationDisposition;	SERIALIZE_BASIC(CreationDisposition);
	LONGLONG	VirtualDiskSize;		SERIALIZE_BASIC(VirtualDiskSize);
	LONG		LogicalSectorSize;		SERIALIZE_BASIC(LogicalSectorSize);
	LONG		PhysicalSectorSize;		SERIALIZE_BASIC(PhysicalSectorSize);
	LONG		BlockSize;				SERIALIZE_BASIC(BlockSize);
	LONG		VhdxDiskType;			SERIALIZE_BASIC(VhdxDiskType);
	SERIALIZE_END_STRUCT(CreateInstanceCDiskImageVhdxParamsXml, CreateInstanceCDiskImageVhdxParamsXml);

	STRUCT_XML_SERIALIZER;
};


struct CreateInstanceRawIFileDeviceXml
{
	SERIALIZE_BEGIN_STRUCT(CreateInstanceRawIFileDeviceXml, CreateInstanceRawIFileDeviceXml);
	ULONGLONG FileDevicePtr;		SERIALIZE_BASIC(FileDevicePtr);
	SERIALIZE_END_STRUCT(CreateInstanceRawIFileDeviceXml, CreateInstanceRawIFileDeviceXml);
	STRUCT_XML_SERIALIZER;

	CreateInstanceRawIFileDeviceXml(IFileDevice * pIFileDevice) :
		FileDevicePtr((ULONGLONG)(pIFileDevice))
	{
	}
};