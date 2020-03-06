#include "stdafx.h"
#include "AFCmpr.h"
#if 0
#include "zlib.h"
#include <stdlib.h>

#define CMPR_FLAG_COMPRESSED   0x01
#define CMPR_BLOCK_SIZE (2*1024*1024)
#define CMPR_SIG   0x52504D43

typedef struct _Node_Header
{
    ULONG ulSig;
    ULONG ulSize;
    ULONG ulFlags;
}NODEHDR, *PNODEHDR;

class CCachedFile
{
    DWORD m_dwSectorSize;
    HANDLE m_hFile;
    UCHAR* m_pCache;
    DWORD m_dwOffsetInCache;
    DWORD m_dwBufferSize;

    BOOL m_bReadMode;
    DWORD GetPaddingSize(DWORD dwSize, DWORD dwPaddingTo)
    {
        return (dwSize + dwPaddingTo - 1) & (~(dwPaddingTo-1));
    }

public:
    CCachedFile(LPCWSTR pszFilename, HANDLE hFile, BOOL bReadMode)
    {
        m_bReadMode = bReadMode;
        m_dwSectorSize = 512;
        m_hFile = hFile;
        WCHAR szVolume[MAX_PATH];
        if( GetVolumePathNameW(pszFilename, szVolume, _countof(szVolume)) )
        {
            DWORD dw1, dwSectorSize, dw3, dw4;
            if( GetDiskFreeSpace(szVolume, &dw1, &dwSectorSize, &dw3, &dw4) )
            {
                m_dwSectorSize = dwSectorSize;
            }
        }

        //AFLOG<<"Sector size:"<<m_dwSectorSize;

        m_dwBufferSize = m_dwSectorSize*4096;
        m_pCache = (UCHAR*)VirtualAlloc(0, m_dwBufferSize, MEM_COMMIT, PAGE_READWRITE);

        if( !m_pCache )
        {
            DWORD dwErr = GetLastError();
            //AFLOG<<"VirtualAlloc failed. err="<<dwErr;
        }

        if( bReadMode )
        {
            m_dwOffsetInCache = m_dwBufferSize;
        }
        else
        {
            m_dwOffsetInCache = 0;
        }
    }

    ~CCachedFile()
    {
        Flush(0);
        if( m_pCache )
        {
            VirtualFree(m_pCache, 0, MEM_RELEASE);
            m_pCache = NULL;
        }
    }

    BOOL IsValid()
    {
        return m_pCache != NULL;
    }

    BOOL Write(void* pBuffer, DWORD dwToWrite, DWORD* pdwWritten)
    {
        if( pdwWritten )
        {
            *pdwWritten = 0;
        }

        if( m_bReadMode )
        {
            //AFLOG<<"it's read mode.";
            return FALSE;
        }

        DWORD dwWritten = 0;
        UCHAR* pSrc = (UCHAR*)pBuffer;
        //write cache
        if( m_dwOffsetInCache )
        {
            DWORD dwCacheLeft = m_dwBufferSize - m_dwOffsetInCache;
            DWORD dwCopy = min(dwCacheLeft, dwToWrite);
            memcpy_s(&m_pCache[m_dwOffsetInCache], dwCacheLeft, pSrc, dwCopy);
            pSrc += dwCopy;
            m_dwOffsetInCache += dwCopy;
            dwToWrite -= dwCopy;

            if( m_dwOffsetInCache == m_dwBufferSize )
            {
                WriteFile(m_hFile, m_pCache, m_dwOffsetInCache, &dwWritten, 0);
                if( dwWritten != m_dwOffsetInCache )
                {
                    DWORD dwErr = GetLastError();
                    //AFLOG<<"WriteFile failed. err="<<dwErr;
                    SetLastError(dwErr);
                    return FALSE;
                }

                m_dwOffsetInCache = 0;
            }

            if( pdwWritten )
            {
                *pdwWritten += dwCopy;
            }
        }

        DWORD dwRemains = dwToWrite % m_dwBufferSize;
        DWORD dwWriteTimes = dwToWrite/m_dwBufferSize;
        //DWORD dwWriteSize = dwToWrite - dwRemains;

        for(DWORD i=0; i<dwWriteTimes; i++)
        {
            memcpy_s(m_pCache, m_dwBufferSize, pSrc, m_dwBufferSize);
            WriteFile(m_hFile, m_pCache, m_dwBufferSize, &dwWritten, 0);
            if( m_dwBufferSize != dwWritten )
            {
                DWORD dwErr = GetLastError();
                //AFLOG<<"WriteFile failed. err="<<dwErr;
                SetLastError(dwErr);
                return FALSE;
            }

            pSrc += dwWritten;
            dwToWrite -= dwWritten;
            if( pdwWritten )
            {
                *pdwWritten += dwWritten;
            }
        }

        if( dwRemains )
        {
            memcpy_s(m_pCache, m_dwBufferSize, pSrc, dwRemains);
            if( pdwWritten )
            {
                *pdwWritten += dwRemains;
            }

            m_dwOffsetInCache = dwRemains;
        }

        return TRUE;
    }

    BOOL Read(void* pBuffer, DWORD dwToRead, DWORD* pdwRead)
    {
        if( pdwRead )
        {
            *pdwRead = 0;
        }

        if( !m_bReadMode )
        {
            //AFLOG<<"it's write mode";
            return FALSE;
        }

        DWORD dwRead = 0;
        UCHAR* pDst = (UCHAR*)pBuffer;

        if( m_dwOffsetInCache < m_dwBufferSize )
        {
            DWORD dwCacheLeft = m_dwBufferSize - m_dwOffsetInCache;
            DWORD dwCopy = min(dwCacheLeft, dwToRead);
            memcpy_s(pDst, dwCopy, &m_pCache[m_dwOffsetInCache], dwCopy);
            pDst += dwCopy;
            m_dwOffsetInCache += dwCopy;
            dwToRead -= dwCopy;
            if( pdwRead )
            {
                *pdwRead += dwCopy;
            }
        }

        DWORD dwRemains = dwToRead % m_dwBufferSize;
        DWORD dwReadTimes = dwToRead/m_dwBufferSize;
        //DWORD dwReadSize = dwToRead - dwRemains;

        for(DWORD i=0; i<dwReadTimes; i++)
        {
            if( !ReadFile(m_hFile, m_pCache, m_dwBufferSize, &dwRead, 0) )
            {
                DWORD dwErr = GetLastError();
                //AFLOG<<"ReadFile failed. err="<<dwErr;
                SetLastError(dwErr);
                return FALSE;
            }

            memcpy_s(pDst, m_dwBufferSize, m_pCache, m_dwBufferSize);

            pDst += dwRead;
            dwToRead -= dwRead;
            if( pdwRead )
            {
                *pdwRead += dwRead;
            }
        }

        if( dwToRead )
        {
            if( !ReadFile(m_hFile, m_pCache, m_dwBufferSize, &dwRead, 0) )
            {
                DWORD dwErr = GetLastError();
                //AFLOG<<"ReadFile failed. err="<<dwErr;
                SetLastError(dwErr);
                return FALSE;
            }

            DWORD dwCopy = min(dwRead, dwToRead);
            memcpy_s(pDst, dwToRead, m_pCache, dwCopy);
            if( pdwRead )
            {
                *pdwRead += dwCopy;
            }

            m_dwOffsetInCache = dwCopy;
        }

        return TRUE;
    }

    BOOL Flush(DWORD* pdwPadding)
    {
        if( pdwPadding )
        {
            *pdwPadding = 0;
        }

        if( (!m_bReadMode) && m_dwOffsetInCache )
        {
            DWORD dwWriteSize = GetPaddingSize(m_dwOffsetInCache, m_dwSectorSize);
            DWORD dwWritten = 0;
            WriteFile(m_hFile, m_pCache, dwWriteSize, &dwWritten, 0);
            if( dwWritten != dwWriteSize )
            {
                DWORD dwErr = GetLastError();
                //AFLOG<<"WriteFile failed. err="<<dwErr;
                SetLastError(dwErr);
                return FALSE;
            }

            if( pdwPadding )
            {
                *pdwPadding = dwWriteSize-m_dwOffsetInCache;
            }
            m_dwOffsetInCache = 0;
        }

        return TRUE;
    }
};

ULONG AFCMPR::CompressTo(LPCWSTR pszDstFilename, HANDLE hSrc, ULONGLONG ullSrcSize, HANDLE hDst, LONGLONG* pllCompressedSize)
{
    if( pllCompressedSize )
    {
        *pllCompressedSize = 0;
    }

    CCachedFile cFile(pszDstFilename, hDst, FALSE);

    if( !cFile.IsValid() )
    {
        //AFLOG<<"Cached file is not valid.";
        return -1;
    }

    NODEHDR stHDR;
    stHDR.ulSig = CMPR_SIG;
    UCHAR* pSrc = new UCHAR[CMPR_BLOCK_SIZE];
    UCHAR* pDst = new UCHAR[CMPR_BLOCK_SIZE];
    if( (!pSrc) || (!pDst) )
    {
        if( pSrc ) delete[] pSrc;
        if( pDst ) delete[] pDst;
        ULONG ulRet = GetLastError();
        //AFLOG<<"Allocate buffer failed. err="<<ulRet;
        return ulRet == 0?-1:ulRet;
    }

    ULONG ulRead;
    ULONG ulWritten;
    ULONG ulTimes = (ULONG)(ullSrcSize/CMPR_BLOCK_SIZE);
    ULONG ulRemains = (ULONG)(ullSrcSize%CMPR_BLOCK_SIZE);

    for(ULONG i=0; i<ulTimes; i++)
    {
        ReadFile(hSrc, pSrc, CMPR_BLOCK_SIZE, &ulRead, 0);
        if( ulRead != CMPR_BLOCK_SIZE )
        {
            ULONG ulRet = GetLastError();
            //AFLOG<<"ReadFile failed. err="<<ulRet;
            delete[] pSrc;
            delete[] pDst;
            return ulRet == 0?-1:ulRet;
        }

        ULONG ulCompressed = CMPR_BLOCK_SIZE;
        ULONG zRet = compress2(pDst, &ulCompressed, pSrc, CMPR_BLOCK_SIZE, Z_DEFAULT_COMPRESSION);
        if(zRet != Z_OK || ulCompressed>= CMPR_BLOCK_SIZE)
        {
            stHDR.ulSize = CMPR_BLOCK_SIZE;
            stHDR.ulFlags = 0;
            memcpy_s(pDst, CMPR_BLOCK_SIZE, pSrc, stHDR.ulSize);
        }
        else
        {
            stHDR.ulSize = ulCompressed;
            stHDR.ulFlags = CMPR_FLAG_COMPRESSED;
        }

        //WriteFile(hDst, &stHDR, sizeof(stHDR), &ulWritten, 0);
        cFile.Write(&stHDR, sizeof(stHDR), &ulWritten );
        if( sizeof(stHDR) != ulWritten )
        {
            ULONG ulRet = GetLastError();
            //AFLOG<<"cFile.Write failed. err="<<ulRet;
            delete[] pSrc;
            delete[] pDst;
            return ulRet == 0?-1:ulRet;
        }

        if( pllCompressedSize )
        {
            *pllCompressedSize += ulWritten;
        }

        //WriteFile(hDst, pDst, stHDR.ulSize, &ulWritten, 0);
        cFile.Write(pDst, stHDR.ulSize, &ulWritten );
        if( stHDR.ulSize != ulWritten )
        {
            ULONG ulRet = GetLastError();
            //AFLOG<<"cFile.Write failed. err="<<ulRet;
            delete[] pSrc;
            delete[] pDst;
            return ulRet == 0?-1:ulRet;
        }

        if( pllCompressedSize )
        {
            *pllCompressedSize += ulWritten;
        }
    }

    if( ulRemains )
    {
        ReadFile(hSrc, pSrc, ulRemains, &ulRead, 0);
        if( ulRead != ulRemains )
        {
            ULONG ulRet = GetLastError();
            //AFLOG<<"ReadFile failed. err="<<ulRet;
            delete[] pSrc;
            delete[] pDst;
            return ulRet == 0?-1:ulRet;
        }

        ULONG ulCompressed = CMPR_BLOCK_SIZE;
        ULONG zRet = compress2(pDst, &ulCompressed, pSrc, ulRemains, Z_DEFAULT_COMPRESSION);
        if(zRet != Z_OK || ulCompressed>= ulRemains)
        {
            stHDR.ulSize = ulRemains;
            stHDR.ulFlags = 0;
            memcpy_s(pDst, CMPR_BLOCK_SIZE, pSrc, ulRemains);
        }
        else
        {
            stHDR.ulSize = ulCompressed;
            stHDR.ulFlags = CMPR_FLAG_COMPRESSED;
        }

        //WriteFile(hDst, &stHDR, sizeof(stHDR), &ulWritten, 0);
        cFile.Write(&stHDR, sizeof(stHDR), &ulWritten );
        if( sizeof(stHDR) != ulWritten )
        {
            ULONG ulRet = GetLastError();
            //AFLOG<<"cFile.Write failed. err="<<ulRet;
            delete[] pSrc;
            delete[] pDst;
            return ulRet == 0?-1:ulRet;
        }

        if( pllCompressedSize )
        {
            *pllCompressedSize += ulWritten;
        }

        //WriteFile(hDst, pDst, stHDR.ulSize, &ulWritten, 0);
        cFile.Write(pDst, stHDR.ulSize, &ulWritten );
        if( stHDR.ulSize != ulWritten )
        {
            ULONG ulRet = GetLastError();
            //AFLOG<<"cFile.Write failed. err="<<ulRet;
            delete[] pSrc;
            delete[] pDst;
            return ulRet == 0?-1:ulRet;
        }
        if( pllCompressedSize )
        {
            *pllCompressedSize += ulWritten;
        }
    }

    stHDR.ulFlags = 0;
    stHDR.ulSize = 0;
    //WriteFile(hDst, &stHDR, sizeof(stHDR), &ulWritten, 0);
    cFile.Write(&stHDR, sizeof(stHDR), &ulWritten );
    if( sizeof(stHDR) != ulWritten )
    {
        ULONG ulRet = GetLastError();
        //AFLOG<<"cFile.Write failed. err="<<ulRet;
        delete[] pSrc;
        delete[] pDst;
        return ulRet == 0?-1:ulRet;
    }

    if( pllCompressedSize )
    {
        *pllCompressedSize += ulWritten;
    }

    DWORD dwPadding = 0;
    if( !cFile.Flush(&dwPadding) )
    {
        ULONG ulRet = GetLastError();
        //AFLOG<<"cFile.Flush failed. err="<<ulRet;
        delete[] pSrc;
        delete[] pDst;
        return ulRet == 0?-1:ulRet;
    }

    if( pllCompressedSize )
    {
        *pllCompressedSize += dwPadding;
    }

    if( pSrc )
    {
        delete[] pSrc;
    }
    if( pDst )
    {
        delete[] pDst;
    }

    return 0;
}

ULONG AFCMPR::UncompressTo(LPCWSTR pszSrcFileName, HANDLE hSrc, HANDLE hDst, LONGLONG* pllUncompressedSize)
{
    if( pllUncompressedSize )
    {
        *pllUncompressedSize = 0;
    }

    CCachedFile cFile(pszSrcFileName, hSrc, TRUE);
    if( !cFile.IsValid() )
    {
        //AFLOG<<"Cached file is not valid.";
        return -1;
    }

    UCHAR* pSrc = new UCHAR[CMPR_BLOCK_SIZE];
    UCHAR* pDst = new UCHAR[CMPR_BLOCK_SIZE];
    if( (!pSrc) || (!pDst) )
    {
        if( pSrc ) delete[] pSrc;
        if( pDst ) delete[] pDst;
        ULONG ulRet = GetLastError();
        //AFLOG<<"Allocate buffer failed. err="<<ulRet;
        return ulRet == 0?-1:ulRet;
    }

    NODEHDR stHDR;

    DWORD dwRead;
    DWORD dwWritten;

    ULONG ulIndex = 0;
    while(1)
    {
        //ReadFile(hSrc, &stHDR, sizeof(stHDR), &dwRead, 0);
        cFile.Read(&stHDR, sizeof(stHDR), &dwRead);
        if( sizeof(stHDR) != dwRead )
        {
            ULONG ulRet = GetLastError();
            //AFLOG<<"cFile.Read failed. err="<<ulRet;
            delete[] pSrc;
            delete[] pDst;
            return ulRet == 0?-1:ulRet;
        }

        if( stHDR.ulSig != CMPR_SIG )
        {
            //AFLOG<<"stHDR.ulSig != CMPR_SIG";
            delete[] pSrc;
            delete[] pDst;
            return -1;
        }

        if( stHDR.ulSize == 0 && stHDR.ulFlags == 0 )
        {
            break;
        }

        //ReadFile(hSrc, pSrc, stHDR.ulSize, &dwRead, 0);
        cFile.Read(pSrc, stHDR.ulSize, &dwRead);
        if( dwRead != stHDR.ulSize )
        {
            ULONG ulRet = GetLastError();
            //AFLOG<<"cFile.Read failed. err="<<ulRet;
            delete[] pSrc;
            delete[] pDst;
            return ulRet == 0?-1:ulRet;
        }

        ULONG ulUncompressedSize = CMPR_BLOCK_SIZE;
        if( stHDR.ulFlags & CMPR_FLAG_COMPRESSED )
        {
            ULONG zRet = uncompress(pDst, &ulUncompressedSize, pSrc, dwRead);
            if( zRet != Z_OK )
            {
                //AFLOG<<"uncompress failed. err="<<zRet;
                delete[] pSrc;
                delete[] pDst;
                return zRet;
            }
        }
        else
        {
            memcpy_s(pDst, CMPR_BLOCK_SIZE, pSrc, stHDR.ulSize);
            ulUncompressedSize = stHDR.ulSize;
        }

        WriteFile(hDst, pDst, ulUncompressedSize, &dwWritten, 0);
        if( dwWritten != ulUncompressedSize )
        {
            ULONG ulRet = GetLastError();
            //AFLOG<<"WriteFile failed. err="<<ulRet;
            delete[] pSrc;
            delete[] pDst;
            return ulRet == 0?-1:ulRet;
        }
        if( pllUncompressedSize )
        {
            *pllUncompressedSize += dwWritten;
        }

        ulIndex++;
    }

    if( pSrc )
    {
        delete[] pSrc;
    }
    if( pDst )
    {
        delete[] pDst;
    }

    return 0;
}
#endif // if 0

