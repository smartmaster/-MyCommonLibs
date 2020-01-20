#pragma once

#include <string>
#include <vector>

namespace SmartLib
{
	using namespace ::std;

	//https://leetcode.com/problems/longest-common-subsequence/
	class longest_common_subsequence
	{

	public:

		
		static int MyMax(int aa, int bb)
		{
			return aa > bb ? aa : bb;
		}


		static int longestCommonSubsequence(const string& text1, const string& text2)
		{
			vector<vector<int>> vv;
			vv.resize(text1.size() + 1);
			for (auto& vec : vv)
			{
				vec.resize(text2.size() + 1);
			}

			for (int count1 = 1; count1 <= text1.size(); ++ count1)
			{
				for (int count2 = 1; count2 <= text2.size(); ++count2)
				{
					if (text1[count1 - 1] == text2[count2 - 1])
					{
						vv[count1][count2] = vv[count1 - 1][count2 -1] + 1;
					}
					else
					{
						vv[count1][count2] = MyMax(vv[count1 - 1][count2], vv[count1][count2 - 1]);
					}
				}
			}


			return vv[text1.size()][text2.size()];
		}
	};
}