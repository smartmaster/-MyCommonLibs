#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include "DiskExt.h"

namespace SmartLib
{
	struct CreateIntanceVolumeIoFileDeviceParam
	{
		VolumeType m_volumeType;
		std::vector<DiskExtent> m_disks;
		LONG m_strippedVBlockSize; //used only by stripped or raid5 volumme
	};

	void VolumeInfoToCreateIntanceVolumeIoFileDeviceParam(CONST VolumeInfo & volInfo, CreateIntanceVolumeIoFileDeviceParam & param);

	HRESULT CreateIntanceVolumeIoFileDevice(CreateIntanceVolumeIoFileDeviceParam & param, IFileDeviceFlush ** ppobj);
}