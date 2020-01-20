

#include <tchar.h>
#include <windows.h>
#include <stdio.h>

#include "StringOf.h"

int FuncInc(int aa, int bb, int cc)
{
	int rr = 0;
	rr += aa;
	rr += bb;
	rr += cc;
	return rr;
}

int func_counter()
{
//#define  __COUNTER__ s
//#define  __COUNTER__ __COUNTER__ ## s

#define  LABEL __lable ## __COUNTER__ ## :
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(__lable@__COUNTER__@));

#define x 100 ## __COUNTER__ ## 100
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(__lable@__COUNTER__@));
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(__lable@__COUNTER__@));
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(x));
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(x));
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(x));
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(x));
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(x));

#define  VAR_COUNTER __COUNTER__
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(VAR_COUNTER));
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(VAR_COUNTER));
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(__COUNTER__));
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(__COUNTER__));


	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(__COUNTER__ + 100));
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(__COUNTER__ + 200));
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(__COUNTER__ + 300));
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(__COUNTER__ + 400));
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(__COUNTER__));
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(__COUNTER__));
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(__COUNTER__));

	printf("%d" "\r\n", __COUNTER__ + 100);
	printf("%d" "\r\n", __COUNTER__ + 200);
	printf("%d" "\r\n", __COUNTER__ + 300);
	printf("%d" "\r\n", __COUNTER__);
	printf("%d" "\r\n", __COUNTER__);
	printf("%d" "\r\n", __COUNTER__);
	printf("%d" "\r\n", __COUNTER__);
	printf("%d" "\r\n", __COUNTER__);


#define  MY_COUNTER __S
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(MY_COUNTER));

#define  MY_COUNTER MY_COUNTER ## S
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(MY_COUNTER));

#define  MY_COUNTER MY_COUNTER ## S
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(MY_COUNTER));

#define  MY_COUNTER MY_COUNTER ## S
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(MY_COUNTER));

#define  MY_COUNTER MY_COUNTER ## S
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(MY_COUNTER));

#define  MY_COUNTER MY_COUNTER ## S
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(MY_COUNTER));

#define  MY_COUNTER MY_COUNTER ## S
	printf("%s" "\r\n", SML_EXPANDED_MACRO_STRING(MY_COUNTER));



	//printf("%d" "\r\n", MY_COUNTER);
	//printf("%d" "\r\n", MY_COUNTER);
	//printf("%d" "\r\n", MY_COUNTER);
	//printf("%d" "\r\n", MY_COUNTER);
	//printf("%d" "\r\n", MY_COUNTER);
	//printf("%d" "\r\n", MY_COUNTER);
	//printf("%d" "\r\n", MY_COUNTER);
	//printf("%d" "\r\n", MY_COUNTER);



	return 0;
}

//#define RAII_START(name, number) \
//volatile int sml_ ## name ## _false = 0;  \
//if (sml_raii_false) \
//{ \
//	sml_raii_ ## number: \
//	goto sml_  ## name ## _finsih; \
//}
//
//#define RAII_USING(name, number, number_minus_1, func) \
//if (sml_ ## name ## _false) \
//{ \
//	sml_ ## name ## _1: \
//	func; \
//	goto sml_ ## name ## _0; \
//}

void StartTag(void * pp)
{

}

void EndTag(void * pp)
{

}


int func_cleanup()
{
	volatile int sml_raii_false = 1;
	
	volatile int start = 0;
	volatile int end = 0;

	HANDLE file1 = CreateFile(NULL, 0, 0, NULL, 0, 0, NULL);

	if (sml_raii_false)
	{
		StartTag(&start);
		if (file1 != INVALID_HANDLE_VALUE)
		{
			CloseHandle(file1);
		}
		EndTag(&end);
	}
	



	HANDLE file2 = CreateFile(NULL, 0, 0, NULL, 0, 0, NULL);

	if (sml_raii_false)
	{
		StartTag(&start);
		if (file2 != INVALID_HANDLE_VALUE)
		{
			CloseHandle(file2);
		}
		EndTag(&end);
	}
	


	HANDLE file3 = CreateFile(NULL, 0, 0, NULL, 0, 0, NULL);

	if (sml_raii_false)
	{
		StartTag(&start);
		if (file3 != INVALID_HANDLE_VALUE)
		{
			CloseHandle(file3);
		}
		EndTag(&end);
	}
	
	//run_clean();
	return 100;
}