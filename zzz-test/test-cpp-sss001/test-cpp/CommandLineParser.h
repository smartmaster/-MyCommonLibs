#pragma once

#include <vector>
#include <map>
#include <string>
using namespace  std;

#include "std_tstring.h"

namespace SmartLib
{

	class CCommandLineParser
	{
	private:
		map<std_tstring, vector<std_tstring>> _cmdLine;

	public:
		//the command line must be in the pattern
		//exe name1 value1 name2 value2 ...
		void Parse(int argc, TCHAR ** argv)
		{
			if (argc > 0)
			{
				_cmdLine.insert({ TEXT(""), { argv[0] } });
			}

			for (int ii = 1; ii + 1 < argc; ii += 2)
			{
				auto iter = _cmdLine.find(argv[ii]);
				if (iter != _cmdLine.end())
				{
					iter->second.push_back(argv[ii + 1]);
				}
				else
				{
					_cmdLine.insert({ argv[ii], { argv[ii + 1] } });
				}
			}
		}

		const vector<std_tstring> & GetCommandLines(LPCTSTR name)
		{
			static const vector<std_tstring> empty;

			auto iter = _cmdLine.find(name);
			if (iter == _cmdLine.end())
			{
				return empty;
			}
			else
			{
				return iter->second;
			}
		}


		const std_tstring GetCommandLine(LPCTSTR name)
		{
			const auto & cmdLines = GetCommandLines(name);
			return cmdLines.size() ? cmdLines[0] : TEXT("");
		}
	};
}