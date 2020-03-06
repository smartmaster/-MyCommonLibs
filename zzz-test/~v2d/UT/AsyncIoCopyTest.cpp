#include "stdafx.h"
#include "..\CommonFunctions\AsyncIoCopy.h"
#include "..\ServerDiskVolumeInfo\ServerDiskVolumeInfo_i.h"
#include "..\ServerFileDevice\ServerFileDevice_i.h"
#include "..\ServerVssLayer\ServerVssLayer_i.h"
#include "..\CommonFunctions\LocalFileDevice.h"
#include "..\CommonFunctions\SingleVolumeInfo.h"

INT AsyncIoCopyTest(LPCTSTR szSource, LPCTSTR szDest)
{
	INT nStaus= 0;
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************
		CComPtr<IFileDeviceObject> spIFileDevice;
		hr = spIFileDevice.CoCreateInstance(__uuidof(FileDeviceObject));
		if (FAILED(hr))
		{	
			nStaus = -1;
			break;
		}

		CComBSTR bstrSource = szSource;
		hr = spIFileDevice->Create( 
			bstrSource,///* [in] */ BSTR bstrFileName,
			GENERIC_READ,///* [in] */ ULONG ulDesiredAccess,
			0,///* [in] */ ULONG ulShareMode,
			OPEN_EXISTING,///* [in] */ ULONG ulCreationDisposition,
			0///* [in] */ ULONG ulFlagsAndAttributes
			);
		if (FAILED(hr))
		{	
			nStaus = -1;
			break;
		}

		LONGLONG  llFileSize = 0;
		hr = spIFileDevice->GetFileSizeEx( 
			&llFileSize///* [out] */ LONGLONG *pllFileSize
			);
		if (FAILED(hr))
		{	
			nStaus = -1;
			break;
		}

		ULONG ulBlockSize = 1024*1024;
		LONGLONG llBitCount = AtlAlignUp(llFileSize, ulBlockSize)/ulBlockSize;
		LONGLONG llByteCount = AtlAlignUp(llBitCount, 8)/8;
		CHeapPtr<BYTE> apBitmap;
		apBitmap.Allocate(llByteCount); 
		for (LONGLONG ll=0; ll<llByteCount; ++ll)
		{
			((BYTE*)apBitmap)[ll] = (BYTE)(-1);
		}

		CSimpleBitArray sba((BYTE*)apBitmap, llBitCount);
		
		//******************************************
		CLocalFileDevice LocalFileDevice;
		CComBSTR bstrLocalFile(szDest);
		nStaus = LocalFileDevice.Create(
			bstrLocalFile,//BSTR bstrFileName, 
			GENERIC_WRITE,//ULONG ulDesiredAccess, 
			0,//ULONG ulShareMode, 
			CREATE_ALWAYS,//ULONG ulCreationDisposition, 
			0//ULONG ulFlagsAndAttributes
			);
		if (nStaus)
		{
			break;
		}

		//******************************************
		CAsyncIoCopy AsyncIoCopy(	
			spIFileDevice,//IFileDeviceObject * pSource, //external object
			&LocalFileDevice,//ILocalFileDevice * pDest, //external object
			&sba,//CSimpleBitArray * pSimpleBitArray, //external object
			0,//LONGLONG llOffsetDiff,
			ulBlockSize,//ULONG ulBlockSize,
			llFileSize,//LONGLONG liTotalData
			0
			);

		AsyncIoCopy.CreateMember();
		AsyncIoCopy.Copy();


	} while (FALSE);
	
	return nStaus;
}


INT AsyncIoCopyVolumeDiskTest(LPCTSTR szSource, LPCTSTR szDest)
{
	INT nStaus= 0;
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	do 
	{
		CString strSorceRoot;
		CString strSorceGuid;
		CVolumeHelper::GetVolumeRootAndGuidFromPath(szSource, strSorceRoot, strSorceGuid);
		

		//******************************************
		CLocalFileDevice LocalFileDeviceSource;
		CString strLocalFileSource = strSorceGuid;
		strLocalFileSource.TrimRight(TEXT("\\/"));
		CComBSTR bstrLocalFileSource = strLocalFileSource.GetString();
		nStaus = LocalFileDeviceSource.Create(
			bstrLocalFileSource,//BSTR bstrFileName, 
			GENERIC_READ,//ULONG ulDesiredAccess, 
			FILE_SHARE_READ|FILE_SHARE_WRITE,//ULONG ulShareMode, 
			OPEN_EXISTING,//ULONG ulCreationDisposition, 
			0//ULONG ulFlagsAndAttributes
			);
	
		CSingleVolumeInfo SingleVolumeInfo ;
		nStaus = SingleVolumeInfo.Retrieve(strSorceGuid, TRUE, TRUE);
		if (nStaus)
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
		if (0 == _tcsicmp(TEXT("\\\\?\\PhysicalDrive0"), szDest))
		{
			nStaus = E_INVALIDARG;
			break;
		}
		CLocalFileDevice LocalFileDeviceDest;
		CComBSTR bstrLocalFileDest(szDest);
		nStaus = LocalFileDeviceDest.Create(
			bstrLocalFileDest,//BSTR bstrFileName, 
			GENERIC_WRITE,//ULONG ulDesiredAccess, 
			0,//ULONG ulShareMode, 
			OPEN_EXISTING,//ULONG ulCreationDisposition, 
			0//ULONG ulFlagsAndAttributes
			);
		if (nStaus)
		{
			break;
		}

		//******************************************
		ULONG ulBlockSize = SingleVolumeInfo.m_NtfsVolumeDataBuffer.BytesPerCluster;
		LONGLONG llFileSize = SingleVolumeInfo.m_NtfsVolumeDataBuffer.TotalClusters.QuadPart - SingleVolumeInfo.m_NtfsVolumeDataBuffer.FreeClusters.QuadPart;
		llFileSize *= ulBlockSize;

		if (0== ulBlockSize || 0 == llFileSize)
		{
			ulBlockSize = 4096; //FAT32 hardcode here...
			llFileSize = llBitOneCount;
			llFileSize *= ulBlockSize;
		}

		LONGLONG llOffsetDiff = 16*1024*1024;
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


	} while (FALSE);
	
	return nStaus;
}