#include "stdafx.h"
#include "stdafx.include.h"
#include "Parser.h"
#include "Units.h"
#include "ErrnoException.h"

//<sonmi01>2015-11-13 ###???
#include "LdmParser.h"


void ShowInfo(int count, char* diskNames[]);

int main_parser(int argc, char* argv[])
{
    ShowInfo(argc - 1, argv + 1);
    return 0;
}

const char* FsTypeToString(FsType type)
{
    switch (type)
    {
    case FsRawOrFat16:
        return "RAW or FAT16";
    case FsFat32:
        return "FAT32";
    case FsNtfs:
        return "NTFS";
    default:
        return "Unknown";
    }
}

const char* VolumeTypeToString(VolumeType type)
{
    switch (type)
    {
    case VtSimple:
        return "Simple";
    case VtSpan:
        return "Spanned";
    case VtStripe:
        return "Striped";
    case VtMirror:
        return "Mirrored";
    case VtRaid5:
        return "Raid-5";
    default:
        return "Unknown";
    }
}

uint32_t CalculatePartitionsCount(const Volume& volume)
{
    uint32_t result = 0;

    ComponentContainer::const_iterator it = volume.components.begin();
    ComponentContainer::const_iterator end = volume.components.end();

    for (; it != end; ++it)
    {
        result += (*it)->partitions.size();
    }

    return result;
}

void ShowPartitions(const Volume& volume, const DiskContainer& disks, std::vector<SmartLib::PartitionInfo> & partInfos)
{
    uint32_t number = 1;
    ComponentContainer::const_iterator compIt = volume.components.begin();
    ComponentContainer::const_iterator compEnd = volume.components.end();

    for (; compIt != compEnd; ++compIt)
    {
        Component& component = **compIt;
        PartitionContainer::const_iterator partIt = component.partitions.begin();
        PartitionContainer::const_iterator partEnd = component.partitions.end();

        for (; partIt != partEnd; ++partIt, ++number)
        {
            Partition& partition = **partIt;
            DiskContainer::const_iterator disk = disks.find(partition.diskId);
            assert(disk != disks.end());

            std::cout << "\tPartition #" << number << " from disk " << disk->second.path << std::endl;
			std::cout << "\tPartition #" << number << " from disk " << disk->second.name << std::endl;
			std::cout << "\tPartition id				: " << partition.id << std::endl;
			std::cout << "\tPartition name				: " << partition.name << std::endl;
            std::cout << "\tPartition offset			: " << Units(partition.diskOffsetLba * disk->second.sectorSize) << " " << partition.diskOffsetLba * disk->second.sectorSize << std::endl;
            std::cout << "\tPartition size				: " << Units(partition.sizeLba * disk->second.sectorSize) << " " << partition.sizeLba * disk->second.sectorSize << std::endl;
            if (volume.type == VtSpan || volume.type == VtSimple)
            {
                std::cout << "\tPartition offset in volume : " << Units(partition.volumeOffsetLba * disk->second.sectorSize) << " " << partition.volumeOffsetLba * disk->second.sectorSize << std::endl;
            }

			SmartLib::PartitionInfo partInfo;
		
			partInfo.m_path = disk->second.path;
			partInfo.m_diskName = disk->second.name.c_str();

			partInfo.m_id = partition.id;
			partInfo.m_diskOffsetLba = partition.diskOffsetLba;
			partInfo.m_diskOffset = partition.diskOffsetLba * disk->second.sectorSize;
			partInfo.m_sizeLba = partition.sizeLba;
			partInfo.m_size = partition.sizeLba * disk->second.sectorSize;
			partInfo.m_volumeOffsetLba = partition.volumeOffsetLba;
			partInfo.m_volumeOffset = partition.volumeOffsetLba * disk->second.sectorSize;
			partInfo.m_diskId = partition.diskId;

			partInfo.m_name = partition.name.c_str();
			partInfo.m_flags = partition.flags;
			partInfo.m_commitTransactionId = partition.commitTransactionId;
			partInfo.m_componentId = partition.componentId;


			partInfos.push_back(partInfo);
            
            std::cout << std::endl;
        }
    }
}

void ShowVolumes(const VolumeContainer& volumes, const DiskContainer& disks, std::vector<SmartLib::VolumeInfo> & volumeInfos)
{
    std::cout << "Number of volumes: " << volumes.size() << std::endl;
	volumeInfos.resize(volumes.size());

    VolumeContainer::const_iterator it = volumes.begin();
    VolumeContainer::const_iterator end = volumes.end();

	size_t index = 0;
    for (size_t i = 1; it != end; ++it, ++i)
    {
        const Volume& volume = it->second;

        std::cout << "Volume #" << i << std::endl;
		std::cout << "Volume name          : " << volume.name << std::endl;
		std::cout << "Volume size          : " << Units(volume.sizeLba * 512) << " " << volume.sizeLba * 512 << std::endl;
        std::cout << "Volume file system   : " << FsTypeToString(volume.fileSystem) << std::endl;
        std::cout << "Volume type          : " << VolumeTypeToString(volume.type) << std::endl;
		std::cout << "Volume typeString    : " << volume.typeString << std::endl;
        std::cout << "Number of partitions : " << CalculatePartitionsCount(volume) << std::endl;

		SmartLib::VolumeInfo & volInfo = volumeInfos[index++];
		volInfo.m_id = volume.id;
		volInfo.m_type = (INT)(SmartLib::VolumeType)volume.type;
		volInfo.m_typeS = VolumeTypeToString(volume.type);
		volInfo.m_fileSystem = (INT)(SmartLib::FsType)volume.fileSystem;
		volInfo.m_fileSystemS = FsTypeToString(volume.fileSystem);
		volInfo.m_sizeLba = volume.sizeLba;

		volInfo.m_name = volume.name.c_str();
		volInfo.m_typeString = volume.typeString.c_str();
		volInfo.m_disableDriveLetterAssignment = volume.disableDriveLetterAssignment.c_str();
		volInfo.m_state = volume.state.c_str();
		volInfo.m_readPolicy = volume.readPolicy;
		volInfo.m_volumeNumber = volume.volumeNumber;
		volInfo.m_flags = volume.flags;
		volInfo.m_numberOfComponents = volume.numberOfComponents;
		volInfo.m_commitTransactionId = volume.commitTransactionId;
		volInfo.m_unknownTransactionId = volume.unknownTransactionId;
		volInfo.m_volumeGuid = volume.volumeGuid;
		volInfo.m_numberOfPartitions = CalculatePartitionsCount(volume);


		std::vector<SmartLib::PartitionInfo> & partInfos = volInfo.m_partitions;
        ShowPartitions(volume, disks, partInfos);
        std::cout << std::endl;
    }
}

void ShowPartition(const char * prefix, const Disk & disk, const PartitionContainer & partitions, std::vector<SmartLib::PartitionInfo> & partInfos)
{
	auto begin = std::begin(partitions);
	auto end = std::end(partitions);
	partInfos.resize(partitions.size());
	size_t index = 0;
	for (auto iter = begin; iter != end; ++iter)
	{
		const Partition & partition = (**iter);
		std::cout << std::endl;
		std::cout << prefix << "Partition id				:" << partition.id << std::endl;
		std::cout << prefix << "Partition name				:" << partition.name << std::endl;
		std::cout << prefix << "Partition diskOffsetLba		:" << partition.diskOffsetLba << std::endl;
		std::cout << prefix << "Partition sizeLba			:" << partition.sizeLba << std::endl;
		std::cout << prefix << "Partition volumeOffsetLba	:" << partition.volumeOffsetLba << std::endl;
		std::cout << prefix << "Partition componentId		:" << partition.componentId << std::endl;
		std::cout << prefix << "Partition diskId			:" << partition.diskId << std::endl;

		SmartLib::PartitionInfo & partInfo = partInfos[index++];

		partInfo.m_path = disk.path;
		partInfo.m_diskName = disk.name.c_str();

		partInfo.m_id = partition.id;
		partInfo.m_diskOffsetLba = partition.diskOffsetLba;
		partInfo.m_diskOffset = partition.diskOffsetLba * disk.sectorSize;
		partInfo.m_sizeLba = partition.sizeLba;
		partInfo.m_size = partition.sizeLba * disk.sectorSize;
		partInfo.m_volumeOffsetLba = partition.volumeOffsetLba;
		partInfo.m_volumeOffset = partition.volumeOffsetLba * disk.sectorSize;
		partInfo.m_diskId = partition.diskId;

		partInfo.m_name = partition.name.c_str();
		partInfo.m_flags = partition.flags;
		partInfo.m_commitTransactionId = partition.commitTransactionId;
		partInfo.m_componentId = partition.componentId;

	}
}

void ShowDisks(const DiskContainer& disks, std::vector<SmartLib::DiskInfo> & diskInfos)
{
	auto begin = std::begin(disks);
	auto end = std::end(disks);
	diskInfos.resize(disks.size());
	size_t index = 0;
	for (auto iter = begin; iter != end; ++iter)
	{
		std::cout << std::endl << std::endl;
		const Disk & disk = iter->second;
		std::cout << "Disk id			:" << disk.id << std::endl;
		std::cout << "Disk guid			:" << disk.guid << std::endl;
		std::cout << "Disk name			:" << disk.name << std::endl;
		std::cout << "Partitions count	:" << disk.partitions.size() << std::endl;

		SmartLib::DiskInfo & diskInfo = diskInfos[index++];
		diskInfo.m_id = disk.id;
		diskInfo.m_guid = disk.guid.c_str();
		diskInfo.m_path = disk.path;
		diskInfo.m_sectorSize = disk.sectorSize;
		diskInfo.m_logicalStartLba = disk.logicalStartLba;

		diskInfo.m_name = disk.name.c_str();
		diskInfo.m_lastDevName = disk.lastDevName.c_str();
		diskInfo.m_flags = disk.flags;
		diskInfo.m_commitTransactionId = disk.commitTransactionId;
		diskInfo.m_numberOfPartitions = disk.partitions.size();
		std::vector<SmartLib::PartitionInfo> & partInfos = diskInfo.m_partions;
		ShowPartition("\t", disk, disk.partitions, partInfos);
	}
}

void ShowInfo(int count, char* diskNames[])
{
    try
    {
        Parser parser;
        for (int i = 0; i < count; ++i)
        {
            parser.Parse(diskNames[i]);
        }

        parser.BuildRelations();

        const VolumeContainer& volumes = parser.GetVolumes();
        const DiskContainer& disks = parser.GetDisks();

		//<sonmi01>2015-11-13 ###???
		SmartLib::DynamicVolumesDisksInfos infos;
        ShowVolumes(volumes, disks, infos.m_VolumeInfos);

		std::cout << std::endl << std::endl;
		std::cout << "Disks and partitions" << std::endl;
		
		ShowDisks(disks, infos.m_DiskInfos);

		infos.ToFile(FIELD_NAME(DynamicVolumesDisksInfos), TEXT("000-DynamicVolumesDisksInfos.xml"));
    }
    catch (const ErrnoException& ex)
    {
        std::cout << ex.what() << ": " << ex.GetDescription() << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
}


namespace SmartLib
{
	VOID ParseDynamicDisks(
		IN std::vector<IFileDevice*> & disks, 
		OUT SmartLib::DynamicVolumesDisksInfos & infos,
		LPCTSTR resultFile
		)
	{
		try
		{
			Parser parser;
			for (auto disk : disks)
			{
				parser.Parse(disk);
			}

			parser.BuildRelations();

			const VolumeContainer& volumes = parser.GetVolumes();
			const DiskContainer& disks = parser.GetDisks();

			ShowVolumes(volumes, disks, infos.m_VolumeInfos);

			std::cout << std::endl << std::endl;
			std::cout << "Disks and partitions" << std::endl;
			ShowDisks(disks, infos.m_DiskInfos);

			if (nullptr != resultFile && 0 != resultFile[0])
			{
				infos.ToFile(FIELD_NAME(DynamicVolumesDisksInfos), resultFile);
			}
		}
		catch (const ErrnoException& ex)
		{
			std::cout << ex.what() << ": " << ex.GetDescription() << std::endl;
		}
		catch (const std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}
}