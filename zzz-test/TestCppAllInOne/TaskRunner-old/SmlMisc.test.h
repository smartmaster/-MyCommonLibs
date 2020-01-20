#pragma once

#include <string>
#include <memory>

#include "SmlMisc.h"


namespace SmartLib
{
	using namespace ::std;
	class MiscTest
	{
	public:
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