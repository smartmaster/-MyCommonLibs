#pragma once


#include <cassert>

#include <vector>
#include <functional>
#include <iostream>

#include "Helper.h"

using std::vector;
using std::function;

namespace SmartLib
{
	template<typename T>
	class SmlBinHeap
	{
	private:
		vector<T> & _vec;
		int _heapSize{ 0 };
		function<bool(const T&, const T&)> _order;


	private:

		
		void SwapByIndex(int ii, int jj)
		{
			if (ii != jj)
			{
				T temp = std::move(_vec[ii]);
				_vec[ii] = std::move(_vec[jj]);
				_vec[jj] = std::move(temp);
			}
		}


		void BubbleLast()
		{
			int current = _heapSize - 1;
			while (current)
			{
				int parent = (current + 1) / 2 - 1;
				if (_order(_vec[parent], _vec[current]))
				{
					break;
				}

				SwapByIndex(current, parent);
				current = parent;
			}
		}

		void SinkFirst()
		{
			int current = 0;
			while (current < _heapSize)
			{
				int left = current * 2 + 1;
				int right = left + 1;

				if (left < _heapSize && right < _heapSize)
				{
					if (_order(_vec[current], _vec[left]) && _order(_vec[current], _vec[right]))
					{
						break;
					}

					int down = _order(_vec[left], _vec[right]) ? left : right;
					SwapByIndex(current, down);
					current = down;
				}
				else if (left < _heapSize)
				{
					if (_order(_vec[current], _vec[left]))
					{
						break;
					}

					SwapByIndex(current, left);
					current = left;
				}
				else
				{
					break;
				}
			}
		}

	public:

		SmlBinHeap(vector<T> & vec, function<bool(const T&, const T&)> order) :
			_vec(vec),
			_order(order)
		{
		}

		int HeapSize() const
		{
			return _heapSize;
		}


		bool VerifyOrder()
		{
			bool rv = true;
			for (int ii = 0; ii < _heapSize; ++ ii)
			{
				int left = ii * 2 + 1;
				int right = left + 1;

				if (left < _heapSize && right < _heapSize)
				{
					if (!(_order(_vec[ii], _vec[left]) && _order(_vec[ii], _vec[right])))
					{
						rv = false;
						break;
					}
				}
				else if (left < _heapSize)
				{
					if (!_order(_vec[ii], _vec[left]))
					{
						rv = false;
						break;
					}
				}
				else
				{
					break;
				}
			}

			return rv;
		}

		void Add(const T & val)
		{
			++_heapSize;
			if (_heapSize > _vec.size())
			{
				_vec.push_back(val);
			}
			else
			{
				_vec[_heapSize - 1] = val;
			}

			BubbleLast();
		}

		const T & Remove()
		{
			assert(_heapSize > 0);

			--_heapSize;
			SwapByIndex(0, _heapSize);
			SinkFirst();
			return _vec[_heapSize];
		}

		void MakeHeap()
		{
			assert(_heapSize == 0);
			for (int ii = 0; ii < _vec.size(); ++ii)
			{
				++_heapSize;
				BubbleLast();
			}
		}

		static void Sort(vector<T> & vec, function<bool(const T&, const T&)> func)
		{
			SmlBinHeap heap(vec, func);
			heap.MakeHeap();
			while (heap.HeapSize())
			{
				heap.Remove();
			}
		}
	};



	class SmlHeapSortTest
	{
	public:
		static void Case1()
		{
			vector<int> vec;
			Helper::FillRandom(vec, 19, 100);
			Helper::Print(vec);
			SmlBinHeap<int>::Sort(vec, [](int  val1, int val2) { return val1 >= val2;  });
			bool ok = Helper::VerfiyOrder(vec, [](int  val1,  int  val2) {return val1 <= val2; });
			if (ok)
			{
				std::cout << "Correct!" << std::endl;
			}
			else
			{
				std::cout << "Something wrong!" << std::endl;
			}
			Helper::Print(vec);
		}

		static void Case0()
		{
			//////////////////////////////////////////////////////////////////////////
			vector<int> vec;
			const int max = 1000;
			const int count1 = 17;
			Helper::FillRandom(vec, count1, max);

			auto func = [](const int & val1, const int & val2)
			{
				return val1 <= val2;
			};

			SmlBinHeap<int> heap(vec, func);
			heap.MakeHeap();

			//////////////////////////////////////////////////////////////////////////
			const int count2 = 9;
			for (int ii = 0; ii < count2; ++ ii)
			{
				heap.Add(std::rand() % max);
			}

			//////////////////////////////////////////////////////////////////////////
			if (heap.VerifyOrder())
			{
				std::cout << "Correct!" << std::endl;
			}
			else
			{
				std::cout << "Something wrong!" << std::endl;
			}

			Helper::Print(vec);

			//////////////////////////////////////////////////////////////////////////
			while (heap.HeapSize())
			{
				heap.Remove();
			}

			if (Helper::VerfiyOrder(vec, [](int val, int val2) {return val >= val2; }))
			{
				std::cout << "Correct!" << std::endl;
			}
			else
			{
				std::cout << "Something wrong!" << std::endl;
			}
			Helper::Print(vec);

			//////////////////////////////////////////////////////////////////////////
			heap.MakeHeap();
			if (heap.VerifyOrder())
			{
				std::cout << "Correct!" << std::endl;
			}
			else
			{
				std::cout << "Something wrong!" << std::endl;
			}

			Helper::Print(vec);

			//////////////////////////////////////////////////////////////////////////
			while (heap.HeapSize())
			{
				heap.Remove();
			}

			if (Helper::VerfiyOrder(vec, [](int val, int val2) {return val >= val2; }))
			{
				std::cout << "Correct!" << std::endl;
			}
			else
			{
				std::cout << "Something wrong!" << std::endl;
			}
			Helper::Print(vec);

		}
	};
}