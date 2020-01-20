#pragma once


#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicVMemPtr.h"
#include "..\PublicHeader\PublicMiscHelper.h"
#include "..\PublicHeader\PublicSimpleBitArray.h"

#include "VhdxFileIdentifier.h"
#include "VhdxHeader.h"
#include "VHDXRegionTableHeader.h"
#include "VHDXRegionTableEntry.h"
#include "VHDXMetadataTableEntry.h"
#include "VHDXBATEntry.h"

#include "CacheIO.h"

#include "VhdxHelper.h"
#include "VHDXLogReplayCallback.h" //<sonmi01>2014-3-1 ###???


//selected metadata for use
struct ParserSeparateVHDX
{
	enum EBlockState
	{
		EBS_UNKNOWN,
		EBS_NOT_PRESENT,
		EBS_ALL_ZERO,
		EBS_ALLOCATED
	};


	//////////////////////////////////////////////////////////////////////////
	BYTE										m_VhdxHeaderBuffer[FIELD_OFFSET(VhdxHeader, m_Reserved)]; /***INTERNAL***/
	VhdxHeader		*							m_pVhdxHeader; /***INTERNAL***/ //always point to m_VhdxHeaderBuffer for saving memory; do not access m_Reserved

	VHDXRegionTableEntry						m_VHDXRegionEntryBAT; /***INTERNAL***/

	//////////////////////////////////////////////////////////////////////////
	VHDX_FILE_PARAMETERS						m_FileParameters; /***INTERNAL***/
	VHDX_VIRTUAL_DISK_SIZE						m_VirtualDiskSize; /***INTERNAL***/
	VHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE		m_LogicalSectorSize; /***INTERNAL***/
	VHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE		m_PhysicalSectorSize; /***INTERNAL***/
	VHDX_PAGE83_DATA							m_VHDX_PAGE83_DATA; /***INTERNAL***/ //<sonmi01>2015-6-3 ###???
	map<CString, CString>						m_ParentLocatorEntries; /***INTERNAL***/

	//////////////////////////////////////////////////////////////////////////
	LONG										m_ChunkRatio;
	LONG										m_DataBlocksCount;
	LONG										m_BitmapBlocksCount;
	LONG										m_BatEntriesCount;
	LONG										m_BlockBitmapBufferSize;

	static CONST LONG							MAX_BAT_TABLE_LEN = 5 * 1024 * 1024;
	CVMemPtr<VHDX_BAT_ENTRY>					m_apBatTable; /***INTERNAL***/
	CCacheIO									m_CacheIoBatTable; /***INTERNAL***/ //<sonmi01>2013-9-11 ###???

	CCacheIO									m_CacheIoBitmap;

	LONGLONG									m_AllocPosition; //for write to allocate a new block
	BOOL										m_NeedToUpdateBAT;

	//////////////////////////////////////////////////////////////////////////
	IFileDevice *								m_pIFileDevice; /***INCOMMING***//***INTERNAL***//***TO CLEANUP***/


	//<sonmi01>2014-1-9 ###???
	IFileDevice *								m_pDumpedBatTalbeFile; //<sonmi01>2014-1-9 ###???
	CCacheIO	*								m_pCacheIoDumpedBatTalbe;


	//////////////////////////////////////////////////////////////////////////
	CString										m_OriginalVhdxFile; /***INCOMMING***/ //<sonmi01>2014-2-24 ###???
	CString										m_ReplayedLogFolder; /***INCOMMING***/
	CString										m_ReplayedLogFileName; /***INTERNAL***/
	IFileDevice				*					m_ReplayedLogFileDevice; /***INTERNAL***/ /***TO CLEANUP***/
	IVHDXLogReplayCallback	*					m_pLogReplayCallback; /***TO CLEANUP***/ //<sonmi01>2014-3-1 ###??? 


	//////////////////////////////////////////////////////////////////////////
	ParserSeparateVHDX(IFileDevice * pIFileDevice);
	virtual ~ParserSeparateVHDX();
	HRESULT Parse();
	HRESULT ParseCreateNew(LONGLONG VirtualDiskSize, LONG LogicalSectorSize, LONG PhysicalSectorSize, LONG BlockSize, CONST map<CString, CString> & ParentLocatorEntries, CONST GUID & page83Id, LONG VhdxType); //2018-3-23 //!!@@## //<sonmi01>2014-4-21 ###???

	HRESULT ReadBlocK(IN BOOL bFillZero, IN LONG BlockIndex, OUT BYTE * pBuffer, IN LONG BufferLen);
	HRESULT ReadBlocKBitmap(LONG BlockIndex, IN OUT LONG Len, BYTE * Buffer); //one bit represent one sector (not one block)
	
	BOOL	NeedToInspectParentBlock(LONG BlockIndex);

	LONGLONG GetPhysicalAddress(LONGLONG LogicalAddress); //<sonmi01>2013-10-29 ###???;

	HRESULT WriteBlocK(IN LONG BlockIndex, IN CONST BYTE * pBuffer, IN LONG BufferLen);
	HRESULT UpdateBAT();
	//<sonmi01>2014-1-9 #dump bat table
	HRESULT DumpBatTable(IFileDevice * pIFileDeviceOut);
	HRESULT DumpBatTable(LPCTSTR FileName);

	HRESULT OpenDumpedBatTable(LPCTSTR FileName);
	HRESULT ReadDumpedBatEntry(LONG BlockIndex, VHDX_BAT_ENTRY & BatEntry);
	HRESULT CloseDumpedBatTable();

	//<sonmi01>2014-1-13 ###???
	HRESULT CompareWithDumpedBatForZeroedData(LONG BlockIndex, BOOL & bResult);
	HRESULT ReadBatEntry(LONG BatIndex, VHDX_BAT_ENTRY & batEntry);

	//<sonmi01>2014-9-24 ###???
	//one bit represent one block (not one sector)
	HRESULT GetBlocksBitmap(LONG & BlockSize, LONG & BlockCount, BYTE ** ppBuffer);
	static HRESULT FreeBlocksBitmapBuffer(BYTE * pBuffer);


	//<sonmi01>2015-2-15 ###???
	EBlockState GetBlocKState(LONGLONG LogicalAddrress);


private:
	static int ParseMetadataEntry(LPBYTE pMetadataStart, VHDXMetadata & Metadata, VHDXMetadataTableEntry * pVHDXMetadataTableEntry, ULONG EntryCount, map<CString, CString> & ParentPath);
	HRESULT ParseMetadata(LONGLONG MetadataOffset, LONG Length);
	HRESULT ParseBatTable();
	LONG BlockIndexToBatIndex(LONG BlockIndex);
	LONG BitmapBlockIndexToBatIndex(LONG BitmapBlockIndex);
	LONGLONG GetBlockBitmapFileOffset(LONG BlockIndex);

	VOID SetMetadataAndCalcBatTableCreateNew(LONGLONG VirtualDiskSize, LONG LogicalSectorSize, LONG PhysicalSectorSize, LONG BlockSize, CONST map<CString, CString> & ParentLocatorEntries, LONG VhdxType); //<sonmi01>2014-4-21 ###???
	HRESULT ParseMetadataCreateNew(LONGLONG MetadataOffset);
	VOID ParseMetadataEntryCreateNew(LPBYTE pMetadataStart, VHDXMetadataTableEntry * pVHDXMetadataTableEntry, ULONG EntryCount);
	HRESULT ParseBatTableCreateNew();

	HRESULT WriteBatEntry(LONG BatIndex, CONST VHDX_BAT_ENTRY & batEntry);

	HRESULT CopyNewReplayedLog(LONGLONG CopyLen);
	HRESULT ReplayLog();

	HRESULT UpdateFixedBat(LONGLONG StartOffset);

	HRESULT IsBolockContainsData(LONG BlockIndex, BOOL & bResult); //<sonmi01>2014-9-24 ###???
	HRESULT FindDataBlock(LONG StartBlockIndex, LONG & First);
	HRESULT FindNonDataBlock(LONG StartBlockIndex, LONG & Last);
	HRESULT FindDataBLockRange(LONG StartBlockIndex, LONG & First, LONG & Last);
	

private:
	static HRESULT GenerateReplayedLogFile(CONST CString & OriginalVhdxFile, CONST CString & ReplayedLogFolder, CString & LogFileName, IFileDevice ** ppIFileDevice);
	static VOID ReplayPayloadBlock(LONGLONG BlockOffset, LONG BlockLen, BYTE * pBuffer, IVHDXLogReplayCallback * pCallBack);
	static VOID ReplayPayloadBlockAll(LONGLONG BlockOffset, LONG BlockLen, BYTE * pBuffer, CONST vector<TReplayedData> & replayedData); //<sonmi01>2014-3-1 ###???;
	static VOID ReplayPayloadBlockSingle(LONGLONG BlockOffset, LONG BlockLen, BYTE * pBuffer, CONST TReplayedData & replayedData); //<sonmi01>2014-3-1 ###???;
};