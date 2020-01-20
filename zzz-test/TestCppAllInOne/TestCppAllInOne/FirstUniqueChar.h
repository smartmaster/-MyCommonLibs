#pragma once
#include <string>
#include <map>
#include <vector>

namespace SmartLib
{
	using namespace std;

	class FindUniqueFirsrtChar
	{
	public:
		static int Find(const string& str)
		{
			map<char, int> mm;
			for (int ii = 0; ii < str.size(); ++ ii)
			{
				auto iter = mm.find(str[ii]);
				if (iter == mm.end())
				{
					mm.insert({str[ii], ii});
				}
				else
				{
					mm[str[ii]] = str.size();
				}
			}

			if (mm.size() == 0)
			{
				return -1;
			}

			int result = str.size();
			for (auto iter = mm.begin(); iter != mm.end(); ++iter)
			{
				if (result > iter->second)
				{
					result = iter->second;
				}
			}

			return result == str.size()? -1 : result;
		}


		static int Char2Index(char ch)
		{
			if (ch >= 'a' && ch <= 'z')
			{
				return ch - 'a';
			}

			if (ch >= 'A' && ch <= 'Z')
			{
				return ch - 'A' + 26;
			}

			if (ch >= '0' && ch <= '9')
			{
				return ch - '0' + 52;
			}

		}


		static vector<int> vecmap;

		static int Find2(const string& str)
		{
			int vecmap[256];// = { 0 };
			memset(vecmap, 0, sizeof(vecmap));
			
			
			int size = str.size();
			for (auto ch : str)
			{
				++vecmap[ch];
			}

			int index = -1;
			for (auto ch: str)
			{
				++index;
				if (1 == vecmap[ch])
				{
					return index;
				}
			}

			return -1;
		}
	};
}