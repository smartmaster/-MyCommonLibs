#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "CodeBlock.h"


namespace SmartLib
{
	class IFileOp
	{
	public:
		virtual void OnFile(LPCTSTR file) = 0;
	};

	class CFindFile
	{
	public:

		static void Case0()
		{
			LPCTSTR fileName = L"\\\\10.57.54.61\\sss-root\\sss-data-lv\\hellocpp\\CMake*.txt";
			FindFileEx(fileName, nullptr);
			FindFile(fileName, nullptr);

			LPCTSTR fileName2 = L"\\\\10.57.54.61\\sss-root\\sss-data-lv\\hellocpp\\cmake*.txt";
			FindFileEx(fileName2, nullptr);
			FindFile(fileName2, nullptr);
			
		}

		static void FindFileEx(LPCTSTR filePattern, IFileOp* fileOp)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			HANDLE hfind = INVALID_HANDLE_VALUE;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			WIN32_FIND_DATA FindFileData = { 0 };
			hfind = ::FindFirstFileEx(
				filePattern,//_In_        LPCTSTR lpFileName,
				FindExInfoStandard,//_In_        FINDEX_INFO_LEVELS fInfoLevelId,
				&FindFileData,//_Out_       LPVOID lpFindFileData,
				FindExSearchNameMatch,//_In_        FINDEX_SEARCH_OPS fSearchOp,
				nullptr,//_Reserved_  LPVOID lpSearchFilter,
				FIND_FIRST_EX_LARGE_FETCH//,//_In_        DWORD dwAdditionalFlags
			);

			;
			if (INVALID_HANDLE_VALUE == hfind)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				LEAVE_BLOCK(0);
			}

			if (fileOp)
			{
				fileOp->OnFile(FindFileData.cFileName);
			}

			for (;;)
			{
				BOOL fnf = ::FindNextFile(hfind, &FindFileData);
				if (!fnf)
				{
					break;
				}
				if (fileOp)
				{
					fileOp->OnFile(FindFileData.cFileName);
				}
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);


			if (INVALID_HANDLE_VALUE != hfind)
			{
				::FindClose(hfind);
				hfind = INVALID_HANDLE_VALUE;
			}


			//return hr;

		}


		static void FindFile(LPCTSTR filePattern, IFileOp* fileOp)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			HANDLE hfind = INVALID_HANDLE_VALUE;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			WIN32_FIND_DATA FindFileData = { 0 };
			hfind = ::FindFirstFile(
				filePattern,//_In_   LPCTSTR lpFileName,
				&FindFileData//_Out_  LPWIN32_FIND_DATA lpFindFileData
			);
			if (INVALID_HANDLE_VALUE == hfind)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				LEAVE_BLOCK(0);
			}

			if (fileOp)
			{
				fileOp->OnFile(FindFileData.cFileName);
			}

			for (;;)
			{
				BOOL fnf = ::FindNextFile(hfind, &FindFileData);
				if (!fnf)
				{
					break;
				}
				if (fileOp)
				{
					fileOp->OnFile(FindFileData.cFileName);
				}
			}
			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			

			if (INVALID_HANDLE_VALUE != hfind)
			{
				::FindClose(hfind);
				hfind = INVALID_HANDLE_VALUE;
			}


			//return hr;

		}
	};
}