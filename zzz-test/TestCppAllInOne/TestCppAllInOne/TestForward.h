#pragma once

#include "log.h"

namespace SmartLib
{
	class CTestFwd
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
			Add(std::forward<T>(val));
		}

		static int RetVal()
		{
			return 789;
		}

		static void Case0()
		{
			CallAdd(123);
			CallAdd(RetVal());
			int val = 567;
			CallAdd(val);
			CallAdd((int)val);
			CallAdd((int&)val);
			CallAdd((int&&)val);

			int rv = 123;
			SML_LOG_LINE(TEXT("before %d"), rv);
			ChangeRVal(std::move(rv));
			SML_LOG_LINE(TEXT("after %d"), rv);
		}
	};
}


