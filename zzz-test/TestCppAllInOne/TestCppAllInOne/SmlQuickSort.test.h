#pragma once

#include <random>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <list>
#include <string>
#include "SmlQuickSort.h"



namespace SmartLib
{
	class CQuickSortTest
	{
	public:
		static void RandomFill(int* first, int* last, int min, int max)
		{
			//std::knuth_b randeng;
			//std::uniform_int_distribution<int> distr(min, max);
			std::srand(std::time(nullptr));
			while (first <= last)
			{
				//*first = distr(randeng);
				*first = std::rand() % (max - min) + min;
				++first;
			}
		}

		
		static void PrintList(int* first, int* last)
		{
			while (first <= last)
			{
				std::cout << *first << ' ';
				++first;
			}
			std::cout << std::endl;
		}

		static bool Verify(int* first, int* last)
		{
			bool result = true;
			while (first < last)
			{
				if (first[0] > first[1])
				{
					result = false;
					break;
				}
				++first;
			}
			return result;
		}

#if false
		static void RandomFillString(std::list<std::wstring>& lst, int count, int min, int max)
		{
			std::srand(std::time(nullptr));
			for (int ii = 0; ii < count; ++ii)
			{
				int ival = std::rand() % (max - min) + min;
				lst.push_back(std::to_wstring(ival));
			}
		}

		template<typename TIterator>
		static void PrintListG(TIterator first, TIterator end)
		{
			while (first < end)
			{
				std::cout << *first << ' ';
				++first;
			}
			std::cout << std::endl;
		}

		template<typename TIterator>
		static bool VerifyG(TIterator first, TIterator last)
		{
			bool result = true;
			while (first < last)
			{
				auto next = first;
				++next;
				if (*first > *next)
				{
					result = false;
					break;
				}
				++first;
			}
			return result;
		}
#endif

	public:
		static void Case0()
		{
			int arr[13];
			const int arr_len = _countof(arr);
			RandomFill(arr, arr + arr_len - 1, 100, 200);
			PrintList(arr, arr + arr_len - 1);

			CQuickSort::Sort(arr, arr + arr_len - 1);
			PrintList(arr, arr + arr_len - 1);

			if (Verify(arr, arr + arr_len - 1))
			{
				std::cout << "OK!" << std::endl;
			}
			else
			{
				std::cout << "ERRPR! please fix code." << std::endl;
			}

		}

		static void Case1()
		{
			int arr[13];
			const int arr_len = _countof(arr);
			RandomFill(arr, arr + arr_len - 1, 100, 200);
			PrintList(arr, arr + arr_len - 1);

			CQuickSort::SortNR(arr, arr + arr_len - 1);
			PrintList(arr, arr + arr_len - 1);

			if (Verify(arr, arr + arr_len - 1))
			{
				std::cout << "OK!" << std::endl;
			}
			else
			{
				std::cout << "ERRPR! please fix code." << std::endl;
			}

		}

		static void Case2()
		{
			int arr[13];
			const int arr_len = _countof(arr);
			RandomFill(arr, arr + arr_len - 1, 100, 200);
			PrintList(arr, arr + arr_len - 1);

			CQuickSort2::Sort(arr, arr + arr_len - 1);
			PrintList(arr, arr + arr_len - 1);

			if (Verify(arr, arr + arr_len - 1))
			{
				std::cout << "OK!" << std::endl;
			}
			else
			{
				std::cout << "ERRPR! please fix code." << std::endl;
			}

		}

#if false
		static void Case1()
		{
			std::list<std::wstring> lst;
			const int lst_len = 13;
			RandomFillString(lst, lst_len, 100, 200);
			PrintListG(lst.begin(), lst.end());

			CQuickSortGeneric::Sort(lst.begin(), lst.end()--);
			PrintListG(lst.begin(), lst.end());

			if (VerifyG(lst.begin(), lst.end()--))
			{
				std::cout << "OK!" << std::endl;
			}
			else
			{
				std::cout << "ERRPR! please fix code." << std::endl;
			}

		}
#endif

	};
}