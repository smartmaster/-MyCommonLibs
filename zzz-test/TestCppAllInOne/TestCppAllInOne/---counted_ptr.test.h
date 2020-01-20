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
		static void Case1()
		{
			counted_ptr<SomeObj> sp1 = counted_ptr<SomeObj>::make(123, L"hello");
			counted_ptr<SomeObj> sp2 = sp1;
			counted_ptr<SomeObj> sp3{sp2};
			sp3 = sp1;

			counted_ptr<SomeObj> sp5{ sp1 };
			void* pv = sp5.detach();

			counted_ptr<SomeObj> sp6;
			sp6.attach(pv);

			sp1.attach(sp6.detach());
			
			sp3->Print();

			//std::function<void(SomeObj*)> mydeleter = [](SomeObj * ptr)
			//{
			//	delete ptr;
			//};
			//shared_ptr<SomeObj> sp4{ new SomeObj{123, L"good"}, mydeleter };
		}
	};

}