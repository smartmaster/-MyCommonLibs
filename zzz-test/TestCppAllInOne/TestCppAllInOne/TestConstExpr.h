#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "log.h"

namespace SmartLib
{
	class AA
	{
	public:
		void PrintAA()
		{
			SML_LOG_LINE(TEXT("%s"), TEXT(__FUNCTION__));
		}
	};

	class BB
	{
	public:
		void PrinBB()
		{
			SML_LOG_LINE(TEXT("%s"), TEXT(__FUNCTION__));
		}
	};




	class TestConstExpr
	{
	public:
		template<typename T>
		static void ConstExprTest()
		{
//#define  CPPSTD17
#ifdef CPPSTD17
			T obj;
			if constexpr (std::is_same_v<T, AA>)
			{
				obj.PrintAA();
			}
			else if constexpr (std::is_same_v<T, BB>)
			{
				obj.PrinBB();
			}
			else
			{
				SML_LOG_LINE(TEXT("Do not know what the type is!"));
			}
#endif
		}


		static void Case0()
		{
			ConstExprTest<AA>();
			ConstExprTest<BB>();
			ConstExprTest<int>();
		}
	};
}