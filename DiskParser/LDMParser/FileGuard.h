#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "..\..\PublicHeader\PublicFileDeviceInterface.h"

class FileGuard
{
public:
    //FileGuard(FILE* fd);
	FileGuard(IFileDevice* fd);
    ~FileGuard();

    //operator FILE*();
	operator IFileDevice*();

private:
    FileGuard(const FileGuard&);
    FileGuard& operator=(const FileGuard&);

private:
    //FILE* m_file;
	CComPtr<IFileDevice> m_file;
};
