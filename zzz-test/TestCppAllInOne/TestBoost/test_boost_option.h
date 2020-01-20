#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include <sstream>

#include <boost/program_options.hpp>
#include <iostream>
using namespace boost::program_options;

namespace SmartLib
{
	class TestBoostOption
	{
	public:


		static void on_age(int age)
		{
			std::cout << "On age: " << age << '\n';
		}
		static int Case0(int argc, TCHAR* argv[])
		{
			try
			{
				options_description desc{ "Options微软发生的附件客户是否可接受和角色划分的就是Options" };
				desc.add_options()
					("help,h", "是客人反馈绿色仿射空间发生金\n额开发和数据库恢复健康的\n是衣服第十一房间号多少节\n课发射机返回的是会发生即可\n恢复健康的设色戒\n发货的建设");
				std::stringstream ss;
				ss << desc;
				std::cout << ss.str() << std::endl;
				
			}
			catch (const error & ex)
			{
				std::cerr << ex.what() << '\n';
			}

			return 0;
		}
	};
}