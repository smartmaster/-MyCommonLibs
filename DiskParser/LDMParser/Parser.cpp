#include "stdafx.h"
#include "stdafx.include.h"
#include "Parser.h"
#include "PhysicalDisk.h"
#include "Mbr.h"
#include "Ldm.h"
#include "Endian.h"
#include "ErrnoException.h"

static bool IsLdmDrive(PhysicalDisk& drive)
{
	return drive.DiskInfo().m_isDynamicDisk;
#if 0
    std::vector<uint8_t> mbrSector(drive.SectorSize());
    drive.Read(&mbrSector[0], mbrSector.size(),  0);

    const MasterBootRecord* mbr = reinterpret_cast<const MasterBootRecord*>(&mbrSector[0]);

    return (mbr->mbrSignature == MbrMagicSignature &&
            mbr->partitions[0].partitionType == PtDynamic);
#endif
}

static uint64_t TocOffset(const PrivHead* privhead, PhysicalDisk& drive)
{
    uint64_t privateRegion = ByteReverse(privhead->privateRegionStart);
    uint64_t tocOffset = ByteReverse(privhead->primaryTocLba);
    return (privateRegion + tocOffset) * drive.SectorSize();
}

static uint64_t VmdbOffset(const PrivHead* privhead, const TocBlock* tocblock, PhysicalDisk& drive)
{
    uint64_t privateRegion = ByteReverse(privhead->privateRegionStart);
    uint64_t configOffset = ByteReverse(tocblock->config.start);
    return (privateRegion + configOffset) * drive.SectorSize();
}

static uint64_t FirstVblkOffset(
    const PrivHead* privhead, const TocBlock* tocblock, const Vmdb* vmdb, PhysicalDisk& drive)
{
    return VmdbOffset(privhead, tocblock, drive) + ByteReverse(vmdb->firstVblkOffset);
}

Parser::Parser()
    : m_vblkIsParsed(false)
{
}

void Parser::Parse(const char* drivePath)
{
	CComPtr<IFileDevice> spIFileDevice;
	CString strDrive(drivePath);
	HRESULT hr = CreateInstanceFileDeviceWin32(
		strDrive,//IN LPCTSTR strFileName,
		GENERIC_READ,//IN ULONG ulDesiredAccess,
		FILE_SHARE_READ | FILE_SHARE_WRITE,//IN ULONG ulShareMode,
		OPEN_EXISTING,//IN ULONG ulCreationDisposition,
		0,//IN ULONG ulFlagsAndAttributes,
		&spIFileDevice,//OUT IFileDevice ** ppFileDevice,
		TRUE//,//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		throw ErrnoException("Failed to seek physical disk", hr);
	}

	Parse(spIFileDevice);

    //PhysicalDisk drive(spIFileDevice);

    //if (!IsLdmDrive(drive))
    //{
    //    return;
    //}

    //HandlePrivhead(drive);
}


void Parser::Parse(IFileDevice * fd)
{
	PhysicalDisk drive(fd);

	if (!IsLdmDrive(drive))
	{
		return;
	}

	HandlePrivhead(drive);
}

void Parser::BuildRelations()
{
    m_vblkParser.BuildRelations();
}

const VolumeContainer& Parser::GetVolumes() const
{
    return m_vblkParser.GetVolumes();
}

const DiskContainer& Parser::GetDisks() const
{
    return m_vblkParser.GetDisks();
}

void Parser::HandlePrivhead(PhysicalDisk& drive)
{
    std::vector<uint8_t> privheadSector(drive.SectorSize());
	if (drive.DiskInfo().m_isGPTDisk)
	{
		drive.Read(&privheadSector[0], privheadSector.size(), (drive.DiskInfo().m_GPTLDMStartLBA + (uint64_t)g_privHeadOffsetGPTSector) * drive.SectorSize());
	}
	else
	{
		drive.Read(&privheadSector[0], privheadSector.size(), (uint64_t)g_privHeadOffsetSector * drive.SectorSize());
	}

    const PrivHead* privhead = reinterpret_cast<const PrivHead*>(&privheadSector[0]);
    if (memcmp(&privhead->magic[0], g_privHeadMagic, sizeof(privhead->magic)) != 0)
    {
        throw std::runtime_error("Invalid PrivHead magic constant");
    }

    if (!m_vblkIsParsed)
    {
        HandleTocBlock(privhead, drive);
        m_vblkIsParsed = true;
    }

    m_vblkParser.SetDiskParameters(
        reinterpret_cast<const char*>(privhead->diskId),
        //drive.GetPath(),
		drive.GetFileDevice(),
        drive.SectorSize(),
        ByteReverse(privhead->publicRegionStart));
}

void Parser::HandleTocBlock(const PrivHead* privhead, PhysicalDisk& drive)
{
    std::vector<uint8_t> tocblockSector(drive.SectorSize());
    drive.Read(&tocblockSector[0], tocblockSector.size(), TocOffset(privhead, drive));

    const TocBlock* tocblock = reinterpret_cast<const TocBlock*>(&tocblockSector[0]);
    if (memcmp(&tocblock->magic[0], g_tocblockMagic, sizeof(tocblock->magic)) != 0)
    {
        throw std::runtime_error("Invalid TocBlock magic constant");
    }

    HandleVmdb(privhead, tocblock, drive);
}

void Parser::HandleVmdb(const PrivHead* privhead, const TocBlock* tocblock, PhysicalDisk& drive)
{
    std::vector<uint8_t> vmdbSector(drive.SectorSize());
    drive.Read(&vmdbSector[0], vmdbSector.size(), VmdbOffset(privhead, tocblock, drive));

    const Vmdb* vmdb = reinterpret_cast<const Vmdb*>(&vmdbSector[0]);
    if (memcmp(&vmdb->magic[0], g_vmdbMagic, sizeof(vmdb->magic)) != 0)
    {
        throw std::runtime_error("Invalid Vmdb magic constant");
    }

    HandleVblk(privhead, tocblock, vmdb, drive);
}

void Parser::HandleVblk(
    const PrivHead* privhead, const TocBlock* tocblock, const Vmdb* vmdb, PhysicalDisk& drive)
{
    std::vector<char> vblkLba(drive.SectorSize());

    uint64_t firstVblkOffset = FirstVblkOffset(privhead, tocblock, vmdb, drive);
    uint32_t sizeOfVblk = ByteReverse(vmdb->sizeOfVblk);

    if (drive.SectorSize() % sizeOfVblk != 0)
    {
        throw std::runtime_error("Size of Vblk is not multiple of sector size");
    }

    uint32_t sectorVblk = drive.SectorSize() / sizeOfVblk;
    uint32_t count = (ByteReverse(vmdb->sequenceNumberOfLastVblk) - g_vblkStartSequenceNumber) / sectorVblk;

    for (uint32_t i = 0; i < count; ++i)
    {
        uint64_t vblkOffset = firstVblkOffset + i * drive.SectorSize();
        drive.Read(&vblkLba[0], vblkLba.size(), vblkOffset);

        for (uint32_t j = 0; j < sectorVblk; ++j)
        {
            const ExtendedVblk* vblk = reinterpret_cast<const ExtendedVblk*>(&vblkLba[sizeOfVblk * j]);
            if (memcmp(&vblk->header.magic[0], g_vblkMagic, sizeof(vblk->header.magic)) != 0)
            {
                std::cout << "Invalid Vblk magic constant, skip" << std::endl;
                continue;
            }

            m_vblkParser.Parse(vblk);
        }
    }
}
