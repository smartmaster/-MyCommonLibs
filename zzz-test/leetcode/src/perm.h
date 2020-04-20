#pragma once

#include <vector>
#include <iostream>

namespace SmartLib
{
	using namespace ::std;


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
			PermAlg alg{ count, count - 1   };
			alg.Combine();
		}
	};
}