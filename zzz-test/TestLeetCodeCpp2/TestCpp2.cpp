// TestCpp2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ReverseNodesK.h"
#include "ReverseNodesK2.h"
#include "TestObjLocalScope.h"
#include "ReverseSList.h"
#include "ShortestPalindrome.h"
#include "CalcSum.h"
#include "CreateFullPathFile.h"
#include "StdForwarMmove.h"
#include "test_rvaluee_ref.h"

int _tmain(int argc, TCHAR ** argv)
{
	int testCase = 9;
	switch (testCase)
	{
	case 9:
	{
		SmartLib::TestRVRefCase::Case3();
		SmartLib::TestRVRefCase::Case2();
		SmartLib::TestRVRefCase::Case1();
		SmartLib::TestRVRefCase::Case0();
	}
	break;
	case 8:
	{
		SmartLib::StdForwarMmove::Case5();
		SmartLib::StdForwarMmove::Case4();
		SmartLib::StdForwarMmove::Case3();
		SmartLib::StdForwarMmove::Case2();
	}
	break;
	case 7:
	{
		SmartLib::CreateFullPathFileTest::Case1(argc, argv);
	}
	break;
	case 6:
	{
		SmartLib::CreateFullPathFileTest::Case0();
	}
	break;
	case 5:
	{
		SmartLib::CalcSumTest::Case0();
	}
	break;
	case 4:
	{
		SmartLib::ShortestPalindrome2Test::Case0();
	}
	break;
	case 3:
	{
		SmartLib::ReverseSListTest::Case1();
		SmartLib::ReverseSListTest::Case0();
	}
	break;
	case 2:
	{
		SmartLib::TempObjTest::Case2();
		SmartLib::TempObjTest::Case1();
		SmartLib::TempObjTest::Case0();
	}
	break;
	case 1:
	{
		SmartLib::ReverseNodesK2Test::Case0();
	}
	break;
	case 0:
	{

		SmartLib::ReverseNodesKTest::Case0();
	}
	break;
	}
    return 0;
}

