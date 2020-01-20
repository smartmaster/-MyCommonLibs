#pragma once


#include <string>
#include <utility>
#include <iostream>
using namespace std;

namespace TestCpp
{
	class TestFwd
	{
	public:
		static void Case0()
		{
			auto strv1 = std::forward<string&&>(string{ "value1" });
			auto strv2 = std::forward<string>(string{ "value2" });

			auto var2 = string{ "var2" };
			auto str2 = std::forward<string&&>(var2);

			auto var3 = string{ "var3" };
			auto str3 = std::forward<string&>(var3);

			auto var4 = string{ "var4" };
			auto str4 = std::forward<string>(var4);
		}


		static void Case1()
		{

			{
				string var = std::forward<string>(string{"hello"});
				std::cout << "after forwarding: " << var << ", " << std::endl << std::endl;;
			}


			{
				//string var = std::forward<string&>(string{ "hello" });
				//std::cout << "after forwarding: " << var << ", " << std::endl << std::endl;;
			}

			{
				string var = std::forward<string&&>(string{ "hello" });
				std::cout << "after forwarding: " << var << ", " << std::endl << std::endl;;
			}


			{
				string&& rr{"hello"};
				std::cout<< "before forwarding: " << rr << std::endl;
				string var = std::forward<string>(rr);
				std::cout << "after forwarding: " << var << ", " << rr << std::endl << std::endl;;
			}


			{
				string&& rr{ "hello" };
				std::cout << "before forwarding: " << rr << std::endl;
				string var = std::forward<string&>(rr);
				std::cout << "after forwarding: " << var << ", " << rr << std::endl << std::endl;;
			}

			{
				string&& rr{ "hello" };
				std::cout << "before forwarding: " << rr << std::endl;
				string var = std::forward<string&&>(rr);
				std::cout << "after forwarding: " << var << ", " << rr << std::endl << std::endl;;
			}


			{
				string rr{ "hello" };
				std::cout << "before forwarding: " << rr << std::endl;
				string var = std::forward<string>(rr);
				std::cout << "after forwarding: " << var << ", " << rr << std::endl << std::endl;;
			}


			{
				string rr{ "hello" };
				std::cout << "before forwarding: " << rr << std::endl;
				string var = std::forward<string&>(rr);
				std::cout << "after forwarding: " << var << ", " << rr << std::endl << std::endl;;
			}

			{
				string rr{ "hello" };
				std::cout << "before forwarding: " << rr << std::endl;
				string var = std::forward<string&&>(rr);
				std::cout << "after forwarding: " << var << ", " << rr << std::endl << std::endl;;
			}
		}
	};
}

#if FALSE

template<class _Ty>
_NODISCARD constexpr _Ty&& forward(remove_reference_t<_Ty>& _Arg) _NOEXCEPT
{	// forward an lvalue as either an lvalue or an rvalue
	return (static_cast<_Ty&&>(_Arg));
}


//////////////////////////////////////////////////////////////////////////
_Ty = string&&


_NODISCARD constexpr string&& forward(string& _Arg) _NOEXCEPT
{	// forward an lvalue as either an lvalue or an rvalue
	return (static_cast<string&&>(_Arg));
}

//////////////////////////////////////////////////////////////////////////
_Ty = string&

_NODISCARD constexpr string& forward(string& _Arg) _NOEXCEPT
{	// forward an lvalue as either an lvalue or an rvalue
	return (static_cast<string&>(_Arg));
}

///////////////////////////////////////////////////////////
_Ty = string


constexpr string&& forward(string& _Arg) _NOEXCEPT
{	// forward an lvalue as either an lvalue or an rvalue
	return (static_cast<string&&>(_Arg));
}

//////////////////////////////////////////////////////////////////////////

template<class _Ty>
_NODISCARD constexpr _Ty&& forward(remove_reference_t<_Ty>&& _Arg) _NOEXCEPT
{	// forward an rvalue as an rvalue
	static_assert(!is_lvalue_reference_v<_Ty>, "bad forward call");
	return (static_cast<_Ty&&>(_Arg));
}


//////////////////////////////////////////////////////////////////////////
_Ty = string &&


_NODISCARD constexpr string && forward(string && _Arg) _NOEXCEPT
{	// forward an rvalue as an rvalue
	static_assert(!is_lvalue_reference_v<_Ty>, "bad forward call");
	return (static_cast<_string &&>(_Arg));
}



_Ty = string &
_NODISCARD constexpr string &  forward(string&& _Arg) _NOEXCEPT
{	// forward an rvalue as an rvalue
	static_assert(!is_lvalue_reference_v<_Ty>, "bad forward call");
	return (static_cast<string & >(_Arg));
}


_Ty = string
_NODISCARD constexpr string&& forward(string&& _Arg) _NOEXCEPT
{	// forward an rvalue as an rvalue
	static_assert(!is_lvalue_reference_v<_Ty>, "bad forward call");
	return (static_cast<string&&>(_Arg));
}

#endif