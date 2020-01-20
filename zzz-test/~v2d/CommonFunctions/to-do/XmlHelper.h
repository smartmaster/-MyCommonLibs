#pragma once

#include "stdafx.h"
#include <msxml6.h>

#include <vector>
using namespace std;

struct CXmlElement
{
	CString m_strTagName;
	vector<pair<CString, CString> > m_Attrs;
	CString m_strText;

	HRESULT CreateXmlElement(IXMLDOMDocument * pDocument, IXMLDOMElement ** ppElement);
};

struct CXmlHelper
{
	static HRESULT AppendXmlChild(IXMLDOMNode * pNode, IXMLDOMNode * pChildNode, IXMLDOMNode ** ppNewChildNode);
};

//<mc> 2010-10-21 21:53 UT

namespace NS_MySimpleXmlUT
{
	static CONST LPCTSTR szStreamNames[] = 
	{
		TEXT("m_Name"),
		TEXT("m_Weight"),
		TEXT("m_Price"),
	};

	// {A1D70CA0-924A-4ECF-ABA4-F4C7BDAE7ED8}
	static CONST GUID StgGUID = 
	{ 0xa1d70ca0, 0x924a, 0x4ecf, { 0xab, 0xa4, 0xf4, 0xc7, 0xbd, 0xae, 0x7e, 0xd8 } };
};

struct CMySimpleXmlUT
{
	CString m_Name;	//0
	ULONG m_Weight;	//1
	DOUBLE m_Price;	//2

	HRESULT WriteToNode(IXMLDOMNode * pParentNode, IXMLDOMDocument * pDocument);
	
	HRESULT WriteToStorage(IStorage * pParentStorage);
	HRESULT ReadFromStorage(IStorage * pParentStorage);
};

namespace NS_ComplexXmlUT
{
	static CONST LPCTSTR szStreamNames[] = 
	{
		TEXT("m_Name"),
		TEXT("m_Weight"),
		TEXT("m_Price"),
	};
	
	static CONST LPCTSTR szStorageNames[] = 
	{
		TEXT("m_InnerSimpleXml"),
		TEXT("m_InnerSimpleXml2"),
	};

	// {240DCA0D-A22F-4B88-8148-BA03297DD781}
	static const GUID StgGUID = 
	{ 0x240dca0d, 0xa22f, 0x4b88, { 0x81, 0x48, 0xba, 0x3, 0x29, 0x7d, 0xd7, 0x81 } };

};

struct CComplexXmlUT
{
	CString m_Name;	//0
	ULONG m_Weight;	//1
	DOUBLE m_Price;	//2
	CMySimpleXmlUT m_InnerSimpleXml; //0
	CMySimpleXmlUT m_InnerSimpleXml2; //1

	HRESULT WriteToNode(IXMLDOMNode * pParentNode, IXMLDOMDocument * pDocument);

	HRESULT WriteToStorage(IStorage * pParentStorage);
	HRESULT ReadFromStorage(IStorage * pParentStorage);

	static HRESULT XmlHelperUT(LPCTSTR pszXmlFile);
	static HRESULT StorageHelperUT(LPCTSTR pszXmlFile);
};
//</mc>