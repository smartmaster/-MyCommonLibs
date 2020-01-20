#ifndef _VHDHELPER_H_
#define _VHDHELPER_H_

#include "VHDDef.h"
#include <time.h>
#include "BLIDef.h"
#include <WinSock2.h> //baide02

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

   unsigned __int32 CalcCHS(ULONGLONG ullTotalSectors);

   unsigned __int32 CalcChecksum(void* pBuf, unsigned long ulSize);

   unsigned __int64 htonll(const unsigned __int64 llIn);

   void ConvHeader(PVHDDYNHEADER pHeader);

   void ConvFooter(PVHDFOOTER pFooter);

   BOOL ReadFooter(HANDLE hFile, PVHDFOOTER pFooter, BOOL bChecksum = TRUE);

   void SetBit(unsigned char* pBuf, int nBit, BOOL bVal);
   BOOL GetBit(unsigned char* pBuf, int nBit);


   //
   // << >>
   //

   // @nLen size of pBAT in int
   static void Hton_BAT(unsigned int* pBAT, int nLen)
   {
       for (int i = 0; i<nLen; i++)
           pBAT[i] = htonl(pBAT[i]);
   }
   static void Ntoh_BAT(unsigned int* pBAT, int nLen)
   {
       for (int i = 0; i<nLen; i++)
           pBAT[i] = ntohl(pBAT[i]);
   }

   static void Hton_Footer(VHDFOOTER* pstFooter)
   {
       ConvFooter(pstFooter);
   }

   static void Hton_Header(VHDDYNHEADER* pstHeader)
   {
       ConvHeader(pstHeader);
   }

   static inline UINT32
       SecsRoundUp(__int64 bytes)
   {
       return ((bytes + (VHD_SECTOR_SIZE - 1)) >> VHD_SECTOR_SHIFT);
   }

   static inline UINT32
       SecsRoundUp_NoZero(__int64 bytes)
   {
       return (SecsRoundUp(bytes) ? SecsRoundUp(bytes) : 1);
   }

   static inline __int64
       SectorsToBytes(__int64 sectors)
   {
       return sectors << VHD_SECTOR_SHIFT;
   }

   static inline __int64
       Bytes_PaddedInSectorSize(__int64 bytes)
   {
       return SectorsToBytes(SecsRoundUp_NoZero(bytes));
   }


}


#endif //_VHDHELPER_H_
