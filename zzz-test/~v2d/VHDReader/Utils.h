#pragma once
#include "stdafx.h"
#include <string>
using std::wstring;
using std::string;


wstring GetHumanReadableBytes ( LONGLONG llBytes );

string GetBufferString ( const char *pBuffer, const size_t maxLength );

wstring GetBufferString ( const wchar_t *pBuffer, const size_t maxLength );

wstring GetGUIDString ( const GUID oGUID );

long long ntohll ( const long long llInput );
long long htonll ( const long long llInput );

GUID GuidNtoH ( const GUID oNetworkGUID );
GUID GuidHtoN ( const GUID oNetworkGUID );

const unsigned char *GetMD5 ( unsigned char *pBuffer, const unsigned int iBufferSize );

long long MoveToFileOffset ( HANDLE hFile, long long llGotoPosition, DWORD dwMoveMethod = FILE_BEGIN );