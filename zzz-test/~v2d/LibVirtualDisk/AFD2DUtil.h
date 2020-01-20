#ifndef _AFD2DUTIL_H_
#define _AFD2DUTIL_H_
#include <windows.h>
//#include "brandname.h"
#include <atlbase.h>
#include <string>
using std::wstring;

//
// Get ARCServer D2D installed path
// return value: 0 success, others fail
// @pwszPath - the installed path returned
// @pnCnt    - [in]  the count of input pwszPath buffer size
//             [out] the count of character in pwszPath, include the terminate '\0'.
//
#if 0
static int GetFlashInstallPath(/*out*/wchar_t *pwszPath, /*[in, out]*/unsigned long* pnCnt)
{

    if (pwszPath == NULL || pnCnt == NULL)
        return E_INVALIDARG;

    *pwszPath = L'\0';

    wstring strReg = CST_REG_ROOT_L;
    strReg += L"\\InstallPath";


    CRegKey regKey;
    LONG lRet = ERROR_SUCCESS;

    lRet = regKey.Open(HKEY_LOCAL_MACHINE, strReg.c_str(), KEY_READ);
    if (ERROR_SUCCESS == lRet)
    {
        lRet = regKey.QueryStringValue(L"Path", pwszPath, pnCnt);
    }

    return lRet;
}
#endif

static BOOL CreateAllDirecotries(LPCWSTR lpszFullPath, 
                                 LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{

    WCHAR szBasePath[MAX_PATH] = {0};

    WCHAR *posBasePath = NULL;

    if (NULL == lpszFullPath)
    {
        SetLastError(87);
        return FALSE;
    }

    if(::CreateDirectoryW(lpszFullPath, lpSecurityAttributes))
    {
        return TRUE;
    }
    else
    {
        if(GetLastError() == 183)
        {
            return TRUE;
        }
    }

    wcscpy_s(szBasePath, MAX_PATH, lpszFullPath);
    posBasePath = wcsrchr(szBasePath, '\\');

    if(NULL == posBasePath) 
    {
        return FALSE;
    }
    else 
    {
        *posBasePath = 0;
    }


    if (CreateAllDirecotries(szBasePath, lpSecurityAttributes))
    {
        return ::CreateDirectoryW(lpszFullPath, lpSecurityAttributes);
    }
    else 
    {
        return FALSE;
    }
}


#endif //_AFD2DUTIL_H_

