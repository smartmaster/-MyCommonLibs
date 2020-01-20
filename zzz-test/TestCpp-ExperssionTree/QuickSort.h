#pragma once

#include <vector>
#include <functional>
#include "Helper.h"

namespace SmartLib
{
	class QuickSortAlg
	{
	public:
		static int Find(std::vector<int> & vec, int first, int last, std::function<bool(int)> func)
		{
			int rr = last + 1;
			for (int ii = first; ii <= last; ++ ii)
			{
				if (func(vec[ii]))
				{
					rr = ii;
					break;
				}
			}
			return rr;
		}


		static int FindReverse(std::vector<int> & vec, int first, int last, std::function<bool(int)> func)
		{
			int rr = first - 1;
			for (int ii = last; ii >= first; --ii)
			{
				if (func(vec[ii]))
				{
					rr = ii;
					break;
				}
			}
			return rr;
		}


		static void Swap(std::vector<int> & vec, int ii, int jj)
		{
			if (ii != jj)
			{
				int temp = vec[ii];
				vec[ii] = vec[jj];
				vec[jj] = temp;
			}
			
		}


		static int Partition(std::vector<int> & vec, int first, int last)
		{
			int pivot = vec[first];

			int left = first + 1;
			int right = last;

			while (left <= right)
			{
				left = Find(vec, left, right, [pivot](int val) {return val >= pivot; });
				right = FindReverse(vec, left, right, [pivot](int val) {return val < pivot; });

				if (left < right)
				{
					Swap(vec, left, right);
					++left;
					--right;
				}
			}

			Swap(vec, first, right);
			return right;
		}


		static void QuickSort(std::vector<int> & vec, int first, int last)
		{
			if (first >= last)
			{
				return;
			}

			int part = Partition(vec, first, last);
			QuickSort(vec, first, part - 1);
			QuickSort(vec, part + 1, last);
		}

	};


	class QuickSortAlgTest
	{
	public:
		static void Case0()
		{
			std::vector<int> vec;
			Helper::FillRandom(vec, 17, 1000);
			Helper::Print(vec);
			QuickSortAlg::QuickSort(vec, 0, vec.size() - 1);
			if (Helper::VerfiyOrder(vec, [](int val1, int val2) { return val1 <= val2; }))
			{
				Helper::Print(vec);
			}
			else
			{
				LOG_LINE("Something wrong!");
			}
		}
	};
}