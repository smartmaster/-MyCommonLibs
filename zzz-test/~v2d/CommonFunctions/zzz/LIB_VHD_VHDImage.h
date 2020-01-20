#pragma once
#include "LIB_VHD_diskimage.h"
#include "LIB_VHD_VHD.h"
#include <string>

class CVHDImage :
	public CDiskImage
{
protected:
	CVirtualHardDisk*  m_pBottomVHD;
	VHD_LIST m_listVHD;
	PBATELEMENT m_pBAT;

	DISK_GEOMETRY m_stGeometry;
	LONGLONG m_llDiskSize;

	//revert modification time when close disk.
	FILETIME m_ftCreation;
	FILETIME m_ftModification;

	std::wstring m_strFilename;

public:
	CVHDImage(VOID);
public:
	~CVHDImage(VOID);

public:
	BOOL Create(LPCTSTR szImageFileName, PIMAGE_PARAMETERS pParameters, IMAGE_CREATION_DISPOSITION nCreationDisposition, DWORD dwCompressRatio = 0);
	BOOL Close();

	BOOL Seek(LARGE_INTEGER i64DistanceToMove, PLARGE_INTEGER pi64NewPosition, IMAGE_SEEK_METHOD nSeekMethod);
	BOOL Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead);
	BOOL Write(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten);

	CVirtualHardDisk* GetBottomVHD(VOID) CONST;

	BOOL GetGeometry(PDISK_GEOMETRY pGeometry) CONST;
	BOOL GetDiskSize(PLARGE_INTEGER pi64DiskSize) CONST;

	BOOL Merge(ICallBacks * pCallBack);
	BOOL GetFooter(BYTE* pFooter, DWORD dwBufSize);

	LPCTSTR   GetFilename() CONST;
	FILETIME  GetModificationTime() CONST;
	CVirtualHardDisk* GetVHD() {return m_pBottomVHD;};

	static INT SectorSortFunc(VOID* pParam, CONST VOID *arg1, CONST VOID *arg2 );
	BOOL Sort(ULONG * SectorArray, DWORD Size);
	BOOL GetSectorList(ULONG * SectorArray, DWORD Size);
	PBATELEMENT GetBATRef();
};
