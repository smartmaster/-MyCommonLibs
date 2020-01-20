#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <functional>
#include <fstream>

//#include <fstream>;
#include <string>
using std::wstring;


//#include "StringOf.h";

namespace SmartLib
{


	class MyFormatCode
	{
	private:
		template<typename T>
		static void SkipWhile(T* & buffer, std::function<bool(T)> cond)
		{
			while (cond(*buffer))
			{
				++buffer;
			}
		}

		static void SkipSapce(LPCTSTR& buffer)
		{
			auto cond = [](CONST TCHAR ch)
			{
				return ch != 0 && isspace(ch);
			};

			SkipWhile<CONST TCHAR>(buffer, cond);
		}


		template<typename T, typename C>
		static void TakeWhile(C& c, T*& buffer, std::function<bool(T)> cond)
		{
			while (cond(*buffer))
			{
				c += *buffer;
				++buffer;
			}
		}

		static LPCTSTR NewLine()
		{
			return L"\r\n";
		}

		static TCHAR LeadingSpace()
		{
			return L'\t';
		}

		static void PrefixNewlineAndSpaces(wstring & result, int spacecount, TCHAR ch)
		{
			result += NewLine();
			result.append(spacecount, LeadingSpace());
			if (ch != 0)
			{
				result += ch;
			}
		}

		static int FindComments(const CString & str)
		{
			int pos = str.Find(TEXT("\""));
			if (pos != -1)
			{
				++pos;
				while (pos < str.GetLength())
				{
					if (str[pos] == L'\\')
					{
						++pos;
					}
					else if (str[pos] == L'\"')
					{
						++pos;
						break;
					}

					++pos;
				}

				if (pos < str.GetLength())
				{
					pos = str.Find(TEXT("//"));
				}
			}
			else
			{
				pos = str.Find(TEXT("//"));
			}

			return pos;
		}

	public:
		//to do:
		//not handle comments properly
		//not handle #??? properly
		static wstring Proc(LPCTSTR raw)
		{
			wstring result;
			SkipSapce(raw);

			int leadingSpaceCount = 0;
			bool newline = false;
			while (*raw)
			{
				TCHAR ch = *raw;

				switch (ch)
				{
				case L'{':
				{
					PrefixNewlineAndSpaces(result, leadingSpaceCount, ch);

					++leadingSpaceCount;
					newline = true;

					++raw;
					SkipSapce(raw);
				}
				break;
				
				
				case L'}':
				{
					--leadingSpaceCount;

					PrefixNewlineAndSpaces(result, leadingSpaceCount, ch);
					
					newline = true;

					++raw;
					SkipSapce(raw);
				}
				break;

				case L';':
				{
					result += ch;

					newline = true;
					
					++raw;
					SkipSapce(raw);
				}
				break;

				

				case L'\'':
				case L'\"':
				{
					result += ch;
					++raw;
					while (*raw)
					{
						if (*raw == L'\\')
						{
							result += *raw;
							++raw;
							if (*raw)
							{
								result += *raw;
								++raw;
							}
						}
						else if (*raw == ch)
						{
							result += *raw;
							++raw;
							break;
						}
						else
						{
							result += *raw;
							++raw;
						}
					}
				}
				break;

				

				
#if 0 //single line comment;
				case L'/':  
				{
					result += ch;
					++raw;
					auto cond = [](const TCHAR ch)
					{
						return ch != 0 && ch != L'\r' && ch != L'\n';
					};
					TakeWhile<CONST TCHAR>(result, raw, cond);
					SkipSapce(raw);
					result += NewLine();
					result.append(leadingSpaceCount, LeadingSpace());

					SkipSapce(raw);
				}
				break;

				case L'#':
				{
					result += ch;
					++raw;
					auto cond = [](const TCHAR ch) 
					{
						return ch != 0 && ch != L'\r'&& ch != L'\n' && ch != L'#';
					};
					TakeWhile<CONST TCHAR>(result, raw, cond);
					SkipSapce(raw);
					result += NewLine();
					result.append(leadingSpaceCount, LeadingSpace());

					SkipSapce(raw);
				}
				break;
#endif //;

				default:
				{
					if (newline)
					{
						newline = false;
						PrefixNewlineAndSpaces(result, leadingSpaceCount, 0);
					}

					result += ch;
					++raw;
				}
				break;

				}
			}

			return result;
		}

		static void API(LPCWSTR src, LPCWSTR dest)
		{
			std::wifstream in(src);
			std::wstring line;
			std::wstring raw;
			bool newline = false;
			while (std::getline(in, line))
			{
				CString strline = line.c_str();
				
				strline.Trim();

				raw += strline.GetString();
				//raw += NewLine();

			}
			in.close();

			////SML_LOG_LINE(L"raw code is:");
			////SML_LOG_LINE(L"%s", raw.c_str());
			////SML_LOG_LINE(L"\r\n\r\n");


			std::wstring formated = MyFormatCode::Proc(raw.c_str());
			//SML_LOG_LINE(L"formated code is:");
			//SML_LOG_LINE(L"%s", formated.c_str());

			//wofstream out(dest);
			//out << formated << std::endl;
			//out.close();

			HANDLE hfile = ::CreateFile(
				dest,//_In_      LPCTSTR lpFileName,;
				GENERIC_WRITE,//_In_      DWORD dwDesiredAccess,;
				0,//_In_      DWORD dwShareMode,;
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,;
				CREATE_ALWAYS,//_In_      DWORD dwCreationDisposition,;
				0,//_In_      DWORD dwFlagsAndAttributes,;
				nullptr//,//_In_opt_  HANDLE hTemplateFile;
			);

			if (INVALID_HANDLE_VALUE != hfile)
			{
				BYTE bom[] = { 0xff, 0xfe };
				DWORD NumberOfBytesWritten = 0;
				::WriteFile(
					hfile,//_In_ HANDLE hFile,;
					bom,//_In_reads_bytes_opt_(nNumberOfBytesToWrite) LPCVOID lpBuffer,;
					sizeof(bom),//_In_ DWORD nNumberOfBytesToWrite,;
					&NumberOfBytesWritten,//_Out_opt_ LPDWORD lpNumberOfBytesWritten,;
					nullptr//,//_Inout_opt_ LPOVERLAPPED lpOverlapped;
				);

				NumberOfBytesWritten = 0;
				::WriteFile(
					hfile,//_In_ HANDLE hFile,;
					formated.c_str(),//_In_reads_bytes_opt_(nNumberOfBytesToWrite) LPCVOID lpBuffer,;
					formated.size() * sizeof(TCHAR),//_In_ DWORD nNumberOfBytesToWrite,;
					&NumberOfBytesWritten,//_Out_opt_ LPDWORD lpNumberOfBytesWritten,;
					nullptr//,//_Inout_opt_ LPOVERLAPPED lpOverlapped;
				);

				::CloseHandle(hfile);
				hfile = INVALID_HANDLE_VALUE;
			}
		}

#if 1
		static wstring MyFormatCodeProc(LPCTSTR strRaw)
		{
			const LPCWSTR NEW_LINE = L"\n";
			const WCHAR TAB = L'\t';
			wstring formated;
			int tab = 0;
			bool newLine = false;

			//while (*strRaw && (isspace(*strRaw) || L';' == *strRaw))
			//{
			//	++strRaw;
			//};

			SkipSapce(strRaw);

			bool double_quetes = false;
			bool single_quotes = false;

			while (*strRaw)
			{
				WCHAR ch = *strRaw;
				switch (ch)
				{
					//case L'#':
					//{
					//	while (*strRaw)
					//	{
					//		formated += *strRaw;
					//		++strRaw;
					//		if (*strRaw == L'#')
					//		{
					//			formated += NEW_LINE;
					//			break;
					//		}
					//	}
					//	--strRaw;
					//	newLine = true;
					//}
					//break;
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

						switch (*strRaw)
						{
						case L'}':
							//case L'#':
						{
							--strRaw; //go back one step to process right bracket
						}
						break;
						default:
						{
							formated += NEW_LINE;
							formated.append(tab, TAB);
							formated += *strRaw;
							newLine = false;
						}
						break;
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
#endif
	};
}