#include "stdafx.h"
#include "ValueXmlElementConvert.h"

#define BASIC_VALUE_ELEMENT_CONVERTER_IMPL(typeParam) \
template<> \
VOID ValueToElement<typeParam>(LPCTSTR pName, CONST typeParam & Val, XmlElement & Element) \
{ \
	Element.m_Name = pName; \
	ValueToString(Val, Element.m_Value); \
} \
template<> \
VOID ElementToValue<typeParam>(LPCTSTR pName, typeParam & Val, CONST XmlElement & Element) \
{ \
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName)); \
	StringToValue(Val, Element.m_Value); \
}

BASIC_VALUE_ELEMENT_CONVERTER_IMPL(bool)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(CString)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(wstring)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(LONGLONG)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(ULONGLONG)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(LONG)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(ULONG)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(CHAR)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(UCHAR)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(SHORT)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(USHORT)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(INT)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(UINT)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(VectorBYTE)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(GUID)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(string)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(CStringA)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(SYSTEMTIME)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(DOUBLE)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(FLOAT)
BASIC_VALUE_ELEMENT_CONVERTER_IMPL(FILETIME)

#if 0
template<>
VOID ValueToElement(LPCTSTR pName, CONST CString & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

template<>
VOID ValueToElement(LPCTSTR pName, CONST wstring & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

template<>
VOID ValueToElement(LPCTSTR pName, CONST LONGLONG & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

template<>
VOID ValueToElement(LPCTSTR pName, CONST ULONGLONG & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

template<>
VOID ValueToElement(LPCTSTR pName, CONST LONG & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

template<>
VOID ValueToElement(LPCTSTR pName, CONST ULONG & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

template<>
VOID ValueToElement(LPCTSTR pName, CONST CHAR & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

template<>
VOID ValueToElement(LPCTSTR pName, CONST UCHAR & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

template<>
VOID ValueToElement(LPCTSTR pName, CONST SHORT & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

template<>
VOID ValueToElement(LPCTSTR pName, CONST USHORT & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

template<>
VOID ValueToElement(LPCTSTR pName, CONST INT & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

template<>
VOID ValueToElement(LPCTSTR pName, CONST UINT & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

//template<>
//VOID ValueToElement(LPCTSTR pName, CONST vector<BYTE> & Val, XmlElement & Element)
//{
//	Element.m_Name = pName;
//	ValueToString(Val, Element.m_Value);
//}

template<>
VOID ValueToElement(LPCTSTR pName, CONST GUID & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

template<>
VOID ValueToElement(LPCTSTR pName, CONST string & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

template<>
VOID ValueToElement(LPCTSTR pName, CONST CStringA & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

template<>
VOID ValueToElement(LPCTSTR pName, CONST SYSTEMTIME & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

template<>
VOID ValueToElement(LPCTSTR pName, CONST VectorBYTE & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

template<>
VOID ValueToElement(LPCTSTR pName, CONST FLOAT & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

template<>
VOID ValueToElement(LPCTSTR pName, CONST DOUBLE & Val, XmlElement & Element)
{
	Element.m_Name = pName;
	ValueToString(Val, Element.m_Value);
}

template<>
VOID ElementToValue(LPCTSTR pName, CString & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}

template<>
VOID ElementToValue(LPCTSTR pName, wstring & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}

template<>
VOID ElementToValue(LPCTSTR pName, LONGLONG & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}

template<>
VOID ElementToValue(LPCTSTR pName, ULONGLONG & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}

template<>
VOID ElementToValue(LPCTSTR pName, LONG & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}

template<>
VOID ElementToValue(LPCTSTR pName, ULONG & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}

template<>
VOID ElementToValue(LPCTSTR pName, CHAR & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}

template<>
VOID ElementToValue(LPCTSTR pName, UCHAR & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}

template<>
VOID ElementToValue(LPCTSTR pName, SHORT & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}

template<>
VOID ElementToValue(LPCTSTR pName, USHORT & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}

template<>
VOID ElementToValue(LPCTSTR pName, INT & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}

template<>
VOID ElementToValue(LPCTSTR pName, UINT & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}

//template<>
//VOID ElementToValue(LPCTSTR pName, vector<BYTE> & Val, CONST XmlElement & Element)
//{
//	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
//	StringToValue(Val, Element.m_Value);
//}

template<>
VOID ElementToValue(LPCTSTR pName, GUID & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}

template<>
VOID ElementToValue(LPCTSTR pName, string & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}

template<>
VOID ElementToValue(LPCTSTR pName, CStringA & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}

template<>
VOID ElementToValue(LPCTSTR pName, SYSTEMTIME & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}

template<>
VOID ElementToValue(LPCTSTR pName, VectorBYTE & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}

template<>
VOID ElementToValue(LPCTSTR pName, FLOAT & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}

template<>
VOID ElementToValue(LPCTSTR pName, DOUBLE & Val, CONST XmlElement & Element)
{
	ATLASSERT(0 == Element.m_Name.CompareNoCase(pName));
	StringToValue(Val, Element.m_Value);
}
#endif