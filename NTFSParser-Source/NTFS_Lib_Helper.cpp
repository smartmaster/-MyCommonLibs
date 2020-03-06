#include "stdafx.h"

#include "..\PublicHeader\PublicVMemPtr.h"

#include "NTFS_Lib_Helper.h"

INT CNtfsParserHelper::InitVolume( IFileDevice * pFileVolume, CNTFSVolume ** ppNTFSVolume )
{
	INT nStatus = 0;
	CAutoPtr<CNTFSVolume>  apNtfsVolume(new CNTFSVolume(pFileVolume));
	if (!apNtfsVolume->IsVolumeOK())
	{
		D_API_ERR(0, /*DRNPOS,*/ TEXT("InitVolume"), E_FAIL, TEXT("Not a valid NTFS volume or NTFS version < 3.0"));
		nStatus = E_FAIL;
	}
	else
	{
		*ppNTFSVolume = apNtfsVolume.Detach();
	}
	return nStatus;
}

INT CNtfsParserHelper::FindAndParseFileRecordHelper( CONST CNTFSVolume * pNTFSVolume, LPCTSTR pszPath, LPCTSTR pszName, CFileRecord ** ppFileRecord )
{
	INT nStatus = 0 ;

	do 
	{
		CAutoPtr<CFileRecord> apFileRecord(new CFileRecord(pNTFSVolume));
		apFileRecord->SetAttrMask(MASK_INDEX_ROOT | MASK_INDEX_ALLOCATION);

		BOOL bRet = apFileRecord->ParseFileRecord(MFT_IDX_ROOT);
		if (!bRet)
		{
			D_SET_LAST_STATUS(E_FAIL, -1);
			nStatus = E_FAIL;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("ParseFileRecord"), E_FAIL, TEXT(""));
			break;
		}

		bRet = apFileRecord->ParseAttrs();
		if (!bRet)
		{
			D_SET_LAST_STATUS(E_FAIL, -1);
			nStatus = E_FAIL;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("ParseAttrs"), E_FAIL, TEXT(""));
			break;
		}

		CIndexEntry ie;
		CString strPath = PTR_TO_STRING(pszPath);
		INT nStart = 0;
		for (;;)
		{
			CString strSubDir = strPath.Tokenize(TEXT("\\/"), nStart);
			if ((-1) == nStart || 0 == strSubDir.GetLength())
			{
				break;
			}

			bRet = apFileRecord->FindSubEntry(strSubDir, ie);
			if (!bRet)
			{
				D_SET_LAST_STATUS(E_FAIL, -1);
				nStatus = E_FAIL;
				D_API_ERR(0, /*DRNPOS,*/ TEXT("FindSubEntry"), E_FAIL, TEXT("[SubDir=%s]"), strSubDir);
				break;
			}

			bRet = apFileRecord->ParseFileRecord(ie.GetFileReference());
			if (!bRet)
			{
				D_SET_LAST_STATUS(E_FAIL, -1);
				nStatus = E_FAIL;
				D_API_ERR(0, /*DRNPOS,*/ TEXT("ParseFileRecord"), E_FAIL, TEXT("[SubDir=%s]"), strSubDir);
				break;
			}

			bRet = apFileRecord->ParseAttrs();
			if (!bRet)
			{
				D_SET_LAST_STATUS(E_FAIL, -1);
				nStatus = E_FAIL;
				D_API_ERR(0, /*DRNPOS,*/ TEXT("ParseAttrs"), E_FAIL, TEXT("[SubDir=%s]"), strSubDir);
				break;
			}
		}
		if (nStatus)
		{
			break;
		}


		bRet = apFileRecord->FindSubEntry(pszName, ie);
		if (!bRet)
		{
			D_SET_LAST_STATUS(E_FAIL, -1);
			nStatus = E_FAIL;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("FindSubEntry"), E_FAIL, TEXT("[SubDir=%s]"), pszName);
			break;
		}

		bRet = apFileRecord->ParseFileRecord(ie.GetFileReference());
		if (!bRet)
		{
			D_SET_LAST_STATUS(E_FAIL, -1);
			nStatus = E_FAIL;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("ParseFileRecord"), E_FAIL, TEXT("[SubDir=%s]"), pszName);
			break;
		}

		apFileRecord->SetAttrMask(MASK_ALL);
		bRet = apFileRecord->ParseAttrs();
		if (!bRet)
		{
			D_SET_LAST_STATUS(E_FAIL, -1);
			nStatus = E_FAIL;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("ParseAttrs"), E_FAIL, TEXT("[SubDir=%s]"), pszName);
			break;
		}

		*ppFileRecord = apFileRecord.Detach();

	} while (FALSE);

	return nStatus;
}

INT CNtfsParserHelper::FindAndParseFileRecord( CONST CNTFSVolume * pNTFSVolume, LPCTSTR pszFullPath, CFileRecord ** ppFileRecord )
{
	INT nStatus = 0;

	do 
	{
		CString strFullPath = PTR_TO_STRING(pszFullPath);
		strFullPath.TrimRight(TEXT("\\/"));
		INT nIndex = strFullPath.ReverseFind(TEXT('\\'));
		//if (-1 == nIndex)
		//{
		//	D_SET_LAST_STATUS(E_INVALIDARG, -1);
		//	nStatus = E_INVALIDARG;
		//	D_API_ERR(0, /*DRNPOS,*/ TEXT("FindAndParseFileRecord"), E_INVALIDARG, TEXT("[path=%s]"), pszFullPath);
		//	break;
		//}

		CString strName = strFullPath.Mid(nIndex + 1);
		CString strPath = strFullPath.Left(nIndex);
		nStatus = FindAndParseFileRecordHelper(pNTFSVolume, strPath, strName, ppFileRecord);

	} while (FALSE);


	return nStatus;
}

INT CNtfsParserHelper::EnumHelper( CNTFSVolume * pNTFSVolume, CFileRecord * pFileRecord, ISubEntryCallBack * pSubEntryCallBackFile, ISubEntryCallBack * pSubEntryCallBackPreDir, ISubEntryCallBack * pSubEntryCallBackPostDir )
{
	CAutoPtr<CSubEntryCallBack> apSubEntryCallBack(new CSubEntryCallBack(pNTFSVolume, pFileRecord, pSubEntryCallBackFile, pSubEntryCallBackPreDir, pSubEntryCallBackPostDir));
	pFileRecord->TraverseSubEntriesEx(apSubEntryCallBack/*, FALSE*/);
	apSubEntryCallBack.Free();
	return 0;
}

INT CNtfsParserHelper::Enum( IFileDevice * pFileDeviceVolume, LPCTSTR pszPath, ISubEntryCallBack * pSubEntryCallBackFile, ISubEntryCallBack * pSubEntryCallBackPreDir, ISubEntryCallBack * pSubEntryCallBackPostDir )
{
	INT nStatus = 0;

	do 
	{
		//******************************************
		CAutoPtr<CNTFSVolume> apNTFSVolume;
		CNtfsParserHelper::InitVolume(pFileDeviceVolume, &apNTFSVolume.m_p);
		if (NULL == apNTFSVolume.m_p || !apNTFSVolume->IsVolumeOK())
		{
			D_API_ERR(0, /*DRNPOS,*/ TEXT("InitVolume"), E_FAIL, TEXT(""));
			nStatus = E_FAIL;
			break;;
		}

		//******************************************
		CAutoPtr<CFileRecord> apFileRecord;
		if (TEXT('\\') == pszPath[0] && 0 == pszPath[1])
		{
			apFileRecord.Attach(new CFileRecord(apNTFSVolume));
			apFileRecord->SetAttrMask(MASK_INDEX_ROOT | MASK_INDEX_ALLOCATION);
			BOOL bRet = apFileRecord->ParseFileRecord(MFT_IDX_ROOT);
			if (!bRet)
			{
				D_API_ERR(0, /*DRNPOS,*/ TEXT("ParseFileRecord(MFT_IDX_ROOT)"), E_FAIL, TEXT(""));
				nStatus = E_FAIL;
				break;
			}
			bRet = apFileRecord->ParseAttrs();
			if (!bRet)
			{
				D_API_ERR(0, /*DRNPOS,*/ TEXT("ParseAttrs"), E_FAIL, TEXT(""));
				nStatus = E_FAIL;
				break;
			}
		}
		else
		{
			CNtfsParserHelper::FindAndParseFileRecord(apNTFSVolume, pszPath, &apFileRecord.m_p);
			if (NULL == apFileRecord.m_p)
			{
				D_API_ERR(0, /*DRNPOS,*/ TEXT("FindAndParseFileRecord"), E_FAIL, TEXT(""));
				nStatus = E_FAIL;
				break;
			}
		}

		//******************************************
		if (!apFileRecord->IsDirectory())
		{
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IsDirectory"), E_FAIL, TEXT(""));
			nStatus = E_FAIL;
			break;
		}

		//******************************************
		CNtfsParserHelper::EnumHelper(apNTFSVolume.m_p, apFileRecord.m_p, pSubEntryCallBackFile, pSubEntryCallBackPreDir, pSubEntryCallBackPostDir);
		apFileRecord.Free();

	} while (FALSE);


	return nStatus;
}

VOID CNtfsParserHelper::EnumFindFiles( 
	IFileDevice * pFileDeviceVolume, 
	LPCTSTR pszRootPath, 
	LPCTSTR pszIS, 
	LPCTSTR pszISNOT, 
	vector<CString> * pFiles,
	vector<LONGLONG> * pFileRefences)
{
	CEnumFindFileWorker worker(pFiles, pFileRefences);
	worker.SetPattern(pszIS, pszISNOT);

	CEnumFindFileOp fileOp(worker);
	CEnumFindDirPreOp dirOp(worker);
	CEnumFindDirPostOp dirPostOp(worker);

	CNtfsParserHelper::Enum(pFileDeviceVolume, pszRootPath, &fileOp, &dirOp, &dirPostOp);
}

INT CNtfsParserHelper::ParseFileRecordByRefecenceNumber( CONST CNTFSVolume * pNTFSVolume, LONGLONG llFileReference, CFileRecord ** ppFileRecord )
{
	INT nStatus = 0 ;

	do 
	{
		CAutoPtr<CFileRecord> apFileRecord(new CFileRecord(pNTFSVolume));

		BOOL bRet = apFileRecord->ParseFileRecord(llFileReference);
		if (!bRet)
		{
			D_SET_LAST_STATUS(E_FAIL, -1);
			nStatus = E_FAIL;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("ParseFileRecord"), E_FAIL, TEXT("[FileReference=%I64d]"), llFileReference);
			break;
		}

		apFileRecord->SetAttrMask(MASK_ALL);
		bRet = apFileRecord->ParseAttrs();
		if (!bRet)
		{
			D_SET_LAST_STATUS(E_FAIL, -1);
			nStatus = E_FAIL;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("ParseAttrs"), E_FAIL, TEXT("[FileReference=%I64d]"), llFileReference);
			break;
		}

		*ppFileRecord = apFileRecord.Detach();

	} while (FALSE);

	return nStatus;
}

//<sonmi01>2015-4-24 ###???
HRESULT CNtfsParserHelper::FindPathForRefecenceNumber(CONST CNTFSVolume * pNTFSVolume, LONGLONG llFileReference, IFindPathForRefecenceNumberCallback * pCallback)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	for (;;)
	{
		//////////////////////////////////////////////////////////////////////////
		CAutoPtr<CFileRecord> apFileRecord;
		INT status = ParseFileRecordByRefecenceNumber(pNTFSVolume, llFileReference, &apFileRecord.m_p);
		if (0 != status || nullptr == apFileRecord.m_p)
		{
			hr = E_FAIL;
			D_API_ERR(0, TEXT("ParseFileRecordByRefecenceNumber"), hr, TEXT("FileReference=%I64d"), llFileReference);
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		pCallback->Operate(apFileRecord.m_p);

		//////////////////////////////////////////////////////////////////////////
		CAttr_FileName * Attr_FileName = (CAttr_FileName *)apFileRecord->FindFirstAttr(ATTR_TYPE_FILE_NAME);
		LONG AttrIndex = 1;
		LONG attrCount = apFileRecord->GetAttrCount(ATTR_TYPE_FILE_NAME);
		while (nullptr == Attr_FileName && AttrIndex < attrCount)
		{
			Attr_FileName = (CAttr_FileName *)apFileRecord->FindNextAttr(ATTR_TYPE_FILE_NAME);
			++ AttrIndex;
		}
		if (nullptr == Attr_FileName)
		{
			hr = E_FAIL;
			D_API_ERR(0, TEXT("Attribute file name not found"), hr, TEXT("FileReference=%I64d"), llFileReference);
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		ULONGLONG parentRefence = Attr_FileName->GetParentReference();

		if (-1LL == parentRefence)
		{
			hr = E_FAIL;
			D_API_ERR(0, TEXT("ParseFileRecordByRefecenceNumber"), hr, TEXT("FileReference=%I64d"), llFileReference);
			break;
		}

		if (parentRefence == llFileReference)
		{
			D_INFO(0, TEXT("Root file reference found [%I64d, %I64d]"), parentRefence, llFileReference);
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		llFileReference = parentRefence;
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

//<sonmi01>2015-4-24 ###???
INT CNtfsParserHelper::GetReparsePointData(CONST CNTFSVolume * pNTFSVolume, LONGLONG llFileReference, OUT CFileRecord ** ppFileRecord, OUT BYTE ** ppData, vector<CString> & actualPath)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = ParseFileRecordByRefecenceNumber(pNTFSVolume, llFileReference, ppFileRecord);
	if (S_OK != hr || nullptr == *ppFileRecord)
	{
		hr = E_FAIL;
		D_API_ERR(0, TEXT("ParseFileRecordByRefecenceNumber"), hr, TEXT("FileReference=%I64d"), llFileReference);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CONST CAttrBase* AttrRP = (**ppFileRecord).FindFirstAttr(ATTR_TYPE_REPARSE_POINT);
	if (nullptr == AttrRP)
	{
		hr = E_FAIL;
		D_API_ERR(0, TEXT("No attribute $REPARSE_POINT found"), hr, TEXT("FileReference=%I64d"), llFileReference);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	ULONGLONG dataSize = AttrRP->GetDataSize();
	CVMemPtr<BYTE> apBuffer;
	apBuffer.Allocate(dataSize);
	if (nullptr == apBuffer.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT("dataSize=%I64d"), dataSize);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD actural = 0;
	BOOL bREt = AttrRP->ReadData(0, apBuffer.m_pData, dataSize, &actural);
	if (!bREt || (dataSize != actural))
	{
		hr = E_FAIL;
		D_API_ERR(0, TEXT("ReadData"), hr, TEXT("ToRead=%I64d, Read=%d"), dataSize, actural);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	*ppData = apBuffer.Detach();

	//////////////////////////////////////////////////////////////////////////
#if 1
	REPARSE_POINT * rp = (REPARSE_POINT *)(*ppData);
	if (IsRpTagFlag(rp->ReparseTypeFlags, RP_IS_MICROSOFT))
	{
		LONG skippedZero = 0;
		while (0 == rp->u.symbolic.PathBuffer[skippedZero/sizeof(WCHAR)] && skippedZero <= rp->u.symbolic.SubstituteNameLength + rp->u.symbolic.PrintNameLength)
		{
			skippedZero += sizeof(WCHAR);
		}
		if (rp->u.symbolic.SubstituteNameLength > 0)
		{
			CString strTemp;
			LONG bufferLen = rp->u.symbolic.SubstituteNameLength + 8;
			LPTSTR buffer = strTemp.GetBuffer(bufferLen / sizeof(TCHAR));
			ZeroMemory(buffer, bufferLen);
			CopyMemory(buffer, ((BYTE*)rp->u.symbolic.PathBuffer) + rp->u.symbolic.SubstituteNameOffset + skippedZero, rp->u.symbolic.SubstituteNameLength);
			strTemp.ReleaseBuffer();
			actualPath.push_back(strTemp);
		}

		if (rp->u.symbolic.PrintNameLength > 0)
		{
			CString strTemp;
			LONG bufferLen = rp->u.symbolic.PrintNameLength + 8;
			LPTSTR buffer = strTemp.GetBuffer(bufferLen / sizeof(TCHAR));
			ZeroMemory(buffer, bufferLen);
			CopyMemory(buffer, ((BYTE*)rp->u.symbolic.PathBuffer) + rp->u.symbolic.PrintNameOffset + skippedZero, rp->u.symbolic.PrintNameLength);
			strTemp.ReleaseBuffer();
			actualPath.push_back(strTemp);
		}
	}
	else
	{
		D_INFO(0, TEXT("Ignore non-MS reparse point"));
	}
#else
	SYMBOLIC_LINK_REPARSE_DATA * rp2 = (SYMBOLIC_LINK_REPARSE_DATA *)(*ppData);
	LONG processDataSize = 0;
	for (LONG ii = 0; ii < 2 && processDataSize < rp2->PrintNameOffset; ++ii)
	{
		dataSize = rp2->padding2[ii];
		if (dataSize > rp2->PrintNameOffset - processDataSize)
		{
			dataSize = rp2->PrintNameOffset - processDataSize;
		}
		CString strTemp;
		LONG BufferSize = (dataSize + 8);
		LPTSTR buffer = strTemp.GetBuffer(BufferSize / sizeof(TCHAR));
		ZeroMemory(buffer, BufferSize);
		while (((WCHAR*)((BYTE*)rp2->PathBuffer + processDataSize))[0] == 0)
		{
			processDataSize += sizeof(WCHAR);
			if (processDataSize >= rp2->PrintNameOffset)
			{
				D_INFO(0, TEXT("all zeroed data and ignored"));
				break;
			}
		}
		CopyMemory(buffer, (BYTE*)rp2->PathBuffer + processDataSize, dataSize);
		strTemp.ReleaseBuffer();
		actualPath.push_back(strTemp);

		processDataSize += dataSize + sizeof(WCHAR);
		//if (processDataSize >= rp2->PrintNameOffset)
		//{
		//	D_INFO(0, TEXT("all data processed, [ProcessDataSize=%d, DataSize=%d]"), processDataSize, rp2->PrintNameOffset);
		//	break;
		//}
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

CNtfsParserHelper::CSubEntryCallBack::CSubEntryCallBack( CNTFSVolume * pNTFSVolume, CFileRecord * pParentFileRecord, ISubEntryCallBack * pSubEntryCallBackFile, ISubEntryCallBack * pSubEntryCallBackPreDir, ISubEntryCallBack * pSubEntryCallBackPostDir ) :
m_pNTFSVolume(pNTFSVolume),
	m_pParentFileRecord(pParentFileRecord),
	m_pSubEntryCallBackFile(pSubEntryCallBackFile),
	m_pSubEntryCallBackPreDir(pSubEntryCallBackPreDir),
	m_pSubEntryCallBackPostDir(pSubEntryCallBackPostDir)
{

}

INT CNtfsParserHelper::CSubEntryCallBack::Operate( const CIndexEntry *ie )
{
	if (!ie->IsWin32Name())
	{
		return 0;
	}

	if (ie->IsDirectory())
	{
		m_pSubEntryCallBackPreDir->Operate(ie);

		if (MFT_IDX_ROOT != ie->GetFileReference())
		{
			CAutoPtr<CFileRecord> apFileRecord(new CFileRecord(m_pNTFSVolume));
			apFileRecord->SetAttrMask(MASK_INDEX_ROOT | MASK_INDEX_ALLOCATION);
			BOOL bRet = apFileRecord->ParseFileRecord(ie->GetFileReference());
			if (bRet)
			{
				bRet = apFileRecord->ParseAttrs();
			}
			if (bRet)
			{
				CNtfsParserHelper::EnumHelper(m_pNTFSVolume, apFileRecord, m_pSubEntryCallBackFile, m_pSubEntryCallBackPreDir, m_pSubEntryCallBackPostDir);
			}
			apFileRecord.Free();
		}
		
		m_pSubEntryCallBackPostDir->Operate(ie);
	}
	else
	{
		m_pSubEntryCallBackFile->Operate(ie);
	}

	return 0;
}






VOID CNtfsParserHelper::CEnumFindFileWorker::ComposeFullPath( CString & strFullPath )
{
	for (INT ii=0; ii<m_Paths.size(); ++ii)
	{
		strFullPath += TEXT("\\");
		strFullPath += m_Paths[ii];
	}
}

CNtfsParserHelper::CEnumFindFileWorker::CEnumFindFileWorker(vector<CString> * pFiles, vector<LONGLONG> * pFileReferences) :
m_nDepth(0),
m_pFiles(pFiles),
m_pFileReferences(pFileReferences),
m_FileCount(0LL),
m_DirCount(0LL)
{
}

VOID CNtfsParserHelper::CEnumFindFileWorker::SetPattern( LPCTSTR pszIs, LPCTSTR pszIsNot )
{
	m_strIs = PTR_TO_STRING(pszIs);
	m_strIsNot = PTR_TO_STRING(pszIsNot);
}

VOID CNtfsParserHelper::CEnumFindFileWorker::EnterDir( CONST CIndexEntry * pie )
{
	++m_DirCount;

	TCHAR szName[MAX_PATH] = {0};
	pie->GetFileName(szName, _countof(szName));

	++m_nDepth;
	m_Paths.push_back(szName);
}

VOID CNtfsParserHelper::CEnumFindFileWorker::LeaveDir( CONST CIndexEntry * pie )
{
	--m_nDepth;
	m_Paths.pop_back();
}

VOID CNtfsParserHelper::CEnumFindFileWorker::EnterFile( CONST CIndexEntry * pie )
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
	if (bPatternMatch && m_strIsNot.GetLength())
	{
		bPatternMatch = (!CMiscHelper::MatchPattern(szName, m_strIsNot));
	}

	if (bPatternMatch)
	{
		++m_FileCount;

		if (m_pFiles)
		{
			CString strFile;
			ComposeFullPath(strFile);
			m_pFiles->push_back(strFile);
		}

		if (m_pFileReferences)
		{
			LONGLONG fileRefer = pie->GetFileReference();
			m_pFileReferences->push_back(fileRefer);
		}
	}
}

VOID CNtfsParserHelper::CEnumFindFileWorker::LeaveFile( CONST CIndexEntry * pie )
{
	--m_nDepth;
	m_Paths.pop_back();
}

CNtfsParserHelper::CEnumFindFileWorker::~CEnumFindFileWorker()
{
	DEBUG_TRACE2("DirCount=%I64d, FileCount=%I64d", m_DirCount, m_FileCount);
}

CNtfsParserHelper::CEnumFindFileOp::CEnumFindFileOp( CEnumFindFileWorker & worker ) :
m_worker(worker)
{

}

INT CNtfsParserHelper::CEnumFindFileOp::Operate( const CIndexEntry * ie )
{
	m_worker.EnterFile(ie);
	m_worker.LeaveFile(ie);
	return 0;
}

CNtfsParserHelper::CEnumFindDirPreOp::CEnumFindDirPreOp( CEnumFindFileWorker & worker ) :
m_worker(worker)
{

}

INT CNtfsParserHelper::CEnumFindDirPreOp::Operate( const CIndexEntry * ie )
{
	m_worker.EnterDir(ie);
	return 0;
}

CNtfsParserHelper::CEnumFindDirPostOp::CEnumFindDirPostOp( CEnumFindFileWorker & worker ) :
m_worker(worker)
{

}

INT CNtfsParserHelper::CEnumFindDirPostOp::Operate( const CIndexEntry * ie )
{
	m_worker.LeaveDir(ie);
	return 0;
}

CNtfsAttrReader::CNtfsAttrReader( CONST CAttrBase * pAttrBase ) :
	m_pAttrBase(pAttrBase),
	m_FilePointer(0),
	m_RefCount(0)
{

}

LPCTSTR CNtfsAttrReader::WhoAmI()
{
	static CONST TCHAR WHO_AM_I[] = TEXT("CNtfsAttrReader:IFileDevice");
	return WHO_AM_I;
}

HRESULT CNtfsAttrReader::CreateMember()
{
	m_FilePointer = 0;
	m_TotalSize = m_pAttrBase->GetDataSize();
	return S_OK;
}

HRESULT CNtfsAttrReader::DestroyMember()
{
	m_FilePointer = 0;
	m_TotalSize = 0;
	m_pAttrBase = NULL;
	return S_OK;
}

HRESULT CNtfsAttrReader::SetPointerEx( LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod )
{
	if (FILE_BEGIN == ulMoveMethod)
	{
		m_FilePointer = llDistanceToMove;
	}
	else if (FILE_CURRENT == ulMoveMethod)
	{
		m_FilePointer += llDistanceToMove;
	}
	else if (FILE_END == ulMoveMethod)
	{
		m_FilePointer = m_TotalSize + llDistanceToMove;
	}

	if (m_FilePointer < 0)
	{ 
		m_FilePointer = 0;
	}
	if (m_FilePointer > m_TotalSize)
	{
		m_FilePointer = m_TotalSize;
	}

	if (lpNewFilePointer)
	{
		*lpNewFilePointer = m_FilePointer;
	}

	return S_OK;
}

HRESULT CNtfsAttrReader::Read( ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer )
{
	HRESULT hr = S_OK;
	BOOL bRet = m_pAttrBase->ReadData(m_FilePointer, pBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead);
	if (!bRet)
	{
		hr = E_FAIL;
	}
	else
	{
		m_FilePointer += (*lpNumberOfBytesRead);
	}
	return hr;
}

HRESULT CNtfsAttrReader::Write( ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer )
{
	return E_FAIL;
}

HRESULT CNtfsAttrReader::GetFileSize( LONGLONG * pFileSize )
{
	*pFileSize = m_TotalSize;
	return S_OK;
}

HANDLE CNtfsAttrReader::GetRawHandle()
{
	return INVALID_HANDLE_VALUE;
}

HRESULT CreateInstanceCNtfsAttrReader( CONST CAttrBase * pAttrBase, IFileDevice ** ppObj, BOOL bCreateMember )
{
	return CREATE_INSTCANCE_COBJECT(/*CNtfsAttrReader,*/ new CNtfsAttrReader(pAttrBase), ppObj, bCreateMember);
}

#if 0 //<sonmi01>2015-4-23 ###???

//<sonmi01>2015-4-16 ###???
HRESULT ParseReparsePoints_NS::CParseReparsePoints::ParseIndexEntriesMemory(CONST INDEX_ENTRY_RP * entry, LONG length, vector<LONGLONG> & vecFileRef) //no header
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONG parsedSize = 0;
	while (parsedSize < length)
	{
		if (0 == entry->m_OffsetToData)
		{
			D_INFO(0, TEXT("End IndexEntryRP encountered, end parse"));
			break;
		}

		vecFileRef.push_back(entry->m_FileReference & NTFS_FILE_REF_MASK);
		parsedSize += entry->m_SizeOfIndexEntry;
		entry = (INDEX_ENTRY_RP *)((BYTE*)entry + entry->m_SizeOfIndexEntry); //next entry
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT ParseReparsePoints_NS::CParseReparsePoints::ParseIndexEntriesInIndexRoot(CAttr_IndexRoot * attr, vector<LONGLONG> & vecFileRef)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	ULONGLONG dataSize = attr->GetDataSize();
	CVMemPtr<BYTE> apBuffer;
	apBuffer.Allocate(dataSize);
	if (nullptr == apBuffer.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT("dataSize=%I64d"), dataSize);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD actural = 0;
	BOOL bIO = attr->ReadData(0, apBuffer.m_pData, dataSize, &actural);
	if (!bIO || dataSize != actural)
	{
		hr = E_FAIL;
		D_API_ERR(0, TEXT("ReadData"), hr, TEXT("bIO=%d, actural=%d, dataSize=%I64d"), bIO, actural, dataSize);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	if (dataSize < sizeof(ATTR_INDEX_ROOT))
	{
		D_INFO(0, TEXT("Buffer too small, assume no index entries for reparse points. sizeof(ATTR_INDEX_ROOT)=%d, dataSize=%I64d"), sizeof(ATTR_INDEX_ROOT), dataSize);
		LEAVE_BLOCK(0);
	}
	
	ATTR_INDEX_ROOT * indexRootHeader = (ATTR_INDEX_ROOT *)(apBuffer.m_pData);
	D_INFO(0, TEXT("TotalEntrySize=%d, AllocEntrySize=%d"), indexRootHeader->m_TotalEntrySize, indexRootHeader->m_AllocEntrySize);

	INDEX_ENTRY_RP * ieRP = (INDEX_ENTRY_RP*)((BYTE*)(&(indexRootHeader->m_EntryOffset)) + indexRootHeader->m_EntryOffset);
	hr = ParseIndexEntriesMemory(ieRP, dataSize - sizeof(ATTR_INDEX_ROOT), vecFileRef);
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

BOOL ParseReparsePoints_NS::CParseReparsePoints::IsSubNodePtr(CONST INDEX_ENTRY_RP * ieRp)
{
	if (ieRp)
	{
		return (ieRp->m_Flags & INDEX_ENTRY_FLAG_SUBNODE);
	}
	else
	{
		return FALSE;
	}
}

ULONGLONG ParseReparsePoints_NS::CParseReparsePoints::GetSubNodeVCN(CONST INDEX_ENTRY_RP * ieRp) CONST
{
	if (ieRp)
	{
		return *(ULONGLONG*)((BYTE*)ieRp + (ieRp->m_SizeOfIndexEntry - 8));
	}
	else
	{
		return (ULONGLONG)-1;
	}
}
#endif