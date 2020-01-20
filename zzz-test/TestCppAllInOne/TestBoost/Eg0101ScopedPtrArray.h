#pragma once

#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <iostream>

namespace SmartLib
{
	using namespace ::std;
	using namespace  ::boost;

	class Eg0101ScopedPtr
	{
	public:
		static void Case1()
		{
			scoped_array<int> sa{new int[2]};
			*sa.get() = 100;
			sa[1] = 200;

			sa.reset(new int[3]);
			sa[2] = 300;

			cout << sa[0] <<", " << sa[1] << ", " << sa[2] << endl;
 		}


		static void Case0()
		{
			scoped_ptr<int> sp{ new int{1} };
			cout << *sp << endl;

			sp.reset(new int{ 2 });
			cout << *sp.get() << endl;

			sp.reset();
			cout << boolalpha << static_cast<bool>(sp) << endl;
		}
	};
}