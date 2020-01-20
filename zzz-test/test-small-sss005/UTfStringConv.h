#pragma once

#include <string>
#include <codecvt>
#include <fstream>

using namespace std;

namespace SmartLib
{
	class UTFStringConv
	{
	public:
		static string UTF16To8(const wstring & utf16)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conv;
			return conv.to_bytes(utf16);
		}


		static wstring UTF8To16(const string & utf8)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conv;
			return conv.from_bytes(utf8);
		}
	};


	class UTFStringConvTest
	{
	public:
		static void Case0()
		{
			wstring utf16 = L"aaa我们是大好人zzz";
			string utf8 = UTFStringConv::UTF16To8(utf16);
			wstring utf16c = UTFStringConv::UTF8To16(utf8);
			ofstream of("utf8.txt");
			of.write(utf8.c_str(), utf8.size());
		}

	};
}