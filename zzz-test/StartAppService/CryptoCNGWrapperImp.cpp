#include "stdafx.h"
#include "CryptoCNGWrapperImp.h"
#include "CNGAPIWrapper.h"

namespace nsCryptoCNGWrapper
{
    long CCryptoCNGWrapper::g_lInstRefCount = 0;

    CCryptoCNGWrapper::CCryptoCNGWrapper(
        const CCryptoParam* pCryptoParam /* = NULL */, 
        CCryptoConfig* pCryptoCfg /* = NULL */)
        : m_pCryptoCfg(pCryptoCfg),
          m_bReleaseConfigMgr(false)
    {
        if(pCryptoParam)
            m_CryptoParam = *pCryptoParam;

        if(NULL == m_pCryptoCfg)
        {
            m_pCryptoCfg = new CCryptoConfig;
            m_bReleaseConfigMgr = true;
            CCryptoUtility::UpdateCryptoParam(m_CryptoParam, *m_pCryptoCfg);
        }

        m_CryptoParam.m_dwEncAPIType = ECAT_CNG;

        InterlockedIncrement(&CCryptoCNGWrapper::g_lInstRefCount);
        DBGTRC(L"Current CNG crypto instance or reference count is %u.", CCryptoCNGWrapper::g_lInstRefCount);
    }    

    CCryptoCNGWrapper::~CCryptoCNGWrapper()
    {
        InterlockedDecrement(&CCryptoCNGWrapper::g_lInstRefCount);
        DBGTRC(L"Current CNG crypto instance or reference count is %u.", CCryptoCNGWrapper::g_lInstRefCount);

        if(m_pCryptoCfg && m_bReleaseConfigMgr)
        {
            delete m_pCryptoCfg;
            m_pCryptoCfg = NULL;
        }
    }

    long CCryptoCNGWrapper::Initialize(const CCryptoParam* pCryptoParam)
    {
        if(pCryptoParam)
        {
            m_CryptoParam = *pCryptoParam;
            CCryptoUtility::UpdateCryptoParam(m_CryptoParam, *m_pCryptoCfg);
            m_CryptoParam.m_dwEncAPIType = ECAT_CNG;
        }

        if(m_pCryptoCfg->m_CryptoDBGCfg.m_bDumpPwd)
        {
            INFLOG(L"[CNG] Interface Information: API{%s}, EncAlg{%s}, EncLib{%s}, HashAlg{%s}, Iter{%s}, IV{%s}, PWD{%s}", 
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
            DETLOG(L"[CNG] Interface Information: API{%s}, EncAlg{%s}, EncLib{%s}, HashAlg{%s}, Iter{%s}, IV{%s}", 
                CCryptoUtility::FormatCryptoAPIType((E_CRYPTOAPI_TYPE)m_CryptoParam.m_dwEncAPIType),
                CCryptoUtility::FormatEncAlgType((E_ENCALG_TYPE)m_CryptoParam.m_dwEncAlgType),
                CCryptoUtility::FormatEncLibType((E_ENCLIB_TYPE)m_CryptoParam.m_dwEncLibType),
                CCryptoUtility::FormatHashAlgType((E_HASHALG_TYPE)m_CryptoParam.m_dwHashAlgType),
                CCryptoUtility::FormatInterfaceType((E_INTER_TYPE)m_CryptoParam.m_dwInterfaceType),
                CCryptoUtility::FormatIVUsageStatus((E_IV_USAGE)m_CryptoParam.m_dwIVUsage));
        }

        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        long lRetCode = 0;
        if(m_CryptoParam.m_dwInterfaceType & EIT_CRYPTO)
        {
            AlgIDVector vecAlgID;
            GetRegisteredAlgorithm(BCRYPT_CIPHER_OPERATION, vecAlgID);
            wstring wsAlgProvName;
            AlgProvVector vecAlgProv;
            GetRegisteredAlgorithmProvider(
                CCryptoUtility::GetEncAlgorithmName((E_ENCALG_TYPE)m_CryptoParam.m_dwEncAlgType),
                vecAlgProv,
                &wsAlgProvName);
            //open an algorithm handle
            BCRYPT_RETCHK(nsCryptoCNGWrapper::
                BCryptOpenAlgorithmProvider(
                m_hCryptoCNGAlg,
                CCryptoUtility::GetEncAlgorithmName((E_ENCALG_TYPE)m_CryptoParam.m_dwEncAlgType),
                wsAlgProvName.empty() ? NULL : wsAlgProvName.c_str(),
                0));

            DWORD dwLength = 0;
            BCRYPT_RETCHK(GetObjectLength(dwLength));
            m_hCryptoCNGKey.HandleObj().AllocMem(dwLength);

            lRetCode = SetEncCBC(); 
            if(0 == lRetCode)
            {
                DETTRC(L"Succeed to set CBC mode.");
                lRetCode = GenerateSymmetricKey(NULL);
                if (0 == lRetCode)
                    DETTRC(L"Succed to generate symmetric encryption key.");
                else
                    ERRTRCEX(lRetCode, L"Failed to generate symmetric encryption key.");
            }
            else
                ERRTRCEX(lRetCode, L"Failed to set CBC mode.");
        }

        if (0 == lRetCode)
            DETLOG(L"Succeed to initialize CNG encryption object.");
        else
            ERRLOGEX(lRetCode, L"Failed to inialize CNG encryption object.");
 
        return lRetCode;
    }

    long CCryptoCNGWrapper::GenerateSymmetricKey(const WCHAR* pwzPassword)
    {
        if(m_hCryptoCNGAlg.IsNULL())
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;

        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        // generate the key from supplied input key bytes
        if(NULL == pwzPassword)
            pwzPassword = m_CryptoParam.m_wsPassword.c_str();
        DWORD dwPwdLen = (DWORD)wcslen(pwzPassword);

        CByteBuf SecretInfo;
        GetSecretInfo(SecretInfo, (PBYTE)pwzPassword, (DWORD)(dwPwdLen * sizeof(WCHAR)));

        CByteBuf CryptoSecret;
        BCRYPT_RETCHK(
            CCryptoUtility::AdjustCryptoSecret(
            CryptoSecret,
            SecretInfo.m_pByteBuf,
            SecretInfo.m_dwBufSize,
            (E_ENCALG_TYPE)m_CryptoParam.m_dwEncAlgType));

        BCRYPT_RETCHK(nsCryptoCNGWrapper::
            BCryptGenerateSymmetricKey(
            m_hCryptoCNGAlg, 
            m_hCryptoCNGKey, 
            m_hCryptoCNGKey.HandleObj().m_pByteBuf, 
            m_hCryptoCNGKey.HandleObj().m_dwBufSize, 
            SecretInfo.m_pByteBuf, 
            CryptoSecret.m_dwBufSize, 
            0));

        DWORD dwKeySize = 0;
        long lRetCode = GetKeyLength(dwKeySize);
        if(0 == lRetCode)
            DETLOG(L"CNG: symmetric key size(bits): %u.", dwKeySize);
        else
            WARTRCEX(lRetCode, L"CNG: failed to get symmetric key size. (EC=[%u])", lRetCode);
        BCRYPT_KEY_LENGTHS_STRUCT stKeyLength = {0};
        lRetCode = GetSupportedKeyLength(stKeyLength);
        if(0 == lRetCode)
            DETLOG(L"CNG: supported key size(bits): [%u] to [%u] + [%u].", stKeyLength.dwMinLength, stKeyLength.dwMaxLength, stKeyLength.dwIncrement);
        else
            WARTRCEX(lRetCode, L"CNG: failed to get support key size. (EC=[%u])", lRetCode);

//         DWORD dwLength = 0;
//         BCRYPT_RETCHK(GetExportKeyLength(dwLength));
//         m_pbExportKey.AllocMem(dwLength);
// 
//         //save another copy of the key for later
//         BCRYPT_RETCHK(
//             BCryptExportKey(
//             m_hCryptoCNGKey, 
//             NULL, 
//             BCRYPT_OPAQUE_KEY_BLOB,
//             m_pbExportKey.m_pByteBuf, 
//             m_pbExportKey.m_dwBufSize, 
//             &dwLength, 
//             0));

        return 0;
    }

    long CCryptoCNGWrapper::EncryptData(
        const BYTE* pPlainbataBuf,
        DWORD dwPlainDataBufLen, 
        BYTE* pbCipherDataBuf, 
        DWORD* pdwCipherDataBufLen, 
        bool bPadding /* = false */)
    {
        if((NULL == pPlainbataBuf) || (0 == dwPlainDataBufLen) || (NULL == pdwCipherDataBufLen))
            return D2DCRYPTO_E_INVALID_PARAM;

        if(m_hCryptoCNGKey.IsNULL() || m_hCryptoCNGKey.IsEmpty())
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;

        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        CByteBuf IVBuffer;
        GetIVBuffer(IVBuffer);
        //get the output buffer size

        CCNGHandleWrap<BCRYPT_KEY_HANDLE, EHT_KEY> hTmpKey;
        hTmpKey.HandleObj().AllocMem(m_hCryptoCNGKey.HandleObj().m_dwBufSize);
        BCRYPT_RETCHK(nsCryptoCNGWrapper::
            BCryptDuplicateKey(
            m_hCryptoCNGKey,
            hTmpKey,
            hTmpKey.HandleObj().m_pByteBuf,
            hTmpKey.HandleObj().m_dwBufSize,
            0));

        DWORD dwLength = 0;
        BCRYPT_RETCHK(nsCryptoCNGWrapper::
            BCryptEncrypt(
            hTmpKey, 
            const_cast<PBYTE>(pPlainbataBuf), 
            dwPlainDataBufLen,
            NULL,
            IVBuffer.m_pByteBuf,
            IVBuffer.m_dwBufSize,
            NULL, 
            0, 
            &dwLength, 
            bPadding ? BCRYPT_BLOCK_PADDING : 0));

        if(dwLength > *pdwCipherDataBufLen)
        {
            *pdwCipherDataBufLen = dwLength;
            return D2DCRYPTO_E_MORE_DATA;
        }
        else if(NULL == pbCipherDataBuf)
        {
            *pdwCipherDataBufLen = dwLength;
            return D2DCRYPTO_E_MORE_DATA;
        }

        // use the key to encrypt the plaintext buffer.
        //for block sized messages, block padding will add an extra block
        BCRYPT_RETCHK(nsCryptoCNGWrapper::
            BCryptEncrypt(
            hTmpKey, 
            const_cast<PBYTE>(pPlainbataBuf), 
            dwPlainDataBufLen,
            NULL,
            IVBuffer.m_pByteBuf,
            IVBuffer.m_dwBufSize, 
            pbCipherDataBuf, 
            *pdwCipherDataBufLen, 
            &dwLength, 
            bPadding ? BCRYPT_BLOCK_PADDING : 0));

        *pdwCipherDataBufLen = dwLength;

        return 0;
    }

    long CCryptoCNGWrapper::DecryptData(
        const BYTE* pbCipherDataBuf, 
        DWORD dwCipherDataBufLen,
        BYTE* pPlainbataBuf, 
        DWORD* pdwPlainDataBufLen, 
        bool bPadding /* = false */)
    {
        if((NULL == pbCipherDataBuf) || (0 == dwCipherDataBufLen) || (NULL == pdwPlainDataBufLen))
            return D2DCRYPTO_E_INVALID_PARAM;

        if(m_hCryptoCNGKey.IsNULL() || m_hCryptoCNGKey.IsEmpty())
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;

        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        CByteBuf IVBuffer;
        GetIVBuffer(IVBuffer);

        CCNGHandleWrap<BCRYPT_KEY_HANDLE, EHT_KEY> hTmpKey;
        hTmpKey.HandleObj().AllocMem(m_hCryptoCNGKey.HandleObj().m_dwBufSize);
        BCRYPT_RETCHK(nsCryptoCNGWrapper::
            BCryptDuplicateKey(
            m_hCryptoCNGKey,
            hTmpKey,
            hTmpKey.HandleObj().m_pByteBuf,
            hTmpKey.HandleObj().m_dwBufSize,
            0));

        //get the output buffer size
        DWORD dwLength = 0;
        BCRYPT_RETCHK(nsCryptoCNGWrapper::
            BCryptDecrypt(
            hTmpKey, 
            const_cast<PBYTE>(pbCipherDataBuf), 
            dwCipherDataBufLen, 
            NULL,
            IVBuffer.m_pByteBuf,
            IVBuffer.m_dwBufSize, 
            NULL, 
            0, 
            &dwLength, 
            bPadding ? BCRYPT_BLOCK_PADDING : 0));
        if(dwLength > *pdwPlainDataBufLen)
        {
            *pdwPlainDataBufLen = dwLength;
            return D2DCRYPTO_E_MORE_DATA;
        }
        else if(NULL == pPlainbataBuf)
        {
            *pdwPlainDataBufLen = dwLength;
            return D2DCRYPTO_E_MORE_DATA;
        }

        BCRYPT_RETCHK(nsCryptoCNGWrapper::
            BCryptDecrypt(
            hTmpKey, 
            const_cast<PBYTE>(pbCipherDataBuf), 
            dwCipherDataBufLen, 
            NULL,
            IVBuffer.m_pByteBuf,
            IVBuffer.m_dwBufSize, 
            pPlainbataBuf, 
            *pdwPlainDataBufLen, 
            &dwLength,
            bPadding ? BCRYPT_BLOCK_PADDING : 0));

        //ZZ: Update data size decrypted.
        memset(pPlainbataBuf + dwLength, 0, *pdwPlainDataBufLen - dwLength);
        *pdwPlainDataBufLen = dwLength;

        return 0;
    }

    long CCryptoCNGWrapper::GetHashData(
        const BYTE* pPlainbataBuf,
        DWORD dwPlainDataBufLen, 
        BYTE* pHashbataBuf,
        DWORD* pdwHashDataBufLen, 
        DWORD dwHashAlgType /* = EHAT_UNKNOWN */)
    {
        if((NULL == pPlainbataBuf) || (0 == dwPlainDataBufLen) || (NULL == pdwHashDataBufLen))
            return D2DCRYPTO_E_INVALID_PARAM;

        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        CByteBuf HashValue;
        long lRetCode = GetHashData(pPlainbataBuf, dwPlainDataBufLen, HashValue, dwHashAlgType);
        if(0 == lRetCode)
        {
            if(HashValue.m_dwBufSize > *pdwHashDataBufLen)
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

    long CCryptoCNGWrapper::GetHashData(
        const BYTE* pPlainbataBuf, 
        DWORD dwPlainDataBufLen, 
        wstring& wsHashStr, 
        DWORD dwHashAlgType /* = EHAT_UNKNOWN */)
    {
        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        CByteBuf HashValue;
        long lRetCode = GetHashData(pPlainbataBuf, dwPlainDataBufLen, HashValue, dwHashAlgType);
        if(0 == lRetCode)
        {
            return CCryptoUtility::Bin2HexString(wsHashStr, HashValue.m_pByteBuf, HashValue.m_dwBufSize);
        }
        return lRetCode;
    }

    long CCryptoCNGWrapper::GetHashValueSize(DWORD& dwHashSize, DWORD dwHashAlgType /* = EHAT_UNKNOWN */)
    {
        if (EHAT_UNKNOWN == dwHashAlgType)
            dwHashAlgType = m_CryptoParam.m_dwHashAlgType;

        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        CCNGHandleWrap<BCRYPT_ALG_HANDLE, EHT_PROV>  hHashAlgHadnle;
        BCRYPT_RETCHK(nsCryptoCNGWrapper::
            BCryptOpenAlgorithmProvider(
            hHashAlgHadnle,
            CCryptoUtility::GetHashAlgCSPName((E_HASHALG_TYPE)dwHashAlgType),
            NULL,
            0));
        return GetHashValueLength(dwHashSize, hHashAlgHadnle);
    }

    long CCryptoCNGWrapper::GetObjectLength(DWORD& dwObjLen, BCRYPT_ALG_HANDLE hAlgHandle /* = NULL */)
    {
        if(NULL == hAlgHandle)
            hAlgHandle = m_hCryptoCNGAlg;

        if(NULL == hAlgHandle)
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;

        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        //calculate the size of the buffer to hold the KeyObject
        ULONG ulCopyRslt = 0;
        BCRYPT_RETCHK(
            BCryptGetProperty(
            hAlgHandle, 
            BCRYPT_OBJECT_LENGTH, 
            (PBYTE)&dwObjLen, 
            sizeof(DWORD), 
            &ulCopyRslt, 
            0));

        return 0;
    }

    long CCryptoCNGWrapper::GetHashValueLength(DWORD& dwHashValLen, BCRYPT_ALG_HANDLE hAlgHandle)
    {
        if(NULL == hAlgHandle)
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;

        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        //calculate the size of the buffer to hold the KeyObject
        ULONG ulCopyRslt = 0;
        BCRYPT_RETCHK(
            BCryptGetProperty(
            hAlgHandle, 
            BCRYPT_HASH_LENGTH, 
            (PBYTE)&dwHashValLen, 
            sizeof(DWORD), 
            &ulCopyRslt, 
            0));

        return 0;
    }

    long CCryptoCNGWrapper::GetBlockLength(DWORD& dwBlockLen, BCRYPT_ALG_HANDLE hAlgHandle /* = NULL */)
    {
        if(NULL == hAlgHandle)
            hAlgHandle = m_hCryptoCNGAlg;

        if(NULL == hAlgHandle)
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;

        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        //calculate the block length for the IV
        ULONG ulCopyRslt = 0;
        BCRYPT_RETCHK(
            BCryptGetProperty(
            hAlgHandle, 
            BCRYPT_BLOCK_LENGTH, 
            (PBYTE)&dwBlockLen, 
            sizeof(DWORD), 
            &ulCopyRslt, 
            0));
        return 0;
    }

    long CCryptoCNGWrapper::GetExportKeyLength(DWORD& dwExportKeyLen, BCRYPT_KEY_HANDLE hCryptoKey /* = NULL */)
    {
        if(NULL == hCryptoKey)
            hCryptoKey = m_hCryptoCNGKey;

        if(NULL == hCryptoKey)
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;

        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        //save another copy of the key for later
        BCRYPT_RETCHK(
            BCryptExportKey(
            hCryptoKey,
            NULL,
            BCRYPT_OPAQUE_KEY_BLOB,
            NULL,
            0,
            &dwExportKeyLen,
            0));
        return 0;
    }

    long CCryptoCNGWrapper::GetIVBuffer(CByteBuf& IVBuffer)
    {
        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        // So far we should not use IV for Enc/Dec. It seem encryption will be failed in kernel mode if using IV
        if(EIU_USE_IV == m_CryptoParam.m_dwIVUsage)
        {
            DWORD dwLength = 0;
            BCRYPT_RETCHK(GetBlockLength(dwLength));
            IVBuffer.AllocMem(dwLength);
            memcpy(IVBuffer.m_pByteBuf, g_CNG256IV, dwLength);
        }
        return 0;
    }

    long CCryptoCNGWrapper::SetEncCBC(BCRYPT_ALG_HANDLE hAlgHandle /* = NULL */)
    {
        if(NULL == hAlgHandle)
            hAlgHandle = m_hCryptoCNGAlg;

        if(NULL == hAlgHandle)
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;

        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        BCRYPT_RETCHK(
            BCryptSetProperty(
            hAlgHandle, 
            BCRYPT_CHAINING_MODE, 
            (PBYTE)BCRYPT_CHAIN_MODE_CBC, 
            sizeof(BCRYPT_CHAIN_MODE_CBC), 
            0));
        return 0;
    }

    long CCryptoCNGWrapper::GetKeyLength(DWORD& dwKeylen, BCRYPT_KEY_HANDLE hCryptoKey /* = NULL */)
    {
        if(NULL == hCryptoKey)
            hCryptoKey = m_hCryptoCNGKey;

        if(NULL == hCryptoKey)
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;

        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        //calculate the block length for the IV
        ULONG ulCopyRslt = 0;
        BCRYPT_RETCHK(
            BCryptGetProperty(
            hCryptoKey, 
            BCRYPT_KEY_LENGTH, 
            (PBYTE)&dwKeylen, 
            sizeof(DWORD), 
            &ulCopyRslt, 
            0));

        return 0;
    }

    long CCryptoCNGWrapper::GetSupportedKeyLength(BCRYPT_KEY_LENGTHS_STRUCT& stKeyLength, BCRYPT_ALG_HANDLE hAlgHandle /* = NULL */)
    {
        if(NULL == hAlgHandle)
            hAlgHandle = m_hCryptoCNGAlg;

        if(NULL == hAlgHandle)
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;

        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        //calculate the block length for the IV
        ULONG ulCopyRslt = 0;
        BCRYPT_RETCHK(
            BCryptGetProperty(
            hAlgHandle, 
            BCRYPT_KEY_LENGTHS, 
            (PBYTE)&stKeyLength, 
            sizeof(BCRYPT_KEY_LENGTHS_STRUCT), 
            &ulCopyRslt, 
            0));

        return 0;
    }

    long CCryptoCNGWrapper::GetHashData(
        const BYTE* pPlainbataBuf, 
        DWORD dwPlainDataBufLen, 
        CByteBuf& HashData, 
        DWORD dwHashAlgType /* = EHAT_UNKNOWN */)
    {
        CCNGHandleWrap<BCRYPT_ALG_HANDLE, EHT_PROV> hHashAlgHadnle;
        CCNGHandleWrap<BCRYPT_HASH_HANDLE, EHT_HASH> hHashHandle;

        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        if (EHAT_UNKNOWN == dwHashAlgType)
            dwHashAlgType = m_CryptoParam.m_dwHashAlgType;

        BCRYPT_RETCHK(nsCryptoCNGWrapper::
            BCryptOpenAlgorithmProvider(
            hHashAlgHadnle,
            CCryptoUtility::GetHashAlgCSPName((E_HASHALG_TYPE)dwHashAlgType),
            NULL,
            0));

        DWORD dwLength = 0;
        BCRYPT_RETCHK(GetObjectLength(dwLength, hHashAlgHadnle));
        hHashHandle.HandleObj().AllocMem(dwLength);

        BCRYPT_RETCHK(nsCryptoCNGWrapper::
            BCryptCreateHash(
            hHashAlgHadnle,
            hHashHandle,
            hHashHandle.HandleObj().m_pByteBuf,
            hHashHandle.HandleObj().m_dwBufSize,
            NULL, 
            0,
            0));

        BCRYPT_RETCHK(nsCryptoCNGWrapper::
            BCryptHashData(    
            hHashHandle,
            const_cast<PUCHAR>(pPlainbataBuf),
            dwPlainDataBufLen,
            0));

        BCRYPT_RETCHK(GetHashValueLength(dwLength, hHashAlgHadnle));

        HashData.AllocMem(dwLength);
        // Finish the hash, hHashHandle will not be used any more.
        BCRYPT_RETCHK(nsCryptoCNGWrapper::
            BCryptFinishHash(  
            hHashHandle,
            HashData.m_pByteBuf,
            HashData.m_dwBufSize,
            0));

        return 0;
    }

    long CCryptoCNGWrapper::GetSecretInfo(
        CByteBuf& SecretInfo, 
        const PBYTE pbInKeyInfo, 
        DWORD dwKeyInfoSize, 
        BCRYPT_ALG_HANDLE hAlgHandle /* = NULL */)
    {
        //ZZ: We should allow input empty sesiion password.
        if(NULL == hAlgHandle)
            hAlgHandle = m_hCryptoCNGAlg;

        if(NULL == hAlgHandle)
            return D2DCRYPTO_E_INSTANCE_NOT_INIT;

        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        CCNGHandleWrap<BCRYPT_ALG_HANDLE, EHT_PROV>   hHashAlgHadnle;
        CCNGHandleWrap<BCRYPT_HASH_HANDLE, EHT_HASH>  hHashHandle;

        BCRYPT_RETCHK(nsCryptoCNGWrapper::
            BCryptOpenAlgorithmProvider(
                hHashAlgHadnle,
                CCryptoUtility::GetHashAlgCSPName((E_HASHALG_TYPE)m_CryptoParam.m_dwHashAlgType),
                NULL,
                0));

        DWORD dwhashObjLen = 0;

        BCRYPT_RETCHK(GetObjectLength(dwhashObjLen, hHashAlgHadnle));

        hHashHandle.HandleObj().AllocMem(dwhashObjLen);
        BCRYPT_RETCHK(nsCryptoCNGWrapper::
            BCryptCreateHash(
                hHashAlgHadnle,
                hHashHandle,
                hHashHandle.HandleObj().m_pByteBuf,
                hHashHandle.HandleObj().m_dwBufSize,
                NULL, 
                0,
                0));
        BCRYPT_RETCHK(nsCryptoCNGWrapper::
            BCryptHashData(    
                hHashHandle,
                pbInKeyInfo,
                dwKeyInfoSize,
                0));
        DWORD dwHashValLen = 0;
        BCRYPT_RETCHK(GetHashValueLength(dwHashValLen, hHashAlgHadnle));

        SecretInfo.AllocMem(dwHashValLen * 2);
        BCRYPT_RETCHK(nsCryptoCNGWrapper::
            BCryptDeriveKeyCapi(
                hHashHandle, 
                hAlgHandle, 
                (PUCHAR)SecretInfo.m_pByteBuf, 
                SecretInfo.m_dwBufSize,
                0));
        
        return 0;
    }

    long CCryptoCNGWrapper::GetRegisteredAlgorithm(DWORD dwAlgOperationType, AlgIDVector& vecAlgID)
    {
        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        DWORD dwAlgCount = 0;
        CNGStructurePtrWrap<BCRYPT_ALGORITHM_IDENTIFIER> pAlgList;
        BCRYPT_RETCHK(nsCryptoCNGWrapper::
			BCryptEnumAlgorithms(
            dwAlgOperationType, 
            &dwAlgCount,
            pAlgList,
            0));
        for(UINT uiIdex = 0; uiIdex < dwAlgCount; uiIdex++)
        {
            BCRYPT_ALGORITHM_IDENTIFIER& CNGAlgID = ((BCRYPT_ALGORITHM_IDENTIFIER*)pAlgList)[uiIdex];

            CAlgorithmIdentifier AlgID;
            if(CNGAlgID.pszName)
                AlgID.m_wsAlgName = CNGAlgID.pszName;
            AlgID.m_dwAlgFlag = CNGAlgID.dwFlags;
            AlgID.m_dwAlgClass = CNGAlgID.dwClass;
            vecAlgID.push_back(AlgID);
        }

        return 0;
    }

    long CCryptoCNGWrapper::GetRegisteredAlgorithmProvider(
        const WCHAR* pwzAlgID,
        AlgProvVector& vecAlgProv, 
        wstring* pwsDefaultProv /* = NULL */)
    {
        if(NULL == pwzAlgID)
            return E_INVALIDARG;

        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        DWORD dwAlgCount = 0;
        CNGStructurePtrWrap<BCRYPT_PROVIDER_NAME> pAlgProvList;
        BCRYPT_RETCHK(nsCryptoCNGWrapper::
            BCryptEnumProviders(
            pwzAlgID, 
            &dwAlgCount,
            pAlgProvList,
            0));
        for(UINT uiIdex = 0; uiIdex < dwAlgCount; uiIdex++)
        {
            BCRYPT_PROVIDER_NAME& AlgProvName = ((BCRYPT_PROVIDER_NAME*)pAlgProvList)[uiIdex];
            if(AlgProvName.pszProviderName)
            {
                CAlgorithmProviderIdentifier AlgProv;
                AlgProv.m_wsAlgID = pwzAlgID;
                AlgProv.m_wsProvName = AlgProvName.pszProviderName;
                if((0 == _wcsicmp(AlgProvName.pszProviderName, MS_PRIMITIVE_PROVIDER)) && pwsDefaultProv)
                {
                    *pwsDefaultProv = MS_PRIMITIVE_PROVIDER;
                    DETLOG(L"Default provider for algorithm %s. (Name=[%s])", pwzAlgID, AlgProvName.pszProviderName);
                }
                else
                    DETLOG(L"Provider for algorithm %s. (Name=[%s])", pwzAlgID, AlgProvName.pszProviderName);
                vecAlgProv.push_back(AlgProv);
            }
        }

        return 0;
    }

    long CCryptoCNGWrapper::GetAlgorithmBlockSize(DWORD& dwBlockSize)
    {
        DBGTRC(L"==> CNG Instance GUID: %s.", WZ_S(m_InstanceGUID.GUIDStr()));

        BCRYPT_RETCHK(GetBlockLength(dwBlockSize));
        return 0;
    }

    const WCHAR* CCryptoCNGWrapper::InstanceGUID(GUID* pguidInstance /* = NULL */)
    {
        if (pguidInstance)
            *pguidInstance = m_InstanceGUID.GUIDObject();
        return m_InstanceGUID.GUIDStr();
    }

    long CCryptoCNGWrapper::GetInstanceCount(DWORD& dwInstRefCount)
    {
        dwInstRefCount = CCryptoCNGWrapper::g_lInstRefCount;
        return 0;
    }
}
