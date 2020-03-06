// TestCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <conio.h>

#include "log.h"
#include "TestSharedPtr.h"
#include "TestNullObject.h"
#include "TestInterlocked.h"
#include "TestScope.h"
#include "HeaderOnly.h"
#include "TestRvalueRef.h"
#include "ConnectShare.h"
#include "ListNetSession.h"
#include "MtQueueCsCv.h"
#include "Barrier.h"
#include "CountDownEvent.h"
#include "TestCV.h"
#include "TestClassScopeExit.h"
#include "TestAlg.h"
#include "TestAlg2.h"
#include "TestPerm.h"
#include "testLocale.h"

#include "CircurlarBufferCV.h"
#include "TestCircularRef.h"
#include "TestConst.h"
#include "TestTSX.h"
#include "TestSpinLock.h"
#include "TestMultiMap.h"

int _tmain(int argc, TCHAR ** argv)
{

	int testCase = 23;
	switch (testCase)
	{
	case 24:
	{
		TestTestMultiMap::Case0();
	}
	break;
	case 23:
	{
		TestCpp::SimpleSpinLockTest::Case0();
	}
	break;
	case 22:
	{
		TestCpp::TestTsx::Case0();
	}
	break;
	case 21:
	{
		TestCpp::TestConstCases::Case1();
	}
	break;
	case 20:
	{
		TestCpp::OuterObjTest::Case0();
	}
	break;
	case 19:
	{
		SmartLib::CircurlarBufferCVTest::TestCase();
	}
	break;
	case 18:
	{
		TestCpp::TestLocale::Loc();
	}
	break;
	case 17:
	{
		int total = 5;

		for (int ii = 1; ii <= total; ++ii)
		{
			LOG_LINE(TEXT("Combine %d, %d"), total, ii);
			TestAlg2::TestPerm::CombineApi(total, ii);
			LOG_LINE(TEXT("Press any key to continue..."));
			//_getch();
		}


		for (int ii = 1; ii <= total; ++ii)
		{
			LOG_LINE(TEXT("Perm %d, %d"), total, ii);
			TestAlg2::TestPerm::PermApi(total, ii);
			LOG_LINE(TEXT("Press any key to continue..."));
			//_getch();
		}

	}
	break;
	case 16:
	{
		TestAlg2::LinkedListTest::Case0();
	}
	break;
	case 15:
	{
		TestAlg::LinkedListTest::Case0();
	}
	break;
	case 14:
	{
		TestCpp::CsomeTest::Case0();
	}
	break;
	case 13:
	{
		TestCpp::CTestCV::Case0();
	}
	break;
	case 12:
	{
		TestCpp::CTestCV::Case0();
	}
	break;
	case 11:
	{
		SmartLib::CCountDownEventTest::Case0();
	}
	break;
	case 10:
	{
		SmartLib::CBarrierTest::Case0();
	}
	break;
	case 9:
	{
		SmartLib::CMTqueueTest::Run();
	}
	break;
	case 8:
	{
		TestCpp::CShareManager::Case0(argv[1], argv[2], argv[3],
			argv[4], argv[5], argv[6], argv[7]
		);
	}
	break;
	case 7:
	{
		CListNetSession::List();
	}
	break;
	case 6:
	{
		TestCpp::CShareManager::Connect(argv[1], argv[2], argv[3], argv[4]);
		LOG_LINE(TEXT("press any key to continue..."));
		_getch();
		TestCpp::CShareManager::Disconnect(argv[2]);
	}
	break;
	case 5:
	{
		TestCpp::TestRVRef::Case1();
	}
	break;
	case 4:
	{
		TestCpp::ClassSync classSync;
		classSync.Method1();
		classSync.Method2();
	}
	break;
	case 3:
	{
		TestCpp::TestNoCpp::Case1();
		TestCpp::TestScope::Case0();
	}
	break;
	case 2:
	{
		TestCpp::TestInterlocked::Case0();
	}
	break;
	case  1:
	{
		TestCpp::TestNullObjectCases::Case0();
	}
	break;
	case  0:
	{
		SML::TestSharedPtr::Case0();
	}
	break;
	default:
		break;
	}
	return 0;
}

