// NativeDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include <cassert>
//#include <vsserror.h>
#include <vss.h>
#include <vswriter.h>
#include <VsBackup.h>

#include "NativeDll.h"

void DllFunc(int iCommand, const void * iBuffer, int iSize, int * oCommand, void * oBuffer, int oSize)
{
	switch (iCommand)
	{
	case 1:
	{

	}
	break;
	case 0:
	{
		IVssBackupComponents * pBackup = nullptr;
		HRESULT hr = ::CreateVssBackupComponents(
			&pBackup//_Out_  IVssBackupComponents **ppBackup
		);
		if (SUCCEEDED(hr))
		{
			assert(oSize >= sizeof(pBackup));
			CopyMemory(oBuffer, &pBackup, sizeof(pBackup));
		}

	}
	default:
		break;
	}
}