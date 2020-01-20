#pragma once

/********************
https://leetcode.com/problems/multiply-strings/description/

43. Multiply Strings
DescriptionHintsSubmissionsDiscussSolution
Given two non-negative integers num1 and num2 represented as strings, return the product of num1 and num2, also represented as a string.

Example 1:

Input: num1 = "2", num2 = "3"
Output: "6"
Example 2:

Input: num1 = "123", num2 = "456"
Output: "56088"
Note:

The length of both num1 and num2 is < 110.
Both num1 and num2 contain only digits 0-9.
Both num1 and num2 do not contain any leading zero, except the number 0 itself.
You must not use any built-in BigInteger library or convert the inputs to integer directly.
*********************/

#include <vector>
#include <string>
using namespace std;

#include "log.h"

namespace SmartLib
{
	class MultiplyStrings
	{
	private:
		template<typename T>
		static void ReverseElement(vector<T> & vec, int first, int last)
		{
			while (first < last)
			{
				T temp = std::move(vec[first]);
				vec[first] = std::move(vec[last]);
				vec[last] = std::move(temp);
				++first;
				--last;
			}
		}

		template<typename T>
		static void ReverseSequence(vector<T> & vec, int offset, int len1, int len2)
		{
			ReverseElement(vec, offset, offset + len1 + len2 - 1);
			ReverseElement(vec, offset, offset + len2 - 1);
			ReverseElement(vec, offset + len2, offset + len1 + len2 - 1);
		}

		static void Add(vector<char> & nn1, const vector<char> & nn2)
		{
			int newSize = nn1.size() > nn2.size() ? nn1.size() + 1 : nn2.size() + 1;
			nn1.resize(newSize);

			int ii1 = 0;
			int ii2 = 0;
			int carry = 0;
			while (ii2 < nn2.size())
			{
				nn1[ii1] += nn2[ii2] + carry;
				if (nn1[ii1] >= 10)
				{
					nn1[ii1] -= 10;
					carry = 1;
				}
				else
				{
					carry = 0;
				}

				++ii1;
				++ii2;
			}

			while (ii1 < nn1.size())
			{
				nn1[ii1] += carry;
				if (nn1[ii1] >= 10)
				{
					nn1[ii1] -= 10;
					carry = 1;
				}
				else
				{
					carry = 0;
				}

				++ii1;
			}

			//nn1[ii1] = carry;
		}

		static void ShiftLeft(vector<char> & nn)
		{
			nn.push_back(0);
			ReverseSequence(nn, 0, nn.size() - 1, 1);
		}


		static vector<char> Multiply(const vector<char> & nn1, const vector<char> & nn2)
		{
			vector<vector<char>> saved(10);
			saved[0].push_back(0);

			for (int ii = 1; ii < saved.size(); ++ ii)
			{
				saved[ii] = saved[ii - 1];
				Add(saved[ii], nn1);
			}

			vector<char> rr{ 0 };
			for (int ii = nn2.size() - 1; ii >= 0; -- ii)
			{
				ShiftLeft(rr);
				Add(rr, saved[nn2[ii]]);
			}

			return rr;
		}

		static vector<char> S2V(const string & str)
		{
			vector<char> rr(str.size());
			for (int ii = 0; ii < str.size(); ++ ii)
			{
				rr[ii] = str[str.size() - ii - 1] - '0';
			}

			//ReverseElement(rr, 0, rr.size() - 1);
			return rr;
		}


		static string V2S(const vector<char> & vec)
		{
			string str;
			int ii = vec.size() - 1;
			while (ii > 0)
			{
				if (vec[ii] == 0)
				{
					--ii;
				}
				else
				{
					break;
				}
			}


			while (ii >= 0)
			{
				str += vec[ii] + '0';
				--ii;
			}

			return str;
		}

	public:
		static string MultiplyAPI(const string & str1, const string & str2)
		{
			vector<char> nn1 = S2V(str1);
			vector<char> nn2 = S2V(str2);

			vector<char> rr = Multiply(nn1, nn2);

			return V2S(rr);
		}
	};


	class MultiplyStringsTest
	{
	public:
		static void Case0()
		{
			string str1{"123"};
			string str2{"456"};
			string rr = MultiplyStrings::MultiplyAPI(str1, str2);

			LOG_LINE_A("%s x %s = %s", str1.c_str(), str2.c_str(), rr.c_str());
		}
	};
}