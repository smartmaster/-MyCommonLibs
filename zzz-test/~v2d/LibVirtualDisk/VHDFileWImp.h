/******************************************************************************
*
*        filename  :      VHDFileWImp.h
*        created   :      2010/06/10
*        Written by:      baide02
*        comment   :      class to write dynamic and differential VHD.
*
******************************************************************************/
#ifndef _DYNVHD_FILEW_IMPL_H_
#define _DYNVHD_FILEW_IMPL_H_
#include "HAVhdUtility.h"
#include "VHDDef.h"
#include <string>
using std::wstring;
using namespace VHDConv;

/**
* Represent both dynamic and diff vhd
*/
class CDynVHDFileW : public IVHDFileW
{
public:
    CDynVHDFileW(void);
    virtual ~CDynVHDFileW(void);
public:
    virtual int Release();
    virtual int Write(const char* pBuf, unsigned long cbToWrite,
        unsigned long* pcbWritten, __int64 llStartOffset);
    virtual int Seek(__int64 llOffset, __int64* pllNewPos, int nMethod);
public:
    int Init(const wchar_t*      pwszFilePath, 
             const ST_CREATE_PARMS*    pstCreateParms);
public:
   int Init(const wchar_t*         pwszFilePath, 
            const ST_CREATE_PARMS* pstCreateParms,
            const wchar_t*         pwszParentPath);
protected:
    int Create(const wchar_t* pwszFilePath, const ST_CREATE_PARMS* pstCreateParms,
               const wchar_t* pwszParentPath);

    int Close();
    int InitializeFooter(VHD_TYPE eType, int nVer, __int64 llDiskSize, const GUID& uniqueID);
    int InitializeHeader(VHD_TYPE eType, __int64 llDiskSize, const wchar_t* pwszParentPath);
    int WriteFooterAt(const VHDFOOTER* pstFooter, __int64 llOffset);
    int WriteHeaderAt(const VHDDYNHEADER* pstHeader, __int64 llOffset);
    int CreateBAT();
    int WriteBAT();
    int ReadBAT();

    // @nSize - the size of buffer pointed by pBuf, in bytes
    int ReadBitmap(__int64 llBlk, char* pBuf, int nBufSize);
    int WriteBitmap(__int64 llBlk, const char* pBuf, int nBufSize);
    int ReadFooter(VHDFOOTER* pstFooter, __int64 llOffset);
    int ReadHeader(VHDDYNHEADER* pstHeader);

    int GetDataEndPos(__int64& llPos);      // The end of disk data pos
    int AllocateNewBlock(__int64 llBlk);
    int WriteParentLocators(const wchar_t* pwszParentPath);
    int WriteParentLocatorAt(const wchar_t* pwszParentPath, 
                             int nPlatCode, VHDPARENTLOCATOR* pLoc, __int64 llOffset);

    // Double check the footer, such as checksum error, or the footer does not exist at the
    // end of file. and try repair the footer.
    // return 0 means no error in footer or the footer is repaired successfully.
    //        others means failed to repair the footer.
    int DoubleCheckFooter(); 







    int WriteOut(const void* pBuf, unsigned long cbToWrite, __int64 llOffset);
    int ReadIn(void* pBuf, unsigned long cbToRead,
        unsigned long* pcbRead, __int64 llOffset);

protected:
    wstring             m_wstrFilePath;
    ST_CREATE_PARMS     m_stCreateParms;
    __int64             m_llCurrDiskOffset;     //Current offset in disk        
    HANDLE              m_hVhd;

    VHDFOOTER           m_stFooter;
    VHDDYNHEADER        m_stHeader;
    unsigned int*       m_pBAT;                 //VHD's max size is 2T bytes. so the max sector is 4G.
    int                 m_nSPB;                 //sector per block
    char*               m_pBitmap;
    __int64             m_llCurrBitmapIdx;      //the current block idx of m_pBitmap
    
};




#endif //_DYNVHD_FILEW_IMPL_H_