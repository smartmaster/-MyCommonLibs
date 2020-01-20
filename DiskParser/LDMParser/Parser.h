#pragma once

#include "..\..\PublicHeader\PublicFileDeviceInterface.h"
#include "VblkParser.h"

class PhysicalDisk;
struct PrivHead;
struct TocBlock;
struct Vmdb;

class Parser
{
public:
    Parser();

    void Parse(const char* drivePath);
	void Parse(IFileDevice * fd);
    void BuildRelations();

    const VolumeContainer& GetVolumes() const;
    const DiskContainer& GetDisks() const;

private:
    void HandlePrivhead(PhysicalDisk& drive);
    void HandleTocBlock(const PrivHead* privhead, PhysicalDisk& drive);
    void HandleVmdb(const PrivHead* privhead, const TocBlock* tocblock, PhysicalDisk& drive);
    void HandleVblk(
        const PrivHead* privhead, const TocBlock* tocblock, const Vmdb* vmdb, PhysicalDisk& drive);

private:
    bool m_vblkIsParsed;
    VblkParser m_vblkParser;
};