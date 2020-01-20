#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "DebugLog.h"

namespace SmartLib
{
	class CPrintCommandLine
	{
	public:
		static void PintLine(int arc, TCHAR ** argv)
		{
			for (int ii = 0; ii < arc; ++ ii)
			{
				LOG_LINE_T(TEXT("%s"), argv[ii]);
			}
		}
	};
}