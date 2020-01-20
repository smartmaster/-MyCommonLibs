#pragma once
#include "CryptoDefine.h"
#include <Windows.h>
#include <string>
#include <vector>
using namespace std;

#ifdef CRYPTOWRAPPERDLL_EXPORTS
//#define VSSWRAPPERDLL_API __declspec(dllexport)
#define VSSWRAPPERDLL_API 
#else
//#define VSSWRAPPERDLL_API __declspec(dllimport)
#define VSSWRAPPERDLL_API 
#endif

typedef enum _eEncLibType
{
    ELT_NOENCRYTION = 0,   //ZZ: Not encryption.
    ELT_MSCRYPTO,          //ZZ: (Default) Encryption using library provided by MS. CAPI or CNG.
    ELT_ETPKI              //ZZ: Encryption using library provided by CA. Not supported now.
}E_ENCLIB_TYPE;

typedef enum _eEncAlgorithmType
{
    EEAT_UNKNOWN = 0,      //ZZ: Unknown algorithm, Not encryption.
    EEAT_AES_128BIT,       //ZZ: (Default) AES-128, Fips only require key size 128, 192 and 256 of AES,
    EEAT_AES_192BIT,       //ZZ: AES-192
    EEAT_AES_256BIT,       //ZZ: AES-256
    EEAT_3DES              //ZZ: Triple-DES. Not supported now.
}E_ENCALG_TYPE;

typedef enum _eHashAlgorithmType
{
    EHAT_UNKNOWN = 0,      //ZZ: Unknown hash algorithm.
    EHAT_MD5,              //ZZ: (Default) MD5 algorithm.
    EHAT_SHA1,             //ZZ: SHA-1 algorithm (160 bits).
    EHAT_SHA_256,          //ZZ: Belong to SHA-2 Supported on XP-SP3 and later  
    EHAT_SHA_384,          //ZZ: Belong to SHA-2 Supported on XP-SP3 and later  
    EHAT_SHA_512           //ZZ: Belong to SHA-2 Supported on XP-SP3 and later
}E_HASHALG_TYPE;

typedef enum _eCryptoAPIType
{
    ECAT_UNKNOWN = 0,      //ZZ: Unknown API type. Should not exist.
    ECAT_BY_OS,            //ZZ: (Default) Decided by OS version. Using CNG on win7 and w2k8 r2, while using CAPI on others.
    ECAT_CAPI,             //ZZ: Cryptography API to encrypt/decrypt.
    ECAT_CNG,              //ZZ: Next Generation Cryptography API(CNG) to encrypt/decrypt.
}E_CRYPTOAPI_TYPE;

typedef enum _eIVUsageStatus
{
    EIU_UNKNOWN = 0,        //ZZ: Unknown status. Should not exist.
    EIU_NOT_USE,            //ZZ: (Default) Do not use initialization vector.
    EIU_USE_IV              //ZZ: Use default initialization vector.
}E_IV_USAGE;

typedef enum _eInterfaceType
{
    EIT_UNKNOWN = 0,        //ZZ: Unknown type. Should not exist.
    EIT_CRYPTO = 0x01,      //ZZ: Create instance for encryption. Only with this flag, instance can be used for encryption
    EIT_HASH = 0x02,        //ZZ: Create hash instance. Only with flag, initialization for cryptography will not be performed.
    EIT_CRYPRO_HASH = (EIT_CRYPTO | EIT_HASH) //ZZ: (Default) Instance can be used for enctyption and hash.
}E_INTER_TYPE;

typedef enum _eStrOption
{
    ESEO_BASE64               = CRYPT_STRING_BASE64,
    ESEO_BASE64_HEADER        = CRYPT_STRING_BASE64HEADER,
    ESEO_BASE64_REQUESTHEADER = CRYPT_STRING_BASE64REQUESTHEADER,
    ESEO_HEX                  = CRYPT_STRING_HEX,    //ZZ: Convert hex bin to string and start new line when a multiple of 16 bytes.
    ESEO_HEX_ASCII            = CRYPT_STRING_HEXASCII,
    ESEO_HEX_ADDR             = CRYPT_STRING_HEXADDR,
    ESEO_HEX_ASCII_ADDR       = CRYPT_STRING_HEXASCIIADDR,
    ESEO_HEX_RAW              = CRYPT_STRING_HEXRAW, //ZZ: Ignored on w2k3 and xp. Can be bitwise or with other option.
    ESEO_STR_NOCRLF           = CRYPT_STRING_NOCRLF, //ZZ: Ignored on w2k3 and xp. Can be bitwise or with other option.
    ESEO_STR_NOCR             = CRYPT_STRING_NOCR,
    ESEO_BASE64_ANY           = CRYPT_STRING_BASE64_ANY, //ZZ: Try such order: CRYPT_STRING_BASE64HEADER, CRYPT_STRING_BASE64
    ESEO_HEX_ANY              = CRYPT_STRING_HEX_ANY //ZZ: Try such order: CRYPT_STRING_HEXADDR, CRYPT_STRING_HEXASCIIADDR, CRYPT_STRING_HEX, CRYPT_STRING_HEXRAW, CRYPT_STRING_HEXASCII  
}E_STR_OPT;

typedef enum _eQueryKeySetCtrl
{
    EQKC_ALL = 0,
    EQKC_D2D_ONLY,
    EQKC_D2D_EXCLUDED
}E_QUERY_KEYSET_CTRL;

class CCryptoParam
{
public:
    CCryptoParam() 
        : m_dwHashAlgType(EHAT_UNKNOWN),
          m_dwEncAlgType(EEAT_UNKNOWN),
          m_dwEncLibType(ELT_NOENCRYTION),
          m_dwEncAPIType(ECAT_UNKNOWN),
          m_dwInterfaceType(EIT_CRYPRO_HASH),
          m_dwIVUsage(EIU_UNKNOWN)
    {
    }

    bool operator == (const CCryptoParam& obj)
    {
        if((m_dwHashAlgType == obj.m_dwHashAlgType) &&
           (m_dwEncAlgType == obj.m_dwEncAlgType) && 
           (m_dwEncLibType == obj.m_dwEncLibType) &&
           (m_dwEncAPIType == obj.m_dwEncAPIType) &&
           (m_wsPassword == obj.m_wsPassword) &&
           (m_dwInterfaceType == obj.m_dwInterfaceType) &&
           (m_dwIVUsage == obj.m_dwIVUsage))
           return true;
        return false;
    }

    DWORD   m_dwEncLibType;     //ZZ: Possible values defined in E_ENCLIB_TYPE.
    DWORD   m_dwHashAlgType;    //ZZ: Possible values defined in E_HASHALG_TYPE.
    DWORD   m_dwEncAPIType;     //ZZ: Possible values defined in E_CRYPTOAPI_TYPE.
    DWORD   m_dwEncAlgType;     //ZZ: Possible values defined in E_ENCALG_TYPE.
    DWORD   m_dwInterfaceType;  //ZZ: Possible values defined in E_INTER_TYPE.
    DWORD   m_dwIVUsage;        //ZZ: Possible values defined in E_IV_USAGE.
    wstring m_wsPassword;       //ZZ: Password string used for generating encryption key. Can be empty.
};

class CByteBuf
{
public:
    CByteBuf(DWORD dwBufSize = 0) : m_pByteBuf(NULL), m_dwBufSize(0) { AllocMem(dwBufSize); }
    ~CByteBuf() { AllocMem(0); }

    CByteBuf& operator = (const CByteBuf& ByteBuf)
    {
        if(this != &ByteBuf)
        {
            AllocMem(ByteBuf.m_dwBufSize);
            memcpy(m_pByteBuf, ByteBuf.m_pByteBuf, m_dwBufSize);
        }
        return *this;
    }

    bool IsValid() { return (m_pByteBuf && (0 != m_dwBufSize)); }

    void AllocMem(DWORD dwBufSize)
    {
        if(dwBufSize)
        {
            if((NULL == m_pByteBuf) || (dwBufSize > m_dwBufSize))
            {
                PBYTE pbTemp = new BYTE[dwBufSize];
                if(pbTemp)
                {
                    if(m_pByteBuf)
                        delete []m_pByteBuf;
                    m_dwBufSize = dwBufSize;
                    m_pByteBuf = pbTemp;
                    memset(m_pByteBuf, 0, m_dwBufSize);
                }
            }
            else
            {
                memset(m_pByteBuf, 0, m_dwBufSize);
                m_dwBufSize = dwBufSize;
            }
        }
        else if(m_pByteBuf)
        {
            delete []m_pByteBuf;
            m_pByteBuf = NULL;
            m_dwBufSize = 0;
        }
    }

    BYTE* m_pByteBuf;
    DWORD m_dwBufSize;
};

class ICryptoWrapperInterface
{
public:
    /**
     * Initialize encryption instance, including create object and create encryption key(if required).
     * @Param  pCryptoParam Specify parameters focus on, others will be filled using default value. If it is NUll,
     *                      all parameters will be used as default value.
     * @Return If succeed return zero, while failed return error code defined in D2DCryptoError.h
     */
    virtual long Initialize(
        IN const CCryptoParam* pCryptoParam
        ) = 0;

    /**
     * Encrypt data using algorithm specified when initialization.
     * This function will be failed with D2DCRYPTO_E_MORE_DATA and return required data buffer size when
     * the buffer size for encryption is not large enough. Caller must make sure data size to be encrypted.
     * is a multiple of block size, otherwise encryption will be failed.
     * @param  pPlainbataBuf       Input data buffer to be encrypted, Must NOT be NULL. 
     * @param  dwPlainDataBufLen   Input data buffer size(in byte) to be encrypted. Must NOT be zero.
     * @param  pbCipherDataBuf     Encrypted data buffer. If equal to NULL, pdwCipherDataBufLen stores required size.
     * @param  pdwCipherDataBufLen Must NOT be NULL, Input: encrypted data buffer size(in byte), if not large enough 
     *                             return required size; Output: required buffer size or process data size actually.   
     * @param  bPadding            [true] means allow algorithm to append padding for input data, will cause encryped 
     *                             data large than original data. [false] means not allow algorithm to append padding. 
     *                             Caller should make sure original data size is a multiple of block size.
     * @Return If succeed return zero, while failed return error code defined in D2DCryptoError.h
     */
    virtual long EncryptData(
        IN     const BYTE* pPlainbataBuf,
        IN     DWORD       dwPlainDataBufLen,
        OUT    BYTE*       pbCipherDataBuf,
        IN OUT DWORD*      pdwCipherDataBufLen,
        IN OPT bool        bPadding = false
        ) = 0;

    /**
     * Decrypt data using algorithm specified when initialization.
     * This function will be failed with D2DCRYPTO_E_MORE_DATA and return required data buffer size when
     * the buffer size for decryption is not large enough. Caller must make sure data size to be encrypted.
     * is a multiple of block size, otherwise encryption will be failed.
     * @param  pbCipherDataBuf     Input data buffer to be decrypted, Must NOT be NULL. 
     * @param  dwCipherDataBufLen  Input data buffer size(in byte) to be decrypted. Must NOT be zero.
     * @param  pPlainbataBuf       Decrypted data buffer. If equal to NULL, pdwPlainDataBufLen stores required size.
     * @param  pdwPlainDataBufLen  Must NOT be NULL, Input: decrypted data buffer size(in byte), if not large enough 
     *                             return required size; Output: required buffer size or process data size actually.   
     * @param  bPadding            [true] means algorithm will trim data as padding format(PKCS#5). This may cause
     *                             unexpected loss of data if algorithm doesn't append padding. [false] means not 
     *                             allow algorithm to trim padding. 
     * @Return If succeed return zero, while failed return error code defined in D2DCryptoError.h
     */
    virtual long DecryptData(
        IN     const BYTE* pbCipherDataBuf,
        IN     DWORD       dwCipherDataBufLen, 
        OUT    BYTE*       pPlainbataBuf, 
        IN OUT DWORD*      pdwPlainDataBufLen,
        IN OPT bool        bPadding = false
        ) = 0;

    /**
     * Get hash value for input data buffer.
     * @param  pPlainbataBuf      Input data buffer to be calculated for hash, Must NOT be NULL. 
     * @param  dwPlainDataBufLen  Input data buffer size(in byte) .Must NOT be zero.
     * @param  pHashbataBuf       Data buffer for hash value. If equal to NULL, pdwHashDataBufLen stores required size.
     * @param  pdwHashDataBufLen  Must NOT be NULL, Input: hash value buffer size(in byte), if not large enough return 
     * @Param  dwHashAlgType      Reserved. Modify it only when you exactly know if CSP created support it.
     *                            required size; Output: required buffer size or process data size actually.   
     * @Return If succeed return zero, while failed return error code defined in D2DCryptoError.h
     */
    virtual long GetHashData(
        IN     const BYTE* pPlainbataBuf,
        IN     DWORD       dwPlainDataBufLen, 
        OUT    BYTE*       pHashbataBuf,
        IN OUT DWORD*      pdwHashDataBufLen,
        IN OPT DWORD       dwHashAlgType = EHAT_UNKNOWN
        ) = 0;

    /**
     * Get hash value for input data buffer.
     * The hash value will be convert to a string being composed of HEX characters. 
     * @param  pPlainbataBuf      Input data buffer to be calculated for hash, Must NOT be NULL. 
     * @param  dwPlainDataBufLen  Input data buffer size(in byte) .Must NOT be zero.
     * @param  wsHashStr          Store hash value string.
     * @Param  dwHashAlgType      Reserved. Modify it only when you exactly know if CSP created support it.
     * @Return If succeed return zero, while failed return error code defined in D2DCryptoError.h
     */
    virtual long GetHashData(
        IN  const BYTE* pPlainbataBuf,
        IN  DWORD       dwPlainDataBufLen, 
        OUT wstring&    wsHashStr,
        IN OPT DWORD    dwHashAlgType = EHAT_UNKNOWN
        ) = 0;

    /**
     * Get size of hash value.
     * @param  dwHashSize       Return size of hash value.
     * @Param  dwHashAlgType    Reserved. Modify it only when you exactly know if CSP created support it.
     * @Return If succeed return zero, while failed return error code defined in D2DCryptoError.h
     */
    virtual long GetHashValueSize(
        OUT DWORD&      dwHashSize,
        IN OPT DWORD    dwHashAlgType = EHAT_UNKNOWN
        ) = 0;

    /**
     * Get block size of specified algorithm.
     * @param  dwBlockSize Return block size of encryption algorithm.
     * @Return If succeed return zero, while failed return error code defined in D2DCryptoError.h
     */
    virtual long GetAlgorithmBlockSize(
        OUT DWORD& dwBlockSize
        ) = 0;

    /**
     * Each instance will be assigned a GUID for identity. This API will return a GUID string.
     * If the pguidInstance is set, the FUID object will be returned.
     */
    virtual const WCHAR* InstanceGUID(GUID* pguidInstance = NULL) = 0;

    /**
     * Return the crypto API used actually for implement. 
     */
    virtual E_CRYPTOAPI_TYPE CryptoAPIType() = 0;

    /**
     * Get parameter used for current encryption instance.
     * @Return return a copy of current parameter used for this instance.
     */
    virtual CCryptoParam CryptoParam() const { return m_CryptoParam; }
    
    /**
     * Virtual destructor to make sure decried class release correctly.
     */
    virtual ~ICryptoWrapperInterface() {}

    /**
     * Get the instance or reference count for this kind of crypto instance.
     */
    virtual long GetInstanceCount(
        OUT DWORD& dwInstRefCount
        ) = 0;

protected:
    CCryptoParam m_CryptoParam;   //ZZ: Current parameter for encryption instance.
};

/**
 * To use MSXNL for dealing with configuration file, I call CoInitialize and CoUnInitialize in 
 * constructor and destructor. Be careful, there is an restriction for COM initialization in 
 * APARTMENT mode: [the first thread in the application that calls CoInitialize(0) or 
 * CoInitializeEx(COINIT_APARTMENTTHREADED) must be the last thread to call CoUninitialize(). 
 * If the call sequence is not in this order, then subsequent calls to CoInitialize on the STA 
 * will fail and the application will not work.]. When you use this crypto library please make
 * sure such things:
 * 1. Make sure main thread call CoInitialize before creating crypto instance.
 * 2. (OR) Make sure the first thread create crypto instance exits lastly if #1 thing is missed.
 */
class VSSWRAPPERDLL_API ICryptoWrapFactory
{
public:
    /**
     * Static member method to create a instance for encryption and hash.
     * @param  CryptoInfo      Input parameter to indicate behavior of instance.
     * @param  CreateNewAlways If create a new instance or reuse exist instance. Currently it will create a new always.
     * @param  plErrCode       Error code for indicating error reason. Defined in D2DCryptoError.h
     * @Return If succeed return the instance pointer created. If any error return NULL. 
     */
    static ICryptoWrapperInterface* CryptoWrapInstance(
        IN      CCryptoParam& CryptoInfo, 
        IN OPT  bool CreateNewAlways = true, 
        OUT OPT long* plErrCode = NULL
        );

    /**
     * Static member method to release exist instance.
     * @param  ppCryptoWrapInterface Pointer to instance pointer. After release this pointer is set to NULL
     * @Return void
     */
    static void ReleaseCryptoWrapInstance(
        IN ICryptoWrapperInterface** ppCryptoWrapInterface
        );

    /**
     * Static member method to convert a unicode string to binary.
     * The unicode string should be as such format: base64(both contain header and not), HEX string.
     * @param  pbBinary     Binary buffer for converted string, If equal to NULL, pdwBinSize return required size. 
     * @param  pdwBinSize   Binary buffer size. Must NOT be NULL. if this size is not large enough, return required size.
     * @param  pwzString    Pointer to unicode string to convert. Must NOT be NULL
     * @param  dwStrSize    In CHARACTERS, not including terminating null character.
     * @param  dwFlags      Flags for indicating how to convert, defined in enumerate type E_STR_OPT
     * @param  pdwSkipSize  Return How many characters have been skipped to find real data. (skip header~)
     * @param  pdwFlagsUsed Flags used for converting. if dwFlags is ESEO_BASE64_ANY or ESEO_HEX_ANY, this parameter will
     *                      return the flag used actually.
     * @Return If succeed return zero, while failed return error code defined in D2DCryptoError.h
     * @see    E_STR_OPT
     */
    static long WString2Binary(
        OUT     PBYTE        pbBinary,
        IN OUT  DWORD*       pdwBinSize,
        IN      const WCHAR* pwzString,
        IN      DWORD        dwStrSize,
        IN OPT  DWORD        dwFlags = ESEO_BASE64_ANY,
        OUT OPT DWORD*       pdwSkipSize = NULL,
        OUT OPT DWORD*       pdwFlagsUsed = NULL
        );

    /**
     * Static member method to convert a binary to unicode string.
     * The unicode string should be as such format: base64(both contain header and not), HEX string.
     * @param  pwzString  Pointer to unicode string after converting. If equal to NULL, pdwStrSize return required size.
     * @param  pdwStrSize Must NOT be NULL. In CHARACTERS, including terminating null character, to indicate string length 
     *                    after converting. If it is not large enough, return D2DCRYPTO_E_MORE_DATA and pdwStrSize store required size.
     * @param  pbBinary   Binary buffer to be converted, Must NOT be NULL. 
     * @param  dwBinSize  Binary buffer size. Must NOT be zero.
     * @param  dwFlags    Flags for indicating how to convert, defined in enumerate type E_STR_OPT
     * @Return If succeed return zero, while failed return error code defined in D2DCryptoError.h
     * @see    E_STR_OPT
     */
    static long Binary2StringW(
        OUT    WCHAR*      pwzString,
        IN OUT DWORD*      pdwStrSize,
        IN     const PBYTE pbBinary,
        IN     DWORD       dwBinSize,
        IN OPT DWORD       dwFlags = ESEO_BASE64
        );

    /**
     * Enumerate key container in specified CSP based on input encryption parameter.
     * @param  vecContainerName Receive all key container names.
     * @param  CryptoInfo       Input crypto information which decide the CSP.
     * @Param  eQueryCtrl       Which kind of container will be returned.
     * @param  bSkipSelf        If skip the container name for current instance.
     * @Return If succeed return zero, while failed return error code defined in D2DCryptoError.h
     */
    static long EnumKeyContainerInCSP(
        OUT    std::vector<std::wstring>& vecContainerName,
        IN OPT E_QUERY_KEYSET_CTRL        eQueryCtrl = EQKC_D2D_ONLY,
        IN OPT bool                       bSkipSelf = true,
        IN OPT CCryptoParam*              pCryptoInfo = NULL);

    /**
     * Remove key container in specified CSP based on input vector or results searched.
     * @param  pvecContainerName All container in vector will be remove, If it is NULL, all results searched will be removed.
     * @param  CryptoInfo       Input crypto information which decide the CSP.
     * @Param  eQueryCtrl       Which kind of container will be returned.
     * @Return If succeed return zero, while failed return error code defined in D2DCryptoError.h
     */
    static long RemoveKeyContainerInCSP(
        IN OPT std::vector<std::wstring>* pvecContainerName = NULL,
        IN OPT E_QUERY_KEYSET_CTRL        eQueryCtrl = EQKC_D2D_ONLY,
        IN OPT CCryptoParam*              pCryptoInfo = NULL);

    /**
     * Create empty key container in specified CSP based on input encryption parameter.
     * @param  pwzContainerName Container name will be created.
     * @param  bTryRandomName   When it is unable to create specified key container, if try random name.
     * @param  CryptoInfo       Input crypto information which decide the CSP.
     * @param  pwsActualContianerName The actual contianer name created.
     * @Return If succeed return zero, while failed return error code defined in D2DCryptoError.h
     */
    static long CreateKeyContainerInCSP(
        IN OPT  const WCHAR*  pwzContainerName = NULL,
        IN OPT  bool          bTryRandomName = true,
        IN OPT  CCryptoParam* pCryptoInfo = NULL,
        OUT OPT wstring*      pwsActualContianerName = NULL);
};


