#pragma once
#include "ComposeMax.h"
#include "SmlQuickSort.test.h"
#include "log.h"

namespace Smartlib
{
	using namespace std;

	class FindMaxSeqTest
	{
	public:
		static void Case1()
		{
			const int ARRAY_SIZE = 17;
			int arr[ARRAY_SIZE];
			SmartLib::CQuickSortTest::RandomFill(arr, arr + ARRAY_SIZE - 1, 1, 10000);
			vector<wstring> numbers;
			for (int ii = 0; ii < ARRAY_SIZE; ++ ii)
			{
				numbers.push_back(to_wstring(arr[ii]));
			}

			SmartLib::FindMaxSeq fms{ numbers };
			wstring result1 = fms.Solution();
			wstring result2 = fms.Solution2();

			if (result1 == result2)
			{
				SML_LOG_LINE(TEXT("The results of solutions match."));
			}
			else
			{
				SML_LOG_LINE(TEXT("The results of solutions DOES NOT match!"));
			}

		}
	};
}