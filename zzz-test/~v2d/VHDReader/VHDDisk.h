#pragma once
#include <WINDOWS.h>
#include <string>

#include "VHDFooter.h"
#include "VHDDynamicHeader.h"

#define VHD_SECTOR_SIZE     512

using namespace std;

class VHDDisk
{
public:
    VHDDisk ( wstring fileName );
    ~VHDDisk ();

    wstring &GetFileName ( );
    bool OpenFile ();

    VHDFooter &GetFooter ( void ) ;
    VHDDynamicHeader &GetDynamicHeader ( void ) ;
    bool ReadHeaders ();
    bool HasDynamicHeader ( void );

    long long GetMaxSectors ( void );

    bool GetSector ( long long llSectorNumber, 
                     BYTE *    bSectorBuffer,
                     size_t    iBufferSize );

    wstring GetDiskHierarchy ( void );
	wstring ComposeABSpath(wstring sFullPath, wstring sRelativePath);

private:

    bool ReadBAT ();
    bool ReadBlockBitMap ( long long blockIndex );
    bool IsSectorValidInBlock ( unsigned long ulSectorNumberInBlock );
    bool OpenParentDisk ( );

    wstring                 m_sFileName;
    HANDLE                  m_hVHDFile;
    bool                    m_bIsOpen;
    bool                    m_bHasDynamicHeader;
    bool                    m_bIsBATRead;
    DWORD                   m_dwLastError;
    VHDFooter               m_oVHDFooter;
    VHDDynamicHeader        m_oDynamicHeader;
    unsigned long *         m_pBATTable;
    long long               m_llMaxSectors;
    long long               m_lCurrentBlockMapIndex;
    unsigned long           m_ulSectorsPerBlock;
    unsigned long           m_ulBlockBitMapSectors;
    unsigned long           m_ulActualBlockBitMapSize;
    unsigned char *         m_pBlockBitMap;
    VHDDisk *               m_pParentDisk;
};