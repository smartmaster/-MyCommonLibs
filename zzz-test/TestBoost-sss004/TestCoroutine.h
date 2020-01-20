#pragma once

#include <iostream> 
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <boost/coroutine2/all.hpp>

namespace Smartlib
{
	class TestCoro1
	{
	public:
		static void foo(boost::coroutines2::coroutine<void>::push_type & push) {
			std::cout << "a ";
			push();
			std::cout << "b ";
			push();
			std::cout << "c ";
		}


		static int Main() {
			boost::coroutines2::coroutine<void>::pull_type pull(foo);
			std::cout << "1 ";
			pull();
			std::cout << "2 ";
			pull();
			std::cout << "3 ";
			//getchar();
			return 0;
		}
	};


	class TestCoro2
	{
	public:
		static void foo(boost::coroutines2::coroutine<void>::pull_type & pull) {
			std::cout << "a ";
			pull();
			std::cout << "b ";
			pull();
			std::cout << "c ";
		}

		static int Main() {
			boost::coroutines2::coroutine<void>::push_type push(foo);
			std::cout << "1 ";
			push();
			std::cout << "2 ";
			push();
			std::cout << "3 ";
			return 0;
		}
	};

	class TestCoro3
	{
	public:
		static void foo(boost::coroutines2::coroutine<std::string>::pull_type & pull)
		{
			std::cout << "get " << pull.get() << " from main() by foo()\n";
			pull();
			std::cout << "get " << pull.get() << " from main() by foo()\n";
			pull();
		}

		static int Main()
		{
			std::string str1("HELLO");
			std::string str2("WORLD");
			boost::coroutines2::coroutine<std::string>::push_type push(foo);
			std::cout << "pass " << str1 << " from main() to foo()\n";
			push(str1);
			std::cout << "pass " << str2 << " from main() to foo()\n";
			push(str2);
			return 0;
		}

	};


	class TestCoro4
	{
	public:
		static void Consumer(boost::coroutines2::coroutine<std::string>::pull_type & pull)
		{
			std::cout << "consumer started" << std::endl;
			int count = 0;
			for (;;)
			{
				std::cout << "Consume " << count << ", " << pull.get() << std::endl;
				pull();
				std::cout << "pull done " << count << std::endl;
				++count;
			}
			std::cout << "consumer ended" << std::endl;
		}


		static void Provider()
		{
			std::cout << "Provider started" << std::endl;

			boost::coroutines2::coroutine<std::string>::push_type push(Consumer);
			std::string str{"A"};
			for (int ii = 0; ii < 10; ++ ii)
			{
				std::cout << "Provide " << ii << ", " << str << std::endl;
				push(str);
				std::cout << "push done " << ii << std::endl;
				++ str[0];
				std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(1000));
			}

			std::cout << "Provider ended" << std::endl;
		}
	};
}