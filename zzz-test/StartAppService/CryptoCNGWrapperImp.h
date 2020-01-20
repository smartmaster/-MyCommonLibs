#pragma once
#include "CryptoCommon.h"
#include "CNGAPIWrapper.h"

#include <vector>

namespace nsCryptoCNGWrapper
{
    class CAlgorithmIdentifier
    {
    public:
        wstring m_wsAlgName;
        DWORD   m_dwAlgClass;
        DWORD   m_dwAlgFlag;
    };
    typedef std::vector<CAlgorithmIdentifier> AlgIDVector;

    class CAlgorithmProviderIdentifier
    {
    public:
        wstring m_wsProvName;
        wstring m_wsAlgID;
    };
    typedef std::vector<CAlgorithmProviderIdentifier> AlgProvVector;

    class CCryptoCNGWrapper : public ICryptoWrapperInterface
    {
    public:
        CCryptoCNGWrapper(const CCryptoParam* pCryptoParam = NULL, CCryptoConfig* pCryptoCfg = NULL);

        virtual ~CCryptoCNGWrapper();

        virtual long Initialize(const CCryptoParam* pCryptoParam);

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

        virtual E_CRYPTOAPI_TYPE CryptoAPIType() { return ECAT_CNG; }

        virtual long GetInstanceCount(DWORD& dwInstRefCount);

    protected:
        virtual long GetObjectLength(DWORD& dwObjLen, BCRYPT_ALG_HANDLE hAlgHandle = NULL);
        virtual long GetBlockLength(DWORD& dwBlockLen, BCRYPT_ALG_HANDLE hAlgHandle = NULL);
        virtual long GetHashValueLength(DWORD& dwHashValLen, BCRYPT_ALG_HANDLE hAlgHandle);
        virtual long GetExportKeyLength(DWORD& dwExportKeyLen, BCRYPT_KEY_HANDLE hCryptoKey = NULL);
        virtual long GetIVBuffer(CByteBuf& IVBuffer); // Initialization vector, will be changed when encryption.
        virtual long SetEncCBC(BCRYPT_ALG_HANDLE hAlgHandle = NULL); //Cipher Block Chaining 
        virtual long GetKeyLength(DWORD& dwKeylen, BCRYPT_KEY_HANDLE hCryptoKey = NULL);
        virtual long GetSupportedKeyLength(BCRYPT_KEY_LENGTHS_STRUCT& stKeyLength, BCRYPT_ALG_HANDLE hAlgHandle = NULL);
        virtual long GetHashData(
            const BYTE* pPlainbataBuf,
            DWORD       dwPlainDataBufLen, 
            CByteBuf&   HashData,
            DWORD       dwHashAlgType = EHAT_UNKNOWN);

        virtual long GetSecretInfo(CByteBuf& SecretInfo, const PBYTE pbInKeyInfo, DWORD dwKeyInfoSize, BCRYPT_ALG_HANDLE hAlgHandle = NULL);
        virtual long GetRegisteredAlgorithm(DWORD dwAlgOperationType, AlgIDVector& vecAlgID);
        virtual long GetRegisteredAlgorithmProvider(const WCHAR* pwzAlgID, AlgProvVector& vecAlgProv, wstring* pwsDefaultProv = NULL);

    private:
        CCNGHandleWrap<BCRYPT_KEY_HANDLE, EHT_KEY>   m_hCryptoCNGKey;
        CCNGHandleWrap<BCRYPT_ALG_HANDLE, EHT_PROV>  m_hCryptoCNGAlg;
        CCryptoConfig*                               m_pCryptoCfg;;
        CGUIDWrap                                    m_InstanceGUID;
        bool                                         m_bReleaseConfigMgr;

    public:
        static long                                  g_lInstRefCount;
    };
}
