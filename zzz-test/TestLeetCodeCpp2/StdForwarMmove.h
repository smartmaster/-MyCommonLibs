#pragma once

#include <string>
#include <iostream>
#include <typeinfo>
using namespace std;

namespace SmartLib
{
	class StdForwarMmove
	{
	public:
		template<typename TY>
		static void Case1(TY && str)
		{
			//string ref{ forward<string>(str) };
			string ref{forward<TY>(str)};
			cout << ref <<", " << str << endl;
		}

		static void Case2()
		{
			Case1<string&&>(string{"passed a r-value"});

			string lvalue{"passed a l-value"};
			Case1<string&>(lvalue);
			cout << lvalue << endl;
		}

		static void Case3()
		{
			{
				remove_reference_t<string>  sss{"hello"};
				cout << typeid(&sss).name() << ", " << sss << endl;
			}
			{
				remove_reference_t<string &>  sss{"hello"};
				cout << typeid(&sss).name() << ", " << sss << endl;
			}
			{
				remove_reference_t<string &&>  sss{"hello"};
				cout << typeid(&sss).name() << ", " << sss << endl;
			}

			{
				using VALUE_REF = string &&;
				remove_reference_t<VALUE_REF &>  sss{"hello"};
				cout << typeid(&sss).name() << ", " << sss << endl;
			}
			{
				using VALUE_REF = string && ;
				remove_reference_t<VALUE_REF &&>  sss{"hello"};
				cout << typeid(&sss).name() << ", " << sss << endl;
			}
			{
				using VAR_REF = string &;
				remove_reference_t<VAR_REF &>  sss{"hello"};
				cout << typeid(&sss).name() << ", " << sss << endl;
			}
			{
				//typedef string & VAR_REF;
				using VAR_REF = string &;
				remove_reference_t<VAR_REF &&>  sss{"hello"};
				cout << typeid(&sss).name() << ", " << sss << endl;
			}

			{
				string varNew = forward<string>(string{ "value string" });
				cout << varNew << endl;
			}

			try
			{
				//string varNew = forward<string&>(string{ "value string" }); //cannot compile
				//cout << varNew << endl;
			}
			catch(const std::exception & ex)
			{
				cout << ex.what() << endl;
			}

			{
				string varNew = forward<string&&>(string{ "value string" });
				cout << varNew << endl;
			}

			{
				string var{"var string"};
				string varNew = forward<string>(var);
				cout << var << endl;
				cout << varNew << endl;
			}

			{
				string var{ "var string" };
				string varNew = forward<string&>(var);
				cout << var << endl;
				cout << varNew << endl;
			}

			{
				string var{ "var string" };
				string varNew = forward<string&&>(var);
				cout << var << endl;
				cout << varNew << endl;
			}
			
		}
		

		static void Case4()
		{
			using const_string = const string;
			{
				remove_reference_t<const_string>  sss{ "hello" };
				cout << typeid(&sss).name() << ", " << sss << endl;
				//sss = "world";
			}
			{
				remove_reference_t<const_string &>  sss{ "hello" };
				cout << typeid(&sss).name() << ", " << sss << endl;
			}
			{
				remove_reference_t<const_string &&>  sss{ "hello" };
				cout << typeid(&sss).name() << ", " << sss << endl;
			}

			{
				using VALUE_REF = const_string && ;
				remove_reference_t<VALUE_REF &>  sss{ "hello" };
				cout << typeid(&sss).name() << ", " << sss << endl;
			}
			{
				using VALUE_REF = const_string && ;
				remove_reference_t<VALUE_REF &&>  sss{ "hello" };
				cout << typeid(&sss).name() << ", " << sss << endl;
			}
			{
				using VAR_REF = const_string & ;
				remove_reference_t<VAR_REF &>  sss{ "hello" };
				cout << typeid(&sss).name() << ", " << sss << endl;
			}
			{
				//typedef const_string & VAR_REF;
				using VAR_REF = const_string & ;
				remove_reference_t<VAR_REF &&>  sss{ "hello" };
				cout << typeid(&sss).name() << ", " << sss << endl;
			}
		}


		static int MyAdd(int aa, int bb)
		{
			return aa + bb;
		}

		static int MyMul(int aa, int bb)
		{
			return aa * bb;
		}

		class Alg
		{
			string _msg;
		public:

			Alg(const char * msg) :
				_msg(msg)
			{

			}
			int Calc(int aa, int bb)
			{
				cout << aa << ", " << bb << ", " << _msg << endl;
				return 0;
			}
		};

		static int Lambda(int aa, int bb)
		{
			Alg obj("lambda");
			auto ff = [&obj](int aa, int bb)
			{
				return obj.Calc(aa, bb);
			};

			return ff(aa, bb);
		}
		

		static void Case5()
		{
			using func_t = int(*)(int, int);

			

			func_t func[] = 
			{
				MyAdd,
				MyMul,
				Lambda
			};

			for (auto ff : func)
			{
				cout << ff(2, 3) << endl;
			}
		}
	};
}