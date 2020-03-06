#include "stdafx.h"
#include "VHDHelper.h"

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

unsigned __int32 VHDHelper::CalcCHS(ULONGLONG ullTotalSectors)
{
   unsigned __int32 unCHS;
   unsigned int iCylinders, iHeads, iSectorsPerTrack, iCylinderTimesHead;

   if(ullTotalSectors > 65535 * 16 * 255)
   {
      ullTotalSectors = 65535 * 16 * 255;
   }

   if(ullTotalSectors >= 65535 * 16 * 63)
   {
      iSectorsPerTrack = 255;
      iHeads = 16;
      iCylinderTimesHead = (int)(ullTotalSectors / iSectorsPerTrack);
   }
   else
   {
      iSectorsPerTrack = 17;
      iCylinderTimesHead = (int)(ullTotalSectors / iSectorsPerTrack);

      iHeads = (iCylinderTimesHead + 1023) / 1024;

      if(iHeads < 4)
      {
         iHeads = 4;
      }
      if(iCylinderTimesHead >= (iHeads * 1024) || iHeads > 16)
      {
         iSectorsPerTrack = 31;
         iHeads = 16;
         iCylinderTimesHead = (int)(ullTotalSectors / iSectorsPerTrack);
      }
      if(iCylinderTimesHead >= (iHeads * 1024))
      {
         iSectorsPerTrack = 63;
         iHeads = 16;
         iCylinderTimesHead = (int)(ullTotalSectors / iSectorsPerTrack);
      }
   }
   iCylinders = iCylinderTimesHead / iHeads;


   //Populate diskGeometry...
   ((char *) &unCHS)[0] = ((char *) &iSectorsPerTrack)[0];
   ((char *) &unCHS)[1] = ((char *) &iHeads)[0];
   ((char *) &unCHS)[2] = ((char *) &iCylinders)[0];
   ((char *) &unCHS)[3] = ((char *) &iCylinders)[1];

   return unCHS;
}

unsigned __int32 VHDHelper::CalcChecksum(void* pBuf, unsigned long ulSize)
{
   unsigned __int32 unSum = 0;

   for ( unsigned long i=0; i < ulSize; i++ )
      unSum += ((unsigned char*)pBuf)[i];

   return ~unSum;
}

unsigned __int64 VHDHelper::htonll(const unsigned __int64 llIn)
{
   unsigned __int64 llOut = 0;

   ((char *) &llOut)[0] = ((char *) &llIn)[7];
   ((char *) &llOut)[1] = ((char *) &llIn)[6];
   ((char *) &llOut)[2] = ((char *) &llIn)[5];
   ((char *) &llOut)[3] = ((char *) &llIn)[4];
   ((char *) &llOut)[4] = ((char *) &llIn)[3];
   ((char *) &llOut)[5] = ((char *) &llIn)[2];
   ((char *) &llOut)[6] = ((char *) &llIn)[1];
   ((char *) &llOut)[7] = ((char *) &llIn)[0];        

   return llOut;
}

void VHDHelper::ConvHeader(PVHDDYNHEADER pHeader)
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
   for( int i=0; i<8; i++)
   {
      HTONL(pHeader->ParentLocators[i].PlatformDataSpace);
      HTONL(pHeader->ParentLocators[i].PlatformDataLength);
      HTONLL(pHeader->ParentLocators[i].PlatformDataOffset);
   }
}

void VHDHelper::ConvFooter(PVHDFOOTER pFooter)
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

BOOL VHDHelper::GetBit(unsigned char* pBuf, int nBit)
{
   unsigned char cByte = pBuf[nBit/8];
   int nBitLeft = nBit%8;
   unsigned char cMask = 0;
   cMask = 1<<(7-nBitLeft);

   return cByte & cMask;
}

void VHDHelper::SetBit(unsigned char* pBuf, int nBit, BOOL bVal)
{
   int nBytes = nBit/8;
   int nBitLeft = nBit%8;

   unsigned char cMask = 0;
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
