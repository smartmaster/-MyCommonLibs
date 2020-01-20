#pragma once

#include <vector>
#include <string>
#include <bitset>
using namespace std;

#include "log.h"

namespace SmartLib
{
	

	class  LongestValidParenthesesDP
	{
	private:
		const std::string & _str;
		vector<vector<char>> _saved;

		const char _NULL{ 'N' };
		const char _TRUE{ 'T' };
		const char _FALSE{ 'F' };

		const char LEFT{ '(' };
		const char RIGHT{ ')' };

	private:
		LongestValidParenthesesDP(const string & str) :
			_str(str)
		{
			_saved.resize(_str.size() + 1);
			for (auto & vec : _saved)
			{
				vec.resize(_str.size() + 1, _NULL);
			}
		}

		bool IsOkay(int ii, int jj)
		{
			char saved = _saved[ii][jj];
			if (saved != _NULL)
			{
				return saved == _TRUE;
			}

			bool rv = false;
			if (((jj - ii) & 1) == 0)
			{
				rv = false;
			}
			else if(ii + 1 == jj)
			{
				rv = (_str[ii] == LEFT && _str[jj] == RIGHT);
			}
			else
			{
				bool rv1 = (_str[ii] == LEFT && _str[ii + 1] == RIGHT && IsOkay(ii + 2, jj));
				bool rv2 = (_str[jj - 1] == LEFT && _str[jj] == RIGHT && IsOkay(ii, jj - 2));
				bool rv3 = (_str[ii] == LEFT && _str[jj] == RIGHT && IsOkay(ii + 1, jj - 1));
				rv = rv1 || rv2 || rv3;
			}
			
			_saved[ii][jj] = (rv ? _TRUE : _FALSE);
			return rv;
		}

		int Run()
		{
			int max = 0;
			for (int ii = 0; ii < _str.size(); ++ ii)
			{
				for (int jj = ii + 1; jj < _str.size(); ++ jj)
				{
					if (IsOkay(ii, jj))
					{
						int temp = jj - ii + 1;
						if (max < temp)
						{
							max = temp;
						}
					}
				}
			}

			return max;
		}

	public:
		static int Run(const string & str)
		{
			LongestValidParenthesesDP lvp(str);
			return lvp.Run();
		}
	};

	class  LongestValidParentheses
	{
	public:

		static bool Merge(pair<int, int> & current, pair<int, int> & before)
		{
			bool rv = false;
			if (current.first < before.first && current.second > before.second)
			{
				before = current;
				rv = true;
			}
			else if (before.second + 1 == current.first)
			{
				before.second = current.second;
				rv = true;
			}
			return rv;
		}
		static int Run(const std::string & ss)
		{
			const char LEFT = '(';
			const char RIGHT = ')';

			std::vector<char> data;
			data.reserve(ss.size() + 1);
			data.push_back(0); //add a guard element

			std::vector<int> index;
			index.reserve(ss.size() + 1);
			index.push_back(-1); //add a guard element

			vector<pair<int, int>> ranges;
			ranges.reserve(ss.size() + 1);

			for (int ii = 0; ii < ss.size(); ++ ii)
			{
				if (ss[ii] == LEFT)
				{
					data.push_back(LEFT);
					index.push_back(ii);
				}
				else if (ss[ii] == RIGHT)
				{
					if (data.back() == LEFT)
					{
						ranges.push_back({index.back(), ii});
						data.pop_back();
						index.pop_back();
					}
					else
					{
						data.push_back(RIGHT);
					}
				}
			}

			int max = 0;
			
			for (int ii = ranges.size() - 1; ii >= 1; -- ii)
			{
				
				if (Merge(ranges[ii], ranges[ii - 1]))
				{

				}
				else
				{
					int temp = ranges[ii].second - ranges[ii].first + 1;
					if (max < temp)
					{
						max = temp;
					}
				}
			}

			if (ranges.size())
			{
				int temp1 = ranges[0].second - ranges[0].first + 1;
				if (max < temp1)
				{
					max = temp1;
				}
			}

			

			return max;
		}

	};

	class  LongestValidParenthesesTest
	{
	public:
		static void Case0()
		{
			
			{
				std::string str = "((((()())()()))()(()))";
				//std::string str = "(((()()))()())";
				int rr = LongestValidParentheses::Run(str);
				LOG_LINE_A("%s --> %d", str.c_str(), rr);
				rr = LongestValidParenthesesDP::Run(str);
				LOG_LINE_A("%s --> %d", str.c_str(), rr);
			}
			{
				std::string str = ")(((((()())()()))()(()))(";
				int rr = LongestValidParentheses::Run(str);
				LOG_LINE_A("%s --> %d", str.c_str(), rr);
				rr = LongestValidParenthesesDP::Run(str);
				LOG_LINE_A("%s --> %d", str.c_str(), rr);
			}

			{
				std::string str = "";
				int rr = LongestValidParentheses::Run(str);
				LOG_LINE_A("%s --> %d", str.c_str(), rr);
				rr = LongestValidParenthesesDP::Run(str);
				LOG_LINE_A("%s --> %d", str.c_str(), rr);
			}

			{
				std::string str = "(()";
				int rr = LongestValidParentheses::Run(str);
				LOG_LINE_A("%s --> %d", str.c_str(), rr);
				rr = LongestValidParenthesesDP::Run(str);
				LOG_LINE_A("%s --> %d", str.c_str(), rr);
			}


			{
				std::string str = ")()())";
				int rr = LongestValidParentheses::Run(str);
				LOG_LINE_A("%s --> %d", str.c_str(), rr);
				rr = LongestValidParenthesesDP::Run(str);
				LOG_LINE_A("%s --> %d", str.c_str(), rr);
			}


			{
				std::string str = "()(())";
				int rr = LongestValidParentheses::Run(str);
				LOG_LINE_A("%s --> %d", str.c_str(), rr);
				rr = LongestValidParenthesesDP::Run(str);
				LOG_LINE_A("%s --> %d", str.c_str(), rr);
			}
		}
	};
}