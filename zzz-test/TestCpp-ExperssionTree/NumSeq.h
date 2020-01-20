#pragma once

#include <vector>

#include "log.h"

namespace SmartLib
{
	class Helpers
	{
	public:
		template<class T>
		static void Print(T begin, T end)
		{
			for (T iter = begin; iter != end; ++iter)
			{
				LOG(L"%d ", (*iter) + 1);
			}
			LOG_LINE(L"");
		}
	};

	class Perm
	{
	private:
		int _nn{ 0 }; /***INCOMMING***/
		int _kk{ 0 }; /***INCOMMING***/
		std::vector<bool> _flags; /***INTERNAL***/
		std::vector<int> _stack; /***INTERNAL***/

	
	public:
		Perm(int nn, int kk) :
			_nn(nn),
			_kk(kk),
			_flags(nn)
		{
		}

		void RunPerm()
		{
			for (int ii = 0; ii < _nn; ++ ii)
			{
				if (!_flags[ii])
				{
					_flags[ii] = true;
					_stack.push_back(ii);

					if (_stack.size() == _kk)
					{
						Helpers::Print(_stack.begin(), _stack.end());
					}
					else
					{
						RunPerm(); //recursively call
					}

					_stack.pop_back();
					_flags[ii] = false;
				}
			}
		}
	};


	class Combine
	{
	private:
		int _nn{ 0 };	/***INCOMMING***/
		int _kk{ 0 };	/***INCOMMING***/
		std::vector<int> _stack; /***INTERNAL***/

	public:
		Combine(int nn, int kk) :
			_nn(nn),
			_kk(kk)
		{

			_stack.push_back(-1); //add a guard value
		}

		void RunCombine()
		{
			for (int ii = 0; ii < _nn; ++ii)
			{
				if (ii > _stack.back())
				{
					_stack.push_back(ii);

					if (_stack.size() == _kk + 1)
					{
						Helpers::Print(_stack.begin() + 1, _stack.end());
					}
					else
					{
						RunCombine();
					}

					_stack.pop_back();
				}
			}
		}
	};



	class NumSepTest
	{
	public:
		static void Case0()
		{
			int nn = 5; 
			int kk = 3;

			Perm pp(nn, kk);
			LOG_LINE(L"start perm (%d, %d)", nn, kk);
			pp.RunPerm();


			LOG_LINE(L"start combin (%d, %d)", nn, kk);
			Combine com(nn, kk);
			com.RunCombine();
		}
	};
}