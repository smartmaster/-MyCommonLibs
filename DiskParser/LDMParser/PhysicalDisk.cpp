#include "stdafx.h"
#include "stdafx.include.h"
#include "PhysicalDisk.h"
#include "ErrnoException.h"

#include "..\AdjustBasicDisk4KBTo512B\AdjustBasicDisk4KBTo512B.h"

#ifdef _WIN32
#define fseeko64 _fseeki64
#endif

//PhysicalDisk::PhysicalDisk(const char* path)
//    : m_path(path)
//    , m_drive(Open())
//    , m_sectorSize(GetSectorSize())
//{
//}

PhysicalDisk::PhysicalDisk(IFileDevice * fdDisk) :
	m_drive(fdDisk)//,
	//m_sectorSize(GetDiskInfo())
{
	ZeroMemory(&m_di, sizeof(m_di));
	GetDiskInfo();
}

uint32_t PhysicalDisk::SectorSize()
{
    return m_di.m_sectorSize;
}

//bool PhysicalDisk::IsLdmDrive()
//{
//	return m_di.m_isDynamicDisk;
//}

//const std::string& PhysicalDisk::GetPath()
//{
//    return m_path;
//}

IFileDevice * PhysicalDisk::GetFileDevice()
{
	return (IFileDevice *)(m_drive);
}

uint32_t PhysicalDisk::Read(void* buffer, uint32_t length, uint64_t offset)
{
	ULONG NumberOfBytesRead = 0;
	HRESULT hr = ReadAtOffsetHelper(m_drive, offset, length, &NumberOfBytesRead, (LPBYTE)buffer);
	if (FAILED(hr))
	{
		throw ErrnoException("Failed to seek physical disk");
	}
	return NumberOfBytesRead;
#if 0
    if (fseeko64(m_drive, offset, SEEK_SET) == -1)
    {
        throw ErrnoException("Failed to seek physical disk");
    }

    uint32_t result = fread(buffer, 1, length, m_drive);
    if (result == static_cast<uint32_t>(-1))
    {
        throw ErrnoException("Failed to read physical disk");
    }

    return result;
#endif
}

const SmartLib::DiskInfo & PhysicalDisk::DiskInfo()
{
	return m_di;
}

//FILE* PhysicalDisk::Open()
//{
//	FILE* drive = nullptr;
//	errno_t err = fopen_s(
//		&drive,//FILE** pFile,
//		m_path.c_str(),//const char *filename,
//		"rb"//const char *mode
//		);
//	//drive = fopen(m_path.c_str(), "rb");
//    if (0 != err || !drive)
//    {
//        throw ErrnoException("Failed to open physical disk");
//    }
//
//    return drive;
//}

uint32_t PhysicalDisk::GetDiskInfo()
{
	HRESULT hr = SmartLib::GetDiskInfo(m_drive, m_di);
	if (FAILED(hr))
	{
		throw ErrnoException("failed to GetDiskInfo", hr);
	}
	return hr;
#if 0
    const uint32_t defaultValue = 512;

#ifdef _WIN32
    HANDLE drive = ::CreateFileA(
        m_path.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_DEVICE,
        NULL);

    if (drive == INVALID_HANDLE_VALUE)
    {
        std::cout << __FUNCTION__": failed to open drive. Using default sector size value" << std::endl;
        return defaultValue;
    }

    DISK_GEOMETRY geometry = {};
    DWORD ret = 0;
    BOOL res = ::DeviceIoControl(
        drive, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &geometry, sizeof(DISK_GEOMETRY), &ret, NULL);

    ::CloseHandle(drive);

    if (!res)
    {
        std::cout << __FUNCTION__": IoControl failed. Using default sector size value" << std::endl;
        return defaultValue;
    }

    return geometry.BytesPerSector;
#else
    int drive = open(m_path.c_str(), O_RDONLY);
    if (drive == -1)
    {
        std::cout << __FUNCTION__ << ": failed to open drive. Using default sector size value" << std::endl;
        return defaultValue;
    }

    size_t blockSize = 0;
    int res = ioctl(drive, BLKSSZGET, &blockSize);
    close(drive);

    if (res == -1)
    {
        std::cout << __FUNCTION__ << ": ioctl failed. Using default sector size value" << std::endl;
        return defaultValue;
    }

    return blockSize;
#endif
#endif
}
