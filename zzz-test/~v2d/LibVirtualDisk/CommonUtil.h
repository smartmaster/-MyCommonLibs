#pragma once
#include <windows.h>
#include "HALog.h"
#include <string>
using namespace std;


template <class TV, class TM>
inline TV chROUNDUP(TV Value, TM Multiple) 
{
    return(chROUNDDOWN(Value, Multiple) + 
        (((Value % Multiple) > 0) ? Multiple : 0));
}

template <class TV, class TM>
inline TV chROUNDDOWN(TV Value, TM Multiple) 
{
    return((Value / Multiple) * Multiple);
}

//
//Auto-close handle class
//
template<HANDLE hInvalidVal = NULL>
class CEnsureCloseHandle
{
public:
    explicit CEnsureCloseHandle(HANDLE handle) : m_handle(handle) {};
    CEnsureCloseHandle() : m_handle(hInvalidVal) {};
    ~CEnsureCloseHandle(void) {Close();};
public:
    void Attach(HANDLE h)
    {
        Close();
        m_handle = h;
    }
    void Close()
    {
        if (hInvalidVal != m_handle)
            ::CloseHandle(m_handle);
        m_handle = hInvalidVal;
    }
    operator HANDLE() 
    { 
        return m_handle;
    }
private:
    HANDLE m_handle;
};

static DWORD WriteFileAndPreserveTimePos(HANDLE hFile,
                                         const void* lpBuffer,
                                         DWORD nNumberOfBytesToWrite,
                                         LPDWORD lpNumberOfBytesWritten,
                                         LARGE_INTEGER* pOffset)
{
    DWORD dwRet = 0;

    do
    {
        //Get file's modification time
        FILETIME ftLastModified;
        if (!GetFileTime(hFile, NULL, NULL, &ftLastModified))
        {
            dwRet = ::GetLastError();
            HALogError(L"DiffVHD2::Set parent path::Get not get file's last modified time, rc = %08X", dwRet);
            break;
        }

        //write back the header to file
        LARGE_INTEGER liPos = {0};
        LARGE_INTEGER liCur = {0};

        ::SetFilePointerEx(hFile, liPos, &liCur, FILE_CURRENT); //get current pos

        if (pOffset != NULL)
        {
            liPos.QuadPart = pOffset->QuadPart;
            ::SetFilePointerEx(hFile, liPos, NULL, FILE_BEGIN); //move to begin
        }

        if (!::WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, NULL))
        {
            dwRet = ::GetLastError();
            HALogError(L"WriteAndPreserve::write failed, rc = %08X, cb2Write = %08X, buf = %p", 
                dwRet, nNumberOfBytesToWrite, lpBuffer);
        }

        ::FlushFileBuffers(hFile);
        ::SetFilePointerEx(hFile, liCur, NULL, FILE_BEGIN); //restore file pointer

        //restore file's modification time
        if (!::SetFileTime(hFile, NULL, NULL, &ftLastModified))
        {
            dwRet = ::GetLastError();
            HALogError(L"WriteAndPreserve::Can not get file's last modified time, rc = %08X", dwRet);
            break;
        }
    } while (0);

    return dwRet;
};

static int CompareFilenames(LPCWSTR pszFilename1, LPCWSTR pszFilename2, int* pnBackSlashCount)
{
    LPCWSTR pSearch1 = pszFilename1;
    LPCWSTR pSearch2 = pszFilename2;

    *pnBackSlashCount = 0;
    int nRet = 0;
    while( 1 )
    {
        pSearch1 = wcschr(pSearch1, L'\\');
        pSearch2 = wcschr(pSearch2, L'\\');
        if( !pSearch1 || !pSearch2 )
        {
            return nRet;
        }

        pSearch1++;
        pSearch2++;
        DWORD dwLen1 = (DWORD)(pSearch1 - pszFilename1);
        DWORD dwLen2 = (DWORD)(pSearch2 - pszFilename2);

        if(dwLen1 != dwLen2 )
        {
            return nRet;
        }

        if( _wcsnicmp(pszFilename1, pszFilename2, dwLen1) )
        {
            return nRet;
        }

        nRet = dwLen1;
        (*pnBackSlashCount)++;
    }

    return 0;
}


/*
return 0 success
else do not use relative path
*/
static int GenParentRelitivePath(LPCWSTR pszImageFilename, 
                                 LPCWSTR pszParentFilename, 
                                 wstring& refRelitivePath)
{
    refRelitivePath = L"";
    WCHAR szImageTmp[MAX_PATH], szParentTmp[MAX_PATH];
    if( !GetVolumePathNameW(pszImageFilename, szImageTmp, _countof(szImageTmp)) )
    {
        DWORD dwRet = GetLastError();
        if(dwRet == 0 )
            dwRet = -1;
        HALogError(L"VHD reviser::GenParentRelitivePath::GetVolumePath failed, path = %s, rc = %d",
            pszImageFilename, dwRet);
        return dwRet;
    }

    if( !GetVolumePathNameW(pszParentFilename, szParentTmp, _countof(szParentTmp)) )
    {
        DWORD dwRet = GetLastError();
        if(dwRet == 0 )
            dwRet = -1;
        HALogError(L"VHD reviser::GenParentRelitivePath::GetVolumePathofparent failed, path = %s, rc = %d",
            pszParentFilename, dwRet);
        return dwRet;
    }

    BOOL bUseRelatedPath = FALSE;
    if( !_wcsicmp(szImageTmp, szParentTmp) )
    {
        bUseRelatedPath = TRUE;
    }

    do
    {
        if( bUseRelatedPath )
        {
            int nBackSlash = 0;
            int nPos = CompareFilenames(pszParentFilename, pszImageFilename, &nBackSlash);
            if( nBackSlash == 0 )
            {
                bUseRelatedPath = FALSE;
                break;
            }

            int nRemainBackSlash = 0;
            const WCHAR* p = wcschr(pszImageFilename+nPos, L'\\');
            while( p )
            {
                p++;
                p = wcschr(p, L'\\');
                nRemainBackSlash++;
            }

            szParentTmp[0] = 0;
            for(int i=0; i<nRemainBackSlash; i++)
            {
                wcscat_s(szParentTmp, _countof(szParentTmp), L"..\\");
            }

            if( nRemainBackSlash == 0 )
            {
                wcscpy_s(szParentTmp, _countof(szParentTmp), L".\\");
            }

            wcscat_s(szParentTmp, _countof(szParentTmp), pszParentFilename+nPos);
        }
    } while (0);

    if (!bUseRelatedPath)
        return -1;

    refRelitivePath = szParentTmp;
    return 0;
}

//
//If File exist
//
inline bool FileExist(const wchar_t* pwszFile)
{
    if (pwszFile == NULL) return false;

    bool bExist = false;
    WIN32_FIND_DATA find_data;

    HANDLE hFind = FindFirstFileW(pwszFile, &find_data);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        ::FindClose(hFind);
        bExist = true;
    }

    return bExist;
}