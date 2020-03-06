// DiskVolume4KBTo512B-UT.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Windows.h>
#include <atlstr.h>

#include "DTraceLib.h"
#include "DiskVolumeStructure.h"
#include "FileDeviceInterface.h"
#include "VMemPtr.h"
#include "CrcCalc.h"
#include "AdjustBasicDisk4KBTo512B.h"

namespace { //anonymous namespace start


static HRESULT ReadAtOffsetHelper(IFileDevice * pDiskRW, LONGLONG Offset, ULONG nNumberOfBytesToRead, BYTE* pBuffer )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	LONGLONG NewFilePointer = -1;
	hr = pDiskRW->SetPointerEx(Offset, &NewFilePointer, FILE_BEGIN);
	if (FAILED(hr))
	{
		DSETLASTSTATUS(hr, -1);
		DAPIERR(0, TEXT("SetPointerEx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	ULONG NumberOfBytesRead = 0;
	hr = pDiskRW->Read(nNumberOfBytesToRead, &NumberOfBytesRead, pBuffer);
	if (FAILED(hr))
	{
		DSETLASTSTATUS(hr, -1);
		DAPIERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);

	return hr;
}

static HRESULT WriteAtOffsetHelper(IFileDevice * pDiskRW, LONGLONG Offset, ULONG nNumberOfBytesToWrite, CONST BYTE* pBuffer)
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);

	LONGLONG NewFilePointer = -1;
	hr = pDiskRW->SetPointerEx(Offset, &NewFilePointer, FILE_BEGIN);
	if (FAILED(hr))
	{
		DSETLASTSTATUS(hr, -1);
		DAPIERR(0, TEXT("SetPointerEx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	ULONG NumberOfBytesWritten = 0;
	hr = pDiskRW->Write(nNumberOfBytesToWrite, &NumberOfBytesWritten, pBuffer);
	if (FAILED(hr))
	{
		DSETLASTSTATUS(hr, -1);
		DAPIERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}


static void ReplaceInvalidFileNameChar(CString & str)
{
	static CONST TCHAR szInvalid[] = TEXT("/\\?:.");
	for (INT ii = 0; ii < _countof(szInvalid) - 1; ++ ii)
	{
		str.Replace(szInvalid[ii], TEXT('_'));
	}
}

int _tmain_test_boot_sector(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	CString strStorage = argv[1];
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spStorage = NULL;
	hr = CreateInstanceFileDeviceWin32(
		strStorage.GetString(),//IN LPCTSTR strFileName, 
		GENERIC_READ | GENERIC_WRITE,//IN ULONG ulDesiredAccess, 
		FILE_SHARE_READ | FILE_SHARE_WRITE,//IN ULONG ulShareMode, 
		OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spStorage,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CVMemPtr<BYTE> apBuffer;
	CONST LONG BUFF_SIZE = 64 * 1024;
	apBuffer.Allocate(BUFF_SIZE);

	hr = ReadAtOffsetHelper(spStorage, 0, BUFF_SIZE, apBuffer.m_pData);
	if (FAILED(hr))
	{
		DAPIERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	//<sonmi01>2012-11-1 #test volume boot sector
	TNTFS_BOOT_SECTOR * pBootSector = (TNTFS_BOOT_SECTOR *)(apBuffer.m_pData);

	TFAT32_BOOT_SECTOR * pFatBootSector = (TFAT32_BOOT_SECTOR *)(apBuffer.m_pData);

	//////////////////////////////////////////////////////////////////////////
	INT SECTOR_SIZE = 4096;
	TMBR * pMBR = (TMBR *)(apBuffer.m_pData);
	TLegacyMBR * pPotectivMBR = (TLegacyMBR *)(apBuffer.m_pData);

	TGPTHeader * pGPTHeader = (TGPTHeader *)(apBuffer.m_pData + SECTOR_SIZE);
	TGPT_PTE * pGPTTable = (TGPT_PTE *)(apBuffer.m_pData + SECTOR_SIZE + SECTOR_SIZE);

	//////////////////////////////////////////////////////////////////////////
	//ULONG CRC32Checksum = (*pGPTHeader).CRC32Checksum;
	//(*pGPTHeader).CRC32Checksum = 0;

	//ULONG PartitionEntryArrayCRC32 = (*pGPTHeader).PartitionEntryArrayCRC32;
	////(*pGPTHeader).PartitionEntryArrayCRC32 = 0;

	//CCrcCalc crcCal;
	//crcCal.InitCrc32Table();
	//ULONG CRC32ChecksumTemp = crcCal.GetCrc((CONST BYTE*)pGPTHeader, pGPTHeader->HeaderSize);
	//INT Diff = CRC32ChecksumTemp - CRC32Checksum;

	//crcCal.InitCrc32Table();
	//ULONG PartitionEntryArrayCRC32Temp = crcCal.GetCrc((CONST BYTE*)pGPTTable, pGPTHeader->SizeofPartitionEntry * pGPTHeader->SizeofPartitionEntry);
	//Diff = PartitionEntryArrayCRC32Temp - PartitionEntryArrayCRC32;

	CVMemPtr<BYTE> apBuffer512B;
	apBuffer512B.Allocate(BUFF_SIZE);
	//if (1)
	//{
	//	AdjustNTFS4KBTo512B(
	//		apBuffer.m_pData,//CONST BYTE * p4KBBuffer, 
	//		apBuffer512B.m_pData,//BYTE * p512BBuffer, 
	//		BUFF_SIZE//INT BufferSize
	//		);
	//}
	//else if (0)
	//{
	//	AdjustBasicMBR4KBTo512B(
	//		apBuffer.m_pData,//CONST BYTE * p4KBBuffer, 
	//		apBuffer512B.m_pData,//BYTE * p512BBuffer, 
	//		BUFF_SIZE//INT BufferSize
	//		);
	//}
	//else
	//{
	//	AdjustBasicGPT4KBTo512B(
	//		apBuffer.m_pData,//CONST BYTE * p4KBBuffer, 
	//		apBuffer512B.m_pData,//BYTE * p512BBuffer, 
	//		BUFF_SIZE//INT BufferSize
	//		);
	//}
	//////////////////////////////////////////////////////////////////////////

	ATLASSERT(0);
	//////////////////////////////////////////////////////////////////////////
	hr = WriteAtOffsetHelper(spStorage, 0, BUFF_SIZE, apBuffer512B.m_pData);
	if (FAILED(hr))
	{
		DAPIERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	//<sonmi01>2012-11-1 ###???
	CComPtr<IFileDevice> spIFileDeviceSave;
	CString strFile = strStorage;
	ReplaceInvalidFileNameChar(strFile);
	strFile += TEXT(".storbin");
	hr = CreateInstanceFileDeviceWin32(
		strFile.GetString(),//IN LPCTSTR strFileName, 
		GENERIC_WRITE,//IN ULONG ulDesiredAccess, 
		0,//IN ULONG ulShareMode, 
		CREATE_ALWAYS,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDeviceSave,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = WriteAtOffsetHelper(spIFileDeviceSave, 0, BUFF_SIZE, apBuffer.m_pData);
	if (FAILED(hr))
	{
		DAPIERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}

int tmain_test_basic_disk(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	CString strStorage = argv[1];
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spStorage = NULL;
	hr = CreateInstanceFileDeviceWin32(
		strStorage.GetString(),//IN LPCTSTR strFileName, 
		GENERIC_READ | GENERIC_WRITE,//IN ULONG ulDesiredAccess, 
		FILE_SHARE_READ | FILE_SHARE_WRITE,//IN ULONG ulShareMode, 
		OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spStorage,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	hr = Adjust4KBSectorBasicDisk(spStorage);

	END_BLOCK(0);
	return 0;
}

} //anonymous namespace end

int _tmain(int argc, _TCHAR* argv[])
{
	DSETTINGS(99, FALSE);
	return tmain_test_basic_disk(argc, argv);
	return _tmain_test_boot_sector(argc, argv);
	return 0;
}

