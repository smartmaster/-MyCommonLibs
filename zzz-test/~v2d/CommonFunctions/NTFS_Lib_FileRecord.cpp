#include "stdafx.h"
#include "NTFS_Lib.h"
//#include "NTFS_Lib_Attribute.h"

CFileRecord::CFileRecord(CONST CNTFSVolume *volume)
{
	_ASSERT(volume);
	m_Volume = volume;
	m_FileRecord = NULL;
	m_FileReference = (ULONGLONG)-1;

	ClearAttrRawCallBack();

	// Default to parse all attributes
	m_AttrMask = MASK_ALL;
}

CFileRecord::~CFileRecord()
{
	ClearAttrs();

	if (m_FileRecord)
		delete m_FileRecord;
}

// Free all CAttr_xxx
void CFileRecord::ClearAttrs()
{
	for (INT i=0; i<ATTR_NUMS; i++)
	{
		m_AttrList[i].RemoveAll();
	}
}

// Verify US and update sectors
BOOL CFileRecord::PatchUS(WORD *sector, INT sectors, WORD usn, WORD *usarray)
{
	INT i;

	for (i=0; i<sectors; i++)
	{
		sector += ((m_Volume->m_SectorSize>>1) - 1);
		if (*sector != usn)
			return FALSE;	// USN error
		*sector = usarray[i];	// Write back correct data
		sector++;
	}
	return TRUE;
}

// Call user defined Callback routines for an attribute
void CFileRecord::UserCallBack(DWORD attType, ATTR_HEADER_COMMON *ahc, BOOL *bDiscard)
{
	*bDiscard = FALSE;

	if (m_AttrRawCallBack[attType])
		m_AttrRawCallBack[attType](ahc, bDiscard);
	else if (m_Volume->m_AttrRawCallBack[attType])
		m_Volume->m_AttrRawCallBack[attType](ahc, bDiscard);
}

CAttrBase* CFileRecord::AllocAttr(ATTR_HEADER_COMMON *ahc, BOOL *bUnhandled)
{
	switch (ahc->m_Type)
	{
	case ATTR_TYPE_STANDARD_INFORMATION:
		return new CAttr_StdInfo(ahc, this);

	case ATTR_TYPE_ATTRIBUTE_LIST:
		if (ahc->m_NonResident)
			return new CAttr_AttrList<CAttrNonResident>(ahc, this);
		else
			return new CAttr_AttrList<CAttrResident>(ahc, this);

	case ATTR_TYPE_FILE_NAME:
		return new CAttr_FileName(ahc, this);

	case ATTR_TYPE_VOLUME_NAME:
		return new CAttr_VolName(ahc, this);

	case ATTR_TYPE_VOLUME_INFORMATION:
		return new CAttr_VolInfo(ahc, this);

	case ATTR_TYPE_DATA:
		if (ahc->m_NonResident)
			return new CAttr_Data<CAttrNonResident>(ahc, this);
		else
			return new CAttr_Data<CAttrResident>(ahc, this);

	case ATTR_TYPE_INDEX_ROOT:
		return new CAttr_IndexRoot(ahc, this);

	case ATTR_TYPE_INDEX_ALLOCATION:
		return new CAttr_IndexAlloc(ahc, this);

	case ATTR_TYPE_BITMAP:
		if (ahc->m_NonResident)
			return new CAttr_Bitmap<CAttrNonResident>(ahc, this);
		else
			// Resident Bitmap may exist in a directory's FileRecord
			// or in $MFT for a very small volume in theory
			return new CAttr_Bitmap<CAttrResident>(ahc, this);

		// Unhandled Attributes
	default:
		*bUnhandled = TRUE;
		if (ahc->m_NonResident)
			return new CAttrNonResident(ahc, this);
		else
			return new CAttrResident(ahc, this);
	}
}

// Parse a single Attribute
// Return False on error
BOOL CFileRecord::ParseAttr(ATTR_HEADER_COMMON *ahc)
{
	DWORD attrIndex = ATTR_INDEX(ahc->m_Type);
	if (attrIndex < ATTR_NUMS)
	{
		BOOL bDiscard = FALSE;
		UserCallBack(attrIndex, ahc, &bDiscard);

		if (!bDiscard)
		{
			BOOL bUnhandled = FALSE;
			CAttrBase *attr = AllocAttr(ahc, &bUnhandled);
			if (attr)
			{
				if (bUnhandled)
				{
					DEBUG_TRACE1("Unhandled attribute: 0x%04X", ahc->m_Type);
				}
				m_AttrList[attrIndex].InsertEntry(attr);
				return TRUE;
			}
			else
			{
				DEBUG_TRACE1("Attribute Parse error: 0x%04X", ahc->m_Type);
				return FALSE;
			}
		}
		else
		{
			DEBUG_TRACE1("User processed this Attribute: 0x%04X", ahc->m_Type);
			return TRUE;
		}
	}
	else
	{
		DEBUG_TRACE1("Invalid Attribute Type: 0x%04X", ahc->m_Type);
		return FALSE;
	}
}

// Read File Record
FILE_RECORD_HEADER* CFileRecord::ReadFileRecord(ULONGLONG &fileRef)
{
	FILE_RECORD_HEADER *fr = NULL;
	DWORD len;

	if (fileRef < MFT_IDX_USER || m_Volume->m_MFTData == NULL)
	{
		// Take as continuous disk allocation
		LARGE_INTEGER frAddr;
		frAddr.QuadPart = m_Volume->m_MFTAddr + (m_Volume->m_FileRecordSize) * fileRef;
		LONGLONG llNewFilePointer = 0;
		HRESULT hr = m_Volume->m_pFileDeviceVolume->SetPointerEx( 
			frAddr.QuadPart,///* [in] */ LONGLONG llDistanceToMove,
			&llNewFilePointer,///* [out] */ LONGLONG *lpNewFilePointer,
			FILE_BEGIN///* [in] */ ULONG ulMoveMethod
			);

		if (FAILED(hr))
			return FALSE;
		else
		{
			fr = (FILE_RECORD_HEADER*)new BYTE[m_Volume->m_FileRecordSize];

			if (SUCCEEDED(m_Volume->m_pFileDeviceVolume->Read(m_Volume->m_FileRecordSize, &len, (BYTE*)fr))
				&& len==m_Volume->m_FileRecordSize)
				return fr;
			else
			{
				delete fr;
				return NULL;
			}
		}
	}
	else
	{
		// May be fragmented $MFT
		ULONGLONG frAddr;
		frAddr = (m_Volume->m_FileRecordSize) * fileRef;

		fr = (FILE_RECORD_HEADER*)new BYTE[m_Volume->m_FileRecordSize];

		if (m_Volume->m_MFTData->ReadData(frAddr, fr, m_Volume->m_FileRecordSize, &len)
			&& len == m_Volume->m_FileRecordSize)
			return fr;
		else
		{
			delete fr;
			return NULL;
		}
	}
}

// Read File Record, verify and patch the US (update sequence)
BOOL CFileRecord::ParseFileRecord(ULONGLONG fileRef)
{
	// Clear previous data
	ClearAttrs();
	if (m_FileRecord)
	{
		delete m_FileRecord;
		m_FileRecord = NULL;
	}

	FILE_RECORD_HEADER *fr = ReadFileRecord(fileRef);
	if (fr == NULL)
	{
		DEBUG_TRACE1("Cannot read file record %I64u", fileRef);

		m_FileReference = (ULONGLONG)-1;
	}
	else
	{
		m_FileReference = fileRef;

		if (fr->m_Magic == FILE_RECORD_MAGIC)
		{
			// Patch US
			WORD *usnaddr = (WORD*)((BYTE*)fr + fr->m_OffsetOfUS);
			WORD usn = *usnaddr;
			WORD *usarray = usnaddr + 1;
			if (PatchUS((WORD*)fr, m_Volume->m_FileRecordSize/m_Volume->m_SectorSize, usn, usarray))
			{
				DEBUG_TRACE1("File Record %I64u Found", fileRef);
				m_FileRecord = fr;

				return TRUE;
			}
			else
			{
				DEBUG_TRACE0("Update Sequence Number error");
			}
		}
		else
		{
			DEBUG_TRACE0("Invalid file record");
		}

		delete fr;
	}

	return FALSE;
}

// Visit IndexBlocks recursivly to find a specific FileName
BOOL CFileRecord::VisitIndexBlock(CONST ULONGLONG &vcn, CONST _TCHAR *fileName, CIndexEntry &ieFound) CONST
{
	CAttr_IndexAlloc *ia = (CAttr_IndexAlloc*)FindFirstAttr(ATTR_TYPE_INDEX_ALLOCATION);
	if (ia == NULL)
		return FALSE;

	CIndexBlock ib;
	if (ia->ParseIndexBlock(vcn, ib))
	{
		CIndexEntry *ie = ib.FindFirstEntry();
		while (ie)
		{
			if (ie->HasName())
			{
				// Compare name
				INT i = ie->Compare(fileName);
				if (i == 0)
				{
					ieFound = *ie;
					return TRUE;
				}
				else if (i < 0)		// fileName is smaller than IndexEntry
				{
					// Visit SubNode
					if (ie->IsSubNodePtr())
					{
						// Search in SubNode (IndexBlock), recursive call
						if (VisitIndexBlock(ie->GetSubNodeVCN(), fileName, ieFound))
							return TRUE;
					}
					else
						return FALSE;	// not found
				}
				// Just step forward if fileName is bigger than IndexEntry
			}
			else if (ie->IsSubNodePtr())
			{
				// Search in SubNode (IndexBlock), recursive call
				if (VisitIndexBlock(ie->GetSubNodeVCN(), fileName, ieFound))
					return TRUE;
			}

			ie = ib.FindNextEntry();
		}
	}

	return FALSE;
}

// Traverse SubNode recursivly in ascending order
// Call user defined callback routine once found an subentry
void CFileRecord::TraverseSubNode(CONST ULONGLONG &vcn, SUBENTRY_CALLBACK seCallBack) CONST
{
	CAttr_IndexAlloc *ia = (CAttr_IndexAlloc*)FindFirstAttr(ATTR_TYPE_INDEX_ALLOCATION);
	if (ia == NULL)
		return;

	CIndexBlock ib;
	if (ia->ParseIndexBlock(vcn, ib))
	{
		CIndexEntry *ie = ib.FindFirstEntry();
		while (ie)
		{
			if (ie->IsSubNodePtr())
				TraverseSubNode(ie->GetSubNodeVCN(), seCallBack);	// recursive call

			if (ie->HasName())
				seCallBack(ie);

			ie = ib.FindNextEntry();
		}
	}
}

// Parse all the attributes in a File Record
// And insert them into a link list
BOOL CFileRecord::ParseAttrs()
{
	_ASSERT(m_FileRecord);

	// Clear previous data
	ClearAttrs();

	// Visit all attributes

	DWORD dataPtr = 0;	// guard if data exceeds FileRecordSize bounds
	ATTR_HEADER_COMMON *ahc = (ATTR_HEADER_COMMON*)((BYTE*)m_FileRecord + m_FileRecord->m_OffsetOfAttr);
	dataPtr += m_FileRecord->m_OffsetOfAttr;

	while (ahc->m_Type != (DWORD)-1 && (dataPtr+ahc->m_TotalSize) <= m_Volume->m_FileRecordSize)
	{
		if (ATTR_MASK(ahc->m_Type) & m_AttrMask)	// Skip unwanted attributes
		{
			if (!ParseAttr(ahc))	// Parse error
				return FALSE;

			if (IsEncrypted() || IsCompressed())
			{
				DEBUG_TRACE0("NTFS Compressed and Encrypted not supported");
				return FALSE;
			}
		}

		dataPtr += ahc->m_TotalSize;
		ahc = (ATTR_HEADER_COMMON*)((BYTE*)ahc + ahc->m_TotalSize);	// next attribute
	}

	return TRUE;
}

// Install Attribute raw data CallBack routines for a single File Record
BOOL CFileRecord::InstallAttrRawCallBack(DWORD attrType, ATTR_RAW_CALLBACK cb)
{
	DWORD atIdx = ATTR_INDEX(attrType);
	if (atIdx < ATTR_NUMS)
	{
		m_AttrRawCallBack[atIdx] = cb;
		return TRUE;
	}
	else
		return FALSE;
}

// Clear all Attribute CallBack routines
void CFileRecord::ClearAttrRawCallBack()
{
	for (INT i = 0; i < ATTR_NUMS; i ++)
		m_AttrRawCallBack[i] = NULL;
}

// Choose attributes to handle, unwanted attributes will be discarded silently
void CFileRecord::SetAttrMask(DWORD mask)
{
	// Standard Information and Attribute List is needed always
	m_AttrMask = mask | MASK_STANDARD_INFORMATION | MASK_ATTRIBUTE_LIST;
}

// Traverse all Attribute and return CAttr_xxx classes to User Callback routine
void CFileRecord::TraverseAttrs(ATTRS_CALLBACK attrCallBack, void *context)
{
	_ASSERT(attrCallBack);

	for (INT i = 0; i < ATTR_NUMS; i ++)
	{
		if (m_AttrMask & (((DWORD)1)<<i))	// skip masked attributes
		{
			CONST CAttrBase *ab = m_AttrList[i].FindFirstEntry();
			while (ab)
			{
				BOOL bStop;
				bStop = FALSE;
				attrCallBack(ab, context, &bStop);
				if (bStop)
					return;

				ab = m_AttrList[i].FindNextEntry();
			}
		}
	}
}

// Find Attributes
CONST CAttrBase* CFileRecord::FindFirstAttr(DWORD attrType) CONST
{
	DWORD attrIdx = ATTR_INDEX(attrType);

	return attrIdx < ATTR_NUMS ? m_AttrList[attrIdx].FindFirstEntry() : NULL;
}

CONST CAttrBase* CFileRecord::FindNextAttr(DWORD attrType) CONST
{
	DWORD attrIdx = ATTR_INDEX(attrType);

	return attrIdx < ATTR_NUMS ? m_AttrList[attrIdx].FindNextEntry() : NULL;
}

// Get File Name (First Win32 name)
INT CFileRecord::GetFileName(_TCHAR *buf, DWORD bufLen) CONST
{
	// A file may have several filenames
	// Return the first Win32 filename
	CAttr_FileName *fn = (CAttr_FileName*)m_AttrList[ATTR_INDEX(ATTR_TYPE_FILE_NAME)].FindFirstEntry();
	while (fn)
	{
		if (fn->IsWin32Name())
		{
			INT len = fn->GetFileName(buf, bufLen);
			if (len != 0)
				return len;	// success or fail
		}

		fn = (CAttr_FileName*)m_AttrList[ATTR_INDEX(ATTR_TYPE_FILE_NAME)].FindNextEntry();
	}

	return 0;
}

// Get File Size
ULONGLONG CFileRecord::GetFileSize() CONST
{
	CAttr_FileName *fn = (CAttr_FileName*)m_AttrList[ATTR_INDEX(ATTR_TYPE_FILE_NAME)].FindFirstEntry();
	return fn ? fn->GetFileSize() : 0;
}

// Get File Times
void CFileRecord::GetFileTime(FILETIME *writeTm, FILETIME *createTm, FILETIME *accessTm) CONST
{
	// Standard Information attribute hold the most updated file time
	CAttr_StdInfo *si = (CAttr_StdInfo*)m_AttrList[ATTR_INDEX(ATTR_TYPE_STANDARD_INFORMATION)].FindFirstEntry();
	if (si)
		si->GetFileTime(writeTm, createTm, accessTm);
	else
	{
		writeTm->dwHighDateTime = 0;
		writeTm->dwLowDateTime = 0;
		if (createTm)
		{
			createTm->dwHighDateTime = 0;
			createTm->dwLowDateTime = 0;
		}
		if (accessTm)
		{
			accessTm->dwHighDateTime = 0;
			accessTm->dwLowDateTime = 0;
		}
	}
}

// Traverse all sub directories and files contained
// Call user defined callback routine once found an entry
void CFileRecord::TraverseSubEntries(SUBENTRY_CALLBACK seCallBack) CONST
{
	_ASSERT(seCallBack);

	// Start traversing from IndexRoot (B+ tree root node)

	CAttr_IndexRoot* ir = (CAttr_IndexRoot*)FindFirstAttr(ATTR_TYPE_INDEX_ROOT);
	if (ir == NULL || !ir->IsFileName())
		return;

	CIndexEntryList *ieList = (CIndexEntryList*)ir;
	CIndexEntry *ie = ieList->FindFirstEntry();
	while (ie)
	{
		// Visit subnode first
		if (ie->IsSubNodePtr())
			TraverseSubNode(ie->GetSubNodeVCN(), seCallBack);

		if (ie->HasName())
			seCallBack(ie);

		ie = ieList->FindNextEntry();
	}
}

// Find a specific FileName from InexRoot described B+ tree
CONST BOOL CFileRecord::FindSubEntry(CONST _TCHAR *fileName, CIndexEntry &ieFound) CONST
{
	// Start searching from IndexRoot (B+ tree root node)
	CAttr_IndexRoot *ir = (CAttr_IndexRoot*)FindFirstAttr(ATTR_TYPE_INDEX_ROOT);
	if (ir == NULL || !ir->IsFileName())
		return FALSE;

	CIndexEntryList *ieList = (CIndexEntryList*)ir;
	CIndexEntry *ie = ieList->FindFirstEntry();
	while (ie)
	{
		if (ie->HasName())
		{
			// Compare name
			INT i = ie->Compare(fileName);
			if (i == 0)
			{
				ieFound = *ie;
				return TRUE;
			}
			else if (i < 0)		// fileName is smaller than IndexEntry
			{
				// Visit SubNode
				if (ie->IsSubNodePtr())
				{
					// Search in SubNode (IndexBlock)
					if (VisitIndexBlock(ie->GetSubNodeVCN(), fileName, ieFound))
						return TRUE;
				}
				else
					return FALSE;	// not found
			}
			// Just step forward if fileName is bigger than IndexEntry
		}
		else if (ie->IsSubNodePtr())
		{
			// Search in SubNode (IndexBlock)
			if (VisitIndexBlock(ie->GetSubNodeVCN(), fileName, ieFound))
				return TRUE;
		}

		ie = ieList->FindNextEntry();
	}

	return FALSE;
}

// Find Data attribute class of 
CONST CAttrBase* CFileRecord::FindStream(_TCHAR *name)
{
	CONST CAttrBase *data = FindFirstAttr(ATTR_TYPE_DATA);
	while (data)
	{
		if (data->IsUnNamed() && name == NULL)	// Unnamed stream
			break;
		if ((!data->IsUnNamed()) && name)	// Named stream
		{
			_TCHAR an[MAX_PATH];
			if (data->GetAttrName(an, MAX_PATH))
			{
				if (_tcscmp(an, name) == 0)
					break;
			}
		}

		data = FindNextAttr(ATTR_TYPE_DATA);
	}

	return data;
}

// Check if it's deleted or in use
BOOL CFileRecord::IsDeleted() CONST
{
	return !(m_FileRecord->m_Flags & FILE_RECORD_FLAG_INUSE);
}

// Check if it's a directory
BOOL CFileRecord::IsDirectory() CONST
{
	return m_FileRecord->m_Flags & FILE_RECORD_FLAG_DIR;
}

BOOL CFileRecord::IsReadOnly() CONST
{
	// Standard Information attribute holds the most updated file time
	CONST CAttr_StdInfo *si = (CAttr_StdInfo*)m_AttrList[ATTR_INDEX(ATTR_TYPE_STANDARD_INFORMATION)].FindFirstEntry();
	return si ? si->IsReadOnly() : FALSE;
}

BOOL CFileRecord::IsHidden() CONST
{
	CONST CAttr_StdInfo *si = (CAttr_StdInfo*)m_AttrList[ATTR_INDEX(ATTR_TYPE_STANDARD_INFORMATION)].FindFirstEntry();
	return si ? si->IsHidden() : FALSE;
}

BOOL CFileRecord::IsSystem() CONST
{
	CONST CAttr_StdInfo *si = (CAttr_StdInfo*)m_AttrList[ATTR_INDEX(ATTR_TYPE_STANDARD_INFORMATION)].FindFirstEntry();
	return si ? si->IsSystem() : FALSE;
}

BOOL CFileRecord::IsCompressed() CONST
{
	CONST CAttr_StdInfo *si = (CAttr_StdInfo*)m_AttrList[ATTR_INDEX(ATTR_TYPE_STANDARD_INFORMATION)].FindFirstEntry();
	return si ? si->IsCompressed() : FALSE;
}

BOOL CFileRecord::IsEncrypted() CONST
{
	CONST CAttr_StdInfo *si = (CAttr_StdInfo*)m_AttrList[ATTR_INDEX(ATTR_TYPE_STANDARD_INFORMATION)].FindFirstEntry();
	return si ? si->IsEncrypted() : FALSE;
}

BOOL CFileRecord::IsSparse() CONST
{
	CONST CAttr_StdInfo *si = (CAttr_StdInfo*)m_AttrList[ATTR_INDEX(ATTR_TYPE_STANDARD_INFORMATION)].FindFirstEntry();
	return si ? si->IsSparse() : FALSE;
}

void CFileRecord::TraverseSubNodeEx( CONST ULONGLONG &vcn, ISubEntryCallBack * pSubEntryCallBack ) CONST
{
	CAttr_IndexAlloc *ia = (CAttr_IndexAlloc*)FindFirstAttr(ATTR_TYPE_INDEX_ALLOCATION);
	if (ia == NULL)
		return;

	CIndexBlock ib;
	if (ia->ParseIndexBlock(vcn, ib))
	{
		CIndexEntry *ie = ib.FindFirstEntry();
		while (ie)
		{
			if (ie->IsSubNodePtr())
			{
				TraverseSubNodeEx(ie->GetSubNodeVCN(), pSubEntryCallBack);	// recursive call
			}

			if (ie->HasName())
			{
				pSubEntryCallBack->Operate(ie);
			}

			ie = ib.FindNextEntry();
		}
	}
}

void CFileRecord::TraverseSubEntriesEx( ISubEntryCallBack * pSubEntryCallBack ) CONST
{
	_ASSERT(pSubEntryCallBack);

	// Start traversing from IndexRoot (B+ tree root node)
	CAttr_IndexRoot* ir = (CAttr_IndexRoot*)FindFirstAttr(ATTR_TYPE_INDEX_ROOT);
	if (ir == NULL || !ir->IsFileName())
		return;

	CIndexEntryList *ieList = (CIndexEntryList*)ir;
	CIndexEntry *ie = ieList->FindFirstEntry();
	while (ie)
	{
		// Visit sub node first
		if (ie->IsSubNodePtr())
		{
			TraverseSubNodeEx(ie->GetSubNodeVCN(), pSubEntryCallBack);
		}

		if (ie->HasName())
		{
			pSubEntryCallBack->Operate(ie);
		}

		ie = ieList->FindNextEntry();
	}
}

//******************************************
// NTFS Volume Implementation
//******************************************
CNTFSVolume::CNTFSVolume(IFileDeviceObject * pFileDeviceVolume)
{
	m_pFileDeviceVolume = pFileDeviceVolume;
	m_VolumeOK = FALSE;
	m_MFTRecord = NULL;
	m_MFTData = NULL;
	m_Version = 0;
	ClearAttrRawCallBack();

	if (!OpenVolume(m_pFileDeviceVolume))
		return;

	// Verify NTFS volume version (must >= 3.0)

	CFileRecord volFileRecord(this);
	volFileRecord.SetAttrMask(MASK_VOLUME_NAME | MASK_VOLUME_INFORMATION);
	if (!volFileRecord.ParseFileRecord(MFT_IDX_VOLUME))
		return;

	volFileRecord.ParseAttrs();
	CAttr_VolInfo *vi = (CAttr_VolInfo*)volFileRecord.FindFirstAttr(ATTR_TYPE_VOLUME_INFORMATION);
	if (!vi)
		return;

	m_Version = vi->GetVersion();
	DEBUG_TRACE2("NTFS volume version: %u.%u", HIBYTE(m_Version), LOBYTE(m_Version));
	if (m_Version < 0x0300)	// NT4 ?
		return;

#ifdef	_DEBUG
	CAttr_VolName *vn = (CAttr_VolName*)volFileRecord.FindFirstAttr(ATTR_TYPE_VOLUME_NAME);
	if (vn)
	{
		char volname[MAX_PATH];
		if (vn->GetName(volname, MAX_PATH) > 0)
		{
			DEBUG_TRACE1("NTFS volume name: %S", volname);
		}
	}
#endif

	m_VolumeOK = TRUE;

	m_MFTRecord = new CFileRecord(this);
	m_MFTRecord->SetAttrMask(MASK_DATA);
	if (m_MFTRecord->ParseFileRecord(MFT_IDX_MFT))
	{
		m_MFTRecord->ParseAttrs();
		m_MFTData = m_MFTRecord->FindFirstAttr(ATTR_TYPE_DATA);
		if (m_MFTData == NULL)
		{
			delete m_MFTRecord;
			m_MFTRecord = NULL;
		}
	}
}

CNTFSVolume::~CNTFSVolume()
{
	m_pFileDeviceVolume = NULL;

	if (m_MFTRecord)
		delete m_MFTRecord;
}

// Open a volume ('a' - 'z', 'A' - 'Z'), get volume handle and BPB
BOOL CNTFSVolume::OpenVolume(IFileDeviceObject * pFileDeviceVolume)
{
	if (TRUE)
	{
		DWORD num;
		NTFS_BPB bpb;

		// Read the first sector (boot sector)
		if (SUCCEEDED(m_pFileDeviceVolume->Read(512, &num, (BYTE*)&bpb)) && num==512)
		{
			if (strncmp((CONST char*)bpb.m_Signature, NTFS_SIGNATURE, 8) == 0)
			{
				// Log important volume parameters

				m_SectorSize = bpb.m_BytesPerSector;
				DEBUG_TRACE1("Sector Size = %u bytes", m_SectorSize);

				m_ClusterSize = m_SectorSize * bpb.m_SectorsPerCluster;
				DEBUG_TRACE1("Cluster Size = %u bytes", m_ClusterSize);

				INT sz = (char)bpb.m_ClustersPerFileRecord;
				if (sz > 0)
					m_FileRecordSize = m_ClusterSize * sz;
				else
					m_FileRecordSize = 1 << (-sz);
				DEBUG_TRACE1("FileRecord Size = %u bytes", m_FileRecordSize);

				sz = (char)bpb.m_ClustersPerIndexBlock;
				if (sz > 0)
					m_IndexBlockSize = m_ClusterSize * sz;
				else
					m_IndexBlockSize = 1 << (-sz);
				DEBUG_TRACE1("IndexBlock Size = %u bytes", m_IndexBlockSize);

				m_MFTAddr = bpb.m_LCN_MFT * m_ClusterSize;
				DEBUG_TRACE1("MFT address = 0x%016I64X", m_MFTAddr);
			}
			else
			{
				DEBUG_TRACE0("Non-NTFS Volume");
				goto IOError;
			}
		}
		else
		{
			DEBUG_TRACE0("Read boot sector error");
			goto IOError;
		}
	}
	else
	{
		DEBUG_TRACE0("Cannnot open volume");
IOError:
		m_pFileDeviceVolume = NULL;
		return FALSE;
	}

	return TRUE;
}

// Check if Volume is successfully opened
BOOL CNTFSVolume::IsVolumeOK() CONST
{
	return m_VolumeOK;
}

// Get NTFS volume version
WORD CNTFSVolume::GetVersion() CONST
{
	return m_Version;
}

// Get File Record count
ULONGLONG CNTFSVolume::GetRecordsCount() CONST
{
	return (m_MFTData->GetDataSize() / m_FileRecordSize);
}

// Get BPB information

DWORD CNTFSVolume::GetSectorSize() CONST
{
	return m_SectorSize;
}

DWORD CNTFSVolume::GetClusterSize() CONST
{
	return m_ClusterSize;
}

DWORD CNTFSVolume::GetFileRecordSize() CONST
{
	return m_FileRecordSize;
}

DWORD CNTFSVolume::GetIndexBlockSize() CONST
{
	return m_IndexBlockSize;
}

// Get MFT starting address
ULONGLONG CNTFSVolume::GetMFTAddr() CONST
{
	return m_MFTAddr;
}

// Install Attribute CallBack routines for the whole Volume
BOOL CNTFSVolume::InstallAttrRawCallBack(DWORD attrType, ATTR_RAW_CALLBACK cb)
{
	DWORD atIdx = ATTR_INDEX(attrType);
	if (atIdx < ATTR_NUMS)
	{
		m_AttrRawCallBack[atIdx] = cb;
		return TRUE;
	}
	else
		return FALSE;
}

// Clear all Attribute CallBack routines
void CNTFSVolume::ClearAttrRawCallBack()
{
	for (INT i = 0; i < ATTR_NUMS; i ++)
		m_AttrRawCallBack[i] = NULL;
}
