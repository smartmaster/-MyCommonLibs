#pragma  once

#include <setjmp.h>
#include <assert.h>

#ifndef _6F7CCBF4_E94C_46F4_BA53_75CF804058F6_
#define  _6F7CCBF4_E94C_46F4_BA53_75CF804058F6_


//#define  sml_setjmp my_setjmp
//#define  sml_longjmp my_longjmp

#define  sml_setjmp setjmp
#define  sml_longjmp longjmp

//////////////////////////////////////////////////////////////////////////
#define res_scope_start(suffix, max_res_count) \
do { \
	jmp_buf res_buff_ ## suffix[max_res_count]; \
	jmp_buf end_buff_ ## suffix = {0}; \
	volatile long res_count_ ## suffix = 0;


//////////////////////////////////////////////////////////////////////////
#define  res_scope_end(suffix) \
	switch (sml_setjmp(end_buff_ ## suffix)) \
	{ \
	case 0: \
	{ \
		if (res_count_ ## suffix > 0) \
		{ \
			--res_count_ ## suffix; \
			sml_longjmp(res_buff_ ## suffix[res_count_ ## suffix], 1); \
		} \
	} \
	break; \
	default: \
	{ \
	} \
	break; \
	} \
} while(0)



//////////////////////////////////////////////////////////////////////////
#define res_start(suffix) \
switch (sml_setjmp(res_buff_ ## suffix[res_count_ ## suffix])) \
{ \
case 0: \
{ \
	++res_count_ ## suffix; \
	if (res_count_ ## suffix > sizeof(res_buff_ ## suffix)/sizeof(res_buff_ ## suffix[0])) \
	{ \
		assert(0); \
	} \
} \
break; \
default: \
{


//////////////////////////////////////////////////////////////////////////
#define res_end(suffix) \
		if (res_count_ ## suffix > 0) \
		{ \
			--res_count_ ## suffix; \
			sml_longjmp(res_buff_ ## suffix[res_count_ ## suffix], 1); \
		} \
		else \
		{ \
			sml_longjmp(end_buff_ ## suffix, 1); \
		} \
	} \
	break; \
}


//////////////////////////////////////////////////////////////////////////
#define res_cleanup(suffix, cleanup) \
res_start(suffix) \
cleanup \
res_end(suffix)


//////////////////////////////////////////////////////////////////////////
#define rs_start(max_res_count)		res_scope_start(_X_Y_Z_ , max_res_count)
#define rs_end()					res_scope_end(_X_Y_Z_)

#define r_start()				res_start(_X_Y_Z_ )
#define r_end()					res_end(_X_Y_Z_ )
#define r_cleanup(cleanup)		res_cleanup(_X_Y_Z_, cleanup)


//for usage, see CLangRaii6() and CLangRaii5()
//for implementation, see CLangRaii4()


#endif //_6F7CCBF4_E94C_46F4_BA53_75CF804058F6_