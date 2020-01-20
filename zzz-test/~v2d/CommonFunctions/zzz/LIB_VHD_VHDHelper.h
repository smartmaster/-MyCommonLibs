#pragma once

#include "LIB_VHD_VHDDef.h"
#include "LIB_VHD_VHD.h"
#include <time.h>

#define VHD_DISK_NAME_EXT L".VHD"
#define VHD_DISK_NAME_EXT_WITHOUT_DOT L"VHD"

#define SAFECLOSEHANDLE(h) \
{  \
   if( h != INVALID_HANDLE_VALUE )  \
   {  \
   CloseHandle(h);   \
   h = INVALID_HANDLE_VALUE;   \
   }  \
}

#define HTONS(n)   \
{  \
   (n) = htons((n)); \
}

#define HTONL(n) \
{  \
   (n) = htonl((n));   \
}

#define HTONLL(n)  \
{  \
   (n) = htonll((n));   \
}

namespace VHDHelper
{
   UINT32 ConvTimeStamp(time_t tTime);

   UINT32 CalcCHS(ULONGLONG ullTotalSectors);

   UINT32 CalcChecksum(VOID* pBuf, ULONG ulSize, UINT32 uiPrevSum=0xFFFFFFFF );

   UINT64 htonll(CONST UINT64 llIn);

   VOID ConvHeader(PVHDDYNHEADER pHeader);

   VOID ConvFooter(PVHDFOOTER pFooter);

   BOOL ReadFooter(HANDLE hFile, PVHDFOOTER pFooter, BOOL bChecksum = TRUE);

   VOID SetBit(BYTE* pBuf, INT nBit, BOOL bVal);
   BOOL GetBit(BYTE* pBuf, INT nBit);

   UINT32 GetMAXBATEntries(LONGLONG llDiskSize);
   VOID BAT_GtoL(PBATELEMENT pGlobal, UINT32* pLocal, CVirtualHardDisk* pLocalDisk, ULONG ulCount);
}
