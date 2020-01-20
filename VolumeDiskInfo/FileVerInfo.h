#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>
using namespace std;


#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"

struct CFileVerInfo
{
private:
	struct LANGANDCODEPAGE
	{
		WORD wLanguage;
		WORD wCodePage;
	};

	static HRESULT GetImageFileType(LPCWSTR pwszModulePath, USHORT & usType);
	static BOOL QueryHelper(LPCVOID pBlock, LPCTSTR Name, LANGANDCODEPAGE * lpTranslate, UINT cbTranslate, CString & result);

public:
	static HRESULT GetImageFileVersion(LPCTSTR wstrPath, USHORT & usImageType, VS_FIXEDFILEINFO & ffi, vector<CString> & vecstr);
};
