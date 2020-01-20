#pragma once

#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>
#include <string>

#include <cstdlib>
#include <ctime>

#include "Helper.h"

using std::vector;
using std::string;
using std::function;

namespace SmartLib
{
	class QuickSortWithOrder
	{
	private:
		vector<int> & _vec;
		function<bool(int, int)> _order;

		int Find(int first, int last, function<bool(int)> func)
		{
			int result = last + 1;
			for (int ii = first; ii <= last; ++ ii)
			{
				if (func(_vec[ii]))
				{
					result = ii;
					break;
				}
			}

			return result;
		}


		int FindReverse(int first, int last, function<bool(int)> func)
		{
			int result = first - 1;
			for (int ii = last; ii >= first; -- ii)
			{
				if (func(_vec[ii]))
				{
					result = ii;
					break;
				}
			}

			return result;
		}

		void SwapByIndex(int ii, int jj)
		{
			if (ii != jj)
			{
				int temp = _vec[ii];
				_vec[ii] = _vec[jj];
				_vec[jj] = temp;
			}
		}


		int Partition(int first, int last)
		{
			int pivot = _vec[first];
			int left = first + 1;
			int right = last;

			while (left <= right)
			{
				left = Find(left, right, [this, pivot](int val) { return !_order(val, pivot); });
				right = FindReverse(left, right, [this, pivot](int val) { return _order(val, pivot); });

				if (left < right)
				{
					SwapByIndex(left, right);
					++left;
					--right;
				}
			}

			SwapByIndex(first, right);
			return right;
		}


		void Sort(int first, int last)
		{
			if (first >= last)
			{
				return;
			}
			int part = Partition(first, last);
			Sort(first, part - 1);
			Sort(part + 1, last);
		}


		QuickSortWithOrder(vector<int> & vec, function<bool(int, int)> order) :
			_vec(vec),
			_order(order)
		{
		}


	public:
		static void Run(vector<int> & vec, function<bool(int, int)> order)
		{
			QuickSortWithOrder qso(vec, order);
			qso.Sort(0, vec.size() - 1);
		}
	};



	class QuickSortWithOrderTest
	{
	public:
		static void Case0()
		{
			vector<int> vec;
			Helper::FillRandom(vec, 17, 1000);
			Helper::Print(vec);

			
			auto func = [](int val1, int val2)
			{
				return val1 > val2;
			};

			QuickSortWithOrder::Run(vec, func);
			bool ok = Helper::VerfiyOrder(vec, func);
			if (ok)
			{
				LOG_LINE_A("Correct");
			}
			else
			{
				LOG_LINE_A("Something wrong");
			}
			Helper::Print(vec);

		}

		static void Case1()
		{
			struct data1
			{
				int _val;
				string _string;
			};

			std::srand(std::time(nullptr));

			const int MAX = 1000;

			const int COUNT = 17;
			vector<data1*> vecData(COUNT);

			char buffer[128] = { 0 };
			for (int ii = 0; ii < COUNT; ++ ii)
			{
				data1 * temp = new data1;
				temp->_val = std::rand() % MAX;
				temp->_string = Helper::IntToString(std::rand() % MAX, buffer, _countof(buffer));
				vecData[ii] = temp;
			}

			vector<int> ref(COUNT);
			for (int ii = 0; ii < COUNT; ++ ii)
			{
				ref[ii] = ii;
			}


			auto func1 = [&vecData](int ref1, int ref2)
			{
				return vecData[ref1]->_val > vecData[ref2]->_val;
			};
			QuickSortWithOrder::Run(ref, func1);
			bool ok = Helper::VerfiyOrder(ref, func1);
			if (ok)
			{
				std::cout << "Correct!" << std::endl;
			}
			else
			{
				std::cout << "Something wrong!" << std::endl;
			}
			std::for_each(ref.begin(), ref.end(), [&vecData](int val)
			{
				std::cout << vecData[val]->_val << ' ';
			});
			std::cout << std::endl;
			std::for_each(ref.begin(), ref.end(), [&vecData](int val)
			{
				std::cout << vecData[val]->_string << ' ';
			});
			std::cout << std::endl;



			auto func2 = [&vecData](int ref1, int ref2) -> bool
			{
				return vecData[ref1]->_string > vecData[ref2]->_string;
			};
			QuickSortWithOrder::Run(ref, func2);
			ok = Helper::VerfiyOrder(ref, func2);
			if (ok)
			{
				std::cout << "Correct!" << std::endl;
			}
			else
			{
				std::cout << "Something wrong!" << std::endl;
			}
			std::for_each(ref.begin(), ref.end(), [&vecData](int val)
			{
				std::cout << vecData[val]->_val << ' ';
			});
			std::cout << std::endl;
			std::for_each(ref.begin(), ref.end(), [&vecData](int val)
			{
				std::cout << vecData[val]->_string << ' ';
			});
			std::cout << std::endl;

			
		}
	};

}