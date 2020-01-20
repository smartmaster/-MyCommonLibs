#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <lmerr.h>

#include <memory>
#include <iostream>

//#include "print_line.h"

namespace SmartLib
{
	struct APIStatus;
	typedef std::shared_ptr<SmartLib::APIStatus> APIStatusPtr;

#define  MAKE_API_STATUS_PTR(inner) std::make_shared<SmartLib::APIStatus>(TEXT(__FUNCTION__), TEXT(__FILE__), __LINE__, inner)
#define  API_STATUS_SUCCEEDED(ret) ((nullptr == ret) || SUCCEEDED(ret->m_hr))


	struct APIStatus
	{
	public:
		HRESULT m_hr{ S_OK };
		HRESULT m_hrEx{ S_OK };
		CString m_message;
		CString m_messageEx;
		CString m_messageLocal;
		CString m_messageLocalEx;
		ULONG m_data{ 0 };
		ULONG m_dataEx{ 0 };
		CString m_dataString;
		CONST CString m_module;
		CONST CString m_function;
		CONST CString m_file;
		CONST INT m_line{ -1 };
		CONST INT m_index{ 0 };
		std::shared_ptr<APIStatus> m_Inner;

	public:
		APIStatus(LPCTSTR function, LPCTSTR file, INT line, std::shared_ptr<APIStatus> inner) :
			m_module(GetCurrentModuleName()),
			m_function(function),
			m_file(file),
			m_line(line),
			m_index(inner ? inner->m_index + 1 : 0),
			m_Inner(inner)
		{
		}

		~APIStatus()
		{
			//PRINT_LINE(L"dtor [%d]", m_Index);
		}

		VOID ToString(CString & str, BOOL bRecursive)
		{
			str.AppendFormat(TEXT("hr=[%d(0X%08X), %d(0X%08X)]") TEXT("\r\n"), m_hr, m_hr, m_hrEx, m_hrEx);
			str.AppendFormat(TEXT("Message=[$@%s $@%s $@]") TEXT("\r\n"), m_message.GetString(), m_messageEx.GetString());
			str.AppendFormat(TEXT("MessageLocal=[$@%s $@%s $@]") TEXT("\r\n"), m_messageLocal.GetString(), m_messageLocalEx.GetString());
			str.AppendFormat(TEXT("Data=[%d(0X%08X), %d(0X%08X), $@%s $@]") TEXT("\r\n"), m_data, m_data, m_dataEx, m_dataEx, m_dataString.GetString());
			str.AppendFormat(TEXT("@[%s, %s, %s, %d], Index=[%d]") TEXT("\r\n"), m_module.GetString(), m_function.GetString(), m_file.GetString(), m_line, m_index);
			str.Append(TEXT("\r\n") TEXT("\r\n"));
			if (bRecursive && nullptr != m_Inner)
			{
				m_Inner->ToString(str, bRecursive); //recursive call
			}
		}

		VOID SetStatus(HRESULT hr, LPCTSTR message, LPCTSTR format, ...)
		{
			//////////////////////////////////////////////////////////////////////////
			m_hr = hr;

			//////////////////////////////////////////////////////////////////////////
			if (message == nullptr)
			{
				HMODULE hModule = NULL;
				DWORD dwFormatFlags = /*FORMAT_MESSAGE_ALLOCATE_BUFFER |*/
					FORMAT_MESSAGE_IGNORE_INSERTS |
					FORMAT_MESSAGE_FROM_SYSTEM;

#define __USE_NETMSG__ 0

#if __USE_NETMSG__
				if ((LONG)(hr) > 0 && hr >= NERR_BASE && hr <= MAX_NERR)
				{
					hModule = LoadLibraryEx(
						TEXT("netmsg.dll"),
						NULL,
						LOAD_LIBRARY_AS_DATAFILE
						);
					if (NULL != hModule)
					{
						dwFormatFlags |= FORMAT_MESSAGE_FROM_HMODULE;
					}
				}
#endif

				TCHAR lpMsgBuf[1024] = {0};
				::FormatMessage(
					dwFormatFlags,
					hModule,
					hr,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					lpMsgBuf,
					_countof(lpMsgBuf), NULL);

#if __USE_NETMSG__
				if (NULL != hModule)
				{
					FreeLibrary(hModule);
					hModule = NULL;
				}
#endif
				RemoveRN(lpMsgBuf, (ULONG)(-1));
				m_message = lpMsgBuf;
				//::LocalFree(lpMsgBuf);
			}
			else
			{
				m_message = message;
			}

			//////////////////////////////////////////////////////////////////////////
			va_list args;
			va_start(args, format);
			m_messageEx.FormatV(format ? format : TEXT(""), args);
			va_end(args);
		}

		private:
			static VOID RemoveRN(LPTSTR lpMsgBuf, ULONG dwMax)
			{
				ULONG ii = 0;
				while (lpMsgBuf && lpMsgBuf[ii] && ii < dwMax)
				{
					if (lpMsgBuf[ii] == TEXT('\n') || lpMsgBuf[ii] == TEXT('\r'))
					{
						lpMsgBuf[ii] = 0;
						break;
					}
					++ii;
				}
			}

			static LPCTSTR GetCurrentModuleName()
			{
				static HMODULE s_hModule = NULL;
				static TCHAR s_ModuleName[MAX_PATH + MAX_PATH] = { 0 };
				static CComAutoCriticalSection s_CritSec;

				HRESULT hr = S_OK;

				if (NULL == s_hModule)
				{
					CComCritSecLock<CComAutoCriticalSection> Lock(s_CritSec, FALSE);
					Lock.Lock();
					if (NULL == s_hModule) //check again
					{
						MEMORY_BASIC_INFORMATION mbi = { 0 };
						SIZE_T Size = VirtualQuery(
							(LPVOID)(GetCurrentModuleName),//_In_opt_  LPCVOID lpAddress,
							&mbi,//_Out_     PMEMORY_BASIC_INFORMATION lpBuffer,
							sizeof(MEMORY_BASIC_INFORMATION)//_In_      SIZE_T dwLength
							);
						if (Size)
						{
							HMODULE hModuleTemp = (HMODULE)(mbi.AllocationBase);

							//////////////////////////////////////////////////////////////////////////
							TCHAR ExeName[MAX_PATH] = { 0 };
							GetModuleFileName(
								NULL,//_In_opt_  HMODULE hModule,
								ExeName,//_Out_     LPTSTR lpFilename,
								_countof(ExeName)//_In_      DWORD nSize
								);
							LPCTSTR pExe = _tcsrchr(ExeName, TEXT('\\'));
							if (pExe)
							{
								++pExe;
							}

							//////////////////////////////////////////////////////////////////////////
							TCHAR ModName[MAX_PATH] = { 0 };
							GetModuleFileName(
								hModuleTemp,//_In_opt_  HMODULE hModule,
								ModName,//_Out_     LPTSTR lpFilename,
								_countof(ModName)//_In_      DWORD nSize
								);
							LPCTSTR pMod = _tcsrchr(ModName, TEXT('\\'));
							if (pMod)
							{
								++pMod;
							}

							//////////////////////////////////////////////////////////////////////////
							if (pExe)
							{
								_tcscat_s(s_ModuleName, pExe);
								_tcscat_s(s_ModuleName, TEXT("!"));
							}
							if (pMod)
							{
								_tcscat_s(s_ModuleName, pMod);
							}

							s_hModule = hModuleTemp; //it is the last step to assign s_hModule value
						}
					}
				}


				return s_ModuleName;
			}
	};


	struct APIErrorTest
	{
	private:
		static APIStatusPtr TestProc()
		{
			CString tmp;
			APIStatusPtr ret;
			for (int ii = 0; ii < 5; ++ii)
			{
				ret = MAKE_API_STATUS_PTR(ret);
				ret->SetStatus(HRESULT_FROM_WIN32(NERR_ServerNotStarted + ii), nullptr, TEXT("index=[%d], name=[%s]"), ii, TEXT("<null>"));

				BOOL bSucceeded = API_STATUS_SUCCEEDED(ret);
				if (!bSucceeded)
				{
					tmp = ret->m_message;
				}
			}


			for (int ii = 0; ii < 5; ++ii)
			{
				ret = MAKE_API_STATUS_PTR(ret);
				ret->SetStatus(HRESULT_FROM_WIN32(NERR_ServerNotStarted + ii), nullptr, nullptr);

				BOOL bSucceeded = API_STATUS_SUCCEEDED(ret);
				if (!bSucceeded)
				{
					tmp = ret->m_message;
				}
			}

			//CString text;
			//ret->ToString(text, TRUE); 

			return ret;
		}

	public:
		static APIStatusPtr Test()
		{
			APIStatusPtr statusPtr = TestProc();
			if (!API_STATUS_SUCCEEDED(statusPtr))
			{
				CString strErrorStack;
				statusPtr->ToString(strErrorStack, TRUE);
			}
			return statusPtr;
		}
	};

} //namespace SmartLib