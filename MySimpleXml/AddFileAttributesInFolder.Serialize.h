#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <vector>


#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"

#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\PublicHeader\PublicSimpleXml.h"
#include "..\PublicHeader\PublicValueXmlElementConvert.h"
#include "..\PublicHeader\PublicParameterStreamDef.h"


struct AddFileAttributesInFolderSerialize
{
	SERIALIZE_BEGIN_STRUCT(AddFileAttributesInFolderSerialize, AddFileAttributesInFolderSerialize);
	std::vector<CString> FileExtensions;	SERIALIZE_VECTOR(FileExtensions);
	SERIALIZE_END_STRUCT(AddFileAttributesInFolderSerialize, AddFileAttributesInFolderSerialize);

	STRUCT_XML_SERIALIZER;
};