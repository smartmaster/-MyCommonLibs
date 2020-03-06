#include "stdafx.h"

#include "AllVolumeDiskInfoXml.h"

#include "AllVolumeDiskInfoXml.h.xml.Output.hpp"


HRESULT GetAllVolumeDiskInfoXml(CAllVolumeDiskInfoXml & AllVolumeDiskInfoXml, BOOL /*bGetVolumeBitmap*/)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	
	BEGIN_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	INT diskCount = 256; //assume max 256 disks

	//////////////////////////////////////////////////////////////////////////
	{
		CAllVdsDiskVolumeInfo var;
		var.Retrieve();
		BinXmlConv_NS::Bin2Xml(var, AllVolumeDiskInfoXml.AllVdsDiskVolumeInfoXml);

		if (var.m_pVdsDiskInfos.size())
		{
			diskCount = var.m_pVdsDiskInfos.size() + 2; // check 2 additional disks
		}
	}

	//////////////////////////////////////////////////////////////////////////
	{
		CAllVolumeInfo var;
		var.Retrieve(FALSE);
		BinXmlConv_NS::Bin2Xml(var, AllVolumeDiskInfoXml.AllVolumeInfoXml);
	}

	//////////////////////////////////////////////////////////////////////////
	{
		CAllDiskInfo var;
		var.Retrieve(diskCount);
		BinXmlConv_NS::Bin2Xml(var, AllVolumeDiskInfoXml.AllDiskInfoXml);
	}
	
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	CoUninitialize();
	
	return hr;
}
