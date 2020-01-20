#pragma once

#include <tchar.h>
#include <atlbase.h>
#include <windows.h>

namespace SmartLib
{
	enum class ContextFlags : ULONG
	{
		Unknown = 0,
		Max = ULONG_MAX;
	};

	enum class ContextEnums : ULONG
	{
		Unknown = 0,
		Max = ULONG_MAX;
	};

	struct Context
	{
		LPCVOID in;
		ULONG inFlags;
		ULONG inEnum;

		LPVOID out;
		ULONG outFlags;
		ULONG outEnum;
	};
} //end namespace SmartLib


