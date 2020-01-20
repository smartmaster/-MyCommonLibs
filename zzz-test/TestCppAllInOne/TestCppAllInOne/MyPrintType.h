#pragma once

#include <iostream>
#include <typeinfo>
#include <memory>
#include <string>
using namespace std;

#include "log.h"

namespace SmartLib
{
	template<typename T>
	class CPrintType
	{
	};


	template<typename T>
	class CPrintType<T&>
	{
	public:
		using Type = typename T;
		static void Print(T& val)
		{
			SML_LOG(TEXT("Type&:\t '%S&' is '"), typeid(Type).name());
			std::cout << val;
			SML_LOG_LINE(TEXT("'"));
		}
	};


	template<typename T>
	class CPrintType<const T&>
	{
	public:
		using Type = typename T;
		static void Print(const T& val)
		{
			SML_LOG(TEXT("const Type&:\t 'const %S&' is '"), typeid(Type).name());
			std::cout << val;
			SML_LOG_LINE(TEXT("'"));
		}
	};


	template<typename T>
	class CPrintType<T&&>
	{
	public:
		using Type = typename T;
		static void Print(T&& val)
		{
			SML_LOG(TEXT("Type&&:\t '%S&&' is '"), typeid(Type).name());
			std::cout << val;
			SML_LOG_LINE(TEXT("'"));
		}
	};


	template<typename T>
	class CPrintType<const T&&>
	{
	public:
		using Type = typename T;
		static void Print(const T&& val)
		{
			SML_LOG(TEXT("const Type&&:\t 'const %S&&' is '"), typeid(Type).name());
			std::cout << val;
			SML_LOG_LINE(TEXT("'"));
		}
	};

	
	class CPrintTypeAPI
	{
	public:
		template<typename T>
		static void Print(T && val)
		{
			CPrintType<T&&>::Print(std::forward<T>(val));
		}
	};


	class CPrintTypeAPITest
	{
	public:
		static void Case1()
		{
			std::string val{ "100" };
			CPrintTypeAPI::Print(val);
			CPrintTypeAPI::Print(std::string{"200"});

			const std::string cval = "300";
			CPrintTypeAPI::Print(cval);
			CPrintTypeAPI::Print((const std::string&&)(std::string{ "400" }));

		}

		static void Case0()
		{
			int val = 100;
			CPrintTypeAPI::Print(val);
			CPrintTypeAPI::Print(200);

			const int cval = 300;
			CPrintTypeAPI::Print(cval);
			CPrintTypeAPI::Print((const int&&)(400));

		}
	};
}