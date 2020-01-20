#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "FileGuard.h"
#include "..\AdjustBasicDisk4KBTo512B\AdjustBasicDisk4KBTo512B.h"

class PhysicalDisk
{
public:
    //PhysicalDisk(const char* path);
	PhysicalDisk(IFileDevice * fdDisk);

    //const std::string& GetPath();
	IFileDevice * GetFileDevice();
    uint32_t SectorSize();
	//bool IsLdmDrive();
    uint32_t Read(void* buffer, uint32_t length, uint64_t offset);
	const SmartLib::DiskInfo & DiskInfo();

private:
    //FILE* Open();
    uint32_t GetDiskInfo();

private:
    //std::string m_path;
    FileGuard m_drive;
    //int32_t m_sectorSize;
	SmartLib::DiskInfo m_di;
};
