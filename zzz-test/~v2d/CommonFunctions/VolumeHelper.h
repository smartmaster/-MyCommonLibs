#pragma once 

#include "stdafx.h"
#include "Ntddvol.h"

#include <vector>
using namespace std;

struct CVolumeHelper
{
	//******************************************************************************
	static INT GetAllVolumeGuid(vector<CString> & VolumeGuid);
	static INT GetVolumePathFromGuid(LPCTSTR szVolumeGuid, vector<CString> & Path);
	static INT GetVolumeRootAndGuidFromPath(LPCTSTR szPath, CString & strRoot, CString & strGuid);
	static INT GetVolumeMountPoint(LPCTSTR szRootPathName, vector<CString> & MountPoint);

	struct CVolumeInfomation
	{
		CString m_VolumeNameBuffer;
		DWORD m_VolumeSerialNumber;
		DWORD m_MaximumComponentLength;
		DWORD m_FileSystemFlags;
		CString m_FileSystemNameBuffer;
	};
	static INT GetVolumeInfo(LPCTSTR lpRootPathName, CVolumeInfomation & volumeInfo);

	static HANDLE CreateHandle(LPCTSTR szVolume, DWORD dwDesiredAccess);
	
	static INT FSCTL_GET_NTFS_VOLUME_DATA_Helper(HANDLE hDevice, NTFS_VOLUME_DATA_BUFFER & NtfsVolumeDataBuffer);
	static INT FSCTL_GET_RETRIEVAL_POINTERS_Helper(HANDLE hDevice, RETRIEVAL_POINTERS_BUFFER ** ppRetrievalPointersBuffer);
	static INT FSCTL_GET_VOLUME_BITMAP_Helper(HANDLE hDevice, VOLUME_BITMAP_BUFFER ** ppVolumeBitmapBuffer);
	static INT IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS_Helper(HANDLE hDevice, VOLUME_DISK_EXTENTS ** ppVolumeDiskExtents);
	static INT IOCTL_VOLUME_LOGICAL_TO_PHYSICAL_Helper(HANDLE hDevice, VOLUME_LOGICAL_OFFSET & VolumeLogicalOffset, VOLUME_PHYSICAL_OFFSETS ** ppVolumePhysicalOffsets);
	static INT IOCTL_VOLUME_PHYSICAL_TO_LOGICAL_Helper(HANDLE hDevice, VOLUME_PHYSICAL_OFFSET & VolumePhysicalOffset, VOLUME_LOGICAL_OFFSET & VolumeLogicalOffset);
	static INT FSCTL_GET_RETRIEVAL_POINTER_BASE_Helper(HANDLE hDevice, RETRIEVAL_POINTER_BASE & RetrievalPointerBase);
	static INT FSCTL_QUERY_FILE_SYSTEM_RECOGNITION_Helper(HANDLE hDevice, FILE_SYSTEM_RECOGNITION_INFORMATION & FileSystemRecognitionInformation);
	static INT IOCTL_VOLUME_GET_GPT_ATTRIBUTES_Helper(HANDLE hDevice, VOLUME_GET_GPT_ATTRIBUTES_INFORMATION & GptAttributes);
};

INT TestVolume();