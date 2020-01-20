#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>

#include "..\..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\DiskExt.h"

namespace SmartLib
{
	VOID ParseDynamicDisks(
		IN std::vector<IFileDevice*> & disks,
		OUT SmartLib::DynamicVolumesDisksInfos & infos,
		LPCTSTR resultFile
		);
}