#include "stdafx.h"
#include "LIB_VHD_VHDHelper.h"

#pragma comment(lib, "Ws2_32.lib")

UINT32 VHDHelper::ConvTimeStamp(time_t tTime)
{
   tm t;
   ZeroMemory(&t, sizeof(t));
   t.tm_year = 2000-1900;
   t.tm_mday = 1;
   time_t t2000 = mktime(&t);
   return (UINT32)(tTime-t2000);
}

UINT32 VHDHelper::CalcCHS(ULONGLONG ullTotalSectors)
{
   UINT32 unCHS;
   UINT iCylinders, iHeads, iSectorsPerTrack, iCylinderTimesHead;

   if(ullTotalSectors > 65535 * 16 * 255)
   {
      ullTotalSectors = 65535 * 16 * 255;
   }

   if(ullTotalSectors >= 65535 * 16 * 63)
   {
      iSectorsPerTrack = 255;
      iHeads = 16;
      iCylinderTimesHead = (INT)(ullTotalSectors / iSectorsPerTrack);
   }
   else
   {
      iSectorsPerTrack = 17;
      iCylinderTimesHead = (INT)(ullTotalSectors / iSectorsPerTrack);

      iHeads = (iCylinderTimesHead + 1023) / 1024;

      if(iHeads < 4)
      {
         iHeads = 4;
      }
      if(iCylinderTimesHead >= (iHeads * 1024) || iHeads > 16)
      {
         iSectorsPerTrack = 31;
         iHeads = 16;
         iCylinderTimesHead = (INT)(ullTotalSectors / iSectorsPerTrack);
      }
      if(iCylinderTimesHead >= (iHeads * 1024))
      {
         iSectorsPerTrack = 63;
         iHeads = 16;
         iCylinderTimesHead = (INT)(ullTotalSectors / iSectorsPerTrack);
      }
   }
   iCylinders = iCylinderTimesHead / iHeads;


   //Populate diskGeometry...
   ((CHAR *) &unCHS)[0] = ((CHAR *) &iSectorsPerTrack)[0];
   ((CHAR *) &unCHS)[1] = ((CHAR *) &iHeads)[0];
   ((CHAR *) &unCHS)[2] = ((CHAR *) &iCylinders)[0];
   ((CHAR *) &unCHS)[3] = ((CHAR *) &iCylinders)[1];

   return unCHS;
}

UINT32 VHDHelper::CalcChecksum(VOID* pBuf, ULONG ulSize, UINT32 uiPrevSum/* =0xFFFFFFFF */)
{
   UINT32 unSum = ~uiPrevSum;

   for ( ULONG i=0; i < ulSize; i++ )
      unSum += ((BYTE*)pBuf)[i];

   return ~unSum;
}

UINT64 VHDHelper::htonll(CONST UINT64 llIn)
{
   UINT64 llOut = 0;

   ((CHAR *) &llOut)[0] = ((CHAR *) &llIn)[7];
   ((CHAR *) &llOut)[1] = ((CHAR *) &llIn)[6];
   ((CHAR *) &llOut)[2] = ((CHAR *) &llIn)[5];
   ((CHAR *) &llOut)[3] = ((CHAR *) &llIn)[4];
   ((CHAR *) &llOut)[4] = ((CHAR *) &llIn)[3];
   ((CHAR *) &llOut)[5] = ((CHAR *) &llIn)[2];
   ((CHAR *) &llOut)[6] = ((CHAR *) &llIn)[1];
   ((CHAR *) &llOut)[7] = ((CHAR *) &llIn)[0];        

   return llOut;
}

VOID VHDHelper::ConvHeader(PVHDDYNHEADER pHeader)
{
   HTONLL(pHeader->DataOffset);
   HTONLL(pHeader->TableOffset);
   HTONL(pHeader->HeaderVersion);
   HTONL(pHeader->MaxTableEntries);
   HTONL(pHeader->BlockSize);
   HTONL(pHeader->Checksum);
   HTONL(pHeader->ParentUniqueID.Data1);
   HTONS(pHeader->ParentUniqueID.Data2);
   HTONS(pHeader->ParentUniqueID.Data3);
   HTONL(pHeader->ParentTimeStamp);
   for( INT i=0; i<8; i++)
   {
      HTONL(pHeader->ParentLocators[i].PlatformDataSpace);
      HTONL(pHeader->ParentLocators[i].PlatformDataLength);
      HTONLL(pHeader->ParentLocators[i].PlatformDataOffset);
   }
}

VOID VHDHelper::ConvFooter(PVHDFOOTER pFooter)
{
   HTONL(pFooter->Features);
   HTONL(pFooter->FileFormatVersion);
   HTONLL(pFooter->DataOffset);
   HTONL(pFooter->TimeStamp);
   HTONL(pFooter->CreatorVersion);
   HTONLL(pFooter->OriginalSize);
   HTONLL(pFooter->CurrentSize);
   HTONL(pFooter->DiskGeometry.Value);
   HTONL(pFooter->DiskType);
   HTONL(pFooter->Checksum);
   HTONL(pFooter->UniqueId.Data1);
   HTONS(pFooter->UniqueId.Data2);
   HTONS(pFooter->UniqueId.Data3);
};

BOOL VHDHelper::ReadFooter(HANDLE hFile, PVHDFOOTER pFooter, BOOL bChecksum)
{
   LARGE_INTEGER liPos;
   liPos.QuadPart = -512;
   if( !SetFilePointerEx(hFile, liPos, NULL, FILE_END) )
   {
      return FALSE;
   }

   DWORD dwRead = 0;
   ReadFile(hFile, pFooter, sizeof(VHDFOOTER), &dwRead, NULL);
   if( dwRead != sizeof(VHDFOOTER) )
   {
      return FALSE;
   }

   ConvFooter(pFooter);

   if( !bChecksum )
   {
      return TRUE;
   }

   ULONG ulParentChecksum = pFooter->Checksum;
   pFooter->Checksum = 0;
   if( ulParentChecksum != CalcChecksum(pFooter, sizeof(VHDFOOTER)) )
   {
      ZeroMemory(pFooter, sizeof(VHDFOOTER));
      liPos.QuadPart = -511;
      if( !SetFilePointerEx(hFile, liPos, NULL, FILE_END) )
      {
         return FALSE;
      }

      DWORD dwRead = 0;
      ReadFile(hFile, pFooter, sizeof(VHDFOOTER), &dwRead, NULL);
      if( dwRead != 511 )
      {
         return FALSE;
      }

      ConvFooter(pFooter);
      ulParentChecksum = pFooter->Checksum;
      pFooter->Checksum = 0;
      if( ulParentChecksum != CalcChecksum(pFooter, sizeof(VHDFOOTER) - 1) )
      {
         return FALSE;
      }
   }

   pFooter->Checksum = CalcChecksum(pFooter, sizeof(VHDFOOTER));
   return TRUE;
}

BOOL VHDHelper::GetBit(BYTE* pBuf, INT nBit)
{
   BYTE cByte = pBuf[nBit/8];
   INT nBitLeft = nBit%8;
   BYTE cMask = 0;
   cMask = 1<<(7-nBitLeft);

   return cByte & cMask;
}

VOID VHDHelper::SetBit(BYTE* pBuf, INT nBit, BOOL bVal)
{
   INT nBytes = nBit/8;
   INT nBitLeft = nBit%8;

   BYTE cMask = 0;
   if( bVal )
   {
      cMask = 1<<(7-nBitLeft);
      pBuf[nBytes] |= cMask;
   }
   else
   {
      cMask = ~(1<<(7-nBitLeft));
      pBuf[nBytes] &= cMask;
   }
}

UINT32 VHDHelper::GetMAXBATEntries(LONGLONG llDiskSize)
{
   UINT32 unRet = 0;
   unRet = (UINT32)(llDiskSize/VHD_BLOCK_SIZE);
   unRet = ((unRet*sizeof(UINT32)+VHD_SECTOR_SIZE-1)&~(VHD_SECTOR_SIZE-1))/sizeof(UINT32);
   return unRet;
}

VOID VHDHelper::BAT_GtoL(PBATELEMENT pGlobal, UINT32* pLocal, CVirtualHardDisk* pLocalDisk, ULONG ulCount)
{
   for(ULONG i=0; i<ulCount; i++)
   {
      if( pGlobal[i].pDisk == pLocalDisk )
      {
         pLocal[i] = pGlobal[i].nOffsetInSector;
         HTONL(pLocal[i]);
      }
      else
      {
         pLocal[i] = 0xffffffff;
      }
   }
}
