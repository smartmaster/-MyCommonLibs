
#pragma once


#include <iostream>
#include <set>
#include <cassert>
#include <queue>
#include <memory>
#include <cstdlib>
#include <list>
#include <map>
#include <tuple>

#include "sml-common.h"


namespace queries_on_a_permutation_with_key
{
	//https://leetcode.com/problems/queries-on-a-permutation-with-key/

	using namespace  ::std;
	class MutiIndexQue
	{
	private:
		//unordered_map<int, int> _val_pos;
		map<int, int> _val_pos;
		map<int, decltype(_val_pos.begin())> _pos_iter;

	public:
		MutiIndexQue(int m)
		{
			for (int ii = 0; ii < m; ++ii)
			{
				//_val_pos[ii + 1] = ii;
				Insert(ii + 1, ii);
			}
		}

		void Insert(int val, int pos)
		{
			auto [iter, ok] = _val_pos.insert({ val, pos });
			_pos_iter.insert({ pos, iter });
			//_pos_iter[pos] = iter;
		}

		void Remove(int pos)
		{
			auto pos_iter = _pos_iter.find(pos);
			if (pos_iter != _pos_iter.end())
			{
				_val_pos.erase(pos_iter->second);
				_pos_iter.erase(pos_iter);
			}
		}

		int GetValPos(int val)
		{
			int pos = _val_pos[val];
			Remove(pos);

			for (int ii = pos - 1; ii >= 0; --ii)
			{
				int v = _pos_iter[ii]->first;
				Remove(ii);
				Insert(v, ii + 1);
			}
			Insert(val, 0);

			return pos;
		}
	};

	class Solution {
	public:
		static vector<int> processQueries(vector<int>& queries, int m) {

			MutiIndexQue miq{ m };
			vector<int>  res;
			for (auto val : queries)
			{
				res.push_back(miq.GetValPos(val));
			}

			return res;
		}


	public:
		static void Case0()
		{
			vector<int> queries{ 3,1,2,1 };
			int m = 5;

			auto res = processQueries(queries, m);

			return;
		}
	};
}


namespace SmartLib
{
	using namespace ::std;

	struct IdVals
	{
		int id;
		int val1;
		int val2;
		int val3;
	};

	class MutiIndexQueue
	{
	public:
		list<IdVals> _table;

		
		multimap<int, decltype(_table.begin())> _val1_iter;
		multimap<int, decltype(_table.begin())> _val2_iter;
		multimap<int, decltype(_table.begin())> _val3_iter;

		map<int,
			tuple<decltype(_table.begin()),
			decltype(_val1_iter.begin()),
			decltype(_val2_iter.begin()),
			decltype(_val3_iter.begin())>> _id_iters;

	public:
		void Insert(const IdVals& idVals)
		{
			_table.push_front(idVals);
			auto iterData = _table.begin();

			auto iter1 = _val1_iter.insert({ idVals.val1, iterData });
			auto iter2 = _val2_iter.insert({ idVals.val2, iterData });
			auto iter3 = _val3_iter.insert({ idVals.val3, iterData });

			_id_iters.insert({ idVals.id, {iterData, iter1, iter2, iter3} });
		}

		void Print()
		{
			cout << "Print by ID:" << endl;
			for (auto iter = _id_iters.begin(); iter != _id_iters.end(); ++ iter)
			{
				const auto& idvals = *get<0>(iter->second);
				cout << "[" << idvals.id  << "]" << " " << idvals.val1 << " " << idvals.val2 << " " << idvals.val3 << endl;
			}
			cout << endl;


			cout << "Print by val1:" << endl;
			for (auto iter = _val1_iter.begin(); iter != _val1_iter.end(); ++iter)
			{
				const auto& idvals = *iter->second;
				cout << idvals.id << " " << "[" << idvals.val1 << "]" << " " << idvals.val2 << " " << idvals.val3 << endl;
			}
			cout << endl;


			cout << "Print by val2:" << endl;
			for (auto iter = _val2_iter.begin(); iter != _val2_iter.end(); ++iter)
			{
				const auto& idvals = *iter->second;
				cout << idvals.id << " " << idvals.val1 << " " << "["  << idvals.val2 << "]" << " " << idvals.val3 << endl;
			}
			cout << endl;

			cout << "Print by val3:" << endl;
			for (auto iter = _val3_iter.begin(); iter != _val3_iter.end(); ++iter)
			{
				const auto& idvals = *iter->second;
				cout << idvals.id << " " << idvals.val1 << " " << idvals.val2 << " " << "[" << idvals.val3 << "]" << endl;
			}
			cout << endl;
		}

		int Size()
		{
			return _table.size();
		}

		bool EraseById(int id)
		{
			bool ok = false;
			auto iter = _id_iters.find(id);
			if (iter != _id_iters.end())
			{
				auto [iterData, iter1, iter2, iter3] = iter->second;
				_val1_iter.erase(iter1);
				_val2_iter.erase(iter2);
				_val3_iter.erase(iter3);
				_table.erase(iterData);
				_id_iters.erase(iter);
				ok = true;
			}

			return ok;
		}

		bool EraseByValue(int val, int valNumber)
		{
			bool ok = false;
			switch (valNumber)
			{
			case 1:
			{
				auto iter = _val1_iter.find(val);
				if (iter != _val1_iter.end())
				{
					auto iterData = iter->second;
					ok = EraseById(iterData->id);
				}
			}
			break;
			case 2:
			{
				auto iter = _val2_iter.find(val);
				if (iter != _val2_iter.end())
				{
					auto iterData = iter->second;
					ok = EraseById(iterData->id);
				}
			}
			break;
			case 3:
			{
				auto iter = _val3_iter.find(val);
				if (iter != _val3_iter.end())
				{
					auto iterData = iter->second;
					ok = EraseById(iterData->id);
				}
			}
			break;
			}

			return ok;
		}


	public:
		static void Case0()
		{
			MutiIndexQueue miq;
			int id = 0;
			for (int ii = 1; ii <= 5; ++ ii)
			{
				for (int jj = 1; jj <= 3; ++ jj)
				{
					++id;
					IdVals idVals{id, ii, ii*10, ii*100};
					miq.Insert(idVals);
				}
			}

			miq.Print();

			//////////////////////////////////////////////////////////////////////////
			int val1 = 1;
			while (miq.EraseByValue(val1, 1))
			{
				miq.Print();
			}


			//////////////////////////////////////////////////////////////////////////
			int val2 = 20;
			while (miq.EraseByValue(val2, 2))
			{
				miq.Print();
			}

			//////////////////////////////////////////////////////////////////////////
			int val3 = 300;
			while (miq.EraseByValue(val3, 3))
			{
				miq.Print();
			}


			for (int ii = 0; ii < 100; ++ ii)
			{
				bool ok = miq.EraseById(ii);
				if (ok)
				{
					cout << "Deleted id " << ii << endl;
					miq.Print();
				}
				if (0 == miq.Size())
				{
					break;
				}
			}

		}
	};


	class TestNewDelete
	{
	public:
		static void* operator new(size_t size)
		{
			cout << "call operator new" << endl;
			auto p = malloc(size);
			return p;
		}

	public:
		static void operator delete(void * ptr)
		{
			cout << "call operator delete" << endl;
			free(ptr);
		}

	public:
		static void* operator new[](size_t size)
		{
			cout << "call operator new[]" << endl;
			auto p = malloc(size);
			return p;
		}

	public:
		static void operator delete[](void * ptr)
		{
			cout << "call operator delete[]" << endl;
			free(ptr);
		}

	private:
		inline static long s_count{ 0 };
		long _index{ 0 };

	public:
		TestNewDelete()
		{
			_index = s_count++;
			cout << "construct object " << _index << endl;
		}

		~TestNewDelete()
		{
			cout << "deconstruct object " << _index << endl;
		}


	public:
		static void Case0()
		{
			TestNewDelete* p = new TestNewDelete{};
			delete p;

			TestNewDelete* parray = new TestNewDelete[10]{};
			//delete[10] parray;
			delete[] parray;
		}
	};

	class TestAllocator
	{
	public:
		static void Case0()
		{
			vector<TestNewDelete>::allocator_type allocator;
			TestNewDelete* pv = allocator.allocate(100);
			allocator.deallocate(pv, 100);
		}
	};


	class Self :
		public enable_shared_from_this<Self>
	{
	private:
		string _data;

	private:
		void Print()
		{
			cout << "the data is " << _data << endl;
		}


	public:
		Self(const char* data) :
			_data{data}
		{
		}

		void CallPrint()
		{
			auto self{ shared_from_this() };
			self->Print();
		}

	public:
		static void Case0()
		{
			shared_ptr<Self> sp{ make_shared<Self>("hello world!") };
			sp->CallPrint();
		}
	};

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