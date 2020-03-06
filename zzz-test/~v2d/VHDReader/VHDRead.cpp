#include "stdafx.h"
#include <string>
#include <iostream>
#include "VHDRead.h"
#include "VHDDisk.h"
using namespace std;

#define VHD_HANDLE_SIGNATURE 0xA1B2C2D4

typedef struct _VHD_HANDLE_STRUCTURE
{
    int signature;
    VHDDisk *pDisk;
} VHD_HANDLE_STRUCTURE, *PVHD_HANDLE_STRUCTURE; 

#define MAX_NT_PATH		512
typedef struct LogicalPhysicalPathTriplet_
{
	WCHAR LogicalPath [MAX_NT_PATH+1];
	WCHAR PhysicalPath[MAX_NT_PATH+1];
	WCHAR MountedPath[MAX_NT_PATH+1];
}LPPathTriplet;

vector<LPPathTriplet> *vecPathMap = NULL;


PVHD_HANDLE_STRUCTURE GetValidHandle ( VHD_HANDLE handle )
{
    PVHD_HANDLE_STRUCTURE pHandle = (PVHD_HANDLE_STRUCTURE) handle;

    if ( pHandle == VHD_HANDLE_INVALID )
    {
        wcerr << "Error - invalid handle" << endl;
        return (PVHD_HANDLE_STRUCTURE)VHD_HANDLE_INVALID;
    }

    if ( pHandle->signature != VHD_HANDLE_SIGNATURE )
    {
        wcerr << "Error - invalid handle. Signature mismatch" << endl;
        return (PVHD_HANDLE_STRUCTURE)VHD_HANDLE_INVALID;
    } // if ( pHandle->signature != VHD_HANDLE_SIGNATURE )

    if ( pHandle->pDisk == NULL )
    {
        wcerr << "pDisk is null" << endl;
        return (PVHD_HANDLE_STRUCTURE)VHD_HANDLE_INVALID;
    } // if ( pHandle->pDisk == NULL )

    return pHandle;
}

VHD_HANDLE OpenVHDFileA ( const char *vhdFileName )
{
    if ( vhdFileName == NULL )
    {
        wcerr << "NULL File Name" << endl;
        return VHD_HANDLE_INVALID;
    }
 
    wchar_t sWideFileName [ MAX_PATH*2 + 1 ];

    MultiByteToWideChar ( CP_ACP, MB_PRECOMPOSED, vhdFileName, -1, sWideFileName, _countof(sWideFileName ) );

    return OpenVHDFileW ( sWideFileName );
};

VHD_HANDLE OpenVHDFileW ( const wchar_t *vhdFileName )
{
    if ( vhdFileName == NULL )
    {
        wcerr << "NULL File Name" << endl;
        return VHD_HANDLE_INVALID;
    }

    PVHD_HANDLE_STRUCTURE pHandle = new VHD_HANDLE_STRUCTURE;

    if (pHandle == NULL )
    {
        wcerr << "Failed to allocate handle" << endl;
        return VHD_HANDLE_INVALID;
    }

    pHandle->pDisk = new VHDDisk ( vhdFileName );

    if ( pHandle->pDisk == NULL )
    {
        wcerr << "Failed to create VHD Object" << endl;
        delete pHandle;
        return VHD_HANDLE_INVALID;
    }

    if ( !pHandle->pDisk->OpenFile() )
    {
        wcerr << "Failed to open VHD File " << vhdFileName << endl;
        delete pHandle->pDisk;
        delete pHandle;
        return VHD_HANDLE_INVALID;
    }

    if ( !pHandle->pDisk->ReadHeaders() )
    {
        wcerr << "Failed to parse headers from VHD File " << vhdFileName << endl;
        delete pHandle->pDisk;
        delete pHandle;
        return VHD_HANDLE_INVALID;
    }

    // File is successfully opened - assign the handle a signature 
    pHandle->signature = VHD_HANDLE_SIGNATURE;
    return pHandle;
};


VHDREAD_FUNCTION  void CloseVHDFile ( VHD_HANDLE handle)
{
    PVHD_HANDLE_STRUCTURE pHandle = GetValidHandle ( handle );

    if ( pHandle == VHD_HANDLE_INVALID )
    {
        wcerr << "Error - closing invalid handle" << endl;
        return;
    } // if ( pHandle == VHD_HANDLE_INVALID )

    delete pHandle->pDisk;
    pHandle->pDisk = NULL;

    delete pHandle;
    pHandle = (PVHD_HANDLE_STRUCTURE)VHD_HANDLE_INVALID;
}

VHDREAD_FUNCTION long long   GetVHDTotalSectors ( VHD_HANDLE handle)
{
    PVHD_HANDLE_STRUCTURE pHandle = GetValidHandle (handle);

    if ( pHandle == VHD_HANDLE_INVALID )
    {
        wcerr << "Error - invalid handle" << endl;
        return 0;
    }

    return pHandle->pDisk->GetMaxSectors();
}

VHDREAD_FUNCTION unsigned int GetVHDSectorSize  ( VHD_HANDLE handle)
{
    PVHD_HANDLE_STRUCTURE pHandle = GetValidHandle (handle);

    if ( pHandle == VHD_HANDLE_INVALID )
    {
        wcerr << "Error - invalid handle" << endl;
        return 0;
    }

    return VHD_SECTOR_SIZE;
}

VHDREAD_FUNCTION bool GetVHDSector ( VHD_HANDLE handle, long long sectorNumber, unsigned char *buffer, unsigned int size )
{
    PVHD_HANDLE_STRUCTURE pHandle = GetValidHandle ( handle );
    
    if ( pHandle == VHD_HANDLE_INVALID )
    {
        wcerr << "Error - invalid handle" << endl;
        return false;
    } // if ( pHandle == VHD_HANDLE_INVALID )

    return pHandle->pDisk->GetSector ( sectorNumber, buffer, size );
}

VHDREAD_FUNCTION unsigned int GetVHDFixedDiskFooter ( VHD_HANDLE handle, PVOID pFooter, unsigned int *pSize)
{
    PVHD_HANDLE_STRUCTURE pHandle = GetValidHandle ( handle );
    
    if ( pHandle == VHD_HANDLE_INVALID )
    {
        wcerr << "Error - invalid handle" << endl;
        return -2;
    } // if ( pHandle == VHD_HANDLE_INVALID )

	if( (VHDFooter::PVHDHardDiskFooter)pFooter )
	{
		if(sizeof(VHDFooter::VHDHardDiskFooter) > *pSize)
		{
			*pSize = sizeof(VHDFooter::VHDHardDiskFooter);
			wcerr << "Error - insufficient data" << endl;
			return -1;
		}

		VHDFooter oNewFooter = pHandle->pDisk->GetFooter();
		oNewFooter.SetDiskType   ( VHDFooter::DiskTypeFixed );
		oNewFooter.SetDataOffset ( -1 );
		oNewFooter.SetSavedState (  0 );
		oNewFooter.GetVHDHardDiskFooter()->checksum = oNewFooter.ComputeCheckSum(oNewFooter.GetVHDHardDiskFooter());
		oNewFooter.HostToNetwork();
		oNewFooter.GenerateFixedDiskFooter((VHDFooter::PVHDHardDiskFooter)pFooter);

		*pSize = sizeof(VHDFooter::VHDHardDiskFooter);
	}
	else
		if( pSize ) 
			*pSize = sizeof(VHDFooter::VHDHardDiskFooter);

	return 0;
}

VHDREAD_FUNCTION  bool GetVHDFile ( wchar_t* pszFileNameIn, wchar_t* pszVHDFileNameOut, int size )
{
	bool bRet = false;
	wstring sDiskHierarchy;
	VHD_HANDLE handle = OpenVHDFileW ( pszFileNameIn );

    PVHD_HANDLE_STRUCTURE pHandle = GetValidHandle ( handle );
    
    if ( pHandle == VHD_HANDLE_INVALID )
    {
        wcerr << "Error - invalid handle" << endl;
        goto _return;
    } // if ( pHandle == VHD_HANDLE_INVALID )
	
	sDiskHierarchy = pHandle->pDisk->GetDiskHierarchy();
	
	if(sDiskHierarchy.empty())
    {
        wcerr << "Error - Empty VHD file" << endl;
        goto _return;
    } 

	int pos = 0;
	pos = sDiskHierarchy.find(L"*");

	if(pos != -1)
		sDiskHierarchy = sDiskHierarchy.substr(0, pos);

	wcscpy_s(pszVHDFileNameOut, size, (wchar_t*)sDiskHierarchy.c_str());	
	bRet = TRUE;

_return:
	CloseVHDFile ( handle );
	return bRet;
}

void GetVSSPath( wchar_t* szDiskName, wchar_t* sVssPath, int sVSSPathBufSize)
{
	//return original string
	if(vecPathMap == NULL)
	{
		wcscpy_s(sVssPath, sVSSPathBufSize, szDiskName);
		return;
	}

	for(int index=0; index<(*vecPathMap).size(); index++)
	{
		if(_wcsnicmp(((*vecPathMap)[index]).PhysicalPath, szDiskName, 1)==0) //first char is driver letter
		{
			swprintf_s(sVssPath, sVSSPathBufSize, L"%s%s", ((*vecPathMap)[index]).MountedPath, szDiskName+2);
			return;
		} 
	}

	wcscpy_s(sVssPath, sVSSPathBufSize, szDiskName);
	return;
}

VHDREAD_FUNCTION void SetPathMap( void* vecPathMapIn )
{		
	vecPathMap = (vector<LPPathTriplet>*)vecPathMapIn;
	return;
}