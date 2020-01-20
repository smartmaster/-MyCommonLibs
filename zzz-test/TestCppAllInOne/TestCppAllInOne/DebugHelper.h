#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "CodeBlock.h"
#include "log.h"

//////////////////////////////////////////////////////////////////////////
#ifndef SML_BEGIN_BLOCK
//#define SML_BEGIN_BLOCK


#define SML_BEGIN_BLOCK(level_number)			do{	int	sml_1qaz_2wsx_3edc_4rfv_ ## level_number = (level_number) + (__LINE__) - (level_number);
#define SML_LEAVE_BLOCK(level_number)			{(sml_1qaz_2wsx_3edc_4rfv_ ## level_number);}	break;
#define SML_END_BLOCK(level_number)				{(sml_1qaz_2wsx_3edc_4rfv_ ## level_number);}	}while(0);

#define BEGIN_BLOCK SML_BEGIN_BLOCK
#define LEAVE_BLOCK SML_LEAVE_BLOCK
#define END_BLOCK SML_END_BLOCK

#endif // !SML_BEGIN_BLOCK

namespace SmartLib
{
	class DebugHelper
	{
	public:
		static void LoopWaitForDebug(int loopCount, int sleepMilSec, bool toBreak)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;


			bool toDebug = false;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			TCHAR fileName[1024] = { 0 };
			DWORD dwgmfn = ::GetModuleFileName(
				nullptr,//_In_opt_  HMODULE hModule,
				fileName,//_Out_     LPTSTR lpFilename,
				_countof(fileName)//,//_In_      DWORD nSize
			);
			if (0 == dwgmfn)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("[%s] failed with [%d, 0x%08x]"), TEXT("GetModuleFileName"), LastError, hr);
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			static const TCHAR DEBUG_EXT[] = TEXT(".debug.txt");
			_tcscat_s(fileName, DEBUG_EXT);
			//if exists <full path>\this.exe.debug.txt
			DWORD attr = ::GetFileAttributes(
				fileName//_In_  LPCTSTR lpFileName
			);
			if ((INVALID_FILE_ATTRIBUTES != attr) && 0 == (FILE_ATTRIBUTE_DIRECTORY & attr))
			{
				toDebug = true;
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			//if exists c:\this.exe.debug.txt
			--dwgmfn;
			while (dwgmfn >= 2)
			{
				if (TEXT('\\') == fileName[dwgmfn])
				{
					break;
				}
				--dwgmfn;
			}

			if (dwgmfn >= 2)
			{
				fileName[--dwgmfn] = TEXT(':');
				fileName[--dwgmfn] = TEXT('c');
				attr = ::GetFileAttributes(
					fileName + dwgmfn//_In_  LPCTSTR lpFileName
				);
				if ((INVALID_FILE_ATTRIBUTES != attr) && 0 == (FILE_ATTRIBUTE_DIRECTORY & attr))
				{
					toDebug = true;
					LEAVE_BLOCK(0);
				}
			}


			//////////////////////////////////////////////////////////////////////////
			static const TCHAR smDebug[] = TEXT("c:\\debug.debug.txt");
			attr = ::GetFileAttributes(
				smDebug//_In_  LPCTSTR lpFileName
			);
			if ((INVALID_FILE_ATTRIBUTES != attr) && 0 == (FILE_ATTRIBUTE_DIRECTORY & attr))
			{
				toDebug = true;
				LEAVE_BLOCK(0);
			}

			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);


			if (toDebug)
			{
				if (toBreak)
				{
					::DebugBreak();
				}
				else
				{
					for (int ii = 0; ii < loopCount; ++ ii)
					{
						::Sleep(sleepMilSec);
					}
				}
			}
			
			//return hr;
		}
	};
}