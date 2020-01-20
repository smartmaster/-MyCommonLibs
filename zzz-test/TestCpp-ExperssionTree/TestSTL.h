#pragma once

#include <type_traits>
#include <vector>
#include <atomic>
#include <thread>
#include <mutex>
#include <algorithm>


#include "Helper.h"

namespace SmartLib
{
	class TestSTL
	{
	public:

		static void Case5()
		{
			using std::vector;
			vector<int> vec{ 2, 4, 5, 6, 8, 10 };
			auto iter = std::lower_bound(vec.begin(), vec.end(), 7);
			LOG_LINE_A("%d", *iter);
		}

		static void Case4()
		{
			std::thread thread;
			std::mutex mutex;
			//std::unique_lock lock(mutex);

		}

		static void Case3()
		{
			std::atomic<int> val = 100;
			int expected, result;
			do 
			{
				expected = val.load();
				result = expected * 9;
			} while (!val.compare_exchange_weak(expected, result));
		}

		static void Case2()
		{
			std::vector<int> vec{ 1, 2, 3 };
			auto & r_vec = vec;

			vec.push_back(100);

			Helper::Print(r_vec);
			auto v1(std::move(r_vec));
			Helper::Print(r_vec);
			Helper::Print(v1);
			auto v2(vec);
			Helper::Print(v2);
		}

		static void Case1()
		{
			class Base{};
			class Derived : public Base {};

			//if constexpr (std::is_base_of(Base, Derived)::value)
			{

			}


		}
		static void Case0()
		{
			std::conditional_t<false, int&, const int &> ref = 4;
		}

	};

}