#pragma once

#include <string>
#include <vector>

//https://leetcode.com/problems/zigzag-conversion/


namespace SmartLib
{
	using namespace ::std;

	class ZigzagConversion
	{
	private:
		const string& _str;
		int _nn;
		int _count;
	public:
		ZigzagConversion(const string& str, int nn) :
			_str{ str },
			_nn{ nn },
			_count{ 2 * nn - 2 }
		{
		}

		int GetRow(int pos)
		{
			pos %= _count;
			return pos < _nn ? pos : _count - pos;
		}

		string Run()
		{
			vector<string> rows( _nn );
			int size = _str.size();
			for (int ii = 0; ii < size; ++ ii)
			{
				rows[GetRow(ii)] += _str[ii];
			}

			string result;
			for (const auto& rr : rows)
			{
				result += rr;
			}

			return result;
		}
	};


	class Solution
	{
	public:
		static string  convert(const string& s, int numRows) {

			if (1 == numRows)
			{
				return s;
			}

			ZigzagConversion zzs{ s, numRows };
			return zzs.Run();
		}
	};
}