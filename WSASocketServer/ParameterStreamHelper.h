#pragma once

#include "ParameterStreamRW.h"

//////////////////////////////////////////////////////////////////////////
MethodStringDef(AddRef);
struct  ParameterStruct(AddRef)
{
	SERIALIZE_BEGIN_STRUCT(ParameterAddRef, AddRef);
	SERIALIZE_END_STRUCT(ParameterAddRef, AddRef);

public:
	STRUCT_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};

MethodStringDef(Release);
struct  ParameterStruct(Release)
{
	SERIALIZE_BEGIN_STRUCT(ParameterRelease, Release);
	SERIALIZE_END_STRUCT(ParameterRelease, Release);

public:
	STRUCT_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};

MethodStringDef(CreateMember);
struct  ParameterStruct(CreateMember)
{
	SERIALIZE_BEGIN_STRUCT(ParameterCreateMember, CreateMember);
	SERIALIZE_END_STRUCT(ParameterCreateMember, CreateMember);

public:
	STRUCT_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};

MethodStringDef(DestroyMember);
struct  ParameterStruct(DestroyMember)
{
	SERIALIZE_BEGIN_STRUCT(ParameterDestroyMember, DestroyMember);
	SERIALIZE_END_STRUCT(ParameterDestroyMember, DestroyMember);

public:
	STRUCT_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};



//////////////////////////////////////////////////////////////////////////
MethodStringDef(Result);
struct  ParameterStruct(Result)
{
	SERIALIZE_BEGIN_STRUCT(ParameterResult, Result);
	HRESULT hrStatus; SERIALIZE_BASIC(hrStatus);
	CString strErrorMessage; SERIALIZE_CSTRING(strErrorMessage);
	SERIALIZE_END_STRUCT(ParameterResult, Result);

public:
	STRUCT_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};
