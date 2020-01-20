#pragma once

#include <memory>
#include <string>
#include <iostream>

#include "log.h"
#include "CodeBlock.h"

namespace SmartLib
{
	class TestSharedPtr
	{
		class SomeObj
		{
			int _ii;
			std::wstring _str;

		public:
			SomeObj(int ii, const std::wstring & str) :
				_ii(ii),
				_str(str)//,
			{
			}

			void Print()
			{
				SML_LOG_LINE(TEXT("ii=%d, str=%s"), _ii, _str.c_str());
			}
		};


	public:
		static void Case1()
		{
			std::shared_ptr<SomeObj> sp1 = std::make_shared<SomeObj>(123, L"hello");
			std::shared_ptr<SomeObj> sp2 = sp1;
			std::shared_ptr<SomeObj> sp3{ sp2 };
			sp3->Print();

			std::function<void(SomeObj*)> mydeleter = [](SomeObj* ptr)
			{
				delete ptr;
			};
			shared_ptr<SomeObj> sp4{ new SomeObj{123, L"good"}, mydeleter };
		}
	};
}