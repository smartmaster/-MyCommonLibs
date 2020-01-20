#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include <iterator>
#include <vector>
#include <list>
#include <deque>

#include "StringValueConvert.h"
#include "SimpleXml.h"

//////////////////////////////////////////////////////////////////////////
template<typename T>
VOID ValueToElement(LPCTSTR pName, CONST T & Val, XmlElement & Element)
{
	//Element.m_Name = pName;
	//ValueToString(Val, Element.m_Value);
	Val.ToElement(pName, Element);
}

template<typename T>
VOID ElementToValue(LPCTSTR pName, T & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	//StringToValue(Val, Element.m_Value);
	Val.FromElement(pName, Element);
}

//////////////////////////////////////////////////////////////////////////
#define BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(typeParam) \
template<> \
VOID ValueToElement<typeParam>(LPCTSTR pName, CONST typeParam & Val, XmlElement & Element); \
template<> \
VOID ElementToValue<typeParam>(LPCTSTR pName, typeParam & Val, CONST XmlElement & Element);

BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(bool)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(CString)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(wstring)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(LONGLONG)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(ULONGLONG)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(LONG)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(ULONG)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(CHAR)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(UCHAR)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(SHORT)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(USHORT)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(INT)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(UINT)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(VectorBYTE)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(GUID)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(string)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(CStringA)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(SYSTEMTIME)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(DOUBLE)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(FLOAT)
BASIC_VALUE_ELEMENT_CONVERTER_DECLARE(FILETIME)



//////////////////////////////////////////////////////////////////////////
#define CONTAINER_VALUE_ELEMENT_CONVERTER(typeParam) \
template<typename T> \
VOID ValueToElement(LPCTSTR pName, CONST typeParam<T> & Val, XmlElement & Element) \
{ \
	ContainerToElement(pName, Val, Element); \
} \
template<typename T> \
VOID ElementToValue(LPCTSTR pName, typeParam<T> & Val, CONST XmlElement & Element) \
{ \
	ElementToContainer(pName, Val, Element); \
}

CONTAINER_VALUE_ELEMENT_CONVERTER(std::vector)
CONTAINER_VALUE_ELEMENT_CONVERTER(std::list)
CONTAINER_VALUE_ELEMENT_CONVERTER(std::deque)


//////////////////////////////////////////////////////////////////////////
template<typename Container>
VOID ContainerToElement(LPCTSTR pName, CONST Container & con, XmlElement & Element)
{
	Element.m_Name = pName;
	Element.m_Children.resize(con.size());

	CString ItemName;
	static CONST TCHAR ITEM_FORMAT[] = TEXT("Item%d");
	int ii = 0;
	for (auto iter = std::begin(con); iter != std::end(con); ++ iter)
	{
		ItemName.Format(ITEM_FORMAT, ii);
		CONST auto & Val = *iter;
		XmlElement & Child = Element.m_Children[ii];
		ValueToElement(ItemName.GetString(), Val, Child);
		++ ii;
	}
}

template<typename Container>
VOID ElementToContainer(LPCTSTR pName, Container & con, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	con.resize(Element.m_Children.size());

	CString ItemName;
	static CONST TCHAR ITEM_FORMAT[] = TEXT("Item%d");
	int ii = 0;
	for (auto iter = std::begin(con); iter != std::end(con); ++ iter)
	{
		ItemName.Format(ITEM_FORMAT, ii);
		auto & Val = *iter;
		CONST XmlElement & Child = Element.m_Children[ii];
		ElementToValue(ItemName.GetString(), Val, Child);
		++ ii;
	}
}

#if 1
template<typename Container>
VOID ContainerStructToElement(LPCTSTR pName, CONST Container & con, XmlElement & Element)
{
	Element.m_Name = pName;
	Element.m_Children.resize(con.size());

	CString ItemName;
	static CONST TCHAR ITEM_FORMAT[] = TEXT("Item%d");
	int ii = 0;
	for (auto iter = std::begin(con); iter != std::end(con); ++iter)
	{
		ItemName.Format(ITEM_FORMAT, ii);
		CONST auto & Val = *iter;
		XmlElement & Child = Element.m_Children[ii];
		Val.ToElement(ItemName.GetString(), Child);
		++ ii;
	}
}


template<typename Container>
VOID ElementToContainerStruct(LPCTSTR pName, Container & con, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	con.resize(Element.m_Children.size());

	CString ItemName;
	static CONST TCHAR ITEM_FORMAT[] = TEXT("Item%d");
	int ii = 0;
	for (auto iter = std::begin(con); iter != std::end(con); ++iter)
	{
		ItemName.Format(ITEM_FORMAT, ii);
		auto & Val = *iter;
		CONST XmlElement & Child = Element.m_Children[ii];
		Val.FromElement(ItemName.GetString(), Child);
		++ ii;
	}
}
//////////////////////////////////////////////////////////////////////////


template<typename T>
VOID VectorToElement(LPCTSTR pName, CONST vector<T> & vec, XmlElement & Element)
{
#if 1
	ContainerToElement(pName, vec, Element);
#else
	Element.m_Name = pName;
	Element.m_Children.resize(vec.size());

	CString ItemName;
	static CONST TCHAR ITEM_FORMAT[] = TEXT("Item%d");
	for (size_t ii = 0; ii < vec.size(); ++ii)
	{
		ItemName.Format(ITEM_FORMAT, ii);
		CONST T & Val = vec[ii];
		XmlElement & Child = Element.m_Children[ii];
		ValueToElement(ItemName.GetString(), Val, Child);
	}
#endif
	
}

template<typename T>
VOID Vector2DToElement(LPCTSTR pName, CONST vector<vector<T> > & vec2D, XmlElement & Element)
{
	Element.m_Name = pName;
	Element.m_Children.resize(vec2D.size());

	CString ItemName;
	static CONST TCHAR ITEM_FORMAT[] = TEXT("Item%d");
	for (size_t ii = 0; ii < vec2D.size(); ++ii)
	{
		ItemName.Format(ITEM_FORMAT, ii);
		CONST vector<T> & Val = vec2D[ii];
		XmlElement & Child = Element.m_Children[ii];
		VectorToElement(ItemName.GetString(), Val, Child);
	}
}

template<typename T>
VOID ElementToVector(LPCTSTR pName, vector<T> & vec, CONST XmlElement & Element)
{
#if 1
	ElementToContainer(pName, vec, Element);
#else
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	vec.resize(Element.m_Children.size());

	CString ItemName;
	static CONST TCHAR ITEM_FORMAT[] = TEXT("Item%d");
	for (size_t ii = 0; ii < vec.size(); ++ii)
	{
		ItemName.Format(ITEM_FORMAT, ii);
		T & Val = vec[ii];
		CONST XmlElement & Child = Element.m_Children[ii];
		ElementToValue(ItemName.GetString(), Val, Child);
	}
#endif
}


template<typename T>
VOID ElementToVector2D(LPCTSTR pName, vector<vector<T> > & vec2D, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	vec2D.resize(Element.m_Children.size());

	CString ItemName;
	static CONST TCHAR ITEM_FORMAT[] = TEXT("Item%d");
	for (size_t ii = 0; ii < vec2D.size(); ++ii)
	{
		ItemName.Format(ITEM_FORMAT, ii);
		vector<T> & Val = vec2D[ii];
		CONST XmlElement & Child = Element.m_Children[ii];
		ElementToVector(ItemName.GetString(), Val, Child);
	}
}


template<typename T>
VOID VectorStructToElement(LPCTSTR pName, CONST vector<T> & vec, XmlElement & Element)
{
#if 1
	ContainerStructToElement(pName, vec, Element);
#else
	Element.m_Name = pName;
	Element.m_Children.resize(vec.size());

	CString ItemName;
	static CONST TCHAR ITEM_FORMAT[] = TEXT("Item%d");
	for (size_t ii = 0; ii < vec.size(); ++ii)
	{
		ItemName.Format(ITEM_FORMAT, ii);
		CONST T & Val = vec[ii];
		XmlElement & Child = Element.m_Children[ii];
		Val.ToElement(ItemName.GetString(), Child);
	}
#endif
}

template<typename T>
VOID Vector2DStructToElement(LPCTSTR pName, CONST vector<vector<T> > & vec2D, XmlElement & Element)
{
	Element.m_Name = pName;
	Element.m_Children.resize(vec2D.size());

	CString ItemName;
	static CONST TCHAR ITEM_FORMAT[] = TEXT("Item%d");
	for (size_t ii = 0; ii < vec2D.size(); ++ii)
	{
		ItemName.Format(ITEM_FORMAT, ii);
		CONST vector<T> & Val = vec2D[ii];
		XmlElement & Child = Element.m_Children[ii];
		VectorStructToElement(ItemName.GetString(), Val, Child);
	}
}

template<typename T>
VOID ElementToVectorStruct(LPCTSTR pName, vector<T> & vec, CONST XmlElement & Element)
{
#if 1
	ElementToContainerStruct(pName, vec, Element);
#else
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	vec.resize(Element.m_Children.size());

	CString ItemName;
	static CONST TCHAR ITEM_FORMAT[] = TEXT("Item%d");
	for (size_t ii = 0; ii < vec.size(); ++ii)
	{
		ItemName.Format(ITEM_FORMAT, ii);
		T & Val = vec[ii];
		CONST XmlElement & Child = Element.m_Children[ii];
		Val.FromElement(ItemName.GetString(), Child);
	}
#endif
}


template<typename T>
VOID ElementToVector2DStruct(LPCTSTR pName, vector<vector<T> > & vec2D, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	vec2D.resize(Element.m_Children.size());

	CString ItemName;
	static CONST TCHAR ITEM_FORMAT[] = TEXT("Item%d");
	for (size_t ii = 0; ii < vec2D.size(); ++ii)
	{
		ItemName.Format(ITEM_FORMAT, ii);
		vector<T> & Val = vec2D[ii];
		CONST XmlElement & Child = Element.m_Children[ii];
		ElementToVectorStruct(ItemName.GetString(), Val, Child);
	}
}

//////////////////////////////////////////////////////////////////////////
template<typename T>
VOID Vector3DToElement(LPCTSTR pName, CONST vector<vector<vector<T> > > & vec3D, XmlElement & Element)
{
	Element.m_Name = pName;
	Element.m_Children.resize(vec3D.size());

	CString ItemName;
	static CONST TCHAR ITEM_FORMAT[] = TEXT("Item%d");
	for (size_t ii = 0; ii < vec3D.size(); ++ii)
	{
		ItemName.Format(ITEM_FORMAT, ii);
		CONST vector<vector<T> > & Val = vec3D[ii];
		XmlElement & Child = Element.m_Children[ii];
		Vector2DToElement(ItemName.GetString(), Val, Child);
	}
}

template<typename T>
VOID ElementToVector3D(LPCTSTR pName, vector<vector<vector<T> > > & vec3D, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	vec3D.resize(Element.m_Children.size());

	CString ItemName;
	static CONST TCHAR ITEM_FORMAT[] = TEXT("Item%d");
	for (size_t ii = 0; ii < vec3D.size(); ++ii)
	{
		ItemName.Format(ITEM_FORMAT, ii);
		vector<vector<T> > & Val = vec3D[ii];
		CONST XmlElement & Child = Element.m_Children[ii];
		ElementToVector2D(ItemName.GetString(), Val, Child);
	}
}

//////////////////////////////////////////////////////////////////////////
template<typename T>
VOID Vector3DStructToElement(LPCTSTR pName, CONST vector<vector<vector<T> > > & vec3D, XmlElement & Element)
{
	Element.m_Name = pName;
	Element.m_Children.resize(vec3D.size());

	CString ItemName;
	static CONST TCHAR ITEM_FORMAT[] = TEXT("Item%d");
	for (size_t ii = 0; ii < vec3D.size(); ++ii)
	{
		ItemName.Format(ITEM_FORMAT, ii);
		CONST vector<vector<T> > & Val = vec3D[ii];
		XmlElement & Child = Element.m_Children[ii];
		Vector2DStructToElement(ItemName.GetString(), Val, Child);
	}
}

template<typename T>
VOID ElementToVector3DStruct(LPCTSTR pName, vector<vector<vector<T> > > & vec3D, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	vec3D.resize(Element.m_Children.size());

	CString ItemName;
	static CONST TCHAR ITEM_FORMAT[] = TEXT("Item%d");
	for (size_t ii = 0; ii < vec3D.size(); ++ii)
	{
		ItemName.Format(ITEM_FORMAT, ii);
		vector<vector<T> > & Val = vec3D[ii];
		CONST XmlElement & Child = Element.m_Children[ii];
		ElementToVector2DStruct(ItemName.GetString(), Val, Child);
	}
}
#endif

#if 0
//////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToElement(LPCTSTR pName, CONST XYZXYZXYZ & Val, XmlElement & Element)
//{
//	Element.m_Name = pName;
//	ValueToString(Val, Element.m_Value);
//}
//
//template<>
//VOID ElementToValue(LPCTSTR pName, XYZXYZXYZ & Val, CONST XmlElement & Element)
//{
//	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
//	StringToValue(Val, Element.m_Value);
//}

////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<CString>(CONST CString & Value, CString & str);
//template<>
//VOID StringToValue<CString>(CString & Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST CString & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, CString & Val, CONST XmlElement & Element);
//
////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<wstring>(CONST wstring & Value, CString & str);
//template<>
//VOID StringToValue<wstring>(wstring & Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST wstring & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, wstring & Val, CONST XmlElement & Element);
//
////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<LONGLONG>(CONST LONGLONG & Value, CString & str);
//template<>
//VOID StringToValue<LONGLONG>(LONGLONG & Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST LONGLONG & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, LONGLONG & Val, CONST XmlElement & Element);
//
////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<ULONGLONG>(CONST ULONGLONG & Value, CString & str);
//template<>
//VOID StringToValue<ULONGLONG>(ULONGLONG & Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST ULONGLONG & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, ULONGLONG & Val, CONST XmlElement & Element);
//
////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<LONG>(CONST LONG & Value, CString & str);
//template<>
//VOID StringToValue<LONG>(LONG & Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST LONG & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, LONG & Val, CONST XmlElement & Element);
//
////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<ULONG>(CONST ULONG & Value, CString & str);
//template<>
//VOID StringToValue<ULONG>(ULONG & Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST ULONG & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, ULONG & Val, CONST XmlElement & Element);
//
////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<CHAR>(CONST CHAR & Value, CString & str);
//template<>
//VOID StringToValue<CHAR>(CHAR &Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST CHAR & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, CHAR & Val, CONST XmlElement & Element);
//
////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<UCHAR>(CONST UCHAR & Value, CString & str);
//template<>
//VOID StringToValue<UCHAR>(UCHAR & Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST UCHAR & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, UCHAR & Val, CONST XmlElement & Element);
//
////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<SHORT>(CONST SHORT & Value, CString & str);
//template<>
//VOID StringToValue<SHORT>(SHORT & Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST SHORT & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, SHORT & Val, CONST XmlElement & Element);
//
////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<USHORT>(CONST USHORT & Value, CString & str);
//template<>
//VOID StringToValue<USHORT>(USHORT & Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST USHORT & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, USHORT & Val, CONST XmlElement & Element);
//
////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<INT>(CONST INT & Value, CString & str);
//template<>
//VOID StringToValue<INT>(INT & Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST INT & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, INT & Val, CONST XmlElement & Element);
//
////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<UINT>(CONST UINT & Value, CString & str);
//template<>
//VOID StringToValue<UINT>(UINT & Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST UINT & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, UINT & Val, CONST XmlElement & Element);
//
////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<vector<BYTE> >(CONST vector<BYTE> & Value, CString & str);
//template<>
//VOID StringToValue<vector<BYTE> >(vector<BYTE> & Value, CONST CString & str);
//template<>
//VOID ValueToElement(LPCTSTR pName, CONST vector<BYTE> & Val, XmlElement & Element);
//
//template<>
//VOID ElementToValue(LPCTSTR pName, vector<BYTE> & Val, CONST XmlElement & Element);

//template<>
//VOID ValueToString<VectorBYTE>(CONST VectorBYTE & Value, CString & str);
//template<>
//VOID StringToValue<VectorBYTE>(VectorBYTE & Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST VectorBYTE & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, VectorBYTE & Val, CONST XmlElement & Element);
//
////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<GUID>(CONST GUID & Value, CString & str);
//template<>
//VOID StringToValue<GUID>(GUID & Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST GUID & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, GUID & Val, CONST XmlElement & Element);
//
////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<string>(CONST string & Value, CString & str);
//template<>
//VOID StringToValue<string>(string & Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST string & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, string & Val, CONST XmlElement & Element);
//
////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<CStringA>(CONST CStringA & Value, CString & str);
//template<>
//VOID StringToValue<CStringA>(CStringA & Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST CStringA & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, CStringA & Val, CONST XmlElement & Element);
//
////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<SYSTEMTIME>(CONST SYSTEMTIME & Value, CString & str);
//template<>
//VOID StringToValue<SYSTEMTIME>(SYSTEMTIME & Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST SYSTEMTIME & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, SYSTEMTIME & Val, CONST XmlElement & Element);


////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<DOUBLE>(CONST DOUBLE & Value, CString & str);
//
//template<>
//VOID StringToValue<DOUBLE>(DOUBLE & Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST DOUBLE & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, DOUBLE & Val, CONST XmlElement & Element);
//
////////////////////////////////////////////////////////////////////////////
//template<>
//VOID ValueToString<FLOAT>(CONST FLOAT & Value, CString & str);
//
//template<>
//VOID StringToValue<FLOAT>(FLOAT & Value, CONST CString & str);
template<>
VOID ValueToElement(LPCTSTR pName, CONST FLOAT & Val, XmlElement & Element);

template<>
VOID ElementToValue(LPCTSTR pName, FLOAT & Val, CONST XmlElement & Element);
#endif


#if 0
template<typename T>
VOID ValueToElement(LPCTSTR pName, CONST std::vector<T> & Val, XmlElement & Element)
{
	ContainerToElement(pName, Val, Element);
}

template<typename T>
VOID ElementToValue(LPCTSTR pName, std::vector<T> & Val, CONST XmlElement & Element)
{
	ElementToContainer(pName, Val, Element);
}

//////////////////////////////////////////////////////////////////////////
template<typename T>
VOID ValueToElement(LPCTSTR pName, CONST std::list<T> & Val, XmlElement & Element)
{
	ContainerToElement(pName, Val, Element);
}

template<typename T>
VOID ElementToValue(LPCTSTR pName, std::list<T> & Val, CONST XmlElement & Element)
{
	ElementToContainer(pName, Val, Element);
}

//////////////////////////////////////////////////////////////////////////
template<typename T>
VOID ValueToElement(LPCTSTR pName, CONST std::deque<T> & Val, XmlElement & Element)
{
	ContainerToElement(pName, Val, Element);
}

template<typename T>
VOID ElementToValue(LPCTSTR pName, std::deque<T> & Val, CONST XmlElement & Element)
{
	ElementToContainer(pName, Val, Element);
}
#endif