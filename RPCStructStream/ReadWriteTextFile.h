#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>
using namespace std;

#include "..\PublicHeader\PublicFileDeviceInterface.h"

enum class E_WRITESOURCEFILE
{
	E_UTF8_NO_TAG = 0,
	E_UTF8,
	E_UTF16,
};


HRESULT ReadSourceFile(LPCTSTR pFile, CString & strSource);
HRESULT WriteSourceFile(LPCTSTR pFile, CONST vector<CString> & strSource, E_WRITESOURCEFILE e_writesourcefile);

HRESULT ReadSourceStream(IFileDevice * pFile, CString & strSource);
HRESULT WriteSourceStream(IFileDevice * pFile, CONST vector<CString> & strSource, E_WRITESOURCEFILE e_writesourcefile);