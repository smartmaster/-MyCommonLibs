#pragma once

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
#ifndef BEGIN_BLOCK
#define BEGIN_BLOCK(LevelNumber)		do{ int __abc_xyz_##LevelNumber(LevelNumber); 
#define LEAVE_BLOCK(LevelNumber)		{(__abc_xyz_##LevelNumber);} break;
#define END_BLOCK(LevelNumber)			{(__abc_xyz_##LevelNumber);} }while(0);
#endif
//////////////////////////////////////////////////////////////////////////