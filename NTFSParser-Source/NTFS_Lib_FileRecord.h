/*
 * NTFS Volume and File Record Class
  */

#pragma once

#ifndef	__NTFS_FILERECORD_H_3B4851CD_4EB5_4612_9288_B5B444B35FBE
#define	__NTFS_FILERECORD_H_3B4851CD_4EB5_4612_9288_B5B444B35FBE

#include "..\PublicHeader\PublicFileDeviceInterface.h"

//******************************************
// NTFS Volume forward declaration
//******************************************
class CNTFSVolume
{
public:
	CNTFSVolume(IFileDevice * pFileDeviceVolume);
	virtual ~CNTFSVolume();

	friend class CFileRecord;
	friend class CAttrBase;

private:
	WORD m_SectorSize;
	DWORD m_ClusterSize;
	DWORD m_FileRecordSize;
	DWORD m_IndexBlockSize;
	ULONGLONG m_MFTAddr;
	IFileDevice * m_pFileDeviceVolume; //external object
	BOOL m_VolumeOK;
	ATTR_RAW_CALLBACK m_AttrRawCallBack[ATTR_NUMS];
	WORD m_Version;

	// MFT file records ($MFT file itself) may be fragmented
	// Get $MFT Data attribute to translate FileRecord to correct disk offset
	CFileRecord *m_MFTRecord;		// $MFT File Record
	CONST CAttrBase *m_MFTData;	// $MFT Data Attribute

	BOOL OpenVolume(IFileDevice * pFileDeviceVolume);

public:
	 BOOL IsVolumeOK() CONST;
	 WORD GetVersion() CONST;
	 ULONGLONG GetRecordsCount() CONST;

	 DWORD GetSectorSize() CONST;
	 DWORD GetClusterSize() CONST;
	 DWORD GetFileRecordSize() CONST;
	 DWORD GetIndexBlockSize() CONST;
	 ULONGLONG GetMFTAddr() CONST;

	BOOL InstallAttrRawCallBack(DWORD attrType, ATTR_RAW_CALLBACK cb);
	 void ClearAttrRawCallBack();
};	// CNTFSVolume


//******************************************
// List to hold Attributes of the same type
//******************************************
typedef class CNtfsSList<CAttrBase> CAttrList;

#if	_MSC_VER <= 1200
class CAttrResident;
class CAttrNonResident;
template <class TYPE_RESIDENT> class CAttr_AttrList;
#endif


//******************************************
struct ISubEntryCallBack
{
	virtual INT Operate(const CIndexEntry *ie) = 0;
};

//<sonmi01>2015-4-23 ###???
struct ISubEntryCallBackRP
{
	virtual INT Operate(const INDEX_ENTRY_RP *ieRP) = 0;
};



//******************************************
// Process a single File Record
//******************************************
class CFileRecord
{
public:
	CFileRecord(CONST CNTFSVolume *volume);
	virtual ~CFileRecord();

	friend class CAttrBase;
#if	_MSC_VER <= 1200
	friend class CAttr_AttrList<CAttrResident>;
	friend class CAttr_AttrList<CAttrNonResident>;
#else
	template <class TYPE_RESIDENT> friend class CAttr_AttrList;
#endif

private:
	CONST CNTFSVolume *m_Volume;
	FILE_RECORD_HEADER *m_FileRecord;
	ULONGLONG m_FileReference;
	ATTR_RAW_CALLBACK m_AttrRawCallBack[ATTR_NUMS];
	DWORD m_AttrMask;
	CAttrList m_AttrList[ATTR_NUMS];	// Attributes
	LONG m_IndexRootVirtualClusterSize; //<sonmi01>2014-4-18 ###???
	LONG m_IndexRootIndexBufferSize; //<sonmi01>2014-4-18 ###???

	void ClearAttrs();
	BOOL PatchUS(WORD *sector, INT sectors, WORD usn, WORD *usarray);
	 void UserCallBack(DWORD attType, ATTR_HEADER_COMMON *ahc, BOOL *bDiscard);
	CAttrBase* AllocAttr(ATTR_HEADER_COMMON *ahc, BOOL *bUnhandled);
	BOOL ParseAttr(ATTR_HEADER_COMMON *ahc);
	FILE_RECORD_HEADER* ReadFileRecord(CONST ULONGLONG fileRef);
	BOOL VisitIndexBlock(CONST ULONGLONG vcn, CONST _TCHAR *fileName, CIndexEntry &ieFound) CONST;
#if 0
	void TraverseSubNode(CONST ULONGLONG vcn, SUBENTRY_CALLBACK seCallBack) CONST;
#endif
	void TraverseSubNodeEx(CONST ULONGLONG vcn, ISubEntryCallBack * pSubEntryCallBack/*, BOOL bAllEntries*/) CONST;
	HRESULT TraverseSubNodeExRP(CONST ULONGLONG vcn, ISubEntryCallBackRP * pSubEntryCallBackRP/*, BOOL bAllEntries*/) CONST; //<sonmi01>2015-4-23 ###???

public:
	BOOL ParseFileRecord(ULONGLONG fileRef);
	BOOL ParseAttrs();

	BOOL InstallAttrRawCallBack(DWORD attrType, ATTR_RAW_CALLBACK cb);
	 void ClearAttrRawCallBack();

	 void SetAttrMask(DWORD mask);
	void TraverseAttrs(ATTRS_CALLBACK attrCallBack, void *context);
	 CONST CAttrBase* FindFirstAttr(DWORD attrType) CONST;
	CONST CAttrBase* FindNextAttr(DWORD attrType) CONST;
	//<sonmi01>2015-4-24 ###???
	LONG GetAttrCount(DWORD attrType) CONST;


	INT GetFileName(_TCHAR *buf, DWORD bufLen) CONST;
	 ULONGLONG GetFileSize() CONST;
	void GetFileTime(FILETIME *writeTm, FILETIME *createTm = NULL, FILETIME *accessTm = NULL) CONST;

#if 0
	void TraverseSubEntries(SUBENTRY_CALLBACK seCallBack) CONST;
#endif
	void TraverseSubEntriesEx(ISubEntryCallBack * pSubEntryCallBack/*, BOOL bAllEntries*/) CONST;
	HRESULT TraverseSubEntriesExRP(ISubEntryCallBackRP * pSubEntryCallBackRP/*, BOOL bAllEntries*/) CONST;

	CONST BOOL FindSubEntry(CONST _TCHAR *fileName, CIndexEntry &ieFound) CONST;
	CONST CAttrBase* FindStream(_TCHAR *name = NULL);

	 BOOL IsDeleted() CONST;
	 BOOL IsDirectory() CONST;
	 BOOL IsReadOnly() CONST;
	 BOOL IsHidden() CONST;
	 BOOL IsSystem() CONST;
	 BOOL IsCompressed() CONST;
	 BOOL IsEncrypted() CONST;
	 BOOL IsSparse() CONST;
	 


	 LONGLONG GetFileReference();
};	// CFileRecord



#endif
