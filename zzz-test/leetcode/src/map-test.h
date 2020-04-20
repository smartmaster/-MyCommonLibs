#pragma once



#include <map>
#include <unordered_map>

#include <iostream>

#include "sml-common.h"

namespace SmartLib
{
	using namespace ::std;

	class MapTest
	{
	public:
		static void Case0()
		{
			map<long, long> mm;
			for (long ii = 0; ii < 100; ii += 3)
			{
				mm.insert({ii, ii});
			}

			//////////////////////////////////////////////////////////////////////////
			auto lb = mm.lower_bound(20);
			auto ub = mm.upper_bound(40);

			while (lb != ub)
			{
				Common::Print(cout, '{', lb->first, ", ", lb->second, "} ");
				++lb;
			}
			cout << endl;

			//////////////////////////////////////////////////////////////////////////
			auto first = mm.find(33);
			if (mm.end() != first)
			{
				auto end = mm.upper_bound(33);
				while (first != end)
				{
					Common::Print(cout, '{', first->first, ", ", first->second, "} ");
					++first;
				}
				cout << endl;
			}

			//////////////////////////////////////////////////////////////////////////
			auto range = mm.equal_range(33);
			while (range.first != range.second)
			{
				Common::Print(cout, '{', range.first->first, ", ", range.first->second, "} ") << endl;
				++range.first;
			}
		}

		static void Case1()
		{
			multimap<long, long> mm;
			for (long ii = 0; ii < 100; ii += 3)
			{
				mm.insert({ ii, ii});
				mm.insert({ ii, ii + 1 });
				mm.insert({ ii, ii + 2 });
			}


			//////////////////////////////////////////////////////////////////////////
			auto lb = mm.lower_bound(20);
			auto ub = mm.upper_bound(40);

			while (lb != ub)
			{
				Common::Print(cout, '{', lb->first, ", ", lb->second, "} ");
				++lb;
			}
			cout << endl;

			//////////////////////////////////////////////////////////////////////////
			auto first = mm.find(33);
			if (mm.end() != first)
			{
				auto end = mm.upper_bound(33);
				while (first != end)
				{
					Common::Print(cout, '{', first->first, ", ", first->second, "} ");
					++first;
				}
				cout << endl;
			}

			//////////////////////////////////////////////////////////////////////////
			auto range = mm.equal_range(33);
			while (range.first != range.second)
			{
				Common::Print(cout, '{', range.first->first, ", ", range.first->second, "} ") << endl;
				++range.first;
			}
		}

		static void Case2()
		{
			unordered_map<long, long> mm;
			for (long ii = 0; ii < 100; ii += 3)
			{
				mm.insert({ ii, ii });
			}

			auto iter = mm.find(20);
			if (mm.end() != iter)
			{
				Common::Print(cout, '{', iter->first, ", ", iter->second, "} ") << endl;
			}


			iter = mm.find(33);
			if (mm.end() != iter)
			{
				Common::Print(cout, '{', iter->first, ", ", iter->second, "} ") << endl;
			}

			auto range = mm.equal_range(33);
			while (range.first != range.second)
			{
				Common::Print(cout, '{', range.first->first, ", ", range.first->second, "} ") << endl;
				++range.first;
			}

		}

		static void Case3()
		{
			unordered_multimap<long, long> mm;
			for (long ii = 0; ii < 100; ii += 3)
			{
				mm.insert({ ii, ii });
				mm.insert({ ii, ii + 1 });
				mm.insert({ ii, ii + 2 });
			}


			auto iter = mm.find(20);
			if (mm.end() != iter)
			{
				Common::Print(cout, '{', iter->first, ", ", iter->second, "} ") << endl;
			}


			iter = mm.find(33);
			if (mm.end() != iter)
			{
				Common::Print(cout, '{', iter->first, ", ", iter->second, "} ") << endl;
			}

			auto range = mm.equal_range(33);
			while (range.first != range.second)
			{
				Common::Print(cout, '{', range.first->first, ", ", range.first->second, "} ") << endl;
				++range.first;
			}
		}
	};
}