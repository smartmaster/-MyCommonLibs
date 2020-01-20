#pragma once
#include "HAVhdUtility.h"
#include <WinNT.h>
#include <Winioctl.h.>
using namespace HaVhdUtility;


//
enum OPENMODE 
{
    MODE_NULL = 0,
    MODE_READ = GENERIC_READ, 
    MODE_WRITE = GENERIC_WRITE, 
    MODE_READWRITE = GENERIC_READ | GENERIC_WRITE
};

