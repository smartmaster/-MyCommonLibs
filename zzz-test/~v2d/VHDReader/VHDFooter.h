#pragma once 

#include "stdafx.h"
#include "utils.h"

using namespace std;


class VHDFooter 
{
public:
    VHDFooter ();
    VHDFooter ( const VHDFooter &rhs );
    ~VHDFooter ();

    bool IsRead(void);
    bool ReadFooter ( HANDLE hFile );
    bool WriteFooter ( HANDLE hFile );

    enum VHDDiskType
    {
        DiskTypeNone            = 0,
        DiskTypeReserved1       = 1,
        DiskTypeFixed           = 2,
        DiskTypeDynamic         = 3,
        DiskTypeDifferencing    = 4,
        DiskTypeReserved5       = 5,
        DiskTypeReserved6       = 6
    };

    string         GetCookie                ( void ) const;
    string         GetCreatorApplication    ( void ) const;
    string         GetCreatorHostOS         ( void ) const;
    unsigned long   GetFeatures             ( void ) const;
    unsigned long   GetFileFormatVersion    ( void ) const;
    long long       GetDataOffset           ( void ) const;
    unsigned long   GetTimeStamp            ( void ) const;
    unsigned long   GetCreatorVersion       ( void ) const;
    long long       GetOriginalSize         ( void ) const;
    long long       GetCurrentSize          ( void ) const;
    unsigned long   GetDiskGeometry         ( void ) const;
    unsigned long   GetDiskType             ( void ) const;
    unsigned long   GetCheckSum             ( void ) const;
    GUID            GetUUID                 ( void ) const;
    bool            IsInSavedState          ( void ) const;

    unsigned long   SetDiskType ( VHDDiskType newType ) ;
    long long       SetDataOffset ( long long newDataOffset );
    bool            SetSavedState ( bool newState );

    // assignment operator 
    VHDFooter & operator = ( const VHDFooter &rhs );

    static wstring GetDiskTypeName ( DWORD dwDiskType );
	/**
	 * VHD Header and Footer Format. Same structure is kept in header and footer
	 */
	typedef struct _VHDHardDiskFooterFormat
	{
		char			cookie [ 8 ];				// 8
		unsigned long	features;					// 4	12
		unsigned long	fileFormatVersion;			// 4	16
		long long		dataOffset; 				// 8	24
		unsigned long	timeStamp;					// 4	28
		char			creatorApplication[4];		// 4	32
		unsigned long	creatorVersion; 			// 4	36
		char			createrHostOS[4];			// 4	40
		long long		originalSize;				// 8	48
		long long		currentSize;				// 8	56
		unsigned long	diskGemoetry;				// 4	60
		unsigned long	diskType;					// 4	64
		unsigned long	checksum;					// 4	68
		GUID			uuid;						// 16	84
		unsigned char	savedState; 				// 1	85
		unsigned char	reserved [ 427 ];			// 427	512
	} VHDHardDiskFooter, 
	 *PVHDHardDiskFooter;
	
	bool GenerateFixedDiskFooter (PVHDHardDiskFooter pFooter);
    void NetworkToHost (void);
    void HostToNetwork (void);
	PVHDHardDiskFooter GetVHDHardDiskFooter();
	unsigned long ComputeCheckSum ( PVHDHardDiskFooter pFooter );
 private:

    VHDHardDiskFooter   m_oFooter;
    DWORD               m_dwLastError;
    bool                m_bIsRead;
};
