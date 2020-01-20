#pragma once
#include "HCPPCreate.h"

namespace SmartLib
{
	class CHCPPCreatorTest
	{
	public:
		static void Case1()
		{
			SmartLib::CHCPPCreator::Create(TEXT(".\\"), TEXT("sss-hellow-world"));
		}
	};
}