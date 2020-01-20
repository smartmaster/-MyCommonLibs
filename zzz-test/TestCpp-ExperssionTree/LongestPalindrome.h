#pragma once

#include <cassert>
#include <string>
#include <vector>
using namespace std;

#include "log.h"

namespace SmartLib
{
	class LongestPalindrome
	{
	private:
		const string _str;  /***INCOMMING***/
		vector<vector<char>> _saved; /***INTERNAL***/

		static const char _NULL = 'N';
		static const char _TRUE = 'T';
		static const char _FALSE = 'F';

		int _first{ 0 }; /***INTERNAL***/
		int _last{ 0 }; /***INTERNAL***/


	private:
		bool IsPalindrome(int first, int last)
		{
			//assert(first <= last);

			char saved = _saved[first][last];
			if (_NULL != saved)
			{
				return saved == _TRUE;
			}

			bool rv = false;
			if (first == last)
			{
				rv = true;
			}
			else if (first + 1 == last)
			{
				rv = (_str[first] == _str[last]);
			}
			else
			{
				rv = (_str[first] == _str[last]) && (IsPalindrome(first + 1, last - 1));
			}

			rv ? _saved[first][last] = _TRUE : _saved[first][last] = _FALSE;

			return rv;
		}

	public:
		LongestPalindrome(const string & str) :
			_str(str)
		{
			_saved.resize(_str.size());
			for (auto & vec : _saved)
			{
				vec.resize(_str.size(), _NULL);
			}
		}

		string Run()
		{
			int max = 0;
			for (int first = 0; first < _str.size(); ++ first)
			{
				for (int last = first; last < _str.size(); ++ last)
				{
					if (IsPalindrome(first, last))
					{
						int len = last - first + 1;
						if (max < len)
						{
							max = len;
							_first = first;
							_last = last;
						}
					}
				}
			}

			return _str.substr(_first, max);
		}
	};

	class LongestPalindromeTest
	{
	public:
		static void Case0()
		{
			vector<string> samples
			{
				"cbbd",
				"babad"
			};

			for (const auto & str : samples)
			{
				LongestPalindrome lp(str);
				string rr = lp.Run();
				LOG_LINE_A("%s --> %s", str.c_str(), rr.c_str());
			}
		}
	};
}