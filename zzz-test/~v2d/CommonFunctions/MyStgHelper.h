#pragma once

#include "stdafx.h"

struct CMyStgHelper
{
	static HRESULT CreateMemoryStorage(ILockBytes ** ppILockBytes, IStorage ** ppIStorage,  DWORD grfMode);
	static HRESULT CreateFileStorage(LPCTSTR szFileName, IStorage ** ppIStorage,  DWORD grfMode);
	static HRESULT OpenFileStorage(LPCTSTR szFileName, IStorage ** ppIStorage,  DWORD grfMode);
};