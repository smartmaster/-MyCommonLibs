
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clang-raii.h"


extern int my_setjmp(jmp_buf env);
extern void my_longjmp(jmp_buf env, int value);


void CLangRaii6()
{

	rs_start(16);

	//////////////////////////////////////////////////////////////////////////
	char* str0 = strdup("this is the 1st resource allocated");
	printf("Allocated resource: %s" "\r\n", str0);


	r_cleanup({
		printf("To free resource: %s" "\r\n", str0);
		free(str0);
		});

	//////////////////////////////////////////////////////////////////////////
	char* str1 = strdup("this is the 2nd resource allocated");
	printf("Allocated resource: %s" "\r\n", str1);

	r_cleanup({
		printf("To free resource: %s" "\r\n", str1);
		free(str1);
		});

	//////////////////////////////////////////////////////////////////////////
	char* str2 = strdup("this is the 3rd resource allocated");
	printf("Allocated resource: %s" "\r\n", str2);


	r_cleanup({
		printf("To free resource: %s" "\r\n", str2);
		free(str2);
		});


	//////////////////////////////////////////////////////////////////////////
	rs_end();

	return;
}


void CLangRaii5()
{

	res_scope_start(CLangRaii5, 16);

	//////////////////////////////////////////////////////////////////////////
	char* str0 = strdup("this is the 1st resource allocated");
	printf("Allocated resource: %s" "\r\n", str0);


	res_cleanup(CLangRaii5, {
		printf("To free resource: %s" "\r\n", str0);
		free(str0);
		});

	//////////////////////////////////////////////////////////////////////////
	char* str1 = strdup("this is the 2nd resource allocated");
	printf("Allocated resource: %s" "\r\n", str1);

	res_cleanup(CLangRaii5, {
		printf("To free resource: %s" "\r\n", str1);
		free(str1);
		});

	//////////////////////////////////////////////////////////////////////////
	char* str2 = strdup("this is the 3rd resource allocated");
	printf("Allocated resource: %s" "\r\n", str2);


	res_cleanup(CLangRaii5, {
		printf("To free resource: %s" "\r\n", str2);
		free(str2);
		});


	//////////////////////////////////////////////////////////////////////////
	res_scope_end(CLangRaii5);

	return;
}




void CLangRaii4()
{

	//////////////////////////////////////////////////////////////////////////
	jmp_buf res_buff_suffix[4];
	jmp_buf end_buff_suffix = { 0 };
	volatile long res_count_suffix = 0;

	//////////////////////////////////////////////////////////////////////////
	char* str0 = strdup("this is the 1st resource allocated");
	printf("Allocated resource: %s" "\r\n", str0);

	switch (sml_setjmp(res_buff_suffix[res_count_suffix]))
	{
	case 0:
	{
		++res_count_suffix;
		if (res_count_suffix > sizeof(res_buff_suffix)/sizeof(res_buff_suffix[0]))
		{
			assert(0);
		}
	}
	break;
	default:
	{
		printf("To free resource: %s" "\r\n", str0);
		free(str0);

		if (res_count_suffix > 0)
		{
			--res_count_suffix;
			sml_longjmp(res_buff_suffix[res_count_suffix], 1);
		}
		else
		{
			sml_longjmp(end_buff_suffix, 1);
		}
	}
	break;
	}

	//////////////////////////////////////////////////////////////////////////
	char* str1 = strdup("this is the 2nd resource allocated");
	printf("Allocated resource: %s" "\r\n", str1);

	switch (sml_setjmp(res_buff_suffix[res_count_suffix]))
	{
	case 0:
	{
		++res_count_suffix;
		if (res_count_suffix > sizeof(res_buff_suffix) / sizeof(res_buff_suffix[0]))
		{
			assert(0);
		}
	}
	break;
	default:
	{
		printf("To free resource: %s" "\r\n", str1);
		free(str1);

		if (res_count_suffix > 0)
		{
			--res_count_suffix;
			sml_longjmp(res_buff_suffix[res_count_suffix], 1);
		}
		else
		{
			sml_longjmp(end_buff_suffix, 1);
		}
	}
	break;
	}

	//////////////////////////////////////////////////////////////////////////
	char* str2 = strdup("this is the 3rd resource allocated");
	printf("Allocated resource: %s" "\r\n", str2);

	switch (sml_setjmp(res_buff_suffix[res_count_suffix]))
	{
	case 0:
	{
		++res_count_suffix;
		if (res_count_suffix > sizeof(res_buff_suffix) / sizeof(res_buff_suffix[0]))
		{
			assert(0);
		}
	}
	break;
	default:
	{
		printf("To free resource: %s" "\r\n", str2);
		free(str2);

		if (res_count_suffix > 0)
		{
			--res_count_suffix;
			sml_longjmp(res_buff_suffix[res_count_suffix], 1);
		}
		else
		{
			sml_longjmp(end_buff_suffix, 1);
		}

	}
	break;
	}

	//////////////////////////////////////////////////////////////////////////
	printf("To free all resource" "\r\n");

	//////////////////////////////////////////////////////////////////////////
	switch (sml_setjmp(end_buff_suffix))
	{
	case 0:
	{
		if (res_count_suffix > 0)
		{
			--res_count_suffix;
			sml_longjmp(res_buff_suffix[res_count_suffix], 1);
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

	res_scope_start(CLangRaii3, 16);
	
	//////////////////////////////////////////////////////////////////////////
	char* str0 = strdup("this is the 1st resource allocated");
	printf("Allocated resource: %s" "\r\n", str0);

	res_start(CLangRaii3);
	printf("To free resource: %s" "\r\n", str0);
	free(str0);
	res_end(CLangRaii3);


	//////////////////////////////////////////////////////////////////////////
	char* str1 = strdup("this is the 2nd resource allocated");
	printf("Allocated resource: %s" "\r\n", str1);

	res_start(CLangRaii3);
	printf("To free resource: %s" "\r\n", str1);
	free(str1);
	res_end(CLangRaii3);

	//////////////////////////////////////////////////////////////////////////
	char* str2 = strdup("this is the 3rd resource allocated");
	printf("Allocated resource: %s" "\r\n", str2);

	res_start(CLangRaii3);
	printf("To free resource: %s" "\r\n", str2);
	free(str2);
	res_end(CLangRaii3);


	//////////////////////////////////////////////////////////////////////////
	res_scope_end(CLangRaii3);

	return;
}


#if 1
void CLangRaii2()
{

	//////////////////////////////////////////////////////////////////////////
	jmp_buf res_buff_suffix[4];
	jmp_buf end_buff_suffix = { 0 };
	volatile long res_count_suffix = 0;

	//////////////////////////////////////////////////////////////////////////
	char* str0 = strdup("this is the 1st resource allocated");
	printf("Allocated resource: %s" "\r\n", str0);

	switch (sml_setjmp(res_buff_suffix[res_count_suffix]))
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
			sml_longjmp(res_buff_suffix[res_count_suffix], 1);
		}
		else
		{
			sml_longjmp(end_buff_suffix, 1);
		}
	}
	break;
	}

	//////////////////////////////////////////////////////////////////////////
	char* str1 = strdup("this is the 2nd resource allocated");
	printf("Allocated resource: %s" "\r\n", str1);

	switch (sml_setjmp(res_buff_suffix[res_count_suffix]))
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
			sml_longjmp(res_buff_suffix[res_count_suffix], 1);
		}
		else
		{
			sml_longjmp(end_buff_suffix, 1);
		}
	}
	break;
	}

	//////////////////////////////////////////////////////////////////////////
	char* str2 = strdup("this is the 3rd resource allocated");
	printf("Allocated resource: %s" "\r\n", str2);

	switch (sml_setjmp(res_buff_suffix[res_count_suffix]))
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
			sml_longjmp(res_buff_suffix[res_count_suffix], 1);
		}
		else
		{
			sml_longjmp(end_buff_suffix, 1);
		}
		
	}
	break;
	}

	//////////////////////////////////////////////////////////////////////////
	printf("To free all resource" "\r\n");

	//////////////////////////////////////////////////////////////////////////
	switch (sml_setjmp(end_buff_suffix))
	{
	case 0:
	{
		if (res_count_suffix > 0)
		{
			--res_count_suffix;
			sml_longjmp(res_buff_suffix[res_count_suffix], 1);
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
#endif


void CLangRaii()
{
#define STATIC (0);

	STATIC jmp_buf env_end = { 0 };

	char* str1 = strdup("this is the 1st resource allocated");
	STATIC jmp_buf env_str1;
	switch (sml_setjmp(env_str1))
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
		sml_longjmp(env_end, 1);
	}
	break;
	}

	char* str2 = strdup("this is the 2nd resource allocated");
	STATIC jmp_buf env_str2;
	switch (sml_setjmp(env_str2))
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
		sml_longjmp(env_str1, 1);
	}
	break;
	}


	char* str3 = strdup("this is the 3rd resource allocated");
	STATIC jmp_buf env_str3;
	switch (sml_setjmp(env_str3))
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
		sml_longjmp(env_str2, 1);
	}
	break;
	}

	   	 	
	switch (sml_setjmp(env_end))
	{
	case 0:
	{
		printf("To free all resource" "\r\n");
		sml_longjmp(env_str3, 1);
	}
	break;
	default:
	{
		printf("all resource are freed" "\r\n");
	}
	break;
	}
}