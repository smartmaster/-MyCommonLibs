#pragma once


#include "..\PublicHeader\PublicPluginFileFactoryInterface.h"
#include "..\PublicHeader\PublicPluginFileFactoryConfig.h"
#include "..\PublicHeader\PublicDiskImageInterface.h"
#include "..\PublicHeader\PublicCreateWin32FileHelper.h"
#include "..\PublicHeader\PublicVMemPtr.h"
#include "..\PublicHeader\PublicSimpleBitArray.h"
#include "..\PublicHeader\PublicMySystemInfo.h"
#include "..\PublicHeader\PublicAllVolumeDiskInfoXml.h"

#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "..\PublicHeader\PublicSimpleXml.h"
#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\PublicHeader\PublicValueXmlElementConvert.h"
#include "..\PublicHeader\PublicParameterStreamDef.h"


struct CMyAllSystemInfo
{
	SERIALIZE_BEGIN_STRUCT(CMyAllSystemInfo, CMyAllSystemInfo);
	My_System_Info System_Info;										SERIALIZE_COMPLEX(System_Info);
	My_MSFT_DisksPartitionsVolumes MSFT_DisksPartitionsVolumes;		SERIALIZE_COMPLEX(MSFT_DisksPartitionsVolumes);
	CAllVolumeDiskInfoXml AllVolumeDiskInfoXml;						SERIALIZE_COMPLEX(AllVolumeDiskInfoXml);
	SERIALIZE_END_STRUCT(CMyAllSystemInfo, CMyAllSystemInfo);
	STRUCT_XML_SERIALIZER;


//private:
//	CString m_machine;
//	CString m_user;
//	CString m_domain;
//	CString m_password;

public:
	CMyAllSystemInfo(
		LPCTSTR machine = nullptr,
		LPCTSTR user = nullptr,
		LPCTSTR domain = nullptr,
		LPCTSTR password = nullptr
		);

	HRESULT Retrieve();
};