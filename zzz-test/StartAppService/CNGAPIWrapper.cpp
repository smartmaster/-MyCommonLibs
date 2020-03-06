#include "stdafx.h"
#include "CryptoCommon.h"
#include "CryptoCAPIWrapperImp.h"
#include "CryptoCNGWrapperImp.h"

namespace nsCryptoCNGWrapper
{
#ifndef _USING_CNG_LIB

    #define BCRYPT_DLL_NAME  L"Bcrypt.dll"

    NTSTATUS WINAPI BCryptOpenAlgorithmProvider(
        __out       BCRYPT_ALG_HANDLE   *phAlgorithm,
        __in        LPCWSTR pszAlgId,
        __in_opt    LPCWSTR pszImplementation,
        __in        ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptOpenAlgorithmProvider)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptOpenAlgorithmProvider(phAlgorithm, pszAlgId, pszImplementation, dwFlags);
    }

    NTSTATUS WINAPI BCryptEnumAlgorithms(
        __in    ULONG   dwAlgOperations,
        __out   ULONG   *pAlgCount,
        __out   BCRYPT_ALGORITHM_IDENTIFIER **ppAlgList,
        __in    ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptEnumAlgorithms)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptEnumAlgorithms(dwAlgOperations, pAlgCount, ppAlgList, dwFlags);
    }

    NTSTATUS WINAPI BCryptEnumProviders(
        __in    LPCWSTR pszAlgId,
        __out   ULONG   *pImplCount,
        __out   BCRYPT_PROVIDER_NAME **ppImplList,
        __in    ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptEnumProviders)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptEnumProviders(pszAlgId, pImplCount, ppImplList, dwFlags);
    }

    NTSTATUS WINAPI BCryptGetProperty(
        __in                                        BCRYPT_HANDLE   hObject,
        __in                                        LPCWSTR pszProperty,
        __out_bcount_part_opt(cbOutput, *pcbResult) PUCHAR   pbOutput,
        __in                                        ULONG   cbOutput,
        __out                                       ULONG   *pcbResult,
        __in                                        ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptGetProperty)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptGetProperty(hObject, pszProperty, pbOutput, cbOutput, pcbResult, dwFlags);
    }

    NTSTATUS WINAPI BCryptSetProperty(
        __inout                 BCRYPT_HANDLE   hObject,
        __in                    LPCWSTR pszProperty,
        __in_bcount(cbInput)    PUCHAR   pbInput,
        __in                    ULONG   cbInput,
        __in                    ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptSetProperty)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptSetProperty(hObject, pszProperty, pbInput, cbInput, dwFlags);
    }

    NTSTATUS WINAPI BCryptCloseAlgorithmProvider(
        __inout BCRYPT_ALG_HANDLE   hAlgorithm,
        __in    ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptCloseAlgorithmProvider)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptCloseAlgorithmProvider(hAlgorithm, dwFlags);
    }

    VOID WINAPI BCryptFreeBuffer(__in PVOID pvBuffer)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptFreeBuffer)
            return;
        CCNGAPIManager::g_pfnBCryptFreeBuffer(pvBuffer);
    }

    NTSTATUS WINAPI BCryptGenerateSymmetricKey(
        __inout                             BCRYPT_ALG_HANDLE   hAlgorithm,
        __out                               BCRYPT_KEY_HANDLE   *phKey,
        __out_bcount_full_opt(cbKeyObject)  PUCHAR   pbKeyObject,
        __in                                ULONG   cbKeyObject,
        __in_bcount(cbSecret)               PUCHAR   pbSecret,
        __in                                ULONG   cbSecret,
        __in                                ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptGenerateSymmetricKey)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptGenerateSymmetricKey(hAlgorithm, phKey, pbKeyObject, cbKeyObject, pbSecret, cbSecret, dwFlags);
    }

    NTSTATUS WINAPI BCryptGenerateKeyPair(
        __inout BCRYPT_ALG_HANDLE   hAlgorithm,
        __out   BCRYPT_KEY_HANDLE   *phKey,
        __in    ULONG   dwLength,
        __in    ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptGenerateKeyPair)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptGenerateKeyPair(hAlgorithm, phKey, dwLength, dwFlags);
    }

    NTSTATUS WINAPI BCryptEncrypt(
        __inout                                     BCRYPT_KEY_HANDLE hKey,
        __in_bcount_opt(cbInput)                    PUCHAR   pbInput,
        __in                                        ULONG   cbInput,
        __in_opt                                    VOID    *pPaddingInfo,
        __inout_bcount_opt(cbIV)                    PUCHAR   pbIV,
        __in                                        ULONG   cbIV,
        __out_bcount_part_opt(cbOutput, *pcbResult) PUCHAR   pbOutput,
        __in                                        ULONG   cbOutput,
        __out                                       ULONG   *pcbResult,
        __in                                        ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptEncrypt)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptEncrypt(hKey, 
                                                  pbInput, 
                                                  cbInput, 
                                                  pPaddingInfo, 
                                                  pbIV, 
                                                  cbIV, 
                                                  pbOutput, 
                                                  cbOutput, 
                                                  pcbResult, 
                                                  dwFlags);
    }

    NTSTATUS WINAPI BCryptDecrypt(
        __inout                                     BCRYPT_KEY_HANDLE   hKey,
        __in_bcount_opt(cbInput)                    PUCHAR   pbInput,
        __in                                        ULONG   cbInput,
        __in_opt                                    VOID    *pPaddingInfo,
        __inout_bcount_opt(cbIV)                    PUCHAR   pbIV,
        __in                                        ULONG   cbIV,
        __out_bcount_part_opt(cbOutput, *pcbResult) PUCHAR   pbOutput,
        __in                                        ULONG   cbOutput,
        __out                                       ULONG   *pcbResult,
        __in                                        ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptDecrypt)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptDecrypt(hKey, 
                                                  pbInput,
                                                  cbInput,
                                                  pPaddingInfo,
                                                  pbIV,
                                                  cbIV,
                                                  pbOutput,
                                                  cbOutput,
                                                  pcbResult,
                                                  dwFlags);
    }

    NTSTATUS WINAPI BCryptExportKey(
        __in                                        BCRYPT_KEY_HANDLE   hKey,
        __in_opt                                    BCRYPT_KEY_HANDLE   hExportKey,
        __in                                        LPCWSTR pszBlobType,
        __out_bcount_part_opt(cbOutput, *pcbResult) PUCHAR   pbOutput,
        __in                                        ULONG   cbOutput,
        __out                                       ULONG   *pcbResult,
        __in                                        ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptExportKey)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptExportKey(hKey, hExportKey, pszBlobType, pbOutput, cbOutput, pcbResult, dwFlags);
    }

    NTSTATUS WINAPI BCryptImportKey(
        __in                                BCRYPT_ALG_HANDLE hAlgorithm,
        __in_opt                            BCRYPT_KEY_HANDLE hImportKey,
        __in                                LPCWSTR pszBlobType,
        __out                               BCRYPT_KEY_HANDLE *phKey,
        __out_bcount_full_opt(cbKeyObject)  PUCHAR   pbKeyObject,
        __in                                ULONG   cbKeyObject,
        __in_bcount(cbInput)                PUCHAR   pbInput,
        __in                                ULONG   cbInput,
        __in                                ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptImportKey)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptImportKey(hAlgorithm,
                                                    hImportKey,
                                                    pszBlobType,
                                                    phKey,
                                                    pbKeyObject,
                                                    cbKeyObject,
                                                    pbInput,
                                                    cbInput,
                                                    dwFlags);
    }

    NTSTATUS WINAPI BCryptImportKeyPair(
        __in                            BCRYPT_ALG_HANDLE hAlgorithm,
        __in_opt                        BCRYPT_KEY_HANDLE hImportKey,
        __in                            LPCWSTR pszBlobType,
        __out                           BCRYPT_KEY_HANDLE *phKey,
        __in_bcount(cbInput)            PUCHAR   pbInput,
        __in                            ULONG   cbInput,
        __in                            ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptImportKeyPair)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptImportKeyPair(hAlgorithm, 
                                                        hImportKey, 
                                                        pszBlobType, 
                                                        phKey, 
                                                        pbInput, 
                                                        cbInput, 
                                                        dwFlags);
    }

    NTSTATUS WINAPI BCryptDuplicateKey(
        __in                                BCRYPT_KEY_HANDLE   hKey,
        __out                               BCRYPT_KEY_HANDLE   *phNewKey,
        __out_bcount_full_opt(cbKeyObject)  PUCHAR   pbKeyObject,
        __in                                ULONG   cbKeyObject,
        __in                                ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptDuplicateKey)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptDuplicateKey(hKey, phNewKey, pbKeyObject, cbKeyObject, dwFlags);
    }

    NTSTATUS WINAPI BCryptFinalizeKeyPair(
        __inout BCRYPT_KEY_HANDLE   hKey,
        __in    ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptFinalizeKeyPair)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptFinalizeKeyPair(hKey, dwFlags);
    }

    NTSTATUS WINAPI BCryptDestroyKey(__inout BCRYPT_KEY_HANDLE hKey)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptDestroyKey)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptDestroyKey(hKey);
    }

    NTSTATUS WINAPI BCryptDestroySecret(__inout BCRYPT_SECRET_HANDLE hSecret)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptDestroySecret)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptDestroySecret(hSecret);
    }

    NTSTATUS WINAPI BCryptSignHash(
        __in                                        BCRYPT_KEY_HANDLE   hKey,
        __in_opt                                    VOID    *pPaddingInfo,
        __in_bcount(cbInput)                        PUCHAR   pbInput,
        __in                                        ULONG   cbInput,
        __out_bcount_part_opt(cbOutput, *pcbResult) PUCHAR   pbOutput,
        __in                                        ULONG   cbOutput,
        __out                                       ULONG   *pcbResult,
        __in                                        ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptSignHash)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptSignHash(hKey, 
                                                   pPaddingInfo, 
                                                   pbInput, 
                                                   cbInput, 
                                                   pbOutput, 
                                                   cbOutput, 
                                                   pcbResult, 
                                                   dwFlags);
    }

    NTSTATUS WINAPI BCryptVerifySignature(
        __in                        BCRYPT_KEY_HANDLE   hKey,
        __in_opt                    VOID    *pPaddingInfo,
        __in_bcount(cbHash)         PUCHAR   pbHash,
        __in                        ULONG   cbHash,
        __in_bcount(cbSignature)    PUCHAR   pbSignature,
        __in                        ULONG   cbSignature,
        __in                        ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptVerifySignature)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptVerifySignature(hKey, 
                                                          pPaddingInfo, 
                                                          pbHash,
                                                          cbHash, 
                                                          pbSignature, 
                                                          cbSignature,
                                                          dwFlags);
    }

    NTSTATUS WINAPI BCryptSecretAgreement(
        __in    BCRYPT_KEY_HANDLE       hPrivKey,
        __in    BCRYPT_KEY_HANDLE       hPubKey,
        __out   BCRYPT_SECRET_HANDLE    *phAgreedSecret,
        __in    ULONG                   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptSecretAgreement)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptSecretAgreement(hPrivKey, hPubKey, phAgreedSecret, dwFlags);
    }

    NTSTATUS WINAPI BCryptDeriveKey(
        __in        BCRYPT_SECRET_HANDLE hSharedSecret,
        __in        LPCWSTR              pwszKDF,
        __in_opt    BCryptBufferDesc     *pParameterList,
        __out_bcount_part_opt(cbDerivedKey, *pcbResult) PUCHAR pbDerivedKey,
        __in        ULONG                cbDerivedKey,
        __out       ULONG                *pcbResult,
        __in        ULONG                dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptDeriveKey)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptDeriveKey(hSharedSecret, 
                                                    pwszKDF, 
                                                    pParameterList, 
                                                    pbDerivedKey, 
                                                    cbDerivedKey, 
                                                    pcbResult, 
                                                    dwFlags);
    }

    NTSTATUS WINAPI BCryptCreateHash(
        __inout                             BCRYPT_ALG_HANDLE   hAlgorithm,
        __out                               BCRYPT_HASH_HANDLE  *phHash,
        __out_bcount_full_opt(cbHashObject) PUCHAR   pbHashObject,
        __in                                ULONG   cbHashObject,
        __in_bcount_opt(cbSecret)           PUCHAR   pbSecret,   // optional
        __in                                ULONG   cbSecret,   // optional
        __in                                ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptCreateHash)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptCreateHash(hAlgorithm, 
                                                     phHash, 
                                                     pbHashObject, 
                                                     cbHashObject, 
                                                     pbSecret, 
                                                     cbSecret, 
                                                     dwFlags);
    }

    NTSTATUS WINAPI BCryptHashData(
        __inout                 BCRYPT_HASH_HANDLE  hHash,
        __in_bcount(cbInput)    PUCHAR   pbInput,
        __in                    ULONG   cbInput,
        __in                    ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptHashData)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptHashData(hHash, pbInput, cbInput, dwFlags);
    }

    NTSTATUS WINAPI BCryptFinishHash(
        __inout                     BCRYPT_HASH_HANDLE hHash,
        __out_bcount_full(cbOutput) PUCHAR   pbOutput,
        __in                        ULONG   cbOutput,
        __in                        ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptFinishHash)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptFinishHash(hHash, pbOutput, cbOutput, dwFlags);
    }

    NTSTATUS WINAPI BCryptDuplicateHash(
        __in                                BCRYPT_HASH_HANDLE  hHash,
        __out                               BCRYPT_HASH_HANDLE  *phNewHash,
        __out_bcount_full_opt(cbHashObject) PUCHAR   pbHashObject,
        __in                                ULONG   cbHashObject,
        __in                                ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptDuplicateHash)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptDuplicateHash(hHash, phNewHash, pbHashObject, cbHashObject, dwFlags);
    }

    NTSTATUS WINAPI BCryptDestroyHash(__inout BCRYPT_HASH_HANDLE hHash)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptDestroyHash)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptDestroyHash(hHash);
    }

    NTSTATUS WINAPI BCryptGenRandom(
        __in_opt                        BCRYPT_ALG_HANDLE   hAlgorithm,
        __inout_bcount_full(cbBuffer)   PUCHAR  pbBuffer,
        __in                            ULONG   cbBuffer,
        __in                            ULONG   dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptGenRandom)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptGenRandom(hAlgorithm, pbBuffer, cbBuffer, dwFlags);
    }

    NTSTATUS WINAPI BCryptDeriveKeyCapi(
        __in                            BCRYPT_HASH_HANDLE  hHash,
        __in_opt                        BCRYPT_ALG_HANDLE   hTargetAlg,
        __out_bcount( cbDerivedKey )    PUCHAR              pbDerivedKey,
        __in                            ULONG               cbDerivedKey,
        __in                            ULONG               dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptDeriveKeyCapi)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptDeriveKeyCapi(hHash, hTargetAlg, pbDerivedKey, cbDerivedKey, dwFlags);
    }

    NTSTATUS WINAPI BCryptDeriveKeyPBKDF2(
        __in                            BCRYPT_ALG_HANDLE   hPrf,
        __in_bcount_opt( cbPassword )   PUCHAR              pbPassword,
        __in                            ULONG               cbPassword,
        __in_bcount_opt( cbSalt )       PUCHAR              pbSalt,
        __in                            ULONG               cbSalt,
        __in                            ULONGLONG           cIterations,
        __out_bcount( cbDerivedKey )    PUCHAR              pbDerivedKey,
        __in                            ULONG               cbDerivedKey,
        __in                            ULONG               dwFlags)
    {
        if(NULL == CCNGAPIManager::g_pfnBCryptDeriveKeyPBKDF2)
        {
            ERRTRCEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function in Bcrypt.dll.");
            return D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR;
        }
        return CCNGAPIManager::g_pfnBCryptDeriveKeyPBKDF2(hPrf, 
                                                          pbPassword, 
                                                          cbPassword, 
                                                          pbSalt, 
                                                          cbSalt, 
                                                          cIterations,
                                                          pbDerivedKey, 
                                                          cbDerivedKey, 
                                                          dwFlags);
    }


    HMODULE                         CCNGAPIManager::g_hBcryptDll = NULL;
    long                            CCNGAPIManager::g_lInstRefCount = 0;

    pfnBCryptOpenAlgorithmProvider  CCNGAPIManager::g_pfnBCryptOpenAlgorithmProvider = NULL;
    pfnBCryptEnumAlgorithms         CCNGAPIManager::g_pfnBCryptEnumAlgorithms = NULL;
    pfnBCryptEnumProviders          CCNGAPIManager::g_pfnBCryptEnumProviders = NULL;
    pfnBCryptGetProperty            CCNGAPIManager::g_pfnBCryptGetProperty = NULL;
    pfnBCryptSetProperty            CCNGAPIManager::g_pfnBCryptSetProperty = NULL;
    pfnBCryptCloseAlgorithmProvider CCNGAPIManager::g_pfnBCryptCloseAlgorithmProvider = NULL;
    pfnBCryptFreeBuffer             CCNGAPIManager::g_pfnBCryptFreeBuffer = NULL;
    pfnBCryptGenerateSymmetricKey   CCNGAPIManager::g_pfnBCryptGenerateSymmetricKey = NULL;
    pfnBCryptGenerateKeyPair        CCNGAPIManager::g_pfnBCryptGenerateKeyPair = NULL;
    pfnBCryptEncrypt                CCNGAPIManager::g_pfnBCryptEncrypt = NULL;
    pfnBCryptDecrypt                CCNGAPIManager::g_pfnBCryptDecrypt = NULL;
    pfnBCryptExportKey              CCNGAPIManager::g_pfnBCryptExportKey = NULL;
    pfnBCryptImportKey              CCNGAPIManager::g_pfnBCryptImportKey = NULL;
    pfnBCryptImportKeyPair          CCNGAPIManager::g_pfnBCryptImportKeyPair = NULL;
    pfnBCryptDuplicateKey           CCNGAPIManager::g_pfnBCryptDuplicateKey = NULL;
    pfnBCryptFinalizeKeyPair        CCNGAPIManager::g_pfnBCryptFinalizeKeyPair = NULL;
    pfnBCryptDestroyKey             CCNGAPIManager::g_pfnBCryptDestroyKey = NULL;
    pfnBCryptDestroySecret          CCNGAPIManager::g_pfnBCryptDestroySecret = NULL;
    pfnBCryptSignHash               CCNGAPIManager::g_pfnBCryptSignHash = NULL;
    pfnBCryptVerifySignature        CCNGAPIManager::g_pfnBCryptVerifySignature = NULL;
    pfnBCryptSecretAgreement        CCNGAPIManager::g_pfnBCryptSecretAgreement = NULL;
    pfnBCryptDeriveKey              CCNGAPIManager::g_pfnBCryptDeriveKey = NULL;
    pfnBCryptCreateHash             CCNGAPIManager::g_pfnBCryptCreateHash = NULL;
    pfnBCryptHashData               CCNGAPIManager::g_pfnBCryptHashData = NULL;
    pfnBCryptFinishHash             CCNGAPIManager::g_pfnBCryptFinishHash = NULL;
    pfnBCryptDuplicateHash          CCNGAPIManager::g_pfnBCryptDuplicateHash = NULL;
    pfnBCryptDestroyHash            CCNGAPIManager::g_pfnBCryptDestroyHash = NULL;
    pfnBCryptGenRandom              CCNGAPIManager::g_pfnBCryptGenRandom = NULL;
    pfnBCryptDeriveKeyCapi          CCNGAPIManager::g_pfnBCryptDeriveKeyCapi = NULL;
    pfnBCryptDeriveKeyPBKDF2        CCNGAPIManager::g_pfnBCryptDeriveKeyPBKDF2 = NULL;

    long CCNGAPIManager::Initialization()
    {
        CCNGAPIManager::g_hBcryptDll = LoadLibraryExW(BCRYPT_DLL_NAME, NULL, 0);
        if(NULL == CCNGAPIManager::g_hBcryptDll)
        {
            long lRetCode = GetLastError();
            ERRTRCEX(lRetCode, L"Failed to load library for bcrypt.dll. (EC=[0x%08x], Instance Count=CNG[%u]|CAPI[%u])", 
                lRetCode,
                nsCryptoCNGWrapper::CCryptoCNGWrapper::g_lInstRefCount, 
                nsCryptoCAPIWrapper::CCryptoCAPIWrapper::g_lInstRefCount);
            return lRetCode;
        }

        CCNGAPIManager::g_lInstRefCount = InterlockedIncrement(&CCNGAPIManager::g_lInstRefCount);
        DETLOG(L"Succeed to load bcrypt.dll dynamically. Reference count=%u, CNG=[%u], CAPI=[%u]", 
            CCNGAPIManager::g_lInstRefCount,
            nsCryptoCNGWrapper::CCryptoCNGWrapper::g_lInstRefCount, 
            nsCryptoCAPIWrapper::CCryptoCAPIWrapper::g_lInstRefCount);

        CCNGAPIManager::g_pfnBCryptOpenAlgorithmProvider = (pfnBCryptOpenAlgorithmProvider)GetProcAddress(g_hBcryptDll, "BCryptOpenAlgorithmProvider");
        if(NULL == CCNGAPIManager::g_pfnBCryptOpenAlgorithmProvider)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptOpenAlgorithmProvider");

        CCNGAPIManager::g_pfnBCryptEnumAlgorithms = (pfnBCryptEnumAlgorithms)GetProcAddress(g_hBcryptDll, "BCryptEnumAlgorithms");
        if(NULL == CCNGAPIManager::g_pfnBCryptEnumAlgorithms)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptEnumAlgorithms");

        CCNGAPIManager::g_pfnBCryptEnumProviders = (pfnBCryptEnumProviders)GetProcAddress(g_hBcryptDll, "BCryptEnumProviders");
        if(NULL == CCNGAPIManager::g_pfnBCryptEnumProviders)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptEnumProviders");

        CCNGAPIManager::g_pfnBCryptGetProperty = (pfnBCryptGetProperty)GetProcAddress(g_hBcryptDll, "BCryptGetProperty");
        if(NULL == CCNGAPIManager::g_pfnBCryptGetProperty)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptGetProperty");

        CCNGAPIManager::g_pfnBCryptSetProperty = (pfnBCryptSetProperty)GetProcAddress(g_hBcryptDll, "BCryptSetProperty");
        if(NULL == CCNGAPIManager::g_pfnBCryptSetProperty)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptSetProperty");

        CCNGAPIManager::g_pfnBCryptCloseAlgorithmProvider = (pfnBCryptCloseAlgorithmProvider)GetProcAddress(g_hBcryptDll, "BCryptCloseAlgorithmProvider");
        if(NULL == CCNGAPIManager::g_pfnBCryptCloseAlgorithmProvider)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptCloseAlgorithmProvider");

        CCNGAPIManager::g_pfnBCryptFreeBuffer = (pfnBCryptFreeBuffer)GetProcAddress(g_hBcryptDll, "BCryptFreeBuffer");
        if(NULL == CCNGAPIManager::g_pfnBCryptFreeBuffer)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptFreeBuffer");

        CCNGAPIManager::g_pfnBCryptGenerateSymmetricKey = (pfnBCryptGenerateSymmetricKey)GetProcAddress(g_hBcryptDll, "BCryptGenerateSymmetricKey");
        if(NULL == CCNGAPIManager::g_pfnBCryptGenerateSymmetricKey)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptGenerateSymmetricKey");

        CCNGAPIManager::g_pfnBCryptGenerateKeyPair = (pfnBCryptGenerateKeyPair)GetProcAddress(g_hBcryptDll, "BCryptGenerateKeyPair");
        if(NULL == CCNGAPIManager::g_pfnBCryptGenerateKeyPair)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptGenerateKeyPair");

        CCNGAPIManager::g_pfnBCryptEncrypt = (pfnBCryptEncrypt)GetProcAddress(g_hBcryptDll, "BCryptEncrypt");
        if(NULL == CCNGAPIManager::g_pfnBCryptEncrypt)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptEncrypt");

        CCNGAPIManager::g_pfnBCryptDecrypt = (pfnBCryptDecrypt)GetProcAddress(g_hBcryptDll, "BCryptDecrypt");
        if(NULL == CCNGAPIManager::g_pfnBCryptDecrypt)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptDecrypt");

        CCNGAPIManager::g_pfnBCryptExportKey = (pfnBCryptExportKey)GetProcAddress(g_hBcryptDll, "BCryptExportKey");
        if(NULL == CCNGAPIManager::g_pfnBCryptExportKey)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptExportKey");

        CCNGAPIManager::g_pfnBCryptImportKey = (pfnBCryptImportKey)GetProcAddress(g_hBcryptDll, "BCryptImportKey");
        if(NULL == CCNGAPIManager::g_pfnBCryptImportKey)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptImportKey");

        CCNGAPIManager::g_pfnBCryptImportKeyPair = (pfnBCryptImportKeyPair)GetProcAddress(g_hBcryptDll, "BCryptImportKeyPair");
        if(NULL == CCNGAPIManager::g_pfnBCryptImportKeyPair)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptImportKeyPair");

        CCNGAPIManager::g_pfnBCryptDuplicateKey = (pfnBCryptDuplicateKey)GetProcAddress(g_hBcryptDll, "BCryptDuplicateKey");
        if(NULL == CCNGAPIManager::g_pfnBCryptDuplicateKey)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptDuplicateKey");

        CCNGAPIManager::g_pfnBCryptFinalizeKeyPair = (pfnBCryptFinalizeKeyPair)GetProcAddress(g_hBcryptDll, "BCryptFinalizeKeyPair");
        if(NULL == CCNGAPIManager::g_pfnBCryptFinalizeKeyPair)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptFinalizeKeyPair");

        CCNGAPIManager::g_pfnBCryptDestroyKey = (pfnBCryptDestroyKey)GetProcAddress(g_hBcryptDll, "BCryptDestroyKey");
        if(NULL == CCNGAPIManager::g_pfnBCryptDestroyKey)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptDestroyKey");

        CCNGAPIManager::g_pfnBCryptDestroySecret = (pfnBCryptDestroySecret)GetProcAddress(g_hBcryptDll, "BCryptDestroySecret");
        if(NULL == CCNGAPIManager::g_pfnBCryptDestroySecret)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptDestroySecret");

        CCNGAPIManager::g_pfnBCryptSignHash = (pfnBCryptSignHash)GetProcAddress(g_hBcryptDll, "BCryptSignHash");
        if(NULL == CCNGAPIManager::g_pfnBCryptSignHash)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptSignHash");

        CCNGAPIManager::g_pfnBCryptVerifySignature = (pfnBCryptVerifySignature)GetProcAddress(g_hBcryptDll, "BCryptVerifySignature");
        if(NULL == CCNGAPIManager::g_pfnBCryptVerifySignature)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptVerifySignature");

        CCNGAPIManager::g_pfnBCryptSecretAgreement = (pfnBCryptSecretAgreement)GetProcAddress(g_hBcryptDll, "BCryptSecretAgreement");
        if(NULL == CCNGAPIManager::g_pfnBCryptSecretAgreement)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptSecretAgreement");

        CCNGAPIManager::g_pfnBCryptDeriveKey = (pfnBCryptDeriveKey)GetProcAddress(g_hBcryptDll, "BCryptDeriveKey");
        if(NULL == CCNGAPIManager::g_pfnBCryptDeriveKey)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptDeriveKey");

        CCNGAPIManager::g_pfnBCryptCreateHash = (pfnBCryptCreateHash)GetProcAddress(g_hBcryptDll, "BCryptCreateHash");
        if(NULL == CCNGAPIManager::g_pfnBCryptCreateHash)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptCreateHash");

        CCNGAPIManager::g_pfnBCryptHashData = (pfnBCryptHashData)GetProcAddress(g_hBcryptDll, "BCryptHashData");
        if(NULL == CCNGAPIManager::g_pfnBCryptHashData)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptHashData");

        CCNGAPIManager::g_pfnBCryptFinishHash = (pfnBCryptFinishHash)GetProcAddress(g_hBcryptDll, "BCryptFinishHash");
        if(NULL == CCNGAPIManager::g_pfnBCryptFinishHash)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptFinishHash");

        CCNGAPIManager::g_pfnBCryptDuplicateHash = (pfnBCryptDuplicateHash)GetProcAddress(g_hBcryptDll, "BCryptDuplicateHash");
        if(NULL == CCNGAPIManager::g_pfnBCryptDuplicateHash)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptDuplicateHash");

        CCNGAPIManager::g_pfnBCryptDestroyHash = (pfnBCryptDestroyHash)GetProcAddress(g_hBcryptDll, "BCryptDestroyHash");
        if(NULL == CCNGAPIManager::g_pfnBCryptDestroyHash)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptDestroyHash");

        CCNGAPIManager::g_pfnBCryptGenRandom = (pfnBCryptGenRandom)GetProcAddress(g_hBcryptDll, "BCryptGenRandom");
        if(NULL == CCNGAPIManager::g_pfnBCryptGenRandom)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptGenRandom");

        CCNGAPIManager::g_pfnBCryptDeriveKeyCapi = (pfnBCryptDeriveKeyCapi)GetProcAddress(g_hBcryptDll, "BCryptDeriveKeyCapi");
        if(NULL == CCNGAPIManager::g_pfnBCryptDeriveKeyCapi)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptDeriveKeyCapi");

        CCNGAPIManager::g_pfnBCryptDeriveKeyPBKDF2 = (pfnBCryptDeriveKeyPBKDF2)GetProcAddress(g_hBcryptDll, "BCryptDeriveKeyPBKDF2");
        if(NULL == CCNGAPIManager::g_pfnBCryptDeriveKeyPBKDF2)
            DETLOGEX(D2DCOMM_E_UNABLE_TO_FIND_FUNC_ADDR, L"Unable to find function address in Bcrypt.dll. (FnName=[%s])", L"BCryptDeriveKeyPBKDF2");

        return 0;
    }

    long CCNGAPIManager::UnInitialization()
    {
        //ZZ: Current we will release any CNG APIs because the these APIs will be used for all CNG calling.
        //    As a result, there will be one handle taken when use CNG.
        return 0;

        CCNGAPIManager::g_lInstRefCount = InterlockedDecrement(&CCNGAPIManager::g_lInstRefCount);

        if (0 != CCNGAPIManager::g_lInstRefCount)
        {
            DETTRC(L"No need to release CNG APIs and free bcrypt.dll because reference count is %u.", CCNGAPIManager::g_lInstRefCount);
            return 0;
        }
    
        if(CCNGAPIManager::g_hBcryptDll)
        {
            DETLOG(L"Free library for bcrypt.dll.");
            if(!FreeLibrary(CCNGAPIManager::g_hBcryptDll))
            {
                long lRetCode = GetLastError();
                ERRTRCEX(lRetCode, L"Failed to free library for bcrypt.dll. (EC=[0x%08x])", lRetCode);
            }
            CCNGAPIManager::g_hBcryptDll = NULL;
        }

        DETLOG(L"Release all CNG APIs and free bcrypt.dll. Instance count: CNG=[%u], CAPI=[%u]", 
            nsCryptoCNGWrapper::CCryptoCNGWrapper::g_lInstRefCount, 
            nsCryptoCAPIWrapper::CCryptoCAPIWrapper::g_lInstRefCount);

        CCNGAPIManager::g_pfnBCryptOpenAlgorithmProvider = NULL;
        CCNGAPIManager::g_hBcryptDll = NULL;
        CCNGAPIManager::g_pfnBCryptEnumAlgorithms = NULL;
        CCNGAPIManager::g_pfnBCryptEnumProviders = NULL;
        CCNGAPIManager::g_pfnBCryptGetProperty = NULL;
        CCNGAPIManager::g_pfnBCryptSetProperty = NULL;
        CCNGAPIManager::g_pfnBCryptCloseAlgorithmProvider = NULL;
        CCNGAPIManager::g_pfnBCryptFreeBuffer = NULL;
        CCNGAPIManager::g_pfnBCryptGenerateSymmetricKey = NULL;
        CCNGAPIManager::g_pfnBCryptGenerateKeyPair = NULL;
        CCNGAPIManager::g_pfnBCryptEncrypt = NULL;
        CCNGAPIManager::g_pfnBCryptDecrypt = NULL;
        CCNGAPIManager::g_pfnBCryptExportKey = NULL;
        CCNGAPIManager::g_pfnBCryptImportKey = NULL;
        CCNGAPIManager::g_pfnBCryptImportKeyPair = NULL;
        CCNGAPIManager::g_pfnBCryptDuplicateKey = NULL;
        CCNGAPIManager::g_pfnBCryptFinalizeKeyPair = NULL;
        CCNGAPIManager::g_pfnBCryptDestroyKey = NULL;
        CCNGAPIManager::g_pfnBCryptDestroySecret = NULL;
        CCNGAPIManager::g_pfnBCryptSignHash = NULL;
        CCNGAPIManager::g_pfnBCryptVerifySignature = NULL;
        CCNGAPIManager::g_pfnBCryptSecretAgreement = NULL;
        CCNGAPIManager::g_pfnBCryptDeriveKey = NULL;
        CCNGAPIManager::g_pfnBCryptCreateHash = NULL;
        CCNGAPIManager::g_pfnBCryptHashData = NULL;
        CCNGAPIManager::g_pfnBCryptFinishHash = NULL;
        CCNGAPIManager::g_pfnBCryptDuplicateHash = NULL;
        CCNGAPIManager::g_pfnBCryptDestroyHash = NULL;
        CCNGAPIManager::g_pfnBCryptGenRandom = NULL;
        CCNGAPIManager::g_pfnBCryptDeriveKeyCapi = NULL;
        CCNGAPIManager::g_pfnBCryptDeriveKeyPBKDF2 = NULL;

        return 0;
    }
#endif
}

