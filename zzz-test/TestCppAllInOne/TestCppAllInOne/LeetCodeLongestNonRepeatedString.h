#pragma once

//https://leetcode.com/problems/longest-substring-without-repeating-characters/submissions/

#include <map>
#include "vcruntime_string.h"  // for memset
#include "xstring"             // for string

namespace SmartLib
{
	using namespace ::std;

	class ArrayMap
	{
	private:
		int _array[256];
		int _size{ 0 };

	public:
		ArrayMap()
		{
			memset(_array, 0xff, sizeof(_array));
		}
		void insert(char ch, int index)
		{
			_array[ch] = index;
			++_size;
		}

		void remove(char ch)
		{
			_array[ch] = -1;
			--_size;
		}

		int find(char ch)
		{
			return _array[ch];
		}

		int size()
		{
			return _size;
		}

		void clear()
		{
			memset(_array, 0xff, sizeof(_array));
			_size = 0;
		}
	};

	class LeetCodeLongestNonRepeatedString
	{
	public:
	 static	int Run(const string& str)
		{
			if(str.empty())
			{
			 return 0;
			}

			ArrayMap arrmap;
			arrmap.insert(str[0], 0);
			int max = 1;
			int SIZE = str.size();
			for (int ii = 1; ii < SIZE; ++ ii)
			{
				char temp = str[ii];
				int pos = arrmap.find(temp);
				if (-1 != pos)
				{
					if (max < arrmap.size())
					{
						max = arrmap.size();
					}

					int keep_count = ii - pos - 1;
					int remove_count = arrmap.size() - keep_count;
					for (int jj = 0; jj < remove_count; ++ jj)
					{
						arrmap.remove(str[pos - jj]);
					}
					arrmap.insert(temp, ii);
				}
				else
				{
					arrmap.insert(temp, ii);
				}
			}

			if (max < arrmap.size())
			{
				max = arrmap.size();
			}


			return max;
		}

	};
}