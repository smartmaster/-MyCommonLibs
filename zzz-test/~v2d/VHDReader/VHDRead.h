#pragma once 
#include <Windows.h>
#include <vector>

#if defined (__cplusplus)
extern "C"
{
#endif // __cplusplus

#if defined (VHDREADER_EXPORTS)
    // Export Definations 
#define VHDREAD_FUNCTION    __declspec(dllexport)
#else // VHDREAD_EXPORTS
    // Import Definations 
#define VHDREAD_FUNCTION    __declspec(dllimport)
#endif // VHDREAD_EXPORTS

#define VHD_HANDLE          void *
#define VHD_HANDLE_INVALID (void *) -1

/// Open a VHD File. On success returns a VHD_HANDLE. On failure returns VHD_HANDLE_INVALID
VHDREAD_FUNCTION VHD_HANDLE     OpenVHDFileA        ( const char *vhdFileName );

/// Open a VHD File. On success returns a VHD_HANDLE. On failure returns VHD_HANDLE_INVALID
VHDREAD_FUNCTION VHD_HANDLE     OpenVHDFileW        ( const wchar_t *vhdFileName );

/// Determine the total number of sectors in this VHD File 
VHDREAD_FUNCTION long long      GetVHDTotalSectors  ( VHD_HANDLE );

/// Determine the size of individual sector in this VHD File 
VHDREAD_FUNCTION unsigned int   GetVHDSectorSize    ( VHD_HANDLE );

/// Obtain the contain of 'sectorNumber' sector from an open VHD file. 
VHDREAD_FUNCTION bool           GetVHDSector        ( VHD_HANDLE, long long sectorNumber,unsigned char *buffer, unsigned int size );

/// Close an open VHD File 
VHDREAD_FUNCTION void           CloseVHDFile        ( VHD_HANDLE );

/// Generate Fixed disk Footer in 'pFooter' for the VHD referenced by handle, *pSize will be set to size of data returned or size of buffer required if insufficient buffer supplied.
///	Returns 0 on SUCCESS, -1 if insufficient buffer and -2 on Invalid VHD handle.
VHDREAD_FUNCTION unsigned int GetVHDFixedDiskFooter ( VHD_HANDLE handle, PVOID pFooter, unsigned int *pSize);

//return top level path(vhd file path) of AVHD/VHD hierarchy
VHDREAD_FUNCTION  bool GetVHDFile ( wchar_t* pszFileNameIn, wchar_t* pszVHDFileNameOut, int size );
VHDREAD_FUNCTION void SetPathMap( void* vecPathMap );
//get VSS path
void GetVSSPath( wchar_t* sParent, wchar_t* sVssPath, int size );
#if defined (__cplusplus)
}
#endif // __cplusplus
