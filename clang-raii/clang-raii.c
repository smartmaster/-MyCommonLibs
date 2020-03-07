
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clang-raii.h"


extern int my_setjmp(jmp_buf env);
extern void my_longjmp(jmp_buf env, int value);


void CLangRaii4()
{

#define  xx_setjmp my_setjmp
#define  xx_longjmp my_longjmp

	//////////////////////////////////////////////////////////////////////////
	jmp_buf res_buff_suffix[4];
	jmp_buf end_buff_suffix = { 0 };
	volatile long res_count_suffix = 0;

	//////////////////////////////////////////////////////////////////////////
	char* str0 = strdup("this is the 1st resource allocated");
	printf("Allocated resource: %s" "\r\n", str0);

	switch (xx_setjmp(res_buff_suffix[res_count_suffix]))
	{
	case 0:
	{
		++res_count_suffix;
	}
	break;
	default:
	{
		printf("To free resource: %s" "\r\n", str0);
		free(str0);

		if (res_count_suffix > 0)
		{
			--res_count_suffix;
			xx_longjmp(res_buff_suffix[res_count_suffix], 1);
		}
		else
		{
			xx_longjmp(end_buff_suffix, 1);
		}
	}
	break;
	}

	//////////////////////////////////////////////////////////////////////////
	char* str1 = strdup("this is the 2nd resource allocated");
	printf("Allocated resource: %s" "\r\n", str1);

	switch (xx_setjmp(res_buff_suffix[res_count_suffix]))
	{
	case 0:
	{
		++res_count_suffix;
	}
	break;
	default:
	{
		printf("To free resource: %s" "\r\n", str1);
		free(str1);

		if (res_count_suffix > 0)
		{
			--res_count_suffix;
			xx_longjmp(res_buff_suffix[res_count_suffix], 1);
		}
		else
		{
			xx_longjmp(end_buff_suffix, 1);
		}
	}
	break;
	}

	//////////////////////////////////////////////////////////////////////////
	char* str2 = strdup("this is the 3rd resource allocated");
	printf("Allocated resource: %s" "\r\n", str2);

	switch (xx_setjmp(res_buff_suffix[res_count_suffix]))
	{
	case 0:
	{
		++res_count_suffix;
	}
	break;
	default:
	{
		printf("To free resource: %s" "\r\n", str2);
		free(str2);

		if (res_count_suffix > 0)
		{
			--res_count_suffix;
			xx_longjmp(res_buff_suffix[res_count_suffix], 1);
		}
		else
		{
			xx_longjmp(end_buff_suffix, 1);
		}

	}
	break;
	}

	//////////////////////////////////////////////////////////////////////////
	printf("To free all resource" "\r\n");

	//////////////////////////////////////////////////////////////////////////
	switch (xx_setjmp(end_buff_suffix))
	{
	case 0:
	{
		if (res_count_suffix > 0)
		{
			--res_count_suffix;
			xx_longjmp(res_buff_suffix[res_count_suffix], 1);
		}
	}
	break;
	default:
	{
	}
	break;
	}


	//////////////////////////////////////////////////////////////////////////
	printf("all resource are freed" "\r\n");
}




void CLangRaii3()
{

	RES_SCOPE_START(__FUNCTION__, 16);
	
	//////////////////////////////////////////////////////////////////////////
	char* str0 = strdup("this is the 1st resource allocated");
	printf("Allocated resource: %s" "\r\n", str0);

	RES_START(__FUNCTION__);
	printf("To free resource: %s" "\r\n", str0);
	free(str0);
	RES_END(__FUNCTION__);


	//////////////////////////////////////////////////////////////////////////
	char* str1 = strdup("this is the 2nd resource allocated");
	printf("Allocated resource: %s" "\r\n", str1);

	RES_START(__FUNCTION__);
	printf("To free resource: %s" "\r\n", str1);
	free(str1);
	RES_END(__FUNCTION__);

	//////////////////////////////////////////////////////////////////////////
	char* str2 = strdup("this is the 3rd resource allocated");
	printf("Allocated resource: %s" "\r\n", str2);

	RES_START(__FUNCTION__);
	printf("To free resource: %s" "\r\n", str2);
	free(str2);
	RES_END(__FUNCTION__);


	//////////////////////////////////////////////////////////////////////////
	RES_SCOPE_END(__FUNCTION__);

	return;
}



void CLangRaii2()
{

	//////////////////////////////////////////////////////////////////////////
	jmp_buf res_buff_suffix[4];
	jmp_buf end_buff_suffix = { 0 };
	volatile long res_count_suffix = 0;

	//////////////////////////////////////////////////////////////////////////
	char* str0 = strdup("this is the 1st resource allocated");
	printf("Allocated resource: %s" "\r\n", str0);

	switch (setjmp(res_buff_suffix[res_count_suffix]))
	{
	case 0:
	{
		++res_count_suffix;
	}
	break;
	default:
	{
		printf("To free resource: %s" "\r\n", str0);
		free(str0);

		if (res_count_suffix > 0)
		{
			--res_count_suffix;
			longjmp(res_buff_suffix[res_count_suffix], 1);
		}
		else
		{
			longjmp(end_buff_suffix, 1);
		}
	}
	break;
	}

	//////////////////////////////////////////////////////////////////////////
	char* str1 = strdup("this is the 2nd resource allocated");
	printf("Allocated resource: %s" "\r\n", str1);

	switch (setjmp(res_buff_suffix[res_count_suffix]))
	{
	case 0:
	{
		++res_count_suffix;
	}
	break;
	default:
	{
		printf("To free resource: %s" "\r\n", str1);
		free(str1);

		if (res_count_suffix > 0)
		{
			--res_count_suffix;
			longjmp(res_buff_suffix[res_count_suffix], 1);
		}
		else
		{
			longjmp(end_buff_suffix, 1);
		}
	}
	break;
	}

	//////////////////////////////////////////////////////////////////////////
	char* str2 = strdup("this is the 3rd resource allocated");
	printf("Allocated resource: %s" "\r\n", str2);

	switch (setjmp(res_buff_suffix[res_count_suffix]))
	{
	case 0:
	{
		++res_count_suffix;
	}
	break;
	default:
	{
		printf("To free resource: %s" "\r\n", str2);
		free(str2);

		if (res_count_suffix > 0)
		{
			--res_count_suffix;
			longjmp(res_buff_suffix[res_count_suffix], 1);
		}
		else
		{
			longjmp(end_buff_suffix, 1);
		}
		
	}
	break;
	}

	//////////////////////////////////////////////////////////////////////////
	printf("To free all resource" "\r\n");

	//////////////////////////////////////////////////////////////////////////
	switch (setjmp(end_buff_suffix))
	{
	case 0:
	{
		if (res_count_suffix > 0)
		{
			--res_count_suffix;
			longjmp(res_buff_suffix[res_count_suffix], 1);
		}
	}
	break;
	default:
	{
	}
	break;
	}


	//////////////////////////////////////////////////////////////////////////
	printf("all resource are freed" "\r\n");
}



void CLangRaii()
{
#define STATIC (0);

	STATIC jmp_buf env_end = { 0 };

	char* str1 = strdup("this is the 1st resource allocated");
	STATIC jmp_buf env_str1;
	switch (setjmp(env_str1))
	{
	case 0:
	{
		printf("Allocated resource: %s" "\r\n", str1);
	}
	break;
	default:
	{
		printf("To free resource: %s" "\r\n", str1);
		free(str1);
		longjmp(env_end, 1);
	}
	break;
	}

	char* str2 = strdup("this is the 2nd resource allocated");
	STATIC jmp_buf env_str2;
	switch (setjmp(env_str2))
	{
	case 0:
	{
		printf("Allocated resource: %s" "\r\n", str2);
	}
	break;
	default:
	{
		printf("To free resource: %s" "\r\n", str2);
		free(str2);
		longjmp(env_str1, 1);
	}
	break;
	}


	char* str3 = strdup("this is the 3rd resource allocated");
	STATIC jmp_buf env_str3;
	switch (setjmp(env_str3))
	{
	case 0:
	{
		printf("Allocated resource: %s" "\r\n", str3);
	}
	break;
	default:
	{
		printf("To free resource: %s" "\r\n", str3);
		free(str3);
		longjmp(env_str2, 1);
	}
	break;
	}

	   	 	
	switch (setjmp(env_end))
	{
	case 0:
	{
		printf("To free all resource" "\r\n");
		longjmp(env_str3, 1);
	}
	break;
	default:
	{
		printf("all resource are freed" "\r\n");
	}
	break;
	}
}