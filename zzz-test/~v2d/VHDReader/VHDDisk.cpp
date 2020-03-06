#include "stdafx.h"
#include "VHDDisk.h"
#include "VHDRead.h"
#include <iostream>
using namespace std;


#define PARENT_LOCATOR_WINDOWS_ABSOLUTE 0x57326b75
#define PARENT_LOCATOR_WINDOWS_RELATIVE 0x57327275

/**
 *
 * \param fileName 
 * \return 
 */
VHDDisk::VHDDisk ( wstring fileName ) : 
    m_sFileName ( fileName ), 
    m_hVHDFile  ( INVALID_HANDLE_VALUE ), 
    m_bIsOpen   ( false ),
    m_bHasDynamicHeader ( false ),
    m_pBATTable ( NULL ),
    m_bIsBATRead ( false ),
    m_ulSectorsPerBlock ( 0 ),
    m_llMaxSectors ( 0 ),
    m_ulBlockBitMapSectors ( 0 ),
    m_ulActualBlockBitMapSize ( 0 ),
    m_pBlockBitMap ( NULL ),
    m_lCurrentBlockMapIndex ( -1 ),
    m_pParentDisk ( NULL )
{
}

VHDDisk::~VHDDisk ()
{
    if ( m_pBATTable != NULL )
    {
        delete [] m_pBATTable;
    }

    if ( m_bIsOpen && m_hVHDFile != INVALID_HANDLE_VALUE )
    {
        CloseHandle ( m_hVHDFile );
    }

    if (m_pParentDisk != NULL )
    {
        delete m_pParentDisk;
        m_pParentDisk = NULL;
    }

    if ( m_pBlockBitMap != NULL )
        delete [] m_pBlockBitMap;
}

/**
 *
 * \return 
 */
wstring &VHDDisk::GetFileName ( )
{
    return m_sFileName;
}

/**
 * Open VHD File 
 * \return 
 */
bool VHDDisk::OpenFile ()
{
    if ( m_bIsOpen )
        return true;

    m_hVHDFile = CreateFile ( m_sFileName.c_str(), 
                              GENERIC_READ, 
                              FILE_SHARE_READ | FILE_SHARE_WRITE, 
                              NULL, 
                              OPEN_EXISTING, 
                              FILE_ATTRIBUTE_NORMAL , 
                              NULL) ;

    if ( m_hVHDFile == INVALID_HANDLE_VALUE )
    {
        m_dwLastError = GetLastError ();
        return false;
    } // if ( m_hVHDFile == INVALID_HANDLE_VALUE )

    m_bIsOpen = true;

    return m_bIsOpen;
}

bool VHDDisk::ReadBAT () 
{

    if ( m_pBATTable != NULL )
    {
        delete [] m_pBATTable;
        m_pBATTable = NULL;
    } // if ( m_pBATTable != NULL )

    m_bIsBATRead = false;

    m_pBATTable = new unsigned long [ m_oDynamicHeader.GetMaxTableEntries() ];

    if ( m_pBATTable == NULL )
        return false; // Insufficient memory to allocate BAT Table 
    
    // Position to BAT Table 
    if ( MoveToFileOffset ( m_hVHDFile, m_oDynamicHeader.GetTableOffset() ) != m_oDynamicHeader .GetTableOffset() )
        return false;

    DWORD dwRead = 0;
    if (!ReadFile ( m_hVHDFile, m_pBATTable, m_oDynamicHeader.GetMaxTableEntries() * sizeof(unsigned long), &dwRead, NULL ) )
        return false;

    for ( unsigned long x = 0;
          x < m_oDynamicHeader.GetMaxTableEntries();
          x ++ )
        m_pBATTable [ x ] = ntohl ( m_pBATTable[x] );

    m_bIsBATRead = true;
    return m_bIsBATRead ;

}

/**
 *
 * \return 
 */
bool VHDDisk::ReadHeaders ()
{

    if ( !m_bIsOpen )
        return false;

    if ( !m_oVHDFooter.ReadFooter ( m_hVHDFile ) )
        return false;

    m_llMaxSectors = m_oVHDFooter.GetCurrentSize() / VHD_SECTOR_SIZE;

    if ( m_oVHDFooter.GetDiskType () == VHDFooter::DiskTypeDynamic || 
         m_oVHDFooter.GetDiskType() == VHDFooter::DiskTypeDifferencing )
    {
        if ( !m_oDynamicHeader.Read ( m_hVHDFile, m_oVHDFooter.GetDataOffset() ) )
            return false;
        else
            m_bHasDynamicHeader = true;

        if ( m_oVHDFooter.GetDiskType() == VHDFooter::DiskTypeDifferencing )
        {
            // Differencing disk - open parent disk as well 
            if ( !OpenParentDisk ( ) )
            {
                wcerr << "Failed to open parent disk" << endl;
                return false;
            }
        }

        m_ulSectorsPerBlock = m_oDynamicHeader.GetBlockSize() / VHD_SECTOR_SIZE;
        
        // One bit is required for each sector. So size in bytes is obtained by
        // dividing by 8
        m_ulActualBlockBitMapSize = m_ulSectorsPerBlock / 8;
        m_ulBlockBitMapSectors = m_ulActualBlockBitMapSize;

        // Block Bit Map is expanded to occupy a sector boundry
        if ( m_ulBlockBitMapSectors % VHD_SECTOR_SIZE )
            m_ulBlockBitMapSectors += (VHD_SECTOR_SIZE - (m_ulBlockBitMapSectors % VHD_SECTOR_SIZE));
        m_ulBlockBitMapSectors /= VHD_SECTOR_SIZE;

        m_pBlockBitMap = new unsigned char [m_ulBlockBitMapSectors*VHD_SECTOR_SIZE];
        if ( m_pBlockBitMap == NULL )
        {
            wcerr << "Failed to allocate buffer for block bit map" << endl;
            return false;
        }

        // Read the Block Allocation Table 
        ReadBAT ();
    }
    return true;
}

VHDFooter &VHDDisk::GetFooter ( void ) 
{
    return m_oVHDFooter;
}

VHDDynamicHeader &VHDDisk::GetDynamicHeader ( void ) 
{
    return m_oDynamicHeader;
}

bool VHDDisk::HasDynamicHeader ( void )
{
    return m_bHasDynamicHeader;
}

long long VHDDisk::GetMaxSectors ( void )
{
    return m_llMaxSectors;
}

bool VHDDisk::ReadBlockBitMap ( long long blockIndex )
{
    if ( blockIndex < 0 || blockIndex > m_oDynamicHeader.GetMaxTableEntries()  )
    {
        wcerr << "Block Index " << blockIndex << " out of bounds (0,"<<
            m_oDynamicHeader.GetMaxTableEntries() << ")"<< endl;
        return false;
    }

    if ( blockIndex == m_lCurrentBlockMapIndex )
        return true; // We already have the data 

    long long llSectorNumber = m_pBATTable [ blockIndex ];

    if ( llSectorNumber != 0xffffffff )
    {
        long long llAbsOffset = llSectorNumber * VHD_SECTOR_SIZE;
        if ( MoveToFileOffset ( m_hVHDFile, llAbsOffset, FILE_BEGIN ) != llAbsOffset )
        {
            wcerr << "Failed to position to offset " << llAbsOffset << ". Error = " << GetLastError() << endl;
            return false;
        } // if ( MoveToFileOffset ( m_hVHDFile, llAbsOffset, FILE_BEGIN ) != llAbsOffset )

        DWORD dwRead = 0;
        if ( !ReadFile ( m_hVHDFile, m_pBlockBitMap, m_ulBlockBitMapSectors*VHD_SECTOR_SIZE, &dwRead, NULL) )
        {
            wcerr << "Failed to read " << m_ulBlockBitMapSectors*VHD_SECTOR_SIZE << ". Error=" << GetLastError () << endl;
            return false;
        } // if ( !ReadFile ( m_hVHDFile, m_pBlockBitMap, m_ulBlockBitMapSectors*VHD_SECTOR_SIZE, &dwRead, NULL) )

        // successfully read the block index 
        m_lCurrentBlockMapIndex = blockIndex;
    }
    else
    {
        ZeroMemory ( m_pBlockBitMap, m_ulBlockBitMapSectors*VHD_SECTOR_SIZE );
        m_lCurrentBlockMapIndex = blockIndex;
    }

    return true;
}

/**
 * Is the given sector number populated in the block
 * \param ulSectorNumberInBlock - sector number in block
 * \return true if the sector is populated, false otherwise 
 */
bool VHDDisk::IsSectorValidInBlock ( unsigned long ulSectorNumberInBlock )
{
    if ( ulSectorNumberInBlock < 0 || ulSectorNumberInBlock >= m_ulSectorsPerBlock)
    {
        wcerr << "Invalid sector number " << ulSectorNumberInBlock << ". Valid range (0,"
              << m_ulSectorsPerBlock << ")." << endl;
        return false;
    }

    // Byte in which to find the bit 
    unsigned long ulBitMapByte = ulSectorNumberInBlock / 8;
    unsigned char ucByteInBit  = (unsigned char)(ulSectorNumberInBlock - (ulBitMapByte * 8));

    unsigned char ucMask = 0x80;

    while (ucByteInBit--)
        ucMask >>= 1;

    if ( m_pBlockBitMap [ ulBitMapByte ] & ucMask )
        return true;
    else
        return false;

}

bool VHDDisk::GetSector ( long long llSectorNumber, 
                          BYTE *    bSectorBuffer,
                          size_t    iBufferSize )
{
    if ( m_oVHDFooter.GetDiskType() == VHDFooter::DiskTypeFixed )
    {
        // Fixed Disks come here 
        long long llSeekOffset = llSectorNumber * VHD_SECTOR_SIZE;

        if ( MoveToFileOffset ( m_hVHDFile, llSeekOffset ) != llSeekOffset )
            return false;

        DWORD dwRead = 0;

        if (!ReadFile ( m_hVHDFile, bSectorBuffer, (DWORD)(iBufferSize > 512 ? 512 : iBufferSize), &dwRead, NULL ) )
            return false;        
    }
    else
    {
        // Dynamic and differencing disks come here 
        long long ulBATIndex = (llSectorNumber / m_ulSectorsPerBlock);
        unsigned int uiSectorNumberInBlock = (unsigned int) (llSectorNumber % m_ulSectorsPerBlock);

        // Absolute sector number from start of VHD file 
        unsigned long ulVHDSectorNumber = m_pBATTable [ ulBATIndex ];

        if ( ulVHDSectorNumber != 0xffffffff )
        {
            if ( !ReadBlockBitMap ( ulBATIndex ) )
            {
                wcerr << "Failed to read block bitmap" << endl;
                return false;
            }

            if ( !IsSectorValidInBlock ( uiSectorNumberInBlock ) )
            {
                if ( m_oDynamicHeader.HasParent()  )
                {   
                    if ( m_pParentDisk != NULL )
                        return m_pParentDisk->GetSector ( llSectorNumber, bSectorBuffer, iBufferSize );
                    else
                    {
                        wcerr << "NULL Parent Disk" << endl;
                        return false; 
                    }
                }
                else
                {
                    wcerr << "Sector " << uiSectorNumberInBlock << " is not valid" << endl;
                }
            }
            else
            {
				long long llAbsSectorOffset 
                    = (ulVHDSectorNumber + m_ulBlockBitMapSectors + uiSectorNumberInBlock);
					llAbsSectorOffset *= VHD_SECTOR_SIZE;

                //wcout << "seeking to " << llAbsSectorOffset << endl;
                if ( MoveToFileOffset ( m_hVHDFile, llAbsSectorOffset ) != llAbsSectorOffset )
                {
                    wcerr << "Failed to position to " << llAbsSectorOffset 
                          << ". Error=" << GetLastError() << endl;
                    return false;
                }

                DWORD dwRead = 0;
                if (!ReadFile ( m_hVHDFile, bSectorBuffer, 
                                (DWORD)(iBufferSize > 512 ? 512 : iBufferSize), &dwRead, NULL ) )
                {
                    wcerr << "Failed to read 512 bytes. Error = " << GetLastError () << endl;
                    return false;
                } // if (!ReadFile ( m_hVHDFile, bSectorBuffer, (DWORD)(iBufferSize > 512 ? 512 : iBufferSize), &dwRead, NULL ) )
            }
        }
        else
        {
            if ( m_oDynamicHeader.HasParent() )
            {
                if ( m_pParentDisk != NULL )
                    // Read from the parent 
                    return m_pParentDisk->GetSector ( llSectorNumber, bSectorBuffer, iBufferSize );
                else
                {
                    wcerr << "NULL Parent Disk" << endl;
                    return false;
                }
            } // if ( m_oDynamicHeader.HasParent() )
            else
            {
                // No parent - this buffer is filled with zero
                ZeroMemory ( bSectorBuffer, iBufferSize );
            }
        }
    }

    return true;
}

wstring VHDDisk::ComposeABSpath(wstring sFullPath, wstring sRelativePath)
{
	wstring sAbsPath;
	wstring sRelativePath1;
	wstring sFullPath1;
	int index1 = 0;
	int index2 = 0;

	wstring  sFToken = L"\\";
	wstring  sRToken = L"..\\";
	wstring	 sRToken2 = L".\\";		

	sAbsPath.clear();
	if(sFullPath.c_str()[sFullPath.length()-1] == '\\')
		sFullPath=sFullPath.substr(0, sFullPath.length()-1);

	sFullPath1 = sFullPath;
	sRelativePath1 = sRelativePath;

composePath:
	index2 = sRelativePath1.find(sRToken);

	//take care relative path as ".\XXX"
	if(index2 == -1)
	{
		index2 = sRelativePath1.find(sRToken2);
		if(index2 != -1)
		{
			sAbsPath = sFullPath1 + L"\\" + sRelativePath1.substr(index2+sRToken2.length(), sRelativePath1.length());
			return sAbsPath;
		}
	}

	index1 = sFullPath1.rfind(sFToken);

	if ( (index2 != -1) && (index1 != -1) )
	{
		sFullPath1 = sFullPath1.substr(0, index1);
		sRelativePath1 = sRelativePath1.substr(index2+sRToken.length(), sRelativePath1.length());
		goto composePath;
	}
	else if(index2 == -1)
	{
		sAbsPath = sFullPath1 + L"\\" + sRelativePath1;
	}
	else
	{
		return NULL;
	}

	return sAbsPath;
}

bool VHDDisk::OpenParentDisk ( )
{
	
    if ( m_oVHDFooter.GetDiskType() != VHDFooter::DiskTypeDifferencing )
        return false;

    if (m_pParentDisk != NULL )
    {
        delete m_pParentDisk;
        m_pParentDisk = NULL;
    } // if (m_pParentDisk != NULL )

    // First try to open using absoulute path 

    for ( int parent = 0; parent < 8; parent++ )
    {

        if ( m_oDynamicHeader.GetParentLocator(parent).platformCode 
            == PARENT_LOCATOR_WINDOWS_ABSOLUTE )
        {
            m_pParentDisk = new VHDDisk ( m_oDynamicHeader.GetParentPath (parent) );

            if ( !m_pParentDisk->OpenFile( ) )
            {
                wcerr << "Unable to open parent disk " 
                      << m_oDynamicHeader.GetParentPath (parent) << endl;
                delete m_pParentDisk;
                m_pParentDisk = NULL;
                continue;
            } // if ( m_pParentDisk->OpenParentDisk( ) )

            if ( !m_pParentDisk->ReadHeaders() )
            {
                wcerr << "Failed to open parent disk " 
                      << m_oDynamicHeader.GetParentPath(parent) << endl;
                delete m_pParentDisk;
                m_pParentDisk = NULL;
                continue;
            } // if ( !m_pParentDisk->ReadHeaders() )

            // Verify that the GUIDs match 
            if ( !IsEqualGUID ( m_pParentDisk->GetFooter().GetUUID(), 
                                m_oDynamicHeader.GetParentUUID() ) )
            {
                wcerr << "Parent GUID " <<  GetGUIDString ( m_pParentDisk->GetFooter().GetUUID() ) << " does not match "
                    << " the parent GUID " << GetGUIDString ( m_oDynamicHeader.GetParentUUID() ) << " stored in this disk " << endl;
                delete m_pParentDisk;
                m_pParentDisk = NULL;
                continue;
            }

            // We opened the VHD using absolute path 
            wcout << "Parent disk " << m_oDynamicHeader.GetParentPath (parent)  
                  << " opened " << endl;
            break;
        }
    } // for ( int parent = 0; parent < 8; parent++ )

    if ( m_pParentDisk == NULL )
    {

        wchar_t sFullPath [ MAX_PATH * 2 + 1];
        wchar_t *pAbsPath = _wfullpath ( sFullPath, m_sFileName.c_str(), _countof(sFullPath));
        
        wchar_t sDrive     [ 10 ];
        wchar_t sDirectory [ MAX_PATH ];
        wchar_t sName      [ MAX_PATH ];
        wchar_t sExtension [ MAX_PATH ];

        _wsplitpath_s ( sFullPath, sDrive, sDirectory, sName, sExtension );
        
        _snwprintf_s ( sFullPath, _countof ( sFullPath ), _TRUNCATE,
                       L"%s%s", sDrive, sDirectory );

        wstring sBasePath = sFullPath;

        // Failed to open parent disk using absoulte path. Let us now try relative paths 
        for ( int parent = 0; parent < 8; parent++ )
        {
            if ( m_oDynamicHeader.GetParentLocator(parent).platformCode 
                    == PARENT_LOCATOR_WINDOWS_RELATIVE )
            {
                wstring sRelativePath = m_oDynamicHeader.GetParentPath(parent);

				wstring sAbsPath = ComposeABSpath(sBasePath, sRelativePath);
				if(sAbsPath.empty())
					sAbsPath = sBasePath + sRelativePath;

                m_pParentDisk = new VHDDisk ( sAbsPath );

                // Open Parent Disk 
                if ( !m_pParentDisk->OpenFile( ) )
                {
                    wcerr << "Unable to open parent disk " 
                        << sAbsPath << endl;
                    delete m_pParentDisk;
                    m_pParentDisk = NULL;
                    continue;
                } // if ( m_pParentDisk->OpenParentDisk( ) )

                // Read headers 
                if ( !m_pParentDisk->ReadHeaders() )
                {
                    wcerr << "Failed to open parent disk " 
                          << sAbsPath << endl;
                    delete m_pParentDisk;
                    m_pParentDisk = NULL;
                    continue;
                } // if ( !m_pParentDisk->ReadHeaders() )

                // Verify that the GUIDs match 
                if ( !IsEqualGUID ( m_pParentDisk->GetFooter().GetUUID(), 
                    m_oDynamicHeader.GetParentUUID() ) )
                {
                    wcerr << "Parent GUID " << GetGUIDString ( m_pParentDisk->GetFooter().GetUUID() ) << " does not match "
                          << " the parent GUID " << GetGUIDString ( m_oDynamicHeader.GetParentUUID() ) << " stored in this disk " << endl;
                    delete m_pParentDisk;
                    m_pParentDisk = NULL;
                    continue;
                }

                // We opened the VHD using absolute path 
                wcout << "Parent disk " << sAbsPath  
                      << " opened " << endl;
                break;
            }
        }
    }
    else
        return true;

	//retry the absolute path
	if ( m_pParentDisk == NULL )
    {

		for ( int parent = 0; parent < 8; parent++ )
		{
			if ( m_oDynamicHeader.GetParentLocator(parent).platformCode 
				== PARENT_LOCATOR_WINDOWS_ABSOLUTE )
			{
				wchar_t vssPath[1024] = {0};
				wstring sParent = m_oDynamicHeader.GetParentPath (parent);

				GetVSSPath((wchar_t*)sParent.c_str(), vssPath, 1024);
				m_pParentDisk = new VHDDisk ( vssPath );

				if ( !m_pParentDisk->OpenFile( ) )
				{
					wcerr << "Unable to open parent disk " 
						  << m_oDynamicHeader.GetParentPath (parent) << endl;
					delete m_pParentDisk;
					m_pParentDisk = NULL;
					continue;
				} // if ( m_pParentDisk->OpenParentDisk( ) )

				if ( !m_pParentDisk->ReadHeaders() )
				{
					wcerr << "Failed to open parent disk " 
						  << m_oDynamicHeader.GetParentPath(parent) << endl;
					delete m_pParentDisk;
					m_pParentDisk = NULL;
					continue;
				} // if ( !m_pParentDisk->ReadHeaders() )

				// Verify that the GUIDs match 
				if ( !IsEqualGUID ( m_pParentDisk->GetFooter().GetUUID(), 
									m_oDynamicHeader.GetParentUUID() ) )
				{
					wcerr << "Parent GUID " <<  GetGUIDString ( m_pParentDisk->GetFooter().GetUUID() ) << " does not match "
						<< " the parent GUID " << GetGUIDString ( m_oDynamicHeader.GetParentUUID() ) << " stored in this disk " << endl;
					delete m_pParentDisk;
					m_pParentDisk = NULL;
					continue;
				}

				// We opened the VHD using absolute path 
				wcout << "Parent disk " << m_oDynamicHeader.GetParentPath (parent)  
					  << " opened " << endl;
				break;
			}
		} // for ( int parent = 0; parent < 8; parent++ )



	}

    if ( m_pParentDisk == NULL )
        return false;
    else 
        return true;
}

wstring VHDDisk::GetDiskHierarchy ( void )
{
    wstring sPath = L"";

    if ( m_pParentDisk != NULL )
        sPath = m_pParentDisk->GetDiskHierarchy();

    if ( sPath.length () )
		sPath += L"*";

    sPath += m_sFileName;

    return sPath;
}


