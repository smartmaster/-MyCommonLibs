#pragma once

#include <algorithm>
#include <string>
#include <locale>
#include <iostream>

namespace TestCpp
{
	class TestLocale
	{
	public:

		static void Loc()
		{
			std::locale loc("");

			auto func = [](const wchar_t ch)
			{
				return std::toupper(ch, std::locale::empty());
			};

			//auto func = toupper;

			std::wstring chs = L"AbÄãºÃcd";
			std::transform(chs.begin(), chs.end(), chs.begin(), func);


			std::wcout << chs << std::endl;
		}
	};
}