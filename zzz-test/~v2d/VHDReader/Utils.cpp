#include "stdafx.h"
#include "utils.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
using namespace std;

extern "C"
{
#include "md5.h"
}

wstring GetHumanReadableBytes ( LONGLONG llBytes )
{
    wstringstream sStream;
    if ( llBytes < 1024 )
    {
        sStream << llBytes << " bytes";
    }
    else if ( llBytes < 1024*1024 ) 
    {
        sStream << setiosflags(ios::fixed) << setprecision ( 2 );
        sStream << (float)llBytes/(float)1024 << " KB";
    }
    else if ( llBytes < 1024 * 1024 * 1024 )
    {
        sStream << setiosflags (ios::fixed) << setprecision ( 2 );
        sStream << (float)llBytes / (float)(1024*1024) << " MB";
    }
    else
    {
        sStream << setiosflags (ios::fixed) << setprecision ( 2 );
        sStream << (float)llBytes / (float)(1024*1024*1024) << " GB";
    }

    return sStream.str();
};

string GetBufferString ( const char *pBuffer, const size_t maxLength )
{
    char *pTemp = new char [maxLength+1];

    if ( pTemp == NULL )
        return "Error : Insufficient memory";
    
    ZeroMemory ( pTemp, maxLength + 1 );
    strncpy_s ( pTemp, maxLength+1, pBuffer, maxLength );

    string sReturn = pTemp;
    delete [] pTemp;

    return sReturn;
};

wstring GetBufferString ( const wchar_t *pBuffer, const size_t maxLength )
{
    wchar_t *pTemp = new wchar_t [maxLength+1];

    if ( pTemp == NULL )
        return L"Error : Insufficient memory";
    
    ZeroMemory ( pTemp, (maxLength + 1)*sizeof(wchar_t) );
    wcsncpy_s ( pTemp, maxLength+1, pBuffer, maxLength );

    wstring sReturn = pTemp;
    delete [] pTemp;

    return sReturn;
};

wstring GetGUIDString ( const GUID oGUID )
{
    wstringstream oStream;

    oStream << L"{";
    oStream << setiosflags (ios::fixed) << setw ( 8 ) << setbase(16) <<  setfill (L'0') << setprecision(0) << oGUID.Data1;
    oStream << L"-";
    oStream << setw ( 4 ) << oGUID.Data2;
    oStream << L"-";
    oStream << setw ( 4 ) << oGUID.Data3;
    oStream << L"-";
    oStream << setw(2);
    oStream << setw(2) << (BYTE) oGUID.Data4[0];
    oStream << setw(2) << (BYTE) oGUID.Data4[1];
    oStream << L"-";
    oStream << setw(2) << (BYTE) oGUID.Data4[2];
    oStream << setw(2) << (BYTE) oGUID.Data4[3];
    oStream << setw(2) << (BYTE) oGUID.Data4[4];
    oStream << setw(2) << (BYTE) oGUID.Data4[5];
    oStream << setw(2) << (BYTE) oGUID.Data4[6];
    oStream << setw(2) << (BYTE) oGUID.Data4[7];
    oStream << L"}";
    return oStream.str();
}

long long htonll ( const long long llInput )
{
    long long lTemp = llInput;
    long long lOut = 0;

    // TODO: This translation should only be done in BIG ENDIAN machines. We need
    //       to figure out a way to determine the endinness of a machine here. For
    //       now assume that it is big engine machine.
    for ( int x = 0; x < 8; x++ )
    {
        lOut <<= 8;
        lOut += (lTemp & 0xFF);
        lTemp >>=8;
    } // for ( int x = 0; x < 8; x++ )

    return lOut;
}

long long ntohll ( const long long llInput )
{
    long long lTemp = llInput;
    long long lOut = 0;

    // TODO: This translation should only be done in BIG ENDIAN machines. We need
    //       to figure out a way to determine the endinness of a machine here. For
    //       now assume that it is big engine machine.
    for ( int x = 0; x < 8; x++ )
    {
        lOut <<= 8;
        lOut += (lTemp & 0xFF);
        lTemp >>=8;
    } // for ( int x = 0; x < 8; x++ )

    return lOut;
}


long long MoveToFileOffset ( HANDLE hFile, long long llGotoPosition, DWORD dwMoveMethod  )
{
    if ( hFile == INVALID_HANDLE_VALUE )
        return -1;
    
    LARGE_INTEGER   liMovePosition;
    LARGE_INTEGER   liNewPosition;

    liMovePosition.QuadPart = llGotoPosition;
    liNewPosition.QuadPart = 0;

    if (!SetFilePointerEx (hFile, liMovePosition, &liNewPosition, dwMoveMethod ) )
    {
        //m_dwLastError = GetLastError ();
        return -1;
    } // if (!SetFilePointerEx (m_hVHDFile, &liMovePosition, &liNewPosition, dwMoveMethod ) )

    return liNewPosition.QuadPart;
}

GUID GuidNtoH ( const GUID oNetworkGUID)
{
    GUID oOutGUID;

    oOutGUID.Data1 = ntohl ( oNetworkGUID.Data1 );
    oOutGUID.Data2 = ntohs ( oNetworkGUID.Data2 );
    oOutGUID.Data3 = ntohs ( oNetworkGUID.Data3 );

    memcpy ( oOutGUID.Data4, oNetworkGUID.Data4, 8 );

    return oOutGUID;
}

GUID GuidHtoN ( const GUID oNetworkGUID )
{
    GUID oOutGUID;

    oOutGUID.Data1 = htonl ( oNetworkGUID.Data1 );
    oOutGUID.Data2 = htons ( oNetworkGUID.Data2 );
    oOutGUID.Data3 = htons ( oNetworkGUID.Data3 );

    memcpy ( oOutGUID.Data4, oNetworkGUID.Data4, 8 );

    return oOutGUID;
}

const unsigned char *GetMD5 ( unsigned char *pBuffer, const unsigned int iBufferSize )
{
    MD5_CTX oMD5;
    static unsigned char sMD5 [ 16 ];

    MD5Init ( &oMD5 );
    MD5Update ( &oMD5, pBuffer, iBufferSize );
    MD5Final ( sMD5, &oMD5 );

    return sMD5;
}