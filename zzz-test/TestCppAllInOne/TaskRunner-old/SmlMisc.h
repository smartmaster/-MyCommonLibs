#pragma once

#include <string>
#include <iostream>


namespace SmartLib
{
	using namespace ::std;

	class Misc
	{
	public:
		template<typename D>
		static void Assign(D&& data)
		{
			string str = static_cast<D&&>(data);
			cout << str << ", " << data << endl;
		}
	};
}