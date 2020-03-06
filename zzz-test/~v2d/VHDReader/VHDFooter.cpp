#include "stdafx.h"

#include "VHDFooter.h"

/**
 *
 * \return 
 */
VHDFooter::VHDFooter () : m_bIsRead ( false )
{
    ZeroMemory ( &m_oFooter, sizeof(m_oFooter) );
}

/**
 *
 * \return 
 */
VHDFooter::~VHDFooter ()
{
}

/**
 *
 * \param hFile 
 * \return 
 */
bool VHDFooter::ReadFooter ( HANDLE hFile )
{
    DWORD dwRead = 0;

    // Footer is always last 512 bytes of the file
    MoveToFileOffset ( hFile, ((-1) * (long)sizeof(m_oFooter)), FILE_END );

    if (!ReadFile ( hFile, &m_oFooter, sizeof(m_oFooter), &dwRead, NULL ))
    {
        m_dwLastError = GetLastError ();
        return false;
    } // if (!ReadFile ( hFile, &m_oFooter, sizeof(m_oFooter), &dwRead, NULL ))

    ATLASSERT ( dwRead == sizeof(m_oFooter) );

    // Normalize 
    NetworkToHost ();

    m_bIsRead = true;

    return m_bIsRead;
}
 
/**
 * Is the footer read?
 * \param void 
 * \return 
 */
bool VHDFooter::IsRead(void)
{
    return m_bIsRead;
}

/**
 *
 */
void VHDFooter::NetworkToHost ()
{
    unsigned long lTestChecksum   = ComputeCheckSum ( &m_oFooter );

    m_oFooter.features            = ntohl  (m_oFooter.features );
    m_oFooter.fileFormatVersion   = ntohl  (m_oFooter.fileFormatVersion );
    m_oFooter.dataOffset          = ntohll (m_oFooter.dataOffset);
    m_oFooter.timeStamp           = ntohl  (m_oFooter.timeStamp );
    m_oFooter.creatorVersion      = ntohl  (m_oFooter.creatorVersion );
    m_oFooter.originalSize        = ntohll (m_oFooter.originalSize );
    m_oFooter.currentSize         = ntohll (m_oFooter.currentSize );
    m_oFooter.diskGemoetry        = ntohl  (m_oFooter.diskGemoetry );
    m_oFooter.diskType            = ntohl  (m_oFooter.diskType );
    m_oFooter.checksum            = ntohl  (m_oFooter.checksum );

    ATLASSERT ( m_oFooter.checksum == lTestChecksum );

    m_oFooter.uuid                = GuidNtoH ( m_oFooter.uuid );
}

/**
 * Convert to network byte order 
 * \param void 
 */
void VHDFooter::HostToNetwork (void)
{
    m_oFooter.checksum = ComputeCheckSum ( &m_oFooter );

    m_oFooter.features            = htonl  (m_oFooter.features );
    m_oFooter.fileFormatVersion   = htonl  (m_oFooter.fileFormatVersion );
    m_oFooter.dataOffset          = htonll (m_oFooter.dataOffset);
    m_oFooter.timeStamp           = htonl  (m_oFooter.timeStamp );
    m_oFooter.creatorVersion      = htonl  (m_oFooter.creatorVersion );
    m_oFooter.originalSize        = htonll (m_oFooter.originalSize );
    m_oFooter.currentSize         = htonll (m_oFooter.currentSize );
    m_oFooter.diskGemoetry        = htonl  (m_oFooter.diskGemoetry );
    m_oFooter.diskType            = htonl  (m_oFooter.diskType );
    m_oFooter.checksum            = htonl  (m_oFooter.checksum );

    m_oFooter.uuid                = GuidHtoN ( m_oFooter.uuid );
}


/**
 *
 * \param void 
 * \return 
 */
string VHDFooter::GetCookie ( void ) const 
{
    char buf [ sizeof(m_oFooter.cookie)+1 ];

    strncpy_s ( buf, sizeof(m_oFooter.cookie)+1 , m_oFooter.cookie, sizeof(m_oFooter.cookie) );

    string sReturn = (char *)buf;

    return sReturn;
};

/**
 *
 * \param void 
 * \return 
 */
string VHDFooter::GetCreatorApplication ( void ) const
{
    char buf [ sizeof(m_oFooter.creatorApplication)+1 ];

    strncpy_s ( buf, sizeof(m_oFooter.creatorApplication)+1 , m_oFooter.creatorApplication, sizeof(m_oFooter.creatorApplication) );

    string sReturn = (char *)buf;

    return sReturn;
};

/**
 *
 * \param void 
 * \return 
 */
string VHDFooter::GetCreatorHostOS ( void ) const
{
    char buf [ sizeof(m_oFooter.createrHostOS)+1 ];

    strncpy_s ( buf, sizeof(m_oFooter.createrHostOS)+1 , m_oFooter.createrHostOS, sizeof(m_oFooter.createrHostOS) );

    string sReturn = (char *)buf;

    return sReturn;
};

/**
 *
 * \param void 
 * \return 
 */
unsigned long VHDFooter::GetFeatures ( void ) const
{
    return m_oFooter.features;
};

/**
 *
 * \param void 
 * \return 
 */
unsigned long VHDFooter::GetFileFormatVersion ( void ) const
{
    return m_oFooter.fileFormatVersion;
};

/**
 *
 * \param void 
 * \return 
 */
long long VHDFooter::GetDataOffset ( void ) const
{
    return m_oFooter.dataOffset;
};

/**
 *
 * \param void 
 * \return 
 */
unsigned long VHDFooter::GetTimeStamp ( void ) const
{
    return m_oFooter.timeStamp;
};

/**
 *
 * \param void 
 * \return 
 */
unsigned long VHDFooter::GetCreatorVersion ( void ) const
{
    return m_oFooter.creatorVersion;
};

/**
 *
 * \param void 
 * \return 
 */
long long VHDFooter::GetOriginalSize ( void ) const
{
    return m_oFooter.originalSize;
};

/**
 *
 * \param void 
 * \return 
 */
long long VHDFooter::GetCurrentSize ( void ) const
{
    return m_oFooter.currentSize;
};

/**
 *
 * \param void 
 * \return 
 */
unsigned long VHDFooter::GetDiskGeometry ( void ) const
{
    return m_oFooter.diskGemoetry;
};

/**
 *
 * \param void 
 * \return 
 */
unsigned long VHDFooter::GetDiskType ( void ) const
{
    return m_oFooter.diskType;
};

/**
 *
 * \param void 
 * \return 
 */
unsigned long VHDFooter::GetCheckSum ( void ) const
{
    return m_oFooter.checksum;
};

/**
 *
 * \param void 
 * \return 
 */
GUID VHDFooter::GetUUID ( void ) const
{
    return m_oFooter.uuid;
};

/**
 *
 * \param void 
 * \return 
 */
bool VHDFooter::IsInSavedState ( void ) const
{
    return m_oFooter.savedState == 1 ? true : false;
};

/**
 *
 * \param dwDiskType 
 * \return 
 */
wstring VHDFooter::GetDiskTypeName ( DWORD dwDiskType )
{
    switch ( dwDiskType )
    {
    case DiskTypeNone:
        return L"None";
    case DiskTypeReserved1:
        return L"Reserved (deprecated-1)";
    case DiskTypeFixed:
        return L"Fixed hard disk";
    case DiskTypeDynamic:
        return L"Dynamic hard disk";
    case DiskTypeDifferencing:
        return L"Differencing hard disk";
    case DiskTypeReserved5:
        return L"Reserved (deprecated-5)";
    case DiskTypeReserved6:
        L"Reserved (deprecated-5)";
    default:
        return L"Unknown-default";
    }

    return L"Unknown";
}

/**
 *
 * \param pFooter 
 * \return 
 */
unsigned long VHDFooter::ComputeCheckSum ( PVHDHardDiskFooter pFooter )
{
    VHDHardDiskFooter oTempFooter;
    
    memcpy ( &oTempFooter, pFooter, sizeof(VHDHardDiskFooter));

    oTempFooter.checksum = 0;

    BYTE *pBuffer = (BYTE *)&oTempFooter;
    unsigned long lSum = 0;

    for ( int x=0; x < sizeof(VHDHardDiskFooter); x++ )
        lSum += pBuffer[x];

    unsigned long computedChecksum = ~lSum;

    return computedChecksum;
}

/**
 * Assignment operator 
 * \param &rhs - source of assignment
 * \return - reference to self
 */
VHDFooter & VHDFooter::operator = ( const VHDFooter &rhs )
{
    // copy the backing structure 
    memcpy ( &m_oFooter, &rhs.m_oFooter, sizeof(m_oFooter) );
    return *this;
}

VHDFooter::VHDFooter ( const VHDFooter &rhs )
{
    // copy the backing structure 
    memcpy ( &m_oFooter, &rhs.m_oFooter, sizeof(m_oFooter) );
}

/**
 * Set a new disk type 
 * \param &newType 
 * \return 
 */
unsigned long VHDFooter::SetDiskType ( VHDDiskType newType ) 
{
    m_oFooter.diskType = newType;
    return m_oFooter.diskType;
}

bool VHDFooter::WriteFooter ( HANDLE hFile )
{
    DWORD dwWrote = 0;

    // Footer is always last 512 bytes of the file
    MoveToFileOffset ( hFile, 0, FILE_END );

    HostToNetwork ();
    if (!WriteFile ( hFile, &m_oFooter, sizeof(m_oFooter), &dwWrote, NULL ))
    {
        m_dwLastError = GetLastError ();
        return false;
    } // if (!ReadFile ( hFile, &m_oFooter, sizeof(m_oFooter), &dwRead, NULL ))

    ATLASSERT ( dwWrote == sizeof(m_oFooter) );

    // Normalize 
    NetworkToHost ();

    return true;
}

bool VHDFooter::GenerateFixedDiskFooter (PVHDHardDiskFooter pFooter)
{
	CopyMemory(pFooter, &m_oFooter, sizeof(VHDHardDiskFooter));
    return true;
}
long long VHDFooter::SetDataOffset ( long long newDataOffset )
{
    return m_oFooter.dataOffset = newDataOffset;
}

bool VHDFooter::SetSavedState ( bool newState ) 
{
    m_oFooter.savedState = (newState ? 1 : 0 );

    return m_oFooter.savedState == 1 ? true : false;
}

VHDFooter::PVHDHardDiskFooter VHDFooter::GetVHDHardDiskFooter()
{
	return &m_oFooter;
}