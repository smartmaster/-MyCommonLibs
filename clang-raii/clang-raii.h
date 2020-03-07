#pragma  once

#include <setjmp.h>

#ifndef _6F7CCBF4_E94C_46F4_BA53_75CF804058F6_
#define  _6F7CCBF4_E94C_46F4_BA53_75CF804058F6_



#define RES_SCOPE_START(suffix, max_res_count) \
jmp_buf res_buff_ ## suffix[max_res_count]; \
jmp_buf end_buff_ ## suffix = {0}; \
volatile long res_count_ ## suffix = 0;


#define  RES_SCOPE_END(suffix) \
switch (setjmp(end_buff_ ## suffix)) \
{ \
case 0: \
{ \
	if (res_count_ ## suffix > 0) \
	{ \
		--res_count_ ## suffix; \
		longjmp(res_buff_ ## suffix[res_count_ ## suffix], 1); \
	} \
} \
break; \
default: \
{ \
} \
break; \
}

#define RES_START(suffix) \
switch (setjmp(res_buff_ ## suffix[res_count_ ## suffix])) \
{ \
case 0: \
{ \
	++res_count_ ## suffix; \
} \
break; \
default: \
{


#define RES_END(suffix) \
		if (res_count_ ## suffix > 0) \
		{ \
			--res_count_ ## suffix; \
			longjmp(res_buff_ ## suffix[res_count_ ## suffix], 1); \
		} \
		else \
		{ \
			longjmp(end_buff_ ## suffix, 1); \
		} \
	} \
	break; \
}


#endif //_6F7CCBF4_E94C_46F4_BA53_75CF804058F6_