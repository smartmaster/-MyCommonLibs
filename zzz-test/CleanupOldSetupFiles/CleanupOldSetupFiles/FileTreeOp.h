#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

//#include "..\PublicHeader\PublicDTraceLib.h"
#include "CommonDef.h"

#include "EnumWinFile.h"

//static INT __AddFileAttrs(LPCTSTR szName, DWORD dwAttr);


class CSingleFileOp : public IEnumFolderFileOperation
{
public:
	enum  class E_HOW
	{
		ADD_ATTRIB,
		REMOVE_ATTRIB,
		DELETE_FILE,
		REMOVE_DIR,
	};
private:
	DWORD m_FileAttr;
	LONG m_FileCount;
	LONG m_ErrorFileCount;
	E_HOW m_how;

public:
	//struct IEnumFolderFileOperation
	//{
	//	virtual INT Operate(CONST CString & strFile) = 0;
	//};

	CSingleFileOp(DWORD FileAttr, E_HOW how);

	virtual INT Operate(CONST CString & strFile);

	VOID PrintResult();
};


HRESULT RemoveFileInFolder(LPCTSTR Dir, LPCTSTR fileExt, BOOL bRecur, BOOL bDeleteSelf);