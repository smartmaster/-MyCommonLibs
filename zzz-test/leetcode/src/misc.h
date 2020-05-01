
#pragma once


#include <iostream>
#include <set>
#include <cassert>
#include <queue>
#include "sml-common.h"



namespace SmartLib
{
	using namespace ::std;

	class Misc
	{
	public:

		template<typename T>
		static 	void print_queue(T& q) {
			while (!q.empty()) {
				std::cout << q.top() << " ";
				q.pop();
			}
			std::cout << '\n';
		}

		static void TestPriorityQue()
		{
			auto comparer = [](long child, long parent)
			{
				return child > parent;
			};
			priority_queue<long, vector<long>, decltype(comparer)> lpq{comparer};
			for (int n : {1, 8, 5, 6, 3, 4, 0, 9, 7, 2})
			{
				lpq.push(n);
			}
			print_queue(lpq);

			priority_queue<int> q;

			for (int n : {1, 8, 5, 6, 3, 4, 0, 9, 7, 2})
				q.push(n);

			print_queue(q);

			priority_queue<int, std::vector<int>, std::greater<int> > q2;

			for (int n : {1, 8, 5, 6, 3, 4, 0, 9, 7, 2})
				q2.push(n);

			print_queue(q2);

			// Using lambda to compare elements.
			auto cmp = [](int left, int right) { return (left ^ 1) < (right ^ 1); };
			std::priority_queue<int, std::vector<int>, decltype(cmp)> q3(cmp);

			for (int n : {1, 8, 5, 6, 3, 4, 0, 9, 7, 2})
				q3.push(n);

			print_queue(q3);
		}

		static void TestInitializrer()
		{
			std::initializer_list<int> hello{1, 2, 3};
			for (auto iter = hello.begin(); iter != hello.end(); ++ iter)
			{
				cout << *iter << " ";
			}
			cout << endl;
		}

		static void TestStructBind()
		{
			struct ArrayPair
			{
				int arr_i[16];
				char arr_b[3];
			};


			ArrayPair ap;
			ap.arr_i[0] = 999;
			ap.arr_i[_countof(ap.arr_i) - 1] = -999;

			ap.arr_b[0] = 'a';
			ap.arr_b[_countof(ap.arr_b) - 1] = 'z';

			auto [ai, ab] = ap;
			//auto& [ai, ab] = ap;
			Common::Print(cout, "sizeof(ai) = ", sizeof(ai), ", sizeof(ab) = ", sizeof(ab)) << endl;

			assert(ai[0] == 999);
			assert(ai[_countof(ai) - 1] == -999);


			assert(ab[0] == 'a');
			assert(ab[_countof(ab) - 1] == 'z');


		}

		static void TestLowerBound()
		{
			auto compare = [](long aa, long bb)
			{
				return aa > bb;
			};

			set<long, decltype(compare)> descding{compare};

			for (long ii = 0; ii <= 20; ii += 2)
			{
				descding.insert(ii);
			}

			Common::PrintIterator(cout, begin(descding), end(descding)) << endl;


			const long data = 5;
			auto iter = descding.lower_bound(data);
			if (iter != descding.end())
			{
				Common::Print(cout, "lower bound for ", data, " is ", *iter) << endl;
			}
		}
	};
}