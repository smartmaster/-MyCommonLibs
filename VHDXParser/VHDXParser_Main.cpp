// VHDXParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <intsafe.h>

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicVMemPtr.h"


#include "VhdxFileIdentifier.h"
#include "VhdxHeader.h"
#include "VHDXRegionTableHeader.h"
#include "VHDXRegionTableEntry.h"
#include "VHDXMetadataTableHeader.h"
#include "VHDXMetadataTableEntry.h"
#include "VhdxHelper.h"

#include "ParserSeparateVHDX.h"
#include "FileDeviceForVhdxCacheIO.h"
#include "CacheIO.h"
#include "ParserChainVHDX.h"

#include "SeparateVhdxInterface.h"
#include "DiskImageInterface.h"
#include "..\PublicHeader\PublicMTCopy.h"
#include "..\PublicHeader\PublicGetTestCaseNumber.h"

#include "VHDXLogReplay.h"
#include "SparseBlockMemory.h"

#include "Apply_VMIOF_RT_CBT.h"


int ParseMetadataEntry(LPBYTE pMetadataStart,  VHDXMetadata & Metadata, VHDXMetadataTableEntry * pVHDXMetadataTableEntry, ULONG EntryCount, map<CString, CString> & ParentPath)
{
	for (ULONG ii = 0; ii < EntryCount; ++ ii)
	{
		pVHDXMetadataTableEntry[ii].Print();
		pVHDXMetadataTableEntry[ii].ParseMetadata(pMetadataStart, Metadata, ParentPath);
	}

	return 0;
}

int ParseMetadata(IFileDevice * pFileDevice, LONGLONG MetadataOffset)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BOOL bRet = FALSE;

	CVMemPtr<BYTE> apMetadataBuffer;
	apMetadataBuffer.Allocate(VHDXMetadataTableHeader::SizeSpecConst());

	ULONG NumberOfBytesRead = 0;
	hr = ReadAtOffsetHelper(pFileDevice, MetadataOffset, VHDXMetadataTableHeader::SizeSpecConst(), &NumberOfBytesRead, apMetadataBuffer.m_pData);
	if (SUCCEEDED(hr))
	{
		VHDXMetadataTableHeader * pVHDXMetadataTableHeader = (VHDXMetadataTableHeader *)(apMetadataBuffer.m_pData);
		pVHDXMetadataTableHeader->Print();
		bRet = pVHDXMetadataTableHeader->IsValid();

		VHDXMetadata Metadata;
		map<CString, CString> ParentPath;
		VHDXMetadataTableEntry * pVHDXMetadataTableEntry = (VHDXMetadataTableEntry *)((BYTE*)(pVHDXMetadataTableHeader) + sizeof(VHDXMetadataTableHeader));
		ParseMetadataEntry(apMetadataBuffer.m_pData, Metadata, pVHDXMetadataTableEntry, pVHDXMetadataTableHeader->m_EntryCount, ParentPath);
	}

	return hr;
}



int _tmain_test_header(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LPCTSTR strFileName = argv[1];
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(
		strFileName,//IN LPCTSTR strFileName, 
		GENERIC_READ,//IN ULONG ulDesiredAccess, 
		FILE_SHARE_READ,//IN ULONG ulShareMode, 
		OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDevice,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CONST LONG HEADER_SIZE = 256 * 1024;
	CVMemPtr<BYTE> apBuffer;
	apBuffer.Allocate(HEADER_SIZE);

	ULONG NumberOfBytesRead = 0;
	hr = ReadAtOffsetHelper(spIFileDevice, 
		0, 
		HEADER_SIZE,//ULONG nNumberOfBytesToRead, 
		&NumberOfBytesRead,//ULONG* lpNumberOfBytesRead, 
		apBuffer.m_pData//BYTE* pBuffer
		);
	if (FAILED(hr))
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	VhdxFileIdentifier * pVhdxFileIdentifier = (VhdxFileIdentifier*)(apBuffer.m_pData + VhdxFileIdentifier::OffsetSpecConst());
	pVhdxFileIdentifier->Print();
	BOOL bRet = pVhdxFileIdentifier->IsValid();
	CString strCreator;
	pVhdxFileIdentifier->ReadCreator(strCreator);

	//////////////////////////////////////////////////////////////////////////
	VhdxHeader * pVhdxHeader1 = (VhdxHeader*)(apBuffer.m_pData + VhdxHeader::Headser1OffsetSpecConst());
	pVhdxHeader1->Print();
	bRet = pVhdxHeader1->IsValid();
	pVhdxHeader1->CalcChecksum();

	VhdxHeader * pVhdxHeader2 = (VhdxHeader*)(apBuffer.m_pData + VhdxHeader::Headser2OffsetSpecConst());
	pVhdxHeader2->Print();
	bRet = pVhdxHeader2->IsValid();
	pVhdxHeader2->CalcChecksum();

	//////////////////////////////////////////////////////////////////////////
	VHDXRegionTableHeader * pVHDXRegionTableHeader = (VHDXRegionTableHeader*)(apBuffer.m_pData + VHDXRegionTableHeader::OffsetSpecConst());
	pVHDXRegionTableHeader->Print();
	bRet = pVHDXRegionTableHeader->IsValid();

	VHDXRegionTableEntry * pRegionEntry = (VHDXRegionTableEntry*)((BYTE*)(pVHDXRegionTableHeader) + sizeof(VHDXRegionTableHeader));
	for (UINT32 ii = 0; ii < pVHDXRegionTableHeader->m_EntryCount; ++ ii)
	{
		pRegionEntry[ii].Print();

		if (pRegionEntry[ii].IsBatEntry())
		{
			D_INFO(0, TEXT("BatEntry. offset=%I64d, m_Required=%s"), pRegionEntry[ii].m_FileOffset, (pRegionEntry[ii].m_Required? TEXT("TRUE") : TEXT("FALSE")));
		}
		else if (pRegionEntry[ii].IsMetadataRegionEntry())
		{
			D_INFO(0, TEXT("MetadataRegionEntry. offset=%I64d, m_Required=%s"), pRegionEntry[ii].m_FileOffset, (pRegionEntry[ii].m_Required? TEXT("TRUE") : TEXT("FALSE")));
			ParseMetadata(spIFileDevice, pRegionEntry[ii].m_FileOffset); 
		}
		else
		{
			D_INFO(0, TEXT("UnknownRegionEntry. offset=%I64d, m_Required=%s"), pRegionEntry[ii].m_FileOffset, (pRegionEntry[ii].m_Required? TEXT("TRUE") : TEXT("FALSE")));
		}
	}

	
	END_BLOCK(0);
	
	return hr;
}


int _tmain_test_header_product_code(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LPCTSTR strFileName = argv[1];
	LPCTSTR strDestDrive = argv[2];

	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(
		strFileName,//IN LPCTSTR strFileName, 
		GENERIC_READ,//IN ULONG ulDesiredAccess, 
		FILE_SHARE_READ,//IN ULONG ulShareMode, 
		OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDevice,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		LEAVE_BLOCK(0);
	}

	CAutoPtr<ParserSeparateVHDX> apParserVHDX(new ParserSeparateVHDX(spIFileDevice.Detach()));
	hr = apParserVHDX->Parse();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Parse"), hr, TEXT("Invalid VHDX"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spIFileDeviceDest;
	hr = CreateInstanceFileDeviceWin32(
		strDestDrive,//IN LPCTSTR strFileName, 
		GENERIC_WRITE,//IN ULONG ulDesiredAccess, 
		0,//IN ULONG ulShareMode, 
		OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDeviceDest,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);

	CVMemPtr<BYTE> apBlockData;
	apBlockData.Allocate(apParserVHDX->m_FileParameters.m_BlockSize);
	BOOL bPartiallyPresent = FALSE;
	BOOL bAllZero = FALSE;

	CVMemPtr<BYTE> apBitmapBuffer;
	apBitmapBuffer.Allocate(apParserVHDX->m_BlockBitmapBufferSize);

	CFileDeviceForVhdxCacheIO FileDeviceForCacheIO(apParserVHDX);
	CCacheIO vhdxCacheIo(&FileDeviceForCacheIO, FALSE, apParserVHDX->m_FileParameters.m_BlockSize);

	CONST LONG SMALL_BLOCK_SZIE = 256 * 1024;
	LONG SmallLoopCount = apParserVHDX->m_FileParameters.m_BlockSize / SMALL_BLOCK_SZIE;

	for (LONG ii = 0; ii < apParserVHDX->m_DataBlocksCount; ++ ii)
	{
		LONG BmpLen = apParserVHDX->m_BlockBitmapBufferSize;
		apParserVHDX->ReadBlocKBitmap(ii, BmpLen, apBitmapBuffer.m_pData);

		//VHDX_BLOCK_STATE BlockState;
		//hr = apParserVHDX->ReadBlockState(ii, BlockState);
		bAllZero = IsAllZero(apBitmapBuffer.m_pData, BmpLen);
		if (!bAllZero)
		{
			LONGLONG Offset = ii;
			Offset *= apParserVHDX->m_FileParameters.m_BlockSize;

			for (LONG jj = 0; jj < SmallLoopCount; ++ jj)
			{
				hr = vhdxCacheIo.ReadAtOffset(Offset + jj * SMALL_BLOCK_SZIE, SMALL_BLOCK_SZIE, apBlockData.m_pData + jj * SMALL_BLOCK_SZIE);
			}

			
			ULONG NumberOfBytesWritten = 0;
			hr = WriteAtOffsetHelper(spIFileDeviceDest, Offset, apParserVHDX->m_FileParameters.m_BlockSize, &NumberOfBytesWritten, apBlockData.m_pData);
		}
		
	}

	END_BLOCK(0);

	return hr;
}

VOID Clear(vector<IFileDevice*> & vecIFileDevice)
{
	for (size_t ii = 0; ii < vecIFileDevice.size(); ++ ii)
	{
		if (vecIFileDevice[ii])
		{
			vecIFileDevice[ii]->Release();
			vecIFileDevice[ii] = NULL;
		}
	}
	vecIFileDevice.clear();
}

int _tmain_test_vhdx_chain_simple(vector<CString> & vhdxFiles, LPCTSTR strDestDrive)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	//vector<IFileDevice*> vecIFileDevice;
	ParserChainVHDXBase vhdxChain;

	for (size_t ii = 0; ii < vhdxFiles.size(); ++ ii)
	{
		//////////////////////////////////////////////////////////////////////////
		CComPtr<IFileDevice> spIFileDevice;
		hr = CreateInstanceFileDeviceWin32(
			vhdxFiles[ii].GetString(),//IN LPCTSTR strFileName, 
			GENERIC_READ,//IN ULONG ulDesiredAccess, 
			FILE_SHARE_READ,//IN ULONG ulShareMode, 
			OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
			0,//IN ULONG ulFlagsAndAttributes, 
			&spIFileDevice,//OUT IFileDevice ** ppFileDevice, 
			TRUE//BOOL bCreateMember
			);
		if (FAILED(hr))
		{
			LEAVE_BLOCK(0);
		}

		//vecIFileDevice.push_back(spIFileDevice.Detach());


		//////////////////////////////////////////////////////////////////////////
		CAutoPtr<ParserSeparateVHDX> apParserVHDX(new ParserSeparateVHDX(spIFileDevice.Detach()));
		hr = apParserVHDX->Parse();
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("Parse"), hr, TEXT("Invalid VHDX"));
			LEAVE_BLOCK(0);
		}

		vhdxChain.m_Chain.push_back(apParserVHDX.Detach());

	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	vhdxChain.FindMaxBlockSizeAndAllocateBuffer();

	

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spIFileDeviceDest;
	hr = CreateInstanceFileDeviceWin32(
		strDestDrive,//IN LPCTSTR strFileName, 
		GENERIC_WRITE,//IN ULONG ulDesiredAccess, 
		0,//IN ULONG ulShareMode, 
		OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDeviceDest,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);

	CVMemPtr<BYTE> apBitmapBuffer;
	apBitmapBuffer.Allocate(vhdxChain.m_MaxBitmapSize);


	CVMemPtr<BYTE> apBlockData;
	apBlockData.Allocate(vhdxChain.m_MaxBlockSize);


	for (LONG ii = 0; ii < vhdxChain.m_Chain[vhdxChain.m_MaxBlockSizeVhdxIndex]->m_DataBlocksCount; ++ ii)
	{
		LONG BmpLen = vhdxChain.m_MaxBitmapSize;
		BOOL bContainOne = FALSE;
		BOOL bAllOne = FALSE;
		hr = vhdxChain.ReadBlocKBitmap(ii, BmpLen, apBitmapBuffer.m_pData, bContainOne, bAllOne); //<sonmi01>2013-10-29 ###???

		if (bContainOne)
		{
			LONGLONG offset = ii;
			offset *= vhdxChain.m_MaxBlockSize;

			hr = vhdxChain.ReadBlocK(TRUE, ii, apBlockData.m_pData, vhdxChain.m_MaxBlockSize);

			ULONG NumberOfBytesWritten = 0;
			hr = WriteAtOffsetHelper(spIFileDeviceDest, offset, vhdxChain.m_MaxBlockSize, &NumberOfBytesWritten, apBlockData.m_pData);
		}

	}

	END_BLOCK(0);

	return hr;
}



int _tmain_test_header_cache_read(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LPCTSTR strFileName = argv[1];
	LPCTSTR strDestDrive = argv[2];

	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(
		strFileName,//IN LPCTSTR strFileName, 
		GENERIC_READ,//IN ULONG ulDesiredAccess, 
		FILE_SHARE_READ,//IN ULONG ulShareMode, 
		OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDevice,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		LEAVE_BLOCK(0);
	}

	CAutoPtr<ParserSeparateVHDX> apParserVHDX(new ParserSeparateVHDX(spIFileDevice.Detach()));
	hr = apParserVHDX->Parse();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Parse"), hr, TEXT("Invalid VHDX"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spIFileDeviceDest;
	hr = CreateInstanceFileDeviceWin32(
		strDestDrive,//IN LPCTSTR strFileName, 
		GENERIC_WRITE,//IN ULONG ulDesiredAccess, 
		0,//IN ULONG ulShareMode, 
		OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDeviceDest,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);


	CFileDeviceForVhdxCacheIO FileDeviceForCacheIO(apParserVHDX);
	CCacheIO cacheIO(&FileDeviceForCacheIO, FALSE, apParserVHDX->m_FileParameters.m_BlockSize);

	//static CONST LONG LOOP_COUNT = 3;
	//for (LONGLONG ii = 0; ii < apParserVHDXMetadata->m_DataBlocksCount; ++ ii)
	//{
	//	for (int jj = 0; jj < LOOP_COUNT; ++ jj)
	//	{
	//		cacheIO.CacheReadBlocks(ii * apParserVHDXMetadata->m_VHDX_FILE_PARAMETERS.m_BlockSize, 1);
	//	}
	//}

	//cacheIO.CacheReadBlocks(0 * apParserVHDXMetadata->m_VHDX_FILE_PARAMETERS.m_BlockSize, 10);
	//cacheIO.CacheReadBlocks(4 * apParserVHDXMetadata->m_VHDX_FILE_PARAMETERS.m_BlockSize, 5);
	//cacheIO.CacheReadBlocks(5 * apParserVHDXMetadata->m_VHDX_FILE_PARAMETERS.m_BlockSize, 10);
	//cacheIO.CacheReadBlocks(0 * apParserVHDXMetadata->m_VHDX_FILE_PARAMETERS.m_BlockSize, 5);
	//cacheIO.CacheReadBlocks(0 * apParserVHDXMetadata->m_VHDX_FILE_PARAMETERS.m_BlockSize, 20);

	CVMemPtr<BYTE> apDataBuffer;
	LONG BlockSize = apParserVHDX->m_FileParameters.m_BlockSize;
	apDataBuffer.Allocate(BlockSize * 4);

	cacheIO.ReadAtOffset(100, 10, apDataBuffer.m_pData);
	cacheIO.ReadAtOffset(0, BlockSize, apDataBuffer.m_pData);
	cacheIO.ReadAtOffset(BlockSize - 1, 2, apDataBuffer.m_pData);
	cacheIO.ReadAtOffset(0, BlockSize * 2, apDataBuffer.m_pData);
	cacheIO.ReadAtOffset(0, BlockSize * 3, apDataBuffer.m_pData);
	cacheIO.ReadAtOffset(10, BlockSize, apDataBuffer.m_pData);
	cacheIO.ReadAtOffset(10, BlockSize * 2, apDataBuffer.m_pData);
	cacheIO.ReadAtOffset(10, BlockSize * 3, apDataBuffer.m_pData);

	END_BLOCK(0);

	return hr;
}

int _tmain_test_header_cache_write(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LPCTSTR strFileName = argv[1];
	//LPCTSTR strDestDrive = argv[2];

	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(
		strFileName,//IN LPCTSTR strFileName, 
		GENERIC_ALL,//IN ULONG ulDesiredAccess, 
		0,//IN ULONG ulShareMode, 
		CREATE_ALWAYS,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDevice,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		LEAVE_BLOCK(0);
	}

	LONG BLOCKSIZE = 4 * 1024;
	CCacheIO cacheIO(spIFileDevice, FALSE, BLOCKSIZE, TRUE);

	BYTE Data[] = "0123456789abcdef";

	for (LONG ii = 0; ii < 4*1024; ++ ii)
	{
		cacheIO.Write(sizeof(Data) - 1, Data);
	}
	cacheIO.Flush();

	cacheIO.Seek(0);
	BYTE DataRead[16] = {0};
	for (LONG ii = 0; ii < 4*1024; ++ ii)
	{
		cacheIO.Read(sizeof(DataRead), DataRead);
		ATLASSERT(0 == memcmp(Data, DataRead, sizeof(DataRead)));
	}


	//static CONST LONG LOOP_COUNT = 3;
	//for (LONGLONG ii = 0; ii < apParserVHDXMetadata->m_DataBlocksCount; ++ ii)
	//{
	//	for (int jj = 0; jj < LOOP_COUNT; ++ jj)
	//	{
	//		cacheIO.CacheReadBlocks(ii * apParserVHDXMetadata->m_VHDX_FILE_PARAMETERS.m_BlockSize, 1);
	//	}
	//}

	//cacheIO.CacheReadBlocks(0 * apParserVHDXMetadata->m_VHDX_FILE_PARAMETERS.m_BlockSize, 10);
	//cacheIO.CacheReadBlocks(4 * apParserVHDXMetadata->m_VHDX_FILE_PARAMETERS.m_BlockSize, 5);
	//cacheIO.CacheReadBlocks(5 * apParserVHDXMetadata->m_VHDX_FILE_PARAMETERS.m_BlockSize, 10);
	//cacheIO.CacheReadBlocks(0 * apParserVHDXMetadata->m_VHDX_FILE_PARAMETERS.m_BlockSize, 5);
	//cacheIO.CacheReadBlocks(0 * apParserVHDXMetadata->m_VHDX_FILE_PARAMETERS.m_BlockSize, 20);

	//CVMemPtr<BYTE> apDataBuffer;
	//apDataBuffer.Allocate(BLOCKSIZE * 50);
	//memset(apDataBuffer.m_pData, 's', BLOCKSIZE * 50);

	//cacheIO.WriteAtOffset(100, 10, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(0, BLOCKSIZE, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(BLOCKSIZE - 1, 2, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(0, BLOCKSIZE * 2, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(0, BLOCKSIZE * 3, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(10, BLOCKSIZE, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(10, BLOCKSIZE * 2, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(10, BLOCKSIZE * 3, apDataBuffer.m_pData);

	//memset(apDataBuffer.m_pData, 'M', BLOCKSIZE * 50);

	//cacheIO.WriteAtOffset(100, 10, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(0, BLOCKSIZE, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(BLOCKSIZE - 1, 2, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(0, BLOCKSIZE * 2, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(0, BLOCKSIZE * 3, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(10, BLOCKSIZE, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(10, BLOCKSIZE * 2, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(10, BLOCKSIZE * 3, apDataBuffer.m_pData);


	//cacheIO.WriteAtOffset(20 * BLOCKSIZE + 1, BLOCKSIZE * 50, apDataBuffer.m_pData);

	//memset(apDataBuffer.m_pData, 'A', BLOCKSIZE * 50);
	//cacheIO.WriteAtOffset(2, 3, apDataBuffer.m_pData);

	END_BLOCK(0);

	return hr;
}


int _tmain_test_bulk_write(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LPCTSTR strFileName = argv[1];
	//LPCTSTR strDestDrive = argv[2];

	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(
		strFileName,//IN LPCTSTR strFileName, 
		GENERIC_ALL,//IN ULONG ulDesiredAccess, 
		0,//IN ULONG ulShareMode, 
		CREATE_ALWAYS,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDevice,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		LEAVE_BLOCK(0);
	}

	LONG BLOCKSIZE = 4;
	CBulkWrite bw(spIFileDevice, FALSE, BLOCKSIZE);

	BYTE Data[] = "0123456789abcdefx";
	CONST LONG data_size = sizeof(Data) - 1;

	LONGLONG offset = 0;
	for (LONG ii = 0; ii < 33; ++ii)
	{
		bw.WriteAtOffset(offset, data_size, Data);
		offset += data_size;
	}
	//bw.Flush();


	offset += 40;
	bw.Seek(offset);
	for (LONG ii = 0; ii < 33; ++ii)
	{
		bw.Write(data_size, Data);
		offset += data_size;
	}
	bw.Flush();
	offset;


	//static CONST LONG LOOP_COUNT = 3;
	//for (LONGLONG ii = 0; ii < apParserVHDXMetadata->m_DataBlocksCount; ++ ii)
	//{
	//	for (int jj = 0; jj < LOOP_COUNT; ++ jj)
	//	{
	//		cacheIO.CacheReadBlocks(ii * apParserVHDXMetadata->m_VHDX_FILE_PARAMETERS.m_BlockSize, 1);
	//	}
	//}

	//cacheIO.CacheReadBlocks(0 * apParserVHDXMetadata->m_VHDX_FILE_PARAMETERS.m_BlockSize, 10);
	//cacheIO.CacheReadBlocks(4 * apParserVHDXMetadata->m_VHDX_FILE_PARAMETERS.m_BlockSize, 5);
	//cacheIO.CacheReadBlocks(5 * apParserVHDXMetadata->m_VHDX_FILE_PARAMETERS.m_BlockSize, 10);
	//cacheIO.CacheReadBlocks(0 * apParserVHDXMetadata->m_VHDX_FILE_PARAMETERS.m_BlockSize, 5);
	//cacheIO.CacheReadBlocks(0 * apParserVHDXMetadata->m_VHDX_FILE_PARAMETERS.m_BlockSize, 20);

	//CVMemPtr<BYTE> apDataBuffer;
	//apDataBuffer.Allocate(BLOCKSIZE * 50);
	//memset(apDataBuffer.m_pData, 's', BLOCKSIZE * 50);

	//cacheIO.WriteAtOffset(100, 10, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(0, BLOCKSIZE, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(BLOCKSIZE - 1, 2, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(0, BLOCKSIZE * 2, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(0, BLOCKSIZE * 3, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(10, BLOCKSIZE, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(10, BLOCKSIZE * 2, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(10, BLOCKSIZE * 3, apDataBuffer.m_pData);

	//memset(apDataBuffer.m_pData, 'M', BLOCKSIZE * 50);

	//cacheIO.WriteAtOffset(100, 10, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(0, BLOCKSIZE, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(BLOCKSIZE - 1, 2, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(0, BLOCKSIZE * 2, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(0, BLOCKSIZE * 3, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(10, BLOCKSIZE, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(10, BLOCKSIZE * 2, apDataBuffer.m_pData);
	//cacheIO.WriteAtOffset(10, BLOCKSIZE * 3, apDataBuffer.m_pData);


	//cacheIO.WriteAtOffset(20 * BLOCKSIZE + 1, BLOCKSIZE * 50, apDataBuffer.m_pData);

	//memset(apDataBuffer.m_pData, 'A', BLOCKSIZE * 50);
	//cacheIO.WriteAtOffset(2, 3, apDataBuffer.m_pData);

	END_BLOCK(0);

	return hr;
}

HRESULT test_copy_vhdx(LPCTSTR vhdxFile, LPCTSTR destFile, LPCTSTR bitmapFile)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IDiskImageVirtual> spIDiskImageVirtual;
	CreateInstanceCDiskImageVhdxParams params =
	{
		NULL,//LPCTSTR parentVhdxFileName;
		vhdxFile,//LPCTSTR vhdxFileName;
		GENERIC_READ,//DWORD DesiredAccess;
		0,//DWORD CreationDisposition;
		0,//LONGLONG VirtualDiskSize;
		0,//LONG LogicalSectorSize;
		0,//LONG PhysicalSectorSize;
		0,//LONG BlockSize;
		VHDX_DISK_TYPE_ENUM::DYNAMIC
	};
	hr = CreateInstanceCDiskImageVhdx(params, &spIDiskImageVirtual, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCDiskImageVhdx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spIFileDeviceBitmap;
	hr = CreateInstanceFileDeviceWin32(
		bitmapFile,//IN LPCTSTR strFileName, 
		GENERIC_ALL,//IN ULONG ulDesiredAccess, 
		0,//IN ULONG ulShareMode, 
		CREATE_ALWAYS,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDeviceBitmap,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("bitmapFile"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = spIDiskImageVirtual->GetBitmap(0, -1, spIFileDeviceBitmap);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetBitmap"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spIFileDeviceDest;
	hr = CreateInstanceFileDeviceWin32(
		destFile,//IN LPCTSTR strFileName, 
		GENERIC_WRITE,//IN ULONG ulDesiredAccess, 
		0,//IN ULONG ulShareMode, 
		OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDeviceDest,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("destFile"));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////

	LONGLONG BmpFileSize = 0;
	hr = spIFileDeviceBitmap->GetFileSize(&BmpFileSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CVMemPtr<BYTE> apBitmapBuffer;
	apBitmapBuffer.Allocate(BmpFileSize);
	if (NULL == apBitmapBuffer.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	ULONG NumberOfBytesRead = 0;
	hr = ReadAtOffsetHelper(spIFileDeviceBitmap, 0, BmpFileSize, &NumberOfBytesRead, apBitmapBuffer.m_pData);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CSimpleBitArray bitArr(apBitmapBuffer.m_pData, BmpFileSize * 8);
	CComPtr<IFileCopy> spIFileCopy;
	hr = CreateInstanceCMTCopy(
		spIDiskImageVirtual,//IFileDevice * pSource,
		spIFileDeviceDest,//IFileDevice * pDest, 
		0,//LONGLONG SourceStartOffset, 
		0,//LONGLONG Diff,
		&bitArr,//CSimpleBitArray * pSimpleBitArray,
		spIDiskImageVirtual->GetSectorSize(),//ULONG BlockSize,
		FALSE, //<sonmi01>2014-12-12 ###???
		&spIFileCopy,//IFileCopy ** ppObj,
		TRUE//BOOL bCreateMemeber
		);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCMTCopy"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = spIFileCopy->Copy();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Copy"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}


HRESULT test_copy_raw_vhdx_bitmap_file(LPCTSTR srcVhdxFile, LPCTSTR destVhdxFile, LPCTSTR bitmapFile, LONGLONG VirtualDiskSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IDiskImageVirtual> spDiskSrc;
	CreateInstanceCDiskImageVhdxParams params =
	{
		NULL,//LPCTSTR parentVhdxFileName;
		srcVhdxFile,//LPCTSTR vhdxFileName;
		GENERIC_READ,//DWORD DesiredAccess;
		0,//DWORD CreationDisposition;
		0,//LONGLONG VirtualDiskSize;
		0,//LONG LogicalSectorSize;
		0,//LONG PhysicalSectorSize;
		0,//LONG BlockSize;
		VHDX_DISK_TYPE_ENUM::DYNAMIC
	};
	hr = CreateInstanceCDiskImageVhdx(params, &spDiskSrc, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCDiskImageVhdx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spIFileDeviceBitmap;
	hr = CreateInstanceFileDeviceWin32(
		bitmapFile,//IN LPCTSTR strFileName, 
		GENERIC_ALL,//IN ULONG ulDesiredAccess, 
		0,//IN ULONG ulShareMode, 
		CREATE_ALWAYS,//IN ULONG ulCreationDisposition, 
		FILE_FLAG_NO_BUFFERING,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDeviceBitmap,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("bitmapFile"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = spDiskSrc->GetBitmap(0, -1, spIFileDeviceBitmap);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetBitmap"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONGLONG BmpFileSize = 0;
	hr = spIFileDeviceBitmap->GetFileSize(&BmpFileSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IDiskImageVirtual> spIDiskDest;

	CreateInstanceCDiskImageVhdxParams paramsWrite =
	{
		NULL,//LPCTSTR parentVhdxFileName;
		destVhdxFile,//LPCTSTR vhdxFileName;
		GENERIC_WRITE,//DWORD DesiredAccess;
		0,//DWORD CreationDisposition;
		VirtualDiskSize,//LONGLONG VirtualDiskSize;
		512,//LONG LogicalSectorSize;
		4096,//LONG PhysicalSectorSize;
		2 * 1024 * 1024,//LONG BlockSize;
		VHDX_DISK_TYPE_ENUM::DYNAMIC
	};
	hr = CreateInstanceCDiskImageVhdx(paramsWrite, &spIDiskDest, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCDiskImageVhdx"), hr, TEXT("destFile"));
		LEAVE_BLOCK(0);
	}

	CONST LONG BITMAP_BUFFER_LEN = 8 * 1024 * 1024;
	CVMemPtr<BYTE> apBitmapBuffer;
	apBitmapBuffer.Allocate(BITMAP_BUFFER_LEN);

	LONGLONG BitmapOffset = 0;
	LONG CurrentBitmapLen = 0;

	CComPtr<IFileCopy> spIFileCopy; //unique object

	while (BmpFileSize)
	{
		//////////////////////////////////////////////////////////////////////////
		if (BmpFileSize > BITMAP_BUFFER_LEN)
		{
			CurrentBitmapLen = BITMAP_BUFFER_LEN;
		}
		else
		{
			CurrentBitmapLen = BmpFileSize;
		}

		//////////////////////////////////////////////////////////////////////////
		ZeroMemory(apBitmapBuffer.m_pData, BITMAP_BUFFER_LEN);
		ULONG NumberOfBytesRead = 0;
		hr = ReadAtOffsetHelper(spIFileDeviceBitmap, BitmapOffset, CurrentBitmapLen, &NumberOfBytesRead, apBitmapBuffer.m_pData);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		CSimpleBitArray bitArr(apBitmapBuffer.m_pData, CurrentBitmapLen * 8);

		

		LONG BlockSize = spDiskSrc->GetSectorSize();

		LONGLONG SourceStartOffset = BitmapOffset;
		SourceStartOffset *= 8;
		SourceStartOffset *= BlockSize;

		if (NULL == spIFileCopy)
		{
			hr = CreateInstanceCMTCopy(
				spDiskSrc,//IFileDevice * pSource,
				spIDiskDest,//IFileDevice * pDest, 
				SourceStartOffset,//LONGLONG SourceStartOffset, 
				0,//LONGLONG Diff,
				&bitArr,//CSimpleBitArray * pSimpleBitArray,
				BlockSize,//ULONG BlockSize,
				FALSE, //<sonmi01>2014-12-12 ###???
				&spIFileCopy,//IFileCopy ** ppObj,
				TRUE//BOOL bCreateMemeber
				);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("CreateInstanceCMTCopy"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}
		}
		else
		{
			CMTCopyParams MTCopyParams  = 
			{
				spDiskSrc,//IFileDevice * pSource;
				spIDiskDest,//IFileDevice * pDest; 
				SourceStartOffset,//LONGLONG SourceStartOffset;
				0,//LONGLONG Diff;
				&bitArr,//CSimpleBitArray * pSimpleBitArray;
				BlockSize//ULONG BlockSize;
			};
			spIFileCopy->InitParams(&MTCopyParams);
		}
		

		//////////////////////////////////////////////////////////////////////////
		hr = spIFileCopy->Copy();
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("Copy"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		hr = spIDiskDest->FlushData();
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("FlushData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		hr = spIDiskDest->FlushMetadata();
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("FlushMetadata"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		BitmapOffset += CurrentBitmapLen;
		BmpFileSize -= CurrentBitmapLen;
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}


HRESULT test_copy_raw_vhdx(LPCTSTR srcVhdxFile, LPCTSTR destVhdxFile, LPCTSTR bitmapFile, LONGLONG VirtualDiskSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IDiskImageVirtual> spDiskSrc;
	CreateInstanceCDiskImageVhdxParams params =
	{
		NULL,//LPCTSTR parentVhdxFileName;
		srcVhdxFile,//LPCTSTR vhdxFileName;
		GENERIC_READ,//DWORD DesiredAccess;
		0,//DWORD CreationDisposition;
		0,//LONGLONG VirtualDiskSize;
		0,//LONG LogicalSectorSize;
		0,//LONG PhysicalSectorSize;
		0,//LONG BlockSize;
		VHDX_DISK_TYPE_ENUM::DYNAMIC
	};
	hr = CreateInstanceCDiskImageVhdx(params, &spDiskSrc, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCDiskImageVhdx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	if (NULL == destVhdxFile || _tcslen(destVhdxFile) <= 3)
	{
		D_INFO(0, TEXT("targe vhdx file name is emptry"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IDiskImageVirtual> spIDiskDest;
	CreateInstanceCDiskImageVhdxParams paramsWrite =
	{
		NULL,//LPCTSTR parentVhdxFileName;
		destVhdxFile,//LPCTSTR vhdxFileName;
		GENERIC_WRITE,//DWORD DesiredAccess;
		0,//DWORD CreationDisposition;
		VirtualDiskSize,//LONGLONG VirtualDiskSize;
		512,//LONG LogicalSectorSize;
		4096,//LONG PhysicalSectorSize;
		2 * 1024 * 1024,//LONG BlockSize;
		VHDX_DISK_TYPE_ENUM::DYNAMIC
	};
	hr = CreateInstanceCDiskImageVhdx(paramsWrite, &spIDiskDest, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCDiskImageVhdx"), hr, TEXT("destFile"));
		LEAVE_BLOCK(0);
	}

	CONST LONG BITMAP_BUFFER_LEN = 16 * 1024 * 1024;
	CVMemPtr<BYTE> apBitmapBuffer;
	apBitmapBuffer.Allocate(BITMAP_BUFFER_LEN);

	LONG BITMAP_BUFFER_BLOCK_COUNT = BITMAP_BUFFER_LEN / spDiskSrc->GetBitmapSizePerBlock();
	LONG TotalBlockCount = spDiskSrc->GetBlockCount();
	LONG VDiskBlockSize = spDiskSrc->GetBlockSize();
	LONG SectorSize = spDiskSrc->GetSectorSize();


	

	LONG BlockOffset = 0;
	LONG CurrentBlockCount = 0;

	while (TotalBlockCount)
	{
		//////////////////////////////////////////////////////////////////////////
		if (TotalBlockCount > BITMAP_BUFFER_BLOCK_COUNT)
		{
			CurrentBlockCount = BITMAP_BUFFER_BLOCK_COUNT;
		}
		else
		{
			CurrentBlockCount = TotalBlockCount;
		}

		//////////////////////////////////////////////////////////////////////////
		ZeroMemory(apBitmapBuffer.m_pData, BITMAP_BUFFER_LEN);

		CComPtr<IFileDeviceMem> spBitmap;
		hr = CreateInstanceCFileDeviceMem(apBitmapBuffer.m_pData, BITMAP_BUFFER_LEN, &spBitmap, TRUE);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("CreateInstanceCFileDeviceMem"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		hr = spDiskSrc->GetBitmap(BlockOffset, CurrentBlockCount, spBitmap);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		LONGLONG BmpFileSize = 0;
		spBitmap->GetFileSize(&BmpFileSize);

		//////////////////////////////////////////////////////////////////////////
		LONGLONG BitLen = CurrentBlockCount;
		BitLen *= spDiskSrc->GetBitmapSizePerBlock();
		ATLASSERT(BitLen == BmpFileSize);
		BitLen *= 8;
		CSimpleBitArray bitArr(apBitmapBuffer.m_pData, BitLen);

		LONGLONG SourceStartOffset = BlockOffset;
		SourceStartOffset *= VDiskBlockSize;

		CComPtr<IFileCopy> spIFileCopy; //unique object
		if (NULL == spIFileCopy)
		{
			hr = CreateInstanceCMTCopy(
				spDiskSrc,//IFileDevice * pSource,
				spIDiskDest,//IFileDevice * pDest, 
				SourceStartOffset,//LONGLONG SourceStartOffset, 
				0,//LONGLONG Diff,
				&bitArr,//CSimpleBitArray * pSimpleBitArray,
				SectorSize,//ULONG BlockSize,
				FALSE, //<sonmi01>2014-12-12 ###???
				&spIFileCopy,//IFileCopy ** ppObj,
				TRUE//BOOL bCreateMemeber
				);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("CreateInstanceCMTCopy"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}
		}
		else
		{
			CMTCopyParams MTCopyParams  = 
			{
				spDiskSrc,//IFileDevice * pSource;
				spIDiskDest,//IFileDevice * pDest; 
				SourceStartOffset,//LONGLONG SourceStartOffset;
				0,//LONGLONG Diff;
				&bitArr,//CSimpleBitArray * pSimpleBitArray;
				SectorSize//ULONG BlockSize;
			};
			spIFileCopy->InitParams(&MTCopyParams);
		}


		//////////////////////////////////////////////////////////////////////////
		hr = spIFileCopy->Copy();
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("Copy"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		hr = spIDiskDest->FlushData();
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("FlushData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		hr = spIDiskDest->FlushMetadata();
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("FlushMetadata"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		BlockOffset += CurrentBlockCount;
		TotalBlockCount -= CurrentBlockCount;
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}


HRESULT test_copy_raw_vhdx_sector(LPCTSTR srcVhdxFile, LPCTSTR destVhdxFile, LONG SectorSize, LONGLONG VirtualDiskSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IDiskImageVirtual> spDiskSrc;
	CreateInstanceCDiskImageVhdxParams params =
	{
		NULL,//LPCTSTR parentVhdxFileName;
		srcVhdxFile,//LPCTSTR vhdxFileName;
		GENERIC_READ,//DWORD DesiredAccess;
		0,//DWORD CreationDisposition;
		0,//LONGLONG VirtualDiskSize;
		0,//LONG LogicalSectorSize;
		0,//LONG PhysicalSectorSize;
		0,//LONG BlockSize;
		VHDX_DISK_TYPE_ENUM::DYNAMIC
	};
	hr = CreateInstanceCDiskImageVhdx(params, &spDiskSrc, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCDiskImageVhdx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	if (NULL == destVhdxFile || _tcslen(destVhdxFile) <= 3)
	{
		D_INFO(0, TEXT("targe vhdx file name is emptry"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IDiskImageVirtual> spIDiskDest;
	CreateInstanceCDiskImageVhdxParams paramsWrite =
	{
		NULL,//LPCTSTR parentVhdxFileName;
		destVhdxFile,//LPCTSTR vhdxFileName;
		GENERIC_WRITE,//DWORD DesiredAccess;
		0,//DWORD CreationDisposition;
		VirtualDiskSize,//LONGLONG VirtualDiskSize;
		SectorSize? SectorSize : 512,//LONG LogicalSectorSize;
		4096,//LONG PhysicalSectorSize;
		2 * 1024 * 1024,//LONG BlockSize;
		VHDX_DISK_TYPE_ENUM::DYNAMIC
	};
	hr = CreateInstanceCDiskImageVhdx(paramsWrite, &spIDiskDest, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCDiskImageVhdx"), hr, TEXT("destFile"));
		LEAVE_BLOCK(0);
	}

	CONST LONG BITMAP_BUFFER_LEN = 16 * 1024 * 1024;
	CVMemPtr<BYTE> apBitmapBuffer;
	apBitmapBuffer.Allocate(BITMAP_BUFFER_LEN);

	LONG BITMAP_BUFFER_BLOCK_COUNT = BITMAP_BUFFER_LEN / spDiskSrc->GetBitmapSizePerBlock();
	LONG TotalBlockCount = spDiskSrc->GetBlockCount();
	LONG VDiskBlockSize = spDiskSrc->GetBlockSize();
	LONG SectorSize = spDiskSrc->GetSectorSize();




	LONG BlockOffset = 0;
	LONG CurrentBlockCount = 0;

	while (TotalBlockCount)
	{
		//////////////////////////////////////////////////////////////////////////
		if (TotalBlockCount > BITMAP_BUFFER_BLOCK_COUNT)
		{
			CurrentBlockCount = BITMAP_BUFFER_BLOCK_COUNT;
		}
		else
		{
			CurrentBlockCount = TotalBlockCount;
		}

		//////////////////////////////////////////////////////////////////////////
		ZeroMemory(apBitmapBuffer.m_pData, BITMAP_BUFFER_LEN);

		CComPtr<IFileDeviceMem> spBitmap;
		hr = CreateInstanceCFileDeviceMem(apBitmapBuffer.m_pData, BITMAP_BUFFER_LEN, &spBitmap, TRUE);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("CreateInstanceCFileDeviceMem"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		hr = spDiskSrc->GetBitmap(BlockOffset, CurrentBlockCount, spBitmap);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		LONGLONG BmpFileSize = 0;
		spBitmap->GetFileSize(&BmpFileSize);

		//////////////////////////////////////////////////////////////////////////
		LONGLONG BitLen = CurrentBlockCount;
		BitLen *= spDiskSrc->GetBitmapSizePerBlock();
		ATLASSERT(BitLen == BmpFileSize);
		BitLen *= 8;
		CSimpleBitArray bitArr(apBitmapBuffer.m_pData, BitLen);

		LONGLONG SourceStartOffset = BlockOffset;
		SourceStartOffset *= VDiskBlockSize;

		CComPtr<IFileCopy> spIFileCopy; //unique object
		if (NULL == spIFileCopy)
		{
			hr = CreateInstanceCMTCopy(
				spDiskSrc,//IFileDevice * pSource,
				spIDiskDest,//IFileDevice * pDest, 
				SourceStartOffset,//LONGLONG SourceStartOffset, 
				0,//LONGLONG Diff,
				&bitArr,//CSimpleBitArray * pSimpleBitArray,
				SectorSize,//ULONG BlockSize,
				FALSE, //<sonmi01>2014-12-12 ###???
				&spIFileCopy,//IFileCopy ** ppObj,
				TRUE//BOOL bCreateMemeber
				);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("CreateInstanceCMTCopy"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}
		}
		else
		{
			CMTCopyParams MTCopyParams =
			{
				spDiskSrc,//IFileDevice * pSource;
				spIDiskDest,//IFileDevice * pDest; 
				SourceStartOffset,//LONGLONG SourceStartOffset;
				0,//LONGLONG Diff;
				&bitArr,//CSimpleBitArray * pSimpleBitArray;
				SectorSize//ULONG BlockSize;
			};
			spIFileCopy->InitParams(&MTCopyParams);
		}


		//////////////////////////////////////////////////////////////////////////
		hr = spIFileCopy->Copy();
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("Copy"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		hr = spIDiskDest->FlushData();
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("FlushData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		hr = spIDiskDest->FlushMetadata();
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("FlushMetadata"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		BlockOffset += CurrentBlockCount;
		TotalBlockCount -= CurrentBlockCount;
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}


HRESULT test_simple_read_write_raw_vhdx(LPCTSTR VhdxFile)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IDiskImageVirtual> spIDiskRW;
	CreateInstanceCDiskImageVhdxParams paramsWrite =
	{
		NULL,//LPCTSTR parentVhdxFileName;
		VhdxFile,//LPCTSTR vhdxFileName;
		GENERIC_WRITE,//DWORD DesiredAccess;
		-1,//DWORD CreationDisposition;
		1024LL*1024*1024,//LONGLONG VirtualDiskSize;
		0,//LONG LogicalSectorSize;
		0,//LONG PhysicalSectorSize;
		0,//LONG BlockSize;
		VHDX_DISK_TYPE_ENUM::FIXED
	};
	hr = CreateInstanceCDiskImageVhdx(paramsWrite, &spIDiskRW, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCDiskImageVhdx"), hr, TEXT("destFile"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CONST BYTE Buffer[] = "!!!hello, hwehrwjergwhrgwherwuryowiriwq;ohr;qfs;adhfaslkdfhasjklfhaskjfh. world!!!";
	LONGLONG Offset = 1024 * 1024 +  6785;

	ULONG  NumberOfBytesWritten = 0;
	hr = WriteAtOffsetHelper(spIDiskRW, Offset, sizeof(Buffer), &NumberOfBytesWritten, Buffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = spIDiskRW->FlushData();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FlushData"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = spIDiskRW->FlushMetadata();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FlushMetadata"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	spIDiskRW.Release();

	//////////////////////////////////////////////////////////////////////////
	CreateInstanceCDiskImageVhdxParams params =
	{
		NULL,//LPCTSTR parentVhdxFileName;
		VhdxFile,//LPCTSTR vhdxFileName;
		GENERIC_READ,//DWORD DesiredAccess;
		-1,//DWORD CreationDisposition;
		0,//LONGLONG VirtualDiskSize;
		0,//LONG LogicalSectorSize;
		0,//LONG PhysicalSectorSize;
		0,//LONG BlockSize;
		VHDX_DISK_TYPE_ENUM::DYNAMIC
	};
	hr = CreateInstanceCDiskImageVhdx(params, &spIDiskRW, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCDiskImageVhdx"), hr, TEXT("destFile"));
		LEAVE_BLOCK(0);
	}

	BYTE Buffer2[sizeof(Buffer)] = {0};

	ULONG NumberOfBytesRead = 0;
	hr = ReadAtOffsetHelper(spIDiskRW, Offset, sizeof(Buffer2), &NumberOfBytesRead, Buffer2);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	ATLASSERT(0 == memcmp(Buffer, Buffer2, sizeof(Buffer)));

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT test_simple_create_raw_vhdx(LPCTSTR VhdxFile, LONGLONG VirtualDiskSize, LONG LogicalSectorSize, LONG PhysicalSectorSize, LONG BlockSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IDiskImageVirtual> spIDiskRW;
	CreateInstanceCDiskImageVhdxParams paramsWrite =
	{
		NULL,//LPCTSTR parentVhdxFileName;
		VhdxFile,//LPCTSTR vhdxFileName;
		GENERIC_WRITE,//DWORD DesiredAccess;
		-1,//DWORD CreationDisposition;
		VirtualDiskSize,//LONGLONG VirtualDiskSize;
		LogicalSectorSize,//LONG LogicalSectorSize;
		PhysicalSectorSize,//LONG PhysicalSectorSize;
		BlockSize,//LONG BlockSize;
		VHDX_DISK_TYPE_ENUM::DYNAMIC
	};

	hr = CreateInstanceCDiskImageVhdx(paramsWrite, &spIDiskRW, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCDiskImageVhdx"), hr, TEXT("destFile"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}


HRESULT test_simple_create_raw_vhdx_fixed(LPCTSTR VhdxFile, LONGLONG VirtualDiskSize, LONG LogicalSectorSize, LONG PhysicalSectorSize, LONG BlockSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IDiskImageVirtual> spIDiskRW;
	CreateInstanceCDiskImageVhdxParams paramsWrite =
	{
		NULL,//LPCTSTR parentVhdxFileName;
		VhdxFile,//LPCTSTR vhdxFileName;
		GENERIC_WRITE,//DWORD DesiredAccess;
		-1,//DWORD CreationDisposition;
		VirtualDiskSize,//LONGLONG VirtualDiskSize;
		LogicalSectorSize,//LONG LogicalSectorSize;
		PhysicalSectorSize,//LONG PhysicalSectorSize;
		BlockSize,//LONG BlockSize;
		VHDX_DISK_TYPE_ENUM::FIXED
	};

	hr = CreateInstanceCDiskImageVhdx(paramsWrite, &spIDiskRW, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCDiskImageVhdx"), hr, TEXT("destFile"));
		LEAVE_BLOCK(0);
	}

	spIDiskRW->FlushMetadata();
	spIDiskRW->FlushData();

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

int _tmain_test_header_cache_read_write(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LPCTSTR strFileName = argv[1];
	//LPCTSTR strDestDrive = argv[2];

	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(
		strFileName,//IN LPCTSTR strFileName, 
		GENERIC_ALL,//IN ULONG ulDesiredAccess, 
		0,//IN ULONG ulShareMode, 
		CREATE_ALWAYS,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDevice,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		LEAVE_BLOCK(0);
	}

	LONG BLOCKSIZE = 4 * 1024;
	CCacheIO cacheIO(spIFileDevice, FALSE, BLOCKSIZE);


	//////////////////////////////////////////////////////////////////////////
	LONGLONG offset = 0;
	BYTE byteValWrite = 'a';

	for (int ii = 0; ii < 2; ++ ii)
	{
		cacheIO.WriteAtOffset(offset, sizeof(byteValWrite), &byteValWrite);
		offset += BLOCKSIZE;
		byteValWrite ++;
	}

	//////////////////////////////////////////////////////////////////////////
	BYTE byteValRead = 0;
	offset = 0;
	for (int ii = 0; ii < 5; ++ ii)
	{
		byteValRead = 0;
		cacheIO.ReadAtOffset(offset, sizeof(byteValRead), &byteValRead);
		offset += BLOCKSIZE;
	}

	//////////////////////////////////////////////////////////////////////////
	offset = 0;
	byteValWrite = '1';
	for (int ii = 0; ii < 5; ++ ii)
	{
		cacheIO.WriteAtOffset(offset, sizeof(byteValWrite), &byteValWrite);
		offset += BLOCKSIZE;
		byteValWrite ++;
	}

	//////////////////////////////////////////////////////////////////////////
	byteValRead = 0;
	offset = 0;
	for (int ii = 0; ii < 10; ++ ii)
	{
		byteValRead = 0;
		cacheIO.ReadAtOffset(offset, sizeof(byteValRead), &byteValRead);
		offset += BLOCKSIZE;
	}

	//////////////////////////////////////////////////////////////////////////
	cacheIO.Flush();

	END_BLOCK(0);

	return hr;
}


void InitSomeData(map<CString, CString> & m)
{
	TCHAR key[] = TEXT("11111");
	TCHAR val[] = TEXT("AAAAA");

	for (LONG ii = 0; ii < 5; ++ ii)
	{
		m.insert(make_pair(key, val));
		++ key[0];
		++ val[0];
	}
}


int _tmain_test_create_new_full_vhdx(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LPCTSTR strFileName = argv[2];

	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(
		strFileName,//IN LPCTSTR strFileName, 
		GENERIC_ALL,//IN ULONG ulDesiredAccess, 
		0,//IN ULONG ulShareMode, 
		CREATE_ALWAYS,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDevice,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		LEAVE_BLOCK(0);
	}

	CAutoPtr<ParserSeparateVHDX> apParserVHDX(new ParserSeparateVHDX(spIFileDevice.Detach()));

	LONGLONG VirtualDiskSize = 1; //8T
	VirtualDiskSize <<= 46;

	LONG LogicalSectorSize = 512;
	LONG PhysicalSectorSize = 4096;
	LONG BlockSize = 2 * 1024 * 1024;
	map<CString, CString> ParentLocatorEntries;
	if (0)
	{
		InitSomeData(ParentLocatorEntries);
	}
	
	hr = apParserVHDX->ParseCreateNew(VirtualDiskSize, LogicalSectorSize, PhysicalSectorSize, BlockSize, ParentLocatorEntries, GUID_NULL, VHDX_DISK_TYPE_ENUM::DYNAMIC);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Parse"), hr, TEXT("Invalid VHDX"));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);


	//////////////////////////////////////////////////////////////////////////
	BEGIN_BLOCK(1);
	//////////////////////////////////////////////////////////////////////////
	LPCTSTR strFileName = argv[2];
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(
		strFileName,//IN LPCTSTR strFileName, 
		GENERIC_READ,//IN ULONG ulDesiredAccess, 
		FILE_SHARE_READ,//IN ULONG ulShareMode, 
		OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDevice,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		LEAVE_BLOCK(1);
	}

	CAutoPtr<ParserSeparateVHDX> apParserVHDX(new ParserSeparateVHDX(spIFileDevice.Detach()));
	hr = apParserVHDX->Parse();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Parse"), hr, TEXT("Invalid VHDX"));
		LEAVE_BLOCK(1);
	}

	END_BLOCK(1);

	return hr;
}


/******************************************************************
\\sonmi01-790\q$\zzz-vhd\sonmi01-test-vhdx_C3AA6C5F-2936-4BED-94F2-66AC06EC47B6.avhdx \\lijyo02-9010\e$\zzz.vhdx \\lijyo02-9010\e$\zzz-no-exists.vhdx \\sonmi01-790\q$\zzz-vhd\sonmi01-test-vhdx_C3AA6C5F-2936-4BED-94F2-66AC06EC47B6.avhdx \\lijyo02-9010\e$\remote-parent.avhdx
******************************************************************/
int _tmain_test_create_open_read_write(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;

	//////////////////////////////////////////////////////////////////////////
	CreateInstanceCDiskImageVhdxParams params0 =
	{
		NULL,//LPCTSTR		parentVhdxFileName;
		argv[1],//LPCTSTR		vhdxFileName;
		GENERIC_READ,//DWORD		DesiredAccess;
		0,//DWORD		CreationDisposition;
		0,//LONGLONG	VirtualDiskSize;
		0,//LONG		LogicalSectorSize;
		0,//LONG		PhysicalSectorSize;
		0,//LONG BlockSize;
		VHDX_DISK_TYPE_ENUM::DYNAMIC
	};
	CComPtr<IDiskImageVirtual> spIDiskImageVirtual0;
	hr = CreateInstanceCDiskImageVhdx(params0, &spIDiskImageVirtual0, TRUE);


	//////////////////////////////////////////////////////////////////////////
	LONGLONG VirtualDiskSize = 1;
	VirtualDiskSize <<= 46;

	CreateInstanceCDiskImageVhdxParams params1 =
	{
		NULL,//LPCTSTR		parentVhdxFileName;
		argv[2],//LPCTSTR		vhdxFileName; //file exist
		GENERIC_WRITE,//DWORD		DesiredAccess;
		0,//DWORD		CreationDisposition;
		VirtualDiskSize,//LONGLONG	VirtualDiskSize;
		512,//LONG		LogicalSectorSize;
		4096,//LONG		PhysicalSectorSize;
		32 * 1024 * 1024,//LONG BlockSize;
		VHDX_DISK_TYPE_ENUM::DYNAMIC
	};
	CComPtr<IDiskImageVirtual> spIDiskImageVirtual1;
	hr = CreateInstanceCDiskImageVhdx(params1, &spIDiskImageVirtual1, TRUE);


	CreateInstanceCDiskImageVhdxParams params2 =
	{
		NULL,//LPCTSTR		parentVhdxFileName;
		argv[3],//LPCTSTR		vhdxFileName; //file not exist
		GENERIC_WRITE,//DWORD		DesiredAccess;
		0,//DWORD		CreationDisposition;
		VirtualDiskSize,//LONGLONG	VirtualDiskSize;
		512,//LONG		LogicalSectorSize;
		4096,//LONG		PhysicalSectorSize;
		32 * 1024 * 1024,//LONG BlockSize;
		VHDX_DISK_TYPE_ENUM::DYNAMIC
	};
	CComPtr<IDiskImageVirtual> spIDiskImageVirtual2;
	hr = CreateInstanceCDiskImageVhdx(params2, &spIDiskImageVirtual2, TRUE);


	CreateInstanceCDiskImageVhdxParams params3 =
	{
		argv[4],//LPCTSTR		parentVhdxFileName;
		argv[5],//LPCTSTR		vhdxFileName; //file not exist
		GENERIC_WRITE,//DWORD		DesiredAccess;
		0,//DWORD		CreationDisposition;
		VirtualDiskSize,//LONGLONG	VirtualDiskSize;
		512,//LONG		LogicalSectorSize;
		4096,//LONG		PhysicalSectorSize;
		32 * 1024 * 1024,//LONG BlockSize;
		VHDX_DISK_TYPE_ENUM::DYNAMIC
	};
	CComPtr<IDiskImageVirtual> spIDiskImageVirtual3;
	hr = CreateInstanceCDiskImageVhdx(params3, &spIDiskImageVirtual3, TRUE);

	return 0;
}

HRESULT test_vhdx_VDiskFileList(LPCTSTR srcVhdxFile)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IDiskImageVirtual> spDiskSrc;
	CreateInstanceCDiskImageVhdxParams params =
	{
		NULL,//LPCTSTR parentVhdxFileName;
		srcVhdxFile,//LPCTSTR vhdxFileName;
		GENERIC_READ,//DWORD DesiredAccess;
		0,//DWORD CreationDisposition;
		0,//LONGLONG VirtualDiskSize;
		0,//LONG LogicalSectorSize;
		0,//LONG PhysicalSectorSize;
		0,//LONG BlockSize;
		VHDX_DISK_TYPE_ENUM::DYNAMIC
	};
	hr = CreateInstanceCDiskImageVhdx(params, &spDiskSrc, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCDiskImageVhdx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	CONST vector<CString> & diskList = spDiskSrc->VDiskFileList();
	for (LONG ii = 0; ii < diskList.size(); ++ ii)
	{
		D_INFO(0, TEXT("[disk list] [%02d, %s]"), ii, diskList[ii].GetString());
	}

	END_BLOCK(0);

	return hr;
}

/*************************************
Q:\zzz-vhd\sonmi01-test-vhdx_C3AA6C5F-2936-4BED-94F2-66AC06EC47B6.avhdx Q:\zzz-vhd\zzzz-child-of-xxx.vhdx Q:\zzzz-child-of-xxx.vhdx  Q:\zzz-vhd2\1\2\3\zzzz-child-of-xxx.vhdx
*************************************/
int _tmain_test_create_nwe_child_rel_path(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	for (int ii = 2; ii < 5; ++ ii)
	{
		CreateInstanceCDiskImageVhdxParams paramsWrite =
		{
			argv[1],//LPCTSTR		parentVhdxFileName;
			argv[ii],//LPCTSTR		vhdxFileName; //file not exist
			GENERIC_WRITE,//DWORD		DesiredAccess;
			0,//DWORD		CreationDisposition;
			0,//LONGLONG	VirtualDiskSize;
			0,//LONG		LogicalSectorSize;
			0,//LONG		PhysicalSectorSize;
			0,//LONG BlockSize;
			VHDX_DISK_TYPE_ENUM::DYNAMIC
		};
		CComPtr<IDiskImageVirtual> spWrite;
		hr = CreateInstanceCDiskImageVhdx(paramsWrite, &spWrite, TRUE);
		spWrite->FlushData();
		spWrite->FlushMetadata();
	}

	for (int ii = 2; ii < 5; ++ ii)
	{
		CreateInstanceCDiskImageVhdxParams paramsRead =
		{
			NULL,//LPCTSTR		parentVhdxFileName;
			argv[ii],//LPCTSTR		vhdxFileName; //file not exist
			GENERIC_READ,//DWORD		DesiredAccess;
			0,//DWORD		CreationDisposition;
			0,//LONGLONG	VirtualDiskSize;
			0,//LONG		LogicalSectorSize;
			0,//LONG		PhysicalSectorSize;
			0,//LONG		BlockSize;
			VHDX_DISK_TYPE_ENUM::DYNAMIC
		};
		CComPtr<IDiskImageVirtual> spRead;
		hr = CreateInstanceCDiskImageVhdx(paramsRead, &spRead, TRUE);
		spRead->FlushData();
		spRead->FlushMetadata();
	}
	return 0;
}

//<sonmi01>2014-9-24 ###???
HRESULT test_copy_raw_vhdx_single_to_disk_image(LPCTSTR srcVhdxFile, LPCTSTR destVhdxFile, LONGLONG VirtualDiskSize, LONG SectorSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<ISeparateVhdx> spDiskSrc;
	hr = CreateInstanceCSeparateVhdx(srcVhdxFile, &spDiskSrc, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCDiskImageVhdx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	if (NULL == destVhdxFile || _tcslen(destVhdxFile) <= 3)
	{
		D_INFO(0, TEXT("targe vhdx file name is emptry"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IDiskImageVirtual> spIDiskDest;
	CreateInstanceCDiskImageVhdxParams paramsWrite =
	{
		NULL,//LPCTSTR parentVhdxFileName;
		destVhdxFile,//LPCTSTR vhdxFileName;
		GENERIC_WRITE,//DWORD DesiredAccess;
		0,//DWORD CreationDisposition;
		VirtualDiskSize,//LONGLONG VirtualDiskSize;
		SectorSize ? SectorSize : spDiskSrc->GetSectorSize(),//LONG LogicalSectorSize;
		4096,//LONG PhysicalSectorSize;
		2 * 1024 * 1024,//LONG BlockSize;
		VHDX_DISK_TYPE_ENUM::DYNAMIC
	};
	hr = CreateInstanceCDiskImageVhdx(paramsWrite, &spIDiskDest, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCDiskImageVhdx"), hr, TEXT("destFile"));
		LEAVE_BLOCK(0);
	}


	CHeapPtr<BYTE> bitMapBuffer;
	LONG BlockSize = 0;
	LONG BlockCount = 0;
	spDiskSrc->GetBlocksBitmap(BlockSize, BlockCount, &bitMapBuffer);

	CComPtr<IFileCopy> spIFileCopy;
	CSimpleBitArray bitArr(bitMapBuffer.m_pData, BlockCount);
	hr = CreateInstanceCMTCopy(
		spDiskSrc,//IFileDevice * pSource,
		spIDiskDest,//IFileDevice * pDest, 
		0,//LONGLONG SourceStartOffset, 
		0,//LONGLONG Diff,
		&bitArr,//CSimpleBitArray * pSimpleBitArray,
		BlockSize,//ULONG BlockSize,
		FALSE, //<sonmi01>2014-12-12 ###???
		&spIFileCopy,//IFileCopy ** ppObj,
		TRUE//BOOL bCreateMemeber
		);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCMTCopy"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = spIFileCopy->Copy();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Copy"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = spIDiskDest->FlushData();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FlushData"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = spIDiskDest->FlushMetadata();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FlushMetadata"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

int _tmain(int argc, _TCHAR* argv[])
{
	D_SETTINGS(9, 0, 0, 0);
	/*volatile*/ int testCase = 0;
	GetTestCaseNumber(argc, argv, testCase);
	//volatile int testCase = 13;
	//volatile int testCase = 14;
	//volatile int testCase = 15;
	switch (testCase)
	{
	case 20:
	{
		SmartLib::Apply_VMIOF_RT_CBT::Main(argc, argv);
	}
	break;
	case  19:
	{
		SmartLib::test_CSparseBlockMemory(argv[1], argv[2]);
	}
	break;
	case 18:
	{
		CONST LONG blockSize = 8;
		BYTE buffer[blockSize] = { 0 };
		BYTE guard[blockSize] = { 0 };
		SmartLib::CSparseBlockMemory sbm(8, 8);
		sbm.CreateMember();

		LONGLONG offset = 0;
		LONGLONG newFP = 0;
		for (int ii = 0; ii < 3; ++ ii)
		{
			sbm.SetPointerEx(offset, &newFP, FILE_BEGIN);
			offset += blockSize;
		}

		sbm.SetPointerEx(0, &newFP, FILE_BEGIN);
		ULONG NumberOfBytesWritten = 0;
		for (int ii = 0; ii < 5; ++ ii)
		{
			memset(buffer, 'a' + ii, sizeof(buffer));
			sbm.Write(sizeof(buffer), &NumberOfBytesWritten, buffer);
		}



		sbm.SetPointerEx(0, &newFP, FILE_BEGIN);
		ULONG NumberOfBytesRead = 0;
		for (int ii = 0; ii < 7; ++ii)
		{
			memset(buffer, 0, sizeof(buffer));
			sbm.Read(sizeof(buffer), &NumberOfBytesRead, buffer);
		}

		for (int ii = 0; ii < 2; ++ ii)
		{
			sbm.SetPointerEx(sizeof(buffer), &newFP, FILE_CURRENT);
		}

		LONGLONG backOffset = -2 * (LONGLONG)(sizeof(buffer));
		sbm.SetPointerEx(backOffset, &newFP, FILE_CURRENT);
		for (int ii = 0; ii < 2; ++ii)
		{
			memset(buffer, 0, sizeof(buffer));
			sbm.Read(sizeof(buffer), &NumberOfBytesRead, buffer);
		}
	}
	break;
	case 17:
	{
		_tmain_test_bulk_write(argc, argv);
	}
	break;
	case 0:
		_tmain_test_header(argc, argv);
		break;
	case 1:
		_tmain_test_header_product_code(argc, argv);
		break;
	case 2:
		_tmain_test_header_cache_read(argc, argv);
		break;
	case 3:
		{
			vector<CString> vhdxFiles;
			if (0)
			{
				vhdxFiles.push_back(TEXT("Q:\\zzz-vhd\\sonmi01-test-vhdx_9B163595-CD15-4D53-A2AF-A76DAF40409F.avhdx"));
				vhdxFiles.push_back(TEXT("Q:\\zzz-vhd\\sonmi01-test-vhdx.vhdx"));
			} 
			else
			{
				vhdxFiles.push_back(TEXT("Q:\\zzz-vhd\\liuwe05-hv02_9794C577-A19A-4D1C-9610-FD11018DF69A.avhdx"));
				vhdxFiles.push_back(TEXT("Q:\\zzz-vhd\\liuwe05-hv02.vhdx"));
			}
			
			LPCTSTR strDestDrive = TEXT("\\\\.\\PhysicalDrive2");
			_tmain_test_vhdx_chain_simple(vhdxFiles, strDestDrive);
		}
		break;
	case 4:
		_tmain_test_header_cache_write(argc, argv);
		break;

	case 5:
		{
			LPCTSTR vhdxFile = argv[1];
			LPCTSTR destFile = argv[2];
			LPCTSTR bitmapFile = argv[3];
			test_copy_vhdx(vhdxFile, destFile, bitmapFile);
		}
		break;
	case 6:
		{
			LPCTSTR srcVhdxFile = argv[1];
			LPCTSTR destVhdxFile = argv[2];
			LPCTSTR bitmapFile = argv[3];
			LONGLONG VirtualDiskSize = 64;
			if (argc > 4)
			{
				VirtualDiskSize = _tcstoi64(argv[4], NULL, 0);
				if (VirtualDiskSize <= 0)
				{
					VirtualDiskSize = 64;
				}
			}
			VirtualDiskSize <<= 40;
			test_copy_raw_vhdx(srcVhdxFile, destVhdxFile, bitmapFile, VirtualDiskSize);
		}
		break;
	case 7:
		test_simple_read_write_raw_vhdx(argv[1]);
		break;
	case  8:
		_tmain_test_header_cache_read_write(argc, argv);
		break;
	case 9:
		_tmain_test_create_new_full_vhdx(argc, argv);
		break;
	case  10:
		_tmain_test_create_open_read_write(argc, argv);
		break;
	case 11:
		_tmain_test_create_nwe_child_rel_path(argc, argv);
		break;
	case 12:
		{
			_ftprintf_s(stdout, TEXT("this.exe <VhdxFile> <VirtualDiskSizeGB> <LogicalSectorSizeBYTE> <PhysicalSectorSizeBYTE>") TEXT("\r\n"));
			LPCTSTR VhdxFile = argv[1];
			LONGLONG VirtualDiskSize = _tcstoi64(argv[2], NULL, 0);
			VirtualDiskSize <<= 30;
			LONG LogicalSectorSize = _tcstol(argv[3], NULL, 0);
			LONG PhysicalSectorSize = _tcstol(argv[4], NULL, 0);
			LONG BlockSize = 2 * 1024 * 1024;
			test_simple_create_raw_vhdx(VhdxFile, VirtualDiskSize, LogicalSectorSize, PhysicalSectorSize, BlockSize);
		}
		break;
	case 13:
	{
			   if (argc > 1)
			   {
				   test_vhdx_VDiskFileList(argv[1]);
			   }
			   break;
			   
	}
	case  14:
	{
		IN LPCTSTR strFileName = argc > 1 ? argv[1] : TEXT("\\\\sonmi01-hv12\\d$\\zzzz-log-replay.vhdx\\2.vhdx");
		VhdxReplayLog(strFileName, LONGLONG_MAX, NULL); //<sonmi01>2014-2-25 ###???
		break;
	}
	case  15:
	{
		LONGLONG VirtualDiskSize = 1;
		VirtualDiskSize <<= 30;
		test_simple_create_raw_vhdx_fixed(argv[1], VirtualDiskSize, 4096, 4096, 2*1024*1024);
		break;
	}
	case 16:
	{

		printf("this.exe <sourceVhdx> <destVhdx> [SectorSize] [VirtualDiskSize in TB]");
		LPCTSTR srcVhdxFile = argv[1];
		LPCTSTR destVhdxFile = argv[2];
		LONG SectorSize = 0;
		LONGLONG VirtualDiskSize = 64;

		if (argc > 3)
		{
			SectorSize = _tcstol(argv[3], NULL, 0);
			if (SectorSize <= 0)
			{
				SectorSize = 0;
			}
		}
		
		if (argc > 4)
		{
			VirtualDiskSize = _tcstoi64(argv[4], NULL, 0);
			if (VirtualDiskSize <= 0)
			{
				VirtualDiskSize = 64;
			}
		}
		VirtualDiskSize <<= 40;

		test_copy_raw_vhdx_single_to_disk_image(srcVhdxFile, destVhdxFile, VirtualDiskSize, SectorSize);
		break;
	}
	default:
		break;
	}
	return 0;
}

