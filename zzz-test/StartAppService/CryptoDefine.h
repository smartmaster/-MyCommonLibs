#pragma once

#ifndef IN
#define IN
#endif

#ifndef OPT
#define OPT
#endif

#ifndef OUT
#define OUT
#endif

#define DWORDHIGH(X) ((X) >> 16)

#define DWORDLOW(X) ((X) & 0xFFFF)

#define MERGEDWORD(X, Y) (((X) << 16) | ((Y) & 0xFFFF))

#define WSTR_S(X) ((X) ? (X) : L"")

#define DisplayStr(X) ((X) ? (X) : L"nul")

#define BOOL_S(X) ((x) ? L"true" : L"false")

#define GUID_FMT  L"{%.8X-%.4X-%.4X-%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X}"
#define GUID_ARG(x)                                         \
    (x).Data1,                                              \
    (x).Data2,                                              \
    (x).Data3,                                              \
    (x).Data4[0], (x).Data4[1], (x).Data4[2], (x).Data4[3], \
    (x).Data4[4], (x).Data4[5], (x).Data4[6], (x).Data4[7]

#define VOLSN_FMT L"%04X-%04X"
#define VOLSN_ARG(X) (USHORT)DWORDHIGH(X), (USHORT)DWORDLOW(X)

#define DATE_FMT L"%04d-%02d-%02d %02d:%02d:%02d"
#define DATE_ARG(X)                                         \
    (X).GetYear(),                                          \
    (X).GetMonth(),                                         \
    (X).GetDay(),                                           \
    (X).GetHour(),                                          \
    (X).GetMinute(),                                        \
    (X).GetSecond()

#define GEN_EVAL(X) X
#define GEN_MERGE(A, B) A##B
#define GEN_MAKE_W(A) GEN_MERGE(L, A)

#define GEN_STRINGIZE_ARG(X) #X
#define GEN_STRINGIZE(X) GEN_EVAL(GEN_STRINGIZE_ARG(X))

#ifndef __WFUNCTION__
#define __WFUNCTION__ GEN_MAKE_W(GEN_EVAL(__FUNCTION__))
#endif

#ifndef __WFILE__
#define __WFILE__ GEN_MAKE_W(GEN_EVAL(__FILE__))
#endif

#ifndef MS_ENH_RSA_AES_PROV_XP_W
#define MS_ENH_RSA_AES_PROV_XP_W L"Microsoft Enhanced RSA and AES Cryptographic Provider (Prototype)"
#endif

#ifndef MS_ENH_RSA_AES_PROV_W
#define MS_ENH_RSA_AES_PROV_W   L"Microsoft Enhanced RSA and AES Cryptographic Provider"
#endif

#ifndef BCRYPT_AES_ALGORITHM
#define BCRYPT_AES_ALGORITHM                    L"AES"
#endif

#ifndef BCRYPT_MD5_ALGORITHM
#define BCRYPT_MD5_ALGORITHM                    L"MD5"
#endif

#ifndef BCRYPT_SHA1_ALGORITHM
#define BCRYPT_SHA1_ALGORITHM                   L"SHA1"
#endif

#ifndef BCRYPT_SHA256_ALGORITHM
#define BCRYPT_SHA256_ALGORITHM                 L"SHA256"
#endif

#ifndef BCRYPT_SHA384_ALGORITHM
#define BCRYPT_SHA384_ALGORITHM                 L"SHA384"
#endif

#ifndef BCRYPT_SHA512_ALGORITHM
#define BCRYPT_SHA512_ALGORITHM                 L"SHA512"
#endif

#ifndef CRYPT_STRING_BASE64HEADER
#define CRYPT_STRING_BASE64HEADER           0x00000000
#endif

#ifndef CRYPT_STRING_BASE64
#define CRYPT_STRING_BASE64                 0x00000001
#endif

#ifndef CRYPT_STRING_BASE64REQUESTHEADER
#define CRYPT_STRING_BASE64REQUESTHEADER    0x00000003
#endif

#ifndef CRYPT_STRING_HEX
#define CRYPT_STRING_HEX                    0x00000004
#endif

#ifndef CRYPT_STRING_HEXASCII               
#define CRYPT_STRING_HEXASCII               0x00000005
#endif

#ifndef CRYPT_STRING_HEXADDR
#define CRYPT_STRING_HEXADDR                0x0000000a
#endif

#ifndef CRYPT_STRING_BASE64_ANY
#define CRYPT_STRING_BASE64_ANY             0x00000006
#endif

#ifndef CRYPT_STRING_HEX_ANY
#define CRYPT_STRING_HEX_ANY                0x00000008
#endif

#ifndef CRYPT_STRING_HEXASCIIADDR
#define CRYPT_STRING_HEXASCIIADDR           0x0000000b
#endif

#ifndef CRYPT_STRING_HEXRAW
#define CRYPT_STRING_HEXRAW                 0x0000000c
#endif

#ifndef CRYPT_STRING_NOCRLF
#define CRYPT_STRING_NOCRLF                 0x40000000
#endif

#ifndef CRYPT_STRING_NOCR
#define CRYPT_STRING_NOCR                   0x80000000
#endif

