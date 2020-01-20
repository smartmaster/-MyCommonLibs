#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <vector>
#include <algorithm>
using namespace std;

#include <msxml6.h>

#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "SimpleXml.h"

struct simple
{
	SERIALIZE_BEGIN_STRUCT(simple, simple);
	SHORT s;						SERIALIZE_BASIC(s);
	ULONGLONG ull;					SERIALIZE_BASIC(ull);
	CString cstr;					SERIALIZE_CSTRING(cstr);
	vector<SHORT> vecs;				SERIALIZE_VECTOR(vecs);
	vector<ULONGLONG> vecull;		SERIALIZE_VECTOR(vecull);
	vector<CString> vecstr;			SERIALIZE_VECTOR(vecstr);
	SERIALIZE_END_STRUCT(simple, simple);

	VOID Init()
	{
		s = -1;
		ull = -1;
		cstr = TEXT("hello world");
		for (int ii = 0; ii < 5; ++ ii)
		{
			vecs.push_back(-ii);
			vecull.push_back(-ii);
			vecstr.push_back(TEXT("hoooooo"));
		}
	}

	STRUCT_XML_SERIALIZER;
};

struct complex_1
{
	SERIALIZE_BEGIN_STRUCT(complex_1, complex_1);
	simple sub;				SERIALIZE_COMPLEX(sub);
	vector<simple> vecsub;	SERIALIZE_COMPLEX_VECTOR(vecsub);
	SERIALIZE_END_STRUCT(complex_1, complex_1);

	VOID Init()
	{
		sub.Init();
		vecsub.resize(3);
		for (size_t ii = 0; ii < vecsub.size(); ++ ii)
		{
			vecsub[ii].Init();
		}
	}


	STRUCT_XML_SERIALIZER;
};

struct  sssvecstr
{
	SERIALIZE_BEGIN_STRUCT(sssvecstr, sssvecstr);
	vector<CString> vecs;	SERIALIZE_VECTOR(vecs);
	SERIALIZE_END_STRUCT(sssvecstr, sssvecstr);

	STRUCT_XML_SERIALIZER;
};


struct SSSSBufferTest
{
	SERIALIZE_BEGIN_STRUCT(SSSSBufferTest, SSSSBufferTest);
	VectorBYTE buffer1;				SERIALIZE_BASIC(buffer1);
	VectorBYTE buffer2;				SERIALIZE_BASIC(buffer2);

	vector<VectorBYTE > vecBuffer1;	SERIALIZE_VECTOR(vecBuffer1);
	vector<VectorBYTE > vecBuffer2;	SERIALIZE_VECTOR(vecBuffer2);

	GUID guid;							SERIALIZE_BASIC(guid);
	vector<GUID> vecGuid;				SERIALIZE_VECTOR(vecGuid);

	vector<vector<CString> >	vecsdStr; SERIALIZE_VECTOR_2D(vecsdStr);

	vector<vector<complex_1> > vecComplex2D; SERIALIZE_COMPLEX_VECTOR_2D(vecComplex2D);

	vector<sssvecstr> vecvec;			SERIALIZE_COMPLEX_VECTOR(vecvec);
	SERIALIZE_END_STRUCT(SSSSBufferTest, SSSSBufferTest);

	VOID Init()
	{


		static CONST WCHAR raw[] = L"some text";

		vecsdStr.resize(3);
		for (size_t ii = 0; ii < vecsdStr.size(); ++ii)
		{
			for (int jj = 0; jj < 3; ++ jj)
			{
				vecsdStr[ii].push_back(raw);
			}	
		}

		vecComplex2D.resize(3);
		for (size_t ii = 0; ii < vecComplex2D.size(); ++ii)
		{
			vecComplex2D[ii].resize(2);
			for (int jj = 0; jj < vecComplex2D[ii].size(); ++jj)
			{
				vecComplex2D[ii][jj].Init();
			}
		}

		BufferToVectorHelper(raw, sizeof(raw), buffer1/*.Buffer*//*public vector<byte>*/);
		BufferToVectorHelper(raw, sizeof(raw), buffer2/*.Buffer*//*public vector<byte>*/);

		vecBuffer1.resize(3);
		for (size_t ii = 0; ii < vecBuffer1.size(); ++ ii)
		{
			BufferToVectorHelper(raw, sizeof(raw), vecBuffer1[ii]/*.Buffer*//*public vector<byte>*/);
		}

		vecBuffer2.resize(3);
		for (size_t ii = 0; ii < vecBuffer2.size(); ++ii)
		{
			BufferToVectorHelper(raw, sizeof(raw), vecBuffer2[ii]/*.Buffer*//*public vector<byte>*/);
		}

		guid = __uuidof(IDispatch);

		int ind = 0;
		std::generate_n(back_inserter(vecGuid), 10,
			[ind]() mutable
		{
			++ind;

			if (ind & 1)
			{
				return __uuidof(IDispatch);
			}
			else
			{
				return __uuidof(IClassFactory);
			}

	
		}
			);

		vecvec.resize(3);
		for (size_t ii = 0; ii < vecvec.size(); ++ii)
		{
			vecvec[ii].vecs.resize(2);
			for (size_t jj = 0; jj < vecvec[ii].vecs.size(); ++jj)
			{
				vecvec[ii].vecs[jj].Format(TEXT("hello %02d %02d"), ii, jj);
			}
		}
		
	}


	STRUCT_XML_SERIALIZER;
};


struct vec2d_simple
{
	SERIALIZE_BEGIN_STRUCT(vec2d_simple, vec2d_simple);
	vector<vector<CString> > v2str;		SERIALIZE_VECTOR_2D(v2str);
	vector<vector<GUID> > vvguid;		SERIALIZE_VECTOR_2D(vvguid);
	SERIALIZE_END_STRUCT(vec2d_simple, vec2d_simple);

	STRUCT_XML_SERIALIZER;

	void init()
	{
		//////////////////////////////////////////////////////////////////////////
		v2str.resize(2);
		for (size_t ii = 0; ii < v2str.size(); ++ ii)
		{
			v2str[ii].resize(3);
			for (size_t jj = 0; jj < v2str[ii].size(); ++jj)
			{
				v2str[ii][jj] = L"hello world";
			}
		}


		//////////////////////////////////////////////////////////////////////////
		vvguid.resize(2);
		for (size_t ii = 0; ii < vvguid.size(); ++ii)
		{
			vvguid[ii].resize(3);
			for (size_t jj = 0; jj < vvguid[ii].size(); ++jj)
			{
				vvguid[ii][jj] = __uuidof(IDispatch);
			}
		}
	}
};


struct vec2d_complex
{
	SERIALIZE_BEGIN_STRUCT(vec2d_complex, vec2d_complex);
	vector<vector<vec2d_simple> > vvcomplex; SERIALIZE_COMPLEX_VECTOR_2D(vvcomplex);
	SERIALIZE_END_STRUCT(vec2d_complex, vec2d_complex);

	STRUCT_XML_SERIALIZER;

	void init()
	{
		//////////////////////////////////////////////////////////////////////////
		vvcomplex.resize(2);
		for (size_t ii = 0; ii < vvcomplex.size(); ++ii)
		{
			vvcomplex[ii].resize(3);
			for (size_t jj = 0; jj < vvcomplex[ii].size(); ++jj)
			{
				vvcomplex[ii][jj].init();
			}
		}
	}
};



//////////////////////////////////////////////////////////////////////////

static void initv1(vector<GUID> & v1, size_t count)
{
	v1.resize(count);
	for (auto & var : v1)
	{
		var = __uuidof(IDispatch);
	}
}

static void initv2(vector<vector<GUID> > & v2, size_t count)
{
	v2.resize(count);
	for (auto & var : v2)
	{
		initv1(var, count);
	}
}

static void initv3(vector<vector<vector<GUID> > > & v3, size_t count)
{
	v3.resize(count);
	for (auto & var : v3)
	{
		initv2(var, count);
	}
}



struct ut_simle
{
	SERIALIZE_BEGIN_STRUCT(ut_simle, ut_simle);
	GUID x;										SERIALIZE_BASIC(x);
	vector<GUID> vx;							SERIALIZE_VECTOR(vx);
	vector<vector<GUID> > v2x;					SERIALIZE_VECTOR_2D(v2x);
	vector<vector<vector<GUID> >  > v3x;		SERIALIZE_VECTOR_3D(v3x);
	SERIALIZE_END_STRUCT(ut_simle, ut_simle);

	STRUCT_XML_SERIALIZER;

	void init()
	{
		x = __uuidof(IDispatch);

		CONST size_t ELE_COUNT = 2;
		initv1(vx, ELE_COUNT);
		initv2(v2x, ELE_COUNT);
		initv3(v3x, ELE_COUNT);
	}
};


static void initv1(vector<ut_simle> & v1, size_t count)
{
	v1.resize(count);
	for (auto & var : v1)
	{
		var.init();
	}
}

static void initv2(vector<vector<ut_simle> > & v2, size_t count)
{
	v2.resize(count);
	for (auto & var : v2)
	{
		initv1(var, count);
	}
}

static void initv3(vector<vector<vector<ut_simle> > > & v3, size_t count)
{
	v3.resize(count);
	for (auto & var : v3)
	{
		initv2(var, count);
	}
}

struct ut_complex
{
	SERIALIZE_BEGIN_STRUCT(ut_complex, ut_complex);
	ut_simle x;									SERIALIZE_COMPLEX(x);
	vector<ut_simle> vx;						SERIALIZE_COMPLEX_VECTOR(vx);
	vector<vector<ut_simle> > v2x;				SERIALIZE_COMPLEX_VECTOR_2D(v2x);
	vector<vector<vector<ut_simle> > > v3x;		SERIALIZE_COMPLEX_VECTOR_3D(v3x);
	SERIALIZE_END_STRUCT(ut_complex, ut_complex);

	STRUCT_XML_SERIALIZER;

	void init()
	{
		x.init();

		CONST size_t ELE_COUNT = 2;
		initv1(vx, ELE_COUNT);
		initv2(v2x, ELE_COUNT);
		initv3(v3x, ELE_COUNT);
	}
};
//////////////////////////////////////////////////////////////////////////


struct tsimple
{
	SERIALIZE_BEGIN_STRUCT(tsimple, tsimple);
	bool bool_Value;				SERIALIZE_BASIC(bool_Value);
	bool bool_Value1;				SERIALIZE_BASIC(bool_Value1);
	CString CString_Value;			SERIALIZE_BASIC(CString_Value);
	wstring wstring_Value;			SERIALIZE_BASIC(wstring_Value);
	LONGLONG LONGLONG_Value;			SERIALIZE_BASIC(LONGLONG_Value);
	ULONGLONG ULONGLONG_Value;			SERIALIZE_BASIC(ULONGLONG_Value);
	LONG LONG_Value;			SERIALIZE_BASIC(LONG_Value);
	ULONG ULONG_Value;			SERIALIZE_BASIC(ULONG_Value);
	CHAR CHAR_Value;			SERIALIZE_BASIC(CHAR_Value);
	UCHAR UCHAR_Value;			SERIALIZE_BASIC(UCHAR_Value);
	SHORT SHORT_Value;			SERIALIZE_BASIC(SHORT_Value);
	USHORT USHORT_Value;			SERIALIZE_BASIC(USHORT_Value);
	INT INT_Value;			SERIALIZE_BASIC(INT_Value);
	UINT UINT_Value;			SERIALIZE_BASIC(UINT_Value);
	VectorBYTE vector_BYTE_Value; SERIALIZE_BASIC(vector_BYTE_Value);
	GUID GUID_Value;			SERIALIZE_BASIC(GUID_Value);
	string string_Value;			SERIALIZE_BASIC(string_Value);
	CStringA CStringA_Value;			SERIALIZE_BASIC(CStringA_Value);
	SYSTEMTIME SYSTEMTIME_Value;			SERIALIZE_BASIC(SYSTEMTIME_Value);
	DOUBLE DOUBLE_Value;	SERIALIZE_BASIC(DOUBLE_Value);
	FLOAT FLOAT_Value;		SERIALIZE_BASIC(FLOAT_Value);
	BYTE BYTE_Value;		SERIALIZE_BASIC(BYTE_Value);

	list<bool> vector_bool_Value;			SERIALIZE_VECTOR(vector_bool_Value);
	list<bool> vector_bool_Value1;			SERIALIZE_VECTOR(vector_bool_Value1);
	vector<CString> vector_CString_Value;			SERIALIZE_VECTOR(vector_CString_Value);
	vector<wstring> vector_wstring_Value;			SERIALIZE_VECTOR(vector_wstring_Value);
	vector<LONGLONG> vector_LONGLONG_Value;			SERIALIZE_VECTOR(vector_LONGLONG_Value);
	vector<ULONGLONG> vector_ULONGLONG_Value;			SERIALIZE_VECTOR(vector_ULONGLONG_Value);
	vector<LONG> vector_LONG_Value;			SERIALIZE_VECTOR(vector_LONG_Value);
	vector<ULONG> vector_ULONG_Value;			SERIALIZE_VECTOR(vector_ULONG_Value);
	vector<CHAR> vector_CHAR_Value;			SERIALIZE_VECTOR(vector_CHAR_Value);
	vector<UCHAR> vector_UCHAR_Value;			SERIALIZE_VECTOR(vector_UCHAR_Value);
	vector<SHORT> vector_SHORT_Value;			SERIALIZE_VECTOR(vector_SHORT_Value);
	vector<USHORT> vector_USHORT_Value;			SERIALIZE_VECTOR(vector_USHORT_Value);
	vector<INT> vector_INT_Value;			SERIALIZE_VECTOR(vector_INT_Value);
	vector<UINT> vector_UINT_Value;			SERIALIZE_VECTOR(vector_UINT_Value);
	vector<VectorBYTE > vector_vector_BYTE_Value;			SERIALIZE_VECTOR(vector_vector_BYTE_Value);
	vector<GUID> vector_GUID_Value;			SERIALIZE_VECTOR(vector_GUID_Value);
	vector<string> vector_string_Value;			SERIALIZE_VECTOR(vector_string_Value);
	vector<CStringA> vector_CStringA_Value;			SERIALIZE_VECTOR(vector_CStringA_Value);
	vector<SYSTEMTIME> vector_SYSTEMTIME_Value;			SERIALIZE_VECTOR(vector_SYSTEMTIME_Value);
	vector<DOUBLE> vector_DOUBLE_Value;			SERIALIZE_VECTOR(vector_DOUBLE_Value);
	vector<FLOAT> vector_FLOAT_Value;			SERIALIZE_VECTOR(vector_FLOAT_Value);
	vector<BYTE> vector_basic_BYTE_Value;			SERIALIZE_VECTOR(vector_basic_BYTE_Value);
	SERIALIZE_END_STRUCT(tsimple, tsimple);
	STRUCT_XML_SERIALIZER;

	void Init()
	{
		bool_Value = false;
		bool_Value1 = true;
		CString_Value = L"88";
		wstring_Value = 88;
		LONGLONG_Value = 88;
		ULONGLONG_Value = 88;
		LONG_Value = 88;
		ULONG_Value = 88;
		CHAR_Value = 88;
		UCHAR_Value = 88;
		SHORT_Value = 88;
		USHORT_Value = 88;
		INT_Value = 88;
		UINT_Value = 88;
		vector_BYTE_Value/*.Buffer*//*public vector<byte>*/.resize(3); for (auto & item : vector_BYTE_Value/*.Buffer*//*public vector<byte>*/){item = 88;}
		GUID_Value = { 88 };
		string_Value = "88";
		CStringA_Value = "88";
		SYSTEMTIME_Value = { 88 };
		DOUBLE_Value = 88;
		FLOAT_Value = 88;
		BYTE_Value = 88;
		
		vector_bool_Value.resize(3); for (auto & item : vector_bool_Value){ item = false; }
		vector_bool_Value1.resize(3); for (auto & item : vector_bool_Value1){ item = true; }
		vector_CString_Value.resize(3); for (auto & item : vector_CString_Value){item = L"888";}
		vector_wstring_Value.resize(3); for (auto & item : vector_wstring_Value){item = 88;}
		vector_LONGLONG_Value.resize(3); for (auto & item : vector_LONGLONG_Value){item = 88;}
		vector_ULONGLONG_Value.resize(3); for (auto & item : vector_ULONGLONG_Value){item = 88;}
		vector_LONG_Value.resize(3); for (auto & item : vector_LONG_Value){item = 88;}
		vector_ULONG_Value.resize(3); for (auto & item : vector_ULONG_Value){item = 88;}
		vector_CHAR_Value.resize(3); for (auto & item : vector_CHAR_Value){item = 88;}
		vector_UCHAR_Value.resize(3); for (auto & item : vector_UCHAR_Value){item = 88;}
		vector_SHORT_Value.resize(3); for (auto & item : vector_SHORT_Value){item = 88;}
		vector_USHORT_Value.resize(3); for (auto & item : vector_USHORT_Value){item = 88;}
		vector_INT_Value.resize(3); for (auto & item : vector_INT_Value){item = 88;}
		vector_UINT_Value.resize(3); for (auto & item : vector_UINT_Value){item = 88;}
		vector_vector_BYTE_Value.resize(3); for (auto & item : vector_vector_BYTE_Value){ item/*.Buffer*//*public vector<byte>*/.resize(3); for (auto & item1 : item/*.Buffer*//*public vector<byte>*/) { item1 = 88; } }
		vector_GUID_Value.resize(3); for (auto & item : vector_GUID_Value){ item = { 888 }; }
		vector_string_Value.resize(3); for (auto & item : vector_string_Value){item = 88;}
		vector_CStringA_Value.resize(3); for (auto & item : vector_CStringA_Value){item = "888";}
		vector_SYSTEMTIME_Value.resize(3); for (auto & item : vector_SYSTEMTIME_Value){ item = { 888 }; }

		vector_DOUBLE_Value.resize(3); for (auto & item : vector_DOUBLE_Value){ item = 88; }
		vector_FLOAT_Value.resize(3); for (auto & item : vector_FLOAT_Value){ item = 88; }
		vector_basic_BYTE_Value.resize(3); for (auto & item : vector_basic_BYTE_Value) { item = 88; }
	}
};

struct tcomplex
{
	SERIALIZE_BEGIN_STRUCT(tcomplex, tcomplex);
	tsimple tsimple_Value;			SERIALIZE_BASIC(tsimple_Value);
	vector<tsimple> vector_tsimple_Value;			SERIALIZE_VECTOR(vector_tsimple_Value);
	SERIALIZE_END_STRUCT(tcomplex, tcomplex);
	STRUCT_XML_SERIALIZER;

	void Init()
	{
		tsimple_Value.Init();
		vector_tsimple_Value.resize(3); 
		for (auto & item : vector_tsimple_Value)
		{
			item.Init();
		}
	}
};


struct tcomplex2
{
	SERIALIZE_BEGIN_STRUCT(tcomplex2, tcomplex2);
	tcomplex tcomplex_Value;								SERIALIZE_BASIC(tcomplex_Value);
	vector<tcomplex> vector_tcomplex_Value;					SERIALIZE_VECTOR(vector_tcomplex_Value);
	vector<vector<vector<tcomplex> > > v3_tcomplex;			SERIALIZE_VECTOR(v3_tcomplex);
	list<list<list<tcomplex> > > l3_tcomplex;				SERIALIZE_VECTOR(l3_tcomplex);
	deque<deque<deque<tcomplex> > > q3_tcomplex;			SERIALIZE_VECTOR(q3_tcomplex);
	deque<deque<deque<deque<tcomplex> > > > q4_tcomplex;	SERIALIZE_VECTOR(q4_tcomplex);
	SERIALIZE_END_STRUCT(tcomplex2, tcomplex2);
	STRUCT_XML_SERIALIZER;

	template<typename T>
	static void Init3D(T & container)
	{
		container.resize(2);
		for (auto & i0 : container)
		{
			i0.resize(2);
			for (auto & i1 : i0)
			{
				i1.resize(2);
				for (auto & i2 : i1)
				{
					i2.Init();
				}
			}
		}
	}

	void Init()
	{
		//////////////////////////////////////////////////////////////////////////
		tcomplex_Value.Init();

		//////////////////////////////////////////////////////////////////////////
		vector_tcomplex_Value.resize(3);
		for (auto & item : vector_tcomplex_Value)
		{
			item.Init();
		}

		//////////////////////////////////////////////////////////////////////////
		Init3D(v3_tcomplex);
		Init3D(l3_tcomplex);
		Init3D(q3_tcomplex);
	}

	static void UT()
	{
		//////////////////////////////////////////////////////////////////////////
		tcomplex2 var1;
		var1.Init();
		var1.ToFile(FIELD_NAME(tcomplex2), TEXT("var1.xml"));

		//////////////////////////////////////////////////////////////////////////
		tcomplex2 var2;
		var2.FromFile(FIELD_NAME(tcomplex2), TEXT("var1.xml"));
		var2.ToFile(FIELD_NAME(tcomplex2), TEXT("var2.xml"));

		//bool eq = (var1 == var2);
		//ATLASSERT(eq);
	}
};
