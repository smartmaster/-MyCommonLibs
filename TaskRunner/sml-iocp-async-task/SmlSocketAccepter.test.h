#pragma once
#include "SmlSocketAccepter.h"

#include <iostream>
#include <string>

namespace SmartLib
{
	using namespace ::std;

	class SocketAccepterTest
	{
	public:
		static void Case0()
		{
			SmartLib::SocketAccepter::Run(TEXT("54321"), TaskRunner::Instance());
			cout << "press ENTER to continue..." << endl;

			string line;
			getline(cin, line);

			return;
		}
	};
}