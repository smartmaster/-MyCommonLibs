#include "stdafx.h"
#include "..\CommonFunctions\EnumFile.h"


static INT tabCount = 0;

VOID PrintTabs()
{
	for (INT ii=0; ii<tabCount; ++ii)
	{
		_ftprintf_s(stdout, TEXT("\t"));
	}
}

LPCTSTR GetOnlyFileName(LPCTSTR szName)
{
	LPCTSTR pszName = _tcsrchr(szName, TEXT('\\'));
	if (pszName)
	{
		return pszName + 1;
	}
	else
	{
		return szName;
	}
}

class CFileOp : public IFileDirUserOperation
{
	BOOL m_bShowFullPath;

public:
	CFileOp(BOOL bShowFullPath) :
	  m_bShowFullPath(bShowFullPath)
	  {
	  }

	virtual INT Operate(LPCTSTR pszName)
	{
		++tabCount;
		PrintTabs();
		if (m_bShowFullPath)
		{
			_ftprintf_s(stdout, TEXT("[-]%s") TEXT("\r\n"), pszName);
		}
		else
		{
			_ftprintf_s(stdout, TEXT("[-]%s") TEXT("\r\n"), GetOnlyFileName(pszName) );
		}
		--tabCount;
		return 0;
	}
};

class CDirOp : public IFileDirUserOperation
{
	BOOL m_bShowFullPath;

public:
	CDirOp(BOOL bShowFullPath) :
	m_bShowFullPath(bShowFullPath)
	{
	}

	virtual INT Operate(LPCTSTR pszName)
	{
		++tabCount;
		PrintTabs();
		if (m_bShowFullPath)
		{
			_ftprintf_s(stdout, TEXT("[+]%s") TEXT("\r\n"), pszName);
		}
		else
		{
			_ftprintf_s(stdout, TEXT("[+]%s") TEXT("\r\n"), GetOnlyFileName(pszName) );
		}
		
		return 0;
	}
};

class CDirPostOp : public IFileDirUserOperation
{
	virtual INT Operate(LPCTSTR pszName)
	{
		--tabCount;
		return 0;
	}
};


VOID EnumFileTest(LPCTSTR pszRoot, LPCTSTR pszFileSpec, BOOL bFullPath)
{
	CFileOp fileOp(bFullPath);
	CDirOp dirOp(bFullPath);
	CDirPostOp DirPostOp;
	_ftprintf_s(stdout, TEXT("[ROOT]") TEXT("%s") TEXT("\r\n"), pszRoot);
	CEnumFilesRecursive::Enum(pszRoot, pszFileSpec, &fileOp, &dirOp, &DirPostOp);

	return;
}