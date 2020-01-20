#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "SerializeFieldDefine.h"
#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"
#include "NameValueStream.h"

#include "ParameterStreamRW.h"






//////////////////////////////////////////////////////////////////////////

MethodStringDef(CreateInstanceFileDeviceWin32);
struct ParameterStruct(CreateInstanceFileDeviceWin32)
{
	SERIALIZE_BEGIN_STRUCT(ParameterCreateInstanceFileDeviceWin32, CreateInstanceFileDeviceWin32);
	CString strFileName; SERIALIZE_CSTRING(strFileName);
	ULONG ulDesiredAccess; SERIALIZE_BASIC(ulDesiredAccess);
	ULONG ulShareMode; SERIALIZE_BASIC(ulShareMode);
	ULONG ulCreationDisposition; SERIALIZE_BASIC(ulCreationDisposition);
	ULONG ulFlagsAndAttributes; SERIALIZE_BASIC(ulFlagsAndAttributes);
	//OUT IFileDevice ** ppFileDevice; 
	BOOL bCreateMember;SERIALIZE_BASIC(bCreateMember);
	SERIALIZE_END_STRUCT(ParameterCreateInstanceFileDeviceWin32, CreateInstanceFileDeviceWin32);

public:
	STRUCT_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};

MethodStringDef(SetPointerEx);
struct  ParameterStruct(SetPointerEx)
{

	SERIALIZE_BEGIN_STRUCT(ParameterSetPointerEx, SetPointerEx);
	LONGLONG llDistanceToMove; SERIALIZE_BASIC(llDistanceToMove);
	//LONGLONG* lpNewFilePointer; 
	ULONG ulMoveMethod;SERIALIZE_BASIC(ulMoveMethod);
	SERIALIZE_END_STRUCT(ParameterSetPointerEx, SetPointerEx);

public:
	STRUCT_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};

MethodStringDef(ResultSetPointerEx);
struct  ParameterStruct(ResultSetPointerEx)
{
	SERIALIZE_BEGIN_STRUCT(ParameterResultSetPointerEx, ResultSetPointerEx);
	HRESULT hrStatus;SERIALIZE_BASIC(hrStatus);
	CString strErrorMessage;SERIALIZE_CSTRING(strErrorMessage);
	//LONGLONG llDistanceToMove; 
	LONGLONG NewFilePointer; SERIALIZE_BASIC(NewFilePointer);
	//ULONG ulMoveMethod;
	SERIALIZE_END_STRUCT(ParameterResultSetPointerEx, ResultSetPointerEx);

public:
	STRUCT_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};

MethodStringDef(Read);
struct  ParameterStruct(Read)
{
	SERIALIZE_BEGIN_STRUCT(ParameterRead, Read);
	ULONG nNumberOfBytesToRead;SERIALIZE_BASIC(nNumberOfBytesToRead);
	//ULONG* lpNumberOfBytesRead;
	//BYTE* pBuffer;
	SERIALIZE_END_STRUCT(ParameterRead, Read);

public:
	STRUCT_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};


//<sonmi01>2013-7-4 ###???
MethodStringDef(ReadAtOffsetHelper);
struct  ParameterStruct(ReadAtOffsetHelper)
{
	SERIALIZE_BEGIN_STRUCT(ParameterReadAtOffsetHelper, ReadAtOffsetHelper);
	LONGLONG Offset; SERIALIZE_BASIC(Offset);
	ULONG nNumberOfBytesToRead;SERIALIZE_BASIC(nNumberOfBytesToRead);
	//ULONG* lpNumberOfBytesRead;
	//BYTE* pBuffer;
	SERIALIZE_END_STRUCT(ParameterReadAtOffsetHelper, ReadAtOffsetHelper);

public:
	STRUCT_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};

MethodStringDef(ResultRead);
struct  ParameterStruct(ResultRead)
{
	SERIALIZE_BEGIN_STRUCT(ParameterResultRead, ResultRead);
	HRESULT hrStatus;SERIALIZE_BASIC(hrStatus);
	CString strErrorMessage;SERIALIZE_CSTRING(strErrorMessage);
	//ULONG nNumberOfBytesToRead;
	ULONG NumberOfBytesRead;SERIALIZE_BASIC(NumberOfBytesRead);
	//BYTE* pBuffer;
	SERIALIZE_END_STRUCT(ParameterResultRead, ResultRead);

public:
	STRUCT_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};

MethodStringDef(Write);
struct  ParameterStruct(Write)
{
	SERIALIZE_BEGIN_STRUCT(ParameterWrite, Write);
	ULONG nNumberOfBytesToWrite;SERIALIZE_BASIC(nNumberOfBytesToWrite);
	//ULONG* lpNumberOfBytesWritten;
	//CONST BYTE* pBuffer;
	SERIALIZE_END_STRUCT(ParameterWrite, Write);

public:
	STRUCT_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};

MethodStringDef(WriteAtOffsetHelper);
struct  ParameterStruct(WriteAtOffsetHelper)
{
	SERIALIZE_BEGIN_STRUCT(ParameterWriteAtOffsetHelper, WriteAtOffsetHelper);
	LONGLONG Offset; SERIALIZE_BASIC(Offset);
	ULONG nNumberOfBytesToWrite;SERIALIZE_BASIC(nNumberOfBytesToWrite);
	//ULONG* lpNumberOfBytesWritten;
	//CONST BYTE* pBuffer;
	SERIALIZE_END_STRUCT(ParameterWriteAtOffsetHelper, WriteAtOffsetHelper);

public:
	STRUCT_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};

MethodStringDef(ResultWrite);
struct  ParameterStruct(ResultWrite)
{
	SERIALIZE_BEGIN_STRUCT(ParameterResultWrite, ResultWrite);
	HRESULT hrStatus;SERIALIZE_BASIC(hrStatus);
	CString strErrorMessage;SERIALIZE_CSTRING(strErrorMessage);
	//ULONG nNumberOfBytesToWrite;
	ULONG NumberOfBytesWritten;SERIALIZE_BASIC(NumberOfBytesWritten);
	//CONST BYTE* pBuffer;
	SERIALIZE_END_STRUCT(ParameterResultWrite, ResultWrite);

public:
	STRUCT_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};


MethodStringDef(GetFileSize);
struct  ParameterStruct(GetFileSize)
{
	SERIALIZE_BEGIN_STRUCT(ParameterGetFileSize, GetFileSize);
	//LONGLONG * pFileSize;
	SERIALIZE_END_STRUCT(ParameterGetFileSize, GetFileSize);

public:
	STRUCT_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);

};

MethodStringDef(ResultGetFileSize);
struct  ParameterStruct(ResultGetFileSize)
{
	SERIALIZE_BEGIN_STRUCT(ParameterResultGetFileSize, ResultGetFileSize);
	HRESULT hrStatus;SERIALIZE_BASIC(hrStatus);
	CString strErrorMessage;SERIALIZE_CSTRING(strErrorMessage);
	LONGLONG FileSize;SERIALIZE_BASIC(FileSize);
	SERIALIZE_END_STRUCT(ParameterResultGetFileSize, ResultGetFileSize);

public:
	STRUCT_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};