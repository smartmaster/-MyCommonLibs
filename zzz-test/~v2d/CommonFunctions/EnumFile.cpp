#include "stdafx.h"
#include "EnumFile.h"


VOID CEnumFilesNonRecursive::ComposeFullFilePath( LPCTSTR pszRoot, LPCTSTR pszFile, CString & strFullPath )
{
	strFullPath = pszRoot;
	CMiscHelper::AppendTail(strFullPath, TEXT('\\'));
	strFullPath += pszFile;
}

BOOL CEnumFilesNonRecursive::IsDirectory( LPCTSTR pszName )
{
	BOOL bRet = FALSE;
	DWORD dwAttributes = ::GetFileAttributes(pszName);
	if (INVALID_FILE_ATTRIBUTES != dwAttributes)
	{
		bRet = (FILE_ATTRIBUTE_DIRECTORY == (FILE_ATTRIBUTE_DIRECTORY & dwAttributes));
	}
	return bRet;
}

VOID CEnumFilesNonRecursive::EnumFiles( LPCTSTR pszRootFolder, LPCTSTR pszFileSpec, IFileDirUserOperation * pIFileOperation )
{
	CString strFileSpec;
	ComposeFullFilePath(pszRootFolder, pszFileSpec, strFileSpec);

	do 
	{
		//******************************************
		WIN32_FIND_DATA FindFileData = {0};
		HANDLE  hFind = ::FindFirstFile(
			strFileSpec,//__in   LPCTSTR lpFileName,
			&FindFileData//__out  LPWIN32_FIND_DATA lpFindFileData
			);
		if (INVALID_HANDLE_VALUE == hFind)
		{
			break;
		}

		//******************************************
		CAutoHandleEx<::FindClose> ahFind(hFind); hFind = INVALID_HANDLE_VALUE;
		CString strFileName;
		ComposeFullFilePath(pszRootFolder, FindFileData.cFileName, strFileName);
		if (!IsDirectory(strFileName))
		{
			if (pIFileOperation)
			{
				pIFileOperation->Operate(strFileName);
			}
			
		}

		//******************************************
		for (;;)
		{
			BOOL bRet = ::FindNextFile(
				ahFind,//__in   HANDLE hFindFile,
				&FindFileData//__out  LPWIN32_FIND_DATA lpFindFileData
				);
			if (!bRet)
			{
				break;
			}

			ComposeFullFilePath(pszRootFolder, FindFileData.cFileName, strFileName);
			if (!IsDirectory(strFileName))
			{
				if (pIFileOperation)
				{
					pIFileOperation->Operate(strFileName);
				}
			}
		}

	} while (FALSE);
}

VOID CEnumFilesNonRecursive::EnumSubDirectories( LPCTSTR pszRootFolder, IFileDirUserOperation * pIDirOperation )
{
	CString strDirSpec;
	ComposeFullFilePath(pszRootFolder, TEXT("*"), strDirSpec);

	do 
	{
		//******************************************
		WIN32_FIND_DATA FindFileData = {0};
		HANDLE  hFind = ::FindFirstFile(
			strDirSpec,//__in   LPCTSTR lpFileName,
			&FindFileData//__out  LPWIN32_FIND_DATA lpFindFileData
			);
		if (INVALID_HANDLE_VALUE == hFind)
		{
			break;
		}

		//******************************************
		CAutoHandleEx<::FindClose> ahFind(hFind); hFind = INVALID_HANDLE_VALUE;
		CString strDirName;
		ComposeFullFilePath(pszRootFolder, FindFileData.cFileName, strDirName);
		if (IsDirectory(strDirName) && !IsDotDirectory(FindFileData.cFileName))
		{
			if (pIDirOperation)
			{
				pIDirOperation->Operate(strDirName);
			}
		}

		//******************************************
		for (;;)
		{
			BOOL bRet = ::FindNextFile(
				ahFind,//__in   HANDLE hFindFile,
				&FindFileData//__out  LPWIN32_FIND_DATA lpFindFileData
				);
			if (!bRet)
			{
				break;
			}

			ComposeFullFilePath(pszRootFolder, FindFileData.cFileName, strDirName);
			if (IsDirectory(strDirName) && !IsDotDirectory(FindFileData.cFileName))
			{
				if (pIDirOperation)
				{
					pIDirOperation->Operate(strDirName);
				}
			}
		}

	} while (FALSE);
}

BOOL CEnumFilesNonRecursive::IsDotDirectory( LPCTSTR pszName )
{
	return (0 == _tcscmp(pszName, TEXT(".")) || 0 == _tcscmp(pszName, TEXT("..")) );
}

//VOID CEnumFilesRecursive::EnumFiles( LPCTSTR pszRootFolder, LPCTSTR pszFileSpec, IFileDirUserOperation * pIFileUserOperation, IFileDirUserOperation * pISubDirUserPreOperation, IFileDirUserOperation * pISubDirUserOperation )
//{
//	CEnumFilesNonRecursive::EnumFiles(pszRootFolder, pszFileSpec, pIFileUserOperation);
//
//	CString strDirSpec;
//	CEnumFilesNonRecursive::ComposeFullFilePath(pszRootFolder, TEXT("*"), strDirSpec);
//
//	do 
//	{
//		//******************************************
//		WIN32_FIND_DATA FindFileData = {0};
//		HANDLE  hFind = ::FindFirstFile(
//			strDirSpec,//__in   LPCTSTR lpFileName,
//			&FindFileData//__out  LPWIN32_FIND_DATA lpFindFileData
//			);
//		if (INVALID_HANDLE_VALUE == hFind)
//		{
//			break;
//		}
//
//		//******************************************
//		CAutoHandleEx<::FindClose> ahFind(hFind); hFind = INVALID_HANDLE_VALUE;
//		CString strDirName;
//		CEnumFilesNonRecursive::ComposeFullFilePath(pszRootFolder, FindFileData.cFileName, strDirName);
//		if (CEnumFilesNonRecursive::IsDirectory(strDirName) && !CEnumFilesNonRecursive::IsDotDirectory(FindFileData.cFileName))
//		{
//			if (pISubDirUserPreOperation)
//			{
//				pISubDirUserPreOperation->Operate(strDirName);
//			}
//			EnumFiles(strDirName, pszFileSpec, pIFileUserOperation, pISubDirUserPreOperation, pISubDirUserOperation);
//			if (pISubDirUserOperation)
//			{
//				pISubDirUserOperation->Operate(strDirName);
//			}
//		}
//
//		//******************************************
//		for (;;)
//		{
//			BOOL bRet = ::FindNextFile( ahFind, &FindFileData);
//			if (!bRet)
//			{
//				break;
//			}
//
//			CEnumFilesNonRecursive::ComposeFullFilePath(pszRootFolder, FindFileData.cFileName, strDirName);
//			if (CEnumFilesNonRecursive::IsDirectory(strDirName) && !CEnumFilesNonRecursive::IsDotDirectory(FindFileData.cFileName))
//			{
//				if (pISubDirUserPreOperation)
//				{
//					pISubDirUserPreOperation->Operate(strDirName);
//				}
//				EnumFiles(strDirName, pszFileSpec, pIFileUserOperation, pISubDirUserPreOperation, pISubDirUserOperation);
//				if (pISubDirUserOperation)
//				{
//					pISubDirUserOperation->Operate(strDirName);
//				}
//			}
//
//		}
//
//	} while (FALSE);
//}

VOID CEnumFilesRecursive::Enum( 
	LPCTSTR pszRootFolder, 
	LPCTSTR pszFileSpec, 
	IFileDirUserOperation * pIFileUserOperation, 
	IFileDirUserOperation * pISubDirUserPreOperation, 
	IFileDirUserOperation * pISubDirUserPostOperation )
{
	CEnumFilesNonRecursive::EnumFiles(pszRootFolder, pszFileSpec, pIFileUserOperation);

	CAutoPtr<CEnumSubDir> apEnumSubDir(new CEnumSubDir(pszFileSpec, pIFileUserOperation, pISubDirUserPreOperation, pISubDirUserPostOperation));
	CEnumFilesNonRecursive::EnumSubDirectories(pszRootFolder,  (CEnumSubDir*)apEnumSubDir);
}

CEnumFilesRecursive::CEnumSubDir::CEnumSubDir( LPCTSTR pszFileSpec, IFileDirUserOperation * pIFileUserOperation, IFileDirUserOperation * pISubDirUserPreOperation, IFileDirUserOperation * pISubDirUserPostOperation ) :
m_pszFileSpec(pszFileSpec), 
m_pIFileUserOperation(pIFileUserOperation), 
m_pISubDirUserPreOperation(pISubDirUserPreOperation), 
m_pISubDirUserPostOperation(pISubDirUserPostOperation)
{

}

INT CEnumFilesRecursive::CEnumSubDir::Operate( LPCTSTR pszName )
{
	if (m_pISubDirUserPreOperation)
	{
		m_pISubDirUserPreOperation->Operate(pszName);
	}
	CEnumFilesRecursive::Enum(pszName, m_pszFileSpec, m_pIFileUserOperation, m_pISubDirUserPreOperation, m_pISubDirUserPostOperation);
	if (m_pISubDirUserPostOperation)
	{
		m_pISubDirUserPostOperation->Operate(pszName);
	}
	return 0;
}
