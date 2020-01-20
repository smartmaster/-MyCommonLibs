#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <iostream>
#include <thread>

namespace SmartLib
{
	class CTestThreadSum
	{
	private:
		int _sum{ 0 };

	public:
		int CalcSum(int kk)
		{
			int sum = 0;
			for (int ii = 1; ii <= kk; ++ ii)
			{
				sum += ii;
				std::cout << "running " << ii << " ..." << std::endl;
				::Sleep(200);
			}
			_sum = sum;
			return sum;
		}

		int GetSum()
		{
			return _sum;
		}
	};


	class CTestThreadCase
	{
	public:
		static void Case0()
		{
			CTestThreadSum summer;
			std::thread thread{
				[&summer]()
				{
					summer.CalcSum(100);
				}
			};

			::WaitForSingleObject(thread.native_handle(), INFINITE);
			std::cout << "Thread ended and Sum is " << summer.GetSum() << "." << std::endl;
			::TerminateThread(thread.native_handle(), E_FAIL);
			if (thread.joinable())
			{
				thread.join();
			}
			auto p = thread.native_handle();
			if (thread.joinable())
			{
				thread.join();
			}
		}
	};
}