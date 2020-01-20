#pragma once
#include <BaseTsd.h>
#include <Guiddef.h>

#define VHD_BLOCK_SIZE      2*1024*1024
#define VHD_BLOCK_SHIFT     21
#define VHD_SECTOR_SIZE     512
#define VHD_SECTOR_SHIFT    9
#define CA_BLK_UNUSED       0xFFFFFFFF
#define CA_VHD_COOKIE       "conectix"
#define CA_VHD_FF_VERSION   0x00010000
#define CA_VHD_RESERVED     0x00000002
#define CA_VHD_CreateVer    0x00010000

#define CA_VHD_HR_VER       0x00010000

class CPlatformCode
{
public:
    static const __int32 None = 0x00000000;
    static const __int32 W2ru = 0x75723257; // UNICODE relative   *(__int32*)"W2ru" //little-endian
    static const __int32 W2ku = 0x756B3257; // UNICODE absolute   *(__int32*)"W2ku"
    static const __int32 Wi2r = 0x72326957; // ANSI relative      *(__int32*)"Wi2r"
    static const __int32 Wi2k = 0x6B326957; // ANSI absolute      *(__int32*)"Wi2k"
};

#pragma pack(push, CVHDImageDef)
#pragma  pack(1)
typedef union _VHDFooter
{
   struct  
   {
      char                    Cookie[8];
      UINT32 Features;
      UINT32 FileFormatVersion;
      UINT64 DataOffset;
      UINT32 TimeStamp;
      char                     CreatorApplication[4];
      UINT32 CreatorVersion;
      char                    CreatorHostOS[4];
      UINT64 OriginalSize;
      UINT64 CurrentSize;
      union
      {
         struct  
         {
				UINT8   SectorsPerTrack_Cylinder;
            UINT8   Heads;
				UINT16 Cylinder;
         };
         UINT32 Value;
      }DiskGeometry;
      UINT32 DiskType;
      UINT32 Checksum;
      GUID                  UniqueId;
      UINT8   SavedState;
   };
   unsigned char   argBuffer[512];
}VHDFOOTER, *PVHDFOOTER;

typedef struct _VHDParentLocator
{
   UINT32  PlatformCode;
   UINT32  PlatformDataSpace;
   UINT32  PlatformDataLength;
   UINT32  Reserved;
   UINT64  PlatformDataOffset;
}VHDPARENTLOCATOR, *PVHDPARENTLOCATOR;

typedef union _VHDDYNHeader
{
   struct  
   {
      char Cookie[8];
      UINT64 DataOffset;
      UINT64  TableOffset;
      UINT32  HeaderVersion;
      UINT32  MaxTableEntries;
      UINT32  BlockSize;
      UINT32  Checksum;
      GUID ParentUniqueID;
      UINT32  ParentTimeStamp;
      UINT32  Reserved;
      wchar_t   ParentUnicodeName[256];
      VHDPARENTLOCATOR ParentLocators[8];
   };

   unsigned char argBuffer[1024];
}VHDDYNHEADER, *PVHDDYNHEADER;

#pragma  pack(pop, CVHDImageDef)

//VHD-Writer macro

#define VHD_RET_SUCCESSFULL 0
#define VHD_RET_INVALID_PARA 1
#define VHD_RET_FAILED 2
