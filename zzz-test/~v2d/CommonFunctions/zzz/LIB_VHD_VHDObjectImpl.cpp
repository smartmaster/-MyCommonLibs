#include "StdAfx.h"
#include "LIB_VHD_DiskImage.h"
#include "LIB_VHD_VHDObjectImpl.h"

CVHDObjectImpl::CVHDObjectImpl(VOID)
{
}

CVHDObjectImpl::~CVHDObjectImpl(VOID)
{
}

BOOL CVHDObjectImpl::Create(  LPCTSTR pszFilename, DISK_GEOMETRY* pDG, LARGE_INTEGER liDiskSize, LPCTSTR pszParentDisk, DWORD dwCreationDisposition)
{
	CDiskImage::IMAGE_PARAMETERS stParam;
	stParam.AdapterType = CDiskImage::IMAGE_ADAPTER_IDE;
	stParam.DiskSize.QuadPart = liDiskSize.QuadPart;
	stParam.szParentDiskFileName = pszParentDisk;
	memcpy_s(&stParam.Geometry, sizeof(DISK_GEOMETRY), pDG, sizeof(DISK_GEOMETRY));

	return m_cVHD.Create(pszFilename, &stParam, (CDiskImage::_IMAGE_CREATION_DISPOSITION)dwCreationDisposition);
}

BOOL CVHDObjectImpl::Close()
{
	return m_cVHD.Close();
}

BOOL CVHDObjectImpl::Seek(LARGE_INTEGER i64DistanceToMove, PLARGE_INTEGER pi64NewPosition, DWORD dwSeekMethod)
{
	return m_cVHD.Seek(i64DistanceToMove, pi64NewPosition, (CDiskImage::_IMAGE_SEEK_METHOD)dwSeekMethod);
}

BOOL CVHDObjectImpl::Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead)
{
	return m_cVHD.Read(lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead);
}

BOOL CVHDObjectImpl::Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten)
{
	return m_cVHD.Write(lpBuffer, dwNumberOfBytesToWrite, lpNumberOfBytesWritten);
}

BOOL CVHDObjectImpl::GetGeometry(PDISK_GEOMETRY pGeometry) CONST
{
	return m_cVHD.GetGeometry(pGeometry);
}

BOOL CVHDObjectImpl::GetDiskSize(PLARGE_INTEGER pi64DiskSize) CONST
{
	return m_cVHD.GetDiskSize(pi64DiskSize);
}

VOID CVHDObjectImpl::Release()
{
	delete this;
}

CVHDObject * CVHDObject::CreateInterfaceVhd()
{
	return static_cast<CVHDObject*>(new CVHDObjectImpl);
}