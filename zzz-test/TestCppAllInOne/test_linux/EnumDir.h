#pragma once

#include <dirent.h>
#include <errno.h>
#include <string.h>

#include <iostream>
#include <string>
/*
[" << 
<< "]
*/

namespace SmartLib
{
	class  CEnumDir
	{
	public:
		static void EnumDir(const char * dirName)
		{
			int error = 0;
			const char* errstr = nullptr;
			DIR* dir = ::opendir(dirName);
			if (dir == nullptr)
			{
				error = errno;
				errstr = strerror(error);
				std::cout << "failed to open [" << dirName<< "], errno is [" << error<< "][" << errstr<< "]" << std::endl;
			}
			else
			{
				while (dirent * de = ::readdir(dir))
				{
					if (de->d_type == DT_DIR)
					{
						std::cout << "[D]" << de->d_name << std::endl;
						if (strcmp(".", de->d_name) && strcmp("..", de->d_name))
						{
							std::string subdir = dirName;
							subdir += '/';
							subdir += de->d_name;
							EnumDir(subdir.c_str());
						}
					}
					else
					{
						std::cout << "[F]" << de->d_name << std::endl;
					}
				}
				::closedir(dir);
				dir = nullptr;
			}
		}

	};
}