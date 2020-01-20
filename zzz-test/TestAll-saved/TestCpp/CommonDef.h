#pragma once


#define SML_BEGIN_BLOCK(level_number)			do{	int	sml_1qaz_2wsx_3edc_4rfv_ ## level_number = (level_number) + (__LINE__) - (level_number);
#define SML_LEAVE_BLOCK(level_number)			{(sml_1qaz_2wsx_3edc_4rfv_ ## level_number);}	break;
#define SML_END_BLOCK(level_number)				{(sml_1qaz_2wsx_3edc_4rfv_ ## level_number);}	}while(0);

#define  BEGIN_BLOCK SML_BEGIN_BLOCK
#define  LEAVE_BLOCK SML_LEAVE_BLOCK
#define  END_BLOCK SML_END_BLOCK


#define SML_MAKE_STRING(sml_any) # sml_any
#define SML_EXPAND_MACRO(sml_macro) SML_MAKE_STRING(sml_macro)


#define SML_MAKE_STRING_W(sml_any) L ## # sml_any
#define SML_EXPAND_MACRO_W(sml_macro) SML_MAKE_STRING_W(sml_macro)