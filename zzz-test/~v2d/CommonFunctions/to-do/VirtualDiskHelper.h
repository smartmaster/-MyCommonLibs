#pragma once 

#include "stdafx.h"
#include <Virtdisk.h>

struct CVirtualDiskHelper
{
	static VOID InitializeCreateVirtualDiskParameters(
		PCREATE_VIRTUAL_DISK_PARAMETERS Parameter, 
		ULONGLONG MaximumSize,
		LPCTSTR ParentPath,
		LPCTSTR SourcePath
		);

	static INT Create(
		LPCTSTR szPath,  
		VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask,
		PCREATE_VIRTUAL_DISK_PARAMETERS Parameters,
		OUT PHANDLE pHandle);

	static INT Attach(
		HANDLE hVhd,
		ATTACH_VIRTUAL_DISK_FLAG AttachFlags
		);

	static INT Dettach(HANDLE hVhd);

	static BOOL IsUninitalizedVHD(DWORD dwDiskNumber);
	static DWORD FindUninitalizedVHD();
};

INT VirtualDiskMain(LPCTSTR szVHDPath, ULONG nSizeGB);
INT AppVirtualDiskMain(INT argc, _TCHAR* argv[]);

struct CCmdLineTestVirtualDiskHelper
{
	CString m_strVHDPath;
	ULONG m_SizeGB;

	//********************************************************************
	CCmdLineTestVirtualDiskHelper()
	{
	}
	
	BOOL IsParamIndicator( TCHAR ch )
	{
		return (TEXT('-') == ch || TEXT('/') == ch);
	}

	//this.exe -VhdPath <path> -Size <size in GB>
	VOID Parse(INT argc, _TCHAR* argv[])
	{
		static LPCTSTR szCmdline[] = 
		{
			TEXT("VHDPath"), //0
			TEXT("Size") //1 //GB
		};

		for (INT i=0; i<argc; ++i)
		{
			if (IsParamIndicator(argv[i][0])) 
			{
				if (0 == _tcsicmp(szCmdline[0], &argv[i][1]) ) //TEXT("VhdPath"), //0
				{
					++i;
					m_strVHDPath = argv[i];
				}
				else if (0 == _tcsicmp(szCmdline[1], &argv[i][1]) ) //TEXT("Size"), //1
				{
					++i;
					m_SizeGB = _tcstoul(argv[i], NULL, 10);
				}
			}
		}
	}
};

