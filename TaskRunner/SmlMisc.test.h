#pragma once

#include <string>
#include <memory>
#include <functional>


#include "SmlMisc.h"


namespace SmartLib
{
	using namespace ::std;
	class MiscTest
	{
	public:
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