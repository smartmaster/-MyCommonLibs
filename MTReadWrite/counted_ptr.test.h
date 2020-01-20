#pragma once

#include <string>

#include "counted_ptr.h"

//#include "log.h"

namespace SmartLib
{
	class counted_ptr_test
	{
	private:
		class SomeObj
		{
			int _ii;
			std::wstring _str;

		public:
			SomeObj(int ii, const std::wstring& str) :
				_ii(ii),
				_str(str)//,
			{
			}

			void Print()
			{
				//SML_LOG_LINE(TEXT("ii=%d, str=%s"), _ii, _str.c_str());
				return;
			}


			~SomeObj()
			{
				_ii = 0;
				_str.clear();
				//SML_LOG_LINE(TEXT("dtor called"));
				return;
			}
		};


	public:
		static  DWORD WINAPI ThreadProc(
			_In_  LPVOID lpParameter
		)
		{
			counted_ptr<SomeObj> sp;
			sp.attach((SomeObj*)lpParameter);
			for (int ii = 0; ii < 10; ++ ii)
			{
				::Sleep(200);
				sp->Print();
			}
			return 0;
		}

		static void Case1()
		{
			counted_ptr<SomeObj> sp1 = counted_ptr<SomeObj>::make(123, L"hello");
			counted_ptr<SomeObj> sp2 = sp1;
			counted_ptr<SomeObj> sp3{sp2};
			sp3 = sp1;

			counted_ptr<SomeObj> sp5{ sp1 };
			SomeObj* pv = sp5.dettach();

			counted_ptr<SomeObj> sp6;
			if (sp6)
			{
				sp6->Print();
			}

			sp6.attach(pv);
			if (sp6)
			{
				sp6->Print();
			}



			sp1.attach(sp6.dettach());
			
			sp3->Print();


			DWORD tid = 0;
			SomeObj* param = sp1.dettach();
			HANDLE hthread = ::CreateThread(
				nullptr,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
				0,//_In_       SIZE_T dwStackSize,
				ThreadProc,//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
				param,//_In_opt_   LPVOID lpParameter,
				0,//_In_       DWORD dwCreationFlags,
				&tid//_Out_opt_  LPDWORD lpThreadId
			);

			::WaitForSingleObject(hthread, INFINITE);
			::CloseHandle(hthread);
			hthread = nullptr;



			//std::function<void(SomeObj*)> mydeleter = [](SomeObj * ptr)
			//{
			//	delete ptr;
			//};
			//shared_ptr<SomeObj> sp4{ new SomeObj{123, L"good"}, mydeleter };
		}
	};

}