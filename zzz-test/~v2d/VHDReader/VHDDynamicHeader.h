#pragma once
#include "stdafx.h"
#include "utils.h"
/**
 * VHD Parent Locator Entry -- size 24 bytes
 */
typedef struct _VHDParentLocatorEntry
{
    unsigned long   platformCode;       // 4
    unsigned long   platformDataSpace;  // 4    8
    unsigned long   platformDataLength; // 4    12
    unsigned char   reserved [ 4 ];     // 4    16
    long long       platformDataOffset; // 8    24
} VHDParentLocatorEntry, 
 *PVHDParentLocatorEntry;class VHDDynamicHeader 
{
public:
    VHDDynamicHeader ();
    ~VHDDynamicHeader();
    
    bool IsRead     ( void ) const;
    bool Read       ( HANDLE hFile, long long llHeaderOffset );
    bool HasParent  ( void ) const;

    string                  GetCookie	();
    long long               GetDataOffset ();
    long long               GetTableOffset ();
    unsigned long           GetHeaderVersion ();
    unsigned long           GetMaxTableEntries ();
    unsigned long           GetBlockSize ();
    unsigned long           GetCheckSum ();
    GUID                    GetParentUUID ();
    unsigned long           GetParentTimeStamp ();
    wstring                 GetParentUnicodeName ();
    VHDParentLocatorEntry & GetParentLocator ( int index );
    wstring                 GetParentPath ( int index );


private:

    /**
     * VHD Dynamic / Differencing Disk header
     */
    typedef struct _VHDDynamicDiskHeader
    {
        char                    cookie	[8];                // 8    8
        long long               dataOffset;                 // 8    16
        long long               tableOffset;                // 8    24
        unsigned long           headerVersion;              // 4    28
        unsigned long           maxTableEntries;            // 4    32
        unsigned long           blockSize;                  // 4    36
        unsigned long           checkSum;                   // 4    40
        GUID                    parentUUID;                 // 16   56
        unsigned long           parentTimeStamp;            // 4    60
        unsigned char           reserved [ 4 ];             // 4    64
        wchar_t                 parentUnicodeName [ 256 ];  // 512  576
        VHDParentLocatorEntry   parentLocator[8];           // 24*8=192    768
        unsigned char           reserved2 [ 256 ];          // 256  1024
    } VHDDynamicDiskHeader, 
     *PVHDDynamicDiskHeader;

    void NetworkToHost (void);

    VHDDynamicDiskHeader    m_oDynamicHeader;
    bool                    m_bRead;
    bool                    m_bHasParent;
    DWORD                   m_dwLastError;
    wstring                 m_sParentPaths [ 8 ];
    unsigned char           m_iParentPaths;
};