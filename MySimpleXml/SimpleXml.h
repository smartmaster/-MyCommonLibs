#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>
using namespace std;

#include "..\PublicHeader\PublicFileDeviceInterface.h"

#include "SimpleXmlHelper.h"

struct XmlAttribute
{
	CString m_AttrName;
	CString m_AttrValue;
};

struct XmlElement
{
	CString m_Name;
	vector<XmlAttribute> m_Attrs;
	CString m_Value;
	vector<XmlElement> m_Children;

private:
	#define SML_XmlElement_DEFAULT_SPACE_CHAR TEXT(" ")
	VOID ToStringWithFormat(CString & str, int & spaceCount, LPCTSTR spaceChar) CONST;

public:
	//////////////////////////////////////////////////////////////////////////
	VOID ToString(CString & str) CONST;
	VOID ToStringWithFormat(CString & str, LPCTSTR spaceChar = SML_XmlElement_DEFAULT_SPACE_CHAR) CONST;
	HRESULT ToStream(IFileDevice * pIFileDevice) CONST;
	HRESULT ToStreamWithFormat(IFileDevice * pIFileDevice, LPCTSTR spaceChar = SML_XmlElement_DEFAULT_SPACE_CHAR) CONST;
	HRESULT ToFile(LPCTSTR pFileName) CONST;
	HRESULT ToFileWithFormat(LPCTSTR pFileName, LPCTSTR spaceChar = SML_XmlElement_DEFAULT_SPACE_CHAR) CONST;

	LONG FromString(CONST CString & str, INT & Start); //return element count: <= 0 on error, >0 success
	HRESULT FromStream(IFileDevice * pIFileDevice);
	HRESULT FromFile(LPCTSTR pFileName);

	//////////////////////////////////////////////////////////////////////////
	static  VOID FindAllChildren(CONST XmlElement & ele, LPCTSTR ChildName, vector<CONST XmlElement *> & vecChildren);
	static  BOOL FindByPath( //return TRUE - found; FALSE - not found
		IN CONST XmlElement & ele, //start root element
		IN CONST LPCTSTR * Paths,  //sub path
		IN INT PathsCount,		//sub path count
		OUT CONST XmlElement * & containingEle, //containing element
		OUT vector<CONST XmlElement *> & vecChildren //resulted element
		);

	static  BOOL FindAllByPath( //return TRUE - found; FALSE - not found
		IN CONST XmlElement & ele, //start root element
		IN CONST LPCTSTR * Paths,  //sub path
		IN INT PathsCount,		//sub path count
		OUT vector<CONST XmlElement *> & containingEle, //containing element
		OUT vector<vector<CONST XmlElement *> > & vecChildren //resulted element
		);
};
