#pragma once

#include "..\..\PublicHeader\PublicFileDeviceInterface.h"
#include "LdmObjects.h"

struct ExtendedVblk;

typedef std::map<uint64_t, Volume> VolumeContainer;
typedef std::map<uint64_t, Disk> DiskContainer;

class VblkParser
{
public:
    void Parse(const ExtendedVblk* vblk);
    void SetDiskParameters(
        const std::string& guid, /*const std::string&*/ IFileDevice * path, uint32_t sectorSize, uint64_t logicalStartLba);

    void BuildRelations();

    const VolumeContainer& GetVolumes() const;
    const DiskContainer& GetDisks() const;

private:
    void ParseComponent(const ExtendedVblk* vblk);
    void ParsePartition(const ExtendedVblk* vblk);
    void ParseVolume(const ExtendedVblk* vblk);
    void ParseDisk1(const ExtendedVblk* vblk);
    void ParseDisk2(const ExtendedVblk* vblk);
    void ParseDiskGroup1(const ExtendedVblk* vblk);
    void ParseDiskGroup2(const ExtendedVblk* vblk);
    void PrepareVolumes();
    void PrepareDisks();
    void PrepareVolume(Volume& volume);
    void PrepareComponent(Component& component);
    void PreparePartition(Partition& partition);
    void PrepareDisk(Disk& disk);

private:
    typedef std::set<uint64_t> ObjectIdContainer;
    typedef std::map<uint64_t, ObjectIdContainer> ObjectToObjectContainer;
    typedef std::map<uint64_t, Component> ComponentMap;
    typedef std::map<uint64_t, Partition> PartitionMap;

    VolumeContainer m_volumes;
    ComponentMap m_components;
    PartitionMap m_partitions;
    DiskContainer m_disks;
    ObjectToObjectContainer m_volumeToComponent;
    ObjectToObjectContainer m_componentToPartition;
    ObjectToObjectContainer m_diskToPartition;
};

