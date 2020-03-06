
#include "./sml-iocp-async-task/SmlSocketIOTask.test.h"
#include "sml-iocp-async-task/SmlSocketAccepter.test.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include <string>
#include <iostream>

#include "SmlVector.test.h"
#include "SmlList.test.h"
#include "SmlRefPtr.test.h"
#include "SmlCircularBuffer.test.h"
#include "SmlMTQueue.test.h"
#include "SmlMisc.test.h"
#include "SmlTree.test.h"
#include "SmlVectorList.test.h"
#include "sml-iocp-async-task/SmlTaskRunnner.test.h"
#include "sml-iocp-async-task/SmlWin32FileIOTask.test.h"
#include "SmlTimeList.test.h"
#include "SmlVectorDeque.test.h"




long _tmain(long argc, TCHAR** argv)
{
	using namespace ::std;

	long testCase = -1;
	if (argc > 1)
	{
		testCase = _tcstol(argv[1], nullptr, 10);
	}

	//////////////////////////////////////////////////////////////////////////
	WSADATA wsaData{ 0 };
	WSAStartup(
		MAKEWORD(2, 2),//_In_   WORD wVersionRequested,
		&wsaData//_Out_  LPWSADATA lpWSAData
	);
	SmartLib::ScopeResource<> sr_wsaData{ []()
	{
		::WSACleanup();
	} };


	switch (testCase)
	{
	case 12:
	{
		SmartLib::DequeTest::Case3();
		SmartLib::DequeTest::Case2();
		SmartLib::DequeTest::Case1();
		SmartLib::DequeTest::Case0();
	}
	break;
	case 11:
	{
		SmartLib::SmlTimeListTest::Case0();
	}
	break;
	case 10:
	{
		SmartLib::SocketAccepterTest::Case0();
	}
	break;
	case 9:
	{
		ULONG start = ::GetTickCount();
		SmartLib::SmlWin32FileIOTaskTest::Case2();
		start = ::GetTickCount() - start;
		cout << "Copy file elapsed time is " << start << " milsec." << endl;
		//SmartLib::SmlWin32FileIOTaskTest::Case1();
		//SmartLib::SmlWin32FileIOTaskTest::Case0();
	}
	break;
	case 8:
	{
		SmartLib::TaskRunnerTest::Case0();
	}
	break;
	case 7:
	{
		SmartLib::SocketTaskTest::Case1();
		SmartLib::SocketTaskTest::Case0();
	}
	break;
	case 6:
	{
		SmartLib::VectorListTest::Case13();
		SmartLib::VectorListTest::Case12();
		SmartLib::VectorListTest::Case11();
		SmartLib::VectorListTest::Case10();
		SmartLib::VectorListTest::Case9();
		SmartLib::VectorListTest::Case8();
		SmartLib::VectorListTest::Case7();
		SmartLib::VectorListTest::Case6();
		SmartLib::VectorListTest::Case5();
		SmartLib::VectorListTest::Case4();
		SmartLib::VectorListTest::Case3();
		SmartLib::VectorListTest::Case2();
		SmartLib::VectorListTest::Case1();
		SmartLib::VectorListTest::Case0();
	}
	break;
	case 5:
	{
		SmartLib::TreeTest::Case1();
		SmartLib::TreeTest::Case0();
	}
	break;
	case 4:
	{
		SmartLib::MTQueueTest::Case1();
		SmartLib::MTQueueTest::Case0();
	}
	break;
	case 3:
	{
		SmartLib::CircularBufferTest::Case0();
	}
	break;
	case 2:
	{
		SmartLib::RefPtrTest::Case12();
		SmartLib::RefPtrTest::Case11();
		SmartLib::RefPtrTest::Case10();
		SmartLib::RefPtrTest::Case9();
		SmartLib::RefPtrTest::Case8();
		SmartLib::RefPtrTest::Case7();
		SmartLib::RefPtrTest::Case6();
		SmartLib::RefPtrTest::Case5();
		SmartLib::RefPtrTest::Case4(); //bug...
		SmartLib::RefPtrTest::Case3();
		SmartLib::RefPtrTest::Case2();
		SmartLib::RefPtrTest::Case1();
		SmartLib::RefPtrTest::Case0();
	}
	break;
	case  1:
	{
		SmartLib::ListTest::Case20();
		SmartLib::ListTest::Case19();
		SmartLib::ListTest::Case18();
		SmartLib::ListTest::Case17();
		SmartLib::ListTest::Case16();
		SmartLib::ListTest::Case15();
		SmartLib::ListTest::Case14();
		SmartLib::ListTest::Case13();
		SmartLib::ListTest::Case12(); //perf test
		SmartLib::ListTest::Case11();
		SmartLib::ListTest::Case10();
		SmartLib::ListTest::Case9();
		SmartLib::ListTest::Case8();
		SmartLib::ListTest::Case7();
		SmartLib::ListTest::Case6();
		SmartLib::ListTest::Case5();
		SmartLib::ListTest::Case4();
		SmartLib::ListTest::Case3();
		SmartLib::ListTest::Case2();
		SmartLib::ListTest::Case1();
		SmartLib::ListTest::Case0();
	}
	break;
	case 0:
	{
		SmartLib::VectorTest::Case18();
		SmartLib::VectorTest::Case17();
		SmartLib::VectorTest::Case16();
		SmartLib::VectorTest::Case15();
		SmartLib::VectorTest::Case14();
		SmartLib::VectorTest::Case13();
		SmartLib::VectorTest::Case12();
		SmartLib::VectorTest::Case11();
		SmartLib::VectorTest::Case10();
		SmartLib::VectorTest::Case9();
		SmartLib::VectorTest::Case8();
		SmartLib::VectorTest::Case7();
		SmartLib::VectorTest::Case6();
		SmartLib::VectorTest::Case5();//perf test
		SmartLib::VectorTest::Case4();
		SmartLib::VectorTest::Case3();
		SmartLib::VectorTest::Case2();
		SmartLib::VectorTest::Case1();
		SmartLib::VectorTest::Case0();
	}
	break;
	default:
	{
		//SmartLib::MiscTest2::Print();
		//SmartLib::RefPtrTest::Case11();
		//SmartLib::RefPtrTest::Case4();
		SmartLib::MiscTest::Case4();
		//SmartLib::MiscTest::Case3();
		//SmartLib::MiscTest::Case2();
		//SmartLib::MiscTest::Case1();
		//SmartLib::MiscTest::Case0();
		//SmartLib::MTQueueTest::Case1();
		//SmartLib::MTQueueTest::Case0();
		//SmartLib::CircularBufferTest::Case0();
		//SmartLib::ListTest::Case12(); //perf test
		//SmartLib::VectorTest::Case5();//perf test
	}
		break;
	}

}