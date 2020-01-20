#pragma once

#include "LIB_VHD_VHDDef.h"
#include <list>

using namespace std ;

class CVirtualHardDisk;

typedef list<CVirtualHardDisk*> VHD_LIST;
typedef list<CVirtualHardDisk*>::iterator VHD_LIST_ITERATOR;

#define FILE_NAME_LENGTH 260

#define MAX_DISK_CHAIN	65536

typedef struct tagVHD_CREATEPARAM
{
	DISK_GEOMETRY stGeometry;
	LARGE_INTEGER liDiskSize;
	WCHAR szParentDiskFileName[FILE_NAME_LENGTH];
	DWORD dwCompressRatio;
}VHD_CREATEPARAM, *PVHD_CREATEPARAM;


class CVirtualHardDisk;

typedef struct _BAT_ELEMENT
{
	CVirtualHardDisk* pDisk;
	UINT32 nOffsetInSector;
}BATELEMENT, *PBATELEMENT;

class CVirtualHardDisk
{
protected:
	VHDFOOTER m_stFooter;
	VHDDYNHEADER m_stHeader;
	CVirtualHardDisk* m_pParentDisk;

	DWORD    m_dwErrorCode;

public:
	enum
	{
		VHD_Read = 1,
		VHD_Write = 2,
		VHD_Merge = 3,
	};

	CVirtualHardDisk()
	{
		ZeroMemory(&m_stFooter, sizeof(m_stFooter));
		ZeroMemory(&m_stHeader, sizeof(m_stHeader));

		m_pParentDisk = NULL;

		m_dwErrorCode = ERROR_SUCCESS;
	};

	virtual ~CVirtualHardDisk(){};

public:
	virtual INT32 Create(LPCTSTR pszFilename, DWORD dwCreationDisposition, PBATELEMENT pGlobalBAT, PVHD_CREATEPARAM pParam=NULL) = 0;
	virtual INT32 Close() = 0;
	virtual INT32 Seek(LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER pliNewPosition, DWORD dwMoveMethod) = 0;
	virtual INT32 Read(CONST VOID* pBuffer, DWORD dwNumberOfBytesToRead, PDWORD pdwNumberOfBytesRead) = 0;
	virtual INT32 Write(CONST VOID* pBuffer, DWORD dwNumberOfBytesToWrite, PDWORD pdwNumberOfBytesWritten) = 0;

	virtual INT32 GetParentDiskType() CONST = NULL;

	inline DWORD GetLastErrCode()
	{
		return m_dwErrorCode;
	}

	virtual PVHDFOOTER GetFooter()
	{
		return &m_stFooter;
	}

	virtual PVHDDYNHEADER GetHeader()
	{
		return &m_stHeader;
	}

	virtual VOID SetParent(CVirtualHardDisk* pParent)
	{
		m_pParentDisk = pParent;
	}

	virtual CVirtualHardDisk* GetParentDisk()
	{
		return m_pParentDisk;
	}
};
