#pragma once


#include <string.h>
#include <string>
#include <vector>


namespace SmartLib
{
	class MaxSeqNum
	{
		//	//评测题目: //评测题目:+给定一个任意长度的自然数序列，找出最大的数输出
		//例如:（1，2，3，4） > Max 4321
		//	(31, 3, 34, 37) > 3734331

	public:

		static std::string CalcArray(const std::vector<std::string> & vecStr)
		{
			std::string str;

			for (int ii = 0; ii < vecStr.size(); ++ii)
			{
				str += vecStr[ii];
			}

			return str;
		}



		static void RemoveOneElement(const std::vector<std::string> & vecStr, int index,
			std::vector<std::string> & vec)
		{
			for (int ii = 0; ii < vecStr.size(); ++ii)
			{
				if (ii != index)
				{
					vec.push_back(vecStr[ii]);
				}
			}
		}


		static std::string Max(const std::vector<std::string> & vecStr)
		{

			if (1 == vecStr.size())
			{
				return vecStr[0];
			}

			std::string rr;
			for (int ii = 0; ii < vecStr.size(); ++ii)
			{
				std::vector<std::string> vec;
				RemoveOneElement(vecStr, ii, vec);
				std::string max = Max(vec);

				std::vector<std::string> arr1 = { vecStr[ii], max };
				std::string rr1 = CalcArray(arr1);
				std::vector<std::string> arr2 = { max, vecStr[ii] };
				std::string rr2 = CalcArray(arr2);

				std::string rrTemp = rr1 > rr2 ? rr1 : rr2;
				if (rr < rrTemp)
				{
					rr = rrTemp;
				}
			}


			return rr;
		}


		static void IntArrToStringArr(const long * arr, int nn, std::vector<std::string> & vecStr)
		{
			char buffer[128] = { 0 };
			for (int ii = 0; ii < nn; ++ii)
			{
				sprintf_s(buffer, "%ld", arr[ii]);
				vecStr.push_back(buffer);
			}

		}




		//static std::string Max(const std::vector<std::string> & vec, int first, int last, std::vector<std::vector<std::string>> & saved)
		//{
		//	if (saved.size() == 0)
		//	{
		//		saved.resize(last - first + 1);
		//		for (auto & item : saved)
		//		{
		//			item.resize(last - first + 1);
		//		}
		//	}

		//	if (saved[first][last].size())
		//	{
		//		return saved[first][last];
		//	}

		//	if (first == last)
		//	{
		//		saved[first][last] = vec[first];
		//		return vec[first];
		//	}
		//	
		//}

		static  int Test()
		{
			long arr[] = { 31, 3,34,37 };
			std::vector<std::string> vecStr;
			IntArrToStringArr(arr, sizeof(arr) / sizeof(long), vecStr);

			std::string rr = Max(vecStr);
			printf("%s", rr.c_str());
			return 0;
		}

	};
}