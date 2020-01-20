#pragma once
#include "CryptoCommon.h"

namespace nsCryptoCAPIWrapper
{
    class CCryptoCAPIWrapper : public ICryptoWrapperInterface
    {
    public:
        CCryptoCAPIWrapper(const CCryptoParam* pCryptoParam = NULL, CCryptoConfig* pCryptoCfg = NULL);
        virtual ~CCryptoCAPIWrapper();

        virtual long Initialize(const CCryptoParam* pCryptoParam);

        virtual long InitializeEx(
            const CCryptoParam* pCryptoParam, 
            const WCHAR*        pwzKeyContainerName = NULL, 
            bool                bTryRandomNameWhenFailed = true,
            bool                bKeepContainer = false);

        virtual long GenerateSymmetricKey(const WCHAR* pwzPassword);

        virtual long EncryptData(
            const BYTE* pPlainbataBuf,
            DWORD       dwPlainDataBufLen, 
            BYTE*       pbCipherDataBuf, 
            DWORD*      pdwCipherDataBufLen,
            bool        bPadding = false);

        virtual long DecryptData(
            const BYTE* pbCipherDataBuf,
            DWORD       dwCipherDataBufLen, 
            BYTE*       pPlainbataBuf, 
            DWORD*      pdwPlainDataBufLen,
            bool        bPadding = false);

        virtual long GetHashData(
            const BYTE* pPlainbataBuf,
            DWORD       dwPlainDataBufLen, 
            BYTE*       pHashbataBuf,
            DWORD*      pdwHashDataBufLen,
            DWORD       dwHashAlgType = EHAT_UNKNOWN);

        virtual long GetHashData(
            const BYTE* pPlainbataBuf,
            DWORD       dwPlainDataBufLen, 
            wstring&    wsHashStr,
            DWORD       dwHashAlgType = EHAT_UNKNOWN);

        virtual long GetHashValueSize(DWORD& dwHashSize, DWORD dwHashAlgType = EHAT_UNKNOWN);

        virtual long GetAlgorithmBlockSize(DWORD& dwBlockSize);

        virtual const WCHAR* InstanceGUID(GUID* pguidInstance = NULL);

        virtual E_CRYPTOAPI_TYPE CryptoAPIType() { return ECAT_CAPI; }

        virtual long GetInstanceCount(DWORD& dwInstRefCount);

        virtual long GetKeyContainerList(
            WStrVector&         vecKeyContainer, 
            HCRYPTPROV          hCryptoProv = NULL,
            E_QUERY_KEYSET_CTRL eQueryCtrl = EQKC_D2D_ONLY,
            bool                bSkipSelf = true);

        virtual long RemoveKeyContainer(const WCHAR* pwzKeyContainer);

        const WCHAR* KeyContainerName() const { return m_wsKeySetName.c_str(); }

    private:
        long         AcquireContext(const WCHAR* pwzContanerName = NULL,
                                    bool         bTryRandomNameWhenFailed = true);
        virtual long GetBlockLength(DWORD& dwBlockLen, HCRYPTKEY hAlgKey = NULL);
        virtual long GetKeyLength(DWORD& dwKeylen, HCRYPTKEY hAlgKey = NULL);

        virtual long GetHashData(
            const BYTE* pPlainbataBuf,
            DWORD       dwPlainDataBufLen, 
            CByteBuf&   HashData,
            DWORD       dwHashAlgType = EHAT_UNKNOWN);

    private:
        CCAPIHandleWrap<HCRYPTPROV, EHT_PROV> m_hCryptoProvider;    // watch out the order!! the Key needs to be released before provider instance!!
        CCAPIHandleWrap<HCRYPTKEY, EHT_KEY>   m_hCryptoKey;        
        DWORD            m_dwKeyContainerFlag;
        CCryptoConfig*   m_pCryptoCfg;
        wstring          m_wsKeySetName;
        CGUIDWrap        m_InstanceGUID;
        bool             m_bReleaseConfigMgr;
        bool             m_bKeepContainer;    //ZZ: Not auto-release key container after instance release.

    public:
        static long      g_lInstRefCount;
    };
}
