#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <random>
#include <cstdlib>
#include <ctime>

namespace SmartLib
{
	using namespace std;

	class MyHeap
	{
	private:
		bool _isRootMax{ true };
		vector<int> _data;

		function<bool(int, int)> _compare;
	public:

		MyHeap(bool isRootMax) :
			_isRootMax{ isRootMax }
		{
			if (_isRootMax)
			{
				_compare = [](int child, int parent) {return child < parent; };
			}
			else
			{
				_compare = [](int child, int parent) {return child > parent; };
			}
		}

		int Root() const
		{
			return _data.front();
		}

		int size() const
		{
			return _data.size();
		}

		void make()
		{
			make_heap(_data.begin(), _data.end(), _compare);
		}

		void push(int num)
		{
			_data.push_back(num);
			push_heap(_data.begin(), _data.end(),_compare);
			
		}


		int pop()
		{
			pop_heap(_data.begin(), _data.end(), _compare);

			int num = _data.back();
			_data.pop_back();

			return num;
		}
	};

	class FindMiddle
	{
	private:
		MyHeap _left{ true };
		MyHeap _right{ false };

	public:
		void Insert(int num)
		{
			if (_left.size() < _right.size())
			{
				if (_right.size() && num > _right.Root())
				{
					_right.push(num);
					num = _right.pop();
				}

				_left.push(num);
			}
			else
			{
				if (_left.size() && num < _left.Root())
				{
					_left.push(num);
					num = _left.pop();
				}

				_right.push(num);
			}
		}


		vector<int> Solution(int loop)
		{
			for (int ii = 0; ii < loop; ++ ii)
			{
				Insert(ii);
			}


			if (_left.size() == _right.size())
			{
				return { _left.Root(), _right.Root() };
			}
			else if (_left.size() > _right.size())
			{
				return { _left.Root() };
			}
			else
			{
				return {  _right.Root() };
			}
		}
	};

	class HeapTest
	{
	public:

		static void Print(const vector<int>& vec)
		{
			for (int data : vec)
			{
				cout << data << ' ';
			}
			cout << endl;
		}
		static void Case2()
		{
			{
				FindMiddle fm1;
				auto vec = fm1.Solution(8);
				Print(vec);
			}
			
			{
				FindMiddle fm1;
				auto vec = fm1.Solution(9);
				Print(vec);
			}


			{
				FindMiddle fm1;
				auto vec = fm1.Solution(1000);
				Print(vec);
			}
			
			{
				FindMiddle fm1;
				auto vec = fm1.Solution(1001);
				Print(vec);
			}

		}


		static void Case1()
		{
			MyHeap left{ true };
			MyHeap right{ false };


			srand(time(nullptr));

			for (long ii = 0; ii < 17; ++ ii)
			{
				int num = rand() % 1000;
				left.push(num);
				right.push(num);
			}

			//////////////////////////////////////////////////////////////////////////
			while (left.size())
			{
				cout << left.pop() << ' ';
			}
			cout << endl;

			//////////////////////////////////////////////////////////////////////////
			while (right.size())
			{
				cout << right.pop() << ' ';
			}
			cout << endl;
		}


		static void Case0()
		{
			vector <int> v1;
			vector <int>::iterator Iter1, Iter2;

			int i;
			for (i = 1; i <= 9; i++)
				v1.push_back(i);

			// Make v1 a heap with default less than ordering
			random_device rd;
			mt19937 gen{rd()};
			shuffle(v1.begin(), v1.end(), gen);
			make_heap(v1.begin(), v1.end());
			cout << "The heaped version of vector v1 is ( ";
			for (Iter1 = v1.begin(); Iter1 != v1.end(); Iter1++)
				cout << *Iter1 << " ";
			cout << ")." << endl;

			// Add an element to the back of the heap
			v1.push_back(10);
			push_heap(v1.begin(), v1.end());
			cout << "The reheaped v1 with 10 added is ( ";
			for (Iter1 = v1.begin(); Iter1 != v1.end(); Iter1++)
				cout << *Iter1 << " ";
			cout << ")." << endl;

			// Remove the largest element from the heap
			pop_heap(v1.begin(), v1.end());
			cout << "The heap v1 with 10 removed is ( ";
			for (Iter1 = v1.begin(); Iter1 != v1.end(); Iter1++)
				cout << *Iter1 << " ";
			cout << ")." << endl << endl;

			// Make v1 a heap with greater-than ordering with a 0 element
			make_heap(v1.begin(), v1.end(), greater<int>());
			v1.push_back(0);
			push_heap(v1.begin(), v1.end(), greater<int>());
			cout << "The 'greater than' reheaped v1 puts the smallest "
				<< "element first:\n ( ";
			for (Iter1 = v1.begin(); Iter1 != v1.end(); Iter1++)
				cout << *Iter1 << " ";
			cout << ")." << endl;

			// Application of pop_heap to remove the smallest element
			pop_heap(v1.begin(), v1.end(), greater<int>());
			cout << "The 'greater than' heaped v1 with the smallest element\n "
				<< "removed from the heap is: ( ";
			for (Iter1 = v1.begin(); Iter1 != v1.end(); Iter1++)
				cout << *Iter1 << " ";
			cout << ")." << endl;

		}
	};
}