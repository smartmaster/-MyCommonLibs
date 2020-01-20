#pragma once

#include <Windows.h>
#include <tchar.h>

#include "log.h"

namespace TestCpp
{
	class CWin32Helper
	{
	public:

		static VOID RemoveRN(LPTSTR lpMsgBuf, ULONG dwMax)
		{
			ULONG ii = 0;
			while (lpMsgBuf && lpMsgBuf[ii] && ii < dwMax)
			{
				if (lpMsgBuf[ii] == TEXT('\n') || lpMsgBuf[ii] == TEXT('\r'))
				{
					lpMsgBuf[ii] = 0;
					break;
				}
				++ii;
			}
		}


		static void GetWin32ErrorString(DWORD dwErrorCode, LPTSTR buffer, DWORD bufferSize)
		{
			DWORD dwFormatFlags = 
				FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_FROM_SYSTEM;

			
			FormatMessage(
				dwFormatFlags,
				nullptr,
				dwErrorCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				buffer,
				bufferSize, 
				NULL);


			RemoveRN(buffer, bufferSize);
		}
	};


	class CWin32HelperTest
	{
	public:
		static void Case0()
		{
			TCHAR buffer[256] = { 0 };
			DWORD dwErrorCode = 2;
			CWin32Helper::GetWin32ErrorString(dwErrorCode, buffer, _countof(buffer));
			LOG_LINE(TEXT("%s"), buffer);


			buffer[0] = { 0 };
			dwErrorCode = HRESULT_FROM_WIN32(183);
			CWin32Helper::GetWin32ErrorString(dwErrorCode, buffer, _countof(buffer));
			LOG_LINE(TEXT("%s"), buffer);


			buffer[0] = { 0 };
			dwErrorCode = 0X80010105;
			CWin32Helper::GetWin32ErrorString(dwErrorCode, buffer, _countof(buffer));
			LOG_LINE(TEXT("%s"), buffer);
		}
	};
}