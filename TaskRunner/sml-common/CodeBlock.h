#pragma once

//#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
#ifndef SML_BEGIN_BLOCK
//#define SML_BEGIN_BLOCK


#define SML_BEGIN_BLOCK(level_number)			do{	int	sml_1qaz_2wsx_3edc_4rfv_ ## level_number = (level_number) + (__LINE__) - (level_number);
#define SML_LEAVE_BLOCK(level_number)			{(sml_1qaz_2wsx_3edc_4rfv_ ## level_number);}	break;
#define SML_END_BLOCK(level_number)				{(sml_1qaz_2wsx_3edc_4rfv_ ## level_number);}	}while(0);

#define BEGIN_BLOCK SML_BEGIN_BLOCK
#define LEAVE_BLOCK SML_LEAVE_BLOCK
#define END_BLOCK SML_END_BLOCK

#endif // !SML_BEGIN_BLOCK

