#pragma once
#include "EnumDir.h"

namespace SmartLib
{
	class  CEnumDirTest
	{
	public:
		static void Case0()
		{
			SmartLib::CEnumDir::EnumDir("/home/smart/sss-nix");
			SmartLib::CEnumDir::EnumDir("/home/smart");
		}
	};
}