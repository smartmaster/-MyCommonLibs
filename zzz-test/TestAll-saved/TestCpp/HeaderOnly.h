#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <string>
#include <iostream>


namespace TestCpp
{
	class SomeData
	{
		int _aa{ 0 };
		int _bb{ 0 };
		int _ss{ 0 };
	public: 
		SomeData(int aa, int bb, int ss):
			_aa(aa),
			_bb(bb),
			_ss(ss)
		{
		}


		void Print() const
		{
			std::wcout << _aa
				<< _bb
				<< _ss
				<< std::endl;
		}
	};

	class NoCpp
	{
	public:
		static const std::wstring& StringConst()
		{
			static std::wstring s_str = L"Hello, world";
			return s_str;
		}


		static const SomeData& SomeDataConst()
		{
			static SomeData sd(3, 5, 8);
			return sd;
		}

	};

	class TestNoCpp
	{
	public:
		static void Case1()
		{
			std::wcout << NoCpp::StringConst() << std::endl;

			const auto & sd = NoCpp::SomeDataConst();
			sd.Print();
		}
	};



	class ILock
	{
	public:
		virtual void Lock() = 0;
		virtual void Unlock() = 0;
	};

	class CsLock : public ILock
	{
	private:
		CRITICAL_SECTION _cs;
		std::wstring _desc;

	public:

		CsLock(LPWSTR desc):
			_desc(desc)
		{
			InitializeCriticalSection(&_cs);
		}

		~CsLock()
		{
			DeleteCriticalSection(&_cs);
		}

		virtual void Lock() override
		{
			EnterCriticalSection(&_cs);
			std::wcout << L"Lock -- " << _desc << std::endl;
		}
		virtual void Unlock() override
		{
			std::wcout << L"Unlock -- " << _desc << std::endl;
			LeaveCriticalSection(&_cs);
		}
	};

	class AutoLock
	{
	private:
		ILock * _lock{ nullptr };
	public:
		AutoLock(ILock * lock):
			_lock(lock)
		{
			_lock->Lock();
		}

		~AutoLock()
		{
			_lock->Unlock();
		}
	};

	class ClassSync
	{
		int _index{ 0 };
	public:
		ClassSync()
		{
			AutoLock al(&StaticData::ClassLock());
			_index = ++ StaticData::ClassCount();
			
		}


		void Method1()
		{
			AutoLock al(&StaticData::ClassLock());
			Sleep(2000);
			std::wcout << __FUNCTION__ << std::endl;
		}


		void Method2()
		{
			AutoLock al(&StaticData::ClassLock());
			Sleep(5000);
			std::wcout << __FUNCTION__ << std::endl;
		}

		~ClassSync()
		{
			AutoLock al(&StaticData::ClassLock());
			--StaticData::ClassCount();
		}


	private:
		class StaticData
		{
		public:
			static CsLock & ClassLock()
			{
				static CsLock s_lock(L"type lock for ClassSync");
				return s_lock;
			}

			static int & ClassCount()
			{
				static int s_classCount = 100;
				return s_classCount;
			}
		};
	};
}

