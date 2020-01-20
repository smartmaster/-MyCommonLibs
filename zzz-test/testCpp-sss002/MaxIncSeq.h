#pragma once

#include <vector>

#include "log.h"
#include "RandFill.h"

namespace TestCpp
{
	class CMaxIncSeq
	{
	private:
		const std::vector<int> & _values; /***INCOMMING***/
		std::vector<int> _length; /***INTERNAL***/
		std::vector<int> _prev; /***INTERNAL***/

	public:
		CMaxIncSeq(const std::vector<int> & values) :
			_values(values),
			_length(values.size(), 1),
			_prev(values.size(), -1)
		{

		}

		void ProcessOne(int pos)
		{
			for (int ii = 0; ii < pos; ++ ii)
			{
				if (_values[pos] >= _values[ii])
				{
					if (_length[pos] < _length[ii] + 1)
					{
						_length[pos] = _length[ii] + 1;
						_prev[pos] = ii;
					}
				}
			}
		}


		int ProcessAll()
		{
			for (int pos = 0; pos < _values.size(); ++pos)
			{
				ProcessOne(pos);
			}


			int maxPos = 0;
			for (int ii = 1; ii < _length.size(); ++ ii)
			{
				if (_length[maxPos] < _length[ii])
				{
					maxPos = ii;
				}
			}

			return maxPos;
		}

		void PrintResult(int maxPos)
		{
			std::vector<int> pos;
			while (maxPos >= 0)
			{
				pos.push_back(maxPos);
				maxPos = _prev[maxPos];
			}


			for (auto iter = pos.crbegin(); iter != pos.crend(); ++ iter)
			{
				LOG_LINE(TEXT("[%d]=%d"), *iter, _values[*iter]);
			}
		}
	};


	class CMaxIncSeqTest
	{
	public:
		static void Case0()
		{
			std::vector<int> values;
			for (int ii = 1; ii < 10; ++ ii)
			{
				values.push_back(ii * 1000);
			}

			CMaxIncSeq mis{ values };
			int maxPos = mis.ProcessAll();
			mis.PrintResult(maxPos);

			LOG_LINE(TEXT(""));
		}


		static void Case1()
		{
			std::vector<int> values;
			for (int ii = 1; ii < 10; ++ii)
			{
				values.push_back(- ii * 1000);
			}

			CMaxIncSeq mis{ values };
			int maxPos = mis.ProcessAll();
			mis.PrintResult(maxPos);

			LOG_LINE(TEXT(""));
		}

		static void Case2()
		{
			std::vector<int> values(19);
			RandFill::Fill(values.begin(), values.end(), 1, 100);

			for (int ii = 0; ii < values.size(); ++ ii)
			{
				LOG_LINE(TEXT("[%d]=%d"), ii, values[ii]);
			}
			LOG_LINE(TEXT(""));

			CMaxIncSeq mis{ values };
			int maxPos = mis.ProcessAll();
			mis.PrintResult(maxPos);

			LOG_LINE(TEXT(""));


			for (int ii = 0; ii < values.size(); ++ ii)
			{
				LOG_LINE(TEXT("Results for pos %d ..."), ii);
				mis.PrintResult(ii);
				LOG_LINE(TEXT(""));
			}
		}

	};
}