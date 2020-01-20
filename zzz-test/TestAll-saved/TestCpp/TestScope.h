#pragma once

#include <iostream>

namespace TestCpp
{
	class TestScope
	{
	public:
		static int Case0()
		{
			int a = 5; // define a#1
			{
				a = 3; // assign a#1, a#2 is not defined yet
				int a; // define a#2
				a = 8; // assign a#2, a#1 is hidden
				{
					a = 7; // assign a#2
				}

				std::cout << a << std::endl;

			} // end of a#2¡¯s scope
			a = 11; // assign to a#1 (a#2 out of scope)
			return 0;
		}
	};
}