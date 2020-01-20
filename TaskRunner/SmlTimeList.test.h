#pragma once
#include "SmlTimeList.h"

#include <string>
#include <iostream>

namespace SmartLib
{
	using namespace ::std;

	class SmlTimeListTest
	{
	public:
		static void Case0()
		{
			TimeList<string> tl{1000, 3000};
			tl.SetDispose( [](string& str)
			{
				cout << str << " ";
			} );


			for (long ii = 0; ii < 10; ++ ii)
			{
				tl.push_back(to_string(ii));
			}


			this_thread::sleep_for(chrono::seconds(5));

			tl.StopScan();

		}
	};
}