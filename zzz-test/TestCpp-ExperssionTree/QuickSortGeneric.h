#pragma once

#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include <iostream>

#include "Helper.h"

using std::vector;
using std::string;
using std::function;

namespace SmartLib
{
	template<typename T>
	class QuickSortGeneric
	{
	private:
		vector<T> & _vec;
		function<bool(const T&, const T&)> _order;

		int Find(int first, int last, function<bool(const T&)> func)
		{
			int result = last + 1;
			for (int ii = first; ii <= last; ++ii)
			{
				if (func(_vec[ii]))
				{
					result = ii;
					break;
				}
			}

			return result;
		}


		int FindReverse(int first, int last, function<bool(const T&)> func)
		{
			int result = first - 1;
			for (int ii = last; ii >= first; --ii)
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
				T temp = std::move(_vec[ii]) ;
				_vec[ii] = std::move(_vec[jj]);
				_vec[jj] = std::move(temp);
			}
		}


		int Partition(int first, int last)
		{
			int left = first + 1;
			int right = last;

			while (left <= right)
			{
				left = Find(left, right, [this, first](const T & val) { return !_order(val, _vec[first]); });
				right = FindReverse(left, right, [this, first](const T & val) { return _order(val, _vec[first]); });

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

	public:
		void GetTopN(int nn, int first, int last) //loop forever if with same elements
		{
			if (nn >= last - first + 1)
			{
				return;
			}

			int part = Partition(first, last);
			int count = part - first + 1;
			if (count == nn)
			{
				return;
			}

			if (count < nn)
			{
				GetTopN(nn - count, part + 1, last);
			}
			else
			{
				if (part == last)
				{
					return;
				}
				GetTopN(nn, first, part);
				
			}

		}


		QuickSortGeneric(vector<T> & vec, function<bool(const T&, const T&)> order) :
			_vec(vec),
			_order(order)
		{
		}


	public:
		static void Run(vector<T> & vec, function<bool(const T&, const T&)> order)
		{
			QuickSortGeneric<T> qsg(vec, order);
			qsg.Sort(0, vec.size() - 1);
		}
	};



	class QuickSortGenericTest
	{
	public:
		static void Case2()
		{
			vector<int> vec;
			const int count = 100;
			for (int ii = 0; ii < count; ++ii)
			{
				//vec.push_back(count - ii);
				vec.push_back(9999);
				
			}
			vec[vec.size() / 2] = 888;


			QuickSortGeneric<int> qso(vec, [](int val1, int val2) { return val1 <= val2; });

			const int NN = 10;
			qso.GetTopN(NN, 0, vec.size() - 1);

			for (int ii = 0; ii < NN; ++ ii)
			{
				std::cout << vec[ii] << std::endl;
			}

		}

		static void Case1()
		{
			struct data
			{
				int _val{ 0 };
				string _string;
			};

			std::srand(std::time(nullptr));

			const int MAX = 1000;

			const int COUNT = 17;
			vector<data*> vecData(COUNT);

			char buffer[128] = { 0 };
			for (int ii = 0; ii < COUNT; ++ii)
			{
				data * temp = new data;
				temp->_val = std::rand() % MAX;
				temp->_string = Helper::IntToString(std::rand() % MAX, buffer, _countof(buffer));
				vecData[ii] = temp;
			}

	

			auto func1 = [](const data * const & data1, const data * const & data2)
			{
				return data1->_val >= data2->_val;
			};
			QuickSortGeneric<data*>::Run(vecData, func1);
			bool ok = Helper::VerfiyOrderG<data*>(vecData, func1);
			if (ok)
			{
				std::cout << "Correct!" << std::endl;
			}
			else
			{
				std::cout << "Something wrong!" << std::endl;
			}
			std::for_each(vecData.begin(), vecData.end(), [](const data * val)
			{
				std::cout << val->_val << ' ';
			});
			std::cout << std::endl;
			std::for_each(vecData.begin(), vecData.end(), [](const data * val)
			{
				std::cout << val->_string << ' ';
			});
			std::cout << std::endl;



			auto func2 = [](data * const & val1 , data * const & val2)
			{
				return val1->_string >= val2->_string;
			};
			QuickSortGeneric<data*>::Run(vecData, func2);
			ok = Helper::VerfiyOrderG<data*>(vecData, func2);
			if (ok)
			{
				std::cout << "Correct!" << std::endl;
			}
			else
			{
				std::cout << "Something wrong!" << std::endl;
			}
			std::for_each(vecData.begin(), vecData.end(), [](const data * val)
			{
				std::cout << val->_val << ' ';
			});
			std::cout << std::endl;
			std::for_each(vecData.begin(), vecData.end(), [](const data * val)
			{
				std::cout << val->_string << ' ';
			});
			std::cout << std::endl;


		}


		static void Case0()
		{
			vector<int> vec;
			Helper::FillRandom(vec, 17, 1000);
			Helper::Print(vec);


			auto func = [](const int & val1, const int & val2)
			{
				return val1 > val2;
			};

			QuickSortGeneric<int>::Run(vec, func);
			bool ok = Helper::VerfiyOrderG<int>(vec, func);
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
	};

}