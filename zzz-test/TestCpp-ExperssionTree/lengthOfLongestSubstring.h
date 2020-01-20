#pragma once

/****************************
https://leetcode.com/problems/longest-substring-without-repeating-characters/description/

Given a string, find the length of the longest substring without repeating characters.

Examples:

Given "abcabcbb", the answer is "abc", which the length is 3.

Given "bbbbb", the answer is "b", with the length of 1.

Given "pwwkew", the answer is "wke", with the length of 3. Note that the answer must be a substring, "pwke" is a subsequence and not a substring.

****************************/

#include <cassert>
#include <string>
#include <bitset>
using std::string;

#include "log.h"

namespace SmartLib
{
	class LengthOfLongestSubstringImpl
	{
	

	private:
		const string _str;
		int _max{ 1 };
		int _index{ 0 };
		std::bitset<256> _bitset;


	private:
		void SetChar(char ch)
		{
			_bitset.set(ch);
		}

		void ClearChar(char ch)
		{
			_bitset.reset(ch);
		}

		bool GetChar(char ch)
		{
			return	_bitset.test(ch);
		}

	public:

		LengthOfLongestSubstringImpl(const char * str) :
			_str(str)
		{
			//assert(_str.size());
		}

		int FindUniqueChar(int startIndex)
		{
			while (startIndex < _str.size() && !GetChar(/*_bitmap,*/ _str[startIndex]))
			{
				SetChar(/*_bitmap,*/ _str[startIndex]);
				++startIndex;
			}
			return startIndex;
		}

		int Run()
		{
			if (_str.size() == 0)
			{
				_max = 0;
				return 0;
			}

			SetChar(/*_bitmap,*/ _str[0]);
			int first = 1;
			for (int ii = 0; ii < _str.size(); ++ii)
			{
				int last = FindUniqueChar(first);
				if (_max < last - ii)
				{
					_max = last - ii;
					_index = ii;
				}

				ClearChar(/*_bitmap,*/ _str[ii]);
				first = last;
			}

			return _max;
		}

		void Print()
		{
			string rr;
			for (int ii = 0; ii < _max; ++ ii)
			{
				rr += _str[ii + _index];
			}
			LOG_LINE_A("sample=%s, maxLen=%d, index=%d, result=%s", _str.c_str(), _max, _index, rr.c_str());
		}
	};


	class LengthOfLongestSubstringTest
	{
	public:
		static void Case0()
		{
			const char * sample[] = 
			{
				"abcabcbb",
				"bbbbb",
				"pwwkew",
			};



			for (int ii = 0; ii < _countof(sample); ++ ii)
			{
				LengthOfLongestSubstringImpl lls(sample[ii]);
				int maxLen = lls.Run();
				lls.Print();
			}
		}
	};
}