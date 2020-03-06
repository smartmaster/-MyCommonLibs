#include "stdafx.h"
#include "VHDDynamicHeader.h"
#include <iostream>
using namespace std;

VHDDynamicHeader::VHDDynamicHeader () : m_bRead ( false ), m_bHasParent ( false )
{
    ZeroMemory ( &m_oDynamicHeader, sizeof ( m_oDynamicHeader) );
};
VHDDynamicHeader::~VHDDynamicHeader()
{
};

bool VHDDynamicHeader::IsRead (void) const
{
    return m_bRead;
};

bool VHDDynamicHeader::Read   (HANDLE hFile, long long llHeaderOffset)
{
    DWORD dwRead = 0;

    if ( MoveToFileOffset ( hFile, llHeaderOffset ) != llHeaderOffset )
        return false;

    if (!ReadFile ( hFile, &m_oDynamicHeader, sizeof(m_oDynamicHeader), &dwRead, NULL ))
    {
        m_dwLastError = GetLastError ();
        return false;
    } // if (!ReadFile ( hFile, &m_oFooter, sizeof(m_oFooter), &dwRead, NULL ))

    // Normalize 
    NetworkToHost ();

    wchar_t parentPathBuffer [ 512 ];
    for ( int parent = 0; parent < 8; parent++ )
    {
        if (m_oDynamicHeader.parentLocator[parent].platformCode == 0)
        {
            m_sParentPaths[parent] = L"";
            continue;
        } // if (m_oDynamicHeader.parentLocator[parent].platformCode == 0)

        if ( MoveToFileOffset ( hFile, m_oDynamicHeader.parentLocator[parent].platformDataOffset ) 
             != m_oDynamicHeader.parentLocator[parent].platformDataOffset  )
        {
            wcerr << "Failed to position to parent path location " << 
                m_oDynamicHeader.parentLocator[parent].platformDataOffset  << endl;
            m_sParentPaths[parent] = L"";
            continue;
        } // formDataOffset  )

        ZeroMemory ( parentPathBuffer, sizeof(parentPathBuffer));

        if ( !ReadFile ( hFile, parentPathBuffer, 
                         m_oDynamicHeader.parentLocator[parent].platformDataLength,
                         &dwRead, NULL ))
        {
            m_dwLastError = GetLastError ();
            wcerr << "Failed to read parent path entry " << parent << " from location " 
                  << m_oDynamicHeader.parentLocator[parent].platformDataOffset 
                  << ". Error = " << m_dwLastError << endl;
            m_sParentPaths[parent] = L"";
            continue;
        }
        m_sParentPaths [ parent ] = parentPathBuffer;
    }

    m_bRead = true;

    return m_bRead;
};

void VHDDynamicHeader::NetworkToHost (void)
{
    m_oDynamicHeader.dataOffset = ntohll(m_oDynamicHeader.dataOffset);
    m_oDynamicHeader.tableOffset = ntohll(m_oDynamicHeader.tableOffset);
    m_oDynamicHeader.headerVersion = ntohl(m_oDynamicHeader.headerVersion);
    m_oDynamicHeader.maxTableEntries = ntohl(m_oDynamicHeader.maxTableEntries);
    m_oDynamicHeader.blockSize = ntohl(m_oDynamicHeader.blockSize);
    m_oDynamicHeader.checkSum = ntohl(m_oDynamicHeader.checkSum);
    m_oDynamicHeader.parentTimeStamp = ntohl(m_oDynamicHeader.parentTimeStamp);

    m_oDynamicHeader.parentUUID = GuidNtoH (m_oDynamicHeader.parentUUID ) ;

    if ( IsEqualGUID ( m_oDynamicHeader.parentUUID, GUID_NULL ) )
        m_bHasParent = false;
    else
        m_bHasParent = true;

    m_iParentPaths = 0;
    for ( unsigned x = 0; x < 8; x ++ )
    {
        m_oDynamicHeader.parentLocator[x].platformCode = ntohl ( m_oDynamicHeader.parentLocator[x].platformCode  );
        m_oDynamicHeader.parentLocator[x].platformDataLength = ntohl ( m_oDynamicHeader.parentLocator[x].platformDataLength );
        m_oDynamicHeader.parentLocator[x].platformDataOffset = ntohll ( m_oDynamicHeader.parentLocator[x].platformDataOffset );
        m_oDynamicHeader.parentLocator[x].platformDataSpace  = ntohl ( m_oDynamicHeader.parentLocator[x].platformDataSpace );

    } // for ( int x = 0; x < 8; x ++ )

    for ( unsigned x = 0; x < _countof ( m_oDynamicHeader.parentUnicodeName ); x++ )
        m_oDynamicHeader.parentUnicodeName[x] = ntohs ( m_oDynamicHeader.parentUnicodeName[x]  );
}

string VHDDynamicHeader::GetCookie	()
{
    char buf [ sizeof(m_oDynamicHeader.cookie) + 1 ];

    ZeroMemory ( buf, sizeof(m_oDynamicHeader.cookie) + 1 );
    strncpy_s ( buf, sizeof(m_oDynamicHeader.cookie) + 1 , m_oDynamicHeader.cookie, sizeof(m_oDynamicHeader.cookie) );

    return buf;;
};

long long VHDDynamicHeader::GetDataOffset ()
{
    return m_oDynamicHeader.dataOffset;
};

long long VHDDynamicHeader::GetTableOffset ()
{
    return m_oDynamicHeader.tableOffset;
};
unsigned long VHDDynamicHeader::GetHeaderVersion ()
{
    return m_oDynamicHeader.headerVersion;
};
unsigned long VHDDynamicHeader::GetMaxTableEntries ()
{
    return m_oDynamicHeader.maxTableEntries;
};
unsigned long VHDDynamicHeader::GetBlockSize ()
{
    return m_oDynamicHeader.blockSize;
};
unsigned long VHDDynamicHeader::GetCheckSum ()
{
    return m_oDynamicHeader.checkSum;
};
GUID VHDDynamicHeader::GetParentUUID ()
{
    return m_oDynamicHeader.parentUUID;
};
unsigned long VHDDynamicHeader::GetParentTimeStamp ()
{
    return m_oDynamicHeader.parentTimeStamp;
};
wstring VHDDynamicHeader::GetParentUnicodeName ()
{
    return m_oDynamicHeader.parentUnicodeName;
};
VHDParentLocatorEntry & VHDDynamicHeader::GetParentLocator ( int index )
{
    return m_oDynamicHeader.parentLocator [ index ];
};

bool VHDDynamicHeader::HasParent ( void ) const
{
    return m_bHasParent;
}

wstring VHDDynamicHeader::GetParentPath ( int index )
{
    if ( index < 0 || index >= 8 )
        return L"";
    else
        return m_sParentPaths[index];
}