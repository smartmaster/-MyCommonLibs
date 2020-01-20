#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <typeinfo>
#include <string>

#include "StringOf.h"
#include "log.h"


namespace SmartLib
{
	class SmlRVal
	{
	public:
		template<typename T>
		class MyTypeName
		{
		public:
			static const char * Name()
			{
				return typeid(T).name();
			}
		};


		template<typename T>
		class CRemoveRef
		{
		public:
			using Type = typename T;
		};

		template<typename T>
		class CRemoveRef<T&>
		{
		public:
			using Type = typename T;
		};

		template<typename T>
		class CRemoveRef<T&&>
		{
		public:
			using Type = typename T;
		};

		template<typename T>
		static T&& Fwd(typename CRemoveRef<T>::Type & var)
		{
			return static_cast<T&&>(var);
		}

		template<typename T>
		static T&& Fwd(typename CRemoveRef<T>::Type && var)
		{
			return static_cast<T&&>(var);
		}


		template<typename T>
		static typename CRemoveRef<T>::Type&& Move(T&& va)
		{
			return static_cast<typename CRemoveRef<T>::Type&&>(va);
		}
	};



	class SmlRValTest
	{
	public:
		//static void Add(int  val)
		//{
		//	SML_LOG_LINE(TEXT("call by value %d"), val);
		//}

		static void Add(int && val)
		{
			SML_LOG_LINE(TEXT("call by r-value %d"), val);
			//val = -val;
		}

		static void ChangeRVal(int && val)
		{
			val = -val;
		}


		static void Add(int & val)
		{
			SML_LOG_LINE(TEXT("call by ref %d"), val);
		}


		template<typename T>
		static void CallAdd(T && val)
		{
			Add(SmlRVal::Fwd<T>((T&&)val));
		}

		static int RetVal(int param)
		{
			return -param;
		}

		static void TestParams(std::wstring val, std::wstring & lref, std::wstring && rref)
		{
			SML_LOG_LINE(TEXT("val=%s, lref=%s, rref=%s"), val.c_str(), lref.c_str(), rref.c_str());

			std::wstring val_copy{std::forward<decltype(val)>(val)};
			std::wstring lref_copy{ std::forward<decltype(lref)>(lref) };
			std::wstring rref_copy{ std::forward<decltype(rref)>(rref) };


			SML_LOG_LINE(TEXT("%s = %S"), SML_WSTRING_OF(typeid(std::wstring).name()), typeid(std::wstring).name());
			SML_LOG_LINE(TEXT("%s = %S"), SML_WSTRING_OF(typeid(std::wstring&).name()), typeid(std::wstring&).name());
			SML_LOG_LINE(TEXT("%s = %S"), SML_WSTRING_OF(typeid(std::wstring&&).name()), typeid(std::wstring&&).name());

			SML_LOG_LINE(TEXT("%s = %S"), SML_WSTRING_OF(typeid(const std::wstring).name()), typeid(const std::wstring).name());
			SML_LOG_LINE(TEXT("%s = %S"), SML_WSTRING_OF(typeid(const std::wstring&).name()), typeid(const std::wstring&).name());
			SML_LOG_LINE(TEXT("%s = %S"), SML_WSTRING_OF(typeid(const std::wstring&&).name()), typeid(const std::wstring&&).name());


			SML_LOG_LINE(TEXT("%s = %S"), SML_WSTRING_OF(typeid(int).name()), typeid(int).name());
			SML_LOG_LINE(TEXT("%s = %S"), SML_WSTRING_OF(typeid(int&).name()), typeid(int&).name());
			SML_LOG_LINE(TEXT("%s = %S"), SML_WSTRING_OF(typeid(int&&).name()), typeid(int&&).name());

			SML_LOG_LINE(TEXT("%s = %S"), SML_WSTRING_OF(typeid(const int).name()), typeid(const int).name());
			SML_LOG_LINE(TEXT("%s = %S"), SML_WSTRING_OF(typeid(const int&).name()), typeid(const int&).name());
			SML_LOG_LINE(TEXT("%s = %S"), SML_WSTRING_OF(typeid(const int&&).name()), typeid(const int&&).name());


			std::wstring val_copy_copy{ std::forward<decltype(val_copy)>(val_copy) };

			SML_LOG_LINE(TEXT("val=%s, lref=%s, rref=%s"), val.c_str(), lref.c_str(), rref.c_str());
			SML_LOG_LINE(TEXT("val_copy=%s, lref_copy=%s, rref_copy=%s"), val_copy.c_str(), lref_copy.c_str(), rref_copy.c_str());
		}


		static void Case4()
		{
			std::wstring val{SML_WSTRING_OF(val)};
			std::wstring lref{ SML_WSTRING_OF(lref) };
			std::wstring rref{ SML_WSTRING_OF(rref) };
			SML_LOG_LINE(TEXT("val=%s, lref=%s, rref=%s"), val.c_str(), lref.c_str(), rref.c_str());
			TestParams(val, lref, std::move(rref));
			SML_LOG_LINE(TEXT("val=%s, lref=%s, rref=%s"), val.c_str(), lref.c_str(), rref.c_str());
		}

		static void Case3()
		{
			SML_LOG_LINE(TEXT("%s = %S"), SML_WSTRING_OF(typeid(std::wstring).name()), typeid(std::wstring).name());
			SML_LOG_LINE(TEXT("%s = %S"), SML_WSTRING_OF(typeid(std::wstring&).name()), typeid(std::wstring&).name());
			SML_LOG_LINE(TEXT("%s = %S"), SML_WSTRING_OF(typeid(std::wstring&&).name()), typeid(std::wstring&&).name());

			
			SML_LOG_LINE(TEXT("%s = %S"), SML_WSTRING_OF(SmlRVal::MyTypeName<int>::Name()), SmlRVal::MyTypeName<int>::Name());
			SML_LOG_LINE(TEXT("%s = %S"), SML_WSTRING_OF(SmlRVal::MyTypeName<int&>::Name()), SmlRVal::MyTypeName<int&>::Name());
			SML_LOG_LINE(TEXT("%s = %S"), SML_WSTRING_OF(SmlRVal::MyTypeName<int&&>::Name()), SmlRVal::MyTypeName<int&&>::Name());
		}

		static void Case2()
		{
			SmlRVal::CRemoveRef<int>::Type val0 = 123;
			SmlRVal::CRemoveRef<int&>::Type val1 = 123;
			SmlRVal::CRemoveRef<int&&>::Type val2 = 123;
		}

		static void Case1()
		{
			SmlRVal::Fwd<decltype(100)>(100);
			int val = 200;
			SmlRVal::Fwd<decltype(val)>(val);
		}

		static void Case0()
		{
			
			CallAdd(100);
			CallAdd(RetVal(-200));
			int val = 300;
			CallAdd(val);
			val = 400;
			CallAdd((int)val);
			val = 500;
			CallAdd((int&)val);
			val = 600;
			CallAdd((int&&)val);

			val = 700;
			CallAdd(SmlRVal::Move(val));

			int rv = 123;
			SML_LOG_LINE(TEXT("before %d"), rv);
			ChangeRVal(SmlRVal::Move(rv));
			SML_LOG_LINE(TEXT("after %d"), rv);
		}
	};

}