#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include <memory>
#include <iostream>
#include <string>

#include "log.h"

namespace SmartLib
{
	class TestCppType
	{
	public:
		template<typename T>
		static void foo(T&& param)
		{
			std::remove_reference_t<T> var{std::forward<T>(param)};
			std::cout << var << std::endl;
		}

		//template<typename T1, typename T2>
		//static T1 Conv(T2 data)
		//{
		//	return (T1)(T2)(data);
		//}

		static void Case0()
		{
			{
				std::string str = "111";
				(std::string)(std::string)(str);

				str = "222";
				(std::string)(std::string&)(str);

				str = "333";
				(std::string)(std::string&&)(str);

				str = "444";
				(std::string&)(std::string)(str);

				str = "555";
				(std::string&)(std::string&)(str);

				str = "666";
				(std::string&)(std::string&&)(str);

				str = "777";
				(std::string&&)(std::string)(str);

				str = "888";
				(std::string&&)(std::string&)(str);

				str = "999";
				(std::string&&)(std::string&&)(str);

			}


			
			{
				std::string str = "111";
				(std::string)(std::string)(std::string&&)(str);

				str = "222";
				(std::string)(std::string&)(std::string&&)(str);

				str = "333";
				(std::string)(std::string&&)(std::string&&)(str);

				str = "444";
				(std::string&)(std::string)(std::string&&)(str);

				str = "555";
				(std::string&)(std::string&)(std::string&&)(str);

				str = "666";
				(std::string&)(std::string&&)(std::string&&)(str);

				str = "777";
				(std::string&&)(std::string)(std::string&&)(str);

				str = "888";
				(std::string&&)(std::string&)(std::string&&)(str);

				str = "999";
				(std::string&&)(std::string&&)(std::string&&)(str);

			}


			{
				std::string str = "111";
				(std::string)(std::string)(std::string&)(str);

				str = "222";
				(std::string)(std::string&)(std::string&)(str);

				str = "333";
				(std::string)(std::string&&)(std::string&)(str);

				str = "444";
				 (std::string&)(std::string)(std::string&)(str);

				str = "555";
				(std::string&)(std::string&)(std::string&)(str);

				str = "666";
				(std::string&)(std::string&&)(std::string&)(str);

				str = "777";
				(std::string&&)(std::string)(std::string&)(str);

				str = "888";
				(std::string&&)(std::string&)(std::string&)(str);

				str = "999";
				(std::string&&)(std::string&&)(std::string&)(str);

			}


			{
				std::string str = "111";
				auto data1 = (std::string)(std::string)(str);

				str = "222";
				auto data2 = (std::string)(std::string&)(str);

				str = "333";
				auto data3 = (std::string)(std::string&&)(str);

				str = "444";
				auto data4 = (std::string&)(std::string)(str);

				str = "555";
				auto data5 = (std::string&)(std::string&)(str);

				str = "666";
				auto data6 = (std::string&)(std::string&&)(str);

				str = "777";
				auto data7 = (std::string&&)(std::string)(str);

				str = "888";
				auto data8 = (std::string&&)(std::string&)(str);

				str = "999";
				auto data9 = (std::string&&)(std::string&&)(str);

			}

			{
				
				auto data1 = (std::string)(std::string)(std::string{"111"});

				auto data2 = (std::string)(std::string&)(std::string{ "222" });

				auto data3 = (std::string)(std::string&&)(std::string{ "333" });

				auto data4 = (std::string&)(std::string)(std::string{ "444" });

				auto data5 = (std::string&)(std::string&)(std::string{ "555" });

				auto data6 = (std::string&)(std::string&&)(std::string{ "666" });

				auto data7 = (std::string&&)(std::string)(std::string{ "777" });

				auto data8 = (std::string&&)(std::string&)(std::string{ "888" });

				auto data9 = (std::string&&)(std::string&&)(std::string{ "999" });

			}


			{
				std::string str = "111";
				auto data1 = (std::string)(std::string)(std::string&&)(str);

				str = "222";
				auto data2 = (std::string)(std::string&)(std::string&&)(str);

				str = "333";
				auto data3 = (std::string)(std::string&&)(std::string&&)(str);

				str = "444";
				auto data4 = (std::string&)(std::string)(std::string&&)(str);

				str = "555";
				auto data5 = (std::string&)(std::string&)(std::string&&)(str);

				str = "666";
				auto data6 = (std::string&)(std::string&&)(std::string&&)(str);

				str = "777";
				auto data7 = (std::string&&)(std::string)(std::string&&)(str);

				str = "888";
				auto data8 = (std::string&&)(std::string&)(std::string&&)(str);

				str = "999";
				auto data9 = (std::string&&)(std::string&&)(std::string&&)(str);

			}


			{
				std::string str = "111";
				auto data1 = (std::string)(std::string)(std::string&)(str);

				str = "222";
				auto data2 = (std::string)(std::string&)(std::string&)(str);

				str = "333";
				auto data3 = (std::string)(std::string&&)(std::string&)(str);

				str = "444";
				auto data4 = (std::string&)(std::string)(std::string&)(str);

				str = "555";
				auto data5 = (std::string&)(std::string&)(std::string&)(str);

				str = "666";
				auto data6 = (std::string&)(std::string&&)(std::string&)(str);

				str = "777";
				auto data7 = (std::string&&)(std::string)(std::string&)(str);

				str = "888";
				auto data8 = (std::string&&)(std::string&)(std::string&)(str);

				str = "999";
				auto data9 = (std::string&&)(std::string&&)(std::string&)(str);

			}

			{
				std::string str = std::forward<std::string>(std::string{"hello world"});
				std::string str1 = std::forward<std::string&&>(std::string{ "hello world" });
			}
			{
				std::string named{ "name" };
				using T = std::string;
				foo<std::string>(std::forward<std::string>(named));
			}
			{
				std::string named{ "name" };
				using T = std::string&;
				foo<std::string&>(std::forward<std::string&>(named));
			}
			{
				std::string named{ "name" };
				using T = std::string&&;
				foo<std::string&&>(std::forward<std::string&&>(named));
			}

			{
				foo<std::string>(std::string{"unnamed"});
				//foo<std::string&>(std::string{ "unnamed" });
				foo<std::string&&>(std::string{ "unnamed" });
			}
		}

		class MyTestObj
		{
		private:
			static int& Count()
			{
				static int count = 0;
				return count;
			}

			int _index = 0;

		public:
			MyTestObj() :
				_index(++Count())
			{
				SML_LOG_LINE(TEXT("ctor %d"), _index);
			}


			MyTestObj(MyTestObj& obj) :
				_index(++Count())
			{
				SML_LOG_LINE(TEXT("lval-ref-copy-ctor %d<<%d"), _index, obj._index);
			}

			MyTestObj(MyTestObj&& obj) :
				_index(++Count())
			{
				SML_LOG_LINE(TEXT("rval-ref-copy-ctor %d<<%d"), _index, obj._index);
				obj._index = - obj._index;
			}


			const MyTestObj& operator=(MyTestObj& obj)
			{
				SML_LOG_LINE(TEXT("lval-ref-assignment %d<<%d"), _index, obj._index);
				return *this;
			}

			const MyTestObj& operator=(MyTestObj&& obj)
			{
				SML_LOG_LINE(TEXT("rval-ref-assignment %d<<%d"), _index, obj._index);
				obj._index = -1;
				return *this;
			}
		};


		static void Case1()
		{

			{
				MyTestObj obj;
				(MyTestObj)(MyTestObj)(MyTestObj&)(obj);
				(MyTestObj)(MyTestObj&)(MyTestObj&)(obj);
				(MyTestObj)(MyTestObj&&)(MyTestObj&)(obj);

				(MyTestObj&)(MyTestObj)(MyTestObj&)(obj);
				(MyTestObj&)(MyTestObj&)(MyTestObj&)(obj);
				(MyTestObj&)(MyTestObj&&)(MyTestObj&)(obj);

				(MyTestObj&&)(MyTestObj)(MyTestObj&)(obj);
				(MyTestObj&&)(MyTestObj&)(MyTestObj&)(obj);
				(MyTestObj&&)(MyTestObj&&)(MyTestObj&)(obj);
			}


			{
				MyTestObj obj;
				(MyTestObj)(MyTestObj)(MyTestObj&&)(obj);
				(MyTestObj)(MyTestObj&)(MyTestObj&&)(obj);
				(MyTestObj)(MyTestObj&&)(MyTestObj&&)(obj);

				(MyTestObj&)(MyTestObj)(MyTestObj&&)(obj);
				(MyTestObj&)(MyTestObj&)(MyTestObj&&)(obj);
				(MyTestObj&)(MyTestObj&&)(MyTestObj&&)(obj);

				(MyTestObj&&)(MyTestObj)(MyTestObj&&)(obj);
				(MyTestObj&&)(MyTestObj&)(MyTestObj&&)(obj);
				(MyTestObj&&)(MyTestObj&&)(MyTestObj&&)(obj);
			}

			{
				MyTestObj obj;
				MyTestObj ob1 = (MyTestObj)(MyTestObj)(MyTestObj&)(obj);
				MyTestObj ob2 = (MyTestObj)(MyTestObj&)(MyTestObj&)(obj);
				MyTestObj ob3 = (MyTestObj)(MyTestObj&&)(MyTestObj&)(obj);

				MyTestObj ob4 = (MyTestObj&)(MyTestObj)(MyTestObj&)(obj);
				MyTestObj ob5 = (MyTestObj&)(MyTestObj&)(MyTestObj&)(obj);
				MyTestObj ob6 = (MyTestObj&)(MyTestObj&&)(MyTestObj&)(obj);

				MyTestObj ob7 = (MyTestObj&&)(MyTestObj)(MyTestObj&)(obj);
				MyTestObj ob8 = (MyTestObj&&)(MyTestObj&)(MyTestObj&)(obj);
				MyTestObj ob9 = (MyTestObj&&)(MyTestObj&&)(MyTestObj&)(obj);
			}


			{
				MyTestObj obj;
				MyTestObj ob1 = (MyTestObj)(MyTestObj)(MyTestObj&&)(obj);
				MyTestObj ob2 = (MyTestObj)(MyTestObj&)(MyTestObj&&)(obj);
				MyTestObj ob3 = (MyTestObj)(MyTestObj&&)(MyTestObj&&)(obj);

				MyTestObj ob4 = (MyTestObj&)(MyTestObj)(MyTestObj&&)(obj);
				MyTestObj ob5 = (MyTestObj&)(MyTestObj&)(MyTestObj&&)(obj);
				MyTestObj ob6 = (MyTestObj&)(MyTestObj&&)(MyTestObj&&)(obj);

				MyTestObj ob7 = (MyTestObj&&)(MyTestObj)(MyTestObj&&)(obj);
				MyTestObj ob8 = (MyTestObj&&)(MyTestObj&)(MyTestObj&&)(obj);
				MyTestObj ob9 = (MyTestObj&&)(MyTestObj&&)(MyTestObj&&)(obj);
			}

			{
				MyTestObj obj;

				MyTestObj ob1;
				ob1 = (MyTestObj)(MyTestObj)(MyTestObj&)(obj);
				MyTestObj ob2;
				ob2 = (MyTestObj)(MyTestObj&)(MyTestObj&)(obj);
				MyTestObj ob3;
				ob3 = (MyTestObj)(MyTestObj&&)(MyTestObj&)(obj);
				MyTestObj ob4;
				ob4 = (MyTestObj&)(MyTestObj)(MyTestObj&)(obj);
				MyTestObj ob5;
				ob5 = (MyTestObj&)(MyTestObj&)(MyTestObj&)(obj);
				MyTestObj ob6;
				ob6 = (MyTestObj&)(MyTestObj&&)(MyTestObj&)(obj);
				MyTestObj ob7;
				ob7 = (MyTestObj&&)(MyTestObj)(MyTestObj&)(obj);
				MyTestObj ob8;
				ob8 = (MyTestObj&&)(MyTestObj&)(MyTestObj&)(obj);
				MyTestObj ob9;
				ob9 = (MyTestObj&&)(MyTestObj&&)(MyTestObj&)(obj);
			}


			{
				MyTestObj obj;

				MyTestObj ob1;
				ob1 = (MyTestObj)(MyTestObj)(MyTestObj&&)(obj);
				MyTestObj ob2;
				ob2 = (MyTestObj)(MyTestObj&)(MyTestObj&&)(obj);
				MyTestObj ob3;
				ob3 = (MyTestObj)(MyTestObj&&)(MyTestObj&&)(obj);
				MyTestObj ob4;
				ob4 = (MyTestObj&)(MyTestObj)(MyTestObj&&)(obj);
				MyTestObj ob5;
				ob5 = (MyTestObj&)(MyTestObj&)(MyTestObj&&)(obj);
				MyTestObj ob6;
				ob6 = (MyTestObj&)(MyTestObj&&)(MyTestObj&&)(obj);
				MyTestObj ob7;
				ob7 = (MyTestObj&&)(MyTestObj)(MyTestObj&&)(obj);
				MyTestObj ob8;
				ob8 = (MyTestObj&&)(MyTestObj&)(MyTestObj&&)(obj);
				MyTestObj ob9;
				ob9 = (MyTestObj&&)(MyTestObj&&)(MyTestObj&&)(obj);
			}
		}


		static void Case2()
		{

			{
				MyTestObj var;
				MyTestObj obj = var;
				MyTestObj objNew{ obj };
			}

			{
				MyTestObj var;
				MyTestObj& obj = var;
				MyTestObj objNew{ obj };
			} 
			
			//{
			//	MyTestObj var;
			//	MyTestObj&& obj = var;
			//	MyTestObj objNew{ obj };
			//}

			{
				MyTestObj obj = MyTestObj{};
				MyTestObj objNew{ obj };
			}

			//{
			//	MyTestObj& obj = MyTestObj{};
			//	MyTestObj objNew{ obj };
			//}

			{
				MyTestObj&& obj = MyTestObj{};
				MyTestObj objNew{ obj };
			}
		}

		class TestCallRealArgs
		{
		public:
			static void f0(std::string str, bool change)
			{
				using namespace std;
				cout << str << endl;
				str = "HELLO";
			}

			static void f1(std::string& str, bool change)
			{
				using namespace std;
				cout << str << endl;
				str = "HELLO";
			}


			static void f2(std::string&& str, bool change)
			{
				using namespace std;
				cout << str << endl;
				str = "HELLO";
			}

			static void Case3()
			{
				using namespace std;
				{
					string str = "fox";
					f0(str, false);
					cout << str << endl << endl;
				}
				{
					string str = "fox";
					f0(str, false);
					cout << str << endl << endl;
				}


				{
					string str = "fox";
					f1(str, false);
					cout << str << endl << endl;
				}
				{
					string str = "fox";
					f1(str, false);
					cout << str << endl << endl;
				}

				{
					string str = "fox";
					f2(move(str), false);
					cout << str << endl << endl;
				}
				{
					string str = "fox";
					f2(move(str), false);
					cout << str << endl << endl;
				}

			}
		};



	};

} //namespace SmartLib