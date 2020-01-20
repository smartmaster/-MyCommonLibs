#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <string>
#include <vector>
#include <map>
#include <functional>


#include "CodeBlock.h"
#include "log.h"

namespace SmartLib
{
	class CReadConfig
	{
		template<typename T>
		static T* Find(T* first, T* last, std::function<bool(T*)> test)
		{
			while (first <= last)
			{
				if (test(first))
				{
					break;
				}
				++first;
			}
			return first;
		}

		template<typename T>
		static T* ReverseFind(T* rfirst, T* rlast, std::function<bool(T*)> test)
		{
			while (rfirst >= rlast)
			{
				if (test(rfirst))
				{
					break;
				}
				--rfirst;
			}

			return rfirst;
		}

		static std::wstring Trim(const std::wstring& str)
		{
			if (0 == str.size())
			{
				return str;
			}

			auto first = Find<const WCHAR>(&str[0], &str.back(), 
			[](const WCHAR* ptr) 
			{
				return !std::isspace(*ptr);
			});

			auto last = ReverseFind<const WCHAR>(&str.back(), first,
			[](const WCHAR* ptr)
			{
				return !std::isspace(*ptr);
			});

			return first <= last ? std::wstring{ first, (ULONGLONG)(last - first) + 1 } : std::wstring{};
		}

		static void ParseLine(const std::wstring& line, WCHAR delimeter, bool trimspace, std::wstring& key, std::wstring& value)
		{

			auto pos = line.find(delimeter);
			if (std::wstring::npos != pos)
			{
				key = line.substr(0, pos);
				value = line.substr(pos + 1);
			}
			else
			{
				key = line;
				value = std::wstring{};
			}

			if (trimspace)
			{
				key = Trim(key);
				value = Trim(value);
			}
		}

	public:
		static std::vector<std::wstring> ReadLines(LPCTSTR Filename)
		{
			DWORD lastError = 0;
			HRESULT hr = S_OK;

			std::vector<std::wstring> lines;
			std::wifstream in(Filename);
			if (in)
			{
				in.imbue(std::locale{ in.getloc(), new std::codecvt_utf8_utf16<WCHAR>{} });
				std::wstring line;
				while (std::getline(in, line))
				{
					lines.push_back(std::move(line));
				}

				in.close();
			}

			return lines;
		}

		static std::map<std::wstring, std::wstring> ParseLines(const std::vector<std::wstring>& configs, WCHAR delimeter, bool trimspace)
		{
			std::map<std::wstring, std::wstring> kvmap;
			for (const auto & line : configs)
			{
				if (line.size())
				{
					std::wstring key, value;
					ParseLine(line, delimeter, trimspace, key, value);
					if (key.size())
					{
						kvmap.insert({ std::move(key), std::move(value) });
					}
				}
			}

			return kvmap;
		}

		static std::map<std::wstring, std::wstring> ReadConfigs(LPCTSTR ext, WCHAR delimeter = L'=', bool trimspace = true)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;


			std::map<std::wstring, std::wstring> kvmap;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			TCHAR fileName[512] = { 0 };
			DWORD dwgmf = GetModuleFileName(
				nullptr,//_In_opt_  HMODULE hModule,
				fileName,//_Out_     LPTSTR lpFilename,
				_countof(fileName)//,//_In_      DWORD nSize
			);
			if (0 == dwgmf)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("API [%s] failed with hr=[0x%08x], error=[%d]"), TEXT(""), LastError, hr);
				LEAVE_BLOCK(0);
			}

			static const TCHAR EXT[] = TEXT(".conf");
			_tcscat_s(fileName, ext && ext[0]? ext : EXT);

			auto lines = ReadLines(fileName);
			if (0 == lines.size())
			{
				auto temp = _tcsrchr(fileName, TEXT('\\'));
				if (temp)
				{
					temp -= 2;
					temp[0] = L'c';
					temp[1] = L':';
					lines = ReadLines(temp);
				}
			}
			kvmap = ParseLines(lines, delimeter, trimspace);
			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return kvmap;
		}

	};
}