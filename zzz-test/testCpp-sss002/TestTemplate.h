#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <string>
#include <typeinfo>
#include <iostream>

namespace TestCpp
{
	template<class T>
	class RemoveConst
	{
	public:
		using Type = typename T;
	};

	template<class T>
	class RemoveConst<const T>
	{
	public:
		using Type = typename T;
	};


	class RemoveConstTest
	{
	public:
		static void Case0()
		{
			{
				std::wstring str = L"hello";
				const std::wstring cstr = L"world";

				const std::type_info & ti = typeid(&str);
				std::cout << ti.name() << std::endl;
				const std::type_info & cti = typeid(&cstr);
				std::cout << cti.name() << std::endl;
				std::cout << std::endl;
			}
			
			{
				RemoveConst<std::wstring>::Type str = L"hello";
				RemoveConst<const std::wstring>::Type cstr = L"world";

				const std::type_info & ti = typeid(&str);
				std::cout << ti.name() << std::endl;
				const std::type_info & cti = typeid(&cstr);
				std::cout << cti.name() << std::endl;
				std::cout << std::endl;
			}


			{
				RemoveConst<std::wstring *>::Type str;
				RemoveConst<const std::wstring * const>::Type cstr;

				const std::type_info & ti = typeid(str);
				std::cout << ti.name() << std::endl;
				const std::type_info & cti = typeid(cstr);
				std::cout << cti.name() << std::endl;
				std::cout << std::endl;
			}

			
		}
	};



	
}