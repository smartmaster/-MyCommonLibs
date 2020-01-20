#pragma once

#include "LIB_VHD_VhdDef.h"


typedef struct _MergeCallBackInformation
{
	ULONGLONG TotalSize;
	ULONGLONG FinishedSize;
}ST_MergeCallBackInformation, *LP_MergeCallBackInformation;

interface ICallBacks
{
	virtual DWORD MergeCallBack(LP_MergeCallBackInformation MergeInformation) {return 0;}
};

class CDiskImage
{
public:
	typedef enum _IMAGE_CREATION_DISPOSITION {
		IMAGE_CREATE = CREATE_NEW,
		IMAGE_OPEN = OPEN_EXISTING
	} IMAGE_CREATION_DISPOSITION;

	typedef enum _IMAGE_SEEK_METHOD {
		IMAGE_BEGIN = FILE_BEGIN,
		IMAGE_CURRENT = FILE_CURRENT,
		IMAGE_END = FILE_END
	} IMAGE_SEEK_METHOD;

	typedef enum _DISK_ADAPTER_TYPE {
		IMAGE_ADAPTER_SCSI,
		IMAGE_ADAPTER_IDE
	} DISK_ADAPTER_TYPE;

	typedef struct _IMAGE_PARAMETERS {
		DISK_GEOMETRY Geometry;
		LARGE_INTEGER DiskSize;
		LPCTSTR szParentDiskFileName;
		DISK_ADAPTER_TYPE AdapterType;
	} IMAGE_PARAMETERS, *PIMAGE_PARAMETERS;

	DWORD    m_dwErrCode;

public:
	CDiskImage() { m_dwErrCode = VHD_RET_SUCCESSFULL; }
	virtual ~CDiskImage() = NULL {};

	virtual BOOL Create(LPCTSTR szImageFileName, PIMAGE_PARAMETERS pParameters, IMAGE_CREATION_DISPOSITION nCreationDisposition, DWORD dwCompressRatio) = NULL;
	virtual BOOL Close() = NULL;

	virtual BOOL Seek(LARGE_INTEGER i64DistanceToMove, PLARGE_INTEGER pi64NewPosition, IMAGE_SEEK_METHOD nSeekMethod) = NULL;
	virtual BOOL Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead) = NULL;
	virtual BOOL Write(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten) = NULL;

	virtual BOOL GetGeometry(PDISK_GEOMETRY pGeometry) CONST = NULL;
	virtual BOOL GetDiskSize(PLARGE_INTEGER pi64DiskSize) CONST = NULL;

	virtual BOOL Merge(ICallBacks * pCallBack) = NULL;
	virtual BOOL Sort(ULONG * SectorArray, DWORD Size) = NULL;
	virtual BOOL GetSectorList(ULONG* pList, DWORD pCount) = NULL;
	virtual BOOL GetFooter(BYTE* pFooter, DWORD dwBufSize) = NULL;

	virtual DWORD GetLastErrCode() { return m_dwErrCode; }
};