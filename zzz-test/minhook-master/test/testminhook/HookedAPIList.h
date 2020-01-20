#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <map>
#include <string>

#include "../../include/MinHook.h"
#include "..\..\..\TestCppAllInOne\TestCppAllInOne\CodeBlock.h"
#include "..\..\..\TestCppAllInOne\TestCppAllInOne\log.h"


namespace SmartLib
{
    class ConvHelper
    {
    private:
        template<typename T>
        static LPCTSTR ConvToString(LPCTSTR prefix, T value, std::wstring& str) //assume T is integer
        {
            str.clear();
            if (prefix)
            {
                str = prefix;
            }
            return str.c_str();
        }

    public:
        template<>
        static LPCTSTR ConvToString(LPCTSTR prefix, ULONG value, std::wstring& str) //assume T is integer
        {
            str.clear();
            if (prefix)
            {
                str = prefix;
            }

            TCHAR buffer[128] = { 0 };
            _sntprintf_s(buffer, _countof(buffer), TEXT("%u"), value);
            str += buffer;
            return str.c_str();
        }


        template<>
        static LPCTSTR ConvToString(LPCTSTR prefix, LONG value, std::wstring& str) //assume T is integer
        {
            str.clear();
            if (prefix)
            {
                str = prefix;
            }

            TCHAR buffer[128] = { 0 };
            _sntprintf_s(buffer, _countof(buffer), TEXT("%d"), value);
            str += buffer;
            return str.c_str();
        }


    };
    class SafeKVMap
    {
    private:
        CRITICAL_SECTION _cs{ 0 };
        std::map<std::wstring, std::wstring> _map;

    public:
        SafeKVMap()
        {
            (void)::InitializeCriticalSectionAndSpinCount(&_cs, 1023);
        }

        ~SafeKVMap()
        {
            ::DeleteCriticalSection(&_cs);
        }

        // if value is nullptr, remove the entry
        void Set(LPCTSTR key, LPCTSTR value)
        {
            ::EnterCriticalSection(&_cs);
            auto iter = _map.find(key);
            if (_map.end() == iter)
            {
                if (value != nullptr)
                {
                    _map.insert({ key, value });
                }
            }
            else
            {
                if (value != nullptr)
                {
                    iter->second = value;
                }
                else
                {
                    _map.erase(iter);
                }
            }
            ::LeaveCriticalSection(&_cs);
        }

        std::wstring Get(LPCTSTR key)
        {
            //bool found = true;
            std::wstring ret;
            ::EnterCriticalSection(&_cs);
            auto iter = _map.find(key);
            if (_map.end() != iter)
            {
                ret = iter->second;
            }
            ::LeaveCriticalSection(&_cs);

            return ret;
        }
    };

    class HookedAPIList
    {
    private:
        bool _initOk{ false };
        SafeKVMap _kvmap;

    public:

        bool InitOk()
        {
            return _initOk;
        }

        HookedAPIList()
        {
            _initOk = true;
            MH_STATUS ms = MH_Initialize();
            if (ms != MH_OK)
            {
                _initOk = false;
                SML_LOG_LINE(TEXT("MH_Initialize failed with error %d, %S"), ms, MH_StatusToString(ms));
            }
        }

        ~HookedAPIList()
        {
            MH_Uninitialize();
        }

        SafeKVMap& ConfigData()
        {
            return _kvmap;
        }

        HRESULT Hook()
        {
            HRESULT hr = S_OK;
            DWORD LastError = 0;

            BEGIN_BLOCK(0);
            //////////////////////////////////////////////////////////////////////////
            MH_STATUS ms = MH_CreateHookApi(module_Kernel32, api_CreateFileW, HookedAPIList::MyCreateFile, (LPVOID*)(&pOriginalCreateFile));
            if (ms != MH_OK)
            {
                SML_LOG_LINE(TEXT("MH_CreateHookApi %s!%S failed with error %d, %S"), module_Kernel32, api_CreateFileW, ms, MH_StatusToString(ms));
            }
            else
            {
                MH_EnableHook(&::CreateFileW);
            }


            ms = MH_CreateHookApi(module_Ws2_32, api_connect, HookedAPIList::Myconnect, (LPVOID*)(&pOriginalconnect));
            if (ms != MH_OK)
            {
                SML_LOG_LINE(TEXT("MH_CreateHookApi %s!%S failed with error %d, %S"), module_Ws2_32, api_connect, ms, MH_StatusToString(ms));
            }
            else
            {
                MH_EnableHook(&::connect);
            }
            //////////////////////////////////////////////////////////////////////////


            //MH_EnableHook(MH_ALL_HOOKS);

            //////////////////////////////////////////////////////////////////////////
            END_BLOCK(0);

            return hr;

        }

        static HookedAPIList* GetHookedAPIList()
        {
            static HookedAPIList hookedApi;
            return &hookedApi;
        }

        //////////////////////////////////////////////////////////////////////////
        typedef HANDLE(WINAPI* pfnCreateFile)(
            _In_      LPCTSTR lpFileName,
            _In_      DWORD dwDesiredAccess,
            _In_      DWORD dwShareMode,
            _In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
            _In_      DWORD dwCreationDisposition,
            _In_      DWORD dwFlagsAndAttributes,
            _In_opt_  HANDLE hTemplateFile
            );
        LPCWSTR module_Kernel32{ L"Kernel32.dll" };
        LPCSTR api_CreateFileW{ "CreateFileW" };
        pfnCreateFile pOriginalCreateFile{ nullptr };


        static HANDLE WINAPI MyCreateFile(
            _In_      LPCTSTR lpFileName,
            _In_      DWORD dwDesiredAccess,
            _In_      DWORD dwShareMode,
            _In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
            _In_      DWORD dwCreationDisposition,
            _In_      DWORD dwFlagsAndAttributes,
            _In_opt_  HANDLE hTemplateFile
        )
        {
            HookedAPIList* hookedApi = GetHookedAPIList();
            return hookedApi->pOriginalCreateFile(
                lpFileName,//_In_      LPCTSTR lpFileName,
                dwDesiredAccess,//_In_      DWORD dwDesiredAccess,
                dwShareMode,//_In_      DWORD dwShareMode,
                lpSecurityAttributes,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                dwCreationDisposition,//_In_      DWORD dwCreationDisposition,
                dwFlagsAndAttributes,//_In_      DWORD dwFlagsAndAttributes,
                hTemplateFile//,//_In_opt_  HANDLE hTemplateFile
            );
        }
        //////////////////////////////////////////////////////////////////////////

        typedef int (*pfnconnect)(
            _In_  SOCKET s,
            _In_  const struct sockaddr* name,
            _In_  int namelen
            );

        LPCWSTR module_Ws2_32{ L"Ws2_32.dll" };
        LPCSTR api_connect{ "connect" };
        pfnconnect pOriginalconnect{ nullptr };


        static int Myconnect(
            _In_  SOCKET s,
            _In_  const struct sockaddr* name,
            _In_  int namelen
        )
        {
            int ret = 0;
            HookedAPIList* hookedApi = GetHookedAPIList();
            std::wstring ebableForThread;
            SmartLib::ConvHelper::ConvToString(L"tid:", ::GetCurrentThreadId(), ebableForThread);
            std::wstring config = hookedApi->ConfigData().Get(ebableForThread.c_str());
            if (config == L"1")
            {
                SML_LOG_LINE(TEXT("Hook enabled for %s"), ebableForThread.c_str());
                //do some hook here
                ret = hookedApi->pOriginalconnect(
                    s,//_In_  SOCKET s,
                    name,//_In_  const struct sockaddr* name,
                    namelen//,//_In_  int namelen
                );
            }
            else
            {
                SML_LOG_LINE(TEXT("Hook not enaabled for %s, call original connect API"), ebableForThread.c_str());
                ret = hookedApi->pOriginalconnect(
                    s,//_In_  SOCKET s,
                    name,//_In_  const struct sockaddr* name,
                    namelen//,//_In_  int namelen
                );
            }
            return ret;
            
        }


        //////////////////////////////////////////////////////////////////////////
    };

} //namespace SmartLib
