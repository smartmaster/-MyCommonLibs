#include "stdafx.h"
#include "NTFS_Lib.h"


CAttrBase::CAttrBase( CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr )
{
	_ASSERT(ahc);
	_ASSERT(fr);

	m_AttrHeader = ahc;
	m_FileRecord = fr;

	m__SectorSize = fr->m_Volume->m_SectorSize;
	m__ClusterSize = fr->m_Volume->m_ClusterSize;
	m__IndexBlockSize = fr->m_Volume->m_IndexBlockSize;
	m__pFileDeviceVolume = fr->m_Volume->m_pFileDeviceVolume;
}

CAttrBase::~CAttrBase()
{

}

CONST ATTR_HEADER_COMMON* CAttrBase::GetAttrHeader() CONST
{
	return m_AttrHeader;
}

DWORD CAttrBase::GetAttrType() CONST
{
	return m_AttrHeader->m_Type;
}

DWORD CAttrBase::GetAttrTotalSize() CONST
{
	return m_AttrHeader->m_TotalSize;
}

BOOL CAttrBase::IsNonResident() CONST
{
	return m_AttrHeader->m_NonResident;
}

WORD CAttrBase::GetAttrFlags() CONST
{
	return m_AttrHeader->m_Flags;
}

// Get ANSI Attribute name
// Return 0: Unnamed, <0: buffer too small, -buffersize, >0 Name length

INT CAttrBase::GetAttrName( char *buf, DWORD bufLen ) CONST
{
	if (m_AttrHeader->m_NameLength)
	{
		if (bufLen < m_AttrHeader->m_NameLength)
			return -1*m_AttrHeader->m_NameLength;	// buffer too small

		wchar_t *namePtr = (wchar_t*)((BYTE*)m_AttrHeader + m_AttrHeader->m_NameOffset);
		INT len = WideCharToMultiByte(CP_ACP, 0, namePtr, m_AttrHeader->m_NameLength,
			buf, bufLen, NULL, NULL);
		if (len)
		{
			buf[len] = '\0';
			DEBUG_TRACE1("Attribute name: %S", buf);
			return len;
		}
		else
		{
			DEBUG_TRACE0("Unrecognized attribute name or Name buffer too small");
			return -1*m_AttrHeader->m_NameLength;
		}
	}
	else
	{
		DEBUG_TRACE0("Attribute is unnamed");
		return 0;
	}
}

// Get UNICODE Attribute name
// Return 0: Unnamed, <0: buffer too small, -buffersize, >0 Name length

INT CAttrBase::GetAttrName( wchar_t *buf, DWORD bufLen ) CONST
{
	if (m_AttrHeader->m_NameLength)
	{
		if (bufLen < m_AttrHeader->m_NameLength)
			return -1*m_AttrHeader->m_NameLength;	// buffer too small

		bufLen = m_AttrHeader->m_NameLength;
		wchar_t *namePtr = (wchar_t*)((BYTE*)m_AttrHeader + m_AttrHeader->m_NameOffset);
		wcsncpy_s(buf, bufLen+1, namePtr, bufLen);
		buf[bufLen] = '\0\0';

		DEBUG_TRACE0("Unicode Attribute Name");
		return bufLen;
	}
	else
	{
		DEBUG_TRACE0("Attribute is unnamed");
		return 0;
	}
}

// Verify if this attribute is unnamed
// Useful in analyzing MultiStream files

BOOL CAttrBase::IsUnNamed() CONST
{
	return (m_AttrHeader->m_NameLength == 0);
}

CAttrResident::CAttrResident(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr) : CAttrBase(ahc, fr)
{
	m_AttrHeaderR = (ATTR_HEADER_RESIDENT*)ahc;
	m_AttrBody = (void*)((BYTE*)m_AttrHeaderR + m_AttrHeaderR->m_AttrOffset);
	m_AttrBodySize = m_AttrHeaderR->m_AttrSize;
}

CAttrResident::~CAttrResident()
{
}

 BOOL CAttrResident::IsDataRunOK() CONST
{
	return TRUE;	// Always OK for a resident attribute
}

// Return Actural Data Size
// *allocSize = Allocated Size
 ULONGLONG CAttrResident::GetDataSize(ULONGLONG *allocSize) CONST
{
	if (allocSize)
		*allocSize = m_AttrBodySize;

	return (ULONGLONG)m_AttrBodySize;
}

// Read "bufLen" bytes from "offset" into "bufv"
// Number of bytes acturally read is returned in "*actural"
BOOL CAttrResident::ReadData(CONST ULONGLONG &offset, void *bufv, DWORD bufLen, DWORD *actural) CONST
{
	_ASSERT(bufv);

	*actural = 0;
	if (bufLen == 0)
		return TRUE;

	DWORD offsetd = (DWORD)offset;
	if (offsetd >= m_AttrBodySize)
		return FALSE;	// offset parameter error

	if ((offsetd + bufLen) > m_AttrBodySize)
		*actural = m_AttrBodySize - offsetd;	// Beyond scope
	else
		*actural = bufLen;

	memcpy(bufv, (BYTE*)m_AttrBody + offsetd, *actural);

	return TRUE;
}

CAttrNonResident::CAttrNonResident(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr) : CAttrBase(ahc, fr)
{
	m_AttrHeaderNR = (ATTR_HEADER_NON_RESIDENT*)ahc;

	m_UnalignedBuf = new BYTE[m__ClusterSize];

	m_bDataRunOK = ParseDataRun();
}

CAttrNonResident::~CAttrNonResident()
{
	delete m_UnalignedBuf;

	m_DataRunList.RemoveAll();
}

// Parse a single DataRun unit
BOOL CAttrNonResident::PickData(CONST BYTE **dataRun, LONGLONG *length, LONGLONG *LCNOffset)
{
	BYTE size = **dataRun;
	(*dataRun)++;
	INT lengthBytes = size & 0x0F;
	INT offsetBytes = size >> 4;

	if (lengthBytes > 8 || offsetBytes > 8)
	{
		DEBUG_TRACE1("DataRun decode error: 0x%02X", size);
		return FALSE;
	}

	*length = 0;
	memcpy(length, *dataRun, lengthBytes);
	if (*length < 0)
	{
		DEBUG_TRACE1("DataRun length error: %I64d", *length);
		return FALSE;
	}

	(*dataRun) += lengthBytes;
	*LCNOffset = 0;
	if (offsetBytes)	// Not Sparse File
	{
		if ((*dataRun)[offsetBytes-1] & 0x80)
			*LCNOffset = -1;
		memcpy(LCNOffset, *dataRun, offsetBytes);

		(*dataRun) += offsetBytes;
	}

	return TRUE;
}

// Travers DataRun and insert into a link list
BOOL CAttrNonResident::ParseDataRun()
{
	DEBUG_TRACE0("Parsing Non Resident DataRun");
	DEBUG_TRACE2("Start VCN = %I64u, End VCN = %I64u",
		m_AttrHeaderNR->m_StartVCN, m_AttrHeaderNR->m_LastVCN);

	CONST BYTE *dataRun = (BYTE*)m_AttrHeaderNR + m_AttrHeaderNR->m_DataRunOffset;
	LONGLONG length;
	LONGLONG LCNOffset;
	LONGLONG LCN = 0;
	ULONGLONG VCN = 0;

	while (*dataRun)
	{
		if (PickData(&dataRun, &length, &LCNOffset))
		{
			LCN += LCNOffset;
			if (LCN < 0)
			{
				DEBUG_TRACE0("DataRun decode error");
				return FALSE;
			}

			DEBUG_TRACE2("Data length = %I64d clusters, LCN = %I64d", length, LCN);
			if (0 == LCNOffset)
			{
				DEBUG_TRACE0("Sparse Data");
			}
			

			// Store LCN, Data size (clusters) into list
			DataRun_Entry *dr = new DataRun_Entry;
			dr->m_LCN = (LCNOffset == 0) ? -1 : LCN;
			dr->m_Clusters = length;
			dr->m_StartVCN = VCN;
			VCN += length;
			dr->m_LastVCN = VCN - 1;

			if (dr->m_LastVCN <= (m_AttrHeaderNR->m_LastVCN - m_AttrHeaderNR->m_StartVCN))
			{
				m_DataRunList.InsertEntry(dr);
			}
			else
			{
				DEBUG_TRACE0("DataRun decode error: VCN exceeds bound");

				// Remove entries
				m_DataRunList.RemoveAll();

				return FALSE;
			}
		}
		else
			break;
	}

	return TRUE;
}

// Read clusters from disk, or sparse data
// *actural = Clusters acturally read
BOOL CAttrNonResident::ReadClusters(void *buf, DWORD clusters, LONGLONG lcn)
{
	if (lcn == -1)	// sparse data
	{
		DEBUG_TRACE0("Sparse Data, fill the buffer with 0");

		// Fill the buffer with 0
		memset(buf, 0, clusters * m__ClusterSize);

		return TRUE;
	}

	LARGE_INTEGER addr;
	DWORD len;

	addr.QuadPart = lcn * m__ClusterSize;
	LONGLONG llNewFilePointer = 0;
	HRESULT hr = m__pFileDeviceVolume->SetPointerEx( 
		addr.QuadPart,///* [in] */ LONGLONG llDistanceToMove,
		&llNewFilePointer,///* [out] */ LONGLONG *lpNewFilePointer,
		FILE_BEGIN///* [in] */ ULONG ulMoveMethod
		);

	if (FAILED(hr)/*len == (DWORD)-1 && GetLastError() != NO_ERROR*/)
	{
		DEBUG_TRACE1("Cannot locate cluster with LCN %I64d", lcn);
	}
	else
	{
		if (SUCCEEDED(m__pFileDeviceVolume->Read(clusters*m__ClusterSize, &len, (BYTE*)buf)) &&
			len == clusters*m__ClusterSize)
		{
			DEBUG_TRACE2("Successfully read %u clusters from LCN %I64d", clusters, lcn);
			return TRUE;
		}
		else
		{
			DEBUG_TRACE1("Cannot read cluster with LCN %I64d", lcn);
		}
	}

	return FALSE;
}

// Read Data, cluster based
// clusterNo: Begnning cluster Number
// clusters: Clusters to read
// bufv, bufLen: Returned data
// *actural = Number of bytes acturally read
BOOL CAttrNonResident::ReadVirtualClusters(ULONGLONG vcn, DWORD clusters,
	void *bufv, DWORD bufLen, DWORD *actural)
{
	_ASSERT(bufv);
	_ASSERT(clusters);

	*actural = 0;
	BYTE *buf = (BYTE*)bufv;

	// Verify if clusters exceeds DataRun bounds
	if (vcn + clusters > (m_AttrHeaderNR->m_LastVCN - m_AttrHeaderNR->m_StartVCN +1))
	{
		DEBUG_TRACE0("Cluster exceeds DataRun bounds");
		return FALSE;
	}

	// Verify buffer size
	if (bufLen < clusters*m__ClusterSize)
	{
		DEBUG_TRACE0("Buffer size too small");
		return FALSE;
	}

	// Traverse the DataRun List to find the according LCN
	CONST DataRun_Entry *dr = m_DataRunList.FindFirstEntry();
	while(dr)
	{
		if (vcn>=dr->m_StartVCN && vcn<=dr->m_LastVCN)
		{
			DWORD clustersToRead;

			ULONGLONG vcns = dr->m_LastVCN - vcn + 1;	// Clusters from read pointer to the end

			if ((ULONGLONG)clusters > vcns)	// Fragmented data, we must go on
				clustersToRead = (DWORD)vcns;
			else
				clustersToRead = clusters;
			if (ReadClusters(buf, clustersToRead, dr->m_LCN+(vcn-dr->m_StartVCN)))
			{
				buf += clustersToRead*m__ClusterSize;
				clusters -= clustersToRead;
				*actural += clustersToRead;
				vcn += clustersToRead;
			}
			else
				break;

			if (clusters == 0)
				break;
		}

		dr = m_DataRunList.FindNextEntry();
	}

	*actural *= m__ClusterSize;
	return TRUE;
}

// Judge if the DataRun is successfully parsed
 BOOL CAttrNonResident::IsDataRunOK() CONST
{
	return m_bDataRunOK;
}

// Return Actural Data Size
// *allocSize = Allocated Size
 ULONGLONG CAttrNonResident::GetDataSize(ULONGLONG *allocSize) CONST
{
	if (allocSize)
		*allocSize = m_AttrHeaderNR->m_AllocSize;

	return m_AttrHeaderNR->m_RealSize;
}

// Read "bufLen" bytes from "offset" into "bufv"
// Number of bytes acturally read is returned in "*actural"
BOOL CAttrNonResident::ReadData(CONST ULONGLONG &offset, void *bufv, DWORD bufLen, DWORD *actural) CONST
{
	// Hard disks can only be accessed by sectors
	// To be simple and efficient, only implemented cluster based accessing
	// So cluster unaligned data address should be processed carefully here

	_ASSERT(bufv);

	*actural = 0;
	if (bufLen == 0)
		return TRUE;

	// Bounds check
	if (offset > m_AttrHeaderNR->m_RealSize)
		return FALSE;
	if ((offset + bufLen) > m_AttrHeaderNR->m_RealSize)
		bufLen = (DWORD)(m_AttrHeaderNR->m_RealSize - offset);

	DWORD len;
	BYTE *buf = (BYTE*)bufv;

	// First cluster Number
	ULONGLONG startVCN = offset / m__ClusterSize;
	// Bytes in first cluster
	DWORD startBytes = m__ClusterSize - (DWORD)(offset % m__ClusterSize);
	// Read first cluster
	if (startBytes != m__ClusterSize)
	{
		// First cluster, Unaligned
		if (((CAttrNonResident*)this)->ReadVirtualClusters(startVCN, 1, m_UnalignedBuf, m__ClusterSize, &len)
			&& len == m__ClusterSize)
		{
			len = (startBytes < bufLen) ? startBytes : bufLen;
			memcpy(buf, m_UnalignedBuf + m__ClusterSize - startBytes, len);
			buf += len;
			bufLen -= len;
			*actural += len;
			startVCN++;
		}
		else
			return FALSE;
	}
	if (bufLen == 0)
		return TRUE;

	DWORD alignedClusters = bufLen / m__ClusterSize;
	if (alignedClusters)
	{
		// Aligned clusters
		DWORD alignedSize = alignedClusters*m__ClusterSize;
		if (((CAttrNonResident*)this)->ReadVirtualClusters(startVCN, alignedClusters, buf, alignedSize, &len)
			&& len == alignedSize)
		{
			startVCN += alignedClusters;
			buf += alignedSize;
			bufLen %= m__ClusterSize;
			*actural += len;

			if (bufLen == 0)
				return TRUE;
		}
		else
			return FALSE;
	}

	// Last cluster, Unaligned
	if (((CAttrNonResident*)this)->ReadVirtualClusters(startVCN, 1, m_UnalignedBuf, m__ClusterSize, &len)
		&& len == m__ClusterSize)
	{
		memcpy(buf, m_UnalignedBuf, bufLen);
		*actural += bufLen;

		return TRUE;
	}
	else
		return FALSE;
}

CONST CDataRunList & CAttrNonResident::GetDataRunList() CONST
{
	return m_DataRunList;
}

CAttr_StdInfo::CAttr_StdInfo(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr) : CAttrResident(ahc, fr)
{
	DEBUG_TRACE0("Attribute: Standard Information");

	m_StdInfo = (ATTR_STANDARD_INFORMATION*)m_AttrBody;
}

CAttr_StdInfo::~CAttr_StdInfo()
{
	DEBUG_TRACE0("CAttr_StdInfo dtor");
}

// Change from UTC time to local time
void CAttr_StdInfo::GetFileTime(FILETIME *writeTm, FILETIME *createTm, FILETIME *accessTm) CONST
{
	UTC2Local(m_StdInfo->m_AlterTime, writeTm);

	if (createTm)
		UTC2Local(m_StdInfo->m_CreateTime, createTm);

	if (accessTm)
		UTC2Local(m_StdInfo->m_ReadTime, accessTm);
}

 DWORD CAttr_StdInfo::GetFilePermission() CONST
{
	return m_StdInfo->m_Permission;
}

 BOOL CAttr_StdInfo::IsReadOnly() CONST
{
	return ((m_StdInfo->m_Permission) & ATTR_STDINFO_PERMISSION_READONLY);
}

 BOOL CAttr_StdInfo::IsHidden() CONST
{
	return ((m_StdInfo->m_Permission) & ATTR_STDINFO_PERMISSION_HIDDEN);
}

 BOOL CAttr_StdInfo::IsSystem() CONST
{
	return ((m_StdInfo->m_Permission) & ATTR_STDINFO_PERMISSION_SYSTEM);
}

 BOOL CAttr_StdInfo::IsCompressed() CONST
{
	return ((m_StdInfo->m_Permission) & ATTR_STDINFO_PERMISSION_COMPRESSED);
}

 BOOL CAttr_StdInfo::IsEncrypted() CONST
{
	return ((m_StdInfo->m_Permission) & ATTR_STDINFO_PERMISSION_ENCRYPTED);
}

 BOOL CAttr_StdInfo::IsSparse() CONST
{
	return ((m_StdInfo->m_Permission) & ATTR_STDINFO_PERMISSION_SPARSE);
}

// UTC filetime to Local filetime
void CAttr_StdInfo::UTC2Local(CONST ULONGLONG &ultm, FILETIME *lftm)
{
	LARGE_INTEGER fti;
	FILETIME ftt;

	fti.QuadPart = ultm;
	ftt.dwHighDateTime = fti.HighPart;
	ftt.dwLowDateTime = fti.LowPart;

	if (!FileTimeToLocalFileTime(&ftt, lftm))
		*lftm = ftt;
}

CNtfsFileName::CNtfsFileName(ATTR_FILE_NAME *fn)
{
	m_IsCopy = FALSE;

	m_FileName = fn;

	m_FileNameWUC = NULL;
	m_FileNameLength = 0;

	if (fn)
		GetFileNameWUC();
}

CNtfsFileName::~CNtfsFileName()
{
	if (m_FileNameWUC)
		delete m_FileNameWUC;
}

 void CNtfsFileName::SetFileName(ATTR_FILE_NAME *fn)
{
	m_FileName = fn;

	GetFileNameWUC();
}

// Copy pointer buffers
void CNtfsFileName::CopyFileName(CONST CNtfsFileName *fn, CONST ATTR_FILE_NAME *afn)
{
	if (!m_IsCopy)
	{
		DEBUG_TRACE0("Need not call");
		return;
	}

	_ASSERT(fn && afn);

	DEBUG_TRACE0("FileName Copied");

	if (m_FileNameWUC)
		delete m_FileNameWUC;

	m_FileNameLength = fn->m_FileNameLength;
	m_FileName = afn;

	if (fn->m_FileNameWUC)
	{
		m_FileNameWUC = new wchar_t[m_FileNameLength+1];
		wcsncpy_s(m_FileNameWUC, m_FileNameLength+1, fn->m_FileNameWUC, m_FileNameLength);
		m_FileNameWUC[m_FileNameLength] = wchar_t('\0');
	}
	else
		m_FileNameWUC = NULL;
}

// Get uppercase unicode filename and store it in a buffer
void CNtfsFileName::GetFileNameWUC()
{
#ifdef	_DEBUG
	char fna[MAX_PATH];
	GetFileName(fna, MAX_PATH);	// Just show filename in debug window
#endif

	if (m_FileNameWUC)
	{
		delete m_FileNameWUC;
		m_FileNameWUC = NULL;
		m_FileNameLength = 0;
	}

	wchar_t fns[MAX_PATH];
	m_FileNameLength = GetFileName(fns, MAX_PATH);

	if (m_FileNameLength > 0)
	{
		m_FileNameWUC = new wchar_t[m_FileNameLength+1];
		for (INT i=0; i<m_FileNameLength; i++)
			m_FileNameWUC[i] = towupper(fns[i]);
		m_FileNameWUC[m_FileNameLength] = wchar_t('\0');
	}
	else
	{
		m_FileNameLength = 0;
		m_FileNameWUC = NULL;
	}
}

// Compare Unicode file name
INT CNtfsFileName::Compare(CONST wchar_t *fn) CONST
{
	// Change fn to upper case
	size_t len = wcslen(fn);
	if (len > MAX_PATH)
		return 1;	// Assume bigger

	wchar_t fns[MAX_PATH];

	for (size_t i=0; i<len; i++)
		fns[i] = towupper(fn[i]);
	fns[len] = wchar_t('\0');

	return wcscmp(fns, m_FileNameWUC);
}

// Compare ANSI file name
INT CNtfsFileName::Compare(CONST char *fn) CONST
{
	wchar_t fnw[MAX_PATH];

	INT len = MultiByteToWideChar(CP_ACP, 0, fn, -1, fnw, MAX_PATH);
	if (len)
		return Compare(fnw);
	else
		return 1;	// Assume bigger
}

 ULONGLONG CNtfsFileName::GetFileSize() CONST
{
	return m_FileName ? m_FileName->m_RealSize : 0;
}

 DWORD CNtfsFileName::GetFilePermission() CONST
{
	return m_FileName ? m_FileName->m_Flags : 0;
}

 BOOL CNtfsFileName::IsReadOnly() CONST
{
	return m_FileName ? ((m_FileName->m_Flags) & ATTR_FILENAME_FLAG_READONLY) : FALSE;
}

 BOOL CNtfsFileName::IsHidden() CONST
{
	return m_FileName ? ((m_FileName->m_Flags) & ATTR_FILENAME_FLAG_HIDDEN) : FALSE;
}

 BOOL CNtfsFileName::IsSystem() CONST
{
	return m_FileName ? ((m_FileName->m_Flags) & ATTR_FILENAME_FLAG_SYSTEM) : FALSE;
}

 BOOL CNtfsFileName::IsDirectory() CONST
{
	return m_FileName ? ((m_FileName->m_Flags) & ATTR_FILENAME_FLAG_DIRECTORY) : FALSE;
}

 BOOL CNtfsFileName::IsCompressed() CONST
{
	return m_FileName ? ((m_FileName->m_Flags) & ATTR_FILENAME_FLAG_COMPRESSED) : FALSE;
}

 BOOL CNtfsFileName::IsEncrypted() CONST
{
	return m_FileName ? ((m_FileName->m_Flags) & ATTR_FILENAME_FLAG_ENCRYPTED) : FALSE;
}

 BOOL CNtfsFileName::IsSparse() CONST
{
	return m_FileName ? ((m_FileName->m_Flags) & ATTR_FILENAME_FLAG_SPARSE) : FALSE;
}

// Get ANSI File Name
// Return 0: Unnamed, <0: buffer too small, -buffersize, >0 Name length
INT CNtfsFileName::GetFileName(char *buf, DWORD bufLen) CONST
{
	if (m_FileName == NULL)
		return 0;

	INT len = 0;

	if (m_FileName->m_NameLength)
	{
		if (bufLen < m_FileName->m_NameLength)
			return -1*m_FileName->m_NameLength;	// buffer too small

		len = WideCharToMultiByte(CP_ACP, 0, (wchar_t*)m_FileName->m_Name, m_FileName->m_NameLength,
			buf, bufLen, NULL, NULL);
		if (len)
		{
			buf[len] = '\0';
			DEBUG_TRACE1("File Name: %S", buf);
			DEBUG_TRACE4("File Permission: %S  %c%c%c", IsDirectory()?"Directory":"File",
				IsReadOnly()?'R':' ', IsHidden()?'H':' ', IsSystem()?'S':' ');
		}
		else
		{
			DEBUG_TRACE0("Unrecognized File Name or FileName buffer too small");
		}
	}

	return len;
}

// Get Unicode File Name
// Return 0: Unnamed, <0: buffer too small, -buffersize, >0 Name length
INT CNtfsFileName::GetFileName(wchar_t *buf, DWORD bufLen) CONST
{
	if (m_FileName == NULL)
		return 0;

	if (m_FileName->m_NameLength)
	{
		if (bufLen < m_FileName->m_NameLength)
			return -1*m_FileName->m_NameLength;	// buffer too small

		bufLen = m_FileName->m_NameLength;
		wcsncpy_s(buf, bufLen+1, (wchar_t*)m_FileName->m_Name, bufLen);
		buf[bufLen] = wchar_t('\0');

		return bufLen;
	}

	return 0;
}

 BOOL CNtfsFileName::HasName() CONST
{
	return m_FileNameLength > 0;
}

 BOOL CNtfsFileName::IsWin32Name() CONST
{
	if (m_FileName == NULL || m_FileNameLength <= 0)
		return FALSE;

	return (m_FileName->m_NameSpace != ATTR_FILENAME_NAMESPACE_DOS);	// POSIX, WIN32, WIN32_DOS
}

// Change from UTC time to local time
void CNtfsFileName::GetFileTime(FILETIME *writeTm, FILETIME *createTm, FILETIME *accessTm) CONST
{
	CAttr_StdInfo::UTC2Local(m_FileName ? m_FileName->m_AlterTime : 0, writeTm);

	if (createTm)
		CAttr_StdInfo::UTC2Local(m_FileName ? m_FileName->m_CreateTime : 0, createTm);

	if (accessTm)
		CAttr_StdInfo::UTC2Local(m_FileName ? m_FileName->m_ReadTime : 0, accessTm);
}


CAttr_FileName::CAttr_FileName( CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr ) : CAttrResident(ahc, fr)
{
	DEBUG_TRACE0("Attribute: File Name");

	SetFileName((ATTR_FILE_NAME*)m_AttrBody);
}

CAttr_FileName::~CAttr_FileName()
{
	DEBUG_TRACE0("CAttr_FileName dtor");
}

CAttr_VolInfo::CAttr_VolInfo( CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr ) : CAttrResident(ahc, fr)
{
	DEBUG_TRACE0("Attribute: Volume Information");

	m_VolInfo = (ATTR_VOLUME_INFORMATION*)m_AttrBody;
}

CAttr_VolInfo::~CAttr_VolInfo()
{
	DEBUG_TRACE0("CAttr_VolInfo dtor");
}

WORD CAttr_VolInfo::GetVersion()
{
	return MAKEWORD(m_VolInfo->MinorVersion, m_VolInfo->m_MajorVersion);
}

CAttr_VolName::CAttr_VolName( CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr ) : CAttrResident(ahc, fr)
{
	DEBUG_TRACE0("Attribute: Volume Name");

	m_NameLength = m_AttrBodySize >> 1;
	m_VolNameU = new wchar_t[m_NameLength+1];
	m_VolNameA = new char[m_NameLength+1];

	memcpy(m_VolNameU, m_AttrBody, m_AttrBodySize);
	m_VolNameU[m_NameLength] = wchar_t('\0');

	INT len = WideCharToMultiByte(CP_ACP, 0, m_VolNameU, m_NameLength,
		m_VolNameA, m_NameLength, NULL, NULL);
	m_VolNameA[m_NameLength] = '\0';
}

CAttr_VolName::~CAttr_VolName()
{
	DEBUG_TRACE0("CAttr_VolName dtor");

	delete m_VolNameU;
	delete m_VolNameA;
}

INT CAttr_VolName::GetName( wchar_t *buf, DWORD len ) CONST
{
	if (len < m_NameLength)
		return -1*m_NameLength;	// buffer too small

	wcsncpy_s(buf, m_NameLength+2, m_VolNameU, m_NameLength+1);
	return m_NameLength;
}

INT CAttr_VolName::GetName( char *buf, DWORD len ) CONST
{
	if (len < m_NameLength)
		return -1*m_NameLength;	// buffer too small

	strncpy_s(buf, m_NameLength+2, m_VolNameA, m_NameLength+1);
	return m_NameLength;
}

CIndexEntry::CIndexEntry()
{
	DEBUG_TRACE0("Index Entry");

	m_IsDefault = TRUE;

	m_IndexEntry = NULL;
	SetFileName(NULL);
}

CIndexEntry::CIndexEntry( CONST INDEX_ENTRY *ie )
{
	DEBUG_TRACE0("Index Entry");

	m_IsDefault = FALSE;

	_ASSERT(ie);
	m_IndexEntry = ie;

	if (IsSubNodePtr())
	{
		DEBUG_TRACE0("Pointer to sub-node");
	}

	if (ie->m_StreamSize)
	{
		SetFileName((ATTR_FILE_NAME*)(ie->m_Stream));
	}
	else
	{
		DEBUG_TRACE0("No FileName stream found");
	}
}

CIndexEntry::~CIndexEntry()
{
	// Never touch *IndexEntry here if IsCopy == FALSE !
	// As the memory have been deallocated by ~CIndexBlock()

	if (m_IsCopy && m_IndexEntry)
		delete (void*)m_IndexEntry;

	DEBUG_TRACE0("CIndexEntry dtor");
}

CIndexEntry& CIndexEntry::operator=( CONST CIndexEntry &ieClass )
{
	if (!m_IsDefault)
	{
		DEBUG_TRACE0("Cannot call");
		return *this;
	}

	DEBUG_TRACE0("Index Entry Copied");

	m_IsCopy = TRUE;

	if (m_IndexEntry)
	{
		delete (void*)m_IndexEntry;
		m_IndexEntry = NULL;
	}

	CONST INDEX_ENTRY *ie = ieClass.m_IndexEntry;
	_ASSERT(ie && (ie->m_Size > 0));

	m_IndexEntry = (INDEX_ENTRY*)new BYTE[ie->m_Size];
	memcpy((void*)m_IndexEntry, ie, ie->m_Size);
	CopyFileName(&ieClass, (ATTR_FILE_NAME*)(m_IndexEntry->m_Stream));

	return *this;
}

ULONGLONG CIndexEntry::GetFileReference() CONST
{
	if (m_IndexEntry)
		return m_IndexEntry->m_FileReference & 0x0000FFFFFFFFFFFFUL;
	else
		return (ULONGLONG)-1;
}

BOOL CIndexEntry::IsSubNodePtr() CONST
{
	if (m_IndexEntry)
		return (m_IndexEntry->m_Flags & INDEX_ENTRY_FLAG_SUBNODE);
	else
		return FALSE;
}

ULONGLONG CIndexEntry::GetSubNodeVCN() CONST
{
	if (m_IndexEntry)
		return *(ULONGLONG*)((BYTE*)m_IndexEntry + m_IndexEntry->m_Size - 8);
	else
		return (ULONGLONG)-1;
}

CIndexBlock::CIndexBlock()
{
	DEBUG_TRACE0("Index Block");

	m_IndexBlock = NULL;
}

CIndexBlock::~CIndexBlock()
{
	DEBUG_TRACE0("IndexBlock dtor");

	if (m_IndexBlock)
		delete m_IndexBlock;
}

INDEX_BLOCK * CIndexBlock::AllocIndexBlock( DWORD size )
{
	// Free previous data if any
	if (GetCount() > 0)
		RemoveAll();
	if (m_IndexBlock)
		delete m_IndexBlock;

	m_IndexBlock = (INDEX_BLOCK*)new BYTE[size];

	return m_IndexBlock;
}


CAttr_IndexRoot::CAttr_IndexRoot(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr): CAttrResident(ahc, fr)
{
	DEBUG_TRACE0("Attribute: Index Root");

	m_IndexRoot = (ATTR_INDEX_ROOT*)m_AttrBody;

	if (IsFileName())
	{
		ParseIndexEntries();
	}
	else
	{
		DEBUG_TRACE0("Index View not supported");
	}
}

CAttr_IndexRoot::~CAttr_IndexRoot()
{
	DEBUG_TRACE0("CAttr_IndexRoot dtor");
}

// Get all the index entries
void CAttr_IndexRoot::ParseIndexEntries()
{
	INDEX_ENTRY *ie;
	ie = (INDEX_ENTRY*)((BYTE*)(&(m_IndexRoot->m_EntryOffset)) + m_IndexRoot->m_EntryOffset);

	DWORD ieTotal = ie->m_Size;

	while (ieTotal <= m_IndexRoot->m_TotalEntrySize)
	{
		CIndexEntry *ieClass = new CIndexEntry(ie);
		InsertEntry(ieClass);

		if (ie->m_Flags & INDEX_ENTRY_FLAG_LAST)
		{
			DEBUG_TRACE0("Last Index Entry");
			break;
		}

		ie = (INDEX_ENTRY*)((BYTE*)ie + ie->m_Size);	// Pick next
		ieTotal += ie->m_Size;
	}
}

// Check if this IndexRoot contains FileName or IndexView
 BOOL CAttr_IndexRoot::IsFileName() CONST
{
	return (m_IndexRoot->m_AttrType == ATTR_TYPE_FILE_NAME);
}

CAttr_IndexAlloc::CAttr_IndexAlloc(CONST ATTR_HEADER_COMMON *ahc, CONST CFileRecord *fr) : CAttrNonResident(ahc, fr)
{
	DEBUG_TRACE0("Attribute: Index Allocation");

	m_IndexBlockCount = 0;

	if (IsDataRunOK())
	{
		// Get total number of Index Blocks
		ULONGLONG ibTotalSize;
		ibTotalSize = GetDataSize();
		if (ibTotalSize % m__IndexBlockSize)
		{
			DEBUG_TRACE2("Cannot calulate number of IndexBlocks, total size = %I64u, unit = %u",
				ibTotalSize, m__IndexBlockSize);
			return;
		}
		m_IndexBlockCount = ibTotalSize / m__IndexBlockSize;
	}
	else
	{
		DEBUG_TRACE0("Index Allocation DataRun parse error");
	}
}

CAttr_IndexAlloc::~CAttr_IndexAlloc()
{
	DEBUG_TRACE0("CAttr_IndexAlloc dtor");
}

// Verify US and update sectors
BOOL CAttr_IndexAlloc::PatchUS(WORD *sector, INT sectors, WORD usn, WORD *usarray)
{
	INT i;

	for (i=0; i<sectors; i++)
	{
		sector += ((m__SectorSize>>1) - 1);
		if (*sector != usn)
			return FALSE;		// USN error
		*sector = usarray[i];	// Write back correct data
		sector++;
	}
	return TRUE;
}

 ULONGLONG CAttr_IndexAlloc::GetIndexBlockCount()
{
	return m_IndexBlockCount;
}

// Parse a single Index Block
// vcn = Index Block VCN in Index Allocation Data Attributes
// ibClass holds the parsed Index Entries
BOOL CAttr_IndexAlloc::ParseIndexBlock(CONST ULONGLONG &vcn, CIndexBlock &ibClass)
{
	if (vcn >= m_IndexBlockCount)	// Bounds check
	{
		//return FALSE;
	}

	// Allocate buffer for a single Index Block
	INDEX_BLOCK *ibBuf = ibClass.AllocIndexBlock(m__IndexBlockSize);

	// Sectors Per Index Block
	DWORD sectors = m__IndexBlockSize / m__SectorSize;

	// Read one Index Block
	DWORD len;
	if (ReadData(vcn*m__ClusterSize/*m__IndexBlockSize*/, ibBuf, m__IndexBlockSize, &len) &&
		len == m__IndexBlockSize)
	{
		if (ibBuf->m_Magic != INDEX_BLOCK_MAGIC)
		{
			DEBUG_TRACE0("Index Block parse error: Magic mismatch");
			return FALSE;
		}

		// Patch US
		WORD *usnaddr = (WORD*)((BYTE*)ibBuf + ibBuf->m_OffsetOfUS);
		WORD usn = *usnaddr;
		WORD *usarray = usnaddr + 1;
		if (!PatchUS((WORD*)ibBuf, sectors, usn, usarray))
		{
			DEBUG_TRACE0("Index Block parse error: Update Sequence Number");
			return FALSE;
		}

		INDEX_ENTRY *ie;
		ie = (INDEX_ENTRY*)((BYTE*)(&(ibBuf->m_EntryOffset)) + ibBuf->m_EntryOffset);

		DWORD ieTotal = ie->m_Size;

		while (ieTotal <= ibBuf->m_TotalEntrySize)
		{
			CIndexEntry *ieClass = new CIndexEntry(ie);
			ibClass.InsertEntry(ieClass);

			if (ie->m_Flags & INDEX_ENTRY_FLAG_LAST)
			{
				DEBUG_TRACE0("Last Index Entry");
				break;
			}

			ie = (INDEX_ENTRY*)((BYTE*)ie + ie->m_Size);	// Pick next
			ieTotal += ie->m_Size;
		}

		return TRUE;
	}
	else
		return FALSE;
}

