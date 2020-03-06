// DiskParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>

#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicNtfsTest.h"

#include "..\PublicHeader\PublicMTCopy.h"
#include "..\PublicHeader\PublicSimpleBitArray.h"
#include "..\PublicHeader\PublicVMemPtr.h"

#include ".\LDMParser\LdmParser.h"
#include "VolumeIOFileDevice.h"

extern int main_parser(int argc, char* argv[]);



int main(int argc, char ** argv)
{
	int testCase = 1;

	D_SETTINGS(999, FALSE, TEXT(""), TEXT(""));

	switch (testCase)
	{
	case 2:
	{
		HRESULT hr = S_OK;
		std::vector<IFileDevice*> diskFd;
		for (int ii = 1; ii < argc; ++ii)
		{
			CString name(argv[ii]);
			CComPtr<IFileDevice> spDisk;
			hr = CreateInstanceFileDeviceWin32(
				name.GetString(),//IN LPCTSTR strFileName,
				GENERIC_READ | GENERIC_WRITE,//IN ULONG ulDesiredAccess,
				FILE_SHARE_READ | FILE_SHARE_WRITE,//IN ULONG ulShareMode,
				OPEN_EXISTING,//IN ULONG ulCreationDisposition,
				0,//IN ULONG ulFlagsAndAttributes,
				&spDisk,//OUT IFileDevice ** ppFileDevice,
				TRUE//,//BOOL bCreateMember
				);
			if (!!spDisk)
			{
				diskFd.push_back(spDisk.Detach());
			}
		}

		SmartLib::DynamicVolumesDisksInfos infos;
		SmartLib::ParseDynamicDisks(
			diskFd,//IN std::vector<IFileDevice*> & disks,
			infos,//OUT SmartLib::DynamicVolumesDisksInfos & infos,
			nullptr//,//LPCTSTR resultFile
			);

		for (CONST auto & volInfo : infos.m_VolumeInfos)
		{
			D_INFO(0, TEXT("Begin copying volume [%s]"), volInfo.m_name.GetString());
			SmartLib::CreateIntanceVolumeIoFileDeviceParam param;
			VolumeInfoToCreateIntanceVolumeIoFileDeviceParam(volInfo, param);

			CComPtr<IFileDeviceFlush> spVolumeSrc;
			hr = CreateIntanceVolumeIoFileDevice(param, &spVolumeSrc);

			CComPtr<IFileDeviceFlush> spVolumeDest;
			hr = CreateIntanceVolumeIoFileDevice(param, &spVolumeDest);

			CONST LONGLONG BUFFER_SIZE = (1LL << 20);
			CVMemPtr<BYTE> buffer;
			buffer.Allocate(BUFFER_SIZE);
			memset(buffer.m_pData, 0xff, BUFFER_SIZE);
			CSimpleBitArray sba(buffer.m_pData, (BUFFER_SIZE << 3));

			LONGLONG FileSize = 0;
			spVolumeSrc->GetFileSize(&FileSize);
			ULONG BlockSize = FileSize / (BUFFER_SIZE << 3);
			

			CComPtr<IFileCopy> spIFileCopy;
			hr = CreateInstanceCMTCopy(
				spVolumeSrc,//IFileDevice * pSource,
				spVolumeDest,//IFileDevice * pDest,
				0,//LONGLONG SourceStartOffset,			,// in case 0 == SourceStartOffset,  0 == Diff, nullptr == pSimpleBitArray, it will determine whether rad/write file_seek be called
				0,//LONGLONG Diff,						,//
				&sba,//CSimpleBitArray * pSimpleBitArray, ,//if nullptr, means reading from beginning until no more data
				BlockSize,//ULONG BlockSize,
				TRUE,//BOOL SeekAlways, ,//<sonmi01>2014-12-12 ###???
				&spIFileCopy,//IFileCopy ** ppObj,
				TRUE//,//BOOL bCreateMemeber
				);
			spIFileCopy->Copy();

			spVolumeDest->Flush();
			

			D_INFO(0, TEXT("End copying volume [%s]"), volInfo.m_name.GetString());
		}

		for (auto & fd : diskFd)
		{
			if (nullptr != fd)
			{
				fd->Release();
				fd = nullptr;
			}
		}
		diskFd.clear();

		//return main_parser(argc, argv);
	}
	break;

	case 1:
	{
		HRESULT hr = S_OK;
		std::vector<IFileDevice*> diskFd;
		for (int ii = 1; ii < argc; ++ii)
		{
			CString name(argv[ii]);
			CComPtr<IFileDevice> spDisk;
			hr = CreateInstanceFileDeviceWin32(
				name.GetString(),//IN LPCTSTR strFileName,
				GENERIC_READ,//IN ULONG ulDesiredAccess,
				FILE_SHARE_READ | FILE_SHARE_WRITE,//IN ULONG ulShareMode,
				OPEN_EXISTING,//IN ULONG ulCreationDisposition,
				0,//IN ULONG ulFlagsAndAttributes,
				&spDisk,//OUT IFileDevice ** ppFileDevice,
				TRUE//,//BOOL bCreateMember
				);
			if (!!spDisk)
			{
				diskFd.push_back(spDisk.Detach());
			}
		}

		SmartLib::DynamicVolumesDisksInfos infos;
		SmartLib::ParseDynamicDisks(
			diskFd,//IN std::vector<IFileDevice*> & disks,
			infos,//OUT SmartLib::DynamicVolumesDisksInfos & infos,
			nullptr//,//LPCTSTR resultFile
			);

		for (CONST auto & volInfo : infos.m_VolumeInfos)
		{
			D_INFO(0, TEXT("Begin pasrse volume [%s]"), volInfo.m_name.GetString());
			SmartLib::CreateIntanceVolumeIoFileDeviceParam param;
			VolumeInfoToCreateIntanceVolumeIoFileDeviceParam(volInfo, param);

			CComPtr<IFileDeviceFlush> spVolume;
			hr = CreateIntanceVolumeIoFileDevice(param, &spVolume);
			NtfsEnumAll(spVolume);
			D_INFO(0, TEXT("End pasrse volume [%s]"), volInfo.m_name.GetString());
		}

		for (auto & fd : diskFd)
		{
			if (nullptr != fd)
			{
				fd->Release();
				fd = nullptr;
			}
		}
		diskFd.clear();

		//return main_parser(argc, argv);
	}
	break;

	case 0:
	{
		return main_parser(argc, argv);
	}
	break;
	}

}

