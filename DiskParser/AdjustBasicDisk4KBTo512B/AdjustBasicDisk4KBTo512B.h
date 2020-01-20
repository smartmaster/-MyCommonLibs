#pragma once

#include <Windows.h>
#include "..\..\PublicHeader\PublicFileDeviceInterface.h"


//#if defined(ADJUST_DISK_DLLAPI_76E26A7E_D764_4685_BFD9_1A40D6A0AFF1)
//#define ADJUST_DISK_4KBTO512B_DLLAPI __declspec(dllexport)
//#else
//#define ADJUST_DISK_4KBTO512B_DLLAPI __declspec(dllimport)
//#endif

#define ADJUST_DISK_4KBTO512B_DLLAPI

namespace SmartLib
{
	struct DiskInfo
	{
		UINT m_sectorSize;
		BOOL m_isGPTDisk;
		BOOL m_isDynamicDisk;
		LONGLONG m_GPTLDMStartLBA;
	};

	ADJUST_DISK_4KBTO512B_DLLAPI
	HRESULT GetDiskInfo(IFileDevice * pDiskRW, DiskInfo & di);

	ADJUST_DISK_4KBTO512B_DLLAPI
	HRESULT Adjust4KBSectorBasicDisk(IFileDevice * pDiskRW);
}

