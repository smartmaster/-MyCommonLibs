// DelLongFile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "EnumFiles.h"

int _tmain(int argc, TCHAR ** argv)
{
	SmartLib::StaticAPI::RemoveFile(argv[1], argv[2]);
    return 0;
}

