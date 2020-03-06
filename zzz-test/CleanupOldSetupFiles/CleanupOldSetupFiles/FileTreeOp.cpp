#include "stdafx.h"

//#include "..\PublicHeader\PublicMiscHelper.h"
//#include "AddFileAttributesInFolder.Serialize.h"
#include "FileTreeOp.h"

#if 1

class CMiscHelper
{
public:
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
				//D_API_ERR(0, TEXT("GetFileAttributes"), nStatus, TEXT("%s"), szName);
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
				//D_API_ERR(0, TEXT("SetFileAttributes"), nStatus, TEXT("%s"), szName);
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
			//D_INFO(0, TEXT("Remove attributes %s, 0x%08x"), strFile, FileAttr);
		}
		else
		{
			//D_API_ERR(0, TEXT("__RemoveFileAttrs"), hr, TEXT("Failed to Remove attributes %s, 0x%08x"), strFile, FileAttr);
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
			//D_API_ERR(0, TEXT("DeleteFile"), hr, TEXT("%s"), strFile);
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
			//D_INFO(0, TEXT("Remove attributes %s, 0x%08x"), strFile, FileAttr);
		}
		else
		{
			//D_API_ERR(0, TEXT("__RemoveFileAttrs"), hr, TEXT("Failed to Remove attributes %s, 0x%08x"), strFile, FileAttr);
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
			//D_API_ERR(0, TEXT("RemoveDirectory"), hr, TEXT("%s"), strFile);
			//LEAVE_BLOCK(0);
		}
		else
		{
			hr = S_OK;
		}

		return hr;
	}
};
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
	
	case E_HOW::DELETE_FILE:
	{
		hr = CMiscHelper::__DeleteFile(strFile);
		if (SUCCEEDED(hr))
		{
			++m_FileCount;
			//D_INFO(0, TEXT("__DeleteFile %s"), strFile.GetString());
		}
		else
		{
			++m_ErrorFileCount;
			//D_API_ERR(0, TEXT("__DeleteFile"), hr, TEXT("Failed to delete file %s"), strFile.GetString());
		}


	}
		break;
	case E_HOW::REMOVE_DIR:
	{
		hr = CMiscHelper::__RemoveDirectory(strFile);
		if (SUCCEEDED(hr))
		{
			++m_FileCount;
			//D_INFO(0, TEXT("__RemoveDirectory %s"), strFile.GetString());
		}
		else
		{
			++m_ErrorFileCount;
			//D_API_ERR(0, TEXT("__RemoveDirectory"), hr, TEXT("Failed to delete directory %s"), strFile.GetString());
		}
	}
	break;
	}
	
	return 0;
}

VOID CSingleFileOp::PrintResult()
{
	//D_INFO(0, TEXT("Succeeded file count=%d, Failed file count=%d"), m_FileCount, m_ErrorFileCount);
}




HRESULT RemoveFileInFolder(LPCTSTR Dir, LPCTSTR fileExt, BOOL bRecur, BOOL bDeleteSelf)
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

	if (bDeleteSelf)
	{
		FolderOp.Operate(Dir);
	}
	
	fileOp.PrintResult();

	return 0;
}
