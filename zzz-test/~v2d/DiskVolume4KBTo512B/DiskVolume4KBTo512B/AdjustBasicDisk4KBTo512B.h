#pragma once

#include <Windows.h>
#include "FileDeviceInterface.h"


#if defined(ADJUST_DISK_DLLAPI_76E26A7E_D764_4685_BFD9_1A40D6A0AFF1)
#define ADJUST_DISK_4KBTO512B_DLLAPI __declspec(dllexport)
#else
#define ADJUST_DISK_4KBTO512B_DLLAPI __declspec(dllimport)
#endif

ADJUST_DISK_4KBTO512B_DLLAPI 
HRESULT Adjust4KBSectorBasicDisk(IFileDevice * pDiskRW);