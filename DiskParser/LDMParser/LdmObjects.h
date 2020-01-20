#pragma once

#include "Ldm.h"

struct Partition
{
    uint64_t id;
    uint64_t diskOffsetLba;
    uint64_t sizeLba;
    uint64_t volumeOffsetLba;
    uint64_t diskId;

	std::string name;
	uint32_t flags;
	uint64_t commitTransactionId;
	uint64_t componentId;
};

typedef std::vector<Partition*> PartitionContainer;

enum ComponentLayout
{
    ClStripe = 1,
    ClConcatenated = 2,
    ClRaid = 3
};

struct Component
{
    uint64_t id;
    ComponentLayout layout;
	std::string name;
	std::string state;
	uint32_t flags;
	uint64_t numPartitions;
	uint64_t commitTransactionId;
	uint64_t volumeId;
    PartitionContainer partitions;
};

typedef std::vector<Component*> ComponentContainer;

enum VolumeType
{
    VtUnknown,
    VtSimple,
    VtSpan,
    VtStripe,
    VtMirror,
    VtRaid5
};

enum FsType
{
    FsRawOrFat16 = 0x6,
    FsNtfs = 0x7,
    FsFat32 = 0xB
};

struct Volume
{
    uint64_t id;
    VolumeType type;
    FsType fileSystem;
    uint64_t sizeLba;

	std::string name;
	std::string typeString;
	std::string disableDriveLetterAssignment;
	std::string state;
	uint8_t readPolicy;
	uint64_t volumeNumber;
	uint32_t flags;
	uint64_t numberOfComponents;
	uint64_t commitTransactionId;
	uint64_t unknownTransactionId;
	Guid volumeGuid;

    ComponentContainer components;
};

struct Disk
{
    Disk()
        : id(0)
        , path(nullptr/*"Missing"*/)
        , sectorSize(512)
        , logicalStartLba(0)
    {
    }

    Disk(uint64_t objectId, const std::string& guid,
		std::string name,
		std::string lastDevName,
		uint32_t flags,
		uint64_t commitTransactionId
		)
        : id(objectId)
        , guid(guid)
        , path(nullptr/*"Missing"*/)
        , sectorSize(512)
        , logicalStartLba(0),
		name(name),
		lastDevName(lastDevName),
		flags(flags),
		commitTransactionId(commitTransactionId)
    {
    }

    uint64_t id;
    std::string guid;
	CComPtr<IFileDevice> /*std::string */ path;
    uint32_t sectorSize;
    uint64_t logicalStartLba;

	std::string name;
	std::string lastDevName;
	uint32_t flags;
	uint64_t commitTransactionId;

    PartitionContainer partitions;
};
