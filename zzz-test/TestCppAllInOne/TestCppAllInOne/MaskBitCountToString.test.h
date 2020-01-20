#pragma once
#include "MaskBitCountToString.h"
#include "log.h"

namespace SmartLib
{
	class MaskBitCountToString
	{
	public:
		static void Case0()
		{
			for (int ii = 1; ii <= 32; ++ii)
			{
				CString str;
				CMaskBitCountToString::MaskBitCountToString(ii, str);
				SML_LOG_LINE(TEXT("%02d - %s"), ii, str.GetString());
			}
		}
	};
}