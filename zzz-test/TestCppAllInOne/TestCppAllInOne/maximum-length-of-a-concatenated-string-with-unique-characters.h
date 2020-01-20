#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>


//https://leetcode.com/problems/maximum-length-of-a-concatenated-string-with-unique-characters/

namespace SmartLib_maximum_length_of_a_concatenated_string_with_unique_characters
{
	using namespace ::std;

	class CombineMethod
	{
	private:
		vector<long> _values; //converted values of strings
		long _max{ 0 }; //calculated max value till now
		long _mask{ 0 }; //calaculated mask till now

	public:
		static long Str2Long(const string& str)
		{
			long rr = 0;
			for (const auto ch : str)
			{
				long temp = (1 << (ch - 'a'));
				if (rr & temp)
				{
					rr = -1;
					break;
				}
				else
				{
					rr |= temp;
				}

			}
			return rr;
		}

		static long CountBits(long LL)
		{
			long count = 0;
			while (LL)
			{
				++count;
				LL &= (LL - 1);
			}

			return count;
		}

		static void VectorStr2Long(const vector<string>& strs, vector<long>& vl)
		{
			for (const auto& str : strs)
			{
				long temp = Str2Long(str);
				if (temp > 0)
				{
					vl.push_back(temp);
				}
			}
		}


	public:
		void Combine(long start)
		{

			for (long ii = start; ii < _values.size(); ++ ii)
			{
				long tempval = _values[ii];
				if (0 == (_mask & tempval))
				{
					_mask |= tempval;
					Combine(ii + 1); //continue next
					_mask ^= tempval; //back trace
				}
				else
				{
					long tempmax = CountBits(_mask);
					if (_max < tempmax)
					{
						_max = tempmax;
					}
				}
			}
		}


		long MaxLen(const vector<string>& arr)
		{
			VectorStr2Long(arr, _values);
			if (0 == _values.size())
			{
				return 0;
			}

			_values.push_back(-1); //put -1 as an end marker
			Combine(0);

			return _max;
		}
	};


	class Solution_maximum_length_of_a_concatenated_string_with_unique_characters
	{
	public:
		static int maxLength(const vector<string>& arr)
		{
			return CombineMethod{}.MaxLen(arr);
		}
	};

}

namespace SmartLib
{
	using namespace std;


	class PermNM
	{
	private:
		vector<long> _stack{ {-1} };
		long _NN{ 0 };
		long _MM{ 0 };
		long _count{ 0 };

		vector<bool> _flag;
		function<void(const vector<long>&)> _handler;

	private:
		void Print()
		{
			cout << _count << ": ";
			for (long ii = 1; ii < _stack.size(); ++ii)
			{
				cout << _stack[ii] << " ";
			}
			cout << endl;
		}

	public:
		PermNM(long NN, long MM, function<void(const vector<long>&)> handler) :
			_NN{ NN },
			_MM{ MM },
			_handler{handler}
		{
			_flag.resize(_NN);

			if (!_handler)
			{
				_handler = [this](const vector<long>& stack)
				{
					Print();
				};
			}
		}

		void Perm()
		{
			for (long ii = 0; ii < _NN; ++ii)
			{
				if (!_flag[ii])
				{
					_flag[ii] = true;
					_stack.push_back(ii);
					if (_stack.size() == _MM + 1)
					{
						++_count;
						_handler(_stack);
						
					}
					Perm();
					_stack.pop_back();
					_flag[ii] = false;
				}
			}
		}
	};

	class CombineNM
	{
	private:
		vector<long> _stack{ {-1} };
		long _NN{ 0 };
		long _MM{ 0 };
		long _count{ 0 };
		function<void(const vector<long>&)> _handler;

	private:
		void Print()
		{
			cout << _count << ": ";
			for (long ii = 1; ii < _stack.size(); ++ii)
			{
				cout << _stack[ii] << " ";
			}
			cout << endl;
		}

	public:
		CombineNM(long NN, long MM, function<void(const vector<long>&)> handler) :
			_NN{ NN },
			_MM{ MM },
			_handler{ handler }
		{
			if (!_handler)
			{
				_handler = [this](const vector<long>& stack)
				{
					Print();
				};
			}
		}

		void Combine()
		{
			long start = _stack.back() + 1;
			for (long ii = start; ii < _NN; ++ii)
			{
				_stack.push_back(ii);
				if (_stack.size() == _MM + 1)
				{
					++_count;
					_handler(_stack);
					
				}
				Combine();
				_stack.pop_back();
			}
		}

	};

	class maximum_length_of_a_concatenated_string_with_unique_characters
	{
	private:
		vector<long> _values;

	public:
		static long Str2Long(const string& str)
		{
			long rr = 0;
			for (auto ch : str)
			{
				long temp = (1 << (ch - 'a'));
				if (rr & temp)
				{
					rr = -1;
					break;
				}
				else
				{
					rr |= temp;
				}
				
			}

			return rr;
		}

		static long CountBits(long LL)
		{
			long count = 0;
			while (LL)
			{
				++count;
				LL &= (LL - 1);
			}

			return count;
		}

		static bool IsUnique(long L1, long L2)
		{
			//return CountBits(L1 | L2) == CountBits(L1) + CountBits(L2);
			return 0 == (L1 & L2);
		}

		static void VectorStr2Long(const vector<string>& strs, vector<long>& vl)
		{
			for (const auto& str : strs)
			{
				long temp = Str2Long(str);
				if (temp > 0)
				{
					vl.push_back(temp);
				}
			}
		}


	public:
		long _max = 0;
		void dfs(long index, long mask)
		{
			long bitcount = CountBits(mask);
			if (_max < bitcount)
			{
				_max = bitcount;
			}
			
			long total = _values.size();
			for (long ii = index; ii < total; ++ ii)
			{
				if (0 == (mask & _values[ii]))
				{
					dfs(ii + 1, (_values[ii] | mask));
				}
			}
		}


		long maxLengthDFS(const vector<string>& arr)
		{
			VectorStr2Long(arr, _values);
			dfs(0, 0);
			return _max;
		}

		long maxLength(const vector<string>& arr) 
		{
			VectorStr2Long(arr, _values);
			

			long max = 0;
			auto calc = [this, &max](const vector<long>& pos)
			{
				long ss = 0;
				for (long ii = 1; ii < pos.size(); ++ ii)
				{
					long value = _values[pos[ii]];
					if (IsUnique(ss, value))
					{
						ss |= value;
					}
					else
					{
						ss = 0;
						break;
					}
				}

				if (ss)
				{
					long temp = CountBits(ss);
					if (max < temp)
					{
						max = temp;
					}
				}
			};

			long total = _values.size();
			for (long count = 1; count <= total; ++ count)
			{
				CombineNM{total, count, calc}.Combine();
			}

			return max;
		}
	};

	class Solution_maximum_length_of_a_concatenated_string_with_unique_characters
	{
	public:
		static int maxLength(vector<string>& arr) 
		{
			//return maximum_length_of_a_concatenated_string_with_unique_characters{}.maxLength(arr);
			return maximum_length_of_a_concatenated_string_with_unique_characters{}.maxLengthDFS(arr);
		}
	};

}