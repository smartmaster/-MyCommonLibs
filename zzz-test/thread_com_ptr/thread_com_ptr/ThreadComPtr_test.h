#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>

#include "ThreadComPtr.h"
#include "..\sssscomserver\sssscomserver_i.h"


namespace SmartLib
{

	class CThreadComPtr_test
	{
	private:
		CThreadComPtr<Isss> m_p{ nullptr };

	private:
		CThreadComPtr_test()
		{
			CComPtr<Isss> sp;
			sp.CoCreateInstance(CLSID_sss, nullptr);
			m_p.Set(sp);
			m_p.Set(sp);
			m_p.Set(sp);
		}

		void Proc()
		{
			CComPtr<Isss> ptr = m_p.Ptr();
			ptr->Do();
		}

		static DWORD WINAPI ThreadProc(
			_In_ LPVOID lpParameter
			)
		{
			CComInitThread comInit(COINITBASE_MULTITHREADED);
			CThreadComPtr_test * obj = (CThreadComPtr_test*)(lpParameter);
			obj->Proc();
			return 0;
		}

		void RunThreads(LONG count)
		{
			//if (count > MAXIMUM_WAIT_OBJECTS)
			//{
			//	count = MAXIMUM_WAIT_OBJECTS;
			//}

			std::vector<HANDLE> vec(count);
			for (LONG ii = 0; ii < count; ++ii)
			{
				DWORD ThreadId = 0;
				vec[ii] = ::CreateThread(
					nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES  lpThreadAttributes,
					0,//_In_      SIZE_T                 dwStackSize,
					(LPTHREAD_START_ROUTINE)(CThreadComPtr_test::ThreadProc),//_In_      LPTHREAD_START_ROUTINE lpStartAddress,
					(LPVOID)(this),//_In_opt_  LPVOID                 lpParameter,
					0,//_In_      DWORD                  dwCreationFlags,
					&ThreadId//_Out_opt_ LPDWORD                lpThreadId
					);
			}

			LONG remain = vec.size();
			LONG offset = 0;
			while (remain)
			{
				LONG current = remain;
				if (current > MAXIMUM_WAIT_OBJECTS)
				{
					current = MAXIMUM_WAIT_OBJECTS;
				}

				WaitForMultipleObjects(current, &vec[offset], TRUE, INFINITE);

				remain -= current;
				offset += current;
			}

			
			for (size_t ii = 0; ii < vec.size(); ++ii)
			{
				CloseHandle(vec[ii]);
			}
			vec.clear();
		}

	public:
		static void Test()
		{
			CComInitThread comInit(COINITBASE_MULTITHREADED);
			CThreadComPtr_test obj;
			obj.Proc();
			obj.RunThreads(200);
		}
	};
}