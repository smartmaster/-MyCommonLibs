#pragma once

#pragma pack(push)
#pragma pack(1)

enum PartitionType 
{
    PtDynamic = 0x42
};

struct MbrPartition
{
    uint8_t status;
    uint8_t firstSector[3];
    uint8_t partitionType;
    uint8_t lastSector[3];
    uint32_t lbaFirstSector;
    uint32_t lbaPartitionLength;
};

struct MasterBootRecord
{
    uint8_t bootstrap[446];
    MbrPartition partitions[4];
    uint16_t mbrSignature;
};

static const uint16_t MbrMagicSignature = 0xAA55;

#pragma pack(pop)
