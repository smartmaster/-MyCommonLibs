#pragma once
#include "ReadConfig.h"


namespace Smartlib
{
	class CReadConfigTest
	{
	public:
		static void Case0()
		{
			auto result = SmartLib::CReadConfig::ReadConfigs(nullptr);
			for (const auto & kv : result)
			{
				SML_LOG_LINE(TEXT("{%s, %s}"), kv.first.c_str(), kv.second.c_str());
			}
		}
	};
}