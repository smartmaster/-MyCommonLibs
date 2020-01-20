#pragma once

#include <boost/locale.hpp>
#include <iostream>

namespace bl = boost::locale;

namespace SmartLib
{
	class TestBoostTranslate
	{
	public:
		static void Case0()
		{
			bl::generator gen;
			gen.add_messages_path("E:/MyProjects/~MyCommonLibs/zzz-test/TestCppAllInOne/TestBoost/locale_mo");
			gen.add_messages_domain("ZettaMirror_Message");
			const std::locale& loc = gen("ja-jp");
			std::locale::global(loc);
			std::cout.imbue(std::locale());
			std::cout << bl::translate("VMware settings delivered") << std::endl;
		}

	};
}