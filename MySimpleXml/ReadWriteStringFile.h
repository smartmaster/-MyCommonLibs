#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>
#include <string>
using namespace std;


//////////////////////////////////////////////////////////////////////////
HRESULT ReadSourceStringFile(LPCTSTR pSourceFile, CString & strSource);

//write unicode source files only
HRESULT WriteTargetStringFile(LPCTSTR pFile, CONST vector<CString> & strSource);




namespace SmartLib
{
	HRESULT ReadStdFile(LPCTSTR fileNane, vector<wstring> & lines);
	HRESULT WriteStdFile(LPCTSTR fileName, const vector<wstring> & lines);
} //namespace SmartLib