#pragma once


#include "..\PublicHeader\PublicFileDeviceInterface.h"

#include "VhdxHeader.h"
#include "VHDXRegionTableEntry.h"
#include "VHDXMetadataTableEntry.h"
#include "VHDXBATEntry.h"

#include "Helper.h"

//selected metadata for use
struct ParserVHDXMetadata
{
	//////////////////////////////////////////////////////////////////////////
	BYTE										m_VhdxHeaderBuffer[FIELD_OFFSET(VhdxHeader, m_Reserved)]; /***INTERNAL***/
	VhdxHeader		*							m_pVhdxHeader; /***INTERNAL***/ //always point to m_VhdxHeaderBuffer for saving memory; do not access m_Reserved

	VHDXRegionTableEntry						m_VHDXRegionTableEntryBAT; /***INTERNAL***/

	//////////////////////////////////////////////////////////////////////////
	VHDX_FILE_PARAMETERS						m_VHDX_FILE_PARAMETERS; /***INTERNAL***/
	VHDX_VIRTUAL_DISK_SIZE						m_VHDX_VIRTUAL_DISK_SIZE; /***INTERNAL***/
	VHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE		m_VHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE; /***INTERNAL***/
	VHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE		m_VHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE; /***INTERNAL***/
	map<CString, CString>						m_VHDX_PARENT_LOCATOR_ENTRIES; /***INTERNAL***/

	//////////////////////////////////////////////////////////////////////////
	LONG										m_ChunkRatio;
	LONG										m_DataBlocksCount;
	LONG										m_BitmapBlocksCount;
	LONG										m_BatEntriesCount;
	CVMemPtr<VHDX_BAT_ENTRY>					m_apBatTable; /***INTERNAL***/



	//////////////////////////////////////////////////////////////////////////
	IFileDevice *								m_pIFileDevice; /***INCOMMING***/

	//////////////////////////////////////////////////////////////////////////
	ParserVHDXMetadata(IFileDevice * pIFileDevice);
	HRESULT Parse();

private:
	static int ParseMetadataEntry(LPBYTE pMetadataStart,  VHDXMetadata & Metadata, VHDXMetadataTableEntry * pVHDXMetadataTableEntry, ULONG EntryCount, map<CString, CString> & ParentPath);
	HRESULT ParseMetadata(LONGLONG MetadataOffset);
	HRESULT ParseBatTable();
	LONG BlockIndexToBatIndex(LONG BlockIndex);
};