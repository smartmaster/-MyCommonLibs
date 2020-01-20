#pragma once
#include "ListTest.h"


namespace SmartLib
{
	using namespace ::std;

	class ListTestCases
	{
	public:
		static void Case1()
		{
			//vector<int> vec{ 4, 2, 3, 1 };
			vector<int> vec{ -1,5,3,4,0 };

			auto head = SmartLib::ListTest::MakeList(vec);
			SmartLib::ListTest::Print(head);

			//////////////////////////////////////////////////////////////////////////
			head = SmartLib::ListTest6::sortList(head);
			SmartLib::ListTest::Print(head);
		}
		static void Case0()
		{
			vector<int> vec{4, 2, 3, 1};
			//for (int ii = 0; ii < 10; ++ ii)
			//{
			//	vec.push_back(ii);
			//}

			auto head = SmartLib::ListTest::MakeList(vec);
			SmartLib::ListTest::Print(head);

			//////////////////////////////////////////////////////////////////////////
			head = SmartLib::ListTest::Reverse(head);
			SmartLib::ListTest::Print(head);

			head = SmartLib::ListTest6::sortList(head);
			SmartLib::ListTest::Print(head);


			//////////////////////////////////////////////////////////////////////////
			head = SmartLib::ListTest::Reverse(head);
			SmartLib::ListTest::Print(head);

			head = SmartLib::ListTest3::sortList(head);
			SmartLib::ListTest::Print(head);

			//////////////////////////////////////////////////////////////////////////
			head = SmartLib::ListTest::Reverse(head);
			SmartLib::ListTest::Print(head);
			
			SmartLib::ListTest2::sortList(head);
			SmartLib::ListTest::Print(head);

			//////////////////////////////////////////////////////////////////////////
			head = SmartLib::ListTest::Reverse(head);
			SmartLib::ListTest::Print(head);

			SmartLib::ListTest::sortList(head);
			SmartLib::ListTest::Print(head);
		}
	};

}