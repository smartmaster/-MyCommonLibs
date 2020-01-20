#pragma once


#include <vector>
#include <iostream>

using namespace std;

namespace SmartLib
{
	class CalcSum
	{
	private:
		int _targetSum{ 0 };
		int _curSum{ 0 };
		vector<int> _stack{ 0 };


	private:

		CalcSum(int targetSum) :
			_targetSum(targetSum)
		{
		}

		void PrintOneResult()
		{
	
			cout << _stack[1];
			for (int ii = 2; ii < _stack.size(); ++ ii)
			{
				cout << " + " << _stack[ii];
			}

			cout << endl;
		}

		void DFS()
		{
			for (int ii = 1; ii < _targetSum; ++ ii)
			{
				if (ii >= _stack.back() && _curSum + ii <= _targetSum)
				{
					_stack.push_back(ii);
					_curSum += ii;
					if (_curSum == _targetSum)
					{
						PrintOneResult();
					}
					else
					{
						DFS();
					}
					_curSum -= ii;
					_stack.pop_back();
				}
			}
		}

	public:
		static void API(int targetSum)
		{
			CalcSum cs(targetSum);
			cs.DFS();
		}
		
	};
	

	class CalcSumTest
	{
	public:
		static void Case0()
		{
			int targetSUm = 5;
			CalcSum::API(targetSUm);
		}

	};
}