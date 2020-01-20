#pragma once

#include <string>
#include <vector>

//https://leetcode.com/problems/wildcard-matching/

namespace SmartLib
{
	using namespace  ::std;


	class wildcard_matching
	{
		const string& _str1; /***INCOMMING***/
		const string& _str2; /***INCOMMING***/

		vector<vector<char>> _vv; /***INTERNAL***/
		long _resuseCount{ 0 }; /***INTERNAL***/

	private:
		const char T{ 'T' };
		const char F{ 'F' };

	private:
		static bool IsSingleChar(char ch)
		{
			return  ch != '*';
		}

		static bool IsStarChar(char ch)
		{
			return ch == '*';
		}

		static bool SingleCharMatch(char ch1, char ch2)
		{
			return ch1 == ch2 || ch1 == '?' || ch2 == '?';
		}




		bool WildCardMatch(long count1, long count2)
		{

			char saved = _vv[count1][count2];
			if (saved)
			{
				++_resuseCount;
				return saved == T ? true : false;
			}


			if (count1 == 0 && count2 == 0)
			{
				_vv[0][0] = T;
				return true;
			}


			if (count1 == 0 && count2)
			{
				bool rrtemp = true;;
				for (long ii = 0; ii < count2; ++ ii)
				{
					if (IsSingleChar(_str2[ii]))
					{
						_vv[0][ii+1] = F;
						rrtemp = false;
						break;
					}
					else
					{
						_vv[0][ii+1] = T;
					}
				}
				_vv[0][count2] = rrtemp ? T : F;
				return rrtemp;
			}


			if (count1 && count2 == 0)
			{
				bool rrtemp = true;;
				for (long ii = 0; ii < count1; ++ii)
				{
					if (IsSingleChar(_str1[ii]))
					{
						_vv[ii+1][0] = F;
						rrtemp = false;
						break;
					}
					else
					{
						_vv[ii+1][0] = T;
					}
				}
				_vv[count1][0] = rrtemp ? T : F;
				return rrtemp;
			}


			char ch1 = _str1[count1 - 1];
			char ch2 = _str2[count2 - 1];


			bool rr = false;
			if (IsSingleChar(ch1) && IsSingleChar(ch2))
			{
				if (SingleCharMatch(ch1, ch2))
				{
					rr = WildCardMatch(count1 - 1, count2 - 1);
				}
				else
				{
					rr = false;
				}
			}
			else if (IsStarChar(ch1))
			{
				for (long len2 = count2; len2 >= 0; --len2)
				{
					if (WildCardMatch(count1 - 1, len2))
					{
						rr = true;
						break;
					}
				}
			}
			else if (IsStarChar(ch2))
			{
				for (long len1 = count1; len1 >= 0; --len1)
				{
					if (WildCardMatch(len1, count2 - 1))
					{
						rr = true;
						break;
					}
				}
			}

			_vv[count1][count2] = (rr ? T : F);

			return rr;
		}

	public:
		wildcard_matching(const string& str1, const string& str2) :
			_str1{ str1 },
			_str2{ str2 }
		{
			_vv.resize(str1.size() + 1);
			for (auto& v : _vv)
			{
				v.resize(str2.size() + 1);
			}
		}

		bool Run()
		{
			long count1 = _str1.size();
			long count2 = _str2.size();
			return WildCardMatch(count1, count2);
		}

	};


	class Solution_wildcard_matching
	{
	public:
		static bool isMatch(const string& s, const string& p) 
		{
			wildcard_matching wm{s, p};
			bool rr = wm.Run();
			return rr;
		}
	};
}