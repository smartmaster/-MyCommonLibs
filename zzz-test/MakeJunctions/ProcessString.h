#pragma once



#include <iostream>
#include <fstream>
#include <string>

namespace SmartLib
{
	class PathProcessor
	{
	private:
		std::wifstream _in;
		std::wofstream _out;
		wchar_t _newDrive;
	private:
		static bool IsTrimed(wchar_t ch)
		{
			return 
				ch == L'\t' || 
				ch == L' ' || 
				ch == L'\r' || 
				ch == L'\n' ||
				ch == L'\"';
		}

		static void DataCharRange(const std::wstring & str, int & first, int & last)
		{
			first = 0;
			while (first < str.size())
			{
				if (IsTrimed(str[first]))
				{
					++first;
				}
				else
				{
					break;
				}
			}

			last = str.size() - 1;
			if (first != str.size())
			{
				while (last >= 0)
				{
					if (IsTrimed(str[last]))
					{
						--last;
					}
					else
					{
						break;
					}
				}
			}
		}

		PathProcessor(const wchar_t * inFile, const wchar_t * outFile, wchar_t newDrive)
		{
			_in.open(inFile);
			_out.open(outFile);
			_newDrive = newDrive;
		}
		void ProcessString(std::wstring && str)
		{

/****************************
C:\Program Files (x86)\Windows Kits

mkdir "C:\Program Files (x86)"
mkdir "E:\Program Files (x86)\Windows Kits"
mklink /j "C:\Program Files (x86)\Windows Kits" "E:\Program Files (x86)\Windows Kits"

****************************/


			_out << L"rem Processing \"" << str.c_str() << L"\"" << std::endl;
			size_t index = str.rfind(L'\\');
			if (index == std::wstring::npos)
			{
				return;
			}

			str[index] = 0;
			_out << L"mkdir \"" << str.c_str() << L"\"" << std::endl;
			str[index] = L'\\';

			wchar_t oldDrive = str[0];
			str[0] = _newDrive;
			_out << L"mkdir \"" << str.c_str() << L"\"" << std::endl;


			str[0] = oldDrive;
			_out << L"mklink /j \"" << str.c_str() << L"\" ";
			str[0] = _newDrive;
			_out << L"\"" << str.c_str() << L"\"" << std::endl << std::endl;
		}

		void ProcessFile()
		{
			static const wchar_t min_path[] = L"c:\\12\\34";
			std::wstring line;
			while (std::getline(_in, line))
			{
				int first = 0;
				int last = 0;
				DataCharRange(line, first, last);

				if (first == line.size())
				{
					continue;
				}
				
				if (last - first + 1 < _countof(min_path))
				{
					continue;
				}

				if (line[first + 1] != L':')
				{
					continue;
				}

				ProcessString(line.substr(first, last - first + 1));
			}
		}

	public:
		static void API(const wchar_t * inFile, const wchar_t * outFile, wchar_t newDrive)
		{
			PathProcessor processor(inFile, outFile, newDrive);
			processor.ProcessFile();
		}
	};
}

