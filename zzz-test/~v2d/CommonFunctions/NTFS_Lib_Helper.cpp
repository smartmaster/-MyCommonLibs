#include "stdafx.h"
#include "NTFS_Lib_Helper.h"

INT CNtfsParserHelper::InitVolume( IFileDeviceObject * pFileVolume, CNTFSVolume ** ppNTFSVolume )
{
	INT nStatus = 0;
	CAutoPtr<CNTFSVolume>  apNtfsVolume(new CNTFSVolume(pFileVolume));
	if (!apNtfsVolume->IsVolumeOK())
	{
		_OLD_DAPIERR((0, DRNPOS, TEXT("InitVolume"), E_FAIL, TEXT("Not a valid NTFS volume or NTFS version < 3.0")));
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
			_OLD_DSETLASTSTATUS(E_FAIL);
			nStatus = E_FAIL;
			_OLD_DAPIERR((0, DRNPOS, TEXT("ParseFileRecord"), E_FAIL));
			break;
		}

		bRet = apFileRecord->ParseAttrs();
		if (!bRet)
		{
			_OLD_DSETLASTSTATUS(E_FAIL);
			nStatus = E_FAIL;
			_OLD_DAPIERR((0, DRNPOS, TEXT("ParseAttrs"), E_FAIL));
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
				_OLD_DSETLASTSTATUS(E_FAIL);
				nStatus = E_FAIL;
				_OLD_DAPIERR((0, DRNPOS, TEXT("FindSubEntry"), E_FAIL, TEXT("[SubDir=%s]"), strSubDir));
				break;
			}

			bRet = apFileRecord->ParseFileRecord(ie.GetFileReference());
			if (!bRet)
			{
				_OLD_DSETLASTSTATUS(E_FAIL);
				nStatus = E_FAIL;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ParseFileRecord"), E_FAIL, TEXT("[SubDir=%s]"), strSubDir));
				break;
			}

			bRet = apFileRecord->ParseAttrs();
			if (!bRet)
			{
				_OLD_DSETLASTSTATUS(E_FAIL);
				nStatus = E_FAIL;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ParseAttrs"), E_FAIL, TEXT("[SubDir=%s]"), strSubDir));
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
			_OLD_DSETLASTSTATUS(E_FAIL);
			nStatus = E_FAIL;
			_OLD_DAPIERR((0, DRNPOS, TEXT("FindSubEntry"), E_FAIL, TEXT("[SubDir=%s]"), pszName));
			break;
		}

		bRet = apFileRecord->ParseFileRecord(ie.GetFileReference());
		if (!bRet)
		{
			_OLD_DSETLASTSTATUS(E_FAIL);
			nStatus = E_FAIL;
			_OLD_DAPIERR((0, DRNPOS, TEXT("ParseFileRecord"), E_FAIL, TEXT("[SubDir=%s]"), pszName));
			break;
		}

		apFileRecord->SetAttrMask(MASK_ALL);
		bRet = apFileRecord->ParseAttrs();
		if (!bRet)
		{
			_OLD_DSETLASTSTATUS(E_FAIL);
			nStatus = E_FAIL;
			_OLD_DAPIERR((0, DRNPOS, TEXT("ParseAttrs"), E_FAIL, TEXT("[SubDir=%s]"), pszName));
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
		if (-1 == nIndex)
		{
			_OLD_DSETLASTSTATUS(E_INVALIDARG);
			nStatus = E_INVALIDARG;
			_OLD_DAPIERR((0, DRNPOS, TEXT("FindAndParseFileRecord"), E_INVALIDARG, TEXT("[path=%s]"), pszFullPath));
			break;
		}

		CString strName = strFullPath.Mid(nIndex + 1);
		CString strPath = strFullPath.Left(nIndex);
		nStatus = FindAndParseFileRecordHelper(pNTFSVolume, strPath, strName, ppFileRecord);

	} while (FALSE);


	return nStatus;
}

INT CNtfsParserHelper::EnumHelper( CNTFSVolume * pNTFSVolume, CFileRecord * pFileRecord, ISubEntryCallBack * pSubEntryCallBackFile, ISubEntryCallBack * pSubEntryCallBackPreDir, ISubEntryCallBack * pSubEntryCallBackPostDir )
{
	CAutoPtr<CSubEntryCallBack> apSubEntryCallBack(new CSubEntryCallBack(pNTFSVolume, pFileRecord, pSubEntryCallBackFile, pSubEntryCallBackPreDir, pSubEntryCallBackPostDir));
	pFileRecord->TraverseSubEntriesEx(apSubEntryCallBack);
	apSubEntryCallBack.Free();
	return 0;
}

INT CNtfsParserHelper::Enum( IFileDeviceObject * pFileDeviceVolume, LPCTSTR pszPath, ISubEntryCallBack * pSubEntryCallBackFile, ISubEntryCallBack * pSubEntryCallBackPreDir, ISubEntryCallBack * pSubEntryCallBackPostDir )
{
	INT nStatus = 0;

	do 
	{
		//******************************************
		CAutoPtr<CNTFSVolume> apNTFSVolume;
		CNtfsParserHelper::InitVolume(pFileDeviceVolume, &apNTFSVolume.m_p);
		if (NULL == apNTFSVolume.m_p || !apNTFSVolume->IsVolumeOK())
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("InitVolume"), E_FAIL));
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
				_OLD_DAPIERR((0, DRNPOS, TEXT("ParseFileRecord(MFT_IDX_ROOT)"), E_FAIL));
				nStatus = E_FAIL;
				break;
			}
			bRet = apFileRecord->ParseAttrs();
			if (!bRet)
			{
				_OLD_DAPIERR((0, DRNPOS, TEXT("ParseAttrs"), E_FAIL));
				nStatus = E_FAIL;
				break;
			}
		}
		else
		{
			CNtfsParserHelper::FindAndParseFileRecord(apNTFSVolume, pszPath, &apFileRecord.m_p);
			if (NULL == apFileRecord.m_p)
			{
				_OLD_DAPIERR((0, DRNPOS, TEXT("FindAndParseFileRecord"), E_FAIL));
				nStatus = E_FAIL;
				break;
			}
		}

		//******************************************
		if (!apFileRecord->IsDirectory())
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("IsDirectory"), E_FAIL));
			nStatus = E_FAIL;
			break;
		}

		//******************************************
		CNtfsParserHelper::EnumHelper(apNTFSVolume, apFileRecord, pSubEntryCallBackFile, pSubEntryCallBackPreDir, pSubEntryCallBackPostDir);

	} while (FALSE);


	return nStatus;
}

VOID CNtfsParserHelper::EnumFindFiles( 
	IFileDeviceObject * pFileDeviceVolume, 
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
			_OLD_DSETLASTSTATUS(E_FAIL);
			nStatus = E_FAIL;
			_OLD_DAPIERR((0, DRNPOS, TEXT("ParseFileRecord"), E_FAIL, TEXT("[FileReference=%I64d]"), llFileReference));
			break;
		}

		apFileRecord->SetAttrMask(MASK_ALL);
		bRet = apFileRecord->ParseAttrs();
		if (!bRet)
		{
			_OLD_DSETLASTSTATUS(E_FAIL);
			nStatus = E_FAIL;
			_OLD_DAPIERR((0, DRNPOS, TEXT("ParseAttrs"), E_FAIL, TEXT("[FileReference=%I64d]"), llFileReference));
			break;
		}

		*ppFileRecord = apFileRecord.Detach();

	} while (FALSE);

	return nStatus;
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

CNtfsAttrReader::CNtfsAttrReader( CONST CAttrBase * pAttrBase ) :
m_pAttrBase(pAttrBase),
	m_FilePointer(0)
{
	m_TotalSize = m_pAttrBase->GetDataSize();
}

HRESULT STDMETHODCALLTYPE CNtfsAttrReader::QueryInterface( REFIID riid, void **ppvObject )
{
	return S_OK;
}

ULONG STDMETHODCALLTYPE CNtfsAttrReader::AddRef( void )
{
	return 1;
}

ULONG STDMETHODCALLTYPE CNtfsAttrReader::Release( void )
{
	return 1;
}

HRESULT STDMETHODCALLTYPE CNtfsAttrReader::Create( BSTR bstrFileName, ULONG ulDesiredAccess, ULONG ulShareMode, ULONG ulCreationDisposition, ULONG ulFlagsAndAttributes )
{
	m_FilePointer = 0;
	m_TotalSize = m_pAttrBase->GetDataSize();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CNtfsAttrReader::SetPointerEx( LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod )
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

HRESULT STDMETHODCALLTYPE CNtfsAttrReader::Read( ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer )
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

HRESULT STDMETHODCALLTYPE CNtfsAttrReader::Write( ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, BYTE* pBuffer )
{
	return E_FAIL;
}

HRESULT STDMETHODCALLTYPE CNtfsAttrReader::Close( void )
{
	m_FilePointer = 0;
	m_TotalSize = 0;
	m_pAttrBase = NULL;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CNtfsAttrReader::GetFileSizeEx( LONGLONG* pllFileSize )
{
	*pllFileSize = m_TotalSize;
	return S_OK;
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
m_pFileReferences(pFileReferences)
{
}

VOID CNtfsParserHelper::CEnumFindFileWorker::SetPattern( LPCTSTR pszIs, LPCTSTR pszIsNot )
{
	m_strIs = PTR_TO_STRING(pszIs);
	m_strIsNot = PTR_TO_STRING(pszIsNot);
}

VOID CNtfsParserHelper::CEnumFindFileWorker::EnterDir( CONST CIndexEntry * pie )
{
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