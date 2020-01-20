#pragma once
#include "interleaving-string.h"

#include <iostream>

namespace SmartLib
{
	using namespace  ::std;

	class interleaving_string_test
	{
	public:
		static void Case0()
		{
			//string s1 = "ab";
			//string s2 = "bc";
			//string s3 = "abcb";


			string s1 = "aabcc";
			string s2 = "dbbca";
			string s3 = "aadbbcbcac";

			{
				bool ok = SmartLib::Solution_isInterleave::isInterleave(s1, s2, s3);
				cout << ok << endl;
			}
			

			{
				bool ok = SmartLib::interleaving_string::isInterleave(s1, s2, s3);
				cout << ok << endl;
			}
			
		}
	};

}