#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>
#include <list>
#include <deque>
#include <string>
#include <algorithm>

using namespace std;

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"

#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\PublicHeader\PublicSimpleXml.h"
#include "..\PublicHeader\PublicValueXmlElementConvert.h"
#include "..\PublicHeader\PublicParameterStreamDef.h"




namespace NS_testSerializeSample
{
	struct tsimple
	{
		SERIALIZE_BEGIN_STRUCT(tsimple, tsimple);
		bool bool_Value;								SERIALIZE_BASIC(bool_Value);
		bool bool_Value1;								SERIALIZE_BASIC(bool_Value1);
		CString CString_Value;							SERIALIZE_BASIC(CString_Value);
		wstring wstring_Value;							SERIALIZE_BASIC(wstring_Value);
		LONGLONG LONGLONG_Value;						SERIALIZE_BASIC(LONGLONG_Value);
		ULONGLONG ULONGLONG_Value;						SERIALIZE_BASIC(ULONGLONG_Value);
		LONG LONG_Value;								SERIALIZE_BASIC(LONG_Value);
		ULONG ULONG_Value;								SERIALIZE_BASIC(ULONG_Value);
		CHAR CHAR_Value;								SERIALIZE_BASIC(CHAR_Value);
		UCHAR UCHAR_Value;								SERIALIZE_BASIC(UCHAR_Value);
		SHORT SHORT_Value;								SERIALIZE_BASIC(SHORT_Value);
		USHORT USHORT_Value;							SERIALIZE_BASIC(USHORT_Value);
		INT INT_Value;									SERIALIZE_BASIC(INT_Value);
		UINT UINT_Value;								SERIALIZE_BASIC(UINT_Value);
		VectorBYTE vector_BYTE_Value;					SERIALIZE_BASIC(vector_BYTE_Value);
		GUID GUID_Value;								SERIALIZE_BASIC(GUID_Value);
		string string_Value;							SERIALIZE_BASIC(string_Value);
		CStringA CStringA_Value;						SERIALIZE_BASIC(CStringA_Value);
		SYSTEMTIME SYSTEMTIME_Value;					SERIALIZE_BASIC(SYSTEMTIME_Value);
		DOUBLE DOUBLE_Value;							SERIALIZE_BASIC(DOUBLE_Value);
		FLOAT FLOAT_Value;								SERIALIZE_BASIC(FLOAT_Value);
		BYTE BYTE_Value;								SERIALIZE_BASIC(BYTE_Value);

		list<bool> vector_bool_Value;					SERIALIZE_VECTOR(vector_bool_Value);
		list<bool> vector_bool_Value1;					SERIALIZE_VECTOR(vector_bool_Value1);
		vector<CString> vector_CString_Value;			SERIALIZE_VECTOR(vector_CString_Value);
		vector<wstring> vector_wstring_Value;			SERIALIZE_VECTOR(vector_wstring_Value);
		vector<LONGLONG> vector_LONGLONG_Value;			SERIALIZE_VECTOR(vector_LONGLONG_Value);
		vector<ULONGLONG> vector_ULONGLONG_Value;		SERIALIZE_VECTOR(vector_ULONGLONG_Value);
		vector<LONG> vector_LONG_Value;					SERIALIZE_VECTOR(vector_LONG_Value);
		vector<ULONG> vector_ULONG_Value;				SERIALIZE_VECTOR(vector_ULONG_Value);
		vector<CHAR> vector_CHAR_Value;					SERIALIZE_VECTOR(vector_CHAR_Value);
		vector<UCHAR> vector_UCHAR_Value;				SERIALIZE_VECTOR(vector_UCHAR_Value);
		vector<SHORT> vector_SHORT_Value;				SERIALIZE_VECTOR(vector_SHORT_Value);
		vector<USHORT> vector_USHORT_Value;				SERIALIZE_VECTOR(vector_USHORT_Value);
		vector<INT> vector_INT_Value;					SERIALIZE_VECTOR(vector_INT_Value);
		vector<UINT> vector_UINT_Value;					SERIALIZE_VECTOR(vector_UINT_Value);
		vector<VectorBYTE > vector_vector_BYTE_Value;	SERIALIZE_VECTOR(vector_vector_BYTE_Value);
		vector<GUID> vector_GUID_Value;					SERIALIZE_VECTOR(vector_GUID_Value);
		vector<string> vector_string_Value;				SERIALIZE_VECTOR(vector_string_Value);
		vector<CStringA> vector_CStringA_Value;			SERIALIZE_VECTOR(vector_CStringA_Value);
		vector<SYSTEMTIME> vector_SYSTEMTIME_Value;		SERIALIZE_VECTOR(vector_SYSTEMTIME_Value);
		vector<DOUBLE> vector_DOUBLE_Value;				SERIALIZE_VECTOR(vector_DOUBLE_Value);
		vector<FLOAT> vector_FLOAT_Value;				SERIALIZE_VECTOR(vector_FLOAT_Value);
		vector<BYTE> vector_basic_BYTE_Value;			SERIALIZE_VECTOR(vector_basic_BYTE_Value);

		FILETIME FILETIME_Value;						SERIALIZE_BASIC(FILETIME_Value);
		vector<FILETIME> vector_FILETIME_Value;			SERIALIZE_VECTOR(vector_FILETIME_Value);

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
			vector_BYTE_Value/*.Buffer*//*public vector<byte>*/.resize(3); for (auto & item : vector_BYTE_Value/*.Buffer*//*public vector<byte>*/) { item = 88; }
			GUID_Value = { 88 };
			string_Value = "88";
			CStringA_Value = "88";
			SYSTEMTIME_Value = { 88 };
			DOUBLE_Value = 88;
			FLOAT_Value = 88;
			BYTE_Value = 88;

			vector_bool_Value.resize(3); for (auto & item : vector_bool_Value) { item = false; }
			vector_bool_Value1.resize(3); for (auto & item : vector_bool_Value1) { item = true; }
			vector_CString_Value.resize(3); for (auto & item : vector_CString_Value) { item = L"888"; }
			vector_wstring_Value.resize(3); for (auto & item : vector_wstring_Value) { item = 88; }
			vector_LONGLONG_Value.resize(3); for (auto & item : vector_LONGLONG_Value) { item = 88; }
			vector_ULONGLONG_Value.resize(3); for (auto & item : vector_ULONGLONG_Value) { item = 88; }
			vector_LONG_Value.resize(3); for (auto & item : vector_LONG_Value) { item = 88; }
			vector_ULONG_Value.resize(3); for (auto & item : vector_ULONG_Value) { item = 88; }
			vector_CHAR_Value.resize(3); for (auto & item : vector_CHAR_Value) { item = 88; }
			vector_UCHAR_Value.resize(3); for (auto & item : vector_UCHAR_Value) { item = 88; }
			vector_SHORT_Value.resize(3); for (auto & item : vector_SHORT_Value) { item = 88; }
			vector_USHORT_Value.resize(3); for (auto & item : vector_USHORT_Value) { item = 88; }
			vector_INT_Value.resize(3); for (auto & item : vector_INT_Value) { item = 88; }
			vector_UINT_Value.resize(3); for (auto & item : vector_UINT_Value) { item = 88; }
			vector_vector_BYTE_Value.resize(3); for (auto & item : vector_vector_BYTE_Value) { item/*.Buffer*//*public vector<byte>*/.resize(3); for (auto & item1 : item/*.Buffer*//*public vector<byte>*/) { item1 = 88; } }
			vector_GUID_Value.resize(3); for (auto & item : vector_GUID_Value) { item = { 888 }; }
			vector_string_Value.resize(3); for (auto & item : vector_string_Value) { item = 88; }
			vector_CStringA_Value.resize(3); for (auto & item : vector_CStringA_Value) { item = "888"; }
			vector_SYSTEMTIME_Value.resize(3); for (auto & item : vector_SYSTEMTIME_Value) { item = { 888 }; }

			vector_DOUBLE_Value.resize(3); for (auto & item : vector_DOUBLE_Value) { item = 88; }
			vector_FLOAT_Value.resize(3); for (auto & item : vector_FLOAT_Value) { item = 88; }
			vector_basic_BYTE_Value.resize(3); for (auto & item : vector_basic_BYTE_Value) { item = 88; }

			SYSTEMTIME st = { 0 };
			st.wYear = 8888;
			st.wMonth = 8;
			st.wDay = 8;
			st.wHour = 8;
			st.wMinute = 8;
			st.wSecond = 8;
			st.wMilliseconds = 8;
			FILETIME ft = { 0 };
			SystemTimeToFileTime(&st, &ft);

			FILETIME_Value.dwHighDateTime = ft.dwHighDateTime;
			FILETIME_Value.dwLowDateTime = ft.dwLowDateTime;
			vector_FILETIME_Value.resize(3); 
			for (auto & item : vector_FILETIME_Value)
			{
				item.dwHighDateTime = ft.dwHighDateTime;
				item.dwLowDateTime = ft.dwLowDateTime;
			}
		}
	};

	struct tcomplex
	{
		SERIALIZE_BEGIN_STRUCT(tcomplex, tcomplex);
		tsimple tsimple_Value;							SERIALIZE_BASIC(tsimple_Value);
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

		template<typename T>
		static void Init4D(T & container)
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
						i2.resize(2);
						for (auto & i3 : i2)
						{
							i3.Init();
						}

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
			Init4D(q4_tcomplex);
		}

		static void UT()
		{
			//////////////////////////////////////////////////////////////////////////
			tcomplex2 var1;
			var1.Init();
			var1.ToFile(FIELD_NAME(tcomplex2), TEXT("sample-var1.xml"));

			//////////////////////////////////////////////////////////////////////////
			tcomplex2 var2;
			var2.FromFile(FIELD_NAME(tcomplex2), TEXT("sample-var1.xml"));
			var2.ToFile(FIELD_NAME(tcomplex2), TEXT("sample-var2.xml"));

			//bool eq = (var1 == var2);
			//ATLASSERT(eq);
		}
	};

}//end namespace