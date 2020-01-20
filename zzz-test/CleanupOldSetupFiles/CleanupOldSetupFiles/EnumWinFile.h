#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


struct IEnumFolderFileOperation
{
	virtual INT Operate(CONST CString & strFile) = 0;
};


INT EnumDirectoryFileFlat(LPCTSTR szDirectory, LPCTSTR szFileSpec, IEnumFolderFileOperation * pFileOperation);

INT EnumDirectoryFileTree(LPCTSTR szDirectory, LPCTSTR szFileSpec, IEnumFolderFileOperation * pFolderOperation, IEnumFolderFileOperation * pFileOperation);
