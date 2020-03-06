// TestAsm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

EXTERN_C_START

__declspec(noinline)
void SetFunc(INT_PTR func);

__declspec(noinline)
INT_PTR RipRelJump(...);

__declspec(noinline)
INT_PTR CallProc(INT_PTR func, ...);

__declspec(noinline)
void InspectBytecode();

__declspec(noinline)
void __stdcall StdProc(DWORD arg0, DWORD arg1, DWORD arg2);

EXTERN_C_END


int _tmain(int argc, TCHAR** argv)
{
	int testCase = 0;
	if (argc > 1)
	{
		testCase = _tcstol(argv[1], nullptr, 10);
	}
	switch (testCase)
	{
	case 1:
	{
		StdProc(0xaaaa, 0xbbbb, 0xdddd);
		InspectBytecode();
	}
	break;
	case 0:
	{
		SetFunc((INT_PTR)(printf));
		INT_PTR result = RipRelJump("hello %d %s" "\r\n", 123, "text");
		result = CallProc((INT_PTR)(printf), "hello %d %s" "\r\n", 123, "text");
	}
	break;
	default:
	{
		
	}
	break;
	}
}

