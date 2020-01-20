#pragma once


#include <cassert>
#include <string>
#include <vector>


#include "log.h"
#include "Helper.h"

using std::vector;
using std::string;

//评测题目:给定一个任意长度的自然数序列，找出最大的数输出
//例如:（1，2，3，4） > Max 4321
//例如: (31, 3, 34, 37) > 3734331

namespace SmartLib
{
	class MaxSeq
	{
	private:
		vector<string> _seq; /***INCOMMING***/

	

	private:

		template<typename T>
		static void Reverse(T * first, T * last)
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
		static void ReverseSeq(T* first, int offset, int len1, int len2)
		{
			Reverse(first + offset, first + offset + len1 + len2 - 1);
			Reverse(first + offset, first + offset + len2 - 1);
			Reverse(first + offset + len2, first + offset + len2 - 1);
		}

		static void IntArrToStringArr(const long * arr, int nn, vector<string> & vecStr)
		{
			char buffer[128] = { 0 };
			for (int ii = 0; ii < nn; ++ii)
			{
				sprintf_s(buffer, "%ld", arr[ii]);
				vecStr.push_back(buffer);
			}

		}

		string CalcString(const vector<int> & order)
		{
			string str;
			for (int index : order)
			{
				str += _seq[index];
			}
			return str;
		}

		void StepForward(vector<int> & vec, int pos)
		{
			assert(pos > 0 && pos < vec.size());
			int temp = vec[pos];
			vec[pos] = vec[pos - 1];
			vec[pos - 1] = temp;
		}


		void FindNextMax(const vector<int> & order, int newIndex, vector<int> & nextOrder, string & nextVal)
		{
			assert(newIndex == order.size());

			nextOrder.clear();
			nextVal.clear();

			vector<int> newOrder = order;
			newOrder.push_back(newIndex);
			

			for (int ii = newOrder.size() - 1; ii >= 0; --ii)
			{
				string nextValTemp = CalcString(newOrder);
				if (nextVal < nextValTemp)
				{
					nextVal = nextValTemp;
					nextOrder = newOrder;
				}

				if (ii != 0)
				{
					StepForward(newOrder, ii);
				}
			}
		}


		void MaxOrder(int nn, vector<int> & rrOrder, string & rrString)
		{
			if (1 == nn)
			{
				rrOrder = { 0 };
				rrString = _seq[0];
				return;
			}

			vector<int> rrOrderTemp;
			string rrStringTemp;
			MaxOrder(nn - 1, rrOrderTemp, rrStringTemp);

			FindNextMax(rrOrderTemp, nn - 1, rrOrder, rrString);
		}


		void MaxOrderNR(vector<int> & rrOrder, string & rrString)
		{
			assert(_seq.size());

			vector<int> order{ 0 };
			for (int ii = 1; ii <= _seq.size() - 1; ++ ii)
			{
				FindNextMax(order, ii, rrOrder, rrString);
				order = rrOrder;
			}
		}


	public:
		static void Run(const long * arr, int nn)
		{
			MaxSeq mq;
			IntArrToStringArr(arr, nn, mq._seq);

			vector<int> order;
			string result;
			mq.MaxOrder(mq._seq.size(), order, result);

			for (int index : order)
			{
				LOG_LINE_A("seq[%d]=%s", index, mq._seq[index].c_str());
			}
			LOG_LINE_A("result = %s", result.c_str());
		}

		static void RunNR(const long * arr, int nn)
		{
			MaxSeq mq;
			IntArrToStringArr(arr, nn, mq._seq);

			vector<int> order;
			string result;
			mq.MaxOrderNR(order, result);

			for (int index : order)
			{
				LOG_LINE_A("seq[%d]=%s", index, mq._seq[index].c_str());
			}
			LOG_LINE_A("result = %s", result.c_str());
		}

		
	};


	class MaxSeqTest
	{
	public:
		static  int Case0()
		{
			vector<long> vec = { 31, 3,34,37 };

			//vector<long> vec;
			//Helper::FillRandom(vec, 7, 201L);

			MaxSeq::Run(&vec[0], vec.size());

			MaxSeq::RunNR(&vec[0], vec.size());


			return 0;
		}


	};
}