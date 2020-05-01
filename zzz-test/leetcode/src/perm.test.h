#pragma once
#include "perm.h"


namespace SmartLib
{
	class DFSCombineAlgTest
	{
	public:
		static void Case0()
		{
			long maxNumer = 5;
			long maxCount = 2;
			SmartLib::DFSCombineAlg::Solution(maxNumer, maxCount);
		}

		static void Case1()
		{
			long maxNumer = 5;
			long maxCount = 2;
			SmartLib::DFSPermAlg::Solution(maxNumer, maxCount);
		}
	};
}