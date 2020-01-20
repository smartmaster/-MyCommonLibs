#pragma once

#include "LIB_VHD_vhd.h"
#include "LIB_VHD_autoHandle.h"
#include <string>

class CDynamicVHD :
	public CVirtualHardDisk
{
	friend class CVHDImage;

protected:
	BOOL   m_bD2D;
	std::wstring m_strFilename;
	CSafeHandle<>  m_hVHDFile;

	PBATELEMENT m_pBAT;

	ULONG m_ulSectorsPerBlock;
	ULONG m_ulBlockBitmapSectorCount;
	ULONG m_ulChunkSize;
	ULONGLONG m_ullCurPos;
	BYTE* m_pChunkBuf;

	//BOOL m_bChunkBufferDirty;

	//we will not remove the VHD footer until a write operation want to
	//increase VHD file. when that happened, this will be set to TRUE.
	//it means the VHD footer had already been removed from VHD file,
	//we must append the correct footer when VHD close.
	BOOL m_bNeedWriteFooterWhenClose;

	BOOL m_bUseBuffering;

public:
	enum
	{
		VHDType = 3,
	};

public:
	CDynamicVHD(VOID);
	~CDynamicVHD(VOID);

public:
	INT32 Create(LPCTSTR pszFilename, DWORD dwCreationDisposition, PBATELEMENT pGlobalBAT, PVHD_CREATEPARAM pParam=NULL);
	INT32 Close();
	INT32 Seek(LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER pliNewPosition, DWORD dwMoveMethod);
	INT32 Read(CONST VOID* pBuffer, DWORD dwNumberOfBytesToRead, PDWORD pdwNumberOfBytesRead);
	INT32 Write(CONST VOID* pBuffer, DWORD dwNumberOfBytesToWrite, PDWORD pdwNumberOfBytesWritten);

	inline INT32 GetParentDiskType() CONST {return 0;};

	inline PVHDDYNHEADER GetDynDiskHeader() {return &m_stHeader;};
	inline LPCTSTR GetDiskName() CONST{return m_strFilename.c_str();};

protected:
	VOID LtoP(LONGLONG llLogic, LONGLONG* pllPhysical);
};
