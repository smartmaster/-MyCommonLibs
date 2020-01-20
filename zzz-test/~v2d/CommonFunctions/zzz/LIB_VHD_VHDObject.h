#pragma once

class CVHDObject
{
public:
	virtual BOOL Create(  LPCWSTR pszFilename, 
		DISK_GEOMETRY* pDG, 
		LARGE_INTEGER liDiskSize, 
		LPCWSTR pszParentDisk,   //it can be NULL
		DWORD dwCreationDisposition   //CREATE_NEW or OPEN_EXISTING
		) = NULL;

	virtual BOOL Close() = NULL;
	virtual BOOL Seek(LARGE_INTEGER i64DistanceToMove, PLARGE_INTEGER pi64NewPosition, DWORD dwSeekMethod/*FILE_BEGIN;FILE_CURRENT;FILE_END*/) = NULL;
	virtual BOOL Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead) = NULL;
	virtual BOOL Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpdwNumberOfBytesWritten) = NULL;

	virtual BOOL GetGeometry(PDISK_GEOMETRY pGeometry) CONST = NULL;
	virtual BOOL GetDiskSize(PLARGE_INTEGER pi64DiskSize) CONST = NULL;

	virtual VOID Release() = NULL;

	//********************************************************************
	static CVHDObject * CreateInterfaceVhd();
};
