#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <fstream>
#include <string>
using std::wstring;


#include "StringOf.h"

namespace SmartLib
{
	class MyFormatCode
	{
	public:
		static wstring MyFormatCodeProc(LPCTSTR strRaw)
		{
			const LPCWSTR NEW_LINE = L"\r\n";
			const WCHAR TAB = L'\t';
			wstring formated;
			int tab = 0;
			bool newLine = false;

			while (*strRaw && (isspace(*strRaw) || L';' == *strRaw))
			{
				++strRaw;
			}

			bool double_quetes = false;
			bool single_quotes = false;

			while (*strRaw)
			{
				WCHAR ch = *strRaw;
				switch (ch)
				{
				case '\\':
				{
					formated += ch;
					++strRaw;
					if (*strRaw)
					{
						formated += *strRaw;
					}
					else
					{
						--strRaw;
					}
				}
				break;
				case L'\"':
				{
					double_quetes = !double_quetes;
					formated += ch;
				}
				break;
				case L'\'':
				{
					single_quotes = !single_quotes;
					formated += ch;
				}
				break;
				case L'{':
				{
					if (double_quetes || single_quotes)
					{
						formated += ch;
					}
					else
					{
						formated += NEW_LINE;
						formated.append(tab, TAB);
						formated += ch;
						newLine = true;
						++tab;
					}

				}
				break;
				case L'}':
				{
					if (double_quetes || single_quotes)
					{
						formated += ch;
					}
					else
					{
						newLine = true;
						--tab;
						formated += NEW_LINE;
						formated.append(tab, TAB);
						formated += ch;
					}

				}
				break;
				case L';':
				{
					if (double_quetes || single_quotes)
					{
						formated += ch;
					}
					else
					{
						newLine = true;
						formated += ch;
					}

				}
				break;
				default:
				{
					if (newLine)
					{
						while (*strRaw && (isspace(*strRaw) || L';' == *strRaw))
						{
							++strRaw;
						}

						if (*strRaw != L'}')
						{
							formated += NEW_LINE;
							formated.append(tab, TAB);
							formated += *strRaw;
							newLine = false;
						}
						else
						{
							--strRaw; //go back one step to process right bracket
						}

					}
					else
					{
						formated += *strRaw;
					}
				}
				break;
				}

				if (*strRaw)
				{
					++strRaw;
				}

			}
			return formated;
		}

		static void Case2(LPCWSTR src, LPCWSTR dest)
		{
			wifstream in(src);
			wstring line;
			wstring raw;
			while (std::getline(in, line))
			{
				raw += line;
			}

			SML_LOG_LINE(L"raw code is:");
			SML_LOG_LINE(L"%s", raw.c_str());
			SML_LOG_LINE(L"\r\n\r\n");


			std::wstring formated = MyFormatCodeProc(raw.c_str());
			SML_LOG_LINE(L"formated code is:");
			SML_LOG_LINE(L"%s", formated.c_str());

			wofstream out(dest);
			out << formated;
		}

		static void Case1()
		{
			LPCWSTR strRaw = SML_WSTRING_OF(if (1) { 1 + 1; });
			SML_LOG_LINE(L"raw code is:");
			SML_LOG_LINE(L"%s", strRaw);
			SML_LOG_LINE(L"\r\n\r\n");


			std::wstring formated = MyFormatCodeProc(strRaw);
			SML_LOG_LINE(L"formated code is:");
			SML_LOG_LINE(L"%s", formated.c_str());
		}

		static void Case0()
		{
			LPCWSTR strRaw = SML_WSTRING_OF(static wstring MyFormatCodeProc(LPCTSTR strRaw)
			{
				const LPCWSTR NEW_LINE = L"\r\n";
				const WCHAR TAB = L'\t';
				wstring formated;
				int tab = 0;
				bool newLine = false;

				while (*strRaw && (isspace(*strRaw) || L';' == *strRaw))
				{
					++strRaw;
				}

				bool double_quetes = false;
				bool single_quotes = false;

				while (*strRaw)
				{
					WCHAR ch = *strRaw;
					switch (ch)
					{
					case '\\':
					{
						formated += ch;
						++strRaw;
						if (*strRaw)
						{
							formated += *strRaw;
						}
						else
						{
							--strRaw;
						}
					}
					break;
					case L'\"':
					{
						double_quetes = !double_quetes;
						formated += ch;
					}
					break;
					case L'\'':
					{
						single_quotes = !single_quotes;
						formated += ch;
					}
					break;
					case L'{':
					{
						if (double_quetes || single_quotes)
						{
							formated += ch;
						}
						else
						{
							formated += NEW_LINE;
							formated.append(tab, TAB);
							formated += ch;
							newLine = true;
							++tab;
						}

					}
					break;
					case L'}':
					{
						if (double_quetes || single_quotes)
						{
							formated += ch;
						}
						else
						{
							newLine = true;
							--tab;
							formated += NEW_LINE;
							formated.append(tab, TAB);
							formated += ch;
						}

					}
					break;
					case L';':
					{
						if (double_quetes || single_quotes)
						{
							formated += ch;
						}
						else
						{
							newLine = true;
							formated += ch;
						}

					}
					break;
					default:
					{
						if (newLine)
						{
							while (*strRaw && (isspace(*strRaw) || L';' == *strRaw))
							{
								++strRaw;
							}

							if (*strRaw != L'}')
							{
								formated += NEW_LINE;
								formated.append(tab, TAB);
								formated += *strRaw;
								newLine = false;
							}
							else
							{
								--strRaw; //go back one step to process right bracket
							}

						}
						else
						{
							formated += *strRaw;
						}
					}
					break;
					}

					if (*strRaw)
					{
						++strRaw;
					}

				}
				return formated;
			});


			SML_LOG_LINE(L"raw code is:");
			SML_LOG_LINE(L"%s", strRaw);
			SML_LOG_LINE(L"\r\n\r\n");


			std::wstring formated = MyFormatCodeProc(strRaw);
			SML_LOG_LINE(L"formated code is:");
			SML_LOG_LINE(L"%s", formated.c_str());
		}
	};
}