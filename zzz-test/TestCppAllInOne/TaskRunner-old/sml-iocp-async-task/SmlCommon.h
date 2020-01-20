#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


namespace SmartLib
{
	struct BUFFER_SIZE
	{
		constexpr static const long _IO_BUFFER_SIZE{ 256 * 1024 };
		constexpr static const long _CIRCULAR_BUFFER_SIZE{ 2 * 1024 * 1024 };
	};

	struct Disposer
	{
	public:
		template<typename T>
		static void DeletePtr(T*& ptr)
		{
			if (ptr)
			{
				delete ptr;
				ptr = nullptr;
			}
		}
	};

	struct CBDataHeader
	{
		long version;
		long flag;
		long command;
		long dataLen;
		LARGE_INTEGER offset;
		LARGE_INTEGER source;
		LARGE_INTEGER destination;

		CBDataHeader()
		{
			ZeroMemory(this, sizeof(CBDataHeader));
		}
	};
}