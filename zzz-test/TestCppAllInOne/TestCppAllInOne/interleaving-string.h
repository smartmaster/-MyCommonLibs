#pragma once
#include <string>
#include <unordered_map>

//https://leetcode.com/problems/interleaving-string/

namespace SmartLib
{
	using namespace  ::std;

	union MyUnion
	{
		long long ll;
		struct
		{
			short s1;
			short s2;
			short s3;
			short s4;
		} ss;

		MyUnion(int p1, int p2, int p3)
		{
			ss.s1 = p1;
			ss.s2 = p2;
			ss.s3 = p3;
			ss.s4 = 0;
		}
	};


	class MyDict
	{
	private:
		unordered_map<long long, bool> _mm;


	public:
		void Insert(int p1, int p2, int p3, bool bb)
		{
			MyUnion uu{ p1, p2, p3 };
			_mm.insert({ uu.ll, bb });
		}


		bool Find(int p1, int p2, int p3, bool& bb)
		{
			MyUnion uu{ p1, p2, p3 };
			auto iter = _mm.find(uu.ll);
			if (iter != _mm.end())
			{
				bb = iter->second;
				return true;
			}
			return false;
		}
	};

	class interleaving_string2
	{
	private:
		const string& str1;
		const string& str2;
		const string& str3;

		MyDict _dict;

	public:
		interleaving_string2(const string& s1, const string& s2, const string& s3) :
			str1{ s1 },
			str2{ s2 },
			str3{ s3 }
		{
		}


		bool isInterleave(int p1, int p2, int p3)
		{
			//////////////////////////////////////////////////////////////////////////
			if (p3 == -1)
			{
				return true;
			}

			if (p1 == -1)
			{
				return memcmp(str2.c_str(), str3.c_str(), p3 + 1) == 0;
			}


			if (p2 == -1)
			{
				return memcmp(str1.c_str(), str3.c_str(), p3 + 1) == 0;
			}


			//////////////////////////////////////////////////////////////////////////
			bool saved = false;
			if (_dict.Find(p1, p2, p3, saved))
			{
				return saved;
			}


			//////////////////////////////////////////////////////////////////////////
			bool result = false;

			char ch1 = str1[p1];
			char ch2 = str2[p2];
			char ch3 = str3[p3];
			if (ch1 == ch3 && ch2 == ch3)
			{
				result = isInterleave(p1 - 1, p2, p3 - 1);
				if (!result)
				{
					result = isInterleave(p1, p2 - 1, p3 - 1);
				}
			}
			else if (ch1 == ch3)
			{
				result = isInterleave(p1 - 1, p2, p3 - 1);
			}
			else if (ch2 == ch3)
			{
				result = isInterleave(p1, p2 - 1, p3 - 1);
			}
			else
			{
				result = false;
			}

			_dict.Insert(p1, p2, p3, result);

			return result;
		}
	};

	class Solution_isInterleave
	{
	public:

		static  bool isInterleave(const string& s1, const string& s2, const string& s3)
		{
			//////////////////////////////////////////////////////////////////////////
			if (s1.size() + s2.size() != s3.size())
			{
				return false;
			}

			if (s1.size() == 0)
			{
				return s2 == s3;
			}


			if (s2.size() == 0)
			{
				return s1 == s3;
			}


			//////////////////////////////////////////////////////////////////////////
			interleaving_string2 interstr{ s1, s2, s3 };
			return interstr.isInterleave(s1.size() - 1, s2.size() - 1, s3.size() - 1);
		}
	};


	class interleaving_string
	{
	public:


		static string Split(int pos, const string& str)
		{
			return str.substr(0, pos) + str.substr(pos + 1);
		}

		static  bool isInterleave(const string& s1, const string& s2, const string& s3) 
		{
			if (s1.size() + s2.size() != s3.size())
			{
				return false;
			}

			if (s1.size() == 0)
			{
				return s2 == s3;
			}


			if (s2.size() == 0)
			{
				return s1 == s3;
			}
			
			char ch = s1[0];
			string news1 = s1.substr(1);
			int pos = 0;
			for (;;)
			{
				pos = s3.find(ch, pos);
				if (string::npos == pos)
				{
					return false;
				}

				string news3 = Split(pos, s3);
				bool can = isInterleave(news1, s2, news3);
				if (can)
				{
					return true;
				}

				++pos;
			}


			return false;

		}
	};
}