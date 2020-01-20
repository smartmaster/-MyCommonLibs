#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include <vector>
#include <string>

#include "CodeBlock.h"
#include "log.h"

namespace SmartLib
{
	class CHCPPCreator
	{
		static HRESULT CreateAndWrite(LPCTSTR fileName, const std::vector<std::string>& lines)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			HANDLE hFile = INVALID_HANDLE_VALUE; /***CLEANUP***/

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			hFile = ::CreateFile(
				fileName,//_In_      LPCTSTR lpFileName,
				GENERIC_WRITE,//_In_      DWORD dwDesiredAccess,
				0,//_In_      DWORD dwShareMode,
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				CREATE_NEW,//_In_      DWORD dwCreationDisposition,
				0,//_In_      DWORD dwFlagsAndAttributes,
				nullptr//,//_In_opt_  HANDLE hTemplateFile
			);
			if (INVALID_HANDLE_VALUE == hFile)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("Failed to CreateFile [%s], LastError=[%d], hr=[0x%08x]"), fileName, LastError, hr);
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			for (const auto& line : lines)
			{
				DWORD bytesWritten = 0;
				BOOL bw = ::WriteFile(hFile, line.c_str(), line.size(), &bytesWritten, nullptr);
				if (!bw)
				{
					LastError = ::GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					SML_LOG_LINE(TEXT("Failed to WriteFile [%s], LastError=[%d], hr=[0x%08x]"), fileName, LastError, hr);
					break;
				}
			}
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);


			if (INVALID_HANDLE_VALUE != hFile)
			{
				::CloseHandle(hFile);
				hFile = INVALID_HANDLE_VALUE;
			}

			return hr;
		}
	public:
		static HRESULT Create(LPCTSTR dir, LPCTSTR fileName)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			std::wstring strfile = dir;
			strfile += fileName;
			strfile += TEXT(".h");
			std::vector<std::string> lines_h =
			{
				"#pragma once" "\r\n"
			};
			hr = CreateAndWrite(strfile.c_str(), lines_h);
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			strfile = dir;
			strfile += fileName;
			strfile += TEXT(".cpp");
			std::vector<std::string> lines_cpp =
			{
				"#include \"pch.h\"" "\r\n",
			};
			//#include "pch.h"
			//#include "HCPPCreate.h"
			std::string temp = "#include \"";
			temp += CStringA{fileName}.GetString();
			temp += ".h\"" "\r\n";
			lines_cpp.push_back(std::move(temp));
			hr = CreateAndWrite(strfile.c_str(), lines_cpp);
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			strfile = dir;
			strfile += fileName;
			strfile += TEXT(".test.h");
			std::vector<std::string> lines_test_h =
			{
				"#pragma once" "\r\n",
			};
			//#pragma once
			//#include "HCPPCreate.h"
			temp = "#include \"";
			temp += CStringA{ fileName }.GetString();
			temp += ".h\""  "\r\n";
			lines_test_h.push_back(std::move(temp));
			hr = CreateAndWrite(strfile.c_str(), lines_test_h);
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			strfile = dir;
			strfile += fileName;
			strfile += TEXT(".test.cpp");
			std::vector<std::string> lines_test_cpp =
			{
				"#include \"pch.h\"" "\r\n",
			};
			//#pragma once
			//#include "HCPPCreate.h"
			temp = "#include \"";
			temp += CStringA{ fileName }.GetString();
			temp += ".test.h\""  "\r\n";
			lines_test_cpp.push_back(std::move(temp));
			hr = CreateAndWrite(strfile.c_str(), lines_test_cpp);
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}
			
			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}
	};
}