#pragma once

#include <cstdlib>
#include <functional>
#include <iostream>

#include "../TaskRunner-old/SmlVector.h"

namespace SmartLib
{
	using namespace ::std;

	class Permutation
	{
	private:
		long _NN{ 0 };	/***INCOMMING***/
		long _MM{ 0 };	/***INCOMMING***/
		function<void(long index, const long* first, long count)> _handler; /***INCOMMING***//***INTERNAL***/

		long _index{ 0 };
		Vector<long, false, 1> _stack;	/***INTERNAL***/
		Vector<char, false, 1> _flags;	/***INTERNAL***/


	private:
		static void Print(long index, const long* first, long count)
		{
			cout << index << ": ";
			for (long ii = 0; ii < count; ++ii)
			{
				cout << first[ii] << " ";
			}
			cout << endl;
		}
	public:
		Permutation(long NN, long MM, function<void(long index, const long* first, long count)> handler) :
			_NN{ NN },
			_MM{ MM },
			_handler{ handler }
		{
			if (!handler)
			{
				_handler = Print;
			}

			_flags.resize(_NN);
			memset(_flags.First(), 0, _flags.size() * sizeof(char));
		}

		void Permute()
		{
			for (long ii = 0; ii < _NN; ++ii)
			{
				if (!_flags[ii])
				{
					_flags[ii] = 1;
					_stack.push_back(ii);
					if (_MM == _stack.size())
					{
						++_index;
						_handler(_index, _stack.First(), _stack.size());
					}
					else
					{
						Permute();
					}
					_stack.pop_back();
					_flags[ii] = 0;
				}
			}
		}
	};

	class Combination
	{
	private:
		long _NN{ 0 };	/***INCOMMING***/
		long _MM{ 0 };	/***INCOMMING***/
		function<void(long index, const long* first, long count)> _handler; /***INCOMMING***//***INTERNAL***/

		long _index{ 0 };
		Vector<long, false, 1> _stack;	/***INTERNAL***/


	private:
		static void Print(long index, const long* first, long count)
		{
			cout << index << ": ";
			for (long ii = 0; ii < count; ++ii)
			{
				cout << first[ii] << " ";
			}
			cout << endl;

		}



	public:

		Combination(long NN, long MM, function<void(long index, const long* first, long count)> handler) :
			_NN{ NN },
			_MM{ MM },
			_handler{ handler }
		{
			if (!handler)
			{
				_handler = Print;
			}
			_stack.push_back(-1); //put guard value
		}

		void Combine()
		{
			long start = _stack.back() + 1;
			for (long ii = start; ii < _NN; ++ii)
			{
				_stack.push_back(ii);
				if (_MM + 1 == _stack.size())
				{
					++_index;
					_handler(_index, _stack.First() + 1, _stack.size() - 1);
				}
				else
				{
					Combine();
				}
				_stack.pop_back();
			}
		}

		void CombineStart(long start)
		{
			for (long ii = start; ii < _NN; ++ii)
			{
				_stack.push_back(ii);
				if (_MM + 1 == _stack.size())
				{
					++_index;
					_handler(_index, _stack.First() + 1, _stack.size() - 1);
				}
				else
				{
					CombineStart(ii + 1);
				}
				_stack.pop_back();
			}
		}
	};
}