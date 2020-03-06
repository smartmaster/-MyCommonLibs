#include "stdafx.h"
#include "..\CommonFunctions\VhdHelper.h"
#include "..\CommonFunctions\DiskHelper.h"
#include "..\CommonFunctions\SingleDiskInfo.h"
#include "..\CommonFunctions\LocalFileDevice.h"
#include "..\CommonFunctions\GPTDiskLayout.h"
#include "..\CommonFunctions\SingleVolumeInfo.h"
#include "..\CommonFunctions\SimpleBitArray.h"
#include "..\CommonFunctions\AsyncIoCopy.h"

INT VolumeToDiskCopyTest(LPCTSTR szSource, LPCTSTR szVHDPath, ULONG nSizeGB )
{
	INT nStatus = 0;

	BOOL bAttached = FALSE;
	CVhdHelper vhdHelper;

	do 
	{
		//********************************************************************
		ULONGLONG ullDiskSize = ((ULONGLONG)(nSizeGB))*1024*1024*1024;
		nStatus = vhdHelper.Create(
			szVHDPath,//LPCTSTR szPath, 
			VIRTUAL_DISK_ACCESS_ALL,//VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask, 
			ullDiskSize,//ULONGLONG MaximumSize, 
			NULL//LPCTSTR ParentPath
			);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("CVirtualDiskHelper::Create"), E_FAIL));
			break;
		}

		//********************************************************************
		ATTACH_VIRTUAL_DISK_FLAG AttachFlags = ATTACH_VIRTUAL_DISK_FLAG_NO_DRIVE_LETTER | ATTACH_VIRTUAL_DISK_FLAG_PERMANENT_LIFETIME;
		nStatus = vhdHelper.Attach(AttachFlags);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("CVirtualDiskHelper::Attach"), E_FAIL));
			break;
		}
		bAttached = TRUE;

		//********************************************************************
		DWORD dwDiskNumber = vhdHelper.FindUninitalizedVHD();
		if ((DWORD)(-1) == dwDiskNumber || 0 == dwDiskNumber)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("FindUninitalizedVHD"), E_FAIL));
			nStatus = E_FAIL;
			break;
		}
		_ftprintf_s(stdout, TEXT("*** ") TEXT("[VHDPath, SizeGB, DiskNumber]=[%s, %u, %u]") TEXT(" ***\r\n"), szVHDPath, nSizeGB, dwDiskNumber);

		//********************************************************************
		CSingleDiskInfo SingleDiskInfo;
		SingleDiskInfo.Retrieve(dwDiskNumber, FALSE);
		LONGLONG llDiskSize = ((*((SingleDiskInfo).m_pDiskGeometryEx)).DiskSize).QuadPart;

		//******************************************
		CLocalFileDevice LocalFileDeviceDest;
		CString strName;
		strName.Format(TEXT("\\\\?\\PhysicalDrive%u"), dwDiskNumber);
		CComBSTR bstrName = strName.GetString();
		LocalFileDeviceDest.Create(
			bstrName,//BSTR bstrFileName, 
			GENERIC_WRITE,//ULONG ulDesiredAccess, 
			FILE_SHARE_READ|FILE_SHARE_WRITE,//ULONG ulShareMode, 
			OPEN_EXISTING,//ULONG ulCreationDisposition, 
			FILE_FLAG_NO_BUFFERING//ULONG ulFlagsAndAttributes
			);

		CGptDiskPartition GptDiskPartition(llDiskSize, &LocalFileDeviceDest);
		GptDiskPartition.Partition(FALSE);

		GUID UniquePartitionGUID;
		GUID DiskGUID;
		LONGLONG llDiskStartLba;
		LONGLONG llDiskEndLba;
		LONGLONG llPartitionStartLba;
		LONGLONG llPartitionEndLba;
		GptDiskPartition.GetPartitionInfo(
			UniquePartitionGUID,
			DiskGUID,
			llDiskStartLba,
			llDiskEndLba,
			llPartitionStartLba,
			llPartitionEndLba
			);

		//******************************************
		CString strSorceRoot;
		CString strSorceGuid;
		CVolumeHelper::GetVolumeRootAndGuidFromPath(szSource, strSorceRoot, strSorceGuid);

		//******************************************
		CLocalFileDevice LocalFileDeviceSource;
		CString strLocalFileSource = strSorceGuid;
		strLocalFileSource.TrimRight(TEXT("\\/"));
		CComBSTR bstrLocalFileSource = strLocalFileSource.GetString();
		nStatus = LocalFileDeviceSource.Create(
			bstrLocalFileSource,//BSTR bstrFileName, 
			GENERIC_READ,//ULONG ulDesiredAccess, 
			FILE_SHARE_READ|FILE_SHARE_WRITE,//ULONG ulShareMode, 
			OPEN_EXISTING,//ULONG ulCreationDisposition, 
			0//ULONG ulFlagsAndAttributes
			);

		//******************************************
		CSingleVolumeInfo SingleVolumeInfo ;
		nStatus = SingleVolumeInfo.Retrieve(strSorceGuid, TRUE, TRUE);
		if (nStatus)
		{
			break;
		}
		CSimpleBitArray sba(SingleVolumeInfo.m_pVolumeBitmapBuffer->Buffer, SingleVolumeInfo.m_pVolumeBitmapBuffer->BitmapSize.QuadPart);

		LONGLONG llBitOneCount = 0;
		for (LONGLONG iii=0; iii<sba.GetBitLength(); ++iii)
		{
			llBitOneCount += sba.Get(iii);
		}

		//******************************************
		ULONG ulBlockSize = SingleVolumeInfo.m_NtfsVolumeDataBuffer.BytesPerCluster;
		LONGLONG llFileSize = SingleVolumeInfo.m_NtfsVolumeDataBuffer.TotalClusters.QuadPart - SingleVolumeInfo.m_NtfsVolumeDataBuffer.FreeClusters.QuadPart;
		llFileSize *= ulBlockSize;

		if (0== ulBlockSize || 0 == llFileSize)
		{
			ulBlockSize = 4096; //FAT32 hard coded here...
			llFileSize = llBitOneCount;
			llFileSize *= ulBlockSize;
		}

		//******************************************
		LONGLONG llOffsetDiff = llPartitionStartLba;
		llOffsetDiff *= GPT_LBA_BLOCK_SIZE;
		CAsyncIoCopy AsyncIoCopy(	
			&LocalFileDeviceSource,//IFileDeviceObject * pSource, //external object
			&LocalFileDeviceDest,//ILocalFileDevice * pDest, //external object
			&sba,//CSimpleBitArray * pSimpleBitArray, //external object
			llOffsetDiff,//LONGLONG llOffsetDiff,
			ulBlockSize,//ULONG ulBlockSize,
			llFileSize,//LONGLONG liTotalData
			0
			);

		AsyncIoCopy.CreateMember();
		AsyncIoCopy.Copy();

		//******************************************
		LocalFileDeviceDest.Close();
		LocalFileDeviceSource.Close();

		//******************************************
	} while (FALSE);


	//******************************************
	if (bAttached)
	{
		vhdHelper.Dettach();
		bAttached = FALSE;
	}

	return nStatus;
}