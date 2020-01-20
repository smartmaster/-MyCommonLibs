#pragma once

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "SocketFileDeviceHandler.h"
#include "InterfaceWSASocketServerModule.h"
#include "SocketRunAppHandler.h"
#include "WSALibInit.h"

#include "SocketFileRPCDefines.h"

#include "..\PublicHeader\PublicSimpleXml.h"
#include "..\PublicHeader\PublicValueXmlElementConvert.h"

//////////////////////////////////////////////////////////////////////////
//#define simple_t_FakeRawArr_size _countof(FakeRawArr)

MethodStringDef(my_simple_t);
struct my_simple_t 
{
	SERIALIZE_BEGIN_STRUCT(my_simple_t, my_simple_t);
	int ii;							SERIALIZE_BASIC(ii);
	LONGLONG ll;					SERIALIZE_BASIC(ll);
	CString cstr;					SERIALIZE_CSTRING(cstr);
	wstring stdstr;					SERIALIZE_STDSTRING(stdstr);
	WCHAR fixedArr[128];			SERIALIZE_FIXED_ARRAY(fixedArr);
	WCHAR FakeRawArr[256];			SERIALIZE_RAW_ARRAY(  FakeRawArr  ,   _countof(FakeRawArr) );
	SERIALIZE_END_STRUCT(my_simple_t, my_simple_t);

	void Init()
	{
		ii = 1111;
		ll = 22222;
		cstr = TEXT("3333");
		stdstr = TEXT("44444");
		_tcscpy_s(fixedArr, TEXT("555555555555555555555"));
		_tcscpy_s(FakeRawArr, TEXT("666666666666666666666666666666666"));
	}

	STRUCT_SERIALIZER;
	STRUCT_XML_SERIALIZER;
};

MethodStringDef(my_complex_t);
struct my_complex_t
{
	SERIALIZE_BEGIN_STRUCT(my_complex_t, my_complex_t);
	int ii;					SERIALIZE_BASIC(ii);
	LONGLONG ll;			SERIALIZE_BASIC(ll);
	CString cstr;			SERIALIZE_CSTRING(cstr);
	wstring stdstr;			SERIALIZE_STDSTRING(stdstr);
	WCHAR fixedArr[128];	SERIALIZE_FIXED_ARRAY(fixedArr);
	WCHAR FakeRawArr[256];	SERIALIZE_RAW_ARRAY(  FakeRawArr  ,   _countof(FakeRawArr) );
	my_simple_t t1;			SERIALIZE_COMPLEX(t1);
	my_simple_t t2;			SERIALIZE_COMPLEX(t2);
	SERIALIZE_END_STRUCT(my_complex_t, my_complex_t);

	void Init()
	{
		ii = 1111;
		ll = 22222;
		cstr = TEXT("3333");
		stdstr = TEXT("44444");
		_tcscpy_s(fixedArr, TEXT("555555555555555555555"));
		_tcscpy_s(FakeRawArr, TEXT("666666666666666666666666666666666"));
		t1.Init();
		t2.Init();
	}


	STRUCT_SERIALIZER;
	STRUCT_XML_SERIALIZER;
};

MethodStringDef(my_very_complex_t);
struct my_very_complex_t
{
	SERIALIZE_BEGIN_STRUCT(my_very_complex_t, my_very_complex_t);
	int ii;					SERIALIZE_BASIC(ii);
	LONGLONG ll;			SERIALIZE_BASIC(ll);
	CString cstr;			SERIALIZE_CSTRING(cstr);
	wstring stdstr;			SERIALIZE_STDSTRING(stdstr);
	WCHAR fixedArr[128];	SERIALIZE_FIXED_ARRAY(fixedArr);
	WCHAR FakeRawArr[256];	SERIALIZE_RAW_ARRAY(  FakeRawArr  ,   _countof(FakeRawArr) );
	my_simple_t t1;			SERIALIZE_COMPLEX(t1);
	my_simple_t t2;			SERIALIZE_COMPLEX(t2);
	my_complex_t ct1;		SERIALIZE_COMPLEX(ct1);
	my_complex_t ct2;		SERIALIZE_COMPLEX(ct2);
	SERIALIZE_END_STRUCT(my_very_complex_t, my_very_complex_t);

	void Init()
	{
		ii = 1111;
		ll = 22222;
		cstr = TEXT("3333");
		stdstr = TEXT("44444");
		_tcscpy_s(fixedArr, TEXT("555555555555555555555"));
		_tcscpy_s(FakeRawArr, TEXT("666666666666666666666666666666666"));
		t1.Init();
		t2.Init();
		ct1.Init();
		ct2.Init();
	}


	STRUCT_SERIALIZER;
	STRUCT_XML_SERIALIZER;
};

MethodStringDef(my_hard_t);
struct my_hard_t
{
	SERIALIZE_BEGIN_STRUCT(my_hard_t, my_hard_t);
	int ii;					SERIALIZE_BASIC(ii);
	LONGLONG ll;			SERIALIZE_BASIC(ll);
	CString cstr;			SERIALIZE_CSTRING(cstr);
	wstring stdstr;			SERIALIZE_STDSTRING(stdstr);
	WCHAR fixedArr[128];	SERIALIZE_FIXED_ARRAY(fixedArr);
	WCHAR FakeRawArr[256];	SERIALIZE_RAW_ARRAY(  FakeRawArr  ,   _countof(FakeRawArr) );
	my_simple_t t1;			SERIALIZE_COMPLEX(t1);
	my_simple_t t2;			SERIALIZE_COMPLEX(t2);
	my_complex_t ct1;		SERIALIZE_COMPLEX(ct1);
	my_complex_t ct2;		SERIALIZE_COMPLEX(ct2);

	vector<CHAR> vecVh;					SERIALIZE_VECTOR(vecVh);
	vector<CString> vecCstr;			SERIALIZE_VECTOR(vecCstr);
	vector<my_simple_t> vecSimple;		SERIALIZE_COMPLEX_VECTOR(vecSimple);
	vector<my_complex_t> vecComplex;	SERIALIZE_COMPLEX_VECTOR(vecComplex);
	vector<wstring> vecstdstr;			SERIALIZE_VECTOR(vecstdstr);

	vector<CHAR> vecVh1;				SERIALIZE_VECTOR(vecVh1);
	vector<CString> vecCstr1;			SERIALIZE_VECTOR(vecCstr1);
	vector<my_simple_t> vecSimple1;		SERIALIZE_COMPLEX_VECTOR(vecSimple1);
	vector<my_complex_t> vecComplex1;	SERIALIZE_COMPLEX_VECTOR(vecComplex1);
	vector<wstring> vecstdstr1;			SERIALIZE_VECTOR(vecstdstr1);

	SERIALIZE_END_STRUCT(my_hard_t, my_hard_t);
	

	void Init()
	{
		ii = 1111;
		ll = 22222;
		cstr = TEXT("3333");
		stdstr = TEXT("44444");
		_tcscpy_s(fixedArr, TEXT("555555555555555555555"));
		_tcscpy_s(FakeRawArr, TEXT("666666666666666666666666666666666"));
		t1.Init();
		t2.Init();
		ct1.Init();
		ct2.Init();

		vecSimple.resize(3);
		vecComplex.resize(3);
		//vecstdstr.resize(3);

		vecSimple1.resize(3);
		vecComplex1.resize(3);
		//vecstdstr1.resize(3);

		for (int ii = 0; ii < 3; ++ ii)
		{
			vecVh.push_back('7');
			vecCstr.push_back(TEXT("888"));
			vecSimple[ii].Init();
			vecComplex[ii].Init();
			vecstdstr.push_back(TEXT("999"));

			vecVh1.push_back('7');
			vecCstr1.push_back(TEXT("888"));
			vecSimple1[ii].Init();
			vecComplex1[ii].Init();
			vecstdstr1.push_back(TEXT("999"));
		}

	}

	STRUCT_SERIALIZER;
	STRUCT_XML_SERIALIZER;
};

MethodStringDef(my_hard_array_t);
struct  my_hard_array_t
{
	SERIALIZE_BEGIN_STRUCT(my_hard_array_t, my_hard_array_t);
	vector<my_hard_t> arr; SERIALIZE_COMPLEX_VECTOR(arr);
	SERIALIZE_END_STRUCT(my_hard_array_t, my_hard_array_t);

	void Init()
	{
		CONST INT SIZE = 10;
		arr.resize(10);
		for (int ii = 0; ii < SIZE; ++ ii)
		{
			arr[ii].Init();
		}
	}

	STRUCT_SERIALIZER;
	STRUCT_XML_SERIALIZER;
};



