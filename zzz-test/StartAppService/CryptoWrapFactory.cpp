#include "stdafx.h"
#include "CryptoSynchUtility.h"
#include "CryptoCAPIWrapperImp.h"
#include "CryptoCNGWrapperImp.h"
#include "CryptoCommon.h"
#include "CryptoCAPIWrapperImp.h"
#include <set>
using namespace std;

std::set<ICryptoWrapperInterface*> g_setCryptoWrapInterface;
ICryptoWrapperInterface* FindCryptoWrapInterface(CCryptoParam& CryptoInfo, set<ICryptoWrapperInterface*>* psetCryptoWrapInter = NULL)
{
    if(NULL == psetCryptoWrapInter)
        psetCryptoWrapInter = &g_setCryptoWrapInterface;

    ICryptoWrapperInterface* pCryptoWrapInstance = NULL;

    for(set<ICryptoWrapperInterface*>::iterator Iterator = psetCryptoWrapInter->begin();
        Iterator != psetCryptoWrapInter->end();
        Iterator++)
    {
        pCryptoWrapInstance = *Iterator;
        if(pCryptoWrapInstance && (pCryptoWrapInstance->CryptoParam() == CryptoInfo))
        {
            INFLOG(L"Found a exist crypto interface. (Param: API{%s}, EncAlg{%s}, EncLib{%s}, HashAlg{%s}, Iter{%s}, IV{%s})",
                CCryptoUtility::FormatCryptoAPIType((E_CRYPTOAPI_TYPE)CryptoInfo.m_dwEncAPIType),
                CCryptoUtility::FormatEncAlgType((E_ENCALG_TYPE)CryptoInfo.m_dwEncAlgType),
                CCryptoUtility::FormatEncLibType((E_ENCLIB_TYPE)CryptoInfo.m_dwEncLibType),
                CCryptoUtility::FormatHashAlgType((E_HASHALG_TYPE)CryptoInfo.m_dwHashAlgType),
                CCryptoUtility::FormatInterfaceType((E_INTER_TYPE)CryptoInfo.m_dwInterfaceType),
                CCryptoUtility::FormatIVUsageStatus((E_IV_USAGE)CryptoInfo.m_dwIVUsage));
            break;
        }
    }
    return pCryptoWrapInstance;
}

ICryptoWrapperInterface*
ICryptoWrapFactory::CryptoWrapInstance(CCryptoParam& CryptoInfo, bool CreateNewAlways /* = true */, long* plErrCode /* = NULL */)
{
    if(plErrCode)
        *plErrCode = 0;

    CCryptoConfig CryptoCfgMgr;
    CCryptoUtility::UpdateCryptoParam(CryptoInfo, CryptoCfgMgr);

    long lRetCode = CCryptoUtility::VerifyCryptoParam(CryptoInfo);
    if(lRetCode != 0)
    {
        if(plErrCode)
            *plErrCode = lRetCode;
        ERRTRCEX(D2DCRYPTO_E_INVALID_PARAM, L"Cannot create crypto interface because at least one input parameters are not support. (EC=[0x%08x])", lRetCode);
        return NULL;
    }

    ICryptoWrapperInterface* pCryptoWrapInstance = NULL;

#ifdef _SAVE_CRYPTO_INSTANCE_GLOBAL
    if(!CreateNewAlways)
    {
        pCryptoWrapInstance = FindCryptoWrapInterface(CryptoInfo, NULL);
        if(pCryptoWrapInstance)
        {
            return pCryptoWrapInstance;
        }
    }
#endif

    if(ELT_MSCRYPTO == (E_ENCLIB_TYPE)CryptoInfo.m_dwEncLibType)
    {
        DETLOG(L"Enc/Dec using microsoft cryptography library.");

        static CCriticalSectionWrapper InstanceMutex;
        CCriticalSectionGuard InstanceGuard(InstanceMutex);

#ifdef _SAVE_CRYPTO_INSTANCE_GLOBAL
        if(!CreateNewAlways)
        {
            pCryptoWrapInstance = FindCryptoWrapInterface(CryptoInfo, NULL);
            if(pCryptoWrapInstance)
            {           
                return pCryptoWrapInstance;
            }
        }
#endif

        if(CCryptoUtility::IsCNGAble2Used())
        {
            if(ECAT_CAPI == (E_CRYPTOAPI_TYPE)CryptoInfo.m_dwEncAPIType)
            {
                pCryptoWrapInstance =  new nsCryptoCAPIWrapper::CCryptoCAPIWrapper(&CryptoInfo, NULL);
                INFLOG(L"Manually, Create CAPI instance to perform crypto operations.");
            }
            else
            {
#ifndef _USING_CNG_LIB
                nsCryptoCNGWrapper::CCNGAPIManager::Initialization();
#endif
                pCryptoWrapInstance =  new nsCryptoCNGWrapper::CCryptoCNGWrapper(&CryptoInfo, NULL);
                DETLOG(L"Automatically, create CNG instance to perform crypto operations.");
            }
        }
        else
        {
            pCryptoWrapInstance =  new nsCryptoCAPIWrapper::CCryptoCAPIWrapper(&CryptoInfo, NULL);
            DETLOG(L"Automatically, create CAPI instance to perform crypto operations.");
        }

        if(pCryptoWrapInstance)
        {
            long lRetCode = pCryptoWrapInstance->Initialize(NULL);
            if(lRetCode != 0)
            {
                ERRTRCEX(lRetCode, L"Failed to initialize crypto instance. (EC=[0x%08x])", lRetCode);
                if (plErrCode)
                    *plErrCode = lRetCode;
                delete pCryptoWrapInstance;
                pCryptoWrapInstance = NULL;
            }

#ifdef _SAVE_CRYPTO_INSTANCE_GLOBAL
            else
                g_setCryptoWrapInterface.insert(pCryptoWrapInstance);
#endif
        }

        //ZZ: Instance information summary.
        if (pCryptoWrapInstance)
        {
            INFLOG(L"Create:   %s | %s | %s | %s | %s | %s | %s", 
                WZ_S(pCryptoWrapInstance->InstanceGUID()),
                CCryptoUtility::FormatCryptoAPIType(pCryptoWrapInstance->CryptoAPIType()),
                CCryptoUtility::FormatEncAlgType((E_ENCALG_TYPE)pCryptoWrapInstance->CryptoParam().m_dwEncAlgType),
                CCryptoUtility::FormatEncLibType((E_ENCLIB_TYPE)pCryptoWrapInstance->CryptoParam().m_dwEncLibType),
                CCryptoUtility::FormatHashAlgType((E_HASHALG_TYPE)pCryptoWrapInstance->CryptoParam().m_dwHashAlgType),
                CCryptoUtility::FormatInterfaceType((E_INTER_TYPE)pCryptoWrapInstance->CryptoParam().m_dwInterfaceType),
                CCryptoUtility::FormatIVUsageStatus((E_IV_USAGE)pCryptoWrapInstance->CryptoParam().m_dwIVUsage));
        }
    }
    return pCryptoWrapInstance;
}

void
ICryptoWrapFactory::ReleaseCryptoWrapInstance(ICryptoWrapperInterface** ppCryptoWrapInterface)
{
#ifdef _SAVE_CRYPTO_INSTANCE_GLOBAL
    static CCriticalSectionWrapper InstanceMutex;
    CCriticalSectionGuard InstanceGuard(InstanceMutex);
    for(set<ICryptoWrapperInterface*>::iterator Iterator = g_setCryptoWrapInterface.begin();
        Iterator != g_setCryptoWrapInterface.end();
        Iterator++)
    {
        ICryptoWrapperInterface* pCryptoWrapInstance = *Iterator;
        if(pCryptoWrapInstance == *ppCryptoWrapInterface)
        {
            delete pCryptoWrapInstance;
            *ppCryptoWrapInterface = NULL;
            g_setCryptoWrapInterface.erase(Iterator);
            break;
        }
    }
#else
    ICryptoWrapperInterface*& pCryptoWrapInstance = *ppCryptoWrapInterface;
    if(pCryptoWrapInstance)
    {
        INFLOG(L"Release:: %s", WZ_S(pCryptoWrapInstance->InstanceGUID()));
        delete pCryptoWrapInstance;
        pCryptoWrapInstance = NULL;
    }
#endif

#ifndef _USING_CNG_LIB
    nsCryptoCNGWrapper::CCNGAPIManager::UnInitialization();
#endif
}

long 
ICryptoWrapFactory::WString2Binary(
    PBYTE pbBinary, 
    DWORD* pdwBinSize, 
    const WCHAR* pwzString, 
    DWORD dwStrSize, 
    DWORD dwFlags /* = ESEO_BASE64_ANY */, 
    DWORD* pdwSkipSize /* = NULL */, 
    DWORD* pdwFlagsUsed /* = NULL */)
{
    if(NULL == pdwBinSize)
        return D2DCRYPTO_E_INVALID_PARAM;

    DWORD dwBinLen = *pdwBinSize;
    CRYPTO_RETCHK(
        CryptStringToBinaryW(
        pwzString,
        dwStrSize,
        dwFlags,
        NULL,
        &dwBinLen,
        pdwSkipSize,
        pdwFlagsUsed));
    if(dwBinLen > *pdwBinSize)
    {
        *pdwBinSize = dwBinLen;
        return D2DCRYPTO_E_MORE_DATA;
    }
    else if(NULL == pbBinary)
    {
        *pdwBinSize = dwBinLen;
        return D2DCRYPTO_E_MORE_DATA;
    }

    CRYPTO_RETCHK(
        CryptStringToBinaryW(
        pwzString,
        dwStrSize,
        dwFlags,
        pbBinary,
        pdwBinSize,
        pdwSkipSize,
        pdwFlagsUsed));

    return 0;
}

long 
ICryptoWrapFactory::Binary2StringW(
    WCHAR* pwzString, 
    DWORD* pdwStrSize, 
    const PBYTE pbBinary, 
    DWORD dwBinSize, 
    DWORD dwFlags /* = ESEO_BASE64 */)
{
    if(NULL == pdwStrSize)
        return D2DCRYPTO_E_INVALID_PARAM;

    DWORD dwWStrLen = *pdwStrSize;
    CRYPTO_RETCHK(
        CryptBinaryToStringW(
        pbBinary,
        dwBinSize,
        dwFlags,
        NULL,
        &dwWStrLen));  //ZZ: Including terminating null character.

    if(dwWStrLen > *pdwStrSize)
    {
        *pdwStrSize = dwWStrLen;
        return D2DCRYPTO_E_MORE_DATA;
    }
    else if(NULL == pwzString)
    {
        *pdwStrSize = dwWStrLen;
        return D2DCRYPTO_E_MORE_DATA;
    }

    CRYPTO_RETCHK(
        CryptBinaryToStringW(
        pbBinary,
        dwBinSize,
        dwFlags,
        pwzString,
        &dwWStrLen));  //ZZ: Not including terminating null character.

    *pdwStrSize = dwWStrLen + 1;  //ZZ: Total data size converted including terminating null character.

    return 0;
}

long 
ICryptoWrapFactory::EnumKeyContainerInCSP(
    std::vector<std::wstring>& vecContainerName, 
    E_QUERY_KEYSET_CTRL eQueryCtrl /* = EQKC_D2D_ONLY */, 
    bool bSkipSelf /* = true */, 
    CCryptoParam* pCryptoInfo /* = NULL */)
{
    CCryptoParam CurCryptoParam;
    long lRetCode = CCryptoUtility::GenerateCryptoParam(CurCryptoParam, pCryptoInfo, EIT_HASH, ECAT_CAPI);
    if (0 != lRetCode)
    {
        ERRTRCEX(lRetCode, L"[CAPI] Cannot create crypto interface because at least one input parameters are not support. (EC=[0x%08x])", lRetCode);
        return lRetCode;
    }

    CCryptoConfig CryptoCfgMgr;
    nsCryptoCAPIWrapper::CCryptoCAPIWrapper CAPIEncInstance(&CurCryptoParam, &CryptoCfgMgr);
    lRetCode = CAPIEncInstance.Initialize(NULL);
    if (0 != lRetCode)
    {
        ERRTRCEX(lRetCode, L"Failed to initialize CAPI encryption object for enumerating container name. (EC=[%#08x])", lRetCode);
        return lRetCode;
    }

    lRetCode = CAPIEncInstance.GetKeyContainerList(vecContainerName, NULL, eQueryCtrl, bSkipSelf);
    if (0 != lRetCode)
        ERRTRCEX(lRetCode, L"Failed to get key container name. (EC=[%#08x])", lRetCode);
 
    DETTRC(L"Succeed to enumerate key container. (Num=[%u])", vecContainerName.size());

    return lRetCode;
}

long 
ICryptoWrapFactory::RemoveKeyContainerInCSP(
    std::vector<std::wstring>* pvecContainerName /* = NULL */, 
    E_QUERY_KEYSET_CTRL eQueryCtrl /* = EQKC_D2D_ONLY */, 
    CCryptoParam* pCryptoInfo /* = NULL */)
{
    CCryptoParam CurCryptoParam;
    long lRetCode = CCryptoUtility::GenerateCryptoParam(CurCryptoParam, pCryptoInfo, EIT_HASH, ECAT_CAPI);
    if (0 != lRetCode)
    {
        ERRTRCEX(lRetCode, L"[CAPI] Cannot create crypto interface because at least one input parameters are not support. (EC=[0x%08x])", lRetCode);
        return lRetCode;
    }

    CCryptoConfig CryptoCfgMgr;
    nsCryptoCAPIWrapper::CCryptoCAPIWrapper CAPIEncInstance(&CurCryptoParam, &CryptoCfgMgr);
    lRetCode = CAPIEncInstance.Initialize(NULL);
    if (0 != lRetCode)
    {
        ERRTRCEX(lRetCode, L"Failed to initialize CAPI encryption object for enumerating container name. (EC=[%#08x])", lRetCode);
        return lRetCode;
    }
    
    WStrVector vecKeyContainer;
    if (NULL == pvecContainerName)
    {
        lRetCode = CAPIEncInstance.GetKeyContainerList(vecKeyContainer, NULL, eQueryCtrl, true);
        if (0 != lRetCode)
        {
            ERRTRCEX(lRetCode, L"Failed to get key container name. (EC=[%#08x])", lRetCode);
            return lRetCode;
        }

        DETTRC(L"Succeed to enumerate key container. (Num=[%u])", vecKeyContainer.size());
        pvecContainerName = &vecKeyContainer;
    }

    if (NULL == pvecContainerName)
    {
        ERRTRCEX(D2DCOMM_E_INVALID_PARAM, L"Container name vector is NULL.");
        return D2DCOMM_E_INVALID_PARAM;
    }

    for (UINT uiIdx = 0; uiIdx < pvecContainerName->size(); uiIdx++)
    {
        wstring& wsContainerName = (*pvecContainerName)[uiIdx];
        lRetCode = CAPIEncInstance.RemoveKeyContainer(wsContainerName.c_str());
        if (0 != lRetCode)
            ERRTRCEX(lRetCode, L"Failed to remove key container: %s. (EC=[%#08x])", WS_S(wsContainerName), lRetCode);
        else
            DBGTRC(L"Succeed to remove key container: %s.", WS_S(wsContainerName));
    }

    DETTRC(L"Succeed to remove key container. (Num=[%u])", pvecContainerName->size());

    return lRetCode;
}

long ICryptoWrapFactory::CreateKeyContainerInCSP(
    const WCHAR* pwzContainerName /* = NULL */, 
    bool bTryRandomName /* = true */, 
    CCryptoParam* pCryptoInfo /* = NULL */, 
    wstring* pwsActualContianerName /* = NULL */)
{
    CCryptoParam CurCryptoParam;
    long lRetCode = CCryptoUtility::GenerateCryptoParam(CurCryptoParam, pCryptoInfo, EIT_HASH, ECAT_CAPI);
    if (0 != lRetCode)
    {
        ERRTRCEX(lRetCode, L"[CAPI] Cannot create crypto interface because at least one input parameters are not support. (EC=[0x%08x])", lRetCode);
        return lRetCode;
    }

    CCryptoConfig CryptoCfgMgr;
    nsCryptoCAPIWrapper::CCryptoCAPIWrapper CAPIEncInstance(&CurCryptoParam, &CryptoCfgMgr);
    lRetCode = CAPIEncInstance.InitializeEx(NULL, pwzContainerName, bTryRandomName, true);
    if (0 != lRetCode)
    {
        ERRTRCEX(lRetCode, L"Failed to initialize CAPI encryption object for enumerating container name. (EC=[%#08x])", lRetCode);
        return lRetCode;
    }

    if (pwsActualContianerName)
        *pwsActualContianerName = CAPIEncInstance.KeyContainerName();

    DETTRC(L"Succeed to create key container: %s.", WZ_S(CAPIEncInstance.KeyContainerName()));

    return lRetCode;
}