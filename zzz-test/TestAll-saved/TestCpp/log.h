#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


namespace SML
{
	struct Logger 
	{
	public:
		static void Log(const TCHAR * format, ...)
		{
			va_list args;
			va_start(args, format);
			_vftprintf_s(stdout, format, args);
			va_end(args);
		}

		static void LogA(const char * format, ...)
		{
			va_list args;
			va_start(args, format);
			vfprintf_s(stdout, format, args);
			va_end(args);
		}

	};

#define  LOG_LINE(format, ...) ::SML::Logger::Log(format TEXT("\r\n"), __VA_ARGS__)
#define  LOG(format, ...) ::SML::Logger::Log(format, __VA_ARGS__)


#define  LOG_LINE_A(format, ...) ::SML::Logger::LogA(format "\r\n", __VA_ARGS__)
#define  LOG_A(format, ...) ::SML::Logger::LogA(format, __VA_ARGS__)
}