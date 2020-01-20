#pragma once

#include "stdafx.h"
#include <vector>
using namespace std;

DWORD TranslateStartType(LPCWSTR StartTypeString);

//"123" "456" "678" to double-null ended multi strings
HRESULT QuotedStringsToBuffer(LPCTSTR pQuoted, TCHAR ** ppBuffer);
VOID FreeQuotedStringsBuffer(TCHAR * pBuffer);
