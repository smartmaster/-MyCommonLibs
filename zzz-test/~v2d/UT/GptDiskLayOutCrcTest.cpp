#include "stdafx.h"
#include "..\CommonFunctions\GPTDiskLayout.h"
#include "..\CommonFunctions\CrcCalc.h"
#include "..\CommonFunctions\MiscHelper.h"
#include "..\CommonFunctions\SingleDiskInfo.h"
#include "..\CommonFunctions\LocalFileDevice.h"


VOID TestGUID()
{
	GUID Guid;
	CString strGuid;
	CMiscHelper::CreateGuid(Guid, strGuid);


	CString strGuid00;
	CMiscHelper::StringFromGuid(strGuid00, Guid);

	GUID Guid00;
	CMiscHelper::GuidFromString(Guid00, strGuid);
	
}

INT CrcTest()
{
	INT nStatus = 0;

	do 
	{
		//******************************************
		CHeapPtr<BYTE> apPartitionTableEntry;
		apPartitionTableEntry.Allocate(32*GPT_LBA_BLOCK_SIZE);

		ZeroMemory((BYTE*)apPartitionTableEntry, 32*GPT_LBA_BLOCK_SIZE);
		CopyMemory((BYTE*)apPartitionTableEntry, GPT_PARTITION_TABLE_ENTRY_SAMPLE, sizeof(GPT_PARTITION_TABLE_ENTRY_SAMPLE));

		CCrcCalc crc;
		crc.InitCrc32Table();
		ULONG ulCrc = crc.GetCrc((BYTE*)apPartitionTableEntry, 32*GPT_LBA_BLOCK_SIZE);

		ULONG ulCrcSample = 0;
		CopyMemory(&ulCrcSample, GPT_HEADER_SAMPLE+0x58, sizeof(ULONG));

		ULONG ulDiff =ulCrc - ulCrcSample;

	} while (FALSE);

	do 
	{
		CHeapPtr<BYTE> apGptHeader;
		apGptHeader.Allocate(GPT_LBA_BLOCK_SIZE);
		CopyMemory((BYTE*)apGptHeader, GPT_HEADER_SAMPLE, GPT_LBA_BLOCK_SIZE);

		ULONG ulCrcSample = 0;
		CopyMemory(&ulCrcSample, GPT_HEADER_SAMPLE+0x10, sizeof(ULONG));

		ULONG crcInitValue = (ULONG)(0);
		CopyMemory((BYTE*)apGptHeader+0x10, &crcInitValue, sizeof(ULONG));
		CCrcCalc crc;
		crc.InitCrc32Table();
		ULONG ulCrc = crc.GetCrc((BYTE*)apGptHeader, 0x5c);

		ULONG ulDiff =ulCrc - ulCrcSample;

	} while (FALSE);

	do 
	{
		CHeapPtr<BYTE> apBuffer;
		apBuffer.Allocate(GPT_LBA_BLOCK_SIZE);
		ZeroMemory((BYTE*)apBuffer, GPT_LBA_BLOCK_SIZE);

		CCrcCalc crc;
		crc.InitCrc32Table();
		ULONG ulCrc = crc.GetCrc((BYTE*)apBuffer, GPT_LBA_BLOCK_SIZE);


	} while (FALSE);


	return nStatus;
}

INT GptDiskLayoutTest(ULONG ulDiskNumber)
{
	INT nStatus = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		if (0 == ulDiskNumber)
		{	
			_OLD_DSETLASTSTATUS(E_INVALIDARG);
			nStatus = E_INVALIDARG;
			_OLD_DAPIERR((0, DRNPOS, TEXT("GptDiskLayoutTest"), E_INVALIDARG, TEXT("[ulDiskNumber=%u]"), ulDiskNumber));
			break;
		}

		//******************************************
		CSingleDiskInfo SingleDiskInfo;
		SingleDiskInfo.Retrieve(ulDiskNumber, FALSE);
		
		LONGLONG llDiskSize = ((*((SingleDiskInfo).m_pDiskGeometryEx)).DiskSize).QuadPart;

		//******************************************
		CLocalFileDevice LocalFileDevice;
		CString strName;
		strName.Format(TEXT("\\\\?\\PhysicalDrive%u"), ulDiskNumber);
		CComBSTR bstrName = strName.GetString();
		LocalFileDevice.Create(
			bstrName,//BSTR bstrFileName, 
			GENERIC_WRITE,//ULONG ulDesiredAccess, 
			FILE_SHARE_READ|FILE_SHARE_WRITE,//ULONG ulShareMode, 
			OPEN_EXISTING,//ULONG ulCreationDisposition, 
			FILE_FLAG_NO_BUFFERING//ULONG ulFlagsAndAttributes
			);

		CGptDiskPartition GptDiskPartition(llDiskSize, &LocalFileDevice);
		GptDiskPartition.Partition(FALSE);

		LocalFileDevice.Close();

		CSingleDiskInfo SingleDiskInfo01;
		SingleDiskInfo01.Retrieve(ulDiskNumber, FALSE);

		//******************************************
	} while (FALSE);

	return nStatus;
}