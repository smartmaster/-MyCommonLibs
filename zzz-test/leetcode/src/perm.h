#pragma once

#include <vector>
#include <iostream>


#include "sml-common.h"

namespace SmartLib
{
	using namespace ::std;

	class DFSPermAlg
	{
		long _MAX_NUMBER{ 0 }; /***INCOMMING***/
		long _MAX_COUNT{ 0 }; /***INCOMMING***/

		long _currentCount{ 0 };
		vector<long> _currentStack;
		vector<bool> _selected;

	public:
		void DFS(long start)
		{
			if (_currentCount < _MAX_COUNT)
			{
				if (!_selected[start])
				{
					++_currentCount;
					_currentStack.push_back(start);
					_selected[start] = true;

					if (_currentCount == _MAX_COUNT)
					{
						Common::PrintIterator(cout, _currentStack.begin(), _currentStack.end()) << endl;
					}


					for (long ii = 0; ii < _MAX_NUMBER; ++ii)
					{
						DFS(ii);
					}

					_currentStack.pop_back();
					--_currentCount;
					_selected[start] = false;

				}
			}
		}

		void DFSALL()
		{
			for (long ii = 0; ii < _MAX_NUMBER; ++ii)
			{
				DFS(ii);
			}
		}

		DFSPermAlg(long maxNumber, long maxCount) :
			_MAX_NUMBER{ maxNumber },
			_MAX_COUNT{ maxCount }
		{
			_selected.resize(_MAX_NUMBER, false);
		}

	public:
		static void Solution(long maxNumber, long maxCount)
		{
			DFSPermAlg{ maxNumber, maxCount }.DFSALL();
		}
	};

	class DFSCombineAlg
	{
		long _MAX_NUMBER{ 0 }; /***INCOMMING***/
		long _MAX_COUNT{ 0 }; /***INCOMMING***/

		long _currentCount{ 0 };
		vector<long> _currentStack;
	public:
		void DFS(long start)
		{
			if (_currentCount < _MAX_COUNT)
			{
				++_currentCount;
				_currentStack.push_back(start);
				if (_currentCount == _MAX_COUNT)
				{
					Common::PrintIterator(cout, _currentStack.begin(), _currentStack.end()) << endl;
				}

				for (long ii = start + 1; ii < _MAX_NUMBER; ++ii)
				{
					DFS(ii);
				}

				--_currentCount;
				_currentStack.pop_back();
			}
		}

		void DFSALL()
		{
			for (long ii = 0; ii <= _MAX_NUMBER - _MAX_COUNT; ++ii)
			{
				DFS(ii);
			}
		}

		DFSCombineAlg(long maxNumber, long maxCount) :
			_MAX_NUMBER{ maxNumber },
			_MAX_COUNT{ maxCount }
		{

		}
	public:
		static void Solution(long maxNumber, long maxCount)
		{
			DFSCombineAlg{ maxNumber, maxCount }.DFSALL();
		}
	};


	class PermAlg
	{
	private:
		long _count{ 0 };
		long _dataCount{ 0 };
		vector<long> _data;
		vector<long> _stack;

	public:
		PermAlg(long count, long dataCount) :
			_count{ count },
			_dataCount{ dataCount },
			_stack{ {-1} }
		{
			_data.resize(_count);
			for (long ii = 0; ii < _count; ++ii)
			{
				_data[ii] = ii;
			}
		}

		template<typename T>
		static void MySwap(T& val1, T& val2)
		{
			auto temp = move(val1);
			val1 = move(val2);
			val2 = move(temp);
		}

		static void Print(const vector<long>& vec)
		{
			for (auto val : vec)
			{
				cout << val << " ";
			}
			cout << endl;
		}

		void Perm()
		{

			for (long ii = 0; ii < _count; ++ii)
			{
				if (_data[ii] >= 0)
				{

					long saved = _data[ii];
					_stack.push_back(saved);

					if (_stack.size() == _dataCount + 1)
					{
						Print(_stack);
					}

					_data[ii] = -1;
					Perm();
					_data[ii] = saved;
					_stack.pop_back();


				}
			}
		}

		void Combine()
		{

			for (long ii = 0; ii < _count; ++ii)
			{
				if (_data[ii] >= 0)
				{
					long saved = _data[ii];
					_data[ii] = -1;

					if (saved > _stack.back())
					{
						_stack.push_back(saved);
						if (_stack.size() == _dataCount + 1)
						{
							Print(_stack);
						}

						Combine();

						_stack.pop_back();
					}


					_data[ii] = saved;
				}

			}


		}



		static void Solution(long count)
		{
			PermAlg alg{ count, count - 1 };
			alg.Perm();
		}

		static void SolutionCombine(long count)
		{
			PermAlg alg{ count, count - 1 };
			alg.Combine();
		}
	};



}