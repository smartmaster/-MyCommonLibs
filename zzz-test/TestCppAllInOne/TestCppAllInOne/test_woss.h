#pragma once

//#include <tchar.h>
//#include <windows.h>
//#include <atlbase.h>
//#include <atlstr.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <cassert>

#include "CodeBlock.h"
using namespace std;

namespace SmartLib
{
	class TestWOSS
	{
	public: 

		static void Case_print_CJK()
		{
			WCHAR composed[] = { 0xc4, 00 };
			WCHAR wide[] = L"+++++++++人人人人人人人人人人人人人人人人人+++++++++";
			int codePages[] = {CP_UTF8, 932 /*japan*/, 936 /*china*/, 949 /*korea*/ };
			const char* country[] = {"utf8", "japan", "china", "korea"};
			for (int ii = 0; ii < _countof(codePages); ++ ii)
			{
				std::string narrow;
				W2N(codePages[ii], 0, wide, narrow);
				cout << country[ii] << narrow.c_str() << std::endl;

				
				std::wstring wideConverted;
				Ok2Convert(codePages, _countof(codePages), MB_ERR_INVALID_CHARS, 0, narrow, wideConverted);
			}

			
		}

		static void Case_conv_file()
		{
			{
				std::wstring wide;
				std::string narrow;
				W2N(CP_ACP, 0, wide, narrow);
			}


			{
				std::wstring wide;
				std::string narrow;
				N2W(CP_ACP, 0, narrow, wide);
			}
			
			ConvertFie(L"E:/MyProjects/~MyCommonLibs/zzz-test/TestCppAllInOne/x64/Debug/j.txt", CP_ACP);
			ConvertFie(L"E:/MyProjects/~MyCommonLibs/zzz-test/TestCppAllInOne/x64/Debug/j.txt", CP_UTF8);
			ConvertFie(L"E:/MyProjects/~MyCommonLibs/zzz-test/TestCppAllInOne/x64/Debug/j.txt", 932);
		}

		static void ConvertFie(LPCWSTR file, int codepage)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			HANDLE handle = ::CreateFile(
				file,//_In_      LPCTSTR lpFileName,
				GENERIC_READ,//_In_      DWORD dwDesiredAccess,
				FILE_SHARE_READ,//_In_      DWORD dwShareMode,
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				OPEN_EXISTING,//_In_      DWORD dwCreationDisposition,
				0,//_In_      DWORD dwFlagsAndAttributes,
				nullptr//_In_opt_  HANDLE hTemplateFile
			);
			if (INVALID_HANDLE_VALUE == handle)
			{
				LEAVE_BLOCK(0);
			}

			const int BUFFER_SIZE = 64 * 1024;
			CHAR* buffer = new char[BUFFER_SIZE];
			ZeroMemory(buffer, BUFFER_SIZE);

			DWORD NumberOfBytesRead = 0;
			::ReadFile(
				handle,//_In_         HANDLE hFile,
				buffer,//_Out_        LPVOID lpBuffer,
				BUFFER_SIZE,//_In_         DWORD nNumberOfBytesToRead,
				&NumberOfBytesRead,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
				nullptr//_Inout_opt_  LPOVERLAPPED lpOverlapped
			);

			std::wstring zstr;
			DWORD dwFlags = 0;
			ToZstring(buffer, zstr,
				codepage, dwFlags);

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			//return hr;

			



		}

#ifdef _WIN32
		static void ToZstring(const std::string &narrow, std::wstring &zstr,
			UINT codePage, DWORD dwFlags)
		{
			// note ( CP_ACP, MB_PRECOMPOSED, ... ) will convert Windows ASCII 0x7F..0xFF
			// note ( CP_UTF8, 0, ... ) does terrible things to unicode (string overrun?)
			int zlength = ::MultiByteToWideChar(codePage, dwFlags, narrow.c_str(), -1, 0, 0);
			if (zlength > 0) {
				zstr.resize(zlength);
				::MultiByteToWideChar(codePage, dwFlags, narrow.c_str(), -1, &zstr[0], zlength);
				zstr.resize(zlength - 1);
			}
			else {
				zstr.clear();
			}
		}

		static void Case_test_zetta()
		{
			char narrow[] = "++++++++++人人呢人人人热热热热热++++++++abcd++++++++";

			std::wstring zstr;
			ToZstring(narrow, zstr, CP_ACP, MB_ERR_INVALID_CHARS);
			
			std::wstring zstr1;
			ToZstring(narrow, zstr1, CP_ACP, MB_ERR_INVALID_CHARS);
			
			std::wstring zstr2;
			ToZstring(narrow, zstr2, CP_UTF8, MB_ERR_INVALID_CHARS);
			
			std::wstring zstr3;
			ToZstring(narrow, zstr3, 936, MB_ERR_INVALID_CHARS);
		}
#endif // def _WIN32

		static void WPrintCJK()
		{

			WCHAR wide[] = L"-----人人人人人人人人人人人人人人人人人人人人人人人人人人人人人人------";

			DWORD NumberOfCharsWritten = 0;
			::WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), wide, _countof(wide) - 1, &NumberOfCharsWritten, nullptr);
			::WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), L"\r\n", 2, &NumberOfCharsWritten, nullptr);

			setlocale(LC_ALL, "");

			WCHAR wideEEEEEEE[] = L"-----EEEEEEEEEEEEEEEEEEEEEEEE------";
			wprintf(wideEEEEEEE);
			wprintf(L"\r\n");

			std::wcout << wideEEEEEEE << std::endl;

			wprintf(wide);
			wprintf(L"\r\n");

			std::wcout << wide << std::endl;
			
		}

		static void PrintCJK()
		{
			WCHAR wide[] = L"------好好好好好好好好好好好好好好好好好好好好好好好好好好好好---------";

			std::string narrow;
			W2N(936, 0, wide, narrow);
			std::cout << "C: " << narrow.c_str() << std::endl << std::endl;

			W2N(932, 0, wide, narrow);
			std::cout << "J: " << narrow.c_str() << std::endl << std::endl;

			W2N(949, 0, wide, narrow);
			std::cout << "K: " << narrow.c_str() << std::endl << std::endl;


			W2N(CP_UTF8, 0, wide, narrow);
			std::cout << "U: " << narrow.c_str() << std::endl << std::endl;

		}

		static void Case_Convertable_current_page()
		{

			CHAR narrowStr[] = "---------发觉色戒覅欧塞u给覅公开萨尔u发---------";

			std::wstring wide;
			PrintCodePageInfo(CP_ACP);
			bool b1 = Ok2Conv(CP_ACP, 0, 0, narrowStr, wide);
			// xxx SML_LOG_LINE(TEXT("convertable %d"), b1);
			// xxx SML_LOG_LINE(TEXT(""));

			PrintCodePageInfo(CP_THREAD_ACP);
			bool b2 = Ok2Conv(CP_THREAD_ACP, 0, 0, narrowStr, wide);
			// xxx SML_LOG_LINE(TEXT("convertable %d"), b2);
			// xxx SML_LOG_LINE(TEXT(""));

			PrintCodePageInfo(CP_UTF8);
			bool b3 = Ok2Conv(CP_UTF8, 0, 0, narrowStr, wide);
			// xxx SML_LOG_LINE(TEXT("convertable %d"), b3);
			// xxx SML_LOG_LINE(TEXT(""));

		}

		static void Case_Convertable_utf8()
		{

			WCHAR wideStr[] = L"---------发觉色戒覅欧塞u给覅公开萨尔u发---------されたパスが見つかりませ---------";
			CHAR ansiStr[] = "---------发觉色戒覅欧塞u给覅公开萨尔u发---------されたパスが見つかりませ---------";

			std::string narrow;
			W2N(CP_UTF8, 0, wideStr, narrow);

			std::cout << "ansi:" << ansiStr << std::endl;
			std::cout << "utf8:" << narrow.c_str() << std::endl;

			PrintCodePageInfo(CP_THREAD_ACP);
			// xxx SML_LOG_LINE(TEXT(""));

			std::wstring wide;
			PrintCodePageInfo(936);
			bool b1 = Ok2Conv(936, 0, 0, narrow, wide);
			// xxx SML_LOG_LINE(TEXT("convertable %d"), b1);
			// xxx SML_LOG_LINE(TEXT(""));

			PrintCodePageInfo(932);
			bool b2 = Ok2Conv(932, 0, 0, narrow, wide);
			// xxx SML_LOG_LINE(TEXT("convertable %d"), b2);
			// xxx SML_LOG_LINE(TEXT(""));

			PrintCodePageInfo(CP_UTF8);
			bool b3 = Ok2Conv(CP_UTF8, 0, 0, narrow, wide);
			// xxx SML_LOG_LINE(TEXT("convertable %d"), b3);
			// xxx SML_LOG_LINE(TEXT(""));

		}

		static void Case_Convertable_China()
		{
			
			CHAR narrowStr[] = "---------发觉色戒覅欧塞u给覅公开萨尔u发---------";

			PrintCodePageInfo(CP_THREAD_ACP);
			// xxx SML_LOG_LINE(TEXT(""));

			std::wstring wide;

			PrintCodePageInfo(936);
			bool b1 = Ok2Conv(936, 0, 0, narrowStr, wide);
			// xxx SML_LOG_LINE(TEXT("convertable %d"), b1);
			// xxx SML_LOG_LINE(TEXT(""));

			PrintCodePageInfo(932);
			bool b2 = Ok2Conv(932, 0, 0, narrowStr, wide);
			// xxx SML_LOG_LINE(TEXT("convertable %d"), b2);
			// xxx SML_LOG_LINE(TEXT(""));

			PrintCodePageInfo(CP_UTF8);
			bool b3 = Ok2Conv(CP_UTF8, 0, 0, narrowStr, wide);
			// xxx SML_LOG_LINE(TEXT("convertable %d"), b3);
			// xxx SML_LOG_LINE(TEXT(""));

		}

		static void Case_Convertable_Japan()
		{

			CHAR narrowStr[] = "---------されたパスが見つかりませ---------";

			PrintCodePageInfo(CP_THREAD_ACP);
			// xxx SML_LOG_LINE(TEXT(""));

			std::wstring wide;

			PrintCodePageInfo(936);
			bool b1 = Ok2Conv(936, 0, 0, narrowStr, wide);
			// xxx SML_LOG_LINE(TEXT("convertable %d"), b1);
			// xxx SML_LOG_LINE(TEXT(""));

			PrintCodePageInfo(932);
			bool b2 = Ok2Conv(932, 0, 0, narrowStr, wide);
			// xxx SML_LOG_LINE(TEXT("convertable %d"), b2);
			// xxx SML_LOG_LINE(TEXT(""));

			PrintCodePageInfo(CP_UTF8);
			bool b3 = Ok2Conv(CP_UTF8, 0, 0, narrowStr, wide);
			// xxx SML_LOG_LINE(TEXT("convertable %d"), b3);
			// xxx SML_LOG_LINE(TEXT(""));
		}


		static void Case_Convertable_Japn()
		{
			WCHAR wideStr[] = L"---------されたパスが見つかりませ---------";
			CHAR narrowStr[] = "---------されたパスが見つかりませ---------";

			PrintCodePageInfo(CP_THREAD_ACP);

			std::string narrow;
			W2N(CP_THREAD_ACP, 0, wideStr, narrow);
			int cmp = memcmp(narrow.c_str(), narrowStr, sizeof(narrowStr));
			assert(0 == cmp);

			std::wstring wide;
			N2W(CP_THREAD_ACP, 0, narrowStr, wide);
			cmp = memcmp(wide.c_str(), wideStr, sizeof(wideStr));
			assert(0 == cmp);
		}


		static bool Ok2Convert(const int * codePadepages, int count, int flags, int destFlags, const std::string & narrow, std::wstring & wide)
		{
			bool ret = false;
			for (int ii = 0; ii < count; ++ ii)
			{
				ret = Ok2Conv(codePadepages[ii], flags, destFlags, narrow, wide);
				if (ret)
				{
					break;
				}
			}

			return ret;
		}

		static bool Ok2Conv(int codePade, int flags, int destFlags, const std::string & narrow, std::wstring & wide)
		{
			std::string narrow2;
			N2N(codePade, flags, narrow, codePade, destFlags, narrow2, wide);
			return narrow.size() == narrow2.size() && 0 == memcmp(narrow.c_str(), narrow2.c_str(), narrow.size());
		}

		static void N2N(int srcCodePage, int srcFlags, const std::string & src, int destCodePage, int destFlags,std::string & dest, std::wstring & wide)
		{
			N2W(srcCodePage, srcFlags, src, wide);
			W2N(destCodePage, destFlags, wide, dest);
		}

		static void W2N(int codePage, int flags, const std::wstring & wide, std::string & narrow)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			int bufferSize = WideCharToMultiByte(
				codePage,//_In_       UINT CodePage,
				flags,//_In_       DWORD dwFlags,
				wide.c_str(),//_In_       LPCWSTR lpWideCharStr,
				wide.size(),//_In_       int cchWideChar,
				nullptr,//_Out_opt_  LPSTR lpMultiByteStr,
				0,//_In_       int cbMultiByte,
				nullptr,//_In_opt_   LPCSTR lpDefaultChar,
				nullptr//_Out_opt_  LPBOOL lpUsedDefaultChar
			);
			if (0 == bufferSize)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				// xxx SML_LOG_LINE(TEXT("Operation failed [function=%s, hr=0x%08x, LastError=%d]."), TEXT("WideCharToMultiByte"), hr, LastError);
				LEAVE_BLOCK(0);
			}
			
			narrow.resize(bufferSize);
			bufferSize = WideCharToMultiByte(
				codePage,//_In_       UINT CodePage,
				flags,//_In_       DWORD dwFlags,
				wide.c_str(),//_In_       LPCWSTR lpWideCharStr,
				wide.size(),//_In_       int cchWideChar,
				&narrow[0],//_Out_opt_  LPSTR lpMultiByteStr,
				narrow.size(),//_In_       int cbMultiByte,
				nullptr,//_In_opt_   LPCSTR lpDefaultChar,
				nullptr//_Out_opt_  LPBOOL lpUsedDefaultChar
			);
			if (0 == bufferSize)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				// xxx SML_LOG_LINE(TEXT("Operation failed [function=%s, hr=0x%08x, LastError=%d]."), TEXT("WideCharToMultiByte"), hr, LastError);
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			//return hr;
		}


		

		static void N2W(int codePage, int flags, const std::string & narrow, std::wstring & wide)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			int bufferSize = MultiByteToWideChar(
				codePage,//_In_       UINT CodePage,
				flags,//_In_       DWORD dwFlags,
				narrow.c_str(),//_In_       LPCSTR lpMultiByteStr,
				narrow.size(),//_In_       int cbMultiByte,
				nullptr,//_Out_opt_  LPWSTR lpWideCharStr,
				0//_In_       int cchWideChar
			);
			if (0 == bufferSize)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				// xxx SML_LOG_LINE(TEXT("Operation failed [function=%s, hr=0x%08x, LastError=%d]."), TEXT("MultiByteToWideChar"), hr, LastError);
				LEAVE_BLOCK(0);
			}

			wide.resize(bufferSize);
			bufferSize = MultiByteToWideChar(
				codePage,//_In_       UINT CodePage,
				flags,//_In_       DWORD dwFlags,
				narrow.c_str(),//_In_       LPCSTR lpMultiByteStr,
				narrow.size(),//_In_       int cbMultiByte,
				&wide[0],//_Out_opt_  LPWSTR lpWideCharStr,
				wide.size()//_In_       int cchWideChar
			);
			if (0 == bufferSize)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				// xxx SML_LOG_LINE(TEXT("Operation failed [function=%s, hr=0x%08x, LastError=%d]."), TEXT("WideCharToMultiByte"), hr, LastError);
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			//return hr;
		}



		static void Case_Conv_China()
		{
			WCHAR wideStr[] = L"---------发觉色戒覅欧塞u给覅公开萨尔u发---------";
			CHAR narrowStr[] = "---------发觉色戒覅欧塞u给覅公开萨尔u发---------";

			PrintCodePageInfo(CP_THREAD_ACP);

			std::string narrow;
			W2N(CP_THREAD_ACP, 0, wideStr, narrow);
			int cmp = memcmp(narrow.c_str(), narrowStr, sizeof(narrowStr));
			assert(0 == cmp);

			std::wstring wide;
			N2W(CP_THREAD_ACP, 0, narrowStr, wide);
			cmp = memcmp(wide.c_str(), wideStr, sizeof(wideStr));
			assert(0 == cmp);
		}


		static void Case_Conv_Japn()
		{
			WCHAR wideStr[] = L"---------されたパスが見つかりませ---------";
			CHAR narrowStr[] = "---------されたパスが見つかりませ---------";

			PrintCodePageInfo(CP_THREAD_ACP);

			std::string narrow;
			W2N(CP_THREAD_ACP, 0, wideStr, narrow);
			int cmp = memcmp(narrow.c_str(), narrowStr, sizeof(narrowStr));
			assert(0 == cmp);

			std::wstring wide;
			N2W(CP_THREAD_ACP, 0, narrowStr, wide);
			cmp = memcmp(wide.c_str(), wideStr, sizeof(wideStr));
			assert(0 == cmp);
		}

		static  BOOL CALLBACK EnumCodePagesProc(
			_In_  LPTSTR lpCodePageString
		)
		{
			// xxx SML_LOG_LINE(TEXT("%s"), lpCodePageString);
			int codePage = _tcstol(lpCodePageString, nullptr, 10);
			if (codePage > CP_THREAD_ACP)
			{
				PrintCodePageInfo(codePage);
				// xxx SML_LOG_LINE(TEXT(""));
			}
			return TRUE;
		}

		static void Case_EnumCodePage(BOOL bInstalledOnly)
		{
			DWORD dwFlags = 0;
			bInstalledOnly ? dwFlags = CP_INSTALLED : dwFlags = CP_SUPPORTED;
			BOOL bEnum = EnumSystemCodePages(
				(CODEPAGE_ENUMPROC)(EnumCodePagesProc),//_In_  CODEPAGE_ENUMPROC lpCodePageEnumProc,
				dwFlags//_In_  DWORD dwFlags
			);
			// xxx SML_LOG_LINE(TEXT(""));
		}

		static HRESULT PrintCodePageInfo(UINT CodePage)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			CPINFOEX CPInfoEx = { 0 };
			BOOL res = GetCPInfoEx(
				CodePage,//_In_   UINT CodePage,
				0,//_In_   DWORD dwFlags,
				&CPInfoEx//,//_Out_  LPCPINFOEX lpCPInfoEx
			);
			if (!res)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				// xxx SML_LOG_LINE(TEXT("Operation failed [function=%s, hr=0x%08x, LastError=%d]."), TEXT("GetCPInfoEx"), hr, LastError);
				LEAVE_BLOCK(0);
			}

			// xxx SML_LOG_LINE(TEXT("CodePageName: %s"), CPInfoEx.CodePageName);
			// xxx SML_LOG_LINE(TEXT("CodePage: %d"), CPInfoEx.CodePage);
			// xxx SML_LOG_LINE(TEXT("MaxCharSize: %d"), CPInfoEx.MaxCharSize);
			for (int ii = 0; ii < MAX_DEFAULTCHAR; ++ ii)
			{
				if (CPInfoEx.DefaultChar[ii] == 0)
				{
					break;
				}
				// xxx SML_LOG_LINE(TEXT("DefaultChar: %c"), (WCHAR)(CPInfoEx.DefaultChar[ii]));
			}
			for (int ii = 0; ii < MAX_LEADBYTES; ++ ii)
			{
				if (CPInfoEx.LeadByte[ii] == 0)
				{
					break;
				}
				// xxx SML_LOG_LINE(TEXT("LeadByte: 0x%02x"), (int)(CPInfoEx.LeadByte[ii]));
			}
			// xxx SML_LOG_LINE(TEXT("UnicodeDefaultChar: %c"), CPInfoEx.UnicodeDefaultChar);
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}

		static void Case_CodePage()
		{
			UINT codePages[] = { CP_ACP, CP_MACCP, CP_OEMCP, CP_THREAD_ACP };
			for (int ii = 0; ii < _countof(codePages); ++ii)
			{
				PrintCodePageInfo(codePages[ii]);
			}
		}


//#define  CPPSTD17
#ifdef  CPPSTD17
		std::codecvt<char16_t, char> * 
#else
		std::codecvt_utf8_utf16<char16_t> *
#endif
			NewCodeCvt()
		{
#ifdef CPPSTD17
			auto ret = new std::codecvt<char16_t, char>();
#else
			auto ret = new std::codecvt_utf8_utf16<char16_t>();
#endif
		}


		static void Case3()
		{
			//SetConsoleOutputCP(65001);
			WCHAR ch[] = L"---------されたパスが見つかりませ---------";
			CHAR ach[] = "---------されたパスが見つかりませ---------";

			fprintf_s(stdout, "output using fprintf_s" "\r\n");
			fprintf_s(stdout, ach);
			fprintf_s(stdout, "\r\n");
			fprintf_s(stdout, "end output using fprintf_s" "\r\n");
			fprintf_s(stdout, "\r\n");

			fwprintf_s(stdout, L"output using fwprintf_s" L"\r\n");
			fwprintf_s(stdout, ch);
			fwprintf_s(stdout, L"\r\n");
			fwprintf_s(stdout, L"end output using fwprintf_s" L"\r\n");
			fwprintf_s(stdout, L"\r\n");


			std::cout << "output using std::cout" << std::endl;
			std::cout << ach << std::endl;
			std::cout << "end output using std::cout" << std::endl;
			std::cout << std::endl;

			//std::wcout.imbue(std::locale(std::wcout.getloc(), new std::codecvt_utf8_utf16<wchar_t>()));
			std::wcout.setstate(ios::goodbit);
			std::wcout << L"output using std::wcout" << std::endl;
			std::wcout << ch << std::endl;
			std::wcout << L"end output using std::wcout" << std::endl;
			std::wcout << std::endl;
		}

		static void Case2()
		{
			//SetConsoleOutputCP(65001);
			WCHAR ch[] = L"+++++++++++++角色u第一改革开放和嘎金额与反思u阿三+++++++++++++";
			CHAR ach[] = "+++++++++++++角色u第一改革开放和嘎金额与反思u阿三+++++++++++++";

			fprintf_s(stdout, "output using fprintf_s" "\r\n");
			fprintf_s(stdout, ach);
			fprintf_s(stdout, "\r\n");
			fprintf_s(stdout, "end output using fprintf_s" "\r\n");
			fprintf_s(stdout, "\r\n");

			fwprintf_s(stdout, L"output using fwprintf_s" L"\r\n");
			fwprintf_s(stdout, ch);
			fwprintf_s(stdout, L"\r\n");
			fwprintf_s(stdout, L"end output using fwprintf_s" L"\r\n");
			fwprintf_s(stdout, L"\r\n");


			std::cout << "output using std::cout" << std::endl;
			std::cout << ach << std::endl;
			std::cout << "end output using std::cout" << std::endl;
			std::cout << std::endl;
			
			//std::wcout.imbue(std::locale(std::wcout.getloc(), new std::codecvt_utf8_utf16<wchar_t>()));
			std::wcout.setstate(ios::goodbit);
			std::wcout << L"output using std::wcout" << std::endl;
			std::wcout << ch << std::endl;
			std::wcout << L"end output using std::wcout" << std::endl;
			std::wcout << std::endl;
		}




		static void Case1(LPCWSTR fileName)
		{
			std::wifstream inf(fileName);
			inf.imbue(std::locale(inf.getloc(), new std::codecvt_utf8_utf16<wchar_t>()));

			wstring line;
			while (std::getline(inf, line))
			{
				// xxx SML_LOG_LINE(TEXT("%s"), line.c_str());
			}
		}
		
		static void Case0(LPCWSTR fileName)
		{
			::SetConsoleOutputCP(65001);
			WCHAR text1[] = L"-----------------------------------------------";
			WCHAR text[] = L"指定されたパスが見つかりません" L"一直以来的绘制粉红色分色法";
			WCHAR text2[] = L"++++++++++++++++++++++++++++++++++++++++++++++";
			std::wofstream of(fileName);



			// xxx SML_LOG_LINE(TEXT("stream locale name is %S"), of.getloc().name().c_str());
			//of.imbue(std::locale(of.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>));
			of.imbue(std::locale(of.getloc(), new std::codecvt_utf8_utf16<wchar_t>()));
			// xxx SML_LOG_LINE(TEXT("stream locale name is %S"), of.getloc().name().c_str());

			of << text1 << std::endl;
			of << text << std::endl;
			of << text2 << std::endl;

			// xxx SML_LOG_LINE(TEXT("stream locale name is %S"), std::wcout.getloc().name().c_str());
			std::wcout.imbue(std::locale(std::wcout.getloc(), new std::codecvt_utf8_utf16<wchar_t>()));
			// xxx SML_LOG_LINE(TEXT("stream locale name is %S"), std::wcout.getloc().name().c_str());

			std::wcout << text1 << std::endl;
			std::wcout << "Chinese - 一直以来的绘制粉红色分色法" << std::endl;
			std::wcout << "Japan - 指定されたパスが見つかりません" << std::endl;
			std::wcout << text2 << std::endl;
		}

		
	};
}