#pragma once

#include <vector>

#include "QuickSort.h"
#include "Helper.h"
#include "log.h"

namespace SmartLib
{
	class DupNumber
	{
	public:
		static void Swap(std::vector<int> & vec, int ii, int jj)
		{
			if (ii != jj)
			{
				int temp = vec[ii];
				vec[ii] = vec[jj];
				vec[jj] = temp;
			}
		}

		static void Place(std::vector<int> & vec, int ii)
		{
			while (vec[ii] != ii)
			{
				if (vec[vec[ii]] == vec[ii])
				{
					break;
				}
				else
				{
					Swap(vec, ii, vec[ii]);
				}
			}
		}


		static void DupNum(std::vector<int> & vec)
		{
			for (int ii = 0; ii < vec.size(); ++ ii)
			{
				Place(vec, ii);
			}

			for (int ii = 0; ii < vec.size(); ++ii)
			{
				if (vec[ii] != -1 && vec[ii] != ii && vec[vec[ii]] != -1)
				{
					LOG(L"%d ", vec[ii]);
					vec[vec[ii]] = -1;
				}
			}
			LOG_LINE(L"");


			for (int ii = 0; ii < vec.size(); ++ii)
			{
				if (vec[ii] == -1)
				{
					vec[ii] = ii;
				}
			}
		}

	};

	class DupNumberTest
	{
	public:
		static void Case0()
		{
			std::vector<int> vec;
			Helper::FillRandom(vec, 20, 1);

			LOG_LINE(L"Orgianl array:");
			Helper::Print(vec);
			

			LOG_LINE(L"Dup Number:");
			DupNumber::DupNum(vec);

			LOG_LINE(L"Adjusted array for dup number:");
			Helper::Print(vec);

			QuickSortAlg::QuickSort(vec, 0, vec.size() - 1);

			LOG_LINE(L"Sorted array:");
			Helper::Print(vec);
		}
	};

}