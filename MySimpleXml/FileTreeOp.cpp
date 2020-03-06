#include "stdafx.h"

#include "..\PublicHeader\PublicMiscHelper.h"
#include "AddFileAttributesInFolder.Serialize.h"
#include "FileTreeOp.h"

#if 0
static INT __AddFileAttrs(LPCTSTR szName, DWORD dwAttr)
{
	INT nStatus = ERROR_SUCCESS;

	do
	{
		DWORD dwAttributes = GetFileAttributes(
			szName//__in  LPCTSTR lpFileName
			);
		if (INVALID_FILE_ATTRIBUTES == dwAttributes)
		{
			nStatus = HRESULT_FROM_WIN32(GetLastError());
			D_API_ERR(0, TEXT("GetFileAttributes"), nStatus, TEXT("%s"), szName);
			nStatus = -1;
			break;
		}

		dwAttributes |= dwAttr;
		BOOL bSetFileAttributes = SetFileAttributes(
			szName,//__in  LPCTSTR lpFileName,
			dwAttributes//__in  DWORD dwFileAttributes
			);
		if (!bSetFileAttributes)
		{
			nStatus = HRESULT_FROM_WIN32(GetLastError());
			D_API_ERR(0, TEXT("SetFileAttributes"), nStatus, TEXT("%s"), szName);
			nStatus = -1;
			break;
		}

	} while (FALSE);

	return nStatus;
}

static INT __RemoveFileAttrs(LPCTSTR szName, DWORD dwAttr)
{
	INT nStatus = ERROR_SUCCESS;

	do
	{
		DWORD dwAttributes = GetFileAttributes(
			szName//__in  LPCTSTR lpFileName
			);
		if (INVALID_FILE_ATTRIBUTES == dwAttributes)
		{
			nStatus = HRESULT_FROM_WIN32(GetLastError());
			D_API_ERR(0, TEXT("GetFileAttributes"), nStatus, TEXT("%s"), szName);
			nStatus = -1;
			break;
		}

		dwAttributes &= (~dwAttr);
		BOOL bSetFileAttributes = SetFileAttributes(
			szName,//__in  LPCTSTR lpFileName,
			dwAttributes//__in  DWORD dwFileAttributes
			);
		if (!bSetFileAttributes)
		{
			nStatus = HRESULT_FROM_WIN32(GetLastError());
			D_API_ERR(0, TEXT("SetFileAttributes"), nStatus, TEXT("%s"), szName);
			nStatus = -1;
			break;
		}

	} while (FALSE);

	return nStatus;
}


static HRESULT __DeleteFile(LPCTSTR strFile)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	DWORD FileAttr = (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN);

	hr = __RemoveFileAttrs(strFile, FileAttr);
	if (SUCCEEDED(hr))
	{
		D_INFO(0, TEXT("Remove attributes %s, 0x%08x"), strFile, FileAttr);
	}
	else
	{
		D_API_ERR(0, TEXT("__RemoveFileAttrs"), hr, TEXT("Failed to Remove attributes %s, 0x%08x"), strFile, FileAttr);
	}

	BOOL bDF = ::DeleteFile(strFile);
	if (!bDF)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("DeleteFile"), hr, TEXT("%s"), strFile);
		//LEAVE_BLOCK(0);
	}
	else
	{
		hr = S_OK;
	}

	return hr;
}


static HRESULT __RemoveDirectory(LPCTSTR strFile)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	DWORD FileAttr = (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN);

	hr = __RemoveFileAttrs(strFile, FileAttr);
	if (SUCCEEDED(hr))
	{
		D_INFO(0, TEXT("Remove attributes %s, 0x%08x"), strFile, FileAttr);
	}
	else
	{
		D_API_ERR(0, TEXT("__RemoveFileAttrs"), hr, TEXT("Failed to Remove attributes %s, 0x%08x"), strFile, FileAttr);
	}

	BOOL bDF = ::RemoveDirectory(strFile);
	if (!bDF)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("RemoveDirectory"), hr, TEXT("%s"), strFile);
		//LEAVE_BLOCK(0);
	}
	else
	{
		hr = S_OK;
	}

	return hr;
}
#endif


CSingleFileOp::CSingleFileOp(DWORD FileAttr, E_HOW how) :
m_FileAttr(FileAttr),
m_FileCount(0),
m_ErrorFileCount(0),
m_how(how)
{
}

INT CSingleFileOp::Operate(CONST CString & strFile)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	switch (m_how)
	{
	case E_HOW::ADD_ATTRIB:
	{
		hr = CMiscHelper::__AddFileAttrs(strFile, m_FileAttr);
		if (SUCCEEDED(hr))
		{
			++m_FileCount;
			D_INFO(0, TEXT("Set attributes %s, 0x%08x"), strFile.GetString(), m_FileAttr);
		}
		else
		{
			++m_ErrorFileCount;
			D_API_ERR(0, TEXT("__AddFileAttrs"), hr, TEXT("Failed to set attributes %s, 0x%08x"), strFile.GetString(), m_FileAttr);
		}
	}
		break;
	case E_HOW::REMOVE_ATTRIB:
	{
		hr = CMiscHelper::__RemoveFileAttrs(strFile, m_FileAttr);
		if (SUCCEEDED(hr))
		{
			++m_FileCount;
			D_INFO(0, TEXT("Remove attributes %s, 0x%08x"), strFile.GetString(), m_FileAttr);
		}
		else
		{
			++m_ErrorFileCount;
			D_API_ERR(0, TEXT("__RemoveFileAttrs"), hr, TEXT("Failed to Remove attributes %s, 0x%08x"), strFile.GetString(), m_FileAttr);
		}
	}
		break;
	case E_HOW::DELETE_FILE:
	{
		hr = CMiscHelper::__DeleteFile(strFile);
		if (SUCCEEDED(hr))
		{
			++m_FileCount;
			D_INFO(0, TEXT("__DeleteFile %s"), strFile.GetString());
		}
		else
		{
			++m_ErrorFileCount;
			D_API_ERR(0, TEXT("__DeleteFile"), hr, TEXT("Failed to delete file %s"), strFile.GetString());
		}


	}
		break;
	case E_HOW::REMOVE_DIR:
	{
		hr = CMiscHelper::__RemoveDirectory(strFile);
		if (SUCCEEDED(hr))
		{
			++m_FileCount;
			D_INFO(0, TEXT("__RemoveDirectory %s"), strFile.GetString());
		}
		else
		{
			++m_ErrorFileCount;
			D_API_ERR(0, TEXT("__RemoveDirectory"), hr, TEXT("Failed to delete directory %s"), strFile.GetString());
		}
	}
	break;
	}
	
	return 0;
}

VOID CSingleFileOp::PrintResult()
{
	D_INFO(0, TEXT("Succeeded file count=%d, Failed file count=%d"), m_FileCount, m_ErrorFileCount);
}

static HRESULT ModifyFileAttributesInFolder(DWORD fileAtrtr, CSingleFileOp::E_HOW how, LPCTSTR Dir, LPCTSTR configFile, BOOL bRecur)
{
	CSingleFileOp AddFileAttrs(fileAtrtr, how);

	static CONST LPCTSTR FILE_EXT_STATIC[] =
	{
		TEXT("*.h"),
		TEXT("*.cpp"),
		TEXT("*.c"),
		TEXT("*.cc"),
		TEXT("*.hpp"),
		TEXT("*.hxx"),
		TEXT("*.hh"),
		TEXT("*.inl"),
		TEXT("*.idl"),
		TEXT("*.asm"),
		TEXT("*.s"),
		TEXT("*.inc"),
		TEXT("*.rc"),
		TEXT("*.cs"),
		TEXT("*.java"),
	};

	AddFileAttributesInFolderSerialize configObj;
	std::vector<LPCTSTR> congigFileExt;
	if (nullptr != configFile && 0 != configFile[0])
	{
		configObj.FromFile(FIELD_NAME(AddFileAttributesInFolderSerialize), configFile);
	}

	for (int ii = 0; ii < configObj.FileExtensions.size(); ++ii)
	{
		congigFileExt.push_back(configObj.FileExtensions[ii].GetString());
	}

	CONST LPCTSTR* FILE_EXT = congigFileExt.size() ? &congigFileExt[0] : FILE_EXT_STATIC;
	INT loopCount = congigFileExt.size() ? congigFileExt.size() : _countof(FILE_EXT_STATIC);
	for (LONG ii = 0; ii < loopCount; ++ii)
	{
		if (bRecur)
		{
			EnumDirectoryFileTree(
				Dir,//LPCTSTR szDirectory, 
				FILE_EXT[ii],//LPCTSTR szFileSpec, 
				NULL,//IEnumFolderFileOperation * pFolderOperation, 
				&AddFileAttrs//IEnumFolderFileOperation * pFileOperation
			);
		}
		else
		{
			EnumDirectoryFileFlat(
				Dir,//LPCTSTR szDirectory, 
				FILE_EXT[ii],//LPCTSTR szFileSpec, 
				&AddFileAttrs//IEnumFolderFileOperation * pFileOperation
			);
		}
	}

	AddFileAttrs.PrintResult();

	return 0;
}


HRESULT RemoveReadonlyInFolder(LPCTSTR Dir, LPCTSTR configFile, BOOL bRecur)
{
	return ModifyFileAttributesInFolder(FILE_ATTRIBUTE_READONLY, CSingleFileOp::E_HOW::REMOVE_ATTRIB, Dir, configFile, bRecur);
}

HRESULT AddReadonlyInFolder(LPCTSTR Dir, LPCTSTR configFile, BOOL bRecur)
{

	return ModifyFileAttributesInFolder(FILE_ATTRIBUTE_READONLY, CSingleFileOp::E_HOW::ADD_ATTRIB, Dir, configFile, bRecur);
#if 0
	CSingleFileOp AddFileAttrs(FILE_ATTRIBUTE_READONLY, CSingleFileOp::E_HOW::ADD_ATTRIB);

	static CONST LPCTSTR FILE_EXT_STATIC[] =
	{
		TEXT("*.h"),
		TEXT("*.cpp"),
		TEXT("*.c"),
		TEXT("*.cc"),
		TEXT("*.hpp"),
		TEXT("*.hxx"),
		TEXT("*.hh"),
		TEXT("*.inl"),
		TEXT("*.idl"),
		TEXT("*.asm"),
		TEXT("*.s"),
		TEXT("*.inc"),
		TEXT("*.rc"),
		TEXT("*.cs"),
		TEXT("*.java"),
	};

	AddFileAttributesInFolderSerialize configObj;
	std::vector<LPCTSTR> congigFileExt;
	if (nullptr != configFile && 0 != configFile[0])
	{
		configObj.FromFile(FIELD_NAME(AddFileAttributesInFolderSerialize), configFile);
	}

	for (int ii = 0; ii < configObj.FileExtensions.size(); ++ ii)
	{
		congigFileExt.push_back(configObj.FileExtensions[ii].GetString());
	}

	CONST LPCTSTR * FILE_EXT = congigFileExt.size() ? &congigFileExt[0] : FILE_EXT_STATIC;
	INT loopCount = congigFileExt.size() ? congigFileExt.size() : _countof(FILE_EXT_STATIC);
	for (LONG ii = 0; ii < loopCount; ++ii)
	{
		if (bRecur)
		{
			EnumDirectoryFileTree(
				Dir,//LPCTSTR szDirectory, 
				FILE_EXT[ii],//LPCTSTR szFileSpec, 
				NULL,//IEnumFolderFileOperation * pFolderOperation, 
				&AddFileAttrs//IEnumFolderFileOperation * pFileOperation
				);
		}
		else
		{
			EnumDirectoryFileFlat(
				Dir,//LPCTSTR szDirectory, 
				FILE_EXT[ii],//LPCTSTR szFileSpec, 
				&AddFileAttrs//IEnumFolderFileOperation * pFileOperation
				);
		}
	}

	AddFileAttrs.PrintResult();

	return 0;
#endif
}

HRESULT AddFileAttributesInFolder(LPCTSTR Dir, DWORD fileAttrib, LPCTSTR fileExt, BOOL bRecur)
{
	CSingleFileOp fileOp(fileAttrib, CSingleFileOp::E_HOW::ADD_ATTRIB);

	if (bRecur)
	{
		EnumDirectoryFileTree(
			Dir,//LPCTSTR szDirectory, 
			fileExt,//LPCTSTR szFileSpec, 
			NULL,//IEnumFolderFileOperation * pFolderOperation, 
			&fileOp//IEnumFolderFileOperation * pFileOperation
			);
	}
	else
	{
		EnumDirectoryFileFlat(
			Dir,//LPCTSTR szDirectory, 
			fileExt,//LPCTSTR szFileSpec, 
			&fileOp//IEnumFolderFileOperation * pFileOperation
			);
	}
	

	fileOp.PrintResult();

	return 0;
}

HRESULT RemoveFileAttributesInFolder(LPCTSTR Dir, DWORD fileAttrib, LPCTSTR fileExt, BOOL bRecur)
{
	CSingleFileOp fileOp(fileAttrib, CSingleFileOp::E_HOW::REMOVE_ATTRIB);

	if (bRecur)
	{
		EnumDirectoryFileTree(
			Dir,//LPCTSTR szDirectory, 
			fileExt,//LPCTSTR szFileSpec, 
			nullptr,//IEnumFolderFileOperation * pFolderOperation, 
			&fileOp//IEnumFolderFileOperation * pFileOperation
			);
	}
	else
	{
		EnumDirectoryFileFlat(
			Dir,//LPCTSTR szDirectory, 
			fileExt,//LPCTSTR szFileSpec, 
			&fileOp//IEnumFolderFileOperation * pFileOperation
			);
	}
	

	fileOp.PrintResult();

	return 0;
}

HRESULT RemoveFileInFolder(LPCTSTR Dir, LPCTSTR fileExt, BOOL bRecur)
{
	CSingleFileOp fileOp(0, CSingleFileOp::E_HOW::DELETE_FILE);
	CSingleFileOp FolderOp(0, CSingleFileOp::E_HOW::REMOVE_DIR);

	if (bRecur)
	{
		EnumDirectoryFileTree(
			Dir,//LPCTSTR szDirectory, 
			fileExt,//LPCTSTR szFileSpec, 
			&FolderOp,//IEnumFolderFileOperation * pFolderOperation, 
			&fileOp//IEnumFolderFileOperation * pFileOperation
			);
	}
	else
	{
		EnumDirectoryFileFlat(
			Dir,//LPCTSTR szDirectory, 
			fileExt,//LPCTSTR szFileSpec, 
			&fileOp//IEnumFolderFileOperation * pFileOperation
			);

	}
	
	fileOp.PrintResult();

	return 0;
}
