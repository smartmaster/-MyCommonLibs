#include "stdafx.h"

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicReadWriteTextFile.h"

#include "SimpleXml.h"

namespace { //anonymous namespace start

	static VOID NOP() {}

	static BOOL IsElementName(CONST XmlElement & ele, LPCTSTR name)
	{
		return (0 == ele.m_Name.CompareNoCase(name));
	}

	////////////////////////////////////////////////////////////////////////////
	//template<typename T>
	//VOID AllocateOneMore(vector<T> & vec)
	//{
	//	vec.resize(vec.size() + 1);
	//}

	//template<typename T>
	//T & Last(vector<T> & vec)
	//{
	//	return vec[vec.size() - 1];
	//}


	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	static INT FindValue(CONST CString & str, T Value, INT Start, INT Stop)
	{
		INT Index = str.Find(Value, Start);
		if (-1 == Index || (Index + StringLen(Value) > Stop + 1))
		{
			Index = -1;
		}
		return Index;
	}

	BOOL isValidRange(int ch)
	{
		return (ch >= -1 && ch <= 255);
	}

	static INT SkipSpace(CONST CString & str, INT Start, INT Stop)
	{
		while (
			Start <= Stop && 
			(!isValidRange(str[Start]) || isspace(str[Start]) || !isgraph(str[Start]))
			)
		{
			++ Start;
		}

		if (Start > Stop)
		{
			Start = -1;
		}

		return Start;
	}

	static INT SkipChar(CONST CString & str, INT Start, INT Stop)
	{
		while (
			Start <= Stop && 
			(!isspace(str[Start]) && isgraph(str[Start]))
			)
		{
			++ Start;
		}

		if (Start > Stop)
		{
			Start = -1;
		}

		return Start;
	}

	static VOID TrimSpace(CString & str)
	{
		str.Trim(TEXT(" \t\r\n"));
	}

	//////////////////////////////////////////////////////////////////////////
	static VOID ParseElementNameAndArributes(CONST CString & str, INT Start, CONST INT EndStop, CString & Name, vector<XmlAttribute> & Attrs)
	{
		ATLASSERT(TEXT('<') == str[Start] && TEXT('>') == str[EndStop]);
		Start = SkipSpace(str, Start + 1, EndStop);
		INT End = SkipChar(str, Start, EndStop);
		if (-1 == End)
		{
			End = EndStop;
		}
		Name = str.Mid(Start, End - Start);
		Name.Trim(TEXT(" /"));

		for (;;)
		{
			Start = SkipSpace(str, End, EndStop);
			if (TEXT('>') == str[Start] || 
				((TEXT('/') == str[Start] && TEXT('>') == str[Start + 1]))
				)
			{
				break;
			}

			End = FindValue(str, TEXT('='), Start, EndStop);
			if (-1 == End)
			{
				break;
			}

			AllocateOneMore(Attrs);
			Last(Attrs).m_AttrName = str.Mid(Start, End - Start);
			TrimSpace(Last(Attrs).m_AttrName);

			Start = FindValue(str, TEXT('\"'), End, EndStop);
			End = FindValue(str, TEXT('\"'), Start + 1, EndStop);
			Last(Attrs).m_AttrValue = str.Mid(Start + 1, End -  Start - 1);

			++ End;
		}
	}


	//////////////////////////////////////////////////////////////////////////
	struct XmlSpecialChar
	{
		LPCTSTR m_Normal;
		LPCTSTR m_Escaped;
	};

	static CONST XmlSpecialChar SpecialChar[] = 
	{
		{TEXT("&"), TEXT("&amp;")},
		{TEXT("<"), TEXT("&lt;")},
		{TEXT(">"), TEXT("&gt;")},
		{TEXT("'"), TEXT("&apos;")},
		{TEXT("\""), TEXT("&quot;")}	
	};

	BOOL HasSpecialChar(CONST CString & str)
	{
		BOOL bRet = FALSE;
		for (int ii = 0; ii < _countof(SpecialChar); ++ ii)
		{
			if (-1 != str.Find(SpecialChar[ii].m_Normal))
			{
				bRet = TRUE;
				break;
			}
		}
		return bRet;
	}

	VOID NormalToEscaped(CString & str)
	{
		for (int ii = 0; ii < _countof(SpecialChar); ii++)
		{
			str.Replace(SpecialChar[ii].m_Normal, SpecialChar[ii].m_Escaped);
		}
	}

	template<typename T>
	int StringLen(T x)
	{
		return -1;
	}

	template<>
	int StringLen(TCHAR ch)
	{
		return 1;
	}

	template<>
	int StringLen(LPCTSTR psz)
	{
		return _tcslen(psz);
	}

	template<typename T>
	VOID GetNextClosedToken(CONST CString & str, INT Start, T Left, T Right, INT & StartOut, INT & EndOut)
	{
		size_t LeftLen = StringLen(Left);
		size_t RightLen = StringLen(Right);

		StartOut = str.Find(Left, Start);
		if (-1 == StartOut)
		{
			return;
		}

		EndOut = str.Find(Right, StartOut + LeftLen);
		if (-1 == EndOut)
		{
			return;
		}

		EndOut += (RightLen - 1);
	}


	static VOID RemoveComments(CString & str)
	{
		INT Start = 0;
		INT End = 0;
		for (;;)
		{
			GetNextClosedToken(str, 0, TEXT("<!--"), TEXT("-->"), Start, End);
			if (-1 == Start || -1 == End)
			{
				break;
			}

			str.Delete(Start, End - Start + 1);
		}
	}


	static VOID EscapedToNormal(CString & str)
	{
		for (int ii = 0; ii < _countof(SpecialChar); ii++)
		{
			str.Replace(SpecialChar[ii].m_Escaped, SpecialChar[ii].m_Normal);
		}
	}

	static BOOL IsPI(CONST CString & str, INT Start, INT End)
	{
		static TCHAR XML_PI[] = TEXT("<?") TEXT("?>");

		if (End - Start + 1 < _countof(XML_PI) - 1)
		{
			return FALSE;
		}

		return (
			XML_PI[0] == str[Start] &&
			XML_PI[1] == str[Start + 1] &&
			XML_PI[2] == str[End - 1] &&
			XML_PI[3] == str[End]
		);
	}

	static BOOL IsComment(CONST CString & str, INT Start, INT End)
	{
		static TCHAR XML_COMMENT[] = TEXT("<!--") TEXT("-->");

		if (End - Start + 1 < _countof(XML_COMMENT) - 1)
		{
			return FALSE;
		}

		return (
			XML_COMMENT[0] == str[Start] &&
			XML_COMMENT[1] == str[Start + 1] &&
			XML_COMMENT[2] == str[Start + 2] &&
			XML_COMMENT[3] == str[Start + 3] &&
			XML_COMMENT[4] == str[End - 2] &&
			XML_COMMENT[5] == str[End - 1] &&
			XML_COMMENT[6] == str[End]
		);
	}

	static BOOL IsXmlOtherData(CONST CString & Str, INT Start, INT End)
	{
		static CONST TCHAR OTHER_TAG[] = TEXT("<!") TEXT(">");

		if (End - Start + 1 < _countof(OTHER_TAG))
		{
			return FALSE;
		}

		return (
			OTHER_TAG[0] == Str[Start] &&
			OTHER_TAG[1] == Str[Start + 1] &&
			OTHER_TAG[2] == Str[End]
		);
	}


	static BOOL IsElementTag(CONST CString & str, INT Start, INT End)
	{
		return !IsPI(str, Start, End) && 
			!IsComment(str, Start, End) &&
			!IsXmlOtherData(str, Start, End);

	}

	static VOID GetNextElementTag(CONST CString & str, INT StartIn, INT & StartOut, INT & EndOut)
	{
		StartOut = StartIn;
		EndOut = -1;

		for (;;)
		{
			GetNextClosedToken(str, StartOut, TEXT('<'), TEXT('>'), StartOut, EndOut);
			if (-1 == StartOut || -1 == EndOut)
			{
				return;
			}

			if (IsElementTag(str, StartOut, EndOut))
			{
				break;
			}

			StartOut = EndOut + 1;
		}
	}

	static BOOL IsSingleClosedElenemt(CONST CString & Str, INT Start, INT End)
	{
		static CONST TCHAR SINGLE_TAG[] = TEXT("<") TEXT("/>");

		if (End - Start + 1 < _countof(SINGLE_TAG))
		{
			return FALSE;
		}

		return (
			SINGLE_TAG[0] == Str[Start] &&
			SINGLE_TAG[1] == Str[End - 1] &&
			SINGLE_TAG[2] == Str[End]
		);
	}

	static BOOL IsEndTag(CONST CString & Str, INT Start, INT End)
	{
		static CONST TCHAR END_TAG[] = TEXT("</") TEXT(">");

		if (End - Start + 1 < _countof(END_TAG))
		{
			return FALSE;
		}

		return (
			END_TAG[0] == Str[Start] && 
			END_TAG[1] == Str[Start + 1] && 
			END_TAG[2] == Str[End]
		);
	}

} //anonymous namespace end



VOID XmlElement::ToString( CString & str ) CONST
{
	str += TEXT('<');
	str += m_Name;
	for (size_t ii = 0; ii < m_Attrs.size(); ++ ii)
	{
		CONST XmlAttribute & Attr = m_Attrs[ii];
		str += TEXT(' ');
		str += Attr.m_AttrName;
		str += TEXT('=');
		str += TEXT('\"');
		str += Attr.m_AttrValue;
		str += TEXT('\"');
	}

	if (m_Value.IsEmpty() && m_Children.empty())
	{
		str += TEXT(" />");
		return;
	}


	str += TEXT('>');
	if (m_Value.GetLength())
	{
		if (HasSpecialChar(m_Value))
		{
			//myCode 2013/06/11
			CString strValue = m_Value;
			NormalToEscaped(strValue);
			str += strValue;
		}
		else
		{
			str += m_Value;
		}
	}



	for (size_t ii = 0; ii < m_Children.size(); ++ ii)
	{
		m_Children[ii].ToString(str); //recursively process children
	}


	if (m_Name.GetLength())
	{
		str += TEXT("</");
		str += m_Name;
		str += TEXT('>');
	}
}

VOID XmlElement::ToStringWithFormat(CString & str, LPCTSTR spaceChar) CONST
{
	int spaceCount = 0;
	ToStringWithFormat(str, spaceCount, spaceChar);
}

VOID XmlElement::ToStringWithFormat(CString & str, int & spaceCount, LPCTSTR spaceChar) CONST
{
	for (int ii = 0; ii < spaceCount; ++ii)
	{
		str += spaceChar;
	}
	str += TEXT('<');
	str += m_Name;
	for (size_t ii = 0; ii < m_Attrs.size(); ++ii)
	{
		CONST XmlAttribute & Attr = m_Attrs[ii];
		str += TEXT(' ');
		str += Attr.m_AttrName;
		str += TEXT('=');
		str += TEXT('\"');
		str += Attr.m_AttrValue;
		str += TEXT('\"');
	}

	if (m_Value.IsEmpty() && m_Children.empty())
	{
		str += TEXT(" />") TEXT("\r\n");
		return;
	}


	str += TEXT(">");
	if (m_Children.size())
	{
		str += TEXT("\r\n");
	}
	if (m_Value.GetLength())
	{
		if (HasSpecialChar(m_Value))
		{
			//myCode 2013/06/11
			CString strValue = m_Value;
			NormalToEscaped(strValue);
			str += strValue;
		}
		else
		{
			str += m_Value;
		}

		if (m_Children.size())
		{
			str += TEXT("\r\n");
		}
	}



	for (size_t ii = 0; ii < m_Children.size(); ++ii)
	{
		++spaceCount;
		m_Children[ii].ToStringWithFormat(str, spaceCount, spaceChar); //recursively process children
		--spaceCount;
	}


	if (m_Name.GetLength())
	{
		if (m_Children.size())
		{
			for (int ii = 0; ii < spaceCount; ++ii)
			{
				str += spaceChar;
			}
		}
		str += TEXT("</");
		str += m_Name;
		str += TEXT(">") TEXT("\r\n");
	}
}


HRESULT XmlElement::ToStream(IFileDevice * pIFileDevice) CONST
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CString strXml;
	this->ToString(strXml);
	static CONST BYTE UTF16_TAG[] = { 0xff, 0xfe };
	ULONG NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(sizeof(UTF16_TAG), &NumberOfBytesWritten, UTF16_TAG);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(strXml.GetLength() * sizeof(TCHAR), &NumberOfBytesWritten, (LPCBYTE)strXml.GetString());
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT XmlElement::ToStreamWithFormat(IFileDevice * pIFileDevice, LPCTSTR spaceChar) CONST
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CString strXml;
	this->ToStringWithFormat(strXml, spaceChar);
	static CONST BYTE UTF16_TAG[] = { 0xff, 0xfe };
	ULONG NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(sizeof(UTF16_TAG), &NumberOfBytesWritten, UTF16_TAG);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(strXml.GetLength() * sizeof(TCHAR), &NumberOfBytesWritten, (LPCBYTE)strXml.GetString());
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT XmlElement::ToFile(LPCTSTR pFileName) CONST
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_WRITE, 0, CREATE_ALWAYS, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = this->ToStream(spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ToStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}


HRESULT XmlElement::ToFileWithFormat(LPCTSTR pFileName, LPCTSTR spaceChar) CONST
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_WRITE, 0, CREATE_ALWAYS, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = this->ToStreamWithFormat(spIFileDevice, spaceChar);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ToStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}


LONG XmlElement::FromString(CONST CString & str, INT & Start)
{
	LONG ElementCount = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (Start < 0)
	{
		LEAVE_BLOCK(0);
	}


	INT End = -1;
	GetNextElementTag(str, Start, Start, End);
	if (-1 == Start || -1 == End)
	{
		LEAVE_BLOCK(0);
	}

	//CString ElementTag = str.Mid(Start, End - Start + 1);
	ParseElementNameAndArributes(str, Start, End, m_Name, m_Attrs);

	//////////////////////////////////////////////////////////////////////////
	if (IsSingleClosedElenemt(str, Start, End)) //single closed element
	{
		++ElementCount;
		Start = End + 1;
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	INT ValueStart = Start = End + 1;
	GetNextElementTag(str, Start, Start, End);
	if (-1 == Start || -1 == End)
	{
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	m_Value = str.Mid(ValueStart, Start - ValueStart);
	RemoveComments(m_Value);
	TrimSpace(m_Value);
	EscapedToNormal(m_Value); //myCode 2013/06/11

	//////////////////////////////////////////////////////////////////////////
	for (;;)
	{
		//////////////////////////////////////////////////////////////////////////
		if (IsEndTag(str, Start, End))
		{
			CString ElenName = str.Mid(Start + 2, End - Start - 2);
			TrimSpace(ElenName);
			//ATLASSERT(ElenName == m_Name);
			if (ElenName != m_Name)
			{
				ElementCount = -1;
				D_API_ERR(0, TEXT("ElementNameMismatch"), ERROR_BAD_FORMAT, TEXT("[ElementName=%s, ExpectedElementName=%s]"), ElenName.GetString(), m_Name.GetString());
				break;
			}

			++ElementCount;
			Start = End + 1;
			//CString debug_01 = str.Mid(Start);
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		AllocateOneMore(m_Children);
		LONG ChildElementCount = m_Children.back().FromString(str, Start); //recursively process children
		if (ChildElementCount > 0)
		{
			ElementCount += ChildElementCount;
		}
		else
		{
			ElementCount = -1;
			break;
		}

		//CString debug_02 = str.Mid(Start);

		GetNextElementTag(str, Start, Start, End);
		if (-1 == Start || -1 == End)
		{
			break;
		}
	}
	if (ElementCount <= 0)
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return ElementCount;
}


HRESULT XmlElement::FromStream(IFileDevice * pIFileDevice)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CString strSource;
	hr = ReadSourceStream(
		pIFileDevice,//IFileDevice * pFile, 
		strSource//CString & strSource
		);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadSourceFile"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	int Start = 0;
	LONG eleCount = this->FromString(strSource, Start);
	if (eleCount <= 0)
	{
		hr = HRESULT_FROM_WIN32(ERROR_BAD_FORMAT);
		D_API_ERR(0, TEXT("FromString"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

HRESULT XmlElement::FromFile(LPCTSTR pFileName)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = this->FromStream(spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FromStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

//<sonmi01>2014-12-15 ###???
VOID XmlElement::FindAllChildren(CONST XmlElement & ele, LPCTSTR ChildName, vector<CONST XmlElement *> & vecChildren)
{
	for (CONST auto & var : ele.m_Children)
	{
		if (IsElementName(var, ChildName))
		{
			vecChildren.push_back(&var);
		}
	}
}


BOOL XmlElement::FindByPath(CONST XmlElement & ele, CONST LPCTSTR * Paths, INT PathsCount, CONST XmlElement * & containingEle, vector<CONST XmlElement *> & vecChildren)
{
	//////////////////////////////////////////////////////////////////////////
	if (PathsCount <= 0)
	{
		return FALSE;
	}


	//////////////////////////////////////////////////////////////////////////
	containingEle = &ele;
	FindAllChildren(*containingEle, Paths[0], vecChildren);
	if (PathsCount == 1)
	{
		return vecChildren.size() > 0;
	}


	//////////////////////////////////////////////////////////////////////////
	++ Paths;
	-- PathsCount;
	BOOL bFound = FALSE;
	for (size_t ii = 0; ii < vecChildren.size(); ++ ii)
	{
		vector<CONST XmlElement *> vecTemp;
		bFound = FindByPath(*vecChildren[ii], Paths, PathsCount, containingEle, vecTemp); //recursively call
		if (vecTemp.size())
		{
			vecChildren = vecTemp;
			break;
		}
	}
	return bFound;
}

BOOL XmlElement::FindAllByPath(
	IN CONST XmlElement & ele, //start root element
	IN CONST LPCTSTR * Paths,  //sub path
	IN INT PathsCount,		//sub path count
	OUT vector<CONST XmlElement *> & vecContainingEle, //containing element
	OUT vector<vector<CONST XmlElement *> > & vecVecChildren //resulted element
	)
{
	//////////////////////////////////////////////////////////////////////////
	if (PathsCount <= 0)
	{
		return FALSE;
	}


	//////////////////////////////////////////////////////////////////////////
	vector<CONST XmlElement *> vecChildrenTemp;
	FindAllChildren(ele, Paths[0], vecChildrenTemp);
	if (PathsCount == 1)
	{
		if (vecChildrenTemp.size())
		{
			vecContainingEle.push_back(&ele);
			vecVecChildren.push_back(vecChildrenTemp);
			return TRUE;
		}
		else
		{
			return FALSE;
		}	
	}


	//////////////////////////////////////////////////////////////////////////
	++Paths;
	--PathsCount;
	BOOL bFound = FALSE;
	for (size_t ii = 0; ii < vecChildrenTemp.size(); ++ii)
	{
		vector<CONST XmlElement *> vecContainingEleTemp;
		vector<vector<CONST XmlElement *> > vecVecTemp;
		BOOL bFoundTemp = FindAllByPath(*vecChildrenTemp[ii], Paths, PathsCount, vecContainingEleTemp, vecVecTemp); //recursively call
		if (bFoundTemp)
		{
			bFound = TRUE;
			for (CONST auto var : vecContainingEleTemp)
			{
				vecContainingEle.push_back(var);
			}

			for (CONST auto & var : vecVecTemp)
			{
				vecVecChildren.push_back(var);
			}
			//break;
		}
	}
	return bFound;
}





//INT XmlElement::SkipXmlHeader( CONST CString & str )
//{
//	INT Index = str.Find(TEXT("?>"));
//	return (-1 == Index)? 0 : (Index + 2);
//}
