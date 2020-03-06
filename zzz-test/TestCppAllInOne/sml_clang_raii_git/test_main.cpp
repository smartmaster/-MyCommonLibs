
#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include "./test/sml_raii_clang.test.h"

int _tmain(int argc, TCHAR** argv)
{
	int testCase = -1;
	if (argc > 1)
	{
		testCase = wcstol(argv[1], NULL, 10);
	}
	
	switch (testCase)
	{
	break;
	case 2:
	{
		sml_raii_clang_test();
	}
	break;
	case 1:
	{
		int loopCount = 2;
		for (int ii = 0; ii < loopCount; ++ii)
		{
			sml_raii_clang_test_nomacro();
		}
		
	}
	break;
	}
	return 0;
}