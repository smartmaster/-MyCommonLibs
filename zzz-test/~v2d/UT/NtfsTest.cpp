#include "stdafx.h"
#include "..\CommonFunctions\NTFS_Lib_helper.h"
#include "..\CommonFunctions\VolumeHelper.h"
#include "..\CommonFunctions\LocalFileDevice.h"
#include "..\CommonFunctions\MiscHelper.h"

LPCTSTR AttrNames[] = 
{
	TEXT("STANDARD_INFORMATION"),
	TEXT("ATTRIBUTE_LIST"),
	TEXT("FILE_NAME"),
	TEXT("OBJECT_ID"),
	TEXT("SECURITY_DESCRIPTOR"),
	TEXT("VOLUME_NAME"),
	TEXT("VOLUME_INFORMATION"),
	TEXT("DATA"),
	TEXT("INDEX_ROOT"),
	TEXT("INDEX_ALLOCATION"),
	TEXT("BITMAP"),
	TEXT("REPARSE_POINT"),
	TEXT("EA_INFORMATION"),
	TEXT("EA"),
	TEXT("NULL"),
	TEXT("LOGGED_UTILITY_STREAM")
};

void appenddata(CString &lines, BYTE *data, DWORD datalen)
{
	// "01 02 03 04 05 06 07 08 - 09 0A 0B 0C 0D 0E 0F   123456789ABCDEF";

	CString line;
	BYTE *p;
	DWORD i;

	for (i=0; i<((datalen-1)>>4); i++)
	{
		p = data + i*16;

		line.Format(_T("%02X %02X %02X %02X %02X %02X %02X %02X - %02X %02X %02X %02X %02X %02X %02X %02X   "),
			p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
		for (int j=0; j<16; j++)
		{
			if (p[j] < 0x20)
				p[j] = '.';
		}

		CString lineCopy = line;
		line.Format(_T("%s%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\r\n"),
			lineCopy, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);

		lines += line;
	}

	// last line
	p = data + i*16;
	BYTE q[16];
	memset(q, 0xFF, 16);
	memcpy(q, p, 16);
	if ((datalen % 16) == 0)
		line.Format(_T("%02X %02X %02X %02X %02X %02X %02X %02X - %02X %02X %02X %02X %02X %02X %02X %02X   "),
		q[0], q[1], q[2], q[3], q[4], q[5], q[6], q[7], q[8], q[9], q[10], q[11], q[12], q[13], q[14], q[15]);
	else
		line.Format(_T("%02X %02X %02X %02X %02X %02X %02X %02X                             "),
		q[0], q[1], q[2], q[3], q[4], q[5], q[6], q[7]);

	for (int j=0; j<16; j++)
	{
		if (q[j] < 0x20)
			q[j] = '.';
	}

	if ((datalen % 16) == 0)
		line.Format(_T("%s%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\r\n"),
		line, q[0], q[1], q[2], q[3], q[4], q[5], q[6], q[7], q[8], q[9], q[10], q[11], q[12], q[13], q[14], q[15]);
	else
		line.Format(_T("%s%c%c%c%c%c%c%c%c\r\n"),
		line, q[0], q[1], q[2], q[3], q[4], q[5], q[6], q[7]);

	lines += line;
}

void printattr(const CAttrBase *attr, void *context, BOOL *bStop)
{
	CString *dump = (CString*)context;

	CString line = _T("\r\n");
	line += AttrNames[ATTR_INDEX(attr->GetAttrType())];

	_TCHAR attrname[100];
	if (attr->GetAttrName(attrname, 100) > 0)
	{
		line += '(';
		line += attrname;
		line += ')';
	}
	line += _T("\r\n");

	appenddata(line, (BYTE*)(attr->GetAttrHeader()), attr->GetAttrTotalSize());

	*dump += line;
}


VOID NtfsAttrTest(LPCTSTR szFileName, BOOL m_dir)
{
	CString m_dump;
	CString m_filename = szFileName;
	if (m_dir)
		m_filename = m_filename.Left(m_filename.ReverseFind(_T('\\')));

	// parse volume
	CString strRoot;
	CString strGuid;
	CVolumeHelper::GetVolumeRootAndGuidFromPath(m_filename, strRoot, strGuid);
	strGuid.TrimRight(TEXT("\\/"));
	CLocalFileDevice FileDeviceVolume;
	CComBSTR bstrFileName = strGuid.GetString();
	FileDeviceVolume.Create(
		bstrFileName,//BSTR bstrFileName, 
		GENERIC_READ,//ULONG ulDesiredAccess, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,//ULONG ulShareMode, 
		OPEN_EXISTING,//ULONG ulCreationDisposition, 
		0//ULONG ulFlagsAndAttributes
		);

	CNTFSVolume volume(&FileDeviceVolume);
	if (!volume.IsVolumeOK())
	{
		//MessageBox(_T("Not a valid NTFS volume or NTFS version < 3.0"));
		return;
	}

	// parse root directory

	CFileRecord fr(&volume);
	// we only need to parse INDEX_ROOT and INDEX_ALLOCATION
	// don't waste time and ram to parse unwanted attributes
	fr.SetAttrMask(MASK_INDEX_ROOT | MASK_INDEX_ALLOCATION);

	if (!fr.ParseFileRecord(MFT_IDX_ROOT))
	{
		//MessageBox(_T("Cannot read root directory of volume"));
		return;
	}

	if (!fr.ParseAttrs())
	{
		//MessageBox(_T("Cannot parse attributes"));
		return;
	}

	// find subdirectory

	CIndexEntry ie;

	int dirs = m_filename.Find(_T('\\'), 0);
	int dire = m_filename.Find(_T('\\'), dirs+1);
	while (dire != -1)
	{
		CString pathname = m_filename.Mid(dirs+1, dire-dirs-1);

		if (fr.FindSubEntry((const _TCHAR*)pathname, ie))
		{
			if (!fr.ParseFileRecord(ie.GetFileReference()))
			{
				//MessageBox(_T("Cannot read root directory of volume"));
				return;
			}

			if (!fr.ParseAttrs())
			{
				if (fr.IsCompressed())
					_OLD_DAPIERR((0, DRNPOS, _T("Compressed directory not supported yet"), E_FAIL));
				else if (fr.IsEncrypted())
					_OLD_DAPIERR((0, DRNPOS, _T("Encrypted directory not supported yet"), E_FAIL));
				else
					_OLD_DAPIERR((0, DRNPOS, _T("Cannot parse directory attributes"), E_FAIL));
				return;
			}
		}
		else
		{
			//MessageBox(_T("File not found\n"));
			return;
		}

		dirs = dire;
		dire = m_filename.Find(_T('\\'), dirs+1);
	}

	// dump it !

	CString filename = m_filename.Right(m_filename.GetLength()-dirs-1);

	if (filename.GetLength() == 2 && (filename.Find(_T(':')) != -1))
		filename = _T('.');	// root directory

	if (fr.FindSubEntry((const _TCHAR*)filename, ie))
	{
		if (!fr.ParseFileRecord(ie.GetFileReference()))
		{
			//MessageBox(_T("Cannot read file"));
			return;
		}

		// parse all attributes
		fr.SetAttrMask(MASK_ALL);
		if (!fr.ParseAttrs())
		{
			if (fr.IsCompressed())
				_OLD_DAPIERR((0, DRNPOS, _T("Compressed directory not supported yet"), E_FAIL));
			else if (fr.IsEncrypted())
				_OLD_DAPIERR((0, DRNPOS, _T("Encrypted directory not supported yet"), E_FAIL));
			else
				_OLD_DAPIERR((0, DRNPOS, _T("Cannot parse directory attributes"), E_FAIL));
			return;
		}

		fr.TraverseAttrs(printattr, &m_dump);
		//UpdateData(FALSE);
	}
	else
	{
		//MessageBox(_T("File not found\n"));
		return;
	}
}

VOID MyNtfsAttrTest(LPCTSTR pszFullPath, BOOL m_dir)
{
	CString strRoot;
	CString strGuid;
	CVolumeHelper::GetVolumeRootAndGuidFromPath(pszFullPath, strRoot, strGuid);
	strGuid.TrimRight(TEXT("\\/"));
	CLocalFileDevice FileDeviceVolume;
	CComBSTR bstrFileName = strGuid.GetString();
	FileDeviceVolume.Create(
		bstrFileName,//BSTR bstrFileName, 
		GENERIC_READ,//ULONG ulDesiredAccess, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,//ULONG ulShareMode, 
		OPEN_EXISTING,//ULONG ulCreationDisposition, 
		0//ULONG ulFlagsAndAttributes
		);

	CNTFSVolume * pNTFSVolumeTemp = NULL;
	CNtfsParserHelper::InitVolume(&FileDeviceVolume, &pNTFSVolumeTemp);
	CAutoPtr<CNTFSVolume> apNTFSVolume(pNTFSVolumeTemp); pNTFSVolumeTemp = NULL;

	CFileRecord * pFileRecord = NULL;
	CString strPath = PTR_TO_STRING(pszFullPath);
	strPath.Insert(0, TEXT("\\"));
	strPath.Replace(strRoot, TEXT(""));
	CNtfsParserHelper::FindAndParseFileRecord(apNTFSVolume, strPath, &pFileRecord);
	CAutoPtr<CFileRecord> apFileRecord(pFileRecord); pFileRecord = NULL;

	CString strDump;
	apFileRecord->TraverseAttrs(printattr, &strDump);
}

struct CEnumWorker
{
private:
	INT m_nDepth;
	vector<CString> m_Paths;

	//******************************************
	CString m_strIs;
	CString m_strIsNot;
	BOOL m_bShowFullPath;

	//******************************************
	VOID PrintRoot(LPCTSTR szRoot)
	{
		_ftprintf_s(stdout, TEXT("[ROOT]") TEXT("%s") TEXT("\r\n"), szRoot);
	}

	VOID PrintTabs()
	{
		for (INT ii=0; ii<m_nDepth; ++ii)
		{
			_ftprintf_s(stdout, TEXT("\t"));
		}
	}

	VOID PrintFullPath()
	{
		for (INT ii=0; ii<m_Paths.size(); ++ii)
		{
			if (0 == ii)
			{
				_ftprintf_s(stdout, TEXT("%s"), m_Paths[ii]);
			}
			else
			{
				_ftprintf_s(stdout, TEXT("\\") TEXT("%s"), m_Paths[ii]);
			}
		}
	}

	VOID PrintDir(LPCTSTR szName, CONST CIndexEntry * pie)
	{
		PrintTabs();
		_ftprintf_s(stdout, TEXT("[+]"));
		if (m_bShowFullPath)
		{
			PrintFullPath();
		}
		else
		{
			_ftprintf_s(stdout, TEXT("%s"), szName);
		}
		_ftprintf_s(stdout, TEXT("\r\n"));
	}

	VOID PrintFile(LPCTSTR szName, CONST CIndexEntry * pie)
	{
		PrintTabs();
		_ftprintf_s(stdout, TEXT("[-]"));
		if (m_bShowFullPath)
		{
			PrintFullPath();
		}
		else
		{
			_ftprintf_s(stdout, TEXT("%s"), szName);
		}

		LONGLONG llFileSize = pie->GetFileSize();
		CString strFileSize;
		CMiscHelper::NumberToString(llFileSize, strFileSize);

		TCHAR szFileAttrs[] = TEXT("---");
		if (pie->IsReadOnly())
		{
			szFileAttrs[0] = TEXT('R');
		}
		if (pie->IsHidden())
		{
			szFileAttrs[1] = TEXT('H');
		}
		if (pie->IsSystem())
		{
			szFileAttrs[2] = TEXT('S');
		}

		_ftprintf_s(stdout, TEXT("\t") TEXT("%s"), strFileSize.GetString());
		_ftprintf_s(stdout, TEXT("\t") TEXT("%s"), szFileAttrs);

		_ftprintf_s(stdout, TEXT("\r\n"));
	}

public:
	CEnumWorker(BOOL bShowFullPath) :
	  m_nDepth(0),
		  m_bShowFullPath(bShowFullPath)
	  {
	  }

	  VOID SetPattern(LPCTSTR pszIs, LPCTSTR pszIsNot)
	  {
		  m_strIs = PTR_TO_STRING(pszIs);
		  m_strIsNot = PTR_TO_STRING(pszIsNot);
	  }

	  VOID EnterRoot(LPCTSTR szRoot)
	  {
		  m_Paths.push_back(szRoot);
		  PrintRoot(szRoot);
	  }

	  VOID LeaveRoot(LPCTSTR szRoot)
	  {
		  m_Paths.pop_back();
	  }

	  VOID EnterDir(CONST CIndexEntry * pie)
	  {
		  TCHAR szName[MAX_PATH] = {0};
		  pie->GetFileName(szName, _countof(szName));

		  ++m_nDepth;
		  m_Paths.push_back(szName);

		  PrintDir(szName, pie);
	  }

	  VOID LeaveDir(CONST CIndexEntry * pie)
	  {
		  --m_nDepth;
		  m_Paths.pop_back();
	  }

	  VOID EnterFile(CONST CIndexEntry * pie)
	  {
		  TCHAR szName[MAX_PATH] = {0};
		  pie->GetFileName(szName, _countof(szName));

		  ++m_nDepth;
		  m_Paths.push_back(szName);

		  BOOL bPatternMatch = TRUE;
		  if (m_strIs.GetLength())
		  {
			  bPatternMatch = CMiscHelper::MatchPattern(szName, m_strIs);
		  }
		  if (bPatternMatch)
		  {
			  bPatternMatch = !CMiscHelper::MatchPattern(szName, m_strIsNot);
		  }

		  if (bPatternMatch)
		  {
			  PrintFile(szName, pie);
		  }
	  }

	  VOID LeaveFile(CONST CIndexEntry * pie)
	  {
		  --m_nDepth;
		  m_Paths.pop_back();
	  }
};

class CNtfsFileOp : public ISubEntryCallBack
{
	CEnumWorker & m_worker;

public:
	CNtfsFileOp(CEnumWorker & worker) :
	  m_worker(worker)
	  {
	  }

	  virtual INT Operate(const CIndexEntry * ie)
	  {
		  m_worker.EnterFile(ie);
		  m_worker.LeaveFile(ie);
		  return 0;
	  }
};

class CNtfsDirOp : public ISubEntryCallBack
{
	CEnumWorker & m_worker;

public:
	CNtfsDirOp(CEnumWorker & worker) :
	  m_worker(worker)
	  {
	  }

	  virtual INT Operate(const CIndexEntry * ie)
	  {
		  m_worker.EnterDir(ie);
		  return 0;
	  }
};

class CNtfsDirPostOp : public ISubEntryCallBack
{	
	CEnumWorker & m_worker;

public:
	CNtfsDirPostOp(CEnumWorker & worker) :
	  m_worker(worker)
	  {
	  }

	  virtual INT Operate(const CIndexEntry * ie)
	  {
		  m_worker.LeaveDir(ie);
		  return 0;
	  }
};

VOID NtfsEnumDir(LPCTSTR pszFullPath, BOOL bShowFullPath)
{
	CString strRoot;
	CString strGuid;
	CVolumeHelper::GetVolumeRootAndGuidFromPath(pszFullPath, strRoot, strGuid);
	strGuid.TrimRight(TEXT("\\/"));
	CLocalFileDevice FileDeviceVolume;
	CComBSTR bstrFileName = strGuid.GetString();
	FileDeviceVolume.Create(
		bstrFileName,//BSTR bstrFileName, 
		GENERIC_READ,//ULONG ulDesiredAccess, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,//ULONG ulShareMode, 
		OPEN_EXISTING,//ULONG ulCreationDisposition, 
		0//ULONG ulFlagsAndAttributes
		);

	CNTFSVolume * pNTFSVolumeTemp = NULL;
	CNtfsParserHelper::InitVolume(&FileDeviceVolume, &pNTFSVolumeTemp);
	CAutoPtr<CNTFSVolume> apNTFSVolume(pNTFSVolumeTemp); pNTFSVolumeTemp = NULL;
	if (NULL == (CNTFSVolume*)apNTFSVolume || !apNTFSVolume->IsVolumeOK())
	{
		return;
	}

	CFileRecord * pFileRecord = NULL;
	CString strPath = PTR_TO_STRING(pszFullPath);
	strPath.Insert(0, TEXT("\\"));
	strPath.Replace(strRoot, TEXT(""));
	BOOL bRet = TRUE;
	if (1 == strPath.GetLength() && TEXT('\\') == strPath[0])
	{
		pFileRecord = new CFileRecord(apNTFSVolume);
		pFileRecord->SetAttrMask(MASK_INDEX_ROOT | MASK_INDEX_ALLOCATION);
		bRet = pFileRecord->ParseFileRecord(MFT_IDX_ROOT);
		if (bRet)
		{
			bRet = pFileRecord->ParseAttrs();;
		}
	}
	else
	{
		CNtfsParserHelper::FindAndParseFileRecord(apNTFSVolume, strPath, &pFileRecord);
	}
	CAutoPtr<CFileRecord> apFileRecord(pFileRecord); pFileRecord = NULL;
	if (NULL == (CFileRecord*)apFileRecord || !bRet)
	{
		return;
	}

	if (!apFileRecord->IsDirectory())
	{
		CONST CAttrBase * pDataAttrBase = apFileRecord->FindFirstAttr(ATTR_TYPE_DATA);
		while (pDataAttrBase)
		{
			CNtfsAttrReader NtfsAttrReader(pDataAttrBase);
			LONGLONG llFileSize = 0;
			NtfsAttrReader.GetFileSizeEx(&llFileSize);
			CONST ULONG BUFF_SIZE = 3*1024;
			CHeapPtr<BYTE> apBuffer;
			apBuffer.Allocate(BUFF_SIZE);
			ULONG ulCurrentToRead = 0;
			ULONG ulRead = 0;
			while (llFileSize)
			{
				ulCurrentToRead = BUFF_SIZE;
				if (llFileSize < BUFF_SIZE)
				{
					ulCurrentToRead = llFileSize;
				}

				ulRead = 0;
				NtfsAttrReader.Read(ulCurrentToRead, &ulRead, apBuffer);

				llFileSize -= ulCurrentToRead;
			}

			pDataAttrBase = apFileRecord->FindNextAttr(ATTR_TYPE_DATA);
		}
		return;
	}
	
	CEnumWorker worker(bShowFullPath);

	CNtfsFileOp fileOp(worker);
	CNtfsDirOp dirOp(worker);
	CNtfsDirPostOp dirPostOp(worker);
	CNtfsParserHelper::EnumHelper(apNTFSVolume, apFileRecord, &fileOp, &dirOp, &dirPostOp);
}


VOID NtfsEnumDirEx(LPCTSTR pszFullPath, BOOL bShowFullPath, LPCTSTR pszIs, LPCTSTR pszIsNot)
{
	CString strRoot;
	CString strGuid;
	CVolumeHelper::GetVolumeRootAndGuidFromPath(pszFullPath, strRoot, strGuid);
	if (0 == strGuid.GetLength())
	{
		strGuid = strRoot;
	}
	strGuid.TrimRight(TEXT("\\/"));
	CLocalFileDevice FileDeviceVolume;
	CComBSTR bstrFileName = strGuid.GetString();
	FileDeviceVolume.Create(
		bstrFileName,//BSTR bstrFileName, 
		GENERIC_READ,//ULONG ulDesiredAccess, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,//ULONG ulShareMode, 
		OPEN_EXISTING,//ULONG ulCreationDisposition, 
		0//ULONG ulFlagsAndAttributes
		);

	CString strPath = PTR_TO_STRING(pszFullPath);
	strPath.Insert(0, TEXT("\\"));
	strPath.Replace(strRoot, TEXT(""));

	CEnumWorker worker(bShowFullPath);
	worker.SetPattern(pszIs, pszIsNot);

	CNtfsFileOp fileOp(worker);
	CNtfsDirOp dirOp(worker);
	CNtfsDirPostOp dirPostOp(worker);

	CString strRootTmp = pszFullPath;
	strRootTmp.TrimRight(TEXT("\\/"));
	worker.EnterRoot(strRootTmp);
	CNtfsParserHelper::Enum(&FileDeviceVolume, strPath, &fileOp, &dirOp, &dirPostOp);
	worker.LeaveRoot(NULL);
}

VOID EnumFindFilesTest(LPCTSTR pszFullPath,LPCTSTR pszIs, LPCTSTR pszIsNot)
{
	vector<CString> files;
	vector<LONGLONG> fileRefers;

	CString strRoot;
	CString strGuid;
	CVolumeHelper::GetVolumeRootAndGuidFromPath(pszFullPath, strRoot, strGuid);
	if (0 == strGuid.GetLength())
	{
		strGuid = strRoot;
	}
	strGuid.TrimRight(TEXT("\\/"));
	CLocalFileDevice FileDeviceVolume;
	CComBSTR bstrFileName = strGuid.GetString();
	FileDeviceVolume.Create(
		bstrFileName,//BSTR bstrFileName, 
		GENERIC_READ,//ULONG ulDesiredAccess, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,//ULONG ulShareMode, 
		OPEN_EXISTING,//ULONG ulCreationDisposition, 
		0//ULONG ulFlagsAndAttributes
		);

	CString strPath = PTR_TO_STRING(pszFullPath);
	strPath.Insert(0, TEXT("\\"));
	strPath.Replace(strRoot, TEXT(""));

	CNtfsParserHelper::EnumFindFiles(
		&FileDeviceVolume,//IFileDeviceObject * pFileDeviceVolume, 
		strPath,//LPCTSTR pszRootPath,
		pszIs,//LPCTSTR pszIS,
		pszIsNot,//LPCTSTR pszISNOT,
		&files,//vector<CString> & files
		&fileRefers
		);
}

//<sonmi01>2012-8-15 ###???
//////////////////////////////////////////////////////////////////////////
VOID GetFileDataRun(CFileRecord * pFileRecord, vector<DataRun_Entry> & vecDR)
{
	CONST CAttrBase * pAttrData = pFileRecord->FindFirstAttr(ATTR_TYPE_DATA);
	while (pAttrData)
	{
		if (pAttrData->IsNonResident() && pAttrData->IsDataRunOK())
		{
			CONST CAttrNonResident * pnr = static_cast<CONST CAttrNonResident *>(pAttrData);
			CONST CDataRunList & drlst = pnr->GetDataRunList();

			CONST DataRun_Entry *dr = drlst.FindFirstEntry();
			while (dr)
			{
				vecDR.push_back(*dr);
				dr = drlst.FindNextEntry();
			}
		}

		pAttrData = pFileRecord->FindNextAttr(ATTR_TYPE_DATA);
	}
}

VOID PrintDataRUn(CONST vector<DataRun_Entry> & vecDR)
{
	DSTART(0);
	for (LONG ii = 0;  ii < vecDR.size(); ++ ii)
	{
		_OLD_DINFO((0, DRNPOS, TEXT("%u:\t LCN=%I64d, Clusters=%I64d, StartVCN=%I64d, LastVCN=%I64d"), 
		ii,
		vecDR[ii].m_LCN,		// -1 to indicate sparse data
		vecDR[ii].m_Clusters,
		vecDR[ii].m_StartVCN,
		vecDR[ii].m_LastVCN));
	}
}

VOID EnumFilesDataRun( 
	IN IFileDeviceObject * pFileDeviceVolume, 
	IN LPCTSTR pszRootPath, 
	IN LPCTSTR pszIS, 
	IN LPCTSTR pszISNOT, 
	OUT vector<CString> * pFiles,
	OUT vector<LONGLONG> * pFileRefences,
	OUT vector<vector<DataRun_Entry> > * pDataRun)
{

	vector<CString> FileNames;
	vector<LONGLONG> FileRefences;
	CNtfsParserHelper::EnumFindFiles( 
		pFileDeviceVolume,//IFileDeviceObject * pFileDeviceVolume, 
		pszRootPath,//LPCTSTR pszRootPath, 
		pszIS,//LPCTSTR pszIS, 
		pszISNOT,//LPCTSTR pszISNOT, 
		&FileNames,//vector<CString> * pFiles,
		&FileRefences//vector<LONGLONG> * pFileRefences
		);

	CAutoPtr<CNTFSVolume> apNTFSVolume;
	LONGLONG Newfp = 0;
	pFileDeviceVolume->SetPointerEx(0, &Newfp, FILE_BEGIN);
	CNtfsParserHelper::InitVolume(pFileDeviceVolume, &apNTFSVolume.m_p);

	pDataRun->resize(FileRefences.size());

	TCHAR FileName[1024] = {0};

	for (size_t ii = 0; ii < FileRefences.size(); ++ ii)
	{
		CAutoPtr<CFileRecord> apFileRecord;
		CNtfsParserHelper::ParseFileRecordByRefecenceNumber(apNTFSVolume, FileRefences[ii], &apFileRecord.m_p);

		FileName[0] = 0;
		apFileRecord->GetFileName(FileName, _countof(FileName));

		if (apFileRecord->IsDirectory())
		{
			_OLD_DINFO((0, DRNPOS, TEXT("FileRefNumber [%I64d] Name [%s, %s] is directory"), FileRefences[ii], FileName, FileNames[ii].GetString()));
		}
		else
		{
			ZeroMemory((VOID*)&((*pDataRun)[ii]), sizeof(DataRun_Entry));
			GetFileDataRun(apFileRecord, (*pDataRun)[ii]);
			_OLD_DINFO((0, DRNPOS, TEXT("FileRefNumber [%I64d] Name [%s, %s] is file"), FileRefences[ii], FileName, FileNames[ii].GetString()));
			PrintDataRUn((*pDataRun)[ii]);
		}
	}

}
//////////////////////////////////////////////////////////////////////////