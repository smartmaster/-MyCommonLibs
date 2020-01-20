#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

namespace SmartLib
{
	class CComInitThread
	{
	private:
		HRESULT m_hr{ E_FAIL };
	public:
		CComInitThread(DWORD dwCoInit)
		{
			m_hr = CoInitializeEx(nullptr, dwCoInit);
		}

		~CComInitThread()
		{
			if (SUCCEEDED(m_hr))
			{
				CoUninitialize();
				m_hr = E_FAIL;
			}
		}
	};


	template<typename T>
	class CThreadComPtr
	{
	private:
		DWORD m_cookie{ (DWORD)(0) };

	public:
		CThreadComPtr(T * p)
		{
			if (nullptr != p)
			{
				Set(p);
			}
		}

		void Set(T * p)
		{
			CComGITPtr<T> gitPtr;
			if (0 != m_cookie)
			{
				gitPtr.Attach(m_cookie);
				gitPtr.Revoke();
				m_cookie = 0;
			}
			
			gitPtr.Attach(p);
			m_cookie = gitPtr.Detach();
		}

		~CThreadComPtr()
		{
			if (0 != m_cookie)
			{
				CComGITPtr<T> gitPtr;
				gitPtr.Attach(m_cookie);
				gitPtr.Revoke();
				m_cookie = 0;
			}
		}

		CComPtr<T> Ptr()
		{
			CComPtr<T> p = nullptr;
			CComGITPtr<T> gitPtr;
			gitPtr.Attach(m_cookie);
			gitPtr.CopyTo(&p);
			gitPtr.Detach();
			return p;
		}

	private:
		CThreadComPtr(CONST CThreadComPtr &); //it is important to disable copy ctor and assignment operator
		CONST CThreadComPtr& operator = (CONST CThreadComPtr &);
	};

}