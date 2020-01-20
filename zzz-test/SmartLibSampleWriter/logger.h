#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


namespace SmartLib
{
	class ILogger
	{
	public:
		virtual void Log(LPCTSTR message, ...) const = 0;
	};

	class Logger : ILogger
	{
	public:
		virtual void Log(LPCTSTR message, ...)  const override
		{
			CString strMessage;
			SYSTEMTIME st{ 0 };
			GetLocalTime(&st);
			strMessage.Format(TEXT("[%04d-%02d-%02d %02d:%02d:%02d-%03d] [%04d.%04d x%04x.x%04x] "), 
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				GetCurrentProcessId(), GetCurrentThreadId(), GetCurrentProcessId(), GetCurrentThreadId()
			);

			va_list valist;
			va_start(valist, message);
			strMessage.AppendFormatV(message, valist);
			va_end(valist);

			wprintf(TEXT("%s") TEXT("\r\n"), strMessage.GetString());
			fflush(stdout);
		}

	public:
		static const ILogger& LoggerInstance()
		{
			static const Logger logger;
			return logger;
		}
	};
}