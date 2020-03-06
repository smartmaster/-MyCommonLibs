#include "stdafx.h"
#include "CryptoCAPIWrapperImp.h"

namespace nsCryptoCAPIWrapper
{
    #pragma comment(lib, "Crypt32.lib")

    long CCryptoCAPIWrapper::g_lInstRefCount = 0;

    CCryptoCAPIWrapper::CCryptoCAPIWrapper(
        const CCryptoParam* pCryptoParam /* = NULL */, 
        CCryptoConfig* pCryptoCfg /* = NULL */)
        : m_dwKeyContainerFlag(CRYPT_MACHINE_KEYSET),
          m_pCryptoCfg(pCryptoCfg),
          m_bReleaseConfigMgr(false),
          m_bKeepContainer(false)
    {
        if(pCryptoParam)
            m_CryptoParam = *pCryptoParam;

        if(NULL == m_pCryptoCfg)
        {
            m_pCryptoCfg = new CCryptoConfig;
            m_bReleaseConfigMgr = true;
            CCryptoUtility::UpdateCryptoParam(m_CryptoParam, *m_pCryptoCfg);
        }

        m_CryptoParam.m_dwEncAPIType = ECAT_CAPI;

        InterlockedIncrement(&CCryptoCAPIWrapper::g_lInstRefCount);
        DBGTRC(L"Current CAPI crypto instance or reference count is %u.", CCryptoCAPIWrapper::g_lInstRefCount);
    }

    CCryptoCAPIWrapper::~CCryptoCAPIWrapper()
    {
        if (!m_wsKeySetName.empty() && !m_bKeepContainer && !m_pCryptoCfg->m_CryptoDBGCfg.m_bKeepContainer)
        {
            long lRetCode = RemoveKeyContainer(m_wsKeySetName.c_str());
            if (0 == lRetCode)
                DETLOG(L"Succeed to remove key container: %s.", WS_S(m_wsKeySetName));
            else
                ERRTRCEX(lRetCode, L"Failed to remove key container: %s. (EC=[%#08x])", WS_S(m_wsKeySetName), lRetCode);
        }
        else
            WARLOG(L"No need remove key container because no container name saved. Maybe not initialze correcly.");

        InterlockedDecrement(&CCryptoCAPIWrapper::g_lInstRefCount);
        DBGTRC(L"Current CAPI crypto instance or reference count is %u.", CCryptoCAPIWrapper::g_lInstRefCount);

        if(m_pCryptoCfg && m_bReleaseConfigMgr)
        {
            delete m_pCryptoCfg;
            m_pCryptoCfg = NULL;
        }
    }

    long CCryptoCAPIWrapper::Initialize(const CCryptoParam* pCryptoParam)
    {
        if(pCryptoParam)
        {
            m_CryptoParam = *pCryptoParam;
            CCryptoUtility::UpdateCryptoParam(m_CryptoParam, *m_pCryptoCfg);
            m_CryptoParam.m_dwEncAPIType = ECAT_CAPI;
        }

        if(m_pCryptoCfg->m_CryptoDBGCfg.m_bDumpPwd)
        {
            INFLOG(L"[CAPI] Interface Information: API{%s}, EncAlg{%s}, EncLib{%s}, HashAlg{%s}, Iter{%s}, IV{%s}, PWD{%s}", 
                CCryptoUtility::FormatCryptoAPIType((E_CRYPTOAPI_TYPE)m_CryptoParam.m_dwEncAPIType),
                CCryptoUtility::FormatEncAlgType((E_ENCALG_TYPE)m_CryptoParam.m_dwEncAlgType),
                CCryptoUtility::FormatEncLibType((E_ENCLIB_TYPE)m_CryptoParam.m_dwEncLibType),
                CCryptoUtility::FormatHashAlgType((E_HASHALG_TYPE)m_CryptoParam.m_dwHashAlgType),
                CCryptoUtility::FormatInterfaceType((E_INTER_TYPE)m_CryptoParam.m_dwInterfaceType),
                CCryptoUtility::FormatIVUsageStatus((E_IV_USAGE)m_CryptoParam.m_dwIVUsage),
                m_CryptoParam.m_wsPassword.c_str());
        }
        else
        {
            DETLOG(L"[CAPI] Interface Information: API{%s}, EncAlg{%s}, EncLib{%s}, HashAlg{%s}, Iter{%s}, IV{%s}", 
                CCryptoUtility::FormatCryptoAPIType((E_CRYPTOAPI_TYPE)m_CryptoParam.m_dwEncAPIType),
                CCryptoUtility::FormatEncAlgType((E_ENCALG_TYPE)m_CryptoParam.m_dwEncAlgType),
                CCryptoUtility::FormatEncLibType((E_ENCLIB_TYPE)m_CryptoParam.m_dwEncLibType),
                CCryptoUtility::FormatHashAlgType((E_HASHALG_TYPE)m_CryptoParam.m_dwHashAlgType),
                CCryptoUtility::FormatInterfaceType((E_INTER_TYPE)m_CryptoParam.m_dwInterfaceType),
                CCryptoUtility::FormatIVUsageStatus((E_IV_USAGE)m_CryptoParam.m_dwIVUsage));
        }

        long lRetCode = AcquireContext(NULL, true);
        if (0 != lRetCode)
        {
            ERRTRCEX(lRetCode, L"Failed to acquire security context for CAPI.");
            return lRetCode;
        }

        DETLOG(L"Succeed to acquire context.");

        if(m_CryptoParam.m_dwInterfaceType & EIT_CRYPTO)
        {
            lRetCode = GenerateSymmetricKey(NULL);
            if(lRetCode != 0)
            {
                ERRTRCEX(lRetCode, L"Failed to generate symmetric encryption key.");
                return lRetCode;
            }

            DETTRC(L"Succeed to generate symmetric encryption key.");
        }

        DETLOG(L"Succeed to initialzie CAPI encryption object. (Container Name=[%s])", WS_S(m_wsKeySetName));;

        DBGTRC(L"==> [CAPI] Instance GUID: %s, Key Set Name: %s.", WZ_S(m_InstanceGUID.GUIDStr()), WS_S(m_wsKeySetName));
        return 0;
    }

    long CCryptoCAPIWrapper::InitializeEx(
        const CCryptoParam* pCryptoParam, 
        const WCHAR* pwzKeyContainerName /* = NULL */, 
        bool bTryRandomNameWhenFailed /* = true */, 
        bool bKeepContainer /* = false */)
    {
        if(pCryptoParam)
        {
            m_CryptoParam = *pCryptoParam;
            CCryptoUtility::UpdateCryptoParam(m_CryptoParam, *m_pCryptoCfg);
            m_CryptoParam.m_dwEncAPIType = ECAT_CAPI;
        }

        if(m_pCryptoCfg->m_CryptoDBGCfg.m_bDumpPwd)
        {
            INFLOG(L"[CAPI] Interface Information: API{%s}, EncAlg{%s}, EncLib{%s}, HashAlg{%s}, Iter{%s}, IV{%s}, PWD{%s}", 
                CCryptoUtility::FormatCryptoAPIType((E_CRYPTOAPI_TYPE)m_CryptoParam.m_dwEncAPIType),
                CCryptoUtility::FormatEncAlgType((E_ENCALG_TYPE)m_CryptoParam.m_dwEncAlgType),
                CCryptoUtility::FormatEncLibType((E_ENCLIB_TYPE)m_CryptoParam.m_dwEncLibType),
                CCryptoUtility::FormatHashAlgType((E_HASHALG_TYPE)m_CryptoParam.m_dwHashAlgType),
                CCryptoUtility::FormatInterfaceType((E_INTER_TYPE)m_CryptoParam.m_dwInterfaceType),
                CCryptoUtility::FormatIVUsageStatus((E_IV_USAGE)m_CryptoParam.m_dwIVUsage),
                m_CryptoParam.m_wsPassword.c_str());
        }
        else
        {
            DETLOG(L"[CAPI] Interface Information: API{%s}, EncAlg{%s}, EncLib{%s}, HashAlg{%s}, Iter{%s}, IV{%s}", 
                CCryptoUtility::FormatCryptoAPIType((E_CRYPTOAPI_TYPE)m_CryptoParam.m_dwEncAPIType),
                CCryptoUtility::FormatEncAlgType((E_ENCALG_TYPE)m_CryptoParam.m_dwEncAlgType),
                CCryptoUtility::FormatEncLibType((E_ENCLIB_TYPE)m_CryptoParam.m_dwEncLibType),
                CCryptoUtility::FormatHashAlgType((E_HASHALG_TYPE)m_CryptoParam.m_dwHashAlgType),
                CCryptoUtility::FormatInterfaceType((E_INTER_TYPE)m_CryptoParam.m_dwInterfaceType),
                CCryptoUtility::FormatIVUsageStatus((E_IV_USAGE)m_CryptoParam.m_dwIVUsage));
        }

        m_bKeepContainer = bKeepContainer;

        long lRetCode = AcquireContext(pwzKeyContainerName, bTryRandomNameWhenFailed);
        if (0 != lRetCode)
        {
            ERRTRCEX(lRetCode, L"Failed to acquire security context for CAPI.");
            return lRetCode;
        }

        DETTRC(L"Succeed to acquire context.");

        return lRetCode;
    }

    long CCryptoCAPIWrapper::GenerateSymmetricKey(const WCHAR* pwzPassword)
    {
        if(m_hCryptoProvider.IsNULL())
        {
            ERRTRCEX(D2DCRYPTO_E_INSTANCE_NOT_INIT, L"Crypto provider handle is empty, maybe not initialized.");
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;
        }

        DBGTRC(L"==> [CAPI] Instance GUID: %s, Key Set Name: %s.", WZ_S(m_InstanceGUID.GUIDStr()), WS_S(m_wsKeySetName));

        if(NULL == pwzPassword)
            pwzPassword = m_CryptoParam.m_wsPassword.c_str();

        DWORD dwPwdLen = (DWORD)wcslen(pwzPassword);
        CCAPIHandleWrap<HCRYPTHASH, EHT_HASH> hCryptoHash;
        CRYPTO_RETCHK(
            CryptCreateHash(
            m_hCryptoProvider, 
            CCryptoUtility::GetHashAlgType((E_HASHALG_TYPE)m_CryptoParam.m_dwHashAlgType), 
            0, 
            0, 
            hCryptoHash));
        // Hash in the password data.
        CRYPTO_RETCHK(
            CryptHashData(
            hCryptoHash, 
            (const PBYTE)pwzPassword, 
            (DWORD)(dwPwdLen * sizeof(WCHAR)),
            0));

        // Read the hash value size and allocate memory.
        DWORD dwCount = sizeof(DWORD);
        DWORD dwHashSize = 0;
        if(CryptGetHashParam(hCryptoHash, HP_HASHSIZE, (BYTE*)&dwHashSize, &dwCount, 0)) 
        {
            PBYTE pbHashData = new BYTE[dwHashSize];
            // Read the hash value.
            if(!CryptGetHashParam(hCryptoHash, HP_HASHVAL, pbHashData, &dwHashSize, 0))
            {
                DWORD dwLastErr = GetLastError();
            }
            if(pbHashData)
                delete []pbHashData;
        }

        // Derive a session key from the hash object.
        CRYPTO_RETCHK(
            CryptDeriveKey(
            m_hCryptoProvider, 
            CCryptoUtility::GetEncAlgorithmID((E_ENCALG_TYPE)m_CryptoParam.m_dwEncAlgType),
            hCryptoHash, 
            0, 
            m_hCryptoKey));

        DWORD dwAlgKeyLen = 0;
        long lRetCode = GetKeyLength(dwAlgKeyLen);
        if(0 == lRetCode)
            DETLOG(L"CAPI: symmetric key size: %u.", dwAlgKeyLen);
        else
            WARTRCEX(lRetCode, L"CAPI: failed to get symmetric key size. (EC=[%u])", lRetCode);

        if(EIU_USE_IV == m_CryptoParam.m_dwIVUsage)
            CRYPTO_RETCHK(CryptSetKeyParam(m_hCryptoKey, KP_IV, (BYTE*)g_CNG256IV, 0));

        // Set Mode
        DWORD dwMode = CRYPT_MODE_CBC;
        CRYPTO_RETCHK(
            CryptSetKeyParam(
            m_hCryptoKey, 
            KP_MODE, 
            (BYTE*)&dwMode, 
            0));

        return 0;
    }

    long CCryptoCAPIWrapper::EncryptData(
        const BYTE* pPlainbataBuf, 
        DWORD dwPlainDataBufLen, 
        BYTE* pbCipherDataBuf, 
        DWORD* pdwCipherDataBufLen, 
        bool bPadding /* = false */)
    {
        if((NULL == pPlainbataBuf) || (0 == dwPlainDataBufLen) || (NULL == pdwCipherDataBufLen))
            return D2DCRYPTO_E_INVALID_PARAM;
        if(m_hCryptoKey.IsNULL())
        {
            ERRTRCEX(D2DCRYPTO_E_INSTANCE_NOT_INIT, L"Crypto key handle is empty, maybe not initialized.");
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;
        }

        DBGTRC(L"==> [CAPI] Instance GUID: %s, Key Set Name: %s.", WZ_S(m_InstanceGUID.GUIDStr()), WS_S(m_wsKeySetName));

        DWORD dwCipherBufLen = *pdwCipherDataBufLen;
        *pdwCipherDataBufLen = dwPlainDataBufLen;

        // Duplicate the key.
        CCAPIHandleWrap<HCRYPTKEY, EHT_KEY> hEncKey;
        CRYPTO_RETCHK(CryptDuplicateKey(m_hCryptoKey, NULL, 0, hEncKey));

        CRYPTO_RETCHK(
            CryptEncrypt(
            hEncKey,
            NULL,
            bPadding ? TRUE : FALSE,
            0, 
            NULL, 
            pdwCipherDataBufLen, 
            dwCipherBufLen));

        if(*pdwCipherDataBufLen > dwCipherBufLen)
            return D2DCRYPTO_E_MORE_DATA;
        else if(NULL == pbCipherDataBuf)
            return D2DCRYPTO_E_MORE_DATA;

        if(pbCipherDataBuf != pPlainbataBuf)
        {
            memset(pbCipherDataBuf, 0, *pdwCipherDataBufLen);
            memcpy(pbCipherDataBuf, pPlainbataBuf, dwPlainDataBufLen);
        }

        *pdwCipherDataBufLen = dwPlainDataBufLen;

        CRYPTO_RETCHK(
            CryptEncrypt(
            hEncKey,
            NULL,
            bPadding ? TRUE : FALSE,
            0, 
            pbCipherDataBuf, 
            pdwCipherDataBufLen, 
            dwCipherBufLen));

        return 0;
    }

    long CCryptoCAPIWrapper::DecryptData(
        const BYTE* pbCipherDataBuf, 
        DWORD dwCipherDataBufLen, 
        BYTE* pPlainbataBuf, 
        DWORD* pdwPlainDataBufLen, 
        bool bPadding /* = false */)
    {
        if((NULL == pbCipherDataBuf) || (0 == dwCipherDataBufLen) || (NULL == pdwPlainDataBufLen))
            return D2DCRYPTO_E_INVALID_PARAM;
        if(m_hCryptoKey.IsNULL())
        {
            ERRTRCEX(D2DCRYPTO_E_INSTANCE_NOT_INIT, L"Crypto provider handle is empty, maybe not initialized.");
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;
        }

        DBGTRC(L"==> [CAPI] Instance GUID: %s, Key Set Name: %s.", WZ_S(m_InstanceGUID.GUIDStr()), WS_S(m_wsKeySetName));

        if(*pdwPlainDataBufLen < dwCipherDataBufLen)
        {
            *pdwPlainDataBufLen = dwCipherDataBufLen;
            return D2DCRYPTO_E_MORE_DATA;
        }
        else if(NULL == pPlainbataBuf)
        {
            *pdwPlainDataBufLen = dwCipherDataBufLen;
            return D2DCRYPTO_E_MORE_DATA;
        }

        if(pPlainbataBuf != pbCipherDataBuf)
        {
            memset(pPlainbataBuf, 0, *pdwPlainDataBufLen);
            memcpy(pPlainbataBuf, pbCipherDataBuf, dwCipherDataBufLen);
        }

        DWORD dwCipherBufLen = *pdwPlainDataBufLen;
        *pdwPlainDataBufLen = dwCipherDataBufLen;

        // Duplicate the key.
        CCAPIHandleWrap<HCRYPTKEY, EHT_KEY> hDecKey;
        CRYPTO_RETCHK(CryptDuplicateKey(m_hCryptoKey, NULL, 0, hDecKey));

        CRYPTO_RETCHK(
            CryptDecrypt(
            hDecKey,
            NULL,
            bPadding ? TRUE : FALSE,
            0, 
            pPlainbataBuf, 
            pdwPlainDataBufLen));

        memset(pPlainbataBuf + *pdwPlainDataBufLen, 0, dwCipherBufLen - *pdwPlainDataBufLen);

        return 0;
    }

    long CCryptoCAPIWrapper::GetHashData(
        const BYTE* pPlainbataBuf, 
        DWORD dwPlainDataBufLen,
        BYTE* pHashbataBuf, 
        DWORD* pdwHashDataBufLen, 
        DWORD dwHashAlgType /* = EHAT_UNKNOWN */)
    {
        if((NULL == pPlainbataBuf) || (0 == dwPlainDataBufLen) || (NULL == pdwHashDataBufLen))
            return D2DCRYPTO_E_INVALID_PARAM;

        DBGTRC(L"==> [CAPI] Instance GUID: %s, Key Set Name: %s.", WZ_S(m_InstanceGUID.GUIDStr()), WS_S(m_wsKeySetName));

        CByteBuf HashValue;
        long lRetCode = GetHashData(pPlainbataBuf, dwPlainDataBufLen, HashValue, dwHashAlgType);
        if(0 == lRetCode)
        {
            if(*pdwHashDataBufLen < HashValue.m_dwBufSize)
            {
                *pdwHashDataBufLen = HashValue.m_dwBufSize;
                return D2DCRYPTO_E_MORE_DATA;
            }
            else if(NULL == pHashbataBuf)
            {
                *pdwHashDataBufLen = HashValue.m_dwBufSize;
                return D2DCRYPTO_E_MORE_DATA;
            }
            memcpy(pHashbataBuf, HashValue.m_pByteBuf, HashValue.m_dwBufSize);
        }
        return lRetCode;
    }

    long CCryptoCAPIWrapper::GetHashData(
        const BYTE* pPlainbataBuf, 
        DWORD dwPlainDataBufLen, 
        wstring& wsHashStr, 
        DWORD dwHashAlgType /* = EHAT_UNKNOWN */)
    {
        DBGTRC(L"==> [CAPI] Instance GUID: %s, Key Set Name: %s.", WZ_S(m_InstanceGUID.GUIDStr()), WS_S(m_wsKeySetName));
        CByteBuf HashValue;
        long lRetCode = GetHashData(pPlainbataBuf, dwPlainDataBufLen, HashValue, dwHashAlgType);
        if(0 == lRetCode)
        {
            return CCryptoUtility::Bin2HexString(wsHashStr, HashValue.m_pByteBuf, HashValue.m_dwBufSize);
        }
        return lRetCode;
    }

    long CCryptoCAPIWrapper::GetHashValueSize(DWORD& dwHashSize, DWORD dwHashAlgType /* = EHAT_UNKNOWN */)
    {
        if(m_hCryptoProvider.IsNULL())
        {
            ERRTRCEX(D2DCRYPTO_E_INSTANCE_NOT_INIT, L"Crypto provider handle is empty, maybe not initialized.");
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;
        }

        DBGTRC(L"==> [CAPI] Instance GUID: %s, Key Set Name: %s.", WZ_S(m_InstanceGUID.GUIDStr()), WS_S(m_wsKeySetName));

        if (EHAT_UNKNOWN == dwHashAlgType)
            dwHashAlgType = m_CryptoParam.m_dwHashAlgType;

        CCAPIHandleWrap<HCRYPTHASH, EHT_HASH> hCryptoHash;
        CRYPTO_RETCHK(
            CryptCreateHash(
            m_hCryptoProvider, 
            CCryptoUtility::GetHashAlgType((E_HASHALG_TYPE)dwHashAlgType), 
            0, 
            0, 
            hCryptoHash));

        // Read the hash value size and allocate memory.
        DWORD dwCount = sizeof(DWORD);
        if(!CryptGetHashParam(hCryptoHash, HP_HASHSIZE, (BYTE*)&dwHashSize, &dwCount, 0)) 
        {
            long lRetCode = GetLastError();
            return lRetCode;
        }
        return 0;
    }

    long CCryptoCAPIWrapper::GetAlgorithmBlockSize(DWORD& dwBlockSize)
    {
        DBGTRC(L"==> [CAPI] Instance GUID: %s, Key Set Name: %s.", WZ_S(m_InstanceGUID.GUIDStr()), WS_S(m_wsKeySetName));
        return GetBlockLength(dwBlockSize);
    }

    const WCHAR* CCryptoCAPIWrapper::InstanceGUID(GUID* pguidInstance /* = NULL */)
    {
        if (pguidInstance)
            *pguidInstance = m_InstanceGUID.GUIDObject();
        return m_InstanceGUID.GUIDStr();
    }

    long CCryptoCAPIWrapper::GetInstanceCount(DWORD& dwInstRefCount)
    {
        dwInstRefCount = CCryptoCAPIWrapper::g_lInstRefCount;
        return 0;
    }

    long CCryptoCAPIWrapper::AcquireContext(const WCHAR* pwzContanerName /* = NULL */, bool bTryRandomNameWhenFailed /* = true */)
    {
        // Attempt to acquire a handle to the specified key container.
        // HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Cryptography\Defaults\Provider shows all CSPs
        //ZZ: We should use machine key set to storage private key. Otherwise initialization will be failed
        //ZZ: when user doesn't login(user profile will not be loaded)

        wstring wsCurKeySetName;
        wstring wsGUIDInKeySetName;
        for (UINT uiIdx = 0; uiIdx < m_pCryptoCfg->m_CryptoDBGCfg.m_dwKeySetRetryTimes; uiIdx++)
        {
            if ((0 == uiIdx) && pwzContanerName)
                wsCurKeySetName = pwzContanerName;
            else
            {
                if (wsGUIDInKeySetName.empty())
                    wsGUIDInKeySetName = m_InstanceGUID.GUIDStr();
                else
                    wsGUIDInKeySetName = CCryptoUtility::GetRandomGUID();

                wsCurKeySetName = CCryptoUtility::GetRandomKeyContainerName(wsGUIDInKeySetName.c_str());
            }

            DETLOG(L"Current key set name is %s.", WS_S(wsCurKeySetName));

            m_dwKeyContainerFlag = CRYPT_MACHINE_KEYSET;
            if(!CryptAcquireContextW(m_hCryptoProvider,
                wsCurKeySetName.c_str(),
                CCryptoUtility::GetCSPName((E_ENCALG_TYPE)m_CryptoParam.m_dwEncAlgType),
                CCryptoUtility::GetCSPType((E_ENCALG_TYPE)m_CryptoParam.m_dwEncAlgType),
                m_dwKeyContainerFlag))
            { 
                //--------------------------------------------------------------------
                // An error occurred in acquiring the context. This could mean
                // that the key container requested does not exist. In this case,
                // the function can be called again to attempt to create a new key 
                // container. Error codes are defined in winerror.h.
                DWORD dwLastErr = GetLastError();
                DETTRCEX(dwLastErr, L"Failed to open key set, try to create it. (EC=[%#08x], Name=[%s], Flag=[%#08x])", dwLastErr, WS_S(wsCurKeySetName), m_dwKeyContainerFlag);
                //if(NTE_BAD_KEYSET == dwLastErr)
                {
                    m_dwKeyContainerFlag |= CRYPT_NEWKEYSET;
                    if (!CryptAcquireContextW(m_hCryptoProvider,
                        wsCurKeySetName.c_str(),
                        CCryptoUtility::GetCSPName((E_ENCALG_TYPE)m_CryptoParam.m_dwEncAlgType),
                        CCryptoUtility::GetCSPType((E_ENCALG_TYPE)m_CryptoParam.m_dwEncAlgType),
                        m_dwKeyContainerFlag))
                    {
                        dwLastErr = GetLastError();
                        ERRTRCEX(dwLastErr, L"Failed to create key set. (EC=[%#08x], Name=[%s], Flag=[%#08x])", dwLastErr, WS_S(m_wsKeySetName), m_dwKeyContainerFlag);
                    }
                    else
                    {
                        DETTRC(L"Succeed to create key set. (Name=[%s])", WS_S(wsCurKeySetName));
                        break;
                    }
                }
            }
            else
            {
                DETTRC(L"Succeed to open key set. (Name=[%s])", WS_S(wsCurKeySetName));
                break;
            }

            if (!bTryRandomNameWhenFailed)
            {
                ERRTRCEX(D2DCRYPTO_E_UNABLE_TO_CREATE_CONTAINER, L"Unable to create key container.");
                return D2DCRYPTO_E_UNABLE_TO_CREATE_CONTAINER;
            }
        }

        if (m_hCryptoProvider.IsNULL())
        {
            ERRTRCEX(D2DCRYPTO_E_INSTANCE_NOT_INIT, L"Failed to initalize crypto instance. Provider handle is empty.");
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;
        }

        m_wsKeySetName = wsCurKeySetName;
        DETTRC(L"Succeed to open key set. (Name=[%s])", WS_S(m_wsKeySetName));

        return 0;
    }

    long CCryptoCAPIWrapper::GetBlockLength(DWORD& dwBlockLen, HCRYPTKEY hAlgKey /* = NULL */)
    {
        if((NULL == hAlgKey) && (NULL == m_hCryptoKey))
            return E_INVALIDARG;
        else if(NULL == hAlgKey)
            hAlgKey = m_hCryptoKey;

        if(NULL == hAlgKey)
        {
            ERRTRCEX(D2DCRYPTO_E_INSTANCE_NOT_INIT, L"Crypto algorithm handle is empty, maybe not initialized.");
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;
        }

        DBGTRC(L"==> [CAPI] Instance GUID: %s, Key Set Name: %s.", WZ_S(m_InstanceGUID.GUIDStr()), WS_S(m_wsKeySetName));

        DWORD dwBlockSizeLen = sizeof(DWORD);
        // Read the initialization vector.
        CRYPTO_RETCHK(
            CryptGetKeyParam(
            hAlgKey, 
            KP_BLOCKLEN, 
            (PBYTE)&dwBlockLen, 
            &dwBlockSizeLen, 
            0));
        
        dwBlockLen /= 8;

        return 0;
    }

    long CCryptoCAPIWrapper::GetKeyLength(DWORD& dwKeylen, HCRYPTKEY hAlgKey /* = NULL */)
    {
        if((NULL == hAlgKey) && (NULL == m_hCryptoKey))
            return E_INVALIDARG;
        else if(NULL == hAlgKey)
            hAlgKey = m_hCryptoKey;

        if(NULL == hAlgKey)
        {
            ERRTRCEX(D2DCRYPTO_E_INSTANCE_NOT_INIT, L"Crypto algorithm handle is empty, maybe not initialized.");
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;
        }

        DBGTRC(L"==> [CAPI] Instance GUID: %s, Key Set Name: %s.", WZ_S(m_InstanceGUID.GUIDStr()), WS_S(m_wsKeySetName));

        DWORD dwKeyLenSize = sizeof(DWORD);
        // Read the initialization vector.
        CRYPTO_RETCHK(
            CryptGetKeyParam(
            hAlgKey, 
            KP_KEYLEN, 
            (PBYTE)&dwKeylen, 
            &dwKeyLenSize, 
            0));

        return 0;
    }

    long CCryptoCAPIWrapper::GetKeyContainerList(
        WStrVector& vecKeyContainer, 
        HCRYPTPROV hCryptoProv /* = NULL */, 
        E_QUERY_KEYSET_CTRL eQueryCtrl /* = EQKC_D2D_ONLY */, 
        bool bSkipSelf /* = true */)
    {
        if(NULL == hCryptoProv)
        {
            if(NULL == m_hCryptoProvider)
                return E_INVALIDARG;
            hCryptoProv = (HCRYPTPROV)m_hCryptoProvider;
        }

        if(NULL == hCryptoProv)
        {
            ERRTRCEX(D2DCRYPTO_E_INSTANCE_NOT_INIT, L"Crypto key handle is empty, maybe not initialized.");
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;
        }

        DBGTRC(L"==> [CAPI] Instance GUID: %s, Key Set Name: %s.", WZ_S(m_InstanceGUID.GUIDStr()), WS_S(m_wsKeySetName));

        DWORD dwKeyContainerMaxSize = 0;
        CHAR szContainerName[1024] = {0};
        DWORD dwSize = sizeof(szContainerName);
        DWORD dwFlags = CRYPT_FIRST;
        for(;;)
        {
            if(!CryptGetProvParam(
                hCryptoProv,
                PP_ENUMCONTAINERS, 
                (BYTE*)szContainerName, 
                &dwSize, 
                dwFlags))
            {
                DWORD dwLastErr = GetLastError();
                if(ERROR_NO_MORE_ITEMS == dwLastErr)
                    break;
                ERRTRCEX(dwLastErr, L"Failed to enumerate key container. (EC=[%u])", dwLastErr);
                return dwLastErr;
            }

            if (CRYPT_FIRST == dwFlags)
                dwKeyContainerMaxSize = dwSize;

            std::wstring wsKeyContainerName;
            CCryptoUtility::AnsiToUnicode(szContainerName, wsKeyContainerName);

            bool bReturnResult = false;
            switch(eQueryCtrl)
            {
            case EQKC_ALL:
                bReturnResult = true;
                break;
            case EQKC_D2D_ONLY:
                if (wstring::npos != wsKeyContainerName.find(gwzKeyContainerNameForCAPI))
                    bReturnResult = true;
                break;
            case EQKC_D2D_EXCLUDED:
                if (wstring::npos == wsKeyContainerName.find(gwzKeyContainerNameForCAPI))
                    bReturnResult = true;
            }

            if (bReturnResult && (!bSkipSelf || (0 != _wcsicmp(wsKeyContainerName.c_str(), m_wsKeySetName.c_str()))))
            {
                DETLOG(L"[Key Container Name]: %s, [Max Size]: %u", wsKeyContainerName.c_str(), dwKeyContainerMaxSize);
                vecKeyContainer.push_back(wsKeyContainerName);
            }

            if(CRYPT_FIRST == dwFlags)
                dwFlags = CRYPT_NEXT;
        }
        return 0;
    }

    long CCryptoCAPIWrapper::GetHashData(
        const BYTE* pPlainbataBuf, 
        DWORD dwPlainDataBufLen, 
        CByteBuf& HashData, 
        DWORD dwHashAlgType /* = EHAT_UNKNOWN */)
    {
        if(m_hCryptoProvider.IsNULL())
        {
            ERRTRCEX(D2DCRYPTO_E_INSTANCE_NOT_INIT, L"Crypto provider handle is empty, maybe not initialized.");
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;
        }

        DBGTRC(L"==> [CAPI] Instance GUID: %s, Key Set Name: %s.", WZ_S(m_InstanceGUID.GUIDStr()), WS_S(m_wsKeySetName));

        if (EHAT_UNKNOWN == dwHashAlgType)
            dwHashAlgType = m_CryptoParam.m_dwHashAlgType;

        CCAPIHandleWrap<HCRYPTHASH, EHT_HASH> hCryptoHash;
        CRYPTO_RETCHK(
            CryptCreateHash(
            m_hCryptoProvider, 
            CCryptoUtility::GetHashAlgType((E_HASHALG_TYPE)dwHashAlgType), 
            0, 
            0, 
            hCryptoHash));

        // Hash in the password data.
        CRYPTO_RETCHK(
            CryptHashData(
            hCryptoHash, 
            pPlainbataBuf, 
            dwPlainDataBufLen,
            0));

        // Read the hash value size and allocate memory.
        DWORD dwCount = sizeof(DWORD);
        DWORD dwHashSize = 0;
        CRYPTO_RETCHK(CryptGetHashParam(hCryptoHash, HP_HASHSIZE, (BYTE*)&dwHashSize, &dwCount, 0));
        HashData.AllocMem(dwHashSize);
        CRYPTO_RETCHK(CryptGetHashParam(hCryptoHash, HP_HASHVAL, HashData.m_pByteBuf, &dwHashSize, 0))
        return 0;
    }

    long CCryptoCAPIWrapper::RemoveKeyContainer(const WCHAR* pwzKeyContainer)
    {
        if(NULL == pwzKeyContainer)
            WARLOGEX(D2DCOMM_E_INVALID_PARAM, L"Default key contianer will be removed, this will affect any other appclaition useing this key container.");

        DBGTRC(L"==> [CAPI] Instance GUID: %s, Key Set Name: %s.", WZ_S(m_InstanceGUID.GUIDStr()), WS_S(m_wsKeySetName));

        HCRYPTPROV hRemoveProv = NULL;
        CRYPTO_RETCHK(CryptAcquireContextW(
            &hRemoveProv,
            pwzKeyContainer,
            CCryptoUtility::GetCSPName((E_ENCALG_TYPE)m_CryptoParam.m_dwEncAlgType),
            CCryptoUtility::GetCSPType((E_ENCALG_TYPE)m_CryptoParam.m_dwEncAlgType),
            CRYPT_MACHINE_KEYSET | CRYPT_DELETEKEYSET));
        return 0;
    }
}

