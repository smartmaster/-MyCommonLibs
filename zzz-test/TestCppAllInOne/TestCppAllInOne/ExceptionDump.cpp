#include "pch.h"
#include  <io.h>
#include <Windows.h>
#include <Dbghelp.h>
#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <string.h>

//#include "ExceptionDump.h"

BOOL g_bFullDump = FALSE;

typedef BOOL(WINAPI * FN_MINIDUMPWRITEDUMP)(
   IN HANDLE hProcess, 
   IN DWORD ProcessId, 
   IN HANDLE hFile, 
   IN MINIDUMP_TYPE DumpType, 
   IN CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, OPTIONAL
   IN CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, OPTIONAL
   IN CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam OPTIONAL
   );

int GetMiniDumpFileName(CHAR * pszPathName, int iPathNameSize)
{
   int   iCount = 0;
   DWORD dwPid = GetCurrentProcessId(); //<sonmi01>2014-1-2 #add pid for easy debug


   char szModuleFilename[MAX_PATH];
   GetModuleFileNameA(NULL, szModuleFilename, MAX_PATH);

   _snprintf_s(pszPathName, iPathNameSize, _TRUNCATE, "%s.%d.%02d.dmp", szModuleFilename, dwPid, iCount++);
   while (_access(pszPathName, 0) == 0 &&iCount < 99)
   {
      _snprintf_s(pszPathName, iPathNameSize, _TRUNCATE, "%s.%d.%02d.dmp", szModuleFilename, dwPid, iCount++);
   }

   return 0;
}

LONG GenerateMiniDump(PEXCEPTION_POINTERS pExceptionInfo)
{
   CHAR szFileName[MAX_PATH];
   LONG retval = EXCEPTION_CONTINUE_SEARCH;
   HMODULE hDll = NULL;
   FN_MINIDUMPWRITEDUMP fnMiniDumpWriteDump = NULL;
   HANDLE hFile = NULL;
   MINIDUMP_EXCEPTION_INFORMATION ExInfo;

   GetMiniDumpFileName(szFileName, MAX_PATH);

   hDll = LoadLibraryA("DBGHELP.DLL");
   if (NULL != hDll)
   {
      fnMiniDumpWriteDump = (FN_MINIDUMPWRITEDUMP)GetProcAddress(hDll, "MiniDumpWriteDump");

      if (NULL != fnMiniDumpWriteDump)
      {
         hFile = CreateFileA(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

         if (hFile != INVALID_HANDLE_VALUE)
         {
            ExInfo.ThreadId = GetCurrentThreadId();
            ExInfo.ExceptionPointers = pExceptionInfo;
            ExInfo.ClientPointers = FALSE;

            // Write the dump
            if (fnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL))
            {
               retval = EXCEPTION_EXECUTE_HANDLER;
            }
            CloseHandle(hFile);
         }
      }
   }

   return retval;
}

LONG GenerateFullDump(PEXCEPTION_POINTERS pExceptionInfo)
{
   CHAR szFileName[MAX_PATH];
   LONG retval = EXCEPTION_CONTINUE_SEARCH;
   HMODULE hDll = NULL;
   FN_MINIDUMPWRITEDUMP fnMiniDumpWriteDump = NULL;
   HANDLE hFile = NULL;
   MINIDUMP_EXCEPTION_INFORMATION ExInfo;

   GetMiniDumpFileName(szFileName, MAX_PATH);

   hDll = LoadLibraryA("DBGHELP.DLL");
   if (NULL != hDll)
   {
      fnMiniDumpWriteDump = (FN_MINIDUMPWRITEDUMP)GetProcAddress(hDll, "MiniDumpWriteDump");

      if (NULL != fnMiniDumpWriteDump)
      {
         hFile = CreateFileA(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

         if (hFile != INVALID_HANDLE_VALUE)
         {
            ExInfo.ThreadId = GetCurrentThreadId();
            ExInfo.ExceptionPointers = pExceptionInfo;
            ExInfo.ClientPointers = FALSE;

            // Write the dump
            if (fnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpWithFullMemory, &ExInfo, NULL, NULL))
            {
               retval = EXCEPTION_EXECUTE_HANDLER;
            }
            CloseHandle(hFile);
         }
      }
   }

   return retval;
}

LONG ExceptionHandler(
                      IN EXCEPTION_POINTERS * ExInfo
                      )
{
   static BOOL bInExceptionHandler = FALSE;

   if( !bInExceptionHandler )
   {
      bInExceptionHandler = TRUE;
   }
   else
   {
      return (EXCEPTION_CONTINUE_SEARCH);
   }

   if( g_bFullDump )
   {
      GenerateFullDump(ExInfo);
   }
   else
   {
      GenerateMiniDump(ExInfo);
   }

   bInExceptionHandler = FALSE;

   return (EXCEPTION_CONTINUE_SEARCH);
}

void InvalidParameterHandler(const wchar_t * expression, 
                             const wchar_t * function, 
                             const wchar_t * file, 
                             unsigned int line, 
                             uintptr_t pReserved)
{
   // Raise an exception to retrieve the Exception Handler
   RaiseException(EXCEPTION_ACCESS_VIOLATION, EXCEPTION_NONCONTINUABLE, 0, NULL);
}

void ExceptionDump_SetExceptionHandler(BOOL bFullDump)
{
   g_bFullDump = bFullDump;
   _set_invalid_parameter_handler((_invalid_parameter_handler)InvalidParameterHandler);

   SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ExceptionHandler);
}