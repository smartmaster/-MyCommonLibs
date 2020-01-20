#pragma once

#include "stdafx.h"

#include "AutoHandleEx.h"
#include "MiscHelper.h"

struct IFileDirUserOperation
{
	virtual INT Operate(LPCTSTR pszName) = 0;
};

struct CEnumFilesNonRecursive
{
	static VOID ComposeFullFilePath(LPCTSTR pszRoot, LPCTSTR pszFile, CString & strFullPath);
	static BOOL IsDirectory(LPCTSTR pszName);
	static BOOL IsDotDirectory(LPCTSTR pszName);

	static VOID EnumFiles(LPCTSTR pszRootFolder, LPCTSTR pszFileSpec, IFileDirUserOperation * pIFileOperation);
	static VOID EnumSubDirectories(LPCTSTR pszRootFolder,  IFileDirUserOperation * pIDirOperation);
};

struct CEnumFilesRecursive
{
	//static VOID EnumFiles( 
	//	LPCTSTR pszRootFolder, 
	//	LPCTSTR pszFileSpec, 
	//	IFileDirUserOperation * pIFileUserOperation, 
	//	IFileDirUserOperation * pISubDirUserPreOperation, 
	//	IFileDirUserOperation * pISubDirUserOperation);

	class CEnumSubDir : public IFileDirUserOperation
	{
	private:
		LPCTSTR m_pszFileSpec; 
		IFileDirUserOperation * m_pIFileUserOperation; 
		IFileDirUserOperation * m_pISubDirUserPreOperation; 
		IFileDirUserOperation * m_pISubDirUserPostOperation;

	public:
		CEnumSubDir(
			LPCTSTR pszFileSpec, 
			IFileDirUserOperation * pIFileUserOperation, 
			IFileDirUserOperation * pISubDirUserPreOperation, 
			IFileDirUserOperation * pISubDirUserPostOperation);

		virtual INT Operate(LPCTSTR pszName);
	};

	static VOID Enum(
		LPCTSTR pszRootFolder, 
		LPCTSTR pszFileSpec, 
		IFileDirUserOperation * pIFileUserOperation, 
		IFileDirUserOperation * pISubDirUserPreOperation, 
		IFileDirUserOperation * pISubDirUserPostOperation);
};