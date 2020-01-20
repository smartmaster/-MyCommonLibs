#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include <vector>
#include <iostream>


namespace TestCpp
{
	class TestRVRef
	{
	public:
		static void Case1()
		{
			std::vector<std::wstring> vec1{ L"111", L"222", L"333", L"444" };
			std::vector<std::wstring> vec2{ L"AAA", L"BBB", L"CCC"};
		

			std::vector<std::wstring> vec3(std::move(vec1));


			auto print = [](const std::wstring & s)
			{
				std::wcout << s.c_str() << L", ";
			};
			
		}
	};
}