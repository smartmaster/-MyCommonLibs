// TestBoost.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>



#include "test_translate.h"
#include "test_rest.h"
#include "test_boost_option.h"
#include "Eg0101ScopedPtrArray.test.h"
#include "Eg0103SharedPtrArray.test.h"
#include "Eg05xxStringAlgotithm.test.h"
#include "Eg08xx_regex.test.h"
#include "Eg12BoostContainer.h"


int _tmain(int argc, TCHAR** argv)
{
	int testCase = -1;
	if (argc > 1)
	{
		testCase = _tcstol(argv[1], nullptr, 10);
	}
	switch (testCase)
	{
	case  6:
	{
		SmartLib::Eg12BoostContainer::Case0();
	}
	break;
	case 5:
	{
		SmartLib::Eg08xx_regex::Case0();
	}
	break;
	case 4:
	{
		SmartLib::Eg05xxStringAlgotithm::Case0();
	}
	break;
	case 3:
	{
		SmartLib::Eg0103SharedPtrArray::Case5();
		SmartLib::Eg0103SharedPtrArray::Case4();
		SmartLib::Eg0103SharedPtrArray::Case3();
		SmartLib::Eg0103SharedPtrArray::Case2();
		SmartLib::Eg0103SharedPtrArray::Case1();
		SmartLib::Eg0103SharedPtrArray::Case0();
		SmartLib::Eg0101ScopedPtr::Case1();
		SmartLib::Eg0101ScopedPtr::Case0();
	}
	break;
	case 2:
	{
		SmartLib::TestBoostOption::Case0(argc, argv);
	}
	break;
	case 1:
	{
		 SmartLib::TestRest1::Case0(argc, argv);
	}
	break;
	case 0:
	{
		SmartLib::TestBoostTranslate::Case0();
	}
	break;
	}

	return testCase;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
