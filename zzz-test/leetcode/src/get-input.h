#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>

namespace SmartLib
{
	using namespace ::std;

	class Helper
	{
	public:
		static vector<int> StringToInts(string& str)
		{
			vector<int> vals;
			const char DELIMITER[] = " \t\n\r";
#ifdef  _WIN32
			char* context = nullptr;
			char* tok = strtok_s(&str[0], DELIMITER, &context);
#else
			char* tok = strtok(&str[0], DELIMITER);
#endif //  _WIN32

	
			while (tok)
			{
				vals.push_back(stoi(tok));

#ifdef _WIN32
				tok = strtok_s(nullptr, DELIMITER, &context);
#else
				tok = strtok(nullptr, DELIMITER);
#endif // _WIN32
	
			}
			return vals;
		}

	public:
		static vector<vector<int>> StringsToInts(vector<string>& lines)
		{
			vector<vector<int>> lineVals;
			for (auto& line : lines)
			{
				auto values = StringToInts(line);
				lineVals.push_back(move(values));
			}

			return lineVals;
		}

	public:
		//template<typename TIN = decltype(cin)>
		static vector<string> GetLines(decltype(cin)& in)
		{
			string line;
			vector<string> lines;
			while (::std::getline(in, line))
			{
				if (0 == line.size())
				{
					break;
				}
				lines.push_back(move(line));
			}

			return lines;
		}

	public:
		//template<typename TIN = decltype(cin)>
		static vector<vector<int>> GetLineVals(decltype(cin)& in)
		{
			auto lines = GetLines(in);
			auto lineVals = StringsToInts(lines);

			return lineVals;
		}

	public:
		template<typename ITER>
		static void PrintContainer(ITER begin, ITER end)
		{
			while (begin != end)
			{
				cout << *begin << ' ';
				++begin;
			}
			cout << endl;
		}

	};
}