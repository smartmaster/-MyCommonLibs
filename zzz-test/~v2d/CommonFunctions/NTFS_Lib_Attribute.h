/*
 * NTFS Attribute Classes
*/

#pragma once

#ifndef	__NTFS_ATTRIBUTE_H_3B4851CD_4EB5_4612_9288_B5B444B35FBE
#define	__NTFS_ATTRIBUTE_H_3B4851CD_4EB5_4612_9288_B5B444B35FBE


//******************************************
// List to hold parsed DataRuns
//******************************************
typedef struct tagDataRun_Entry
{
	LONGLONG			m_LCN;		// -1 to indicate sparse data
	ULONGLONG			m_Clusters;
	ULONGLONG			m_StartVCN;
	ULONGLONG			m_LastVCN;
} DataRun_Entry;
typedef class CNtfsSList<DataRun_Entry> CDataRunList;

//******************************************
// List to hold Index Entry objects
//******************************************
class CIndexEntry;
typedef class CNtfsSList<CIndexEntry> CIndexEntryList;


//******************************************
// Attributes base class
//******************************************
class CAttrBase
{
public:
	CAttrBase(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr);
	virtual ~CAttrBase();

protected:
	CONST ATTR_HEADER_COMMON *m_AttrHeader;
	WORD m__SectorSize;
	DWORD m__ClusterSize;
	DWORD m__IndexBlockSize;
	IFileDeviceObject * m__pFileDeviceVolume;
	CONST CFileRecord *m_FileRecord;

public:
	 CONST ATTR_HEADER_COMMON* GetAttrHeader() CONST;
	 DWORD GetAttrType() CONST;
	 DWORD GetAttrTotalSize() CONST;
	 BOOL IsNonResident() CONST;
	 WORD GetAttrFlags() CONST;
	INT GetAttrName(char *buf, DWORD bufLen) CONST;
	INT GetAttrName(wchar_t *buf, DWORD bufLen) CONST;
	 BOOL IsUnNamed() CONST;

public:
	virtual  BOOL IsDataRunOK() CONST = 0;

public:
	virtual  ULONGLONG GetDataSize(ULONGLONG *allocSize = NULL) CONST = 0;
	virtual BOOL ReadData(CONST ULONGLONG &offset, void *bufv, DWORD bufLen, DWORD *actural) CONST = 0;
};	// CAttrBase


//******************************************
// Resident Attributes
//******************************************
class CAttrResident : public CAttrBase
{
public:
	CAttrResident(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr);
	virtual ~CAttrResident();

protected:
	CONST ATTR_HEADER_RESIDENT *m_AttrHeaderR;
	CONST void *m_AttrBody;	// Points to Resident Data
	DWORD m_AttrBodySize;		// Attribute Data Size

	virtual  BOOL IsDataRunOK() CONST;

public:
	virtual  ULONGLONG GetDataSize(ULONGLONG *allocSize = NULL) CONST;
	virtual BOOL ReadData(CONST ULONGLONG &offset, void *bufv, DWORD bufLen, DWORD *actural) CONST;
};	// CAttrResident


//******************************************
// NonResident Attributes
//******************************************
class CAttrNonResident : public CAttrBase
{
public:
	CAttrNonResident(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr);
	virtual ~CAttrNonResident();

protected:
	CONST ATTR_HEADER_NON_RESIDENT *m_AttrHeaderNR;
	CDataRunList m_DataRunList;

private:
	BOOL m_bDataRunOK;
	BYTE *m_UnalignedBuf;	// Buffer to hold not cluster aligned data
	BOOL PickData(CONST BYTE **dataRun, LONGLONG *length, LONGLONG *LCNOffset);
	BOOL ParseDataRun();
	BOOL ReadClusters(void *buf, DWORD clusters, LONGLONG lcn);
	BOOL ReadVirtualClusters(ULONGLONG vcn, DWORD clusters,
		void *bufv, DWORD bufLen, DWORD *actural);

protected:
	virtual  BOOL IsDataRunOK() CONST;

public:
	virtual  ULONGLONG GetDataSize(ULONGLONG *allocSize = NULL) CONST;
	virtual BOOL ReadData(CONST ULONGLONG &offset, void *bufv, DWORD bufLen, DWORD *actural) CONST;

	CONST CDataRunList & GetDataRunList() CONST;
};	// CAttrNonResident


//******************************************
// Attribute: Standard Information
//******************************************
class CAttr_StdInfo : public CAttrResident
{
public:
	CAttr_StdInfo(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr);
	virtual ~CAttr_StdInfo();

private:
	CONST ATTR_STANDARD_INFORMATION *m_StdInfo;

public:
	void GetFileTime(FILETIME *writeTm, FILETIME *createTm = NULL, FILETIME *accessTm = NULL) CONST;
	 DWORD GetFilePermission() CONST;
	 BOOL IsReadOnly() CONST;
	 BOOL IsHidden() CONST;
	 BOOL IsSystem() CONST;
	 BOOL IsCompressed() CONST;
	 BOOL IsEncrypted() CONST;
	 BOOL IsSparse() CONST;

	static void UTC2Local(CONST ULONGLONG &ultm, FILETIME *lftm);
};	// CAttr_StdInfo


//******************************************
// FileName helper class
// used by FileName and IndexEntry
//******************************************
class CNtfsFileName
{
public:
	CNtfsFileName(ATTR_FILE_NAME *fn = NULL);
	virtual ~CNtfsFileName();

protected:
	CONST ATTR_FILE_NAME *m_FileName;	// May be NULL for an IndexEntry
	wchar_t *m_FileNameWUC;	// Uppercase Unicode File Name, used to compare file names
	INT m_FileNameLength;
	BOOL m_IsCopy;

	 void SetFileName(ATTR_FILE_NAME *fn);
	void CNtfsFileName::CopyFileName(CONST CNtfsFileName *fn, CONST ATTR_FILE_NAME *afn);

private:
	void GetFileNameWUC();

public:
	INT Compare(CONST wchar_t *fn) CONST;
	INT Compare(CONST char *fn) CONST;

	 ULONGLONG GetFileSize() CONST;
	 DWORD GetFilePermission() CONST;
	 BOOL IsReadOnly() CONST;
	 BOOL IsHidden() CONST;
	 BOOL IsSystem() CONST;
	 BOOL IsDirectory() CONST;
	 BOOL IsCompressed() CONST;
	 BOOL IsEncrypted() CONST;
	 BOOL IsSparse() CONST;

	INT GetFileName(char *buf, DWORD bufLen) CONST;
	INT GetFileName(wchar_t *buf, DWORD bufLen) CONST;
	 BOOL HasName() CONST;
	 BOOL IsWin32Name() CONST;

	void GetFileTime(FILETIME *writeTm, FILETIME *createTm = NULL, FILETIME *accessTm = NULL) CONST;
};	// CFileName


//******************************************
// Attribute: File Name
//******************************************
class CAttr_FileName : public CAttrResident, public CNtfsFileName
{
public:
	CAttr_FileName(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr);
	virtual ~CAttr_FileName();

private:
	// File permission and time in $FILE_NAME only updates when the filename changes
	// So hide these functions to prevent user from getting the error information
	// Standard Information and IndexEntry keeps the most recent file time and permission infomation
	void GetFileTime(FILETIME *writeTm, FILETIME *createTm = NULL, FILETIME *accessTm = NULL) CONST {}
	 DWORD GetFilePermission(){}
	 BOOL IsReadOnly() CONST {}
	 BOOL IsHidden() CONST {}
	 BOOL IsSystem() CONST {}
	 BOOL IsCompressed() CONST {}
	 BOOL IsEncrypted() CONST {}
	 BOOL IsSparse() CONST {}
};	// CAttr_FileName


//******************************************
// Attribute: Volume Information
//******************************************
class CAttr_VolInfo : public CAttrResident
{
public:
	CAttr_VolInfo(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr);

	virtual ~CAttr_VolInfo();

private:
	CONST ATTR_VOLUME_INFORMATION *m_VolInfo;

public:
	// Get NTFS Volume Version
	WORD GetVersion();
}; // CAttr_VolInfo


//******************************************
// Attribute: Volume Name
//******************************************
class CAttr_VolName : public CAttrResident
{
public:
	CAttr_VolName(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr);

	virtual ~CAttr_VolName();

private:
	wchar_t *m_VolNameU;
	char *m_VolNameA;
	DWORD m_NameLength;

public:
	// Get NTFS Volume Unicode Name
	INT GetName(wchar_t *buf, DWORD len) CONST;

	// ANSI Name
	INT GetName(char *buf, DWORD len) CONST;
}; // CAttr_VolInfo


//******************************************
// Attribute: Data
//******************************************
template <class TYPE_RESIDENT>
class CAttr_Data : public TYPE_RESIDENT
{
public:
	CAttr_Data(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr) : TYPE_RESIDENT(ahc, fr)
	{
		DEBUG_TRACE1("Attribute: Data (%SResident)", IsNonResident() ? "Non" : "");
	}

	virtual ~CAttr_Data()
	{
		DEBUG_TRACE0("CAttr_Data dtor");
	}
};	// CAttr_Data


//******************************************
// Index Entry helper class
//******************************************
class CIndexEntry : public CNtfsFileName
{
public:
	CIndexEntry();
	CIndexEntry(CONST INDEX_ENTRY *ie);
	virtual ~CIndexEntry();

private:
	BOOL m_IsDefault;

protected:
	CONST INDEX_ENTRY *m_IndexEntry;

public:
	// Use with caution !
	CIndexEntry& operator = (CONST CIndexEntry &ieClass);
	ULONGLONG GetFileReference() CONST;
	BOOL IsSubNodePtr() CONST;
	ULONGLONG GetSubNodeVCN() CONST;
};	// CIndexEntry


//******************************************
// Index Block helper class
//******************************************
class CIndexBlock : public CIndexEntryList
{
public:
	CIndexBlock();
	virtual ~CIndexBlock();

private:
	INDEX_BLOCK *m_IndexBlock;

public:
	INDEX_BLOCK *AllocIndexBlock(DWORD size);
};	// CIndexBlock


//******************************************
// Attribute: Index Root (Resident)
//******************************************
class CAttr_IndexRoot : public CAttrResident, public CIndexEntryList
{
public:
	CAttr_IndexRoot(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr);
	virtual ~CAttr_IndexRoot();

private:
	CONST ATTR_INDEX_ROOT *m_IndexRoot;

	void ParseIndexEntries();

public:
	 BOOL IsFileName() CONST;
};	// CAttr_IndexRoot


//******************************************
// Attribute: Index Allocation (NonResident)
//******************************************
class CAttr_IndexAlloc : public CAttrNonResident
{
public:
	CAttr_IndexAlloc(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr);
	virtual ~CAttr_IndexAlloc();

private:
	ULONGLONG m_IndexBlockCount;

	BOOL PatchUS(WORD *sector, INT sectors, WORD usn, WORD *usarray);

public:
	 ULONGLONG GetIndexBlockCount();
	BOOL ParseIndexBlock(CONST ULONGLONG &vcn, CIndexBlock &ibClass);
};	// CAttr_IndexAlloc


//******************************************
// Attribute: Bitmap
//******************************************
template <class TYPE_RESIDENT>
class CAttr_Bitmap : public TYPE_RESIDENT
{
public:
	CAttr_Bitmap(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr);
	virtual ~CAttr_Bitmap();

private:
	ULONGLONG m_BitmapSize;	// Bitmap data size
	BYTE *m_BitmapBuf;		// Bitmap data buffer
	LONGLONG m_CurrentCluster;

public:
	BOOL IsClusterFree(CONST ULONGLONG &cluster) CONST;
};	// CAttr_Bitmap

template <class TYPE_RESIDENT>
CAttr_Bitmap<TYPE_RESIDENT>::CAttr_Bitmap(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr) : TYPE_RESIDENT(ahc, fr)
{
	DEBUG_TRACE1("Attribute: Bitmap (%SResident)", IsNonResident() ? "Non" : "");

	m_CurrentCluster = -1;

	if (IsDataRunOK())
	{
		m_BitmapSize = GetDataSize();

		if (IsNonResident())
			m_BitmapBuf = new BYTE[m__ClusterSize];
		else
		{
			m_BitmapBuf = new BYTE[(DWORD)m_BitmapSize];

			DWORD len;
			if (!(ReadData(0, m_BitmapBuf, (DWORD)m_BitmapSize, &len)
				&& len == (DWORD)m_BitmapSize))
			{
				m_BitmapBuf = NULL;
				DEBUG_TRACE0("Read Resident Bitmap data failed");
			}
			else
			{
				DEBUG_TRACE1("%u bytes of resident Bitmap data read", len);
			}
		}
	}
	else
	{
		m_BitmapSize = 0;
		m_BitmapBuf = 0;
	}
}

template <class TYPE_RESIDENT>
CAttr_Bitmap<TYPE_RESIDENT>::~CAttr_Bitmap()
{
	if (m_BitmapBuf)
		delete m_BitmapBuf;

	DEBUG_TRACE0("CAttr_Bitmap dtor");
}

// Verify if a single cluster is free
template <class TYPE_RESIDENT>
BOOL CAttr_Bitmap<TYPE_RESIDENT>::IsClusterFree(CONST ULONGLONG &cluster) CONST
{
	if (!IsDataRunOK() || !m_BitmapBuf)
		return FALSE;

	if (IsNonResident())
	{
		LONGLONG idx = (LONGLONG)cluster >> 3;
		DWORD clusterSize = ((CNTFSVolume*)Volume)->GetClusterSize();

		LONGLONG clusterOffset = idx/clusterSize;
		cluster -= (clusterOffset*clusterSize*8);

		// Read one cluster of data if buffer mismatch
		if (m_CurrentCluster != clusterOffset)
		{
			DWORD len;
			if (ReadData(clusterOffset, m_BitmapBuf, clusterSize, &len) && len == clusterSize)
			{
				m_CurrentCluster = clusterOffset;
			}
			else
			{
				m_CurrentCluster = -1;
				return FALSE;
			}
		}
	}

	// All the Bitmap data is already in BitmapBuf
	DWORD idx = (DWORD)(cluster >> 3);
	if (IsNonResident() == FALSE)
	{
		if (idx >= m_BitmapSize)
			return TRUE;	// Resident data bounds check error
	}

	BYTE fac = (BYTE)(cluster % 8);

	return ((m_BitmapBuf[idx] & (1<<fac)) == 0);
}


//******************************************
// List to hold external File Records
//******************************************
typedef CNtfsSList<CFileRecord> CFileRecordList;

//******************************************
// Attribute: Attribute List
//******************************************
template <class TYPE_RESIDENT>
class CAttr_AttrList : public TYPE_RESIDENT
{
public:
	CAttr_AttrList(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr);
	virtual ~CAttr_AttrList();

private:
	CFileRecordList m_FileRecordList;
};	// CAttr_AttrList

template <class TYPE_RESIDENT>
CAttr_AttrList<TYPE_RESIDENT>::CAttr_AttrList(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr) : TYPE_RESIDENT(ahc, fr)
{
	DEBUG_TRACE0("Attribute: Attribute List");
	if (fr->m_FileReference == (ULONGLONG)-1)
		return;

	ULONGLONG offset = 0;
	DWORD len;
	ATTR_ATTRIBUTE_LIST alRecord;

	while (ReadData(offset, &alRecord, sizeof(ATTR_ATTRIBUTE_LIST), &len) &&
		len == sizeof(ATTR_ATTRIBUTE_LIST))
	{
		if (ATTR_INDEX(alRecord.m_AttrType) > ATTR_NUMS)
		{
			DEBUG_TRACE0("Attribute List parse error");
			break;
		}

		DEBUG_TRACE1("Attribute List: 0x%04x", alRecord.m_AttrType);

		ULONGLONG recordRef = alRecord.m_BaseRef & 0x0000FFFFFFFFFFFFUL;
		if (recordRef != fr->m_FileReference)	// Skip contained attributes
		{
			DWORD am = ATTR_MASK(alRecord.m_AttrType);
			if (am & fr->m_AttrMask)	// Skip unwanted attributes
			{
				CFileRecord *frnew = new CFileRecord(fr->m_Volume);
				m_FileRecordList.InsertEntry(frnew);

				frnew->m_AttrMask = am;
				if (!frnew->ParseFileRecord(recordRef))
				{
					DEBUG_TRACE0("Attribute List parse error");
					break;
				}
				frnew->ParseAttrs();

				// Insert new found AttrList to fr->AttrList
				CONST CAttrBase *ab = (CAttrBase*)frnew->FindFirstAttr(alRecord.m_AttrType);
				while (ab)
				{
					CAttrList *al = (CAttrList*)&fr->m_AttrList[ATTR_INDEX(alRecord.m_AttrType)];
					al->InsertEntry((CAttrBase*)ab);
					ab = frnew->FindNextAttr(alRecord.m_AttrType);
				}

				// Throw away frnew->AttrList entries to prevent free twice (fr will delete them)
				frnew->m_AttrList[ATTR_INDEX(alRecord.m_AttrType)].ThrowAll();
			}
		}

		offset += alRecord.m_RecordSize;
	}
}

template <class TYPE_RESIDENT>
CAttr_AttrList<TYPE_RESIDENT>::~CAttr_AttrList()
{
	DEBUG_TRACE0("CAttr_AttrList dtor");
}

#endif
