// This file get from AFStor
#ifndef _AF_COMPRESS_H_
#define _AF_COMPRESS_H_
#include <windows.h>

namespace AFCMPR
{
    ULONG CompressTo(LPCWSTR pszDstFilename, HANDLE hSrc, ULONGLONG ullSrcSize, HANDLE hDst, LONGLONG* pllCompressedSize);
    ULONG UncompressTo(LPCWSTR pszSrcFileName, HANDLE hSrc, HANDLE hDst, LONGLONG* pllUncompressedSize);
}

#endif //_AF_COMPRESS_H_