#pragma once

#include "raii_func.h"

#ifndef SML_RAII_BLOCK_START



//////////////////////////////////////////////////////////////////////////
#define  SML_RAII_BLOCK_START(name) \
static SmlCVector  _ ## name ## _sml_raii_vec_blocks;  \
static SmlCVector  _ ## name ## _sml_raii_vec_jmp;  \
static volatile long  _ ## name ## _sml_raii_initing = 0;  \
static volatile long  _ ## name ## _sml_raii_call_getaddr_size = 0;  \
static volatile long  _ ## name ## _sml_raii_call_jmptag_size = 0;  \
static volatile long  _ ## name ## _sml_raii_const_false = 0;  \
static volatile long  _ ## name ## _sml_raii_reserved1 = 0xbbbb'bbbb;  \
static volatile long  _ ## name ## _sml_raii_reserved2 = 0xcccc'cccc;  \
static volatile long  _ ## name ## _sml_raii_prepared = 0;  \
volatile long  _ ## name ## _sml_raii_initing_local = (InterlockedCompareExchange(& _ ## name ## _sml_raii_initing, 1, 0) == 0);  \
_ ## name ## _sml_raii_block_start_label: \
if ( _ ## name ## _sml_raii_initing_local)  \
{ \
	SmlCVector_Ctor(& _ ## name ## _sml_raii_vec_blocks);  \
	SmlCVector_Ctor(& _ ## name ## _sml_raii_vec_jmp);  \
}



//////////////////////////////////////////////////////////////////////////
#define SML_RAII_START(name, label) \
Sml_AsmGetRetAddr(& _ ## name ## _sml_raii_vec_blocks,  _ ## name ## _sml_raii_initing_local,  _ ## name ## _sml_raii_reserved1,  _ ## name ## _sml_raii_reserved2);  \
_ ## name ## _sml_raii_lable_ ## label: \
if ( _ ## name ## _sml_raii_const_false)  \
{ \
Sml_JmpTag(SML_JMPTAGPATERN_BEGIN);





//////////////////////////////////////////////////////////////////////////
#define SML_RAII_LABEL(name, label) \
if (_ ## name ## _sml_raii_const_false)  \
{ \
	goto _ ## name ## _sml_raii_lable_ ## label;  \
}




//////////////////////////////////////////////////////////////////////////
#define SML_RAII_END(name, label) \
Sml_JmpTag(SML_JMPTAGPATERN_END);  \
}  \
Sml_AsmGetRetAddr(& _ ## name ## _sml_raii_vec_blocks,  _ ## name ## _sml_raii_initing_local,  _ ## name ## _sml_raii_reserved1,  _ ## name ## _sml_raii_reserved2);  \
SML_RAII_LABEL(name, label)




//////////////////////////////////////////////////////////////////////////
#define  SML_RAII_BLOCK_END(name) \
if ( _ ## name ## _sml_raii_initing_local)  \
{  \
	Sml_PrepareCleanup(& _ ## name ## _sml_raii_vec_blocks, & _ ## name ## _sml_raii_vec_jmp);  \
}  \
if (0 == _ ## name ## _sml_raii_initing_local)  \
{  \
	while (0 == _ ## name ## _sml_raii_prepared )  \
	{  \
		Sleep(0);  \
	}  \
}  \
{  \
	int  _ ## name ## _sml_raii_vec_jmp_size = SmlCVector_Size(& _ ## name ## _sml_raii_vec_jmp);  \
	BYTE*  _ ## name ## _sml_raii_first =  _ ## name ## _sml_raii_vec_jmp_size == 0 ? NULL : (BYTE*)SmlCVector_Get(& _ ## name ## _sml_raii_vec_jmp, 0);  \
	if ( _ ## name ## _sml_raii_first)  \
	{  \
		 _ ## name ## _sml_raii_first += sizeof(Sml_JmpTagPatern);  \
	}  \
	BYTE*  _ ## name ## _sml_raii_last =  _ ## name ## _sml_raii_vec_jmp_size == 0 ? NULL : (BYTE*)SmlCVector_Get(& _ ## name ## _sml_raii_vec_jmp,  _ ## name ## _sml_raii_vec_jmp_size - 1);  \
	Sml_AsmRunCleanup( _ ## name ## _sml_raii_first,  _ ## name ## _sml_raii_last, & _ ## name ## _sml_raii_prepared);  \
}  \
if ( _ ## name ## _sml_raii_const_false)  \
{  \
	goto  _ ## name ## _sml_raii_block_start_label;  \
}

/***********************************************
in release build, SML_RAII_BLOCK_END should be followed by SML_RAII_LABEL(s), for example
	SML_RAII_BLOCK_END(block);
	SML_RAII_LABEL(block, lable_aaa);
	SML_RAII_LABEL(block, lable_bbb);
	SML_RAII_LABEL(block, lable_ccc);
	SML_RAII_LABEL(block, lable_ddd);
	SML_RAII_LABEL(block, lable_eee);
***********************************************/

//////////////////////////////////////////////////////////////////////////
#define SML_RAII_USING(func)  \
SML_RAII_START;  \
func;  \
SML_RAII_END



//////////////////////////////////////////////////////////////////////////
#define  raii_block_start	SML_RAII_BLOCK_START
#define  raii_start			SML_RAII_START
#define  raii_end			SML_RAII_END
#define	 raii_label			SML_RAII_LABEL
#define  raii_block_end		SML_RAII_BLOCK_END
#define  raii_using			SML_RAII_USING


#endif // !SML_RAII_BLOCK_START