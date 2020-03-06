// TestCppAllInOne.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "SocketConnect.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <lm.h>

#include <iostream>
#include <cstdio>

#include "log.h"
#include "StringOf.h"
#include "ScopeExit.h"

#include "TestForward.h"
#include "TestFwdMove.h"
#include "SmlList.h"
#include "MyPrintType.h"
#include "IAllocator.h"
#include "IocpCooyFile.h"
#include "TestCircularRef.h"
#include "testInvalidHandler.h"
#include "TestArgs.h"
#include "test_woss.h"
#include "MyFormatCode.h"
#include "TestConstExpr.h"
#include "BstrConv.h"
#include "FindFileExist.h"
#include "LoadLocalStrings.h"
#include "EnumFileSystem.h"
#include "DebugHelper.h"
//#include "..\..\..\WSASocketEchoClient\SocketAddrNameHelper.h"
#include "testCallAsm.h"
#include "smlhook.h"
#include "SmlBlockAllocator.h"
#include "testWinVer.h"
#include "testSharedPtr.h"
#include "../../../MTReadWrite/counted_ptr.test.h"
#include "finc.h"
#include "SmlCVector.test.h"
#include "clangraii.test.h"
#include "MaskBitCountToString.test.h"
#include "sml_raii_clang.test.h"
#include "test_utf8_cout.h"
#include "test_cpp_type.h"
#include "RefPtr.test.h"
#include "SmlQuickSort.test.h"
#include "ReadConfig.test.h"
#include "HCPPCreate.test.h"
#include "misc.small.test.h"
#include "TestWTS.h"
#include "StartAppOnActiveUserSession.test.h"
#include "GetSpecialFolder.h"
#include "test_apache_load.h"
#include "ComposeMax.test.h"
#include "wordBreak.test.h"
#include "ListTest.test.h"
#include "TestStdCppCV.test.h"
#include "partition-equal-subset-sum.test.h"
#include "interleaving-string.test.h"
#include "partition-list.test.h"
#include "frog-jump.test.h"
#include "sort-array-by-parity.test.h"
#include "k-concatenation-maximum-sum.test.h"
#include "wildcard-matching.h"
#include "iocp-async-pattern.h"
#include "maximum-length-of-a-concatenated-string-with-unique-characters.test.h"
#include "permutation-and-combination.test.h"
#include "misc.small.h"

int _tmain(int argc, TCHAR ** argv)
{
	using namespace ::std;

	int testCase = -1;
    if (argc > 1)
    {
		testCase = _tcstol(argv[1], nullptr, 10);
    }

	//std::auto_ptr<int> ap(new int{ 100 });


	switch (testCase)
	{
	case 58:
	{
		SmartLib::TestUnique::Case0();
		SmartLib::TestUsing::Case0();
		SmartLib::STdSort::Case0();
	}
	break;
	case 57:
	{
		SmartLib::permutation_and_combinationTest::Case2();
		cout << endl;
		SmartLib::permutation_and_combinationTest::Case1();
		cout << endl;
		SmartLib::permutation_and_combinationTest::Case0();
		cout << endl;
		SmartLib::CombineNMTest::Case2();
		cout << endl;
		SmartLib::CombineNMTest::Case1();
		cout << endl;
		SmartLib::CombineNMTest::Case0();
		cout << endl;
	}
	break;
	case 56:
	{
		DWORD start = ::GetTickCount();
		SmartLib::AsyncFileCopyInParallel::Copy(argv[2], argv[3]);
		start = ::GetTickCount() - start;
		cout << "Elapsed time is " << start << " millsecs" << endl;
	}
	break;
	case 55:
	{
		DWORD start = ::GetTickCount();
		SmartLib::AsyncFileCopyInSequence::Copy(argv[2], argv[3]);
		start = ::GetTickCount() - start;
		cout << "Elapsed time is " << start << " millsecs" << endl;
	}
	break;
	case 54:
	{
		;
			
		//string str1{ "abbabaaabbabbaababbabbbbbabbbabbbabaaaaababababbbabababaabbababaabbbbbbaaaabababbbaabbbbaabbbbababababbaabbaababaabbbababababbbbaaabbbbbabaaaabbababbbbaababaabbababbbbbababbbabaaaaaaaabbbbbaabaaababaaaabb" };
		//string str2{ "**aa*****ba*a*bb**aa*ab****a*aaaaaa***a*aaaa**bbabb*b*b**aaaaaaaaa*a********ba*bbb***a*ba*bb*bb**a*b*bb" };

		string str1{ "**aa*****ba*a*bb**aa*ab****a*aaaaaa***a*aaaa**bbabb*b*b**aaaaaaaaa*a********ba*bbb***a*ba*bb*bb**a*b*bb" };
		string str2{ "**aa*****ba*a*bb**aa*ab****a*aaaaaa***a*aaaa**bbabb*b*b**aaaaaaaaa*a********ba*bbb***a*ba*bb*bb**a*b*bb" };
		bool rr = SmartLib::Solution_wildcard_matching::isMatch(str1, str2);
		cout << str1 << " and " << str2 << (rr ? " match" : " does not match") << endl;
	}
	break;
	case 53:
	{
		SmartLib::Solution_k_concatenation_maximum_sum_test::Case0();
		SmartLib::sort_array_by_parity_test::Case0();
	}
	break;
	case 52:
	{
		SmartLib::Solution_frog_jumps_test::Case1();
		//SmartLib::Solution_frog_jumps_test::Case0();
	}
	break;
	case 51:
	{
		SmartLib_partition_list::partition_list_test::Case0();
	}
	break;
	case 50:
	{
		SmartLib::interleaving_string_test::Case0();
	}
	break;
	case 49:
	{
		SmartLib::partition_equal_subset_sum_test::Case1();
		SmartLib::Knapsack01::Case0();
		SmartLib::partition_equal_subset_sum_test::Case0();
	}
	break;
	case 48:
	{
		SmartLib::TestCVCases::Case0();
	}
	break;
	case 47:
	{
		SmartLib::ListTestCases::Case1();
		SmartLib::ListTestCases::Case0();
	}
	break;
	case 46:
	{
		SmartLib::CMiscSmall::AdjustTaskOrder();;
		SmartLib_WordBreak::SolutionTest::Case0();
	}
	break;
	case 45:
	{
		Smartlib::FindMaxSeqTest::Case1();
	}
	break;
	case 44:
	{
		LMSTR netname = argv[2];
		NET_API_STATUS ns = NetShareDel(
			nullptr,//_In_  LMSTR servername,
			netname,//_In_  LMSTR netname,
			0//,//DWORD reserved
		);
		if (0 == ns)
		{
			SML_LOG_LINE(TEXT("SMB share %s was deleted"), netname);
		}
		else
		{
			SML_LOG_LINE(TEXT("SMB share %s cannot be deleted with error=%d"), netname, ns);
		}

	}
	break;
	case 43:
	{
		SmartLib::CTestApacheCrash::apxAddToPathW(argv[2]);
	}
	break;
	case 42:
	{
		SmartLib::CGetSpectialFolder::GetLoggerStream();
	}
	break;
	case 41:
	{
		volatile int loopCount = 1000;
		for (int ii = 0; ii < loopCount; ++ ii)
		{
			::Sleep(1000);
		}
		Smartlib::StartAppOnActiveUserSession::Pause();
		SmartLib::CStartAppOnActiveUserSessionTest::Case1();
		SmartLib::CStartAppOnActiveUserSessionTest::Case0();
	}
	break;
	case 40:
	{
		Smartlib::CTestWTS::Case7_WinSec();
		Smartlib::CTestWTS::Case6_WTSGetters();
		Smartlib::CTestWTS::Case5_WTSEnumerateSessionsEx();
		Smartlib::CTestWTS::Case4_WTSEnumerateSessions();
		//Smartlib::CTestWTS::Case3_WTSEnumerateServers();
		Smartlib::CTestWTS::Case2_WTSEnumerateProcessesEx();
		Smartlib::CTestWTS::Case1_WTSEnumerateProcesses();
		Smartlib::CTestWTS::Case0_WTSEnumerateListeners();
	}
	break;
	case  39:
	{
		SmartLib::CMiscSmall::Case0();
	}
	break;
	case 38:
	{
		if (argc > 3)
		{
			SmartLib::CHCPPCreator::Create(argv[2], argv[3]);
		}
		else
		{
			goto case_neg_one;
		}
		
	}
	break;
	case 37:
	{
		SmartLib::CHCPPCreatorTest::Case1();
	}
	break;
	case 36:
	{
		Smartlib::CReadConfigTest::Case0();
	}
	break;
	case 35:
	{
		SmartLib::CQuickSortTest::Case2();
		SmartLib::CQuickSortTest::Case1();
		SmartLib::CQuickSortTest::Case0();
	}
	break;
	case 34:
	{
		SmartLib::TestCppType::TestCallRealArgs::Case3();
		SmartLib::TestCppType::Case2();
		SmartLib::TestCppType::Case1();
		SmartLib::TestCppType::Case0();
	}
	break;
	case 33:
	{
		SmartLib::RefPtrTest::Case1();
		SmartLib::RefPtrTest::Case0();
		SmartLib::TestCppType::Case0();
	}
	break;
	case 32:
	{
		wstring strOrgHost = L"machine.work.local";
		wstring m_strHyperVDNSPrefix;
		wstring strShortNodeName;
		wstring::size_type pos = strOrgHost.find(m_strHyperVDNSPrefix);
		if (pos != wstring::npos)
		{
			strShortNodeName = strOrgHost.substr(0, pos);
		}
		else
		{
			strShortNodeName = strOrgHost;
		}
	}
	break;
	case 31:
	{
		SmartLib::TestUtf8Cout::Case3();
		SmartLib::TestUtf8Cout::Case2();
		SmartLib::TestUtf8Cout::Case1();
		SmartLib::TestUtf8Cout::Case0();
	}
	break;
	case 30:
	{
		sml_raii_clang_test_nomacro();
		sml_raii_clang_test1_nomacro();
		sml_raii_clang_test2_nomacro();
		sml_raii_clang_test2();
		sml_raii_clang_test1();
		sml_raii_clang_test();
		SmartLib::CLangRaiiTest::Case2_loop();
		SmartLib::CLangRaiiTest::Case1();
		SmartLib::CLangRaiiTest::Case0();
		SmartLib::SmlvecTest::Case0();
	}
	break;
	case 29:
	{
		auto p = strstr("adfaseriofskaehrfskahddasfdsaerfsaefsaefsaefsaefasefiuefydskjahekjfhsjkheljkfhase", "kaehrfskahddasfdsaerfsaefsaefsaefsaefasefiuefydskjahekjfhsjk");
		SML_LOG_LINE(TEXT("%S"), p);
		SmartLib::MaskBitCountToString::Case0();
	}
	break;
	case 28:
	{
		SmartLib::TestCallAsm::Case0();
	}
	break;
	case 27:
	{
		func_cleanup();
		func_counter();
		FuncInc(7, 8, 9);
	}
	break;
	case 26:
	{
		SmartLib::counted_ptr_test::Case1();
		SmartLib::TestSharedPtr::Case1();
	}
	break;
	case 25:
	{
		SmartLib::TestWinver::Case0();
	}
	break;
	case 24:
	{
		SmartLib::BlockAllocatorTest::Case0();
	}
	break;
	case 23:
	{
		SmartLib::MyAPIHooks::Case0();
		SmartLib::MyAPIHooks::Case1();
		SmartLib::CSocketConnect::Connect(L"www.baidu.com", L"80");
		SmartLib::TestCallAsm::Case0();
	}
	break;
	case 22:
	{
		SmartLib::DebugHelper::LoopWaitForDebug(3, 1000, false);
	}
	break;
	case 21:
	{
		SmartLib::TestEnumDir::Case_CEnumOpForWixSetup(argv[2], argv[3], argv[4]);
	}
	break;
	case 20:
	{
		SmartLib::TestEnumDir::Case_CEnumOpPrint(argv[2], argv[3]);
	}
	break;
	case 19:
	{
		//argv[2] is the file name
		SmartLib::CLoadLocalStrings::Case0(argv[2]);
	}
	break;
	case 18:
	{
		SmartLib::CFindFile::Case0();
	}
	break;
	case 17:
	{
		SmartLib::CBstrConv::Case0();
	}
	break;
	case 16:
	{
		SmartLib::TestConstExpr::Case0();
	}
	break;
	case 15:
	{
		SmartLib::TestWOSS::Case_print_CJK();
	}
	break;
	case 14:
	{
		
		SmartLib::TestWOSS::Case_conv_file();
		SmartLib::TestWOSS::Case_test_zetta();
		SmartLib::TestWOSS::WPrintCJK();
	}
	break;
	case 13:
	{
		SmartLib::TestWOSS::PrintCJK();
		SmartLib::TestWOSS::Case_Convertable_current_page();
		SmartLib::TestWOSS::Case_Convertable_utf8();
		SmartLib::TestWOSS::Case_Convertable_Japan();
		SmartLib::TestWOSS::Case_Convertable_China();

		SmartLib::TestWOSS::Case_Conv_China();
		SmartLib::TestWOSS::Case_Conv_Japn();
		SmartLib::TestWOSS::Case_EnumCodePage(TRUE);
		SmartLib::TestWOSS::Case_EnumCodePage(FALSE);
		SmartLib::TestWOSS::Case_CodePage();
	}
	break;
	case 12:
	{
		SmartLib::TestWOSS::Case3();
		SmartLib::TestWOSS::Case2();
	}
	break;
	case 11:
	{
		SmartLib::MyFormatCode::Case2(argv[2], argv[3]);
		SmartLib::MyFormatCode::Case1();
		SmartLib::MyFormatCode::Case0();
	}
	break;
	case  10:
	{
		SmartLib::TestWOSS::Case0(argv[2]);
		SmartLib::TestWOSS::Case1(argv[2]);
	}
	break;
	case 9:
	{
		SmartLib::TestArgs::Case0();
	}
	break;
	case 8:
	{
		MyStruct::main_case0();
	}
	break;
	case 7:
	{
		SmartLib::TestCRef::Case0();
	}
	break;
	case 6:
	{
		SmartLib::CIocpCopyFileTest::Case0(argv[2], argv[3]);
	}
	break;
	case 5:
	{
		SmartLib::IAllocatorTest::Case0();
	}
	break;
	case 4:
	{
		SmartLib::CPrintTypeAPITest::Case1();
		SmartLib::CPrintTypeAPITest::Case0();
	}
	break;
	case 3:
	{
		SmartLib::MyListTest::Case2();
		SmartLib::MyListTest::Case1();
		SmartLib::MyListTest::Case0();
	}
	break;
	case 2:
	{
		SmartLib::CSopeExitTest::Case0();
	}
	break;
	case  1:
	{
		SmartLib::SmlRValTest::Case4();
		SmartLib::SmlRValTest::Case3();
		SmartLib::SmlRValTest::Case2();
		SmartLib::SmlRValTest::Case1();
		SmartLib::SmlRValTest::Case0();
		SmartLib::CTestFwd::Case0();
	}
	break;
	case 0:
	{
		SML_LOG_LINE(TEXT("%S"), SML_EXPANDED_MACRO_STRING(_ftprintf_s(stdout, TEXT("%d"), ERROR_FILE_CORRUPT);));
		SML_LOG_LINE(TEXT("%s"), SML_EXPANDED_MACRO_WSTRING(_ftprintf_s(stdout, TEXT("%d"), ERROR_FILE_CORRUPT);));
	}
	break;
	case -1:
	{
		case_neg_one:
		SML_LOG_LINE("Print help here");
		SML_LOG_LINE("to create h and cpp: this.exe 38 <dir ending with'\\'> <file name without extension>");
	}
	break;
	}
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
