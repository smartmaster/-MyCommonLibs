#pragma once

#include <Windows.h>
#include "LIB_VHD_vhdObject.h"
#include "LIB_VHD_VHDImage.h"

class CVHDObjectImpl :
	public CVHDObject
{
protected:
	CVHDImage  m_cVHD;

public:
	CVHDObjectImpl(VOID);
	~CVHDObjectImpl(VOID);

public:
	BOOL Create(LPCTSTR pszFilename, DISK_GEOMETRY* pDG, LARGE_INTEGER liDiskSize, LPCTSTR pszParentDisk, DWORD dwCreationDisposition);
	BOOL Close();
	BOOL Seek(LARGE_INTEGER i64DistanceToMove, PLARGE_INTEGER pi64NewPosition, DWORD dwSeekMethod);
	BOOL Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead);
	BOOL Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten);

	BOOL GetGeometry(PDISK_GEOMETRY pGeometry) CONST;
	BOOL GetDiskSize(PLARGE_INTEGER pi64DiskSize) CONST;
	VOID Release();
};
