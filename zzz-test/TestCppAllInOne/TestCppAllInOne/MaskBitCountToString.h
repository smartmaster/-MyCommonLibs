#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <string>

namespace SmartLib
{
	class CMaskBitCountToString
	{
	public:
		static void MaskBitCountToString(LPCWSTR strbitcount, CString& straddr)
		{
			int bitcount = wcstol(strbitcount, nullptr, 10);
			MaskBitCountToString(bitcount, straddr);
		}

		static void MaskBitCountToString(LPCSTR strbitcount, CString& straddr)
		{
			int bitcount = strtol(strbitcount, nullptr, 10);
			MaskBitCountToString(bitcount, straddr);
		}

		static void MaskBitCountToString(int bitcount, CString& straddr)
		{
			BYTE bb[9];
			ZeroMemory(bb, sizeof(bb));
			for (int jj = 1; jj < 8; ++jj)
			{
				bb[jj] = ((int)bb[jj - 1] | (1 << (8 - jj)));
			}


			BYTE ip[64];
			ZeroMemory(ip, sizeof(ip));
			int ii = 0;
			for (; ii < bitcount / 8; ++ii)
			{
				ip[ii] = 255;
			}
			ip[ii] = bb[bitcount % 8];

			for (int ii = 0; ii < 4; ++ii)
			{
				if (ii + 1 == 4)
				{
					straddr.AppendFormat(TEXT("%d"), (int)ip[ii]);
				}
				else
				{
					straddr.AppendFormat(TEXT("%d."), (int)ip[ii]);
				}
			}

		}
	};
}