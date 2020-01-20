#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <string>
#include <vector>

#include "CmmonDef.h"

namespace SmartLib
{

	class StringHelper
	{
	public:
		static CString& ToString(CONST GUID & guid, CString & str)
		{
			CONST INT BUFFER_SIZE = 128;
			LPTSTR buffer = str.GetBuffer(BUFFER_SIZE);
			StringFromGUID2(guid, buffer, BUFFER_SIZE);
			str.ReleaseBuffer();
			return str;
		}

		static CString& ToString(CONST FILETIME & ft, CString & str)
		{
			SYSTEMTIME st = { 0 };
			FileTimeToSystemTime(&ft, &st);
			str.Format(TEXT("%04d-%02d-%02d %02d:%02d:%02d-%03d"),
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds
			);
			return str;
		}

		static LPCTSTR ToString(VSS_COMPONENT_TYPE ct)
		{
			switch (ct)
			{
			case VSS_CT_UNDEFINED:
				return SML_WSTRING_OF(VSS_CT_UNDEFINED);
			case VSS_CT_DATABASE:
				return SML_WSTRING_OF(VSS_CT_DATABASE);
			case VSS_CT_FILEGROUP:
				return SML_WSTRING_OF(VSS_CT_FILEGROUP);
			default:
				return SML_WSTRING_OF(<null>);
			}

		}


		static LPCTSTR ToString(VSS_BACKUP_TYPE bt)
		{
			switch (bt)
			{
			case VSS_BT_UNDEFINED:
				return SML_WSTRING_OF(VSS_BT_UNDEFINED);
			case VSS_BT_FULL:
				return SML_WSTRING_OF(VSS_BT_FULL);
			case VSS_BT_INCREMENTAL:
				return SML_WSTRING_OF(VSS_BT_INCREMENTAL);
			case VSS_BT_DIFFERENTIAL:
				return SML_WSTRING_OF(VSS_BT_DIFFERENTIAL);
			case VSS_BT_LOG:
				return SML_WSTRING_OF(VSS_BT_LOG);
			case VSS_BT_COPY:
				return SML_WSTRING_OF(VSS_BT_COPY);
			case VSS_BT_OTHER:
				return SML_WSTRING_OF(VSS_BT_OTHER);
			default:
				return SML_WSTRING_OF(<null>);
			}
		}

		static LPCTSTR ToString(VSS_APPLICATION_LEVEL applevel)
		{
			switch (applevel)
			{
			case VSS_APP_UNKNOWN:
				return SML_WSTRING_OF(VSS_APP_UNKNOWN);
			case VSS_APP_SYSTEM:
				return SML_WSTRING_OF(VSS_APP_SYSTEM);
			case VSS_APP_BACK_END:
				return SML_WSTRING_OF(VSS_APP_BACK_END);
			case VSS_APP_FRONT_END:
				return SML_WSTRING_OF(VSS_APP_FRONT_END);
			case VSS_APP_SYSTEM_RM:
				return SML_WSTRING_OF(VSS_APP_SYSTEM_RM);
			case VSS_APP_AUTO:
				return SML_WSTRING_OF(VSS_APP_AUTO);
			default:
				return SML_WSTRING_OF(<null>);
			}
		}

		static void StringArrayToVector(LPCWSTR strarray, std::vector<CString> & vec)
		{

		}
	};
}