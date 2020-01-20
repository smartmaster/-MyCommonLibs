#pragma once


/********************************
https://leetcode.com/problems/zigzag-conversion/description/

The string "PAYPALISHIRING" is written in a zigzag pattern on a given number of rows like this: (you may want to display this pattern in a fixed font for better legibility)

P   A   H   N
A P L S I I G
Y   I   R
And then read line by line: "PAHNAPLSIIGYIR"



Write the code that will take a string and make this conversion given a number of rows:

string convert(string text, int nRows);
convert("PAYPALISHIRING", 3) should return "PAHNAPLSIIGYIR".

********************************/

#include <cassert>
#include <string>
#include <vector>
using namespace std;


namespace SmartLib
{
	class ZigZagString
	{
	private:
		const string _str; /***INCOMMING***/
		vector<string> _rows; /***INTERNAL***/


	private:
		int Process(int start)
		{
			for (int ii = 0; ii < _rows.size() && start < _str.size(); ++ ii)
			{
				_rows[ii] += _str[start ++];
			}

			for (int ii = 0; ii < (int)(_rows.size() - 2) && start < _str.size(); ++ ii)
			{
				_rows[_rows.size() - 2 - ii] += _str[start++];
			}

			return start;
		}

	public:
		
		ZigZagString(const string & s, int numRows) :
			_str(s),
			_rows(numRows)
		{
		}

		string Run()
		{
			int start = 0;
			while (start < _str.size())
			{
				start = Process(start);
			}

			string rv;
			for (const auto & str : _rows)
			{
				rv += str;
			}

			return rv;
		}
	};


	class ZigZagStringTest
	{
	public:
		static void Case0()
		{
			string sample = "PAYPALISHIRING";
			int rows = 3;
			ZigZagString zzs(sample, rows);
			string rv = zzs.Run();

			assert(rv == "PAHNAPLSIIGYIR");
		}
	};
}