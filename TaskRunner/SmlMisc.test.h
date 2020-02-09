#pragma once

#include <string>
#include <memory>
#include <functional>


#include "SmlMisc.h"


namespace SmartLib
{
	using namespace ::std;

	class MiscTest2
	{
	private:
		constexpr static string* const _ptr { (string*)(void*)(16) } ;
	public:
		static void Print()
		{
			string* ptr = _ptr;
			cout << ptr << endl;
		}
	};
	class MiscTest
	{
	public:
		static void Case2()
		{
			string* const ptr = (string*)(void*)(1);

			cout << (long long)(void*)(ptr) << endl;
		}

		static long foo(long ll1, long ll2)
		{
			return ll1 + ll2;
		}

		static void Case1()
		{
			function<long(long, long)> func = foo;
			long res = func(100, 200);
			cout << res << endl;
		}


		static void Case0()
		{
			{
				string str = "hello";
				SmartLib::Misc::Assign(str);
			}
			
			{
				string str = "hello";
				SmartLib::Misc::Assign(move(str));
			}
		}
	};
}