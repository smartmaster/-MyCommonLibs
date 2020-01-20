#pragma once

#include <typeinfo>
#include <iostream>
using namespace std;

namespace SmartLib
{
	template<typename T>
	class sml_remove_ref
	{
	public:
		sml_remove_ref()
		{
			cout << "type::" << typeid(T).name() << endl;
		}
		using type = T;
	};

	template<typename T>
	class sml_remove_ref<T&>
	{
	public:
		sml_remove_ref()
		{
			cout << "var::" << typeid(T).name() << endl;
		}
		using type = T;
	};

	template<typename T>
	class sml_remove_ref<T&&>
	{
	public:
		sml_remove_ref()
		{
			cout << "value::" << typeid(T).name() << endl;;
		}
		using type = T;
	};


	template<typename T>
	using sml_remove_ref_t = typename sml_remove_ref<T>::type;

	template<typename T>
	T&& sml_forward(sml_remove_ref_t<T> & var)
	{
		cout << "forward var" << endl;
		return static_cast<T&&>(var);
	}

	template<typename T>
	T&& sml_forward(sml_remove_ref_t<T> && value)
	{
		cout << "forward value" << endl;
		return static_cast<T&&>(value);
	}


	//template<class T>
	//T&& sml_perfect_forward(T vv)
	//{
	//	return sml_forward<T&&>(vv);
	//}


	class TestRVRefCase
	{
	public:

		static void Case3()
		{
			//{
			//	string var{"i am var"};
			//	string str{ sml_perfect_forward(var) };
			//}

			//{
			//	string && value{ "i am var" };
			//	string str{ sml_perfect_forward(value) };
			//}

			//{
			//	string str{ sml_perfect_forward(string{ "i am var" }) };
			//}
		}

		static void Case2()
		{
			{
				string str1{ sml_forward<string>(string{"i am value"}) };
			}

			{
				string str1{ sml_forward<string&&>(string{ "i am value" }) };
			}


		}

		static void Case1()
		{
			{
				string var{ "i am var" };
				string str1{ sml_forward<string>(var) };
			}

			{
				string var{ "i am var" };
				string str1{ sml_forward<string&>(var) };
			}

			{
				string var{ "i am var" };
				string str1{ sml_forward<string&&>(var) };
			}

		}


		static void Case0()
		{

			sml_remove_ref<string> rr1;
			sml_remove_ref<string&> rr2;
			sml_remove_ref<string&&> rr3;

			sml_remove_ref_t<string> str1{ "1" };
			sml_remove_ref_t<string&> str2{ "2" };
			sml_remove_ref_t<string&&> str3{ "3" };
		}
	};
};