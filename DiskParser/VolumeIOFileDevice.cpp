#include "stdafx.h"
#include "VolumeIOFileDevice.h"

#include "MirrorAndSingleSimpleRW.h"
#include "SpannedAndSpannedSimpleRW.h"
#include "StrippedAndRaid5RW.h"


namespace SmartLib
{

	void VolumeInfoToCreateIntanceVolumeIoFileDeviceParam(CONST VolumeInfo & volInfo, CreateIntanceVolumeIoFileDeviceParam & param)
	{
		param.m_volumeType = (VolumeType)volInfo.m_type;
		if (VolumeType::VtRaid5 == param.m_volumeType)
		{
			param.m_strippedVBlockSize = 64 * 1024; //hard coded for now
		}
		else if (VolumeType::VtStripe == param.m_volumeType)
		{
			param.m_strippedVBlockSize = 64 * 1024; //hard coded for now
		}
		else
		{
			param.m_strippedVBlockSize = 0;
		}
		
		param.m_disks.resize(volInfo.m_partitions.size());
		for (size_t ii = 0; ii < param.m_disks.size(); ++ii)
		{
			DiskExtent & diskExt = param.m_disks[ii];
			CONST PartitionInfo & partInfo = volInfo.m_partitions[ii];
			diskExt.m_disk = partInfo.m_path;
			diskExt.m_offset = partInfo.m_diskOffset;
			diskExt.m_length = partInfo.m_size;
			diskExt.m_volumeOffset = partInfo.m_volumeOffset;
		}
	}

	HRESULT CreateIntanceVolumeIoFileDevice(CreateIntanceVolumeIoFileDeviceParam & param, IFileDeviceFlush ** ppobj)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		switch (param.m_volumeType)
		{
		case VolumeType::VtSimple:
		{
			if (1 == param.m_disks.size())
			{
				hr = CreateInstanceCMirrorAndSingleSimpleRWCacheIOMaker(
					param.m_disks,//std::vector<DiskExtent> & disks, 
					ppobj,//IFileDeviceFlush ** ppObj, 
					TRUE//,/BOOL bCreateMember
					);
			}
			else
			{
				hr = CreateInstanceCSpannedAndSpannedSimpleRWCacheIOMaker(
					param.m_disks,//std::vector<DiskExtent> & disks, 
					ppobj,//IFileDeviceFlush ** ppObj, 
					TRUE//,//BOOL bCreateMember
					);
			}
		}
		break;

		case VolumeType::VtSpan:
		{
			hr = CreateInstanceCSpannedAndSpannedSimpleRWCacheIOMaker(
				param.m_disks,//std::vector<DiskExtent> & disks, 
				ppobj,//IFileDeviceFlush ** ppObj, 
				TRUE//,//BOOL bCreateMember
				);
		}
		break;

		case VolumeType::VtMirror:
		{
			hr = CreateInstanceCMirrorAndSingleSimpleRWCacheIOMaker(
				param.m_disks,//std::vector<DiskExtent> & disks, 
				ppobj,//IFileDeviceFlush ** ppObj, 
				TRUE//,/BOOL bCreateMember
				);
		}
		break;

		case VolumeType::VtStripe:
		{
			hr = CreateInstanceCStrippedAndRaid5RWCacheIOMaker(
				param.m_disks,//std::vector<DiskExtent> & disks, 
				param.m_strippedVBlockSize,//LONG blockSize, 
				FALSE,//BOOL isRaid5, 
				ppobj,//IFileDeviceFlush ** ppObj, 
				TRUE//,//BOOL bCreateMember
				);
		}
		break;

		case VolumeType::VtRaid5:
		{
			hr = CreateInstanceCStrippedAndRaid5RWCacheIOMaker(
				param.m_disks,//std::vector<DiskExtent> & disks, 
				param.m_strippedVBlockSize,//LONG blockSize, 
				TRUE,//BOOL isRaid5, 
				ppobj,//IFileDeviceFlush ** ppObj, 
				TRUE//,//BOOL bCreateMember
				);
		}
		break;

		default:
		{
			hr = E_INVALIDARG;
			D_API_ERR(0, TEXT("Invalid Volume type"), hr, TEXT(""));
		}
		break;
		}

		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}

} //namespace SmartLib