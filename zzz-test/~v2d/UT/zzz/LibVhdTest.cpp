#include "stdafx.h"
#include "..\CommonFunctions\LIB_VHD_VHDObject.h"

VOID LibVhdTest(LPCTSTR pszVhdFileName)
{
	CVHDObject * pVHDObject = CVHDObject::CreateInterfaceVhd();

	LARGE_INTEGER liDiskSize = {0};
	liDiskSize.QuadPart = 1024;
	liDiskSize.QuadPart *= 1024;
	liDiskSize.QuadPart *= 1024;

	DISK_GEOMETRY diskGeo = {0};
	pVHDObject->Create(pszVhdFileName, &diskGeo, liDiskSize, NULL, CREATE_NEW);
	pVHDObject->GetGeometry(&diskGeo);
	pVHDObject->Close();
	pVHDObject->Release();
}
