// VHDConv.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "HAVhdUtility.h"
//#include "InnerInterface.h"
//#include "VHDFile.h"
#include "AFD2DUtil.h"
#include "VDiskDiffImp.h"
#include "IncDisksInFull.h"
#include "IncDisksMixMode.h"
using namespace HaVhdUtility;
#include "HALog.h"
//#include "brandname.h"
#include "VHDFileWImp.h"
#include "VHDImage.h"
//#include "afstorinterface.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif

//#define REG_KEY_ARC_OFFLINECOPY     CST_REG_HEADER_L L"\\" CST_PRODUCT_NAME_L L"\\OfflineCopy"

wchar_t g_wszModulePath[2048] = {0};
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
        {
#if 0
            GetModuleFileNameW(hModule, g_wszModulePath, sizeof(g_wszModulePath)/sizeof(wchar_t));
            if (wchar_t* p = wcsrchr(g_wszModulePath, L'\\'))
                *p = L'\0';

            //Init log
            wchar_t wszPath[1024] = {0};
            wcsncpy_s(wszPath, _countof(wszPath), g_wszModulePath, _TRUNCATE);
            if (wchar_t* p = wcsrchr(wszPath, L'\\'))
                *p = L'\0';
            wcsncat_s(wszPath, _countof(wszPath), L"\\Logs", _TRUNCATE);  

            CreateAllDirecotries(wszPath, NULL);

            wcsncat_s(wszPath, _countof(wszPath), L"\\HAVhdConv.log", _TRUNCATE);

            CHALog::Instance()->Init(wszPath, true, true);
            HALogSetDbgLevelReg(HKEY_LOCAL_MACHINE, REG_KEY_ARC_OFFLINECOPY L"\\DebugLevel", L"HAVhdUtility");
#endif
        }
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

#if 0

VHDCONV_API int HaVhdUtility::CreateVHDFile(const wchar_t* pwszFilePath, IVHDFile** ppVHDFile)
{
    if (pwszFilePath == NULL || ppVHDFile == NULL)
        return E_INVALIDARG;

    *ppVHDFile = NULL;
    int nRet = CVHDFileCreator::Open(pwszFilePath, MODE_READ,
        FILE_SHARE_READ|FILE_SHARE_WRITE, OPEN_EXISTING, ppVHDFile);

    return nRet;
}

VHDCONV_API int HaVhdUtility::CreateVHDFileEx(const wchar_t* pwszFilePath, unsigned long ulDesiredAccess,
                                unsigned long ulShareMode, IVHDFile** ppVHDFile)
{
    // Now only support read, so does not consider the write access
    if (pwszFilePath == NULL || ppVHDFile == NULL)
        return E_INVALIDARG;

    OPENMODE eMode = MODE_NULL;
    if (ulDesiredAccess == GENERIC_READ)
        eMode = MODE_READ;
    else if (ulDesiredAccess == GENERIC_WRITE)
        eMode = MODE_WRITE;
    else if (ulDesiredAccess == (GENERIC_READ | GENERIC_WRITE))
        eMode = MODE_READWRITE;
    else
    {
        HALogError(L"CreateVHDFileEx, input invalid access par, par = %d", ulDesiredAccess);
        return E_INVALIDARG;
    }

    *ppVHDFile = NULL;
    int nRet = CVHDFileCreator::Open(pwszFilePath, eMode, ulShareMode, OPEN_EXISTING, ppVHDFile);

    return nRet;
}

VHDCONV_API int HaVhdUtility::ConvertD2DToVHD(const wchar_t* pwszD2D, bool bSetRelP, const wchar_t* pwszVHDRelP, 
                                bool bSetAbsP, const wchar_t* pwszVHDAbsP)
{
    int nRet = 0;
    CDynVHDFile* pVHD = NULL;

    nRet = CVHDFileCreator::Open(pwszD2D, MODE_READ, OPEN_EXISTING, &pVHD);
    if (nRet)   
    {
        HALogError(L"ConvD2DToVHD::Open D2D VHD failed, rc = 0x%08X", nRet);
        return nRet;
    }

    if (pVHD == NULL)   return -1;

    do
    {
        //1)change footer's signature
        VHDFOOTER footer = {0};
        nRet = pVHD->GetFooter(footer);
        if (nRet)
        {
            HALogError(L"ConvD2DToVHD::Get footer error, rc = 0x%08X", nRet);
            break;
        }

        if (memcmp(footer.Cookie, ORIG_CREATOR_MS, sizeof(footer.Cookie)) == 0)
        {
            CHALog::Instance()->LogMsg(L"ConvD2DToVHD::Already a VHD file");
            break;
        }

        nRet = pVHD->SetCreatorCookie(ORIG_CREATOR_MS);
        if (nRet)
        {
            HALogError(L"ConvD2DToVHD::set MS cookie failed, rc = 0x%08X", nRet);
            break;
        }

        //2)change parent path
        if (bSetRelP)
        {
            nRet = pVHD->SetParentPath(HaVhdUtility::WIN_RELATIVE, pwszVHDRelP);
            if (nRet)
            {
                HALogError(L"ConvD2DToVHD::set relative path failed, rc = 0x%08X", nRet);
                break;
            }
        }

        if (bSetAbsP)
        {
            nRet = pVHD->SetParentPath(HaVhdUtility::WIN_ABSOLUTE, pwszVHDAbsP);
            if (nRet)
            {
                HALogError(L"ConvD2DToVHD::set absolute path failed, rc = 0x%08X", nRet);
                break;
            }
        }

    } while (0);

    if (pVHD)
        pVHD->Release();

    return nRet;
}


VHDCONV_API int HaVhdUtility::ConvertVHDToD2D(const wchar_t* pwszVHD, bool bSetRelP, const wchar_t* pwszD2DRelP, 
                                bool bSetAbsP, const wchar_t* pwszD2DAbsP)
{
    int nRet = 0;
    CDynVHDFile* pVHD = NULL;

    nRet = CVHDFileCreator::Open(pwszVHD, MODE_READ, OPEN_EXISTING, &pVHD);
    if (nRet)   
    {
        HALogError(L"ConvD2DToVHD::Open D2D VHD failed, rc = 0x%08X", nRet);
        return nRet;
    }

    if (pVHD == NULL)   return -1;

    do
    {
        //1)change footer's signature
        VHDFOOTER footer = {0};
        nRet = pVHD->GetFooter(footer);
        if (nRet)
        {
            HALogError(L"ConvD2DToVHD::Get footer error, rc = 0x%08X", nRet);
            break;
        }

        if (memcmp(footer.Cookie, ORIG_CREATOR_D2D, sizeof(footer.Cookie)) == 0)
        {
            CHALog::Instance()->LogMsg(L"ConvD2DToVHD::Already a VHD file");
            break;
        }

        nRet = pVHD->SetCreatorCookie(ORIG_CREATOR_D2D);
        if (nRet)
        {
            HALogError(L"ConvD2DToVHD::set MS cookie failed, rc = 0x%08X", nRet);
            break;
        }

        //2)change parent path
        if (bSetRelP)
        {
            nRet = pVHD->SetParentPath(HaVhdUtility::WIN_RELATIVE, pwszD2DRelP);
            if (nRet)
            {
                HALogError(L"ConvD2DToVHD::set relative path failed, rc = 0x%08X", nRet);
                break;
            }
        }

        if (bSetAbsP)
        {
            nRet = pVHD->SetParentPath(HaVhdUtility::WIN_ABSOLUTE, pwszD2DAbsP);
            if (nRet)
            {
                HALogError(L"ConvD2DToVHD::set absolute path failed, rc = 0x%08X", nRet);
                break;
            }
        }

    } while (0);

    if (pVHD)
        pVHD->Release();

    return nRet;
}

VHDCONV_API int HaVhdUtility::GetVHDProp(const wchar_t* pwszFilePath, VHD_TYPE* peType, VHD_CREATOR* peCreator)
{
    int nRet = 0;
    IVHDFile* pVHD = NULL;

    if (pwszFilePath == NULL) return E_VHD_COV_INVALID_ARG;

    nRet = CreateVHDFile(pwszFilePath, &pVHD);
    if (nRet)
    {
        HALogError(L"GetVHDProp::Crate VHD file failed, rc = 0x%08X", nRet);
        return nRet;
    }

    if (pVHD == NULL)
    {
        HALogError(L"GetVHDProp::Crate VHD file failed");
        return -1;
    }
    
    if (peType)
        *peType = pVHD->GetVHDType();

    if (peCreator)
        *peCreator = pVHD->GetVHDCreator();

    if (pVHD)
        pVHD->Release();

    return nRet;
}


#endif // if 0

//VHDCONV_API int HaVhdUtility::CreateVHDFile(const wchar_t* pwszFilePath, D2D_ENCRYPTION_INFO* pEncInfo, 
//                                            IVHDFile** ppVHDFile)
//{
//    return CreateVHDFileEx(pwszFilePath, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, pEncInfo, ppVHDFile);
//}

//VHDCONV_API int HaVhdUtility::CreateVHDFileEx(const wchar_t* pwszFilePath, unsigned long ulDesiredAccess,
//                                unsigned long ulShareMode, D2D_ENCRYPTION_INFO* pEncInfo, IVHDFile** ppVHDFile)
//{
//    // Now only support read, so does not consider the write access
//    if (pwszFilePath == NULL || ppVHDFile == NULL)
//        return E_INVALIDARG;
//
//    return CreateVHDDiskEx(pwszFilePath, ulDesiredAccess, ulShareMode, pEncInfo, ppVHDFile);
//}

VHDCONV_API int HaVhdUtility::CreateVDiskDiffEx(const wchar_t* pwszVHDOld, 
                                                const wchar_t* pwszVHDNew,
                                                D2D_ENCRYPTION_INFO* pEncInfo,
                                                IVDiskDiff** ppDiskDiff)
{
    if (ppDiskDiff == NULL)
        return E_INVALIDARG;

    int nRet = 0;
    *ppDiskDiff = NULL;

    CVDiskDiffImp* pDiff = new CVDiskDiffImp;
    nRet = pDiff->Init(pwszVHDOld, pwszVHDNew, pEncInfo);
    if (nRet)
    {
        HALogError(L"CreateVDiskDiffEx::Init CVDiskVHD failed, rc = %d", nRet);
    }

    if (nRet)
    {
        pDiff->Release();
        pDiff = NULL;
    }

    *ppDiskDiff = pDiff;

    return nRet;
}

VHDCONV_API int HaVhdUtility::CreateVDiskDiff(const wchar_t* pwszFilePath, D2D_ENCRYPTION_INFO* pEncInfo, 
                                              IVDiskDiff** ppDiskDiff)
{
    if (ppDiskDiff == NULL)
        return E_INVALIDARG;

    int nRet = 0;
    *ppDiskDiff = NULL;

    CVDiskDiffImp* pDiff = new CVDiskDiffImp;
    nRet = pDiff->Init(pwszFilePath, pEncInfo);
    if (nRet)
    {
        HALogError(L"CreateVDiskDiffEx::Init CVDiskVHD failed, rc = %d", nRet);
    }

    if (nRet)
    {
        pDiff->Release();
        pDiff = NULL;
    }

    *ppDiskDiff = pDiff;

    return nRet;
}


VHDCONV_API int HaVhdUtility::CreateVHDFileW(const wchar_t*      pwszFilePath, 
                               const ST_CREATE_PARMS*    pstCreateParms,
                               IVHDFileW** ppVhdW)
{
    if (pwszFilePath == NULL || pstCreateParms == NULL || ppVhdW == NULL)
        return E_INVALIDARG;

    if (pstCreateParms->eType != VHD_Dynamic)
        return E_NOTIMPL; // Currently, does not implement FIXED

    int nRet = 0;

    CDynVHDFileW* pFile = new CDynVHDFileW;
    nRet = pFile->Init(pwszFilePath, pstCreateParms);
    if (nRet == 0)
        *ppVhdW = pFile;
    else
        pFile->Release();


    return nRet;
}

VHDCONV_API int HaVhdUtility::CreateChildVHDFileW(const wchar_t*      pwszFilePath, 
                                    const ST_CREATE_PARMS*    pstCreateParms,
                                    const wchar_t*      pwszParentPath,
                                    IVHDFileW** ppVhdW)
{
    int nRet = 0;

    if (pwszFilePath == NULL || pstCreateParms == NULL || ppVhdW == NULL)
        return E_INVALIDARG;

    if (pstCreateParms->eType != VHD_Diff)
        return E_INVALIDARG;

    CDynVHDFileW* pFile = new CDynVHDFileW;
    nRet = pFile->Init(pwszFilePath, pstCreateParms, pwszParentPath);
    if (nRet == 0)
        *ppVhdW = pFile;
    else
        pFile->Release();


    return nRet;
}

VHDCONV_API int HaVhdUtility::OpenVDisk_HyperV(const wchar_t* pwszPath, bool bROLast, IVDisk** ppDisk)
{
    int nRet = 0;
    if (pwszPath == NULL || ppDisk == NULL)
        return E_INVALIDARG;
    CVHDImage* pImage = new CVHDImage;
    nRet = pImage->Init(pwszPath, bROLast);
    if (nRet == 0)
    {
        *ppDisk = pImage;
    }
    else
    {
        HALogError(L"OpenVDisk_HyperV failed, pwszPath = %s, bROLast = %d, rc = %d", pwszPath, bROLast, nRet);
        pImage->Release();
    }

    return nRet;
}


VHDCONV_API int HaVhdUtility::GetIncDisksFromFull(/*in*/const wchar_t* pwszFullVhd, /*in*/IDiskBmpReader* pDiffBitmap,
                                    D2D_ENCRYPTION_INFO* pEncInfo, /*out*/IVDiskDiff** ppDiskDiff)
{
    int nRet = 0;
    if (pwszFullVhd == NULL || pwszFullVhd[0] == L'\0' || pDiffBitmap == NULL || ppDiskDiff == NULL)
        return E_INVALIDARG;

    CIncDisksInFull* pIncDisks = new CIncDisksInFull;
    nRet = pIncDisks->Init(pwszFullVhd, pDiffBitmap, pEncInfo);
    if (nRet)
    {
        HALogError(L"GetIncDisksFromFull failed, full_vhd = %s, rc = %d", pwszFullVhd, nRet);
        pIncDisks->Release();
        pIncDisks = NULL;
    }

    *ppDiskDiff = pIncDisks;
    return nRet;
}

VHDCONV_API int HaVhdUtility::GetIncDisks_MixMode(const wchar_t* pwszFullVhd, IDiskBmpReader* pDiffBitmap,
                                    const wchar_t* pwszIncVhd, D2D_ENCRYPTION_INFO* pEncInfo, 
                                    IVDiskDiff** ppDiskDiff)
{
    int nRet = 0;
    if (pwszFullVhd == NULL || pwszFullVhd[0] == L'\0' 
        || pwszIncVhd == NULL || pwszIncVhd[0] == L'\0'
        || pDiffBitmap == NULL || ppDiskDiff == NULL)
        return E_INVALIDARG;

    CIncDisksMixMode* pIncDisks = new CIncDisksMixMode;
    nRet = pIncDisks->Init(pwszFullVhd, pDiffBitmap, pwszIncVhd, pEncInfo);
    if (nRet)
    {
        HALogError(L"GetIncDisks_MixMode failed, full_vhd = %s, inc_vhd = %s, rc = %d", 
            pwszFullVhd, pwszIncVhd, nRet);
        pIncDisks->Release();
        pIncDisks = NULL;
    }

    *ppDiskDiff = pIncDisks;
    return nRet;
}
