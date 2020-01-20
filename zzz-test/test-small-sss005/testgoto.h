#pragma once


#include <vector>
#include <iostream>
using namespace std;

namespace SmartLib
{
	class TestGoto
	{
	public:
		static void Case0()
		{
			int data = 0;

		HELLO:
			vector<int> vec;
			vec.push_back(data++);
			cout << vec.size() << endl;
			if (data > 5)
			{
				return;
			}
			goto HELLO;

		}
	};
}