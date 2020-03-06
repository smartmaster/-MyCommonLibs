#include "stdafx.h"
#include "VHDXLogReplay.h"

#include <intsafe.h>

HRESULT VhdxReplayLog(LPCTSTR pVHDX, LONGLONG OriginalVhdxFileSize, IN IVHDXLogReplayCallback *pCallBack) //<sonmi01>2014-3-1 ###???
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(
		pVHDX,//IN LPCTSTR strFileName,
		GENERIC_WRITE | GENERIC_READ,//IN ULONG ulDesiredAccess,
		0,//IN ULONG ulShareMode,
		OPEN_EXISTING,//IN ULONG ulCreationDisposition,
		0,//IN ULONG ulFlagsAndAttributes,
		&spIFileDevice,//OUT IFileDevice ** ppFileDevice,
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		D_SET_FISRT_STATUS(hr, LastError);
		D_SET_FISRT_ERROR_MESSAGE(TEXT("CreateInstanceFileDeviceWin32 [%s]"), pVHDX);
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("%s"), pVHDX);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = VhdxReplayLogAPI(spIFileDevice, OriginalVhdxFileSize, pCallBack);
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		D_SET_FISRT_STATUS(hr, LastError);
		D_SET_FISRT_ERROR_MESSAGE(TEXT("ReplayLogEntry"));
		D_API_ERR(0, TEXT("ReplayLogEntry"), hr, TEXT("%s"), pVHDX);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

//<sonmi01>2014-2-25 ###???
HRESULT VhdxReplayLogAPI(IFileDevice * pIFileDeviceVHDX, LONGLONG OriginalVhdxFileSize, IN IVHDXLogReplayCallback *pCallBack)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	VHDXLogReplay_NS::BackendFile BackendFile;
	BackendFile.m_BackendFile = pIFileDeviceVHDX;
	BackendFile.m_OriginalFileSize = OriginalVhdxFileSize;
	BackendFile.m_pCallback = pCallBack;


	//////////////////////////////////////////////////////////////////////////
	VHDXLogReplay_NS::VDiskContext VDiskContext = { 0 };
	VOID *ErrorReport = NULL;
	hr = VHDXLogReplay_NS::CVHDXLogReplay::ReplayLogEntry(&BackendFile, &VDiskContext, 0, &ErrorReport);
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		D_SET_FISRT_STATUS(hr, LastError);
		D_SET_FISRT_ERROR_MESSAGE(TEXT("ReplayLogEntry"));
		D_API_ERR(0, TEXT("ReplayLogEntry"), hr, TEXT("0x%p"), pIFileDeviceVHDX);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

INT VHDXLogReplay_NS::CVHDXLogReplay::ReplayLogEntry(BackendFile *pBackendFile, VDiskContext *pVDiskContext, INT Flags, VOID ** ppErrorReport)
{
	INT Status = 0;
	UINT64 Signature;

	pVDiskContext->m_bWriteOnce = TRUE;

	Status = BackendFileRead(pBackendFile, 0, (BYTE*)&Signature, sizeof(UINT64));
	if (Status < 0)
	{
		D_API_ERR(0, TEXT("FileRead"), Status, TEXT("Signature"));
		goto FAIL_LABEL;
	}

	
	if (memcmp(&Signature, SIG_VHDXFILE, sizeof(SIG_VHDXFILE) - 1))
	{
		Status = E_INVALIDARG;
		D_API_ERR(0, TEXT("InvalidSignature"), Status, TEXT("Signature=%I64d"), Signature);
		goto FAIL_LABEL;
	}

	GenerateGUID(&pVDiskContext->m_NewGuid);

	Status = ParseVDiskHeader(pBackendFile, pVDiskContext);
	if (Status < 0)
	{
		D_API_ERR(0, TEXT("ParseVDiskHeader"), Status, TEXT(""));
		goto FAIL_LABEL;
	}

	Status = ReplayLog(pBackendFile, pVDiskContext, &pVDiskContext->m_bLogReplayed, ppErrorReport);
	if (Status < 0)
	{
		D_API_ERR(0, TEXT("ParseLog"), Status, TEXT(""));
		goto FAIL_LABEL;
	}


FAIL_LABEL:
	CloseReplayLog(pVDiskContext);
	return Status;
}

INT VHDXLogReplay_NS::CVHDXLogReplay::ParseVDiskHeader(BackendFile *pBackendFile, VDiskContext *pVDiskContext)
{
	INT Status = 0;
	VDiskHeader * Header1 = NULL;
	VDiskHeader * Header2 = NULL;
	BOOL Hdr1Valid = FALSE;
	BOOL Hdr2Valid = FALSE;
	UINT64 H1Seq = 0;
	UINT64 H2Seq = 0;
	UINT8 * Buffer = NULL;

	Header1 = (VDiskHeader *)VALLOC(pBackendFile, sizeof(VDiskHeader));
	Header2 = (VDiskHeader *)VALLOC(pBackendFile, sizeof(VDiskHeader));

	Buffer = (UINT8 *)VALLOC(pBackendFile, VHDX_HEADER_SIZE);

	pVDiskContext->m_Headers[0] = Header1;
	pVDiskContext->m_Headers[1] = Header2;


	Status = BackendFileRead(pBackendFile, VHDX_HEADER1_OFFSET, Buffer, VHDX_HEADER_SIZE);
	if (Status < 0)
	{
		D_API_ERR(0, TEXT("FileRead"), Status, TEXT("HEADER1"));
		goto FAIL_LABEL;
	}

	memcpy(Header1, Buffer, sizeof(VDiskHeader));

	if (VerifyChecksum(Buffer, VHDX_HEADER_SIZE, 4) &&
		!memcmp(&Header1->m_Signature, SIG_HEAD, sizeof(SIG_HEAD)-1) &&
		Header1->m_Version == 1)
	{
		H1Seq = Header1->m_SequenceNumber;
		Hdr1Valid = TRUE;
	}

	Status = BackendFileRead(pBackendFile, VHDX_HEADER2_OFFSET, Buffer, VHDX_HEADER_SIZE);
	if (Status < 0)
	{
		D_API_ERR(0, TEXT("FileRead"), Status, TEXT("HEADER2"));
		goto FAIL_LABEL;
	}


	memcpy(Header2, Buffer, sizeof(VDiskHeader));


	if (VerifyChecksum(Buffer, VHDX_HEADER_SIZE, 4) &&
		!memcmp(&Header2->m_Signature, SIG_HEAD, sizeof(SIG_HEAD)-1) &&
		Header2->m_Version == 1)
	{
		H2Seq = Header2->m_SequenceNumber;
		Hdr2Valid = TRUE;
	}


	if (Hdr1Valid && !Hdr2Valid)
	{
		pVDiskContext->m_CurrentHeaderIndex = 0;
	}
	else if (!Hdr1Valid && Hdr2Valid)
	{
		pVDiskContext->m_CurrentHeaderIndex = 1;
	}
	else if (!Hdr1Valid && !Hdr2Valid)
	{
		D_API_ERR(0, TEXT("InvalidHeader"), Status, TEXT("HEADER"));
		Status = E_INVALIDARG;
		goto FAIL_LABEL;
	}
	else
	{
		if (H1Seq > H2Seq)
		{
			pVDiskContext->m_CurrentHeaderIndex = 0;
		}
		else if (H2Seq > H1Seq)
		{
			pVDiskContext->m_CurrentHeaderIndex = 1;
		}
		else
		{
			D_API_ERR(0, TEXT("InvalidHeaderSeq"), Status, TEXT("%I64d, %I64d"), H1Seq, H2Seq);
			Status = E_INVALIDARG;
			goto FAIL_LABEL;
		}
	}

	Status = 0;
	goto EXIT_LABEL;

FAIL_LABEL:
	VFREE(Header1);
	VFREE(Header2);
	pVDiskContext->m_Headers[0] = NULL;
	pVDiskContext->m_Headers[1] = NULL;
EXIT_LABEL:
	VFREE(Buffer);
	return Status;
}

VOID VHDXLogReplay_NS::CVHDXLogReplay::GenerateGUID(GUID *guid)
{
	CoCreateGuid(guid);
}

INT VHDXLogReplay_NS::CVHDXLogReplay::BackendFileRead(BackendFile * pBackendFile, LONGLONG Offset, BYTE* pBuffer, ULONG nNumberOfBytesToRead)
{
	ULONG NumberOfBytesRead = 0;
	return ReadAtOffsetHelper(pBackendFile->m_BackendFile, Offset, nNumberOfBytesToRead, &NumberOfBytesRead, pBuffer);
}

INT VHDXLogReplay_NS::CVHDXLogReplay::BackendFileWrite(BackendFile * pBackendFile, LONGLONG Offset, CONST BYTE* pBuffer, ULONG nNumberOfBytesToWrite)
{
	ULONG NumberOfBytesWritten = 0;
	return WriteAtOffsetHelper(pBackendFile->m_BackendFile, Offset, nNumberOfBytesToWrite, &NumberOfBytesWritten, pBuffer);
}

LONGLONG VHDXLogReplay_NS::CVHDXLogReplay::BackendFileGetSize(BackendFile * pBackendFile)
{
	LONGLONG FileSize = 0;
	if (-1LL == pBackendFile->m_OriginalFileSize)
	{
		pBackendFile->m_BackendFile->GetFileSize(&FileSize);
	}
	else
	{
		FileSize = pBackendFile->m_OriginalFileSize;
	}
	return FileSize;
}

INT VHDXLogReplay_NS::CVHDXLogReplay::BackendFileResize(BackendFile * pBackendFile, LONGLONG newFileSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	HANDLE hFileTemp = pBackendFile->m_BackendFile->GetRawHandle();

	LARGE_INTEGER liDistanceToMove = { 0 };
	liDistanceToMove.QuadPart = newFileSize;
	LARGE_INTEGER NewFilePointer = { 0 };
	BOOL bSetFilePointerEx = SetFilePointerEx(
		hFileTemp,//_In_       HANDLE hFile,
		liDistanceToMove,//_In_       LARGE_INTEGER liDistanceToMove,
		&NewFilePointer,//_Out_opt_  PLARGE_INTEGER lpNewFilePointer,
		FILE_BEGIN//_In_       DWORD dwMoveMethod
		);
	if (!bSetFilePointerEx)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_SET_FISRT_STATUS(hr, LastError);
		D_SET_FISRT_ERROR_MESSAGE(TEXT("SetFilePointerEx"));
		D_API_ERR(0, TEXT("SetFilePointerEx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	BOOL bSetEndOfFile = SetEndOfFile(
		hFileTemp//_In_  HANDLE hFile
		);
	if (!bSetEndOfFile)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_SET_FISRT_STATUS(hr, LastError);
		D_SET_FISRT_ERROR_MESSAGE(TEXT("SetEndOfFile"));
		D_API_ERR(0, TEXT("SetEndOfFile"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

INT VHDXLogReplay_NS::CVHDXLogReplay::BackendFileFlush(BackendFile *pBackendFile)
{
	DWORD LastError = 0;
	HRESULT hr = S_OK;
	BOOL bFlushFileBuffers = FlushFileBuffers(pBackendFile->m_BackendFile->GetRawHandle());
	if (!bFlushFileBuffers)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_SET_FISRT_STATUS(hr, LastError);
		D_SET_FISRT_ERROR_MESSAGE(TEXT("FlushFileBuffers [0x%p]"), pBackendFile->m_BackendFile->GetRawHandle());
		D_API_ERR(0, TEXT("FlushFileBuffers"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}
	return 0;
}

BOOL VHDXLogReplay_NS::CVHDXLogReplay::VerifyChecksum(UINT8 *Buffer, size_t Size, INT crcOffset)
{
	UINT32 crcOrig;
	UINT32 crcCalc;

	ATLASSERT(Buffer != NULL);
	ATLASSERT(Size > (crcOffset + 4));

	memcpy(&crcOrig, Buffer + crcOffset, sizeof(crcOrig));

	crcCalc = CalcChecksum(0xffffffff, Buffer, Size, crcOffset);

	return crcCalc == crcOrig;
}

UINT32 VHDXLogReplay_NS::CVHDXLogReplay::CalcChecksum(UINT32 crcInitial, UINT8 *Buffer, size_t Size, INT crcOffset)
{
	UINT32 crcNew;
	UINT32 crcOrig;
	ATLASSERT(Buffer != NULL);

	if (crcOffset > 0)
	{
		memcpy(&crcOrig, Buffer + crcOffset, sizeof(crcOrig));
		memset(Buffer + crcOffset, 0, sizeof(crcOrig));
	}

	crcNew = crc32c(crcInitial, Buffer, Size);
	if (crcOffset > 0)
	{
		memcpy(Buffer + crcOffset, &crcOrig, sizeof(crcOrig));
	}

	return crcNew;
}

INT VHDXLogReplay_NS::CVHDXLogReplay::ReplayLog(BackendFile *pBackendFile, VDiskContext *pVDiskContext, BOOL *bLogReplayed, VOID **ErrorReport)
{
	INT Status = 0;
	VDiskHeader *vdiskHeader = NULL;
	LogSequence logSeq = { 0 };

	vdiskHeader = pVDiskContext->m_Headers[pVDiskContext->m_CurrentHeaderIndex];

	*bLogReplayed = FALSE;

	if (pVDiskContext->m_LogEntries.m_LogEntryHeader == NULL)
	{
		pVDiskContext->m_LogEntries.m_LogEntryHeader = (LogEntryHeader *)VALLOC(pBackendFile, sizeof(LogEntryHeader));
	}

	pVDiskContext->m_LogEntries.m_Offset = vdiskHeader->m_LogOffset;
	pVDiskContext->m_LogEntries.m_Length = vdiskHeader->m_LogLength;

	if (pVDiskContext->m_LogEntries.m_Offset < VHDX_LOG_MIN_SIZE ||
		pVDiskContext->m_LogEntries.m_Offset % VHDX_LOG_MIN_SIZE)
	{
		D_API_ERR(0, TEXT("m_LogEntries.m_Offset"), E_INVALIDARG, TEXT("%I64d"), pVDiskContext->m_LogEntries.m_Offset);
		Status = E_INVALIDARG;
		goto EXIT_LABLE;
	}


	if (vdiskHeader->m_LogVersion != 0)
	{
		D_API_ERR(0, TEXT("m_LogVersion"), E_INVALIDARG, TEXT("%d"), vdiskHeader->m_LogVersion);
		Status = E_INVALIDARG;
		goto EXIT_LABLE;
	}


	if (GuidEqual(vdiskHeader->m_LogGuid, NullGuid))
	{
		D_API_ERR(0, TEXT("m_LogGuid"), S_OK, TEXT("NULL"));
		goto EXIT_LABLE;
	}

	if (vdiskHeader->m_LogLength == 0)
	{
		D_API_ERR(0, TEXT("m_LogLength"), S_OK, TEXT("0"));
		goto EXIT_LABLE;
	}

	if (vdiskHeader->m_LogLength % VHDX_LOG_MIN_SIZE)
	{
		D_API_ERR(0, TEXT("m_LogLength"), E_INVALIDARG, TEXT("%d"), vdiskHeader->m_LogLength);
		Status = E_INVALIDARG;
		goto EXIT_LABLE;
	}

	Status = SearchActiveSeq(pBackendFile, pVDiskContext, &logSeq);
	if (Status < 0)
	{
		D_API_ERR(0, TEXT("SearchActiveSeq"), Status, TEXT(""));
		goto EXIT_LABLE;
	}

	if (logSeq.m_Valid)
	{
		Status = FlushLog(pBackendFile, pVDiskContext, &logSeq);
		if (Status < 0)
		{
			D_API_ERR(0, TEXT("FlushLog"), Status, TEXT(""));
			goto EXIT_LABLE;
		}
		*bLogReplayed = TRUE;
	}


EXIT_LABLE:
	return Status;
}

BOOL VHDXLogReplay_NS::CVHDXLogReplay::GuidEqual(CONST GUID & a, CONST GUID & b)
{
	return (0 == memcmp(&(a), &(b), sizeof(GUID)));
}

INT VHDXLogReplay_NS::CVHDXLogReplay::SearchActiveSeq(BackendFile *pBackendFile, VDiskContext *pVDiskContext, LogSequence *LogSeq)
{
	INT Status = 0;
	UINT32 Tail;
	BOOL bSeqValid = FALSE;
	LogSequence candidateSeq = { 0 };
	LogEntryHeader logEntryHdr = { 0 };
	LogEntries currLogEntries;

	memcpy(&currLogEntries, &pVDiskContext->m_LogEntries, sizeof(LogEntries));
	currLogEntries.m_Write = currLogEntries.m_Length;
	currLogEntries.m_Read = 0;

	//<sonmi01>2014-3-5 ###???
	LONG loop_count = 0;

	for (;;)
	{
		UINT64 currSeq = 0;
		LogSequence currentLogSeq = { 0 };

		Tail = currLogEntries.m_Read;

		//<sonmi01>2014-3-5 ###???
		LONG BeforeReadOffset = currLogEntries.m_Read;

		Status = VerifyLogEntry(
			pBackendFile,
			pVDiskContext,
			&currLogEntries,
			currSeq,
			&bSeqValid,
			&logEntryHdr);
		if (Status < 0)
		{
			D_API_ERR(0, TEXT("VerifyLogEntry"), Status, TEXT(""));
			goto EXIT_LABEL;
		}
				
		//////////////////////////////////////////////////////////////////////////
		if (bSeqValid)
		{
			//////////////////////////////////////////////////////////////////////////
			//<sonmi01>2014-3-5 ###???
			D_INFO(0, TEXT("BeforeReadOffset=%08x, EntryLength=%08x, Tail=%08x, SequenceNumber=%016I64x, Read=%08x, Write=%08x ==========start=========="),
				BeforeReadOffset,
				(logEntryHdr).m_EntryLength,
				(logEntryHdr).m_Tail,
				(logEntryHdr).m_SequenceNumber,
				(currLogEntries).m_Read,
				(currLogEntries).m_Write
				);

			//////////////////////////////////////////////////////////////////////////
			currentLogSeq.m_Valid = TRUE;
			currentLogSeq.m_LogEntries = currLogEntries;
			currentLogSeq.m_LogEntries.m_Read = Tail;
			currentLogSeq.m_LogEntries.m_Write = currLogEntries.m_Read;
			currentLogSeq.m_Count = 1;
			currentLogSeq.m_LogEntryHeader = logEntryHdr;
			currSeq = logEntryHdr.m_SequenceNumber; //<sonmi01>2014-3-10 ###???

			//////////////////////////////////////////////////////////////////////////
			for (;;)
			{
				//<sonmi01>2014-3-5 ###???
				BeforeReadOffset = currLogEntries.m_Read;

				Status = VerifyLogEntry(
					pBackendFile,
					pVDiskContext,
					&currLogEntries,
					currSeq,
					&bSeqValid,
					&logEntryHdr);
				if (Status < 0)
				{
					D_API_ERR(0, TEXT("VerifyLogEntry"), Status, TEXT(""));
					goto EXIT_LABEL;
				}


				if (bSeqValid == FALSE)
				{
					D_INFO(0, TEXT("BeforeReadOffset=%08x, Read=0x%08x, Tail=0x%08x, currentSeq=0x%016I64x, Count=0x%08x, EndSeq=0x%016I64x ==========end=========="),
						BeforeReadOffset,
						currLogEntries.m_Read,
						Tail,
						currentLogSeq.m_LogEntryHeader.m_SequenceNumber,
						currentLogSeq.m_Count,
						currentLogSeq.m_LogEntryHeader.m_SequenceNumber + currentLogSeq.m_Count);

					D_API_ERR(0, TEXT("SeqValid"), Status, TEXT("FALSE"));
					break;
				}

				//<sonmi01>2014-3-5 ###???
				D_INFO(0, TEXT("BeforeReadOffset=%08x, EntryLength=%08x, Tail=%08x, SequenceNumber=%016I64x, Read=%08x, Write=%08x"),
					BeforeReadOffset,
					(logEntryHdr).m_EntryLength,
					(logEntryHdr).m_Tail,
					(logEntryHdr).m_SequenceNumber,
					(currLogEntries).m_Read,
					(currLogEntries).m_Write
					);

				currentLogSeq.m_LogEntries.m_Write = currLogEntries.m_Read;
				currentLogSeq.m_Count++;

				currSeq = logEntryHdr.m_SequenceNumber;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		if (currentLogSeq.m_Valid)
		{
			if (candidateSeq.m_Valid == FALSE ||
				currentLogSeq.m_LogEntryHeader.m_SequenceNumber > candidateSeq.m_LogEntryHeader.m_SequenceNumber)
			{
				D_INFO(0, TEXT("Set greater sequence number [0x%016I64x] as candidate, previous one is [0x%016I64x]"), 
					currentLogSeq.m_LogEntryHeader.m_SequenceNumber,
					candidateSeq.m_LogEntryHeader.m_SequenceNumber);
				candidateSeq = currentLogSeq;
			}
		}

		if (currLogEntries.m_Read < Tail)
		{
			break;
		}

		//<sonmi01>2014-3-5 ###???
		CONST LONG MAX_LOOP_COUNT = 4;
		if (currLogEntries.m_Read == Tail &&
			currentLogSeq.m_Valid && 
			candidateSeq.m_Valid && 
			(currentLogSeq.m_LogEntryHeader.m_SequenceNumber + currentLogSeq.m_Count == candidateSeq.m_LogEntryHeader.m_SequenceNumber + candidateSeq.m_Count)
			)
		{
			++loop_count;
			if (1 == loop_count)
			{
				D_INFO(0, TEXT("Detected circular buffer turned around. [Read=0x%08x, Tail=0x%08x, currentSeq=0x%016I64x, Count=0x%08x, candidateSeq=0x%016I64x, Count=0x%08x, EndSeq=0x%016I64x"), 
					currLogEntries.m_Read,
					Tail,
					currentLogSeq.m_LogEntryHeader.m_SequenceNumber,
					currentLogSeq.m_Count,
					candidateSeq.m_LogEntryHeader.m_SequenceNumber,
					candidateSeq.m_Count,
					currentLogSeq.m_LogEntryHeader.m_SequenceNumber + currentLogSeq.m_Count);
			}
			//assume seq 7893456, active seq is 789 or 3456789??????
			if (currentLogSeq.m_Count > candidateSeq.m_Count)
			{
				D_INFO(0, TEXT("Set longer sequence as cadinate. [Read=0x%08x, Tail=0x%08x, currentSeq=0x%016I64x, Count=0x%08x, candidateSeq=0x%016I64x, Count=0x%08x, EndSeq=0x%016I64x"),
					currLogEntries.m_Read,
					Tail,
					currentLogSeq.m_LogEntryHeader.m_SequenceNumber,
					currentLogSeq.m_Count,
					candidateSeq.m_LogEntryHeader.m_SequenceNumber,
					candidateSeq.m_Count,
					currentLogSeq.m_LogEntryHeader.m_SequenceNumber + currentLogSeq.m_Count);

				candidateSeq = currentLogSeq;
			}
			if (loop_count >= MAX_LOOP_COUNT)
			{
				D_INFO(0, TEXT("Finished processig circular entries"));
				break;
			}
		}
		else
		{
			loop_count = 0;
		}
		//////////////////////////////////////////////////////////////////////////
	}

	*LogSeq = candidateSeq;

	if (candidateSeq.m_Valid)
	{
		pVDiskContext->m_LogEntries.m_Sequence = candidateSeq.m_LogEntryHeader.m_SequenceNumber + 1;
	}

EXIT_LABEL:
	return Status;
}

INT VHDXLogReplay_NS::CVHDXLogReplay::VerifyLogEntry(BackendFile *pBackendFile, VDiskContext *pVDiskContext, LogEntries *pLogEntries, UINT64 seqNum, BOOL *bValid, LogEntryHeader *pLogEntryHeader)
{
	INT Status = 0;
	LogEntryHeader logEntryhdr;
	BYTE *Buffer = NULL;
	UINT32 ii, descSectors, totalSectors, crcVal;
	UINT32 sectorsRead = 0;
	LogDescEntries *LogDescEntBuffer = NULL;

	*bValid = FALSE;

	Status = ReadLogEntryHeader(pBackendFile, pLogEntries, &logEntryhdr);
	if (Status < 0)
	{
		D_API_ERR(0, TEXT("ReadLogEntryHeader"), Status, TEXT(""));
		goto INC_EXIT_LABEL;
	}



	if (VerifyLogHeader(pLogEntries, &logEntryhdr, pVDiskContext) == FALSE)
	{
		D_API_ERR(99, TEXT("VerifyLogHeader"), Status, TEXT(""));
		goto INC_EXIT_LABEL;
	}

	if (seqNum > 0)
	{
		if (logEntryhdr.m_SequenceNumber != seqNum + 1)
		{
			goto INC_EXIT_LABEL;
		}
	}

	descSectors = CalcDescSectors(logEntryhdr.m_DescriptorCount);

	totalSectors = logEntryhdr.m_EntryLength / VHDX_LOG_SECTOR_SIZE;

	Status = ReadLogDesc(pBackendFile, pVDiskContext, pLogEntries, &LogDescEntBuffer);
	if (Status < 0)
	{
		D_API_ERR(0, TEXT("ReadLogDesc"), Status, TEXT(""));
		goto FREE_EXIT_LABEL;
	}

	crcVal = CalcChecksum(0xffffffff, (BYTE *)LogDescEntBuffer, descSectors * VHDX_LOG_SECTOR_SIZE, 4);
	crcVal ^= 0xffffffff;

	Buffer = (BYTE *)VALLOC(pBackendFile, VHDX_LOG_SECTOR_SIZE);
	if (totalSectors > descSectors)
	{
		for (ii = 0; ii < totalSectors - descSectors; ii++)
		{
			sectorsRead = 0;
			Status = ReadLogSectors(pBackendFile, pLogEntries, &sectorsRead, Buffer, 1, FALSE);
			if (Status < 0 || sectorsRead != 1)
			{
				goto FREE_EXIT_LABEL;
			}
			crcVal = CalcChecksum(crcVal, Buffer, VHDX_LOG_SECTOR_SIZE, -1);
			crcVal ^= 0xffffffff;
		}
	}
	crcVal ^= 0xffffffff;
	if (crcVal != LogDescEntBuffer->m_LogEntryHeader.m_Checksum)
	{
		D_API_ERR(0, TEXT("Checksum"), Status, TEXT("%d, %d"), crcVal, LogDescEntBuffer->m_LogEntryHeader.m_Checksum);
		goto FREE_EXIT_LABEL;
	}

	*bValid = TRUE;
	*pLogEntryHeader = logEntryhdr;
	goto FREE_EXIT_LABEL;

INC_EXIT_LABEL:
	pLogEntries->m_Read = IncLogIndex(pLogEntries->m_Read, pLogEntries->m_Length);

FREE_EXIT_LABEL:
	VFREE(Buffer);
	VFREE(LogDescEntBuffer);
	return Status;
}

INT VHDXLogReplay_NS::CVHDXLogReplay::ReadLogEntryHeader(BackendFile *pBackendFile, LogEntries *pLogEntries, LogEntryHeader *pLogEntryHeader)
{
	INT Status = 0;
	UINT64 Offset;
	UINT32 Read;

	ATLASSERT(pLogEntryHeader != NULL);

	if (pLogEntries->m_Read % VHDX_LOG_SECTOR_SIZE)
	{
		Status = E_FAIL;
		D_API_ERR(0, TEXT("LogEntries->m_Read"), Status, TEXT("%d"), pLogEntries->m_Read);
		goto EXIT_LABEL;
	}

	Read = pLogEntries->m_Read;
	if ((Read + sizeof(LogEntryHeader)) > pLogEntries->m_Length)
	{
		Read = 0;
	}

	if (Read == pLogEntries->m_Write)
	{
		if (pLogEntries->m_Write == pLogEntries->m_Read) //<sonmi01>2014-3-10 ###???
		{
			D_API_ERR(0, TEXT("pLogEntries->m_Write == pLogEntries->m_Read"), Status, TEXT("%d, %d"), pLogEntries->m_Write, pLogEntries->m_Read);
		}
		else
		{
			Status = E_INVALIDARG;
			D_API_ERR(0, TEXT("LogEntries->m_Write, pLogEntries->m_Read, Read"), Status, TEXT("%d, %d, %d"), pLogEntries->m_Write, pLogEntries->m_Read, Read);
			goto EXIT_LABEL;
		}
		
	}

	Offset = pLogEntries->m_Offset + Read;

	Status = BackendFileRead(pBackendFile, Offset, (BYTE*)pLogEntryHeader, sizeof(LogEntryHeader));
	if (Status < 0)
	{
		D_API_ERR(0, TEXT("FileRead"), Status, TEXT(""));
		goto EXIT_LABEL;
	}

EXIT_LABEL:
	return Status;
}

BOOL VHDXLogReplay_NS::CVHDXLogReplay::VerifyLogHeader(LogEntries *pLogEntries, LogEntryHeader *pLogEntryHeader, VDiskContext *pVDiskContext)
{
	INT valid = FALSE;

	if (memcmp(&pLogEntryHeader->m_Signature, SIG_LOGE, sizeof(SIG_LOGE)-1))
	{
		D_API_ERR(99, TEXT("LogEntryHeader->m_Signature"), E_INVALIDARG, TEXT("%d"), pLogEntryHeader->m_Signature);
		goto EXIT_LABEL;
	}


	if (pLogEntries->m_Length < pLogEntryHeader->m_EntryLength)
	{
		D_API_ERR(0, TEXT("pLogEntries->m_Length"), E_INVALIDARG, TEXT("%d, %d"), pLogEntries->m_Length, pLogEntryHeader->m_EntryLength);
		goto EXIT_LABEL;
	}


	if (pLogEntryHeader->m_EntryLength % VHDX_LOG_SECTOR_SIZE)
	{
		D_API_ERR(0, TEXT("pLogEntries->m_EntryLength"), E_INVALIDARG, TEXT("%d, %d"), pLogEntryHeader->m_EntryLength, VHDX_LOG_SECTOR_SIZE);
		goto EXIT_LABEL;
	}

	if (pLogEntryHeader->m_SequenceNumber == 0)
	{
		D_API_ERR(0, TEXT("pLogEntryHeader->m_SequenceNumber"), E_INVALIDARG, TEXT("%I64d"), pLogEntryHeader->m_SequenceNumber);
		goto EXIT_LABEL;
	}

	if (!GuidEqual(pLogEntryHeader->m_LogGuid, pVDiskContext->m_Headers[pVDiskContext->m_CurrentHeaderIndex]->m_LogGuid))
	{
		D_API_ERR(0, TEXT("m_LogGuid"), E_INVALIDARG, TEXT("m_LogGuid"));
		goto EXIT_LABEL;
	}

	if (pLogEntryHeader->m_DescriptorCount * sizeof(LogDescriptor) > pLogEntryHeader->m_EntryLength)
	{
		D_API_ERR(0, TEXT("LogEntryHeader->m_EntryLength"), E_INVALIDARG, TEXT("%d, %d"), pLogEntryHeader->m_EntryLength, pLogEntryHeader->m_DescriptorCount * sizeof(LogDescriptor));
		goto EXIT_LABEL;
	}

	valid = TRUE;

EXIT_LABEL:
	return valid;
}

INT VHDXLogReplay_NS::CVHDXLogReplay::CalcDescSectors(UINT32 descCount)
{
	UINT32 descSectors;
	descCount += 2;
	descSectors = descCount / 128;
	if (descCount % 128)
	{
		descSectors++;
	}
	return descSectors;
}

INT VHDXLogReplay_NS::CVHDXLogReplay::ReadLogDesc(BackendFile *pBackendFile, VDiskContext *pVDiskContext, LogEntries *pLogEntries, LogDescEntries **ppLogDescEntries)
{
	INT Status = 0;
	UINT32 descSectors;
	UINT32 sectorsRead;
	LogEntryHeader logEntryHeader;
	LogDescEntries *descEntries = NULL;
	INT ii;

	ATLASSERT(*ppLogDescEntries == NULL);

	Status = ReadLogEntryHeader(pBackendFile, pLogEntries, &logEntryHeader);
	if (Status < 0)
	{
		D_API_ERR(0, TEXT("ReadLogEntryHeader"), E_INVALIDARG, TEXT(""));
		goto EXIT_LABEL;
	}

	if (VerifyLogHeader(pLogEntries, &logEntryHeader, pVDiskContext) == FALSE)
	{
		Status = E_INVALIDARG;
		D_API_ERR(0, TEXT("VerifyLogHeader"), E_INVALIDARG, TEXT(""));
		goto EXIT_LABEL;
	}

	descSectors = CalcDescSectors(logEntryHeader.m_DescriptorCount);
	descEntries = (LogDescEntries *)VALLOC(pBackendFile, descSectors * VHDX_LOG_SECTOR_SIZE);

	Status = ReadLogSectors(pBackendFile, pLogEntries, &sectorsRead, (BYTE*)descEntries, descSectors, FALSE);
	if (Status < 0)
	{
		D_API_ERR(0, TEXT("ReadLogSectors"), Status, TEXT(""));
		goto FREE_EXIT_LABEL;
	}
	if (sectorsRead != descSectors)
	{
		Status = E_INVALIDARG;
		D_API_ERR(0, TEXT("ReadLogSectors"), Status, TEXT("%d, %d"), sectorsRead, descSectors);
		goto FREE_EXIT_LABEL;
	}

	for (ii = 0; ii < logEntryHeader.m_DescriptorCount; ii++)
	{
		if (VerifyLogDesc(&descEntries->m_LogDescriptors[ii], &logEntryHeader) == FALSE)
		{
			Status = E_INVALIDARG;
			D_API_ERR(0, TEXT("VerifyLogDesc"), Status, TEXT(""));
			goto FREE_EXIT_LABEL;
		}
	}

	*ppLogDescEntries = descEntries;
	goto EXIT_LABEL;

FREE_EXIT_LABEL:
	VFREE(descEntries);
EXIT_LABEL:
	return Status;
}

INT VHDXLogReplay_NS::CVHDXLogReplay::ReadLogSectors(BackendFile *pBackendFile, LogEntries *pLogEntries, UINT32 *sectorsRead, BYTE *Buffer, UINT32 numberSectors, BOOL bToPeek)
{
	INT Status = 0;
	UINT64 Offset;
	UINT32 readLen;

	readLen = pLogEntries->m_Read;

	*sectorsRead = 0;
	while (numberSectors)
	{
		if (readLen == pLogEntries->m_Write)
		{
			break;
		}
		Offset = pLogEntries->m_Offset + readLen;

		Status = BackendFileRead(pBackendFile, Offset, Buffer, VHDX_LOG_SECTOR_SIZE);
		if (Status < 0)
		{
			D_API_ERR(0, TEXT("FileRead"), Status, TEXT(""));
			goto EXIT_LABEL;
		}
		readLen = IncLogIndex(readLen, pLogEntries->m_Length);

		*sectorsRead = *sectorsRead + 1;
		numberSectors--;
	}

EXIT_LABEL:
	if (!bToPeek)
	{
		pLogEntries->m_Read = readLen;
	}
	return Status;
}

INT VHDXLogReplay_NS::CVHDXLogReplay::FlushLog(BackendFile *pBackendFile, VDiskContext *pVDiskContext, LogSequence *pLogSeq)
{
	INT Status = 0;
	INT ii;
	UINT32 Count, sectorsRead;
	UINT64 newFileSize;
	BYTE *data = NULL;
	LogDescEntries *pLogDescEntries = NULL;
	LogEntryHeader LogEntryHeaderTemp = { 0 };

	Count = pLogSeq->m_Count;

	data = (BYTE *)VALLOC(pBackendFile, VHDX_LOG_SECTOR_SIZE);

	Status = VDiskWriteOnce(pBackendFile, pVDiskContext);
	if (Status < 0)
	{
		D_API_ERR(0, TEXT("VDiskWriteOnce"), Status, TEXT(""));
		goto exit;
	}


	while (Count--)
	{
		Status = ReadLogEntryHeader(pBackendFile, &pLogSeq->m_LogEntries, &LogEntryHeaderTemp);
		if (Status < 0)
		{
			D_API_ERR(0, TEXT("ReadLogEntryHeader"), Status, TEXT(""));
			goto exit;
		}

		if (LogEntryHeaderTemp.m_FlushedFileOffset > BackendFileGetSize(pBackendFile))
		{
			Status = E_INVALIDARG;
			D_API_ERR(0, TEXT("m_FlushedFileOffset"), Status, TEXT("%I64d"), LogEntryHeaderTemp.m_FlushedFileOffset);
			goto exit;
		}

		Status = ReadLogDesc(pBackendFile, pVDiskContext, &pLogSeq->m_LogEntries, &pLogDescEntries);
		if (Status < 0)
		{
			D_API_ERR(0, TEXT("ReadLogDesc"), Status, TEXT(""));
			goto exit;
		}

		for (ii = 0; ii < pLogDescEntries->m_LogEntryHeader.m_DescriptorCount; ii++)
		{
			if (!memcmp(&pLogDescEntries->m_LogDescriptors[ii].m_Signature, SIG_DESC, sizeof(SIG_DESC)-1))
			{
				Status = ReadLogSectors(pBackendFile, &pLogSeq->m_LogEntries, &sectorsRead, data, 1, FALSE);
				if (Status < 0)
				{
					D_API_ERR(0, TEXT("ReadLogSectors"), Status, TEXT(""));
					goto exit;
				}
				if (sectorsRead != 1)
				{
					Status = E_INVALIDARG;
					D_API_ERR(0, TEXT("ReadLogSectors"), Status, TEXT("%d"), sectorsRead);
					goto exit;
				}
			}

			Status = FlushLogDesc(pBackendFile, &pLogDescEntries->m_LogDescriptors[ii], (LogDataSector *)data);
			if (Status < 0)
			{
				D_API_ERR(0, TEXT("FlushLogDesc"), Status, TEXT(""));
				goto exit;
			}
		}

		if (BackendFileGetSize(pBackendFile) < pLogDescEntries->m_LogEntryHeader.m_LastFileOffset)
		{
			//to do... FileGetSize(...) and FileResize(..) should operate on new file or original file?
			newFileSize = pLogDescEntries->m_LogEntryHeader.m_LastFileOffset;
			if (newFileSize % (1024 * 1024))
			{
				newFileSize = ((newFileSize >> 20) + 1) << 20;
				BackendFileResize(pBackendFile, newFileSize);
			}
		}
		VFREE(pLogDescEntries);
		pLogDescEntries = NULL;
	}

	BackendFileFlush(pBackendFile);
	ResetLog(pBackendFile, pVDiskContext);
	BackendFileFlush(pBackendFile);

exit:
	VFREE(data);
	VFREE(pLogDescEntries);
	return Status;
}

INT VHDXLogReplay_NS::CVHDXLogReplay::VDiskWriteOnce(BackendFile *pBackendFile, VDiskContext *pVDiskContext)
{
	INT Status = 0;
	if (pVDiskContext->m_bWriteOnce)
	{
		pVDiskContext->m_bWriteOnce = FALSE;
		Status = VDiskUpdateHeaders(pBackendFile, pVDiskContext, TRUE, NULL);
	}
	return Status;
}

INT VHDXLogReplay_NS::CVHDXLogReplay::VDiskUpdateHeaders(BackendFile *pBackendFile, VDiskContext *pVDiskContext, BOOL bGenGuid, GUID *logGuid)
{
	INT Status;

	Status = VDiskUpdateHeader(pBackendFile, pVDiskContext, bGenGuid, logGuid);
	if (Status < 0)
	{
		return Status;
	}
	Status = VDiskUpdateHeader(pBackendFile, pVDiskContext, bGenGuid, logGuid);
	return Status;
}

INT VHDXLogReplay_NS::CVHDXLogReplay::VDiskUpdateHeader(BackendFile *pBackendFile, VDiskContext *pVDiskContext, BOOL bGenGuid, GUID *logGuid)
{
	INT Status = 0;
	INT hdrIndex = 0;
	UINT64 headerOffset = VHDX_HEADER1_OFFSET;

	VDiskHeader *activeHeader;
	VDiskHeader *inactiveHeader;



	if (pVDiskContext->m_CurrentHeaderIndex == 0)
	{
		hdrIndex = 1;
		headerOffset = VHDX_HEADER2_OFFSET;
	}

	activeHeader = pVDiskContext->m_Headers[pVDiskContext->m_CurrentHeaderIndex];
	inactiveHeader = pVDiskContext->m_Headers[hdrIndex];

	inactiveHeader->m_SequenceNumber = activeHeader->m_SequenceNumber + 1;


	inactiveHeader->m_FileWriteGuid = pVDiskContext->m_NewGuid;

	if (bGenGuid)
	{
		GenerateGUID(&inactiveHeader->m_DataWriteGuid);
	}

	if (logGuid)
	{
		inactiveHeader->m_LogGuid = *logGuid;
	}

	Status = VDiskWriteHeader(pBackendFile, inactiveHeader, headerOffset, TRUE);
	if (Status < 0)
	{
		D_API_ERR(0, TEXT("VDiskWriteHeader"), Status, TEXT(""));
		goto EXIT_LABEL;
	}
	pVDiskContext->m_CurrentHeaderIndex = hdrIndex;

EXIT_LABEL:
	return Status;
}

INT VHDXLogReplay_NS::CVHDXLogReplay::VDiskWriteHeader(BackendFile *pBackendFile, VDiskHeader *pVDiskHeader, UINT64 offset, BOOL bToRead)
{
	UINT8 *buffer = NULL;
	INT Status;


	ATLASSERT(pBackendFile != NULL);
	ATLASSERT(pVDiskHeader != NULL);


	buffer = (UINT8 *)VALLOC(pBackendFile, VHDX_HEADER_SIZE);
	if (bToRead)
	{
		Status = BackendFileRead(pBackendFile, offset, buffer, VHDX_HEADER_SIZE);
		if (Status < 0)
		{
			D_API_ERR(0, TEXT("FileRead"), Status, TEXT(""));
			goto EXIT_LABEL;
		}
	}
	else
	{
		memset(buffer, 0, VHDX_HEADER_SIZE);
	}

	memcpy(buffer, pVDiskHeader, sizeof(VDiskHeader));
	pVDiskHeader->m_Checksum = UpdateChecksum(buffer, VHDX_HEADER_SIZE, offsetof(VDiskHeader, m_Checksum));
	Status = BackendFileWrite(pBackendFile, offset, (CONST BYTE*)pVDiskHeader, sizeof(VDiskHeader));

EXIT_LABEL:
	VFREE(buffer);
	return Status;
}

UINT32 VHDXLogReplay_NS::CVHDXLogReplay::UpdateChecksum(UINT8 *buf, size_t Size, INT crcOffset)
{
	UINT32 crcVal;

	ATLASSERT(buf != NULL);
	ATLASSERT(Size > (crcOffset + sizeof(crcVal)));

	memset(buf + crcOffset, 0, sizeof(crcVal));
	crcVal = crc32c(0xffffffff, buf, Size);
	memcpy(buf + crcOffset, &crcVal, sizeof(crcVal));

	return crcVal;
}

INT VHDXLogReplay_NS::CVHDXLogReplay::FlushLogDesc(BackendFile *pBackendFile, LogDescriptor *pLogDescriptor, LogDataSector *pLogDataSector)
{
	INT Status = 0;
	UINT64 seqNumber, fileOffset;
	UINT32 offset = 0;
	BYTE * Buffer = NULL;
	UINT64 descCount = 1;
	BOOL bZeroedData = FALSE; //<sonmi01>2014-3-1 ###???
	INT ii;

	Buffer = (BYTE *)VALLOC(pBackendFile, VHDX_LOG_SECTOR_SIZE);

	if (!memcmp(&pLogDescriptor->m_Signature, SIG_DESC, sizeof(SIG_DESC)-1))
	{
		if (pLogDataSector == NULL)
		{
			Status = E_FAIL;
			D_API_ERR(0, TEXT("pLogDataSector"), Status, TEXT("NULL"));
			goto EXIT_LABEL;
		}

		seqNumber = pLogDataSector->m_sSequenceHigh;
		seqNumber <<= 32;
		seqNumber |= pLogDataSector->m_SequenceLow & 0xffffffff;

		if (seqNumber != pLogDescriptor->m_SequenceNumber)
		{
			Status = E_INVALIDARG;
			D_API_ERR(0, TEXT("m_SequenceNumber"), Status, TEXT("%I64d, %I64d"), seqNumber, pLogDescriptor->m_SequenceNumber);
			goto EXIT_LABEL;
		}

		memcpy(Buffer, &pLogDescriptor->m_LeadingBytes, 8);
		offset += 8;

		memcpy(Buffer + offset, pLogDataSector->m_Data, 4084);
		offset += 4084;

		memcpy(Buffer + offset, &pLogDescriptor->m_TrailingBytes, 4);

	}
	else if (!memcmp(&pLogDescriptor->m_Signature, SIG_ZERO, sizeof(SIG_ZERO)-1))
	{
		bZeroedData = TRUE;
		memset(Buffer, 0, VHDX_LOG_SECTOR_SIZE);
		descCount = pLogDescriptor->m_ZeroLength / VHDX_LOG_SECTOR_SIZE;
	}

	fileOffset = pLogDescriptor->m_FileOffset;

	if (pBackendFile->m_pCallback) //<sonmi01>2014-3-1 ###???
	{
		HRESULT hr = pBackendFile->m_pCallback->OnReplayedData(
			fileOffset,//ULONGLONG FileOffset,
			bZeroedData ? pLogDescriptor->m_ZeroLength : VHDX_LOG_SECTOR_SIZE,//ULONGLONG Length,
			Buffer,//CONST BYTE * pBuffer,
			bZeroedData//BOOL bZero
			);
		if (FAILED(hr))
		{
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT(""), ...);
			D_API_ERR(0, TEXT("pCallback->OnReplayedData"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}

		if (fileOffset >= pBackendFile->m_pCallback->GetPayloadBlockSartOffset())
		{
			goto EXIT_LABEL; //sector bitmap is saved in memory
		}
	}

	for (ii = 0; ii < descCount; ii++)
	{
		Status = BackendFileWrite(pBackendFile, fileOffset, Buffer, VHDX_LOG_SECTOR_SIZE);
		if (Status < 0)
		{
			D_API_ERR(0, TEXT("FileWrite"), Status, TEXT(""));
			goto EXIT_LABEL;
		}
		fileOffset += VHDX_LOG_SECTOR_SIZE;
	}

EXIT_LABEL:
	VFREE(Buffer);
	return Status;
}

VOID VHDXLogReplay_NS::CVHDXLogReplay::ResetLog(BackendFile *pBackendFile, VDiskContext *pVDiskContext)
{
	GUID guid = { 0 };
	pVDiskContext->m_LogEntries.m_Read = pVDiskContext->m_LogEntries.m_Write = 0;
	VDiskUpdateHeaders(pBackendFile, pVDiskContext, FALSE, &guid);
}

BOOL VHDXLogReplay_NS::CVHDXLogReplay::VerifyLogDesc(LogDescriptor *pLogDescriptor, LogEntryHeader *pLogEntryHeader)
{
	BOOL Status = FALSE;

	if (pLogDescriptor->m_SequenceNumber != pLogEntryHeader->m_SequenceNumber)
	{
		D_API_ERR(0, TEXT("m_SequenceNumber"), Status, TEXT("%I64d, %I64d"), pLogDescriptor->m_SequenceNumber, pLogEntryHeader->m_SequenceNumber);
		goto EXIT_LABEL;
	}

	if (pLogDescriptor->m_FileOffset % VHDX_LOG_SECTOR_SIZE)
	{
		D_API_ERR(0, TEXT("m_FileOffset"), Status, TEXT("%I64d"), pLogDescriptor->m_FileOffset);
		goto EXIT_LABEL;
	}

	if (!memcmp(&pLogDescriptor->m_Signature, SIG_ZERO, sizeof(SIG_ZERO)-1))
	{
		if (pLogDescriptor->m_ZeroLength % VHDX_LOG_SECTOR_SIZE == 0)
		{
			Status = TRUE;
		}
	}
	else if (!memcmp(&pLogDescriptor->m_Signature, SIG_DESC, sizeof(SIG_DESC)-1))
	{
		Status = TRUE;
	}

EXIT_LABEL:
	return Status;
}

VOID VHDXLogReplay_NS::CVHDXLogReplay::CloseReplayLog(VDiskContext *pVDiskContext)
{
	VFREE(pVDiskContext->m_Headers[0]);
	pVDiskContext->m_Headers[0] = NULL;
	VFREE(pVDiskContext->m_Headers[1]);
	pVDiskContext->m_Headers[1] = NULL;
	VFREE(pVDiskContext->m_LogEntries.m_LogEntryHeader);
	pVDiskContext->m_LogEntries.m_LogEntryHeader = NULL;
}

INT VHDXLogReplay_NS::CVHDXLogReplay::IncLogIndex(UINT32 Index, UINT64 Length)
{
	Index += VHDX_LOG_SECTOR_SIZE;
	return Index >= Length ? 0 : Index;
}
