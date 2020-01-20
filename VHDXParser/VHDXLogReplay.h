#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "crc32.h"
#include "VHDXLogReplayCallback.h" //<sonmi01>2014-3-1 ###???

namespace VHDXLogReplay_NS
{

	//////////////////////////////////////////////////////////////////////////
#define VALLOC(notUsed, nBytes)				VirtualAlloc( NULL, nBytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)
#define VFREE(pBuffer)						VirtualFree(pBuffer, 0, MEM_RELEASE )

	//////////////////////////////////////////////////////////////////////////
	static CONST CHAR SIG_VHDXFILE[] = "vhdxfile";
	static CONST CHAR SIG_HEAD[] = "head";
	static CONST CHAR SIG_LOGE[] = "loge";
	static CONST CHAR SIG_DESC[] = "desc";
	static CONST CHAR SIG_ZERO[] = "zero";

	//////////////////////////////////////////////////////////////////////////
#include <pshpack1.h>
	//////////////////////////////////////////////////////////////////////////
	CONST ULONG VHDX_HEADER_BLOCK_SIZE = 64 * 1024;
	CONST ULONG VHDX_HEADER_SIZE = 4 * 1024;
	CONST ULONG VHDX_HEADER1_OFFSET = VHDX_HEADER_BLOCK_SIZE;
	CONST ULONG VHDX_HEADER2_OFFSET = VHDX_HEADER_BLOCK_SIZE + VHDX_HEADER_BLOCK_SIZE;
	CONST ULONG VHDX_HEADER_SIGNATURE = 0x64616568;

	struct VDiskHeader 
	{
		UINT32    m_Signature; 
		UINT32    m_Checksum; 
		UINT64    m_SequenceNumber;
		GUID      m_FileWriteGuid;
		GUID      m_DataWriteGuid; 
		GUID      m_LogGuid;  
		UINT16    m_LogVersion; 
		UINT16    m_Version; 
		UINT32    m_LogLength;
		UINT64    m_LogOffset; 
	};

	//////////////////////////////////////////////////////////////////////////
	CONST ULONG  VHDX_LOG_MIN_SIZE = 1024 * 1024;
	CONST ULONG  VHDX_LOG_SECTOR_SIZE = 4096;
	CONST ULONG  VHDX_LOG_HDR_SIZE = 64;
	CONST ULONG  VHDX_LOG_SIGNATURE = 0x65676f6c;
	struct LogEntryHeader 
	{
		UINT32    m_Signature;          
		UINT32    m_Checksum;           
		UINT32    m_EntryLength;        
		UINT32    m_Tail;               
		UINT64    m_SequenceNumber;     
		UINT32    m_DescriptorCount;    
		UINT32    m_Reserved;
		GUID      m_LogGuid;            
		UINT64    m_FlushedFileOffset;  
		UINT64    m_LastFileOffset;     
	};

	//////////////////////////////////////////////////////////////////////////
	CONST ULONG  VHDX_LOG_DESC_SIZE = 32;
	CONST ULONG  VHDX_LOG_DESC_SIGNATURE = 0x63736564;
	CONST ULONG  VHDX_LOG_ZERO_SIGNATURE = 0x6f72657a;
	struct LogDescriptor 
	{
		UINT32    m_Signature;             
		union  
		{
			UINT32    m_Reserved;          
			UINT32    m_TrailingBytes;    
		};
		union 
		{
			UINT64    m_ZeroLength;       
			UINT64    m_LeadingBytes;     
		};
		UINT64    m_FileOffset;           
		UINT64    m_SequenceNumber;       
	};

	//<sonmi01>2014-3-5 ###???
	struct LogDescEntries
	{
		LogEntryHeader	m_LogEntryHeader;
		LogDescriptor	m_LogDescriptors[1];
	};

	//////////////////////////////////////////////////////////////////////////
	CONST ULONG  VHDX_LOG_DATA_SIGNATURE = 0x61746164;
	struct LogDataSector 
	{
		UINT32    m_DataSignature;      
		UINT32    m_sSequenceHigh;       
		UINT8     m_Data[4084];          
		UINT32    m_SequenceLow;        
	} ;

#include <poppack.h> //end #include <pshpack1.h>

	//////////////////////////////////////////////////////////////////////////
	struct LogEntries 
	{
		UINT64				m_Offset;
		UINT64				m_Length;
		UINT32				m_Write;
		UINT32				m_Read;
		LogEntryHeader *	m_LogEntryHeader;
		LPVOID				m_DescBuffer;
		UINT64				m_Sequence;
		UINT32				m_Tail;
	};

	struct LogSequence 
	{
		BOOL			m_Valid;
		UINT32			m_Count;
		LogEntries		m_LogEntries;
		LogEntryHeader	m_LogEntryHeader;
	} ;

	//<sonmi01>2014-3-5 ###???
	//struct LogDescEntries 
	//{
	//	LogEntryHeader	m_LogEntryHeader;
	//	LogDescriptor	m_LogDescriptors[1];
	//} ;

	static CONST GUID NullGuid = { 0 };

	//////////////////////////////////////////////////////////////////////////
	struct BackendFile
	{
		IFileDevice *	m_BackendFile;
		LONGLONG		m_OriginalFileSize;
		IVHDXLogReplayCallback * m_pCallback; //<sonmi01>2014-3-1 ###???

		//////////////////////////////////////////////////////////////////////////
		BackendFile() :
			m_BackendFile(NULL),
			m_OriginalFileSize(-1LL),
			m_pCallback(NULL)
		{

		}
	};

	

	//////////////////////////////////////////////////////////////////////////
	struct VDiskContext 
	{
		INT					m_CurrentHeaderIndex;
		VDiskHeader *		m_Headers[2];
		BOOL				m_bWriteOnce;
		GUID				m_NewGuid;
		LogEntries			m_LogEntries;
		BOOL				m_bLogReplayed;
	};

	//////////////////////////////////////////////////////////////////////////
	struct  CVHDXLogReplay
	{
		//////////////////////////////////////////////////////////////////////////
		static INT ReplayLogEntry(
			IN BackendFile *pBackendFile,
			OUT VDiskContext *pVDiskContext,
			IN INT Flags,
			OUT VOID ** ppErrorReport
			);

	private:

		//////////////////////////////////////////////////////////////////////////
		static INT BackendFileRead(BackendFile * pBackendFile, LONGLONG Offset, BYTE* pBuffer, ULONG nNumberOfBytesToRead);
		static INT BackendFileWrite(BackendFile * pBackendFile, LONGLONG Offset, CONST BYTE* pBuffer, ULONG nNumberOfBytesToWrite);
		static LONGLONG BackendFileGetSize(BackendFile * pBackendFile);
		static INT BackendFileResize(BackendFile * pBackendFile, LONGLONG newFileSize);
		static INT BackendFileFlush(BackendFile *pBackendFile);


		//////////////////////////////////////////////////////////////////////////
		static INT ParseVDiskHeader(BackendFile *pBackendFile, VDiskContext *pVDiskContext);

		//////////////////////////////////////////////////////////////////////////
		static VOID GenerateGUID(GUID *guid);
		static BOOL GuidEqual(CONST GUID & a, CONST GUID & b);
				
		//////////////////////////////////////////////////////////////////////////
		static BOOL VerifyChecksum(UINT8 *Buffer, size_t Size, INT crcOffset);
		static UINT32 CalcChecksum(UINT32 crc, UINT8 *Buffer, size_t Size,	INT crcOffset);
		static UINT32 UpdateChecksum(UINT8 *Buffer, size_t Size, INT crcOffset);

		//////////////////////////////////////////////////////////////////////////
		static INT SearchActiveSeq(BackendFile *pBackendFile, VDiskContext *pVDiskContext, LogSequence *LogSeq);
		static INT ReplayLog(BackendFile *pBackendFile, VDiskContext *pVDiskContext, BOOL *bLogReplayed, VOID **ErrorReport);
		static INT FlushLog(
			BackendFile *pBackendFile,
			VDiskContext *pVDiskContext,
			LogSequence *pLogSeq);
		static VOID ResetLog(BackendFile *pBackendFile, VDiskContext *pVDiskContext);
		static VOID CloseReplayLog(VDiskContext *pVDiskContext);

		//////////////////////////////////////////////////////////////////////////
		static INT FlushLogDesc(BackendFile *pBackendFile, LogDescriptor *pLogDescriptor, LogDataSector *pLogDataSector);
		static BOOL VerifyLogDesc(LogDescriptor *pLogDescriptor, LogEntryHeader *pLogEntryHeader);

		//////////////////////////////////////////////////////////////////////////
		static INT VerifyLogEntry(
			BackendFile *pBackendFile,
			VDiskContext *pVDiskContext,
			LogEntries *pLogEntries,
			UINT64 seqNum,
			BOOL *bValid,
			LogEntryHeader *pLogEntryHeader);
		static INT ReadLogEntryHeader(BackendFile *pBackendFile, LogEntries *pLogEntries, LogEntryHeader *pLogEntryHeader);
		static BOOL VerifyLogHeader(LogEntries *pLogEntries, LogEntryHeader *pLogEntryHeader, VDiskContext *pVDiskContext);
		static INT CalcDescSectors(UINT32 desc_cnt);
		static INT ReadLogDesc(
			BackendFile *pBackendFile,
			VDiskContext *pVDiskContext,
			LogEntries *pLogEntries,
			LogDescEntries **ppLogDescEntries);


		static INT ReadLogSectors(
			BackendFile *pBackendFile,
			LogEntries *pLogEntries,
			UINT32 *sectors_read,
			BYTE *Buffer,
			UINT32 num_sectors,
			BOOL peek);

		static INT IncLogIndex(UINT32 idx, UINT64 length);


		//////////////////////////////////////////////////////////////////////////
		static INT VDiskWriteOnce(BackendFile *pBackendFile, VDiskContext *pVDiskContext);
		static INT VDiskUpdateHeaders(BackendFile *pBackeFileGetSizendFile, VDiskContext *pVDiskContext, BOOL bGenGuid, GUID *logGuid);
		static INT VDiskUpdateHeader(BackendFile *pBackendFile, VDiskContext *pVDiskContext, BOOL bGenGuid, GUID *logGuid);
		static INT VDiskWriteHeader(BackendFile *pBackendFile, VDiskHeader *pVDiskHeader, UINT64 offset, BOOL read);

		//////////////////////////////////////////////////////////////////////////
	};

}; //end VHDXLogReplay_NS

//////////////////////////////////////////////////////////////////////////
//API
HRESULT VhdxReplayLogAPI(IFileDevice * pIFileDeviceVHDX, LONGLONG OriginalVhdxFileSize, IN IVHDXLogReplayCallback *pCallBack); //<sonmi01>2014-3-1 ###??? //<sonmi01>2014-2-25 ###???
HRESULT VhdxReplayLog(LPCTSTR pVHDX, LONGLONG OriginalVhdxFileSize, IN IVHDXLogReplayCallback *pCallBack);