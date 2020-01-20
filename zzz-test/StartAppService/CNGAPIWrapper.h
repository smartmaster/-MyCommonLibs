#pragma once

//#define _USING_CNG_LIB

namespace nsCryptoCNGWrapper
{
#include <Windows.h>
#include "bcrypt.h"

#ifdef _USING_CNG_LIB
#pragma comment(lib, "bcrypt.lib")
#else
    typedef NTSTATUS (WINAPI *pfnBCryptOpenAlgorithmProvider)(
        __out       BCRYPT_ALG_HANDLE   *phAlgorithm,
        __in        LPCWSTR pszAlgId,
        __in_opt    LPCWSTR pszImplementation,
        __in        ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptEnumAlgorithms)(
        __in    ULONG   dwAlgOperations,
        __out   ULONG   *pAlgCount,
        __out   BCRYPT_ALGORITHM_IDENTIFIER **ppAlgList,
        __in    ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptEnumProviders)(
        __in    LPCWSTR pszAlgId,
        __out   ULONG   *pImplCount,
        __out   BCRYPT_PROVIDER_NAME **ppImplList,
        __in    ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptGetProperty)(
        __in                                        BCRYPT_HANDLE   hObject,
        __in                                        LPCWSTR pszProperty,
        __out_bcount_part_opt(cbOutput, *pcbResult) PUCHAR   pbOutput,
        __in                                        ULONG   cbOutput,
        __out                                       ULONG   *pcbResult,
        __in                                        ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptSetProperty)(
        __inout                 BCRYPT_HANDLE   hObject,
        __in                    LPCWSTR pszProperty,
        __in_bcount(cbInput)    PUCHAR   pbInput,
        __in                    ULONG   cbInput,
        __in                    ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptCloseAlgorithmProvider)(
        __inout BCRYPT_ALG_HANDLE   hAlgorithm,
        __in    ULONG   dwFlags);

    typedef VOID (WINAPI *pfnBCryptFreeBuffer)(__in PVOID pvBuffer);

    typedef NTSTATUS (WINAPI *pfnBCryptGenerateSymmetricKey)(
        __inout                             BCRYPT_ALG_HANDLE   hAlgorithm,
        __out                               BCRYPT_KEY_HANDLE   *phKey,
        __out_bcount_full_opt(cbKeyObject)  PUCHAR   pbKeyObject,
        __in                                ULONG   cbKeyObject,
        __in_bcount(cbSecret)               PUCHAR   pbSecret,
        __in                                ULONG   cbSecret,
        __in                                ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptGenerateKeyPair)(
        __inout BCRYPT_ALG_HANDLE   hAlgorithm,
        __out   BCRYPT_KEY_HANDLE   *phKey,
        __in    ULONG   dwLength,
        __in    ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptEncrypt)(
        __inout                                     BCRYPT_KEY_HANDLE hKey,
        __in_bcount_opt(cbInput)                    PUCHAR   pbInput,
        __in                                        ULONG   cbInput,
        __in_opt                                    VOID    *pPaddingInfo,
        __inout_bcount_opt(cbIV)                    PUCHAR   pbIV,
        __in                                        ULONG   cbIV,
        __out_bcount_part_opt(cbOutput, *pcbResult) PUCHAR   pbOutput,
        __in                                        ULONG   cbOutput,
        __out                                       ULONG   *pcbResult,
        __in                                        ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptDecrypt)(
        __inout                                     BCRYPT_KEY_HANDLE   hKey,
        __in_bcount_opt(cbInput)                    PUCHAR   pbInput,
        __in                                        ULONG   cbInput,
        __in_opt                                    VOID    *pPaddingInfo,
        __inout_bcount_opt(cbIV)                    PUCHAR   pbIV,
        __in                                        ULONG   cbIV,
        __out_bcount_part_opt(cbOutput, *pcbResult) PUCHAR   pbOutput,
        __in                                        ULONG   cbOutput,
        __out                                       ULONG   *pcbResult,
        __in                                        ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptExportKey)(
        __in                                        BCRYPT_KEY_HANDLE   hKey,
        __in_opt                                    BCRYPT_KEY_HANDLE   hExportKey,
        __in                                        LPCWSTR pszBlobType,
        __out_bcount_part_opt(cbOutput, *pcbResult) PUCHAR   pbOutput,
        __in                                        ULONG   cbOutput,
        __out                                       ULONG   *pcbResult,
        __in                                        ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptImportKey)(
        __in                                BCRYPT_ALG_HANDLE hAlgorithm,
        __in_opt                            BCRYPT_KEY_HANDLE hImportKey,
        __in                                LPCWSTR pszBlobType,
        __out                               BCRYPT_KEY_HANDLE *phKey,
        __out_bcount_full_opt(cbKeyObject)  PUCHAR   pbKeyObject,
        __in                                ULONG   cbKeyObject,
        __in_bcount(cbInput)                PUCHAR   pbInput,
        __in                                ULONG   cbInput,
        __in                                ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptImportKeyPair)(
        __in                            BCRYPT_ALG_HANDLE hAlgorithm,
        __in_opt                        BCRYPT_KEY_HANDLE hImportKey,
        __in                            LPCWSTR pszBlobType,
        __out                           BCRYPT_KEY_HANDLE *phKey,
        __in_bcount(cbInput)            PUCHAR   pbInput,
        __in                            ULONG   cbInput,
        __in                            ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptDuplicateKey)(
        __in                                BCRYPT_KEY_HANDLE   hKey,
        __out                               BCRYPT_KEY_HANDLE   *phNewKey,
        __out_bcount_full_opt(cbKeyObject)  PUCHAR   pbKeyObject,
        __in                                ULONG   cbKeyObject,
        __in                                ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptFinalizeKeyPair)(
        __inout BCRYPT_KEY_HANDLE   hKey,
        __in    ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptDestroyKey)(__inout BCRYPT_KEY_HANDLE hKey);

    typedef NTSTATUS (WINAPI *pfnBCryptDestroySecret)(__inout BCRYPT_SECRET_HANDLE hSecret);

    typedef NTSTATUS (WINAPI *pfnBCryptSignHash)(
        __in                                        BCRYPT_KEY_HANDLE   hKey,
        __in_opt                                    VOID    *pPaddingInfo,
        __in_bcount(cbInput)                        PUCHAR   pbInput,
        __in                                        ULONG   cbInput,
        __out_bcount_part_opt(cbOutput, *pcbResult) PUCHAR   pbOutput,
        __in                                        ULONG   cbOutput,
        __out                                       ULONG   *pcbResult,
        __in                                        ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptVerifySignature)(
        __in                        BCRYPT_KEY_HANDLE   hKey,
        __in_opt                    VOID    *pPaddingInfo,
        __in_bcount(cbHash)         PUCHAR   pbHash,
        __in                        ULONG   cbHash,
        __in_bcount(cbSignature)    PUCHAR   pbSignature,
        __in                        ULONG   cbSignature,
        __in                        ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptSecretAgreement)(
        __in    BCRYPT_KEY_HANDLE       hPrivKey,
        __in    BCRYPT_KEY_HANDLE       hPubKey,
        __out   BCRYPT_SECRET_HANDLE    *phAgreedSecret,
        __in    ULONG                   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptDeriveKey)(
        __in        BCRYPT_SECRET_HANDLE hSharedSecret,
        __in        LPCWSTR              pwszKDF,
        __in_opt    BCryptBufferDesc     *pParameterList,
        __out_bcount_part_opt(cbDerivedKey, *pcbResult) PUCHAR pbDerivedKey,
        __in        ULONG                cbDerivedKey,
        __out       ULONG                *pcbResult,
        __in        ULONG                dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptCreateHash)(
        __inout                             BCRYPT_ALG_HANDLE   hAlgorithm,
        __out                               BCRYPT_HASH_HANDLE  *phHash,
        __out_bcount_full_opt(cbHashObject) PUCHAR   pbHashObject,
        __in                                ULONG   cbHashObject,
        __in_bcount_opt(cbSecret)           PUCHAR   pbSecret,   // optional
        __in                                ULONG   cbSecret,   // optional
        __in                                ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptHashData)(
        __inout                 BCRYPT_HASH_HANDLE  hHash,
        __in_bcount(cbInput)    PUCHAR   pbInput,
        __in                    ULONG   cbInput,
        __in                    ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptFinishHash)(
        __inout                     BCRYPT_HASH_HANDLE hHash,
        __out_bcount_full(cbOutput) PUCHAR   pbOutput,
        __in                        ULONG   cbOutput,
        __in                        ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptDuplicateHash)(
        __in                                BCRYPT_HASH_HANDLE  hHash,
        __out                               BCRYPT_HASH_HANDLE  *phNewHash,
        __out_bcount_full_opt(cbHashObject) PUCHAR   pbHashObject,
        __in                                ULONG   cbHashObject,
        __in                                ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptDestroyHash)(__inout BCRYPT_HASH_HANDLE hHash);

    typedef NTSTATUS (WINAPI *pfnBCryptGenRandom)(
        __in_opt                        BCRYPT_ALG_HANDLE   hAlgorithm,
        __inout_bcount_full(cbBuffer)   PUCHAR  pbBuffer,
        __in                            ULONG   cbBuffer,
        __in                            ULONG   dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptDeriveKeyCapi)(
        __in                            BCRYPT_HASH_HANDLE  hHash,
        __in_opt                        BCRYPT_ALG_HANDLE   hTargetAlg,
        __out_bcount( cbDerivedKey )    PUCHAR              pbDerivedKey,
        __in                            ULONG               cbDerivedKey,
        __in                            ULONG               dwFlags);

    typedef NTSTATUS (WINAPI *pfnBCryptDeriveKeyPBKDF2)(
        __in                            BCRYPT_ALG_HANDLE   hPrf,
        __in_bcount_opt( cbPassword )   PUCHAR              pbPassword,
        __in                            ULONG               cbPassword,
        __in_bcount_opt( cbSalt )       PUCHAR              pbSalt,
        __in                            ULONG               cbSalt,
        __in                            ULONGLONG           cIterations,
        __out_bcount( cbDerivedKey )    PUCHAR              pbDerivedKey,
        __in                            ULONG               cbDerivedKey,
        __in                            ULONG               dwFlags);

    class CCNGAPIManager
    {
    public:
        static long Initialization();
        static long UnInitialization();

    public:
        static HMODULE                         g_hBcryptDll;
        static long                            g_lInstRefCount;

    public:
        static pfnBCryptOpenAlgorithmProvider  g_pfnBCryptOpenAlgorithmProvider;
        static pfnBCryptEnumAlgorithms         g_pfnBCryptEnumAlgorithms;
        static pfnBCryptEnumProviders          g_pfnBCryptEnumProviders;
        static pfnBCryptGetProperty            g_pfnBCryptGetProperty;
        static pfnBCryptSetProperty            g_pfnBCryptSetProperty;
        static pfnBCryptCloseAlgorithmProvider g_pfnBCryptCloseAlgorithmProvider;
        static pfnBCryptFreeBuffer             g_pfnBCryptFreeBuffer;
        static pfnBCryptGenerateSymmetricKey   g_pfnBCryptGenerateSymmetricKey;
        static pfnBCryptGenerateKeyPair        g_pfnBCryptGenerateKeyPair;
        static pfnBCryptEncrypt                g_pfnBCryptEncrypt;
        static pfnBCryptDecrypt                g_pfnBCryptDecrypt;
        static pfnBCryptExportKey              g_pfnBCryptExportKey;
        static pfnBCryptImportKey              g_pfnBCryptImportKey;
        static pfnBCryptImportKeyPair          g_pfnBCryptImportKeyPair;
        static pfnBCryptDuplicateKey           g_pfnBCryptDuplicateKey;
        static pfnBCryptFinalizeKeyPair        g_pfnBCryptFinalizeKeyPair;
        static pfnBCryptDestroyKey             g_pfnBCryptDestroyKey;
        static pfnBCryptDestroySecret          g_pfnBCryptDestroySecret;
        static pfnBCryptSignHash               g_pfnBCryptSignHash;
        static pfnBCryptVerifySignature        g_pfnBCryptVerifySignature;
        static pfnBCryptSecretAgreement        g_pfnBCryptSecretAgreement;
        static pfnBCryptDeriveKey              g_pfnBCryptDeriveKey;
        static pfnBCryptCreateHash             g_pfnBCryptCreateHash;
        static pfnBCryptHashData               g_pfnBCryptHashData;
        static pfnBCryptFinishHash             g_pfnBCryptFinishHash;
        static pfnBCryptDuplicateHash          g_pfnBCryptDuplicateHash;
        static pfnBCryptDestroyHash            g_pfnBCryptDestroyHash;
        static pfnBCryptGenRandom              g_pfnBCryptGenRandom;
        static pfnBCryptDeriveKeyCapi          g_pfnBCryptDeriveKeyCapi;
        static pfnBCryptDeriveKeyPBKDF2        g_pfnBCryptDeriveKeyPBKDF2;
    };
#endif
}


namespace nsCryptoCNGWrapper
{
#ifndef _USING_CNG_LIB

#define BCRYPT_DLL_NAME  L"Bcrypt.dll"

	NTSTATUS WINAPI BCryptOpenAlgorithmProvider(
		__out       BCRYPT_ALG_HANDLE   *phAlgorithm,
		__in        LPCWSTR pszAlgId,
		__in_opt    LPCWSTR pszImplementation,
		__in        ULONG   dwFlags);

	NTSTATUS WINAPI BCryptEnumAlgorithms(
		__in    ULONG   dwAlgOperations,
		__out   ULONG   *pAlgCount,
		__out   BCRYPT_ALGORITHM_IDENTIFIER **ppAlgList,
		__in    ULONG   dwFlags);

	NTSTATUS WINAPI BCryptEnumProviders(
		__in    LPCWSTR pszAlgId,
		__out   ULONG   *pImplCount,
		__out   BCRYPT_PROVIDER_NAME **ppImplList,
		__in    ULONG   dwFlags);

	NTSTATUS WINAPI BCryptGetProperty(
		__in                                        BCRYPT_HANDLE   hObject,
		__in                                        LPCWSTR pszProperty,
		__out_bcount_part_opt(cbOutput, *pcbResult) PUCHAR   pbOutput,
		__in                                        ULONG   cbOutput,
		__out                                       ULONG   *pcbResult,
		__in                                        ULONG   dwFlags);

	NTSTATUS WINAPI BCryptSetProperty(
		__inout                 BCRYPT_HANDLE   hObject,
		__in                    LPCWSTR pszProperty,
		__in_bcount(cbInput)    PUCHAR   pbInput,
		__in                    ULONG   cbInput,
		__in                    ULONG   dwFlags);

	NTSTATUS WINAPI BCryptCloseAlgorithmProvider(
		__inout BCRYPT_ALG_HANDLE   hAlgorithm,
		__in    ULONG   dwFlags);

	VOID WINAPI BCryptFreeBuffer(__in PVOID pvBuffer);

	NTSTATUS WINAPI BCryptGenerateSymmetricKey(
		__inout                             BCRYPT_ALG_HANDLE   hAlgorithm,
		__out                               BCRYPT_KEY_HANDLE   *phKey,
		__out_bcount_full_opt(cbKeyObject)  PUCHAR   pbKeyObject,
		__in                                ULONG   cbKeyObject,
		__in_bcount(cbSecret)               PUCHAR   pbSecret,
		__in                                ULONG   cbSecret,
		__in                                ULONG   dwFlags);

	NTSTATUS WINAPI BCryptGenerateKeyPair(
		__inout BCRYPT_ALG_HANDLE   hAlgorithm,
		__out   BCRYPT_KEY_HANDLE   *phKey,
		__in    ULONG   dwLength,
		__in    ULONG   dwFlags);

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
		__in                                        ULONG   dwFlags);

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
		__in                                        ULONG   dwFlags);

	NTSTATUS WINAPI BCryptExportKey(
		__in                                        BCRYPT_KEY_HANDLE   hKey,
		__in_opt                                    BCRYPT_KEY_HANDLE   hExportKey,
		__in                                        LPCWSTR pszBlobType,
		__out_bcount_part_opt(cbOutput, *pcbResult) PUCHAR   pbOutput,
		__in                                        ULONG   cbOutput,
		__out                                       ULONG   *pcbResult,
		__in                                        ULONG   dwFlags);

	NTSTATUS WINAPI BCryptImportKey(
		__in                                BCRYPT_ALG_HANDLE hAlgorithm,
		__in_opt                            BCRYPT_KEY_HANDLE hImportKey,
		__in                                LPCWSTR pszBlobType,
		__out                               BCRYPT_KEY_HANDLE *phKey,
		__out_bcount_full_opt(cbKeyObject)  PUCHAR   pbKeyObject,
		__in                                ULONG   cbKeyObject,
		__in_bcount(cbInput)                PUCHAR   pbInput,
		__in                                ULONG   cbInput,
		__in                                ULONG   dwFlags);

	NTSTATUS WINAPI BCryptImportKeyPair(
		__in                            BCRYPT_ALG_HANDLE hAlgorithm,
		__in_opt                        BCRYPT_KEY_HANDLE hImportKey,
		__in                            LPCWSTR pszBlobType,
		__out                           BCRYPT_KEY_HANDLE *phKey,
		__in_bcount(cbInput)            PUCHAR   pbInput,
		__in                            ULONG   cbInput,
		__in                            ULONG   dwFlags);

	NTSTATUS WINAPI BCryptDuplicateKey(
		__in                                BCRYPT_KEY_HANDLE   hKey,
		__out                               BCRYPT_KEY_HANDLE   *phNewKey,
		__out_bcount_full_opt(cbKeyObject)  PUCHAR   pbKeyObject,
		__in                                ULONG   cbKeyObject,
		__in                                ULONG   dwFlags);

	NTSTATUS WINAPI BCryptFinalizeKeyPair(
		__inout BCRYPT_KEY_HANDLE   hKey,
		__in    ULONG   dwFlags);

	NTSTATUS WINAPI BCryptDestroyKey(__inout BCRYPT_KEY_HANDLE hKey);

	NTSTATUS WINAPI BCryptDestroySecret(__inout BCRYPT_SECRET_HANDLE hSecret);

	NTSTATUS WINAPI BCryptSignHash(
		__in                                        BCRYPT_KEY_HANDLE   hKey,
		__in_opt                                    VOID    *pPaddingInfo,
		__in_bcount(cbInput)                        PUCHAR   pbInput,
		__in                                        ULONG   cbInput,
		__out_bcount_part_opt(cbOutput, *pcbResult) PUCHAR   pbOutput,
		__in                                        ULONG   cbOutput,
		__out                                       ULONG   *pcbResult,
		__in                                        ULONG   dwFlags);

	NTSTATUS WINAPI BCryptVerifySignature(
		__in                        BCRYPT_KEY_HANDLE   hKey,
		__in_opt                    VOID    *pPaddingInfo,
		__in_bcount(cbHash)         PUCHAR   pbHash,
		__in                        ULONG   cbHash,
		__in_bcount(cbSignature)    PUCHAR   pbSignature,
		__in                        ULONG   cbSignature,
		__in                        ULONG   dwFlags);

	NTSTATUS WINAPI BCryptSecretAgreement(
		__in    BCRYPT_KEY_HANDLE       hPrivKey,
		__in    BCRYPT_KEY_HANDLE       hPubKey,
		__out   BCRYPT_SECRET_HANDLE    *phAgreedSecret,
		__in    ULONG                   dwFlags);

	NTSTATUS WINAPI BCryptDeriveKey(
		__in        BCRYPT_SECRET_HANDLE hSharedSecret,
		__in        LPCWSTR              pwszKDF,
		__in_opt    BCryptBufferDesc     *pParameterList,
		__out_bcount_part_opt(cbDerivedKey, *pcbResult) PUCHAR pbDerivedKey,
		__in        ULONG                cbDerivedKey,
		__out       ULONG                *pcbResult,
		__in        ULONG                dwFlags);

	NTSTATUS WINAPI BCryptCreateHash(
		__inout                             BCRYPT_ALG_HANDLE   hAlgorithm,
		__out                               BCRYPT_HASH_HANDLE  *phHash,
		__out_bcount_full_opt(cbHashObject) PUCHAR   pbHashObject,
		__in                                ULONG   cbHashObject,
		__in_bcount_opt(cbSecret)           PUCHAR   pbSecret,   // optional
		__in                                ULONG   cbSecret,   // optional
		__in                                ULONG   dwFlags);

	NTSTATUS WINAPI BCryptHashData(
		__inout                 BCRYPT_HASH_HANDLE  hHash,
		__in_bcount(cbInput)    PUCHAR   pbInput,
		__in                    ULONG   cbInput,
		__in                    ULONG   dwFlags);

	NTSTATUS WINAPI BCryptFinishHash(
		__inout                     BCRYPT_HASH_HANDLE hHash,
		__out_bcount_full(cbOutput) PUCHAR   pbOutput,
		__in                        ULONG   cbOutput,
		__in                        ULONG   dwFlags);

	NTSTATUS WINAPI BCryptDuplicateHash(
		__in                                BCRYPT_HASH_HANDLE  hHash,
		__out                               BCRYPT_HASH_HANDLE  *phNewHash,
		__out_bcount_full_opt(cbHashObject) PUCHAR   pbHashObject,
		__in                                ULONG   cbHashObject,
		__in                                ULONG   dwFlags);

	NTSTATUS WINAPI BCryptDestroyHash(__inout BCRYPT_HASH_HANDLE hHash);

	NTSTATUS WINAPI BCryptGenRandom(
		__in_opt                        BCRYPT_ALG_HANDLE   hAlgorithm,
		__inout_bcount_full(cbBuffer)   PUCHAR  pbBuffer,
		__in                            ULONG   cbBuffer,
		__in                            ULONG   dwFlags);

	NTSTATUS WINAPI BCryptDeriveKeyCapi(
		__in                            BCRYPT_HASH_HANDLE  hHash,
		__in_opt                        BCRYPT_ALG_HANDLE   hTargetAlg,
		__out_bcount( cbDerivedKey )    PUCHAR              pbDerivedKey,
		__in                            ULONG               cbDerivedKey,
		__in                            ULONG               dwFlags);

	NTSTATUS WINAPI BCryptDeriveKeyPBKDF2(
		__in                            BCRYPT_ALG_HANDLE   hPrf,
		__in_bcount_opt( cbPassword )   PUCHAR              pbPassword,
		__in                            ULONG               cbPassword,
		__in_bcount_opt( cbSalt )       PUCHAR              pbSalt,
		__in                            ULONG               cbSalt,
		__in                            ULONGLONG           cIterations,
		__out_bcount( cbDerivedKey )    PUCHAR              pbDerivedKey,
		__in                            ULONG               cbDerivedKey,
		__in                            ULONG               dwFlags);
#endif
}

