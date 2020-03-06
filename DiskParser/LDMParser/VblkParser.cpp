#include "stdafx.h"
#include "stdafx.include.h"
#include "VblkParser.h"
#include "Ldm.h"
#include "Endian.h"

static uint64_t ReadNumber(const uint8_t* ptr)
{
    uint64_t res = 0;

    for (uint8_t len = *ptr++; len > 0; --len, ++ptr)
    {
        res <<= 8;
        res |= *ptr;
    }

    return res;
}

static std::string ReadString(const uint8_t* ptr)
{
    uint8_t len = *ptr++;
    return std::string(ptr, ptr + len);
}

static const uint8_t* Skip(const uint8_t* ptr)
{
    uint8_t len = *ptr++;
    return ptr + len;
}

void VblkParser::Parse(const ExtendedVblk* vblk)
{
    if (vblk->header.groupNumber == 0)
    {
        return;
    }

    switch (vblk->type)
    {
    case VblkComponent:
        ParseComponent(vblk);
        break;
    case VblkPartition:
        ParsePartition(vblk);
        break;
    case VblkVolume:
        ParseVolume(vblk);
        break;
    case VblkDisk1:
        ParseDisk1(vblk);
        break;
    case VblkDisk2:
        ParseDisk2(vblk);
        break;
    case VblkDiskGroup1:
        ParseDiskGroup1(vblk);
        break;
    case VblkDiskGroup2:
        ParseDiskGroup2(vblk);
        break;

    default:
        std::cout << "Unknown Vblk type: " << vblk->type << std::endl;
    }
}

void VblkParser::SetDiskParameters(
    const std::string& guid, IFileDevice * path/*const std::string& path*/, uint32_t sectorSize, uint64_t logicalStartLba)
{
    DiskContainer::iterator it = m_disks.begin();
    DiskContainer::iterator end = m_disks.end();

    for (; it != end; ++it)
    {
        if (it->second.guid == guid)
        {
            it->second.path = path;
            it->second.sectorSize = sectorSize;
            it->second.logicalStartLba = logicalStartLba;

            break;
        }
    }
}

void VblkParser::BuildRelations()
{
    PrepareVolumes();
    PrepareDisks();
}

const VolumeContainer& VblkParser::GetVolumes() const
{
    return m_volumes;
}

const DiskContainer& VblkParser::GetDisks() const
{
    return m_disks;
}

void VblkParser::ParseComponent(const ExtendedVblk* vblk)
{
    const uint8_t* data = &vblk->otherData;
    uint64_t objectId = ReadNumber(data);
    data = Skip(data);
    /* --@@##$$-- */ std::string name = ReadString(data);
    data = Skip(data);
    /* --@@##$$-- */ std::string state = ReadString(data);
    data = Skip(data);
    uint8_t layout = *data++;
    /* --@@##$$-- */ uint32_t flags = ByteReverse(*reinterpret_cast<const uint32_t*>(data));
    data += sizeof(uint32_t);
    /* --@@##$$-- */ uint64_t numPartitions = ReadNumber(data);
    data = Skip(data);
    /* --@@##$$-- */ uint64_t commitTransactionId = ByteReverse(*reinterpret_cast<const uint64_t*>(data));
    data += sizeof(uint64_t);
    data += sizeof(uint64_t); //zeroes
    uint64_t volumeId = ReadNumber(data);
    data = Skip(data);

    Component component = {objectId, static_cast<ComponentLayout>(layout), name, state, flags, numPartitions, commitTransactionId, volumeId};
    m_components[objectId] = component;
    m_volumeToComponent[volumeId].insert(objectId);
}

void VblkParser::ParsePartition(const ExtendedVblk* vblk)
{
    const uint8_t* data = &vblk->otherData;
    uint64_t objectId = ReadNumber(data);
    data = Skip(data);
    /* --@@##$$-- */ std::string name = ReadString(data);
    data = Skip(data);
    /* --@@##$$-- */ uint32_t flags = ByteReverse(*reinterpret_cast<const uint32_t*>(data));
    data += sizeof(uint32_t);
    /* --@@##$$-- */ uint64_t commitTransactionId = ByteReverse(*reinterpret_cast<const uint64_t*>(data));
    data += sizeof(uint64_t);
    uint64_t diskOffsetLba = ByteReverse(*reinterpret_cast<const uint64_t*>(data));
    data += sizeof(uint64_t);
    uint64_t partitionOffsetInColumnLba = ByteReverse(*reinterpret_cast<const uint64_t*>(data));
    data += sizeof(uint64_t);
    uint64_t sizeLba = ReadNumber(data);
    data = Skip(data);
    uint64_t componentId = ReadNumber(data);
    data = Skip(data);
    uint64_t diskId = ReadNumber(data);
    data = Skip(data);

    Partition partition = {objectId, diskOffsetLba, sizeLba, partitionOffsetInColumnLba, diskId, name, flags, commitTransactionId, componentId};
    m_partitions[objectId] = partition;
    m_diskToPartition[diskId].insert(objectId);
    m_componentToPartition[componentId].insert(objectId);
}

void VblkParser::ParseVolume(const ExtendedVblk* vblk)
{
    const uint8_t* data = &vblk->otherData;
    uint64_t objectId = ReadNumber(data);
    data = Skip(data);
    /* --@@##$$-- */ std::string name = ReadString(data);
    data = Skip(data);
    /* --@@##$$-- */ std::string type = ReadString(data);
    data = Skip(data);
    /* --@@##$$-- */ std::string disableDriveLetterAssignment = ReadString(data);
    data = Skip(data);
    /* --@@##$$-- */ std::string state(data, data + 14);
    data += 14;
    /* --@@##$$-- */ uint8_t readPolicy = *data;
    data += sizeof(uint8_t);
    /* --@@##$$-- */ uint64_t volumeNumber = ReadNumber(data);
    data = Skip(data);
    /* --@@##$$-- */ uint32_t flags = ByteReverse(*reinterpret_cast<const uint32_t*>(data));
    data += sizeof(uint32_t);
    /* --@@##$$-- */ uint64_t numberOfComponents = ReadNumber(data);
    data = Skip(data);
    /* --@@##$$-- */ uint64_t commitTransactionId = ByteReverse(*reinterpret_cast<const uint64_t*>(data));
    data += sizeof(uint64_t);
    /* --@@##$$-- */ uint64_t unknownTransactionId = ByteReverse(*reinterpret_cast<const uint64_t*>(data));
    data += sizeof(uint64_t);
    uint64_t sizeLba = ReadNumber(data);
    data = Skip(data);
    data += 4; //zeroes
    uint8_t partitionType = *data;
    data += sizeof(uint8_t);
    Guid volumeGuid = {};
    memcpy(&volumeGuid, data, sizeof(Guid));
    volumeGuid.Data1 = ByteReverse((uint32_t)volumeGuid.Data1);
    volumeGuid.Data2 = ByteReverse(volumeGuid.Data2);
    volumeGuid.Data3 = ByteReverse(volumeGuid.Data3);
    data += sizeof(Guid);

    Volume volume = {objectId, VtUnknown, static_cast<FsType>(partitionType), sizeLba,
	name,
	type,
	disableDriveLetterAssignment,
	state,
	readPolicy,
	volumeNumber,
	flags,
	numberOfComponents,
	commitTransactionId,
	unknownTransactionId,
	volumeGuid
	};
    m_volumes[objectId] = volume;
}

void VblkParser::ParseDisk1(const ExtendedVblk* vblk)
{
    const uint8_t* data = &vblk->otherData;
    uint64_t objectId = ReadNumber(data);
    data = Skip(data);
    /* --@@##$$-- */ std::string name = ReadString(data);
    data = Skip(data);
    std::string guid = ReadString(data);
    data = Skip(data);
    /* --@@##$$-- */ std::string lastDevName = ReadString(data);
    data = Skip(data);
    /* --@@##$$-- */ uint32_t flags = ByteReverse(*reinterpret_cast<const uint32_t*>(data));
    data += sizeof(uint32_t);
    /* --@@##$$-- */ uint64_t commitTransactionId = ByteReverse(*reinterpret_cast<const uint64_t*>(data));

    Disk disk(objectId, guid,
			name,
			lastDevName,
			flags,
			commitTransactionId
		);
    m_disks[objectId] = disk;
}

void VblkParser::ParseDisk2(const ExtendedVblk* /*vblk*/)
{
}

void VblkParser::ParseDiskGroup1(const ExtendedVblk* vblk)
{
    const uint8_t* data = &vblk->otherData;
    /* --@@##$$-- */ uint64_t objectId = ReadNumber(data);
    data = Skip(data);
    /* --@@##$$-- */ std::string name = ReadString(data);
    data = Skip(data);
    /* --@@##$$-- */ std::string guid = ReadString(data);
    data = Skip(data);
    data += 4; //zeroes
    /* --@@##$$-- */ uint64_t commitTransactionId = ByteReverse(*reinterpret_cast<const uint64_t*>(data));
    data += sizeof(uint64_t);
}

void VblkParser::ParseDiskGroup2(const ExtendedVblk* /*vblk*/)
{
}

void VblkParser::PrepareVolumes()
{
    VolumeContainer::iterator it = m_volumes.begin();
    VolumeContainer::iterator end = m_volumes.end();

    for (; it != end; ++it)
    {
        PrepareVolume(it->second);
    }
}

void VblkParser::PrepareDisks()
{
    DiskContainer::iterator it = m_disks.begin();
    DiskContainer::iterator end = m_disks.end();

    for (; it != end; ++it)
    {
        PrepareDisk(it->second);
    }
}

void VblkParser::PrepareVolume(Volume& volume)
{
    ObjectIdContainer& components = m_volumeToComponent[volume.id];
    ObjectIdContainer::const_iterator it = components.begin();
    ObjectIdContainer::const_iterator end = components.end();

    for (; it != end; ++it)
    {
        Component& component = m_components[*it];
        PrepareComponent(component);
        volume.components.push_back(&component);
    }

    assert(volume.components.size() >= 1);
    if (volume.components.size() == 2)
    {
        volume.type = VtMirror;
    }
    else
    {
        Component& component = *volume.components.front();
        if (component.layout == ClRaid)
        {
            volume.type = VtRaid5;
        }
        else if (component.layout == ClStripe)
        {
            volume.type = VtStripe;
        }
        else if (component.layout == ClConcatenated)
        {
            volume.type = VtSimple;

            std::vector<Partition*>::const_iterator it = component.partitions.begin();
            std::vector<Partition*>::const_iterator end = component.partitions.end();
            uint64_t diskId = (*it)->diskId;
            for (++it; it != end; ++it)
            {
                if ((*it)->diskId != diskId)
                {
                    volume.type = VtSpan;
                    break;
                }
            }
        }
    }
}

void VblkParser::PrepareComponent(Component& component)
{
    ObjectIdContainer& partitions = m_componentToPartition[component.id];
    ObjectIdContainer::const_iterator it = partitions.begin();
    ObjectIdContainer::const_iterator end = partitions.end();

    for (; it != end; ++it)
    {
        Partition& partition = m_partitions[*it];
        PreparePartition(partition);
        component.partitions.push_back(&partition);
    }
}

void VblkParser::PreparePartition(Partition& partition)
{
    partition.diskOffsetLba += m_disks[partition.diskId].logicalStartLba;
}

void VblkParser::PrepareDisk(Disk& disk)
{
    ObjectIdContainer& partitions = m_diskToPartition[disk.id];
    ObjectIdContainer::const_iterator it = partitions.begin();
    ObjectIdContainer::const_iterator end = partitions.end();

    for (; it != end; ++it)
    {
        Partition& partition = m_partitions[*it];
        disk.partitions.push_back(&partition);
    }
}
