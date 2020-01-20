#pragma once

#ifdef DEBUG
#define SML_PRIVATE public
#else
#define SML_PRIVATE private
#endif // DEBUG


//////////////////////////////////////////////////////////////////////////
#ifndef BEGIN_BLOCK
#define BEGIN_BLOCK(level_number)			do{	int	_abc_xyz##level_number(level_number);
#define LEAVE_BLOCK(level_number)			{(_abc_xyz##level_number);}	break;
#define END_BLOCK(level_number)				{(_abc_xyz##level_number);}	}while(0);
#endif

