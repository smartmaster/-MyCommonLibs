#pragma once


#include <cstring>
#include <cstdlib>
#include <ctime>

#include <vector>
#include <functional>

#include "log.h"

namespace SmartLib
{
	class Helper
	{
	public:
		
		static bool VerfiyOrder(std::vector<int> & vec, std::function<bool(int, int)> func)
		{
			bool rr = true;
			for (int ii = 0; ii < vec.size() - 1; ++ ii)
			{
				if (!func(vec[ii], vec[ii + 1]))
				{
					rr = false;
					break;
				}
			}

			return rr;
		}

		template<typename T>
		static bool VerfiyOrderG(std::vector<T> & vec, std::function<bool(const T &, const T &)> func)
		{
			bool rr = true;
			for (int ii = 0; ii < vec.size() - 1; ++ii)
			{
				if (!func(vec[ii], vec[ii + 1]))
				{
					rr = false;
					break;
				}
			}

			return rr;
		}



		template<typename T>
		static void FillRandom(std::vector<T> & vec, int count, T max)
		{
			std::srand(std::time(nullptr));
			for (int ii = 0; ii < count; ++ii)
			{
				vec.push_back(std::rand() % max);
			}
		}


		static void Print(const std::vector<int> & vec)
		{
			for (int ii = 0; ii < vec.size(); ++ ii)
			{
				LOG(L"%d ", vec[ii]);
			}

			LOG_LINE(L"");
		}



		static void Print(const std::vector<int> & vec, int first, int last)
		{
			for (int ii = first; ii <= last; ++ii)
			{
				LOG(L"%d ", vec[ii]);
			}

			LOG_LINE(L"");
		}


		static void PrintReverse(const std::vector<int> & vec, int first, int last)
		{
			for (int ii = last; ii >= first; ++ii)
			{
				LOG(L"%d ", vec[ii]);
			}

			LOG_LINE(L"");
		}


		template<typename T>
		static void IntDiv(T aa, T bb, T & integer, T & decimal)
		{
			const T DECIMAL = 10000;
			T rr = ((aa * DECIMAL * 10) / bb + 5) / 10;
			integer = rr / DECIMAL;
			decimal = rr % DECIMAL;
		}


		template<typename T>
		static std::string IntDivString(T aa, T bb)
		{
			T integer;
			T decimal;
			IntDiv(aa, bb, integer, decimal);

			char buffer[128] = { 0 };
			bool isNegtive = (integer < 0 || decimal < 0);
			sprintf_s(buffer, "%s%I64d.%04I64d", isNegtive? "-" : "", integer >= 0? integer : (-integer), decimal >= 0 ? decimal : (-decimal));

			return std::string(buffer);
		}

		static const char * IntToString(int val, char * buffer, int bufferSize)
		{
			buffer[0] = 0;

			sprintf_s(buffer, bufferSize, "%d", val);

			return buffer;
		}

		template<typename T>
		static void Reverse(T* first, T* last)
		{
			while (first < last)
			{
				T temp = std::move(*first);
				*first = std::move(*last);
				*last = std::move(temp);

				++first;
				--last;
			}
		}

		template<typename T>
		static void SwapSeq(T* first, int len1, int len2)
		{
			Reverse(first, first + len1 + len2 - 1);
			Reverse(first, first + len2 - 1);
			Reverse(first + len2, first + len2 + len1 - 1);
		}

	};


	class HelperTest
	{
	public:
		static void Case0()
		{
			char arr[] = "123456abcd";
			LOG_LINE_A("%s", arr);
			Helper::SwapSeq(arr, 6, 4);
			LOG_LINE_A("%s", arr);
		}
	};
}