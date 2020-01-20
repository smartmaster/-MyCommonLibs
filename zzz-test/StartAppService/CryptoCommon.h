#pragma once

#include "ICryptoWrapperInterface.h"
#include "CXMLFileMgr.h"
#include "CNGAPIWrapper.h"
#include "DRCommonlib.h"
#include "DebugLog.h"
#include "D2DCryptoError.h"
#include <vector>
#include "brandname.h"

using namespace std;

#define VSSERRLOG   LL_ERR
#define VSSWRNLOG   LL_WAR
#define VSSINFLOG   LL_INF
#define VSSDETLOG   LL_DET
#define VSSDBGLOG   LL_DBG

#ifdef __MYCRYPTO_DENUG_LOGS__
#define FUNCLOGGER(Level, ...) DebugWriteStringV(__VA_ARGS__)
#define FUNCLOGGEREX(Level, ErrCode, ...) DebugWriteStringV(__VA_ARGS__)
#else
#define FUNCLOGGER(Level, ...)				(0)
#define FUNCLOGGEREX(Level, ErrCode, ...)	(0)
#endif


#define ERRLOG(...) FUNCLOGGER(VSSERRLOG, _T("[ERR] ")##__VA_ARGS__)
#define WARLOG(...) FUNCLOGGER(VSSWRNLOG, _T("[WAR] ")##__VA_ARGS__)
#define INFLOG(...) FUNCLOGGER(VSSINFLOG, _T("[INF] ")##__VA_ARGS__)
#define DETLOG(...) FUNCLOGGER(VSSDETLOG, _T("[DET] ")##__VA_ARGS__)
#define DBGLOG(...) FUNCLOGGER(VSSDBGLOG, _T("[DBG] ")##__VA_ARGS__)

#define ERRTRC(...) FUNCLOGGER(VSSERRLOG, _T("[ERR] [")##_T(__FUNCTION__)##_T("] ")##__VA_ARGS__)
#define WARTRC(...) FUNCLOGGER(VSSWRNLOG, _T("[WAR] [")##_T(__FUNCTION__)##_T("] ")##__VA_ARGS__)
#define INFTRC(...) FUNCLOGGER(VSSINFLOG, _T("[INF] [")##_T(__FUNCTION__)##_T("] ")##__VA_ARGS__)
#define DETTRC(...) FUNCLOGGER(VSSDETLOG, _T("[DET] [")##_T(__FUNCTION__)##_T("] ")##__VA_ARGS__)
#define DBGTRC(...) FUNCLOGGER(VSSDBGLOG, _T("[DBG] [")##_T(__FUNCTION__)##_T("] ")##__VA_ARGS__)

#define ERRLOGEX(ERRCODE, ...) FUNCLOGGEREX(VSSERRLOG, (ERRCODE), _T("[ERR] ")##__VA_ARGS__)
#define WARLOGEX(ERRCODE, ...) FUNCLOGGEREX(VSSWRNLOG, (ERRCODE), _T("[WAR] ")##__VA_ARGS__)
#define INFLOGEX(ERRCODE, ...) FUNCLOGGEREX(VSSINFLOG, (ERRCODE), _T("[INF] ")##__VA_ARGS__)
#define DETLOGEX(ERRCODE, ...) FUNCLOGGEREX(VSSDETLOG, (ERRCODE), _T("[DET] ")##__VA_ARGS__)
#define DBGLOGEX(ERRCODE, ...) FUNCLOGGEREX(VSSDBGLOG, (ERRCODE), _T("[DBG] ")##__VA_ARGS__)

#define ERRTRCEX(ERRCODE, ...) FUNCLOGGEREX(VSSERRLOG, (ERRCODE), _T("[ERR] [")##_T(__FUNCTION__)##_T("] ")##__VA_ARGS__)
#define WARTRCEX(ERRCODE, ...) FUNCLOGGEREX(VSSWRNLOG, (ERRCODE), _T("[WAR] [")##_T(__FUNCTION__)##_T("] ")##__VA_ARGS__)
#define INFTRCEX(ERRCODE, ...) FUNCLOGGEREX(VSSINFLOG, (ERRCODE), _T("[INF] [")##_T(__FUNCTION__)##_T("] ")##__VA_ARGS__)
#define DETTRCEX(ERRCODE, ...) FUNCLOGGEREX(VSSDETLOG, (ERRCODE), _T("[DET] [")##_T(__FUNCTION__)##_T("] ")##__VA_ARGS__)
#define DBGTRCEX(ERRCODE, ...) FUNCLOGGEREX(VSSDBGLOG, (ERRCODE), _T("[DBG] [")##_T(__FUNCTION__)##_T("] ")##__VA_ARGS__)

#define DEBUG_INFO(APIName)  __WFUNCTION__, GEN_MAKE_W(APIName), __WFILE__, __LINE__

#define CRYPTO_RETCHK(Ret) \
{\
    if(!(Ret)) { return ErrorCodeHandler(GetLastError(), DEBUG_INFO(GEN_STRINGIZE_ARG(Ret))); }\
}

#define BCRYPT_RETCHK(Ret) \
{\
    long lRetCode = (Ret);\
    if(lRetCode < 0) { return ErrorCodeHandler(lRetCode, DEBUG_INFO(GEN_STRINGIZE_ARG(Ret))); }\
}

#define MAX_RETRY_TIMES_FOR_KEY_SET    2

const WCHAR gwzMS_ENH_RSA_AES_PROV_XP_W[] = L"Microsoft Enhanced RSA and AES Cryptographic Provider (Prototype)";
const WCHAR gwzKeyContainerNameForCAPI[] = L"D2DCAPIKeySet";

const WCHAR gwzD2DInstallPathRegistryTableKey[] = CST_REG_ROOT_L L"\\InstallPath";
const WCHAR gwzEncAlgDEFConfigFileName[] = L"Encryption.xml";
const WCHAR gwzEncAlgDBGConfigFileName[] = L"CryptoDebug.ini";
const WCHAR gwzD2DConfigurationFolderName[] = L"Configuration";
const WCHAR gwzD2DBinaryFolderName[] = L"BIN";
const WCHAR gwzCryptoWrapperDllName[] = L"CryptoWrapperDll.dll";

static const BYTE g_CNG256IV[] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
};

//ZZ: Helper macros to print a GUID using printf-style formatting
#define WSTR_GUID_FMT  L"{%.8x-%.4x-%.4x-%.2x%.2x-%.2x%.2x%.2x%.2x%.2x%.2x}"
#define GUID_PRINTF_ARG(X)                                  \
    (X).Data1,                                              \
    (X).Data2,                                              \
    (X).Data3,                                              \
    (X).Data4[0], (X).Data4[1], (X).Data4[2], (X).Data4[3], \
    (X).Data4[4], (X).Data4[5], (X).Data4[6], (X).Data4[7]

#define GUID_NULL_STR               L"{00000000-0000-0000-0000-000000000000}"
#define GUID_STR_LEN                38

inline const WCHAR* WS_S(const wstring& wsInStr) { return (wsInStr.empty() ? L"nul" : wsInStr.c_str()); }
inline const WCHAR* WZ_S(const WCHAR* pwzInStr) { return (pwzInStr ? pwzInStr : L"nul"); }

typedef std::vector<std::wstring> WStrVector;

class CCryptoConfig;
class CCryptoUtility
{
public:
    static DWORD        GetCSPType(E_ENCALG_TYPE eEnvAlgType);         // May not unique.
    static const WCHAR* GetCSPName(E_ENCALG_TYPE eEnvAlgType);   // Unique.
    static const WCHAR* GetEncAlgorithmName(E_ENCALG_TYPE eEnvAlgType);
    static DWORD        GetEncAlgorithmID(E_ENCALG_TYPE eEnvAlgType);
    static DWORD        GetHashAlgType(E_HASHALG_TYPE eHashAlgType);
    static const WCHAR* GetHashAlgCSPName(E_HASHALG_TYPE eHashAlgType);
    static const WCHAR* GetKeyContainerName();
    static wstring      GetRandomKeyContainerName(const WCHAR* pwzGUID = NULL);
    static wstring      GetRandomGUID(GUID* pGUID = NULL);
    static void         UpdateCryptoParam(CCryptoParam& CryptoParam, CCryptoConfig& CryptoCfg);
    static wstring      AppendBackslash(wstring str);
    static wstring      RemoveBackslash(wstring str);
    // By default, byte with low address in pBinBuf will be output in  
    static long         Bin2HexString(wstring& wsHexStr, const PBYTE pBinBuf, DWORD dwBinBufSize);
    static long         AdjustCryptoSecret(CByteBuf&     CryptoSecret,
                                           const PBYTE   pbEncSecret, 
                                           DWORD         dwSecretSize,
                                           E_ENCALG_TYPE eEnvAlgType);

public:
    static const WCHAR* FormatEncLibType(E_ENCLIB_TYPE eEncLibType);
    static const WCHAR* FormatEncAlgType(E_ENCALG_TYPE eEncAlgType);
    static const WCHAR* FormatHashAlgType(E_HASHALG_TYPE eHashAlgType);
    static const WCHAR* FormatCryptoAPIType(E_CRYPTOAPI_TYPE eEncAPIType);
    static const WCHAR* FormatInterfaceType(E_INTER_TYPE eInterType);
    static const WCHAR* FormatIVUsageStatus(E_IV_USAGE eIVUsage);
    static long         VerifyCryptoParam(CCryptoParam& CryptoParam);
    static bool         IsCNGAble2Used(POSVERSIONINFOW pOSVersion = NULL);
    static long         GenerateCryptoParam(CCryptoParam& CryptoInfo,
                                            CCryptoParam* pCryptoInfo = NULL, 
                                            E_INTER_TYPE  eInterType = EIT_CRYPRO_HASH,
                                            E_CRYPTOAPI_TYPE eAPIType = ECAT_BY_OS);
    static long         XMLErrLogger(CXMLError& XMLErr);

public:
    static bool IsBigEndian();
    static long ReverseBytes(PBYTE pbByteBuf, DWORD dwBufSize);
    // Copy memory in little endian.
    static void MemcpyEx(void* pDst, void* pSrc, unsigned long ulMaxLen, bool bBigEndianAgreed);

    static DWORD MergeDWORD(
        DWORD& usDstVal,
        DWORD usDstOffSetBit,
        DWORD usSrcVal, 
        DWORD usSrcBits, 
        DWORD usSrcOffsetBit = 0,
        bool  bTruncate = true);

   static DWORD RangeDWORD(
       DWORD dwSrcDWVal, 
       USHORT usOffSetBit, 
       USHORT usValBits, 
       bool bTruncate = true);

   static HRESULT AnsiToUnicode(LPCSTR pszAnsiStr, std::wstring& wsUnicodeStr);

   static HRESULT UnicodeToAnsi(LPCWSTR pwzUnicodeStr, std::string& ssAnsiStr);
};

typedef enum _eCAPIHandleType
{
    EHT_KEY = 0,
    EHT_HASH,
    EHT_PROV
}E_CAPI_HANDLE_TYPE;

template<typename HandleType, E_CAPI_HANDLE_TYPE eHandleType>
class CCAPIHandleWrap
{
public:
    CCAPIHandleWrap<HandleType, eHandleType>() : m_hCryptoHandle(NULL) {}
    ~CCAPIHandleWrap<HandleType, eHandleType>()
    {
        if(m_hCryptoHandle)
        {
            switch(eHandleType)
            {
            case EHT_KEY:
                CryptDestroyKey(m_hCryptoHandle);
                break;
            case EHT_HASH:
                CryptDestroyHash(m_hCryptoHandle);
                break;
            case EHT_PROV:
                CryptReleaseContext(m_hCryptoHandle, 0);
                break;
            }
        }
    }
    operator HandleType() { return m_hCryptoHandle; }
    operator HandleType*() { return &m_hCryptoHandle; }
    bool IsNULL() { return (NULL == m_hCryptoHandle); }

private:
    HandleType m_hCryptoHandle;
};

template<typename HandleType, E_CAPI_HANDLE_TYPE eHandleType>
class CCNGHandleWrap
{
public:
    CCNGHandleWrap<HandleType, eHandleType>() : m_hCryptoHandle(NULL) {}
    ~CCNGHandleWrap<HandleType, eHandleType>()
    {
        if(m_hCryptoHandle)
        {
            switch(eHandleType)
            {
            case EHT_KEY:
                nsCryptoCNGWrapper::BCryptDestroyKey(m_hCryptoHandle);
                break;
            case EHT_HASH:
                nsCryptoCNGWrapper::BCryptDestroyHash(m_hCryptoHandle);
                break;
            case EHT_PROV:
                nsCryptoCNGWrapper::BCryptCloseAlgorithmProvider(m_hCryptoHandle, 0);
                break;
            }
            m_pbObject.AllocMem(0);
        }
    }
    operator HandleType() { return m_hCryptoHandle; }
    operator HandleType*() { return &m_hCryptoHandle; }
    CByteBuf& HandleObj() { return m_pbObject; }
    bool IsNULL() { return (NULL == m_hCryptoHandle); }
    bool IsEmpty() { return ((NULL == m_pbObject.m_pByteBuf) || (0 == m_pbObject.m_dwBufSize)); }

private:
    HandleType m_hCryptoHandle;
    CByteBuf   m_pbObject;
};

template<typename CNGStructurePtr>
class CNGStructurePtrWrap
{
public:
    CNGStructurePtrWrap<CNGStructurePtr>() : m_pCNGSTPtr(NULL) {}
    ~CNGStructurePtrWrap<CNGStructurePtr>()
    {
        if(m_pCNGSTPtr)
        {
            nsCryptoCNGWrapper::BCryptFreeBuffer((PVOID)m_pCNGSTPtr);
            m_pCNGSTPtr = NULL;
        }
    }
    operator CNGStructurePtr*() { return m_pCNGSTPtr; }
    operator CNGStructurePtr**() { return &m_pCNGSTPtr; }
private:
    CNGStructurePtr* m_pCNGSTPtr;
};

class CCryptoInfo
{
public:
    DWORD   m_dwType;
    DWORD   m_dwResID;
    bool    m_bDefault;
    wstring m_wsName;
};

typedef std::vector<CCryptoInfo> CryptoInfoVector;

class CCryptoLib
{
public:
    CryptoInfoVector m_vecEncAlg;
    CryptoInfoVector m_vecHashAlg;
    CryptoInfoVector m_vecCryptoAPI;
    CCryptoInfo      m_CryptoInfo;
};
typedef std::vector<CCryptoLib> CryptoLibVector;
class CCryptoAlgorithmConfig
{
public:
    long ReFreshConfig(const WCHAR* pwzAlgCfgPath);

public:
    CryptoLibVector m_vecCryptoLib; 
    CCryptoParam    m_CryptoParam;
};

class CCryptoAlgConfMgr : public CXMLFileMgr
{
public:
    CCryptoAlgConfMgr(const WCHAR* pwzScriptPath);

public:
    long ReadCryptoAlgConf(CCryptoAlgorithmConfig& CryptoAlgConf, const WCHAR* pwzScriptPath = NULL);
    void LogFunc(DWORD dwLoglevel, DWORD dwRetCode, const WCHAR* pwzLogFormat, ...);

private:
    wstring m_wsScriptPath;
};

class CCryptoDebugConfig
{
public:
    long ReFreshConfig(const WCHAR* pwzAlgCfgPath);

public:
    CCryptoParam  m_CryptoParam;
    bool          m_bDumpPwd;
    bool          m_bKeepContainer;
    DWORD         m_dwKeySetRetryTimes;
};

class CCryptoConfig
{
public:
    CCryptoConfig(const WCHAR* pwzAlgCfgPath = NULL);

protected:
    long ReadCryptoDefaultConfig();
    long ReadCryptoDebugConfig();

protected:
    long GetD2DEncAlgCfgPath();

private:
    wstring m_wsDefCfgFilePath;
    wstring m_wsDBGCfgFilePath;

public:
    CCryptoAlgorithmConfig m_CryptoAlgConfig;
    CCryptoDebugConfig     m_CryptoDBGCfg;

public:
    //static CDbgLog g_Logger;
};

class CGUIDWrap
{
public:
    CGUIDWrap() { Generate(); }

    void Clear()
    {
        memset(&m_guidGUIDObj, 0, sizeof(GUID));
        memset(&m_wzGUIDStr, 0, _countof(m_wzGUIDStr));
    }

    void Generate()
    {
        Clear();
        wstring wsGUIDStr = CCryptoUtility::GetRandomGUID(&m_guidGUIDObj);
        wcscpy_s(m_wzGUIDStr, _countof(m_wzGUIDStr), wsGUIDStr.c_str());
    }

    GUID& GUIDObject()           { return m_guidGUIDObj; }
    const WCHAR* GUIDStr() const { return m_wzGUIDStr; }

private:
    GUID  m_guidGUIDObj;
    WCHAR m_wzGUIDStr[GUID_STR_LEN + 1]; 
};

class CCOMInitGuard
{
public:
    CCOMInitGuard::CCOMInitGuard(DWORD dwCoInit = COINIT_APARTMENTTHREADED)
        : m_bNeedUnInit(false),
          m_bCOMInited(false)
    { InitCOM(dwCoInit); }

    ~CCOMInitGuard() { UnInitCOM(); }

    void InitCOM(DWORD dwCoInit)
    {
        if (!m_bCOMInited)
        {
            HRESULT hrInit = CoInitializeEx(NULL, dwCoInit);
            if (FAILED(hrInit))
            {
                m_bNeedUnInit = false;
                if (hrInit == RPC_E_CHANGED_MODE)
                {
                    m_bCOMInited = true;
                    WARTRCEX(hrInit, L"COM concurrency model has been changed. (EC=[%#08x])", hrInit);
                }
                else
                    ERRTRCEX(hrInit, L"Failed to initialize COM using CoInitializeEx. (EC=[%#08x])", hrInit);
            }
            else
            {
                m_bNeedUnInit = true;
                m_bCOMInited = true;
            }
        }
    }

    void UnInitCOM()
    {
        if (m_bNeedUnInit && m_bCOMInited)
        {
            CoUninitialize();
            m_bNeedUnInit = false;
            m_bCOMInited = false;
        }
    }
    bool IsCOMInited() const { return m_bCOMInited; }

private:
    bool m_bNeedUnInit;
    bool m_bCOMInited;
};

inline long ErrorCodeHandler(long lRetCode,
                             const WCHAR* pwzFuncName = NULL, 
                             const WCHAR* pwzAPIName = NULL, 
                             const WCHAR* pwzFileName = NULL,
                             long lLineNum = 0)
{
    ERRLOGEX(lRetCode, L"[%s] %s Error. (EC=[0x%08x], File=[%s], LN=[%u])", 
        DisplayStr(pwzFuncName), DisplayStr(pwzAPIName), lRetCode, DisplayStr(pwzFileName), lLineNum);
    return lRetCode;
}
