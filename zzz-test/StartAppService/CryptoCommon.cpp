/**************************************************************************************************
                             Some debug switch for cryptography module
 --------------------------------------------------------------------------------------------------
 Usage: Create a file named "CryptoDebug.ini" and copy the relevant keys. If some key is ignored. 
        remove it or set the value to empty(zero).
 **************************************************************************************************
[CryptoParam]
#Encryption library type. ELT_NOENCRYTION=0, ELT_MSCRYPTO=1, ELT_ETPKI=2
dwEncLibType = 1
#Hash algorithm type.  EHAT_MD5=1, EHAT_SHA1=2, EHAT_SHA_256=3, EHAT_SHA_384=4, EHAT_SHA_512=5
dwHashAlgType = 1
#Encryption API type. ECAT_BY_OS=1, ECAT_CAPI=2, ECAT_CNG=3
dwEncAPIType = 1
#Encryption algorithm type. EEAT_AES_128BIT=1, EEAT_AES_192BIT=2, EEAT_AES_256BIT=3
dwEncAlgType = 1
#If use IV for enc/dec, 
dwIVUsage = 1. EIU_NOT_USE=1, EIU_USE_IV=2
#Password for enc/dec
wsPassword = "zhoyu03"
**************************************************************************************************/
#include "stdafx.h"
#include "CryptoCommon.h"

DWORD CCryptoUtility::GetCSPType(E_ENCALG_TYPE eEnvAlgType)
{
    switch(eEnvAlgType)
    {
    case EEAT_AES_128BIT:
    case EEAT_AES_192BIT:
    case EEAT_AES_256BIT:
        return PROV_RSA_AES;
    case EEAT_3DES:
        return PROV_RSA_FULL;
    }
    return PROV_RSA_AES;
}

const WCHAR* CCryptoUtility::GetCSPName(E_ENCALG_TYPE eEnvAlgType)
{
    switch(eEnvAlgType)
    {
    case EEAT_AES_128BIT:
    case EEAT_AES_192BIT:
    case EEAT_AES_256BIT:
        {
            OSVERSIONINFO OSVersion = {0};
            OSVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
            GetVersionEx(&OSVersion);
            // If windows XP.
            if((5 == OSVersion.dwMajorVersion) && (1 == OSVersion.dwMinorVersion))
                return MS_ENH_RSA_AES_PROV_XP_W;
            else
                return MS_ENH_RSA_AES_PROV_W;
        }
    }
    return MS_DEF_PROV;
}

const WCHAR* CCryptoUtility::GetEncAlgorithmName(E_ENCALG_TYPE eEnvAlgType)
{
    switch(eEnvAlgType)
    {
    case EEAT_AES_128BIT:
    case EEAT_AES_192BIT:
    case EEAT_AES_256BIT:
        return BCRYPT_AES_ALGORITHM;
    case EEAT_3DES:
        return BCRYPT_3DES_ALGORITHM;
    }
    return BCRYPT_AES_ALGORITHM;
}

DWORD CCryptoUtility::GetEncAlgorithmID(E_ENCALG_TYPE eEnvAlgType)
{
    switch(eEnvAlgType)
    {
    case EEAT_AES_128BIT:
        return CALG_AES_128;
    case EEAT_AES_192BIT:
        return CALG_AES_192;
    case EEAT_AES_256BIT:
        return CALG_AES_256;
    case EEAT_3DES:
        return CALG_3DES;
    }
    return EEAT_AES_128BIT;
}

DWORD CCryptoUtility::GetHashAlgType(E_HASHALG_TYPE eHashAlgType)
{
    switch(eHashAlgType)
    {
    case EHAT_MD5:
        return CALG_MD5;
    case EHAT_SHA1:
        return CALG_SHA1;
    case EHAT_SHA_256:
        return CALG_SHA_256;
    case EHAT_SHA_384:
        return CALG_SHA_384;
    case EHAT_SHA_512:
        return CALG_SHA_512;
    }
    return CALG_SHA1;
}

const WCHAR* CCryptoUtility::GetHashAlgCSPName(E_HASHALG_TYPE eHashAlgType)
{
    switch(eHashAlgType)
    {
    case EHAT_MD5:
        return BCRYPT_MD5_ALGORITHM;
    case EHAT_SHA1:
        return BCRYPT_SHA1_ALGORITHM;
    case EHAT_SHA_256:
        return BCRYPT_SHA256_ALGORITHM;
    case EHAT_SHA_384:
        return BCRYPT_SHA384_ALGORITHM;
    case EHAT_SHA_512:
        return BCRYPT_SHA512_ALGORITHM;
    }
    return BCRYPT_SHA1_ALGORITHM;
}

const WCHAR* CCryptoUtility::GetKeyContainerName()
{
    return gwzKeyContainerNameForCAPI;
}

wstring CCryptoUtility::GetRandomKeyContainerName(const WCHAR* pwzGUID /* = NULL */)
{
    wstring wsRandomGUID;
    if (NULL == pwzGUID)
        wsRandomGUID = CCryptoUtility::GetRandomGUID();
    else
        wsRandomGUID = pwzGUID;

    SYSTEMTIME stSysTemTime = {0};
    GetSystemTime(&stSysTemTime);

    WCHAR wzRandomKeySetName[MAX_PATH] = {0};
    swprintf_s(wzRandomKeySetName,
        _countof(wzRandomKeySetName), 
        L"%s_%u%u%u%u%u%u%u_%s",
        gwzKeyContainerNameForCAPI,
        stSysTemTime.wYear,
        stSysTemTime.wMonth,
        stSysTemTime.wDay,
        stSysTemTime.wHour,
        stSysTemTime.wMinute,
        stSysTemTime.wSecond,
        stSysTemTime.wMilliseconds,
        WS_S(wsRandomGUID));

    DBGTRC(L"Key container name: %s", WZ_S(wzRandomKeySetName));
    
    return wzRandomKeySetName;  
}

wstring
CCryptoUtility::GetRandomGUID(GUID* pGUID /* = NULL */)
{
    GUID guidRandom = {0};

    CCOMInitGuard COMInit;
    if (COMInit.IsCOMInited())
    {
        HRESULT hrRet = ::CoCreateGuid(&guidRandom);
        if (FAILED(hrRet))
            ERRTRCEX(hrRet, L"Failed to create GUID, return GUID_NULL. (EC=[%#08x])", hrRet);
    }
    COMInit.UnInitCOM();

    WCHAR wzGUIDStr[MAX_PATH] = {0};
    swprintf_s(wzGUIDStr, WSTR_GUID_FMT, GUID_PRINTF_ARG(guidRandom));

    DBGTRC(L"Get random GUID: %s.", WZ_S(wzGUIDStr));

    if (pGUID)
        memcpy(pGUID, &guidRandom, sizeof(GUID));

    return wzGUIDStr;
}

void CCryptoUtility::UpdateCryptoParam(CCryptoParam& CryptoParam, CCryptoConfig& CryptoCfg)
{
    if(CryptoCfg.m_CryptoDBGCfg.m_CryptoParam.m_dwEncLibType != ELT_NOENCRYTION)
        CryptoParam.m_dwEncLibType = CryptoCfg.m_CryptoDBGCfg.m_CryptoParam.m_dwEncLibType;
    else if(ELT_NOENCRYTION == CryptoParam.m_dwEncLibType)
    {
        if(CryptoCfg.m_CryptoAlgConfig.m_CryptoParam.m_dwEncLibType != ELT_NOENCRYTION)
            CryptoParam.m_dwEncLibType = CryptoCfg.m_CryptoAlgConfig.m_CryptoParam.m_dwEncLibType;
        else
            CryptoParam.m_dwEncLibType = ELT_MSCRYPTO;
    }

    if(CryptoCfg.m_CryptoDBGCfg.m_CryptoParam.m_dwEncAlgType != EEAT_UNKNOWN)
        CryptoParam.m_dwEncAlgType = CryptoCfg.m_CryptoDBGCfg.m_CryptoParam.m_dwEncAlgType;
    else if(EEAT_UNKNOWN == CryptoParam.m_dwEncAlgType)
    {
        if(CryptoCfg.m_CryptoAlgConfig.m_CryptoParam.m_dwEncAlgType != EEAT_UNKNOWN)
            CryptoParam.m_dwEncAlgType = CryptoCfg.m_CryptoAlgConfig.m_CryptoParam.m_dwEncAlgType;
        else
            CryptoParam.m_dwEncAlgType = EEAT_AES_128BIT;
    }

    if(CryptoCfg.m_CryptoDBGCfg.m_CryptoParam.m_dwEncAPIType != ECAT_UNKNOWN)
        CryptoParam.m_dwEncAPIType = CryptoCfg.m_CryptoDBGCfg.m_CryptoParam.m_dwEncAPIType;
    else if(ECAT_UNKNOWN == CryptoParam.m_dwEncAPIType)
    {
        if(CryptoCfg.m_CryptoAlgConfig.m_CryptoParam.m_dwEncAPIType != ECAT_UNKNOWN)
            CryptoParam.m_dwEncAPIType = CryptoCfg.m_CryptoAlgConfig.m_CryptoParam.m_dwEncAPIType;
        else
            CryptoParam.m_dwEncAPIType = ECAT_BY_OS;
    }

    if(CryptoCfg.m_CryptoDBGCfg.m_CryptoParam.m_dwHashAlgType != EHAT_UNKNOWN)
        CryptoParam.m_dwHashAlgType = CryptoCfg.m_CryptoDBGCfg.m_CryptoParam.m_dwHashAlgType;
    else if(EHAT_UNKNOWN == CryptoParam.m_dwHashAlgType)
    {
        if(CryptoCfg.m_CryptoAlgConfig.m_CryptoParam.m_dwHashAlgType != EHAT_UNKNOWN)
            CryptoParam.m_dwHashAlgType = CryptoCfg.m_CryptoAlgConfig.m_CryptoParam.m_dwHashAlgType;
        else
            CryptoParam.m_dwHashAlgType = EHAT_MD5;
    }

    if(CryptoCfg.m_CryptoDBGCfg.m_CryptoParam.m_dwIVUsage != EIU_UNKNOWN)
        CryptoParam.m_dwIVUsage = CryptoCfg.m_CryptoDBGCfg.m_CryptoParam.m_dwIVUsage;
    else if(EIU_UNKNOWN == CryptoParam.m_dwIVUsage)
    {
        if(CryptoCfg.m_CryptoAlgConfig.m_CryptoParam.m_dwIVUsage != EIU_UNKNOWN)
            CryptoParam.m_dwIVUsage = CryptoCfg.m_CryptoAlgConfig.m_CryptoParam.m_dwIVUsage;
        else
            CryptoParam.m_dwIVUsage = EIU_NOT_USE;
    }

    if(!CryptoCfg.m_CryptoDBGCfg.m_CryptoParam.m_wsPassword.empty())
        CryptoParam.m_wsPassword = CryptoCfg.m_CryptoDBGCfg.m_CryptoParam.m_wsPassword;
}

wstring CCryptoUtility::AppendBackslash(wstring str)
{
    if (str.length() == 0)
        return wstring(L"\\");
    if (str[str.length() - 1] == L'\\')
        return str;
    return str.append(L"\\");
}

wstring CCryptoUtility::RemoveBackslash(wstring str) 
{
    if (str.length() == 0)
        return wstring(L"");
    if (str[str.length() - 1] == L'\\')
        return str.substr(0, str.length() - 1);
    return str;
}

long CCryptoUtility::Bin2HexString(wstring& wsHexStr, const PBYTE pBinBuf, DWORD dwBinBufSize)
{
    if((NULL == pBinBuf) || (0 == dwBinBufSize))
        return E_INVALIDARG;

    wsHexStr.clear();

    WCHAR wzByteStr[MAX_PATH] = {0};
    for(DWORD dwIndex = 0; dwIndex < dwBinBufSize; dwIndex++)
    {
        swprintf_s(wzByteStr, _countof(wzByteStr), L"%02x", pBinBuf[dwIndex]);
        wsHexStr += wzByteStr;
    }
    return 0;
}

long CCryptoUtility::AdjustCryptoSecret(
    CByteBuf& CryptoSecret, 
    const PBYTE pbEncSecret, 
    DWORD dwSecretSize, 
    E_ENCALG_TYPE eEnvAlgType)
{
    DWORD dwExpectedSecretSize = 0;
    switch(eEnvAlgType)
    {
    case EEAT_AES_128BIT:
        dwExpectedSecretSize = 16;
        break;
    case EEAT_AES_192BIT:
        dwExpectedSecretSize = 24;
        break;
    case EEAT_AES_256BIT:
        dwExpectedSecretSize = 32;
        break;
    case EEAT_3DES:
        dwExpectedSecretSize = 24;
        break;
    }
    CryptoSecret.AllocMem(dwExpectedSecretSize);
    // Use PKCS#5 to padding hash value for password.
    long dwPaddingCnt = dwExpectedSecretSize - dwSecretSize;
    if(dwPaddingCnt > 0)
    {
        memcpy(CryptoSecret.m_pByteBuf, pbEncSecret, dwSecretSize);
        memset(CryptoSecret.m_pByteBuf + dwSecretSize, dwPaddingCnt, dwPaddingCnt);
    }
    else
        memcpy(CryptoSecret.m_pByteBuf, pbEncSecret, dwExpectedSecretSize);
        
    return 0;
}

const WCHAR* CCryptoUtility::FormatEncLibType(E_ENCLIB_TYPE eEncLibType)
{
    switch(eEncLibType)
    {
    case ELT_NOENCRYTION:
        return GEN_MAKE_W(GEN_STRINGIZE(ELT_NOENCRYTION));
    case ELT_MSCRYPTO:
        return GEN_MAKE_W(GEN_STRINGIZE(ELT_MSCRYPTO));
    case ELT_ETPKI:
        return GEN_MAKE_W(GEN_STRINGIZE(ELT_ETPKI));
    }
    return L"Unknown";
}

const WCHAR* CCryptoUtility::FormatEncAlgType(E_ENCALG_TYPE eEncAlgType)
{
    switch(eEncAlgType)
    {
    case EEAT_AES_128BIT:
        return GEN_MAKE_W(GEN_STRINGIZE(EEAT_AES_128BIT));
    case EEAT_AES_192BIT:
        return GEN_MAKE_W(GEN_STRINGIZE(EEAT_AES_192BIT));
    case EEAT_AES_256BIT:
        return GEN_MAKE_W(GEN_STRINGIZE(EEAT_AES_256BIT));
    case EEAT_3DES:
        return GEN_MAKE_W(GEN_STRINGIZE(EEAT_3DES));
    }
    return L"Unknown";
}

const WCHAR* CCryptoUtility::FormatHashAlgType(E_HASHALG_TYPE eHashAlgType)
{
    switch(eHashAlgType)
    {
    case EHAT_MD5:
        return GEN_MAKE_W(GEN_STRINGIZE(EHAT_MD5));
    case EHAT_SHA1:
        return GEN_MAKE_W(GEN_STRINGIZE(EHAT_SHA1));
    case EHAT_SHA_256:
        return GEN_MAKE_W(GEN_STRINGIZE(EHAT_SHA_256));
    case EHAT_SHA_384:
        return GEN_MAKE_W(GEN_STRINGIZE(EHAT_SHA_384));
    case EHAT_SHA_512:
        return GEN_MAKE_W(GEN_STRINGIZE(EHAT_SHA_512));
    }
    return L"Unknown";
}

const WCHAR* CCryptoUtility::FormatCryptoAPIType(E_CRYPTOAPI_TYPE eEncAPIType)
{
    switch(eEncAPIType)
    {
    case ECAT_BY_OS:
        return GEN_MAKE_W(GEN_STRINGIZE(ECAT_BY_OS));
    case ECAT_CAPI:
        return GEN_MAKE_W(GEN_STRINGIZE(ECAT_CAPI));
    case ECAT_CNG:
        return GEN_MAKE_W(GEN_STRINGIZE(ECAT_CNG));
    }
    return L"Unknown";
}

const WCHAR* CCryptoUtility::FormatInterfaceType(E_INTER_TYPE eInterType)
{
    switch(eInterType)
    {
    case EIT_CRYPTO:
        return GEN_MAKE_W(GEN_STRINGIZE(EIT_CRYPTO));
    case EIT_HASH:
        return GEN_MAKE_W(GEN_STRINGIZE(EIT_HASH));
    case EIT_CRYPRO_HASH:
        return GEN_MAKE_W(GEN_STRINGIZE(EIT_CRYPRO_HASH));
    }
    return L"Unknown";
}

const WCHAR* CCryptoUtility::FormatIVUsageStatus(E_IV_USAGE eIVUsage)
{
    switch(eIVUsage)
    {
    case EIU_NOT_USE:
        return GEN_MAKE_W(GEN_STRINGIZE(EIU_NOT_USE));
    case EIU_USE_IV:
        return GEN_MAKE_W(GEN_STRINGIZE(EIU_USE_IV));
    }
    return L"Unknown";
}

long CCryptoUtility::VerifyCryptoParam(CCryptoParam& CryptoParam)
{
    long lRetCode = D2DCRYPTO_S_OK;
    switch(CryptoParam.m_dwEncLibType)
    {
    case ELT_MSCRYPTO:
    //case ELT_ETPKI:
        break;
    default:
        lRetCode = D2DCRYPTO_E_ENCLIB_NOT_SUPPORTED;
        ERRLOGEX(lRetCode, L"Encryption library is not supported. (Input=[%u])", CryptoParam.m_dwEncLibType);
        return lRetCode;
    }

    switch(CryptoParam.m_dwEncAPIType)
    {
    case ECAT_CNG:
        {
            if(!CCryptoUtility::IsCNGAble2Used())
            {
                lRetCode = D2DCRYPTO_E_ENCAPI_NOT_SUPPORTED;
                break;
            }
        }
    case ECAT_CAPI:
    case ECAT_BY_OS:
        break;
    default:
        lRetCode = D2DCRYPTO_E_ENCAPI_NOT_SUPPORTED;
        ERRLOGEX(lRetCode, L"Encryption API is not supported. (Input=[%u])", CryptoParam.m_dwEncAPIType);
    }

    switch(CryptoParam.m_dwEncAlgType)
    {
    case EEAT_AES_128BIT:
    case EEAT_AES_192BIT:
    case EEAT_AES_256BIT:
    //case EEAT_3DES:
        break;
    default:
        lRetCode = D2DCRYPTO_E_ENCALG_NOT_SUPPORTED;
        ERRLOGEX(lRetCode, L"Encryption algorithm is not supported. (Input=[%u])", CryptoParam.m_dwEncAlgType);
    }

    //ZZ: We will support SHA hash algorithm
#if 0
    switch(CryptoParam.m_dwHashAlgType)
    {
    case EHAT_MD5:
    //case EHAT_SHA1:
        break;
    default:
        lRetCode = D2DCRYPTO_E_HASHALG_NOT_SUPPORTED;
        ERRLOGEX(lRetCode, L"Hash algorithm is not supported. (Input=[%u])", CryptoParam.m_dwHashAlgType);
    }
#endif

    return lRetCode;
}

//ZZ: Windows 7 6.1 
//ZZ: Windows Server 2008 R2 6.1 
//ZZ: Windows Server 2008 6.0 
//ZZ: Windows Vista 6.0 
//ZZ: Windows Server 2003 R2 5.2 
//ZZ: Windows Server 2003 5.2 
//ZZ: Windows XP 64-Bit Edition 5.2 
//ZZ: Windows XP 5.1 
//ZZ: Windows 2000 5.0 

bool CCryptoUtility::IsCNGAble2Used(POSVERSIONINFOW pOSVersion /* = NULL */)
{
    OSVERSIONINFOW OSVersion = {0};
    OSVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
    GetVersionExW(&OSVersion);
    if(pOSVersion)
        memcpy(pOSVersion, &OSVersion, sizeof(OSVERSIONINFOW));

    DETLOG(L"OS=[%u.%u.%u]", OSVersion.dwMajorVersion, OSVersion.dwMinorVersion, OSVersion.dwBuildNumber);

    if(OSVersion.dwMajorVersion < 6)
    {
        DETLOG(L"CNG API is not supported, OS is earlier than vista. (OS=[%u.%u.%u])", OSVersion.dwMajorVersion, OSVersion.dwMinorVersion, OSVersion.dwBuildNumber);
        return false;
    }
    else if((6 == OSVersion.dwMajorVersion) && (0 == OSVersion.dwMinorVersion)) //ZZ: Vista or W2k8.
    {
        DETLOG(L"CNG API is not supported for interoperability with CAPI, OS is earlier than win7 and 2k8r2. (OS=[%u.%u.%u])", OSVersion.dwMajorVersion, OSVersion.dwMinorVersion, OSVersion.dwBuildNumber);
        return false;
    }
    INFLOG(L"CNG API can be used for encryption and interoperability with CAPI.");
    return true;
}

long CCryptoUtility::GenerateCryptoParam(
    CCryptoParam& CryptoInfo, 
    CCryptoParam* pCryptoInfo /* = NULL */, 
    E_INTER_TYPE eInterType /* = EIT_CRYPRO_HASH */, 
    E_CRYPTOAPI_TYPE eAPIType /* = ECAT_BY_OS */)
{
    if (pCryptoInfo)
        CryptoInfo = *pCryptoInfo;
    else
    {
        CryptoInfo.m_dwInterfaceType = eInterType;
        CryptoInfo.m_dwEncAPIType = eAPIType;
    }

    CCryptoConfig CryptoCfgMgr;
    CCryptoUtility::UpdateCryptoParam(CryptoInfo, CryptoCfgMgr);
    long lRetCode = CCryptoUtility::VerifyCryptoParam(CryptoInfo);
    if (0 != lRetCode)
        ERRTRCEX(lRetCode, L"Inavalid crypto param, at least one input parameters are not support. (EC=[0x%08x])", lRetCode);

    return lRetCode;
}

long
CCryptoUtility::XMLErrLogger(CXMLError& XMLErr)
{
    ERRTRCEX(XMLErr.m_lErrCode, L"XML process error. (EC=[%#08x], Ln=[%u], LnPos=[%u], FlPos=[%u], RS=[%s], FL=[%s], Txt=[%s])",
        XMLErr.m_lErrCode, XMLErr.m_lErrLine, XMLErr.m_lLinePos, XMLErr.m_lFilePos, 
        WS_S(XMLErr.m_wsErrUrl), WS_S(XMLErr.m_wsErrReason), WS_S(XMLErr.m_wsSrcText));
    return XMLErr.m_lErrCode;
}

inline bool CCryptoUtility::IsBigEndian()
{
    unsigned short usTestNum = 0x1122;
    return (0x11 == *((unsigned char*)&usTestNum));
}

inline long CCryptoUtility::ReverseBytes(PBYTE pbByteBuf, DWORD dwBufSize)
{
    BYTE bTemp = 0;
    for(UINT uiIdx = 0; uiIdx < dwBufSize / 2; uiIdx++)
    {
        bTemp = *(pbByteBuf + uiIdx);
        *(pbByteBuf + uiIdx) = *(pbByteBuf + dwBufSize - 1 - uiIdx);
        *(pbByteBuf + dwBufSize - 1 - uiIdx) = bTemp;
    }
    return 0;
}

// Copy memory in little endian.
inline void CCryptoUtility::MemcpyEx(void* pDst, void* pSrc, unsigned long ulMaxLen, bool bBigEndianAgreed)
{
    bool bIsBigEndian = CCryptoUtility::IsBigEndian();
    if((bBigEndianAgreed && bIsBigEndian) || (!bBigEndianAgreed && !bIsBigEndian))
        memcpy(pDst, pSrc, ulMaxLen);
    else
    {
        PBYTE pbTmpBuf = new BYTE[ulMaxLen];
        memcpy(pbTmpBuf, pSrc, ulMaxLen);
        ReverseBytes(pbTmpBuf, ulMaxLen);
        memcpy(pDst, pbTmpBuf, ulMaxLen);
        delete []pbTmpBuf;
    }
}

inline DWORD CCryptoUtility::MergeDWORD(
    DWORD& usDstVal, 
    DWORD usDstOffSetBit, 
    DWORD usSrcVal, 
    DWORD usSrcBits, 
    DWORD usSrcOffsetBit /* = 0 */, 
    bool bTruncate /* = true */)
{
    DWORD dwMemBytes = 0;
    if((usDstOffSetBit + usSrcBits) > sizeof(DWORD)*8)
    {
        if(!bTruncate)
            return 0;
        dwMemBytes = sizeof(DWORD);
    }
    else
        dwMemBytes = (((usDstOffSetBit + usSrcBits) / 8) + (((usDstOffSetBit + usSrcBits) % 8) ? 1 : 0));

    usDstVal = ((usDstVal&(~(0xFFFFFFFF<<usSrcBits)))|((((usSrcVal>>usSrcOffsetBit)&(~(0xFFFFFFFF<<usSrcBits))))<<usDstOffSetBit));
    return dwMemBytes;
}

inline DWORD CCryptoUtility::RangeDWORD(
    DWORD dwSrcDWVal, 
    USHORT usOffSetBit, 
    USHORT usValBits, 
    bool bTruncate /* = true */)
{
    if((usOffSetBit + usValBits) > sizeof(DWORD)*8)
    {
        if(!bTruncate)
            return 0;
        usValBits = sizeof(DWORD)*8 - usOffSetBit;
    }
    return ((dwSrcDWVal>>usOffSetBit)&(~(0xFFFFFFFF<<usValBits)));
}

HRESULT CCryptoUtility::AnsiToUnicode(LPCSTR pszAnsiStr, std::wstring& wsUnicodeStr)
{

    ULONG cCharacters = 0;;
    DWORD dwError = 0;

    // If input is null then just return the same.
    if (NULL == pszAnsiStr)
    {
        return NOERROR;
    }

    // Determine number of wide characters to be allocated for the
    // Unicode string.
    cCharacters =  (ULONG)strlen(pszAnsiStr) + 1;

    WCHAR* pwzUnicodeStr = new WCHAR[cCharacters * 2];

    // Covert to Unicode.
    if(0 == MultiByteToWideChar(
        CP_ACP, 0, pszAnsiStr, cCharacters, pwzUnicodeStr, cCharacters))
    {
        dwError = GetLastError();
        delete []pwzUnicodeStr;
        return HRESULT_FROM_WIN32(dwError);
    }

    wsUnicodeStr = pwzUnicodeStr;
    delete []pwzUnicodeStr;

    return NOERROR;
}

HRESULT CCryptoUtility::UnicodeToAnsi(LPCWSTR pwzUnicodeStr, std::string& ssAnsiStr)
{

    ULONG cbAnsi = 0;
    ULONG cCharacters = 0;
    DWORD dwError = 0;

    // If input is null then just return the same.
    if (NULL == pwzUnicodeStr)
    {
        return NOERROR;
    }

    cCharacters = (ULONG)wcslen(pwzUnicodeStr) + 1;

    // Determine number of bytes to be allocated for ANSI string. An
    // ANSI string can have at most 2 bytes per character (for Double
    // Byte Character Strings.)
    cbAnsi = cCharacters * 2;

    // Use of the OLE allocator is not required because the resultant
    // ANSI  string will never be passed to another COM component. You
    // can use your own allocator.
    char* pszAnsiStr = new char[cbAnsi];

    // Convert to ANSI.
    if(0 == WideCharToMultiByte(
        CP_ACP, 0, pwzUnicodeStr, cCharacters, pszAnsiStr, cbAnsi, NULL, NULL))
    {
        dwError = GetLastError();
        delete []pszAnsiStr;
        return HRESULT_FROM_WIN32(dwError);
    }
    ssAnsiStr = pszAnsiStr;
    delete []pszAnsiStr;

    return NOERROR;
}


CCryptoAlgConfMgr::CCryptoAlgConfMgr(const WCHAR* pwzScriptPath) : CXMLFileMgr(NULL)
{
    if (pwzScriptPath)
        m_wsScriptPath = pwzScriptPath;
    else
        ERRTRCEX(D2DCOMM_E_INVALID_PARAM, L"Invalid parameter: script path is empty.");
}

long CCryptoAlgConfMgr::ReadCryptoAlgConf(CCryptoAlgorithmConfig& CryptoAlgConf, const WCHAR* pwzScriptPath /* = NULL */)
{
    wstring wsScriptPath;
    if (pwzScriptPath)
        wsScriptPath = pwzScriptPath;
    else if (!m_wsScriptPath.empty())
        wsScriptPath = m_wsScriptPath;
    else
        return D2DCOMM_E_INVALID_PARAM;

    if (wsScriptPath.empty())
    {
        ERRTRCEX(D2DCOMM_E_INVALID_PARAM, L"Unable to read catalog script. Path is empty.");
        return D2DCOMM_E_INVALID_PARAM;
    }

    if (0 != _waccess_s(wsScriptPath.c_str(), 0))
    {
        DETTRCEX(D2DCOMM_E_INVALID_PARAM, L"The script path doesn't exist. (Path=[%s])", WS_S(wsScriptPath));
        return D2DCOMM_E_INVALID_PARAM;
    }

    long lRetCode = CreateXML(wsScriptPath.c_str(), EXMLCM_READ | EXMLCM_FILE);
    if (0 != lRetCode)
    {
        CCryptoUtility::XMLErrLogger(m_LastXMLErr);
        ERRTRCEX(lRetCode, L"Failed to load catalog script. (Path=[%s])", WS_S(wsScriptPath));
        return lRetCode;
    }

    MSXML2::IXMLDOMNodeListPtr pCryptoLibList = m_pXMLDomPtr->selectNodes(L"//Encryption/CryptoLib");
    if(NULL == pCryptoLibList)
    {
        ERRTRCEX(D2DCOMM_E_ITEM_NOT_FOUND, L"Unable to find root tag: '//Encryption/CryptoLib'");
        return D2DCOMM_E_ITEM_NOT_FOUND;
    }
    long lCryptoLibCnt = pCryptoLibList->Getlength();
    for(long lIndex = 0; lIndex < lCryptoLibCnt; lIndex++)
    {
        MSXML2::IXMLDOMNodePtr pCryptoLibPtr = pCryptoLibList->Getitem(lIndex);
        if(NULL == pCryptoLibPtr)
        {
            continue;
        }

        CCryptoLib CryptoLib;

        long lRetCode = ReadNodeAttribute(pCryptoLibPtr, L"name", CryptoLib.m_CryptoInfo.m_wsName);
        lRetCode = ReadNodeAttribute(pCryptoLibPtr, L"type", CryptoLib.m_CryptoInfo.m_dwType);
        lRetCode = ReadNodeAttribute(pCryptoLibPtr, L"resID", CryptoLib.m_CryptoInfo.m_dwResID);
        lRetCode = ReadNodeAttribute(pCryptoLibPtr, L"default", CryptoLib.m_CryptoInfo.m_bDefault); 
        if(CryptoLib.m_CryptoInfo.m_bDefault)
            CryptoAlgConf.m_CryptoParam.m_dwEncLibType = CryptoLib.m_CryptoInfo.m_dwType;

        MSXML2::IXMLDOMNodeListPtr pEncAlgList = pCryptoLibPtr->selectNodes(L"EncAlg/Algorithm");
        if(NULL == pEncAlgList)
        {
            continue;
        }
        long lEncAlgCnt = pEncAlgList->Getlength();
        for(long lIdxEncAlg = 0; lIdxEncAlg < lEncAlgCnt; lIdxEncAlg++)
        {
            CCryptoInfo CryptoInfo;
            MSXML2::IXMLDOMNodePtr pEncAlgPtr = pEncAlgList->Getitem(lIdxEncAlg);
            if(NULL == pEncAlgPtr)
            {
                continue;
            }
            long lRetCode = ReadNodeAttribute(pEncAlgPtr, L"name", CryptoInfo.m_wsName);
            lRetCode = ReadNodeAttribute(pEncAlgPtr, L"type", CryptoInfo.m_dwType);
            lRetCode = ReadNodeAttribute(pEncAlgPtr, L"resID", CryptoInfo.m_dwResID);
            lRetCode = ReadNodeAttribute(pEncAlgPtr, L"default", CryptoInfo.m_bDefault); 
            if( CryptoInfo.m_bDefault)
                CryptoAlgConf.m_CryptoParam.m_dwEncAlgType = CryptoInfo.m_dwType;
            CryptoLib.m_vecEncAlg.push_back(CryptoInfo);
        }

        MSXML2::IXMLDOMNodeListPtr pHashAlgList = pCryptoLibPtr->selectNodes(L"HashAlg/Algorithm");
        if(NULL == pHashAlgList)
        {
            continue;
        }
        long lHashAlgCnt = pHashAlgList->Getlength();
        for(long lIdxHashAlg = 0; lIdxHashAlg < lHashAlgCnt; lIdxHashAlg++)
        {
            CCryptoInfo CryptoInfo;
            MSXML2::IXMLDOMNodePtr pHashAlgPtr = pHashAlgList->Getitem(lIdxHashAlg);
            if(NULL == pHashAlgPtr)
            {
                continue;
            }
            long lRetCode = ReadNodeAttribute(pHashAlgPtr, L"name", CryptoInfo.m_wsName);
            lRetCode = ReadNodeAttribute(pHashAlgPtr, L"type", CryptoInfo.m_dwType);
            lRetCode = ReadNodeAttribute(pHashAlgPtr, L"resID", CryptoInfo.m_dwResID);
            lRetCode = ReadNodeAttribute(pHashAlgPtr, L"default", CryptoInfo.m_bDefault); 
            if(CryptoInfo.m_bDefault)
                CryptoAlgConf.m_CryptoParam.m_dwHashAlgType = CryptoInfo.m_dwType;
            CryptoLib.m_vecHashAlg.push_back(CryptoInfo);
        }

        MSXML2::IXMLDOMNodeListPtr pCryptoAPIList = pCryptoLibPtr->selectNodes(L"CryptoAPI/APIType");
        if(NULL == pCryptoAPIList)
        {
            continue;
        }
        long lCryptoAPICnt = pCryptoAPIList->Getlength();
        for(long lIdxCryptoAPI = 0; lIdxCryptoAPI < lCryptoAPICnt; lIdxCryptoAPI++)
        {
            CCryptoInfo CryptoInfo;
            MSXML2::IXMLDOMNodePtr pCryptoAPIPtr = pCryptoAPIList->Getitem(lIdxCryptoAPI);
            if(NULL == pCryptoAPIPtr)
            {
                continue;
            }
            long lRetCode = ReadNodeAttribute(pCryptoAPIPtr, L"name", CryptoInfo.m_wsName);
            lRetCode = ReadNodeAttribute(pCryptoAPIPtr, L"type", CryptoInfo.m_dwType);
            lRetCode = ReadNodeAttribute(pCryptoAPIPtr, L"resID", CryptoInfo.m_dwResID);
            lRetCode = ReadNodeAttribute(pCryptoAPIPtr, L"default", CryptoInfo.m_bDefault); 
            if(CryptoInfo.m_bDefault)
                CryptoAlgConf.m_CryptoParam.m_dwEncAPIType = CryptoInfo.m_dwType;
            CryptoLib.m_vecCryptoAPI.push_back(CryptoInfo);
        }
        CryptoAlgConf.m_vecCryptoLib.push_back(CryptoLib);
    }
    return 0;
}

void CCryptoAlgConfMgr::LogFunc(DWORD dwLoglevel, DWORD dwRetCode, const WCHAR* pwzLogFormat, ...)
{
    va_list arg_ptr;
    va_start(arg_ptr, pwzLogFormat);
	static CONST INT LOG_BUFFER_LEN = 1024;
    WCHAR wzMsgBuf[LOG_BUFFER_LEN] = {0};
    _vsnwprintf_s(wzMsgBuf, _countof(wzMsgBuf), _countof(wzMsgBuf) - 1, pwzLogFormat, arg_ptr);
    va_end(arg_ptr);
    FUNCLOGGEREX(dwLoglevel, dwRetCode, wzMsgBuf);
}

long CCryptoAlgorithmConfig::ReFreshConfig(const WCHAR* pwzAlgCfgPath)
{
    if(NULL == pwzAlgCfgPath)
        return E_INVALIDARG;

    long lRetCode = 0;
    //ZZ: Currently there is no Encryption.xml under configuration(Algorithm is hard code in web service), thus I comment this.
#if 0
    CCryptoAlgConfMgr CryptoConfMgr(pwzAlgCfgPath);
    lRetCode = CryptoConfMgr.ReadCryptoAlgConf(*this);
    if (0 != lRetCode)
        DETTRCEX(lRetCode, L"Failed to refresh crypto configuration. (Path=[%s])", WZ_S(pwzAlgCfgPath));
#endif

    return lRetCode;
}

long CCryptoDebugConfig::ReFreshConfig(const WCHAR* pwzAlgCfgPath)
{
    if(NULL == pwzAlgCfgPath)
        return E_INVALIDARG;

    WCHAR wzConfigStr[MAX_PATH] = {0};
    //ZZ: CryptoParam.
    GetPrivateProfileStringW(L"CryptoParam", L"dwEncLibType", L"", wzConfigStr, _countof(wzConfigStr), pwzAlgCfgPath);
    m_CryptoParam.m_dwEncLibType = wcstoul(wzConfigStr, NULL, 10);
    GetPrivateProfileStringW(L"CryptoParam", L"dwHashAlgType", L"", wzConfigStr, _countof(wzConfigStr), pwzAlgCfgPath);
    m_CryptoParam.m_dwHashAlgType = wcstoul(wzConfigStr, NULL, 10);
    GetPrivateProfileStringW(L"CryptoParam", L"dwEncAPIType", L"", wzConfigStr, _countof(wzConfigStr), pwzAlgCfgPath);
    m_CryptoParam.m_dwEncAPIType = wcstoul(wzConfigStr, NULL, 10);    
    GetPrivateProfileStringW(L"CryptoParam", L"dwEncAlgType", L"", wzConfigStr, _countof(wzConfigStr), pwzAlgCfgPath);
    m_CryptoParam.m_dwEncAlgType = wcstoul(wzConfigStr, NULL, 10);
    GetPrivateProfileStringW(L"CryptoParam", L"dwIVUsage", L"", wzConfigStr, _countof(wzConfigStr), pwzAlgCfgPath);
    m_CryptoParam.m_dwIVUsage = wcstoul(wzConfigStr, NULL, 10);       
    GetPrivateProfileStringW(L"CryptoParam", L"wsPassword", L"", wzConfigStr, _countof(wzConfigStr), pwzAlgCfgPath);
    m_CryptoParam.m_wsPassword = wzConfigStr;

    //ZZ: ControlParam.
    GetPrivateProfileStringW(L"ControlParam", L"bDumpPwd", L"", wzConfigStr, _countof(wzConfigStr), pwzAlgCfgPath);
    m_bDumpPwd = wcstoul(wzConfigStr, NULL, 10) ? true : false;

    GetPrivateProfileStringW(L"ControlParam", L"bKeepContainer", L"", wzConfigStr, _countof(wzConfigStr), pwzAlgCfgPath);
    m_bKeepContainer = wcstoul(wzConfigStr, NULL, 10) ? true : false;

    GetPrivateProfileStringW(L"ControlParam", L"KeySetRetryTImes", L"", wzConfigStr, _countof(wzConfigStr), pwzAlgCfgPath);
    m_dwKeySetRetryTimes = wcstoul(wzConfigStr, NULL, 10);
    if (0 == m_dwKeySetRetryTimes)
        m_dwKeySetRetryTimes = MAX_RETRY_TIMES_FOR_KEY_SET;

    return 0;
}

//CDbgLog CCryptoConfig::g_Logger(L"CryptoWrapperDll");

CCryptoConfig::CCryptoConfig(const WCHAR* pwzAlgCfgPath /* = NULL */)
{
    GetD2DEncAlgCfgPath();

    ReadCryptoDefaultConfig();
    ReadCryptoDebugConfig();
}

long CCryptoConfig::ReadCryptoDefaultConfig()
{
    long lRetCode = m_CryptoAlgConfig.ReFreshConfig(m_wsDefCfgFilePath.c_str());
    return lRetCode;
}

long CCryptoConfig::ReadCryptoDebugConfig()
{
    long lRetCode = m_CryptoDBGCfg.ReFreshConfig(m_wsDBGCfgFilePath.c_str());
    return lRetCode;
}

long CCryptoConfig::GetD2DEncAlgCfgPath()
{
    HKEY hKey = NULL;
    bool bFoundD2DReg = false;
    long lRetCode = ERROR_SUCCESS;
    WCHAR wzRegPath[MAX_PATH] = {0};
    do 
    {
        lRetCode = RegOpenKeyEx(
            HKEY_LOCAL_MACHINE, 
            gwzD2DInstallPathRegistryTableKey, 
            NULL, 
            KEY_ALL_ACCESS, 
            &hKey);
        if(lRetCode != ERROR_SUCCESS)
        {
            bFoundD2DReg = false;
            break;
        }
        
        DWORD dwDataSize = sizeof(wzRegPath);
        lRetCode = RegQueryValueEx(hKey, L"Path", NULL, NULL, (PBYTE)wzRegPath, &dwDataSize);
        if(lRetCode != ERROR_SUCCESS)
        {
            bFoundD2DReg = false;
            break;
        }
        bFoundD2DReg = true;
    }while (0);
    if(hKey)
        RegCloseKey(hKey);
    if(bFoundD2DReg)
    {
        m_wsDefCfgFilePath = CCryptoUtility::AppendBackslash(wzRegPath) + gwzD2DConfigurationFolderName;
        m_wsDefCfgFilePath = CCryptoUtility::AppendBackslash(m_wsDefCfgFilePath) + gwzEncAlgDEFConfigFileName;

        m_wsDBGCfgFilePath = CCryptoUtility::AppendBackslash(wzRegPath) + gwzD2DConfigurationFolderName;
        DETLOG(L"Current configuration file for CryptoWrapperDll is located at %s.", m_wsDBGCfgFilePath.c_str());

        m_wsDBGCfgFilePath = CCryptoUtility::AppendBackslash(m_wsDBGCfgFilePath) + gwzEncAlgDBGConfigFileName;
    }
    else
    {
        HMODULE hDllModule = NULL;
        wstring wsCurFoder = L".\\";
        if(GetModuleHandleEx(0, gwzCryptoWrapperDllName, &hDllModule))
        {
            if(hDllModule)
            {
                WCHAR wzDllPath[MAX_PATH] = {0};
                GetModuleFileNameW(hDllModule, wzDllPath, _countof(wzDllPath));
                FreeLibrary(hDllModule);
                wsCurFoder = wzDllPath;
                wstring::size_type nPos = wsCurFoder.rfind(L'\\');
                if(wstring::npos != nPos)
                    wsCurFoder = wsCurFoder.substr(0, nPos);
                wsCurFoder = CCryptoUtility::AppendBackslash(wsCurFoder);
            }
        }

        DETLOG(L"Current configuration file for CryptoWrapperDll is located at %s.", wsCurFoder.c_str());

        m_wsDefCfgFilePath = (wsCurFoder + gwzEncAlgDEFConfigFileName);
        m_wsDBGCfgFilePath = (wsCurFoder + gwzEncAlgDBGConfigFileName);
    }

    return 0;
}