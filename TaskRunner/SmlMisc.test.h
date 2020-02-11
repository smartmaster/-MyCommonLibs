#pragma once

#include <string>
#include <memory>
#include <functional>


#include "SmlRefPtr.h"
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
		constexpr /*inline*/ static string* _ptr{ (string*)(void*)(-1) };
	public:
		static void Case4()
		{
			auto func = Case3();
			long res = func(100, 200);
			cout << res << endl;
		}

		static function<long(long, long)> Case3()
		{
			struct BigObj
			{
				string str1;
				string str2;
				string str3;
				string str4;
				string str5;
				string str6;
				string str7;

				BigObj(long start)
				{
					str1 = to_string(start++);
					str2 = to_string(start++);
					str3 = to_string(start++);
					str4 = to_string(start++);
					str5 = to_string(start++);
					str6 = to_string(start++);
					str7 = to_string(start++);
				}

				long operator()(long ll1, long ll2) /*const*/
				{
					long res =
						stol(str1) +
						stol(str2) +
						stol(str3) +
						stol(str4) +
						stol(str5) +
						stol(str6) +
						stol(str7) +
						+ ll1
						+ ll2;

					return res;
				}
			};

			RefPtr<BigObj>  bo = RefPtr<BigObj>::Make(1000);

			function<long(long, long)> func = [bo](long val1, long val2) mutable
			{
				return (*bo)(val1, val2);
			};

			long res = func(100, 200);
			cout << res << endl;


			return func;
		}

		static void Case2()
		{
			string* ptr{ (string*)(void*)(-1) };
			cout << ptr << endl;

			ptr = _ptr;
			cout << ptr << endl;
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