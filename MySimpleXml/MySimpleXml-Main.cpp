// MySimpleXml.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SimpleXml.h"
#include "sampleXmlString.h"
#include "StringValueConvert.h"
#include "ValueXmlElementConvert.h"
#include "VcxprojDisableOpt.h"
#include "FileTreeOp.h"
#include "build_single_file.h"
#include "GetTestCaseNumber.h"
#include "testSerializeSample.h"

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicParameterStreamDef.h"
#include "..\PublicHeader\PublicReadWriteTextFile.h"

#include "ReadWriteStringFile.h"
#include "AddFileAttributesInFolder.Serialize.h"

namespace {
//
//#define BEGIN_BLOCK(level_number)			do{	int	_abc_xyz##level_number(level_number);
//#define LEAVE_BLOCK(level_number)			{(_abc_xyz##level_number);}	break;
//#define END_BLOCK(level_number)				{(_abc_xyz##level_number);}	}while(0);

#if 0
	static HRESULT ReadSourceFile(LPCTSTR pFile, CString & strSource)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		HANDLE hFile = CreateFile(
			pFile,//_In_      LPCTSTR lpFileName,
			GENERIC_READ,//_In_      DWORD dwDesiredAccess,
			0,//_In_      DWORD dwShareMode,
			NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			OPEN_EXISTING,//_In_      DWORD dwCreationDisposition,
			0,//_In_      DWORD dwFlagsAndAttributes,
			NULL//_In_opt_  HANDLE hTemplateFile
			);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("CreateFile"), hr, TEXT("%s"), pFile);
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		CHandle ahFile(hFile); hFile = INVALID_HANDLE_VALUE;
		LARGE_INTEGER FileSize = {0};
		BOOL bRet = GetFileSizeEx(
			ahFile,//_In_   HANDLE hFile,
			&FileSize//_Out_  PLARGE_INTEGER lpFileSize
			);
		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("GetFileSizeEx"), hr, TEXT("%s"), pFile);
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////

		CHeapPtr<BYTE> apBuffer;
		apBuffer.Allocate(FileSize.LowPart + 512);
		ZeroMemory(apBuffer.m_pData, FileSize.LowPart + 512);
		DWORD NumberOfBytesRead = 0;
		bRet = ReadFile(
			ahFile,//_In_         HANDLE hFile,
			apBuffer.m_pData,//_Out_        LPVOID lpBuffer,
			FileSize.LowPart,//_In_         DWORD nNumberOfBytesToRead,
			&NumberOfBytesRead,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
			NULL//_Inout_opt_  LPOVERLAPPED lpOverlapped
			);

		INT iResult = 0;
		if ((IsTextUnicode(apBuffer.m_pData, FileSize.LowPart, &iResult) && (iResult & IS_TEXT_UNICODE_STATISTICS)) ||
			(apBuffer.m_pData[0] == 0xff && apBuffer.m_pData[1] == 0xfe) )
		{
			//D_INFO(0, TEXT("%s is detetced to be UNICODE"), pFile);
			strSource = (WCHAR*)(apBuffer.m_pData);
		}
		else
		{
			//D_INFO(0, TEXT("%s is detetced to be NON-UNICODE"), pFile);
			strSource = (CHAR*)(apBuffer.m_pData);
		}

		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("ReadFile"), hr, TEXT("%s"), pFile);
			LEAVE_BLOCK(0);
		}

		END_BLOCK(0);

		return hr;

	}

	//write unicode source files only
	static HRESULT WriteSourceFile(LPCTSTR pFile, CONST vector<CString> & strSource)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		HANDLE hFile = CreateFile(
			pFile,//_In_      LPCTSTR lpFileName,
			GENERIC_WRITE,//_In_      DWORD dwDesiredAccess,
			0,//_In_      DWORD dwShareMode,
			NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			CREATE_ALWAYS,//_In_      DWORD dwCreationDisposition,
			0,//_In_      DWORD dwFlagsAndAttributes,
			NULL//_In_opt_  HANDLE hTemplateFile
			);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("CreateFile"), hr, TEXT("%s"), pFile);
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		CHandle ahFile(hFile); hFile = INVALID_HANDLE_VALUE;

		DWORD NumberOfBytesRead = 0;
		DWORD NumberOfBytesWritten = 0;
		CONST BYTE ucIndicator[] = {0xff, 0xfe};
		BOOL bRet = WriteFile(
			ahFile,//_In_         HANDLE hFile,
			ucIndicator,//_In_         LPCVOID lpBuffer,
			sizeof(ucIndicator),//_In_         DWORD nNumberOfBytesToWrite,
			&NumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
			NULL//_Inout_opt_  LPOVERLAPPED lpOverlapped
			);
		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("WriteFile"), hr, TEXT("%s"), pFile);
			LEAVE_BLOCK(0);
		}

		for (size_t ii = 0; ii < strSource.size(); ++ ii)
		{
			NumberOfBytesRead = 0;
			NumberOfBytesWritten = 0;
			bRet = WriteFile(
				ahFile,//_In_         HANDLE hFile,
				strSource[ii].GetString(),//_In_         LPCVOID lpBuffer,
				(strSource[ii].GetLength() + 1) * sizeof(TCHAR),//_In_         DWORD nNumberOfBytesToWrite,
				&NumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
				NULL//_Inout_opt_  LPOVERLAPPED lpOverlapped
				);
			if (!bRet)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_LAST_STATUS(hr, LastError);
				D_API_ERR(0, TEXT("WriteFile"), hr, TEXT("%s"), pFile);
				LEAVE_BLOCK(0);
			}
		}


		END_BLOCK(0);

		return hr;
	}
#endif
	static VOID NOP() {}
}




int _tmain_test_rw_xml(int argc, _TCHAR* argv[])
{
	if (argc < 3)
	{
		_ftprintf_s(stdout, TEXT("this.exe sourceXml destXml") TEXT("\r\n"));
		return E_INVALIDARG;
	}

	//////////////////////////////////////////////////////////////////////////
	LPCTSTR pSourceFile = argv[1];
	LPCTSTR pDestFile = argv[2];

	XmlElement ele;
	ele.FromFile(pSourceFile);
	ele.ToFile(pDestFile);


#if 0	
	CString sourceXML;
	CString destXML;

	ReadSourceFile(pSourceFile, sourceXML);

	//////////////////////////////////////////////////////////////////////////
	XmlElement xmlElen;

	INT Start = 0; //XmlElement::SkipXmlHeader(sourceXML);
	xmlElen.FromString(sourceXML, Start);

	xmlElen.ToString(destXML);


	//////////////////////////////////////////////////////////////////////////
	vector<CString> vecXML;
	vecXML.push_back(destXML);
	WriteSourceFile(pDestFile, vecXML, TRUE);
#endif
	return 0;
}

int _tmain_value_string(int argc, _TCHAR* argv[])
{



	vector<LONGLONG> vecll;
	for (int ii = 0; ii < 10; ++ ii)
	{
		vecll.push_back(ii);
	}
	XmlElement e1;
	VectorToElement(FIELD_NAME(vecll), vecll, e1);

	CString strXml;
	e1.ToString(strXml);

	XmlElement e2;
	INT Start = 0;
	e2.FromString(strXml, Start);

	vector<LONGLONG> vecll2;
	ElementToVector(FIELD_NAME(vecll), vecll2, e2);

	ATLASSERT(vecll == vecll2);




	{
		CString str;
		ValueToString<LONGLONG>(-1, str);

		{
			LONGLONG val = 7428364;
			StringToValue(val, str);
			NOP();
		}

		ValueToString<ULONGLONG>(-1, str);
		{
			ULONGLONG val = 7428364;
			StringToValue(val, str);
			NOP();
		}

		ValueToString<LONG>(-1, str);
		{
			LONG val = 7428364;
			StringToValue(val, str);
			NOP();
		}
		ValueToString<ULONG>(-1, str);
		{
			ULONG val = 7428364;
			StringToValue(val, str);
			NOP();
		}
		ValueToString<CHAR>(-1, str);
		{
			CHAR val = (CHAR)7428364;
			StringToValue(val, str);
			NOP();
		}
		ValueToString<UCHAR>(-1, str);
		{
			UCHAR val = (UCHAR)7428364;
			StringToValue(val, str);
			NOP();
		}
		ValueToString<SHORT>(-1, str);
		{
			SHORT val = (SHORT)7428364;
			StringToValue(val, str);
			NOP();
		}
		ValueToString<USHORT>(-1, str);
		{
			USHORT val = (USHORT)7428364;
			StringToValue(val, str);
			NOP();
		}
		ValueToString<INT>(-1, str);
		{
			INT val = 7428364;
			StringToValue(val, str);
			NOP();
		}
		ValueToString<UINT>(-1, str);
		{
			UINT val = 7428364;
			StringToValue(val, str);
			NOP();
		}
		NOP();
	}

	{
		CString str;
		CHAR Value = -1;
		ValueToString(Value, str);
		NOP();
	}

	return 0;
}


#include "testStruct.h"
#include "testStruct.h.xml.Output.hpp"

int _tmain_test_struct_element_ToStringWithFormat(int argc, _TCHAR* argv[])
{
	//{
	//	LPCWSTR rootName = L"root";

	//	ut_complex data1;
	//	data1.init();

	//	CString str1;
	//	data1.ToStringWithFormat(rootName, str1);

	//	ut_complex data2;
	//	data2.FromString(rootName, str1);

	//	CString str2;
	//	data2.ToStringWithFormat(rootName, str2);

	//	//ATLASSERT(data1 == data2);
	//	ATLASSERT(str1 == str2);
	//}

	//{
	//	vec2d_complex var1;
	//	var1.init();

	//	CString str1;
	//	var1.ToStringWithFormat(L"root", str1);


	//	vec2d_complex var2;
	//	var2.FromString(L"root", str1);

	//	CString str2;
	//	var2.ToStringWithFormat(L"root", str2);

	//	//ATLASSERT(var1 == var2);
	//	ATLASSERT(str1 == str2);

	//}

	//////////////////////////////////////////////////////////////////////////
	{
		//////////////////////////////////////////////////////////////////////////
		SSSSBufferTest c1;
		c1.Init();

		XmlElement e1;
		c1.ToElement(FIELD_NAME(c1), e1);

		CString strXml;
		e1.ToStringWithFormat(strXml);


		//////////////////////////////////////////////////////////////////////////
		XmlElement e2;
		int Start = 0;
		e2.FromString(strXml, Start);

		SSSSBufferTest c2;
		c2.FromElement(FIELD_NAME(c1), e2);

		CString strV1 = (LPCTSTR)(&c2.buffer1[0]);
		CString strV2 = (LPCTSTR)(&c2.buffer2[0]);

		//////////////////////////////////////////////////////////////////////////
		XmlElement e3;
		c2.ToElement(FIELD_NAME(c1), e3);

		CString strXml3;
		Start = 0;
		e3.ToStringWithFormat(strXml3);

		ATLASSERT(strXml3 == strXml);
	}

	{
		//////////////////////////////////////////////////////////////////////////
		complex_1 c1;
		c1.Init();

		XmlElement e1;
		c1.ToElement(FIELD_NAME(c1), e1);

		CString strXml;
		e1.ToStringWithFormat(strXml);


		//////////////////////////////////////////////////////////////////////////
		XmlElement e2;
		int Start = 0;
		e2.FromString(strXml, Start);

		complex_1 c2;
		c2.FromElement(FIELD_NAME(c1), e2);

		//////////////////////////////////////////////////////////////////////////
		XmlElement e3;
		c2.ToElement(FIELD_NAME(c1), e3);

		CString strXml3;
		Start = 0;
		e3.ToStringWithFormat(strXml3);

		ATLASSERT(strXml3 == strXml);
	}

	return 0;
}


int _tmain_test_struct_element(int argc, _TCHAR* argv[])
{
	{
		LPCWSTR rootName = L"root";

		ut_complex data1;
		data1.init();

		CString str1;
		data1.ToString(rootName, str1);

		ut_complex data2;
		data2.FromString(rootName, str1);

		CString str2;
		data2.ToString(rootName, str2);

		//ATLASSERT(data1 == data2);
		ATLASSERT(str1 == str2);
	}

	{
		vec2d_complex var1;
		var1.init();

		CString str1;
		var1.ToString(L"root", str1);


		vec2d_complex var2;
		var2.FromString(L"root", str1);

		CString str2;
		var2.ToString(L"root", str2);

		//ATLASSERT(var1 == var2);
		ATLASSERT(str1 == str2);

	}

	//////////////////////////////////////////////////////////////////////////
	{
		//////////////////////////////////////////////////////////////////////////
		SSSSBufferTest c1;
		c1.Init();

		XmlElement e1;
		c1.ToElement(FIELD_NAME(c1), e1);

		CString strXml;
		e1.ToString(strXml);


		//////////////////////////////////////////////////////////////////////////
		XmlElement e2;
		int Start = 0;
		e2.FromString(strXml, Start);

		SSSSBufferTest c2;
		c2.FromElement(FIELD_NAME(c1), e2);

		CString strV1 = (LPCTSTR)(&c2.buffer1[0]);
		CString strV2 = (LPCTSTR)(&c2.buffer2[0]);

		//////////////////////////////////////////////////////////////////////////
		XmlElement e3;
		c2.ToElement(FIELD_NAME(c1), e3);

		CString strXml3;
		Start = 0;
		e3.ToString(strXml3);

		ATLASSERT(strXml3 == strXml);
	}

	{
		//////////////////////////////////////////////////////////////////////////
		complex_1 c1;
		c1.Init();

		XmlElement e1;
		c1.ToElement(FIELD_NAME(c1), e1);

		CString strXml;
		e1.ToString(strXml);


		//////////////////////////////////////////////////////////////////////////
		XmlElement e2;
		int Start = 0;
		e2.FromString(strXml, Start);

		complex_1 c2;
		c2.FromElement(FIELD_NAME(c1), e2);

		//////////////////////////////////////////////////////////////////////////
		XmlElement e3;
		c2.ToElement(FIELD_NAME(c1), e3);

		CString strXml3;
		Start = 0;
		e3.ToString(strXml3);

		ATLASSERT(strXml3 == strXml);
	}

	return 0;
}

int _tmain_test_struct_to_file(int argc, _TCHAR* argv[])
{
	complex_1 c1;
	c1.Init();

	static CONST TCHAR ROOT_NAME[] = TEXT("my_complext");;
	c1.ToFile(ROOT_NAME, argv[1]);


	complex_1 c2;
	c2.FromFile(ROOT_NAME, argv[1]);
	c2.ToFile(ROOT_NAME, argv[2]);

	return 0;
}

int _tmain_base64_coded(int argc, _TCHAR* argv[])
{
	vector<BYTE> vecBin1;
	for (int ii = 0; ii < 26; ++ ii)
	{
		vecBin1.push_back('a' + ii);
	}

	CString str1;
	ValueToString(vecBin1, str1);

	vector<BYTE> vecBin2;
	StringToValue(vecBin2, str1);

	ATLASSERT(vecBin1 == vecBin2);

	CString str2;
	ValueToString(vecBin2, str2);

	ATLASSERT(str1 == str2);

	return 0;


}




int _tmain(int argc, _TCHAR* argv[])
{
	D_SETTINGS(999, FALSE, NULL, NULL);

	int TestCase = 5;
	GetTestCaseNumber(argc, argv, TestCase);
	switch (TestCase)
	{
	case 18:
	{
		static CONST LPCTSTR FILE_EXT[] =
		{
			TEXT("*.h"),
			TEXT("*.hpp"),
			TEXT("*.hxx"),
			TEXT("*.hh"),
			TEXT("*.cpp"),
			TEXT("*.c"),
			TEXT("*.cc"),
			TEXT("*.inl"),
			TEXT("*.idl"),
			TEXT("*.asm"),
			TEXT("*.s"),
			TEXT("*.inc"),
			TEXT("*.rc"),
			TEXT("*.cs"),
			TEXT("*.java"),
			TEXT("*.js"),
			TEXT("*.ts"),
		};

		AddFileAttributesInFolderSerialize obj;
		for (INT ii = 0; ii < _countof(FILE_EXT); ++ ii)
		{
			obj.FileExtensions.push_back(FILE_EXT[ii]);
		}

		obj.ToFile(FIELD_NAME(AddFileAttributesInFolderSerialize), TEXT("AddFileAttributesInFolderSerialize.sample.config"));

	}
	break;
	case  17:
	{

		class CPrintEnumFolderFileOperation : public IEnumFolderFileOperation
		{
			CString m_delim;
		public:
			CPrintEnumFolderFileOperation(LPCTSTR delim) :
				m_delim(delim)
			{
				if (m_delim.GetLength() == 0)
				{
					m_delim = TEXT("\r\n");
				}
			}
			//struct IEnumFolderFileOperation
			//{
			//	virtual INT Operate(CONST CString & strFile) = 0;
			virtual INT Operate(CONST CString & strFile)
			{
				_ftprintf_s(stdout, L"%s" L"%s", strFile.GetString(), m_delim.GetString());
				return 0;
			}
			//};
		};
		CPrintEnumFolderFileOperation printName(argv[2]);
		EnumDirectoryFileTree(argv[1], L"*", &printName, nullptr);
	}
	break;
	case 16:
	{
		SYSTEMTIME st = { 0 };
		st.wYear = 8888;
		st.wMonth = 8;
		st.wDay = 8;
		st.wHour = 8;
		st.wMinute = 8;
		st.wSecond = 8;
		st.wMilliseconds = 8;
		FILETIME ft = {0};
		SystemTimeToFileTime(&st, &ft);
		CString str;
		ValueToString(ft, str);
		FILETIME ft2 = { 0 };
		StringToValue(ft2, str);
	}
	break;
	case 15:
	{
		vector<wstring> lines;
		SmartLib::ReadStdFile(argv[1], lines);
		
		CString fileName = argv[1];
		fileName += TEXT(".copy");
		SmartLib::WriteStdFile(fileName, lines);
	}
	break;
	case 14:
	{
		GUID iid = __uuidof(IUnknown);
		GUID iid1 = GUID_NULL;
		vector<BYTE> vecBin;
		ObjectToVector<GUID>(iid, vecBin);
		VectorToObject<GUID>(iid1, vecBin);
		GUID & iid2 = ConvertVector<GUID>(vecBin);

		ATLASSERT(iid == iid1 && iid1 == iid2);
	}
	break;
	case  13:
	{
		NS_testSerializeSample::tcomplex2::UT();
		tcomplex2::UT();
	}
	break;
	case 19:
	{
		SYSTEMTIME st = { 0 };
		GetSystemTime(&st);
		CString str;
		ValueToString(st, str);

		SYSTEMTIME st2;
		StringToValue(st2, str);

		ATLASSERT(0 == memcmp(&st, &st2, sizeof(SYSTEMTIME)));

		CString strEmpty;
		SYSTEMTIME st3 = { -1, -1, -1, -1, -1, -1, -1, -1};
		StringToValue(st3, strEmpty);
	}
	break;
	case 0:
		return _tmain_test_rw_xml(argc, argv);
		break;
	case 1:
		return _tmain_value_string(argc, argv);
		break;
	case  2:
		_tmain_test_struct_element_ToStringWithFormat(argc, argv);
		return _tmain_test_struct_element(argc, argv);
		break;
	case  3:
		return _tmain_test_struct_to_file(argc, argv);
		break;
	case 4:
		if (argc < 3)
		{
			D_INFO(0, TEXT("Usage: this.exe <source file> <target file>"));
			break;
		}
		VcxprojDisableOpt(argv[1], argv[2]);
		break;
	case 5:
		if (argc < 2)
		{
			D_INFO(0, TEXT("Usage: this.exe <VC projects directory>"));
			break;
		}
		VcxprojDisableOptRecurively(argv[1]);
		break;
	case 6:
		if (argc < 2)
		{
			D_INFO(0, TEXT("Usage: this.exe <source files directory>"));
			break;
		}
		AddReadonlyInFolder(argv[1], argv[2], TRUE);
		break;
	case 7:
		if (argc < 2)
		{
			D_INFO(0, TEXT("Usage: this.exe <source files directory>"));
			break;
		}
		RemoveReadonlyInFolder(argv[1], argv[2], TRUE);
		break;
	case 8:
	{
		_tmain_base64_coded(argc, argv);
		break;
	}
	case 9:
	{
		DWORD fileAttrib = FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN;
		LPCTSTR Dir = argv[2];
		RemoveFileAttributesInFolder(Dir, fileAttrib, TEXT("*.*"), TRUE);
		AddFileAttributesInFolder(Dir, fileAttrib, TEXT("*.*"), TRUE);
		RemoveFileAttributesInFolder(Dir, fileAttrib, TEXT("*.*"), TRUE);
		AddFileAttributesInFolder(Dir, fileAttrib, TEXT("*.*"), TRUE);
		RemoveFileInFolder(Dir, TEXT("*.*"), TRUE);
		RemoveFileInFolder(Dir, TEXT("*.*"), TRUE);
	}
	break;

	case  10:
	{
		vector<CString> Excludes;
		CString libDir;
		for (int ii = 0; ii < argc; ++ ii)
		{
			if (0 == _tcsicmp(TEXT("-x"), argv[ii]))
			{
				Excludes.push_back(argv[++ii]);
			}

			if (0 == _tcsicmp(TEXT("-libDir"), argv[ii]))
			{
				libDir = argv[++ii];
			}
		}
		BuildFilePath(argv[1], argv[2], libDir, Excludes);
	}
	break;

	case 11:
	{
		LPCTSTR strSOurceFile = argv[1];
		CString strSOurce;
		ReadSourceFile(strSOurceFile, strSOurce);
		XmlElement ele;
		INT stattIndex = 0;
		ele.FromString(strSOurce, stattIndex);

		LPCTSTR path[] = 
		{
			argv[2],
			argv[3]
		};

		//////////////////////////////////////////////////////////////////////////
		CONST XmlElement * containingEle = nullptr;
		vector<CONST XmlElement *> vecChildren;
		BOOL bFound = XmlElement::FindByPath(ele, path, _countof(path), containingEle, vecChildren);
		for (CONST auto & var : vecChildren)
		{
			CString str;
			var->ToString(str);
		}

		//////////////////////////////////////////////////////////////////////////
		vector<CONST XmlElement *> vecContainingEle;
		vector<vector<CONST XmlElement *> > vecVecChildren;
		bFound = XmlElement::FindAllByPath( //return TRUE - found; FALSE - not found
			ele,//IN CONST XmlElement & ele, ,//start root element
			path,//IN CONST LPCTSTR * Paths,  ,//sub path
			_countof(path),//IN INT PathsCount,		,//sub path count
			vecContainingEle,//OUT vector<CONST XmlElement *> & containingEle, ,//containing element
			vecVecChildren//OUT vector<vector<CONST XmlElement *> > & vecChildren ,//resulted element
			);

		for (CONST auto & var : vecContainingEle)
		{
			CString str;
			var->ToString(str);
		}

		for (CONST auto & vec : vecVecChildren)
		{
			for (CONST auto & var : vec)
			{
				CString str;
				var->ToString(str);
			}
		}

	}
	break;

	case 12:
	{
		vector<CString> strSource(1);
		ReadSourceFile(argv[1], strSource[0]);

		CString str;
		int index = 0;
		
		str = argv[1];
		index = str.ReverseFind(TEXT('.'));
		str.Insert(index, TEXT(".E_UTF8_NO_TAG"));
		WriteSourceFile(str.GetString(), strSource, E_WRITESOURCEFILE::E_UTF8_NO_TAG);

		str = argv[1];
		index = str.ReverseFind(TEXT('.'));
		str.Insert(index, TEXT(".E_UTF8"));
		WriteSourceFile(str.GetString(), strSource, E_WRITESOURCEFILE::E_UTF8);


		str = argv[1];
		index = str.ReverseFind(TEXT('.'));
		str.Insert(index, TEXT(".E_UTF16"));
		WriteSourceFile(str.GetString(), strSource, E_WRITESOURCEFILE::E_UTF16);
	}
	break;


	}
	return 0;
}