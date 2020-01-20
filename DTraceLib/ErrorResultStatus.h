#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <deque>

#include "..\PublicHeader\PublicGetCurrentModuleName.h"

#include "..\PublicHeader\PublicCodeBlock.h"

#include "..\PublicHeader\PublicValueXmlElementConvert.h"
#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\PublicHeader\PublicSimpleXml.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicParameterStreamDef.h"

#include "DTraceLib.h"

namespace ErrorResultStatus_NS
{
	struct ErrorElement
	{
		SERIALIZE_BEGIN_STRUCT(ErrorElement, ErrorElement);
		//////////////////////////////////////////////////////////////////////////
		HRESULT hr;						SERIALIZE_BASIC(hr);
		CString Message;				SERIALIZE_CSTRING(Message);
		CString LocallizedMessage;		SERIALIZE_CSTRING(LocallizedMessage);

		//////////////////////////////////////////////////////////////////////////
		CString HelpLink;				SERIALIZE_CSTRING(HelpLink);
		CString LocalizedHelpLink;		SERIALIZE_CSTRING(LocalizedHelpLink);
			
		//////////////////////////////////////////////////////////////////////////
		DWORD Pid;						SERIALIZE_BASIC(Pid);
		DWORD Tid;						SERIALIZE_BASIC(Tid);

		//////////////////////////////////////////////////////////////////////////
		CString Module;					SERIALIZE_CSTRING(Module);
		CString Function;				SERIALIZE_CSTRING(File);
		CString File;					SERIALIZE_CSTRING(Line);
		LONG Line;

		//////////////////////////////////////////////////////////////////////////
		ULONG Enum;						SERIALIZE_BASIC(Enum);
		ULONG Flags;					SERIALIZE_BASIC(Flags);
		ULONG MoreData;					SERIALIZE_BASIC(MoreData);
		CString MoreString;				SERIALIZE_CSTRING(MoreString);
		SERIALIZE_END_STRUCT(ErrorElement, ErrorElement);

		STRUCT_XML_SERIALIZER;


		//////////////////////////////////////////////////////////////////////////
		//__DECLSPEC_DLLAPI
		static void InitError(
			ErrorElement & err,
			HRESULT hr,
			LPCTSTR Message,
			LPCTSTR Module,
			LPCTSTR Function,
			LPCTSTR File,
			LONG Line,
			...
			);
	}; //end struct ErrorElement

	struct ErrorCollection
	{
		SERIALIZE_BEGIN_STRUCT(ErrorCollection, ErrorCollection);
		ErrorElement CurrentError;				SERIALIZE_COMPLEX(CurrentError);
		std::deque<ErrorElement> LastErrors;	SERIALIZE_COMPLEX_VECTOR(LastErrors);
		SERIALIZE_END_STRUCT(ErrorCollection, ErrorCollection);
		
		STRUCT_XML_SERIALIZER;

		//////////////////////////////////////////////////////////////////////////
		//__DECLSPEC_DLLAPI
		static void AddCurrentError(ErrorCollection & errors);
	}; //end struct ErrorCollection

} //end namespace ErrorResultStatus_NS


#define ERS_INIT_CURRENT_ERROR(errors, hr, Message, ...)			ErrorResultStatus_NS::ErrorElement::InitError((errors).CurrentError, hr, Message, GetCurrentModuleName(), TEXT(__FUNCTION__), TEXT(__FILE__),  __LINE__, __VA_ARGS__);
#define ERS_ADD_CURRENT_ERROR(errors)								ErrorResultStatus_NS::ErrorCollection::AddCurrentError(errors);

#define ERS_INIT_ADD_CURRENT_ERROR(errors, hr, Message, ...)		ERS_INIT_CURRENT_ERROR(errors, hr, Message,  __VA_ARGS__); ERS_ADD_CURRENT_ERROR(errors);
