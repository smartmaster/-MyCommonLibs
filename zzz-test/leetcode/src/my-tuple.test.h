#pragma once

#include <string>
#include <iostream>

#include "my-tuple.h"



namespace SmartLib
{
	using namespace ::std;

	class TupleTest
	{
	public:
		static void Case0()
		{
			string temp{"hello"};
			Tuple<double, long, string> tpl{12.34, 5678L, static_cast<string&&>(temp)};

			cout << "there are " << tpl.ItemsCount() << " items in the tuple" << endl;
			cout << tpl.Get<0>() << " "
				<< tpl.Get<1>() << " "
				<< tpl.Get<2>() << endl;


			//Tuple<double, long, string> tpl2{111, 111, ""};
			//tpl2 = tpl;
			//cout << "there are " << tpl2.ItemsCount() << " items in the tuple" << endl;
			//cout << tpl2.Get<0>() << " "
			//	<< tpl2.Get<1>() << " "
			//	<< tpl2.Get<2>() << endl;

		}
	};
}