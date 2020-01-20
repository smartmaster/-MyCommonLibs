#pragma once

#include <stdint.h>

#pragma pack(push)
#pragma pack(1)

#ifdef _WIN32
typedef GUID Guid;
#else
struct Guid
{
	uint32_t Data1;
	uint16_t Data2;
	uint16_t Data3;
	uint8_t Data4[8];
};
#endif
    
struct PrivHead
{
    uint8_t magic[8];
    uint32_t checksum;
    uint16_t major;
    uint16_t minor;
    uint64_t timestamp;
    uint64_t sequenceNumber;
    uint64_t primaryPrivateHeaderLBA;
    uint64_t secondaryPrivateHeaderLBA;
    uint8_t diskId[64];
    uint8_t hostId[64];
    uint8_t diskGroupId[64];
    uint8_t diskGroupName[31];
    uint32_t bytesPerBlock;
    uint32_t privateHeaderFlags;
    uint16_t publicRegionSliceNumber;
    uint16_t privateRegionSliceNumber;
    uint64_t publicRegionStart;
    uint64_t publicRegionSize;
    uint64_t privateRegionStart;
    uint64_t privateRegionSize;
    uint64_t primaryTocLba;
    uint64_t secondaryTocLba;
    uint32_t numberOfConfigs;
    uint32_t numberOfLogs;
    uint64_t configSize;
    uint64_t logSize;
    uint8_t diskSignature[4];
    Guid diskSetGuid;
    Guid diskSetGuidDupicate;
};

struct TocRegion
{
    uint8_t name[8];
    uint16_t flags;
    uint64_t start;
    uint64_t size;
    uint16_t unk1;
    uint16_t copyNumber;
    uint8_t zeroes[4];
};

struct TocBlock
{
    uint8_t magic[8];
    uint32_t checksum;
    uint64_t updateSequenceNumber;
    uint8_t zeroes[16];
    TocRegion config;
    TocRegion log;
};

struct Vmdb
{
    uint8_t magic[4];
    uint32_t sequenceNumberOfLastVblk;
    uint32_t sizeOfVblk;
    uint32_t firstVblkOffset;
    uint16_t updateStatus;
    uint16_t major;
    uint16_t minor;
    uint8_t diskGroupName[31];
    uint8_t diskGroupGuid[64];
    uint64_t committedSequence;
    uint64_t pendingSequence;
    uint32_t numberofCommittedVolumeBlocks;
    uint32_t numberOfCommittedComponentBlocks;
    uint32_t numberOfCommittedPartitionBlocks;
    uint32_t numberOfCommittedDiskBlocks;
    uint32_t numberOfCommittedDiskAccessRecords;
    uint32_t unused1;
    uint32_t unused2;
    uint32_t numberOfPendingVolumeBlocks;
    uint32_t numberOfPendingComponentBlocks;
    uint32_t numberOfPendingPartitionBlocks;
    uint32_t numberOfPendingDiskBlocks;
    uint32_t numberOfPendingDiskAccessRecords;
    uint32_t unused3;
    uint32_t unused4;
    //uint8_t zeroes[16]; VSFW 4.0 adds this field upon installation, and removes it during unsinstall, it is unclear how to detect if this field will appear or not, other than checking 0xCD to see if it's in use.
    uint64_t lastAccessTime;
};

struct VblkHeader
{
    uint8_t magic[4];
    uint32_t sequenceNumber;
    uint32_t groupNumber;
    uint16_t recordNumber;
    uint16_t numberOfRecords;
};

struct ExtendedVblk
{
    VblkHeader header;
    uint16_t updateStatus;
    uint8_t flags;
    uint8_t type;
    uint32_t length;
    uint8_t otherData;
};

enum VblkType
{
    VblkComponent = 0x32,
    VblkPartition = 0x33,
    VblkVolume = 0x51,
    VblkDisk1 = 0x34,
    VblkDisk2 = 0x44,
    VblkDiskGroup1 = 0x35,
    VblkDiskGroup2 = 0x45
};

static const uint8_t g_privHeadMagic[] = {'P', 'R', 'I', 'V', 'H', 'E', 'A', 'D'};
static const uint8_t g_tocblockMagic[] = {'T', 'O', 'C', 'B', 'L', 'O', 'C', 'K'};
static const uint8_t g_vmdbMagic[]     = {'V', 'M', 'D', 'B'};
static const uint8_t g_vblkMagic[]     = {'V', 'B', 'L', 'K'};

//static const uint64_t g_privHeadOffset = 0xC00;
static const uint64_t g_privHeadOffsetSector = 0x6;
static const uint64_t g_privHeadOffsetGPTSector = 0x740;

static const uint32_t g_vblkStartSequenceNumber = 4;

#pragma pack(pop)
