#pragma once

#include <vector>

namespace SmartLib
{
	using namespace ::std;

	template<typename T>
	class QuickSortAlg
	{
	private:
		vector<T>& _data;

	private:
		long FindLarger(long begin, long end, const T& data)
		{
			while (begin < end)
			{
				if (_data[begin] > data)
				{
					break;
				}
				++begin;
			}

			return begin;
		}

		long FindSmaller(long begin, long end, const T& data)
		{
			while (begin < end)
			{
				if (_data[begin] <= data)
				{
					break;
				}
				++begin;
			}

			return begin;
		}

		void SwapRange(long p1, long p2, long p3)
		{
			long count = p2 - p1;
			long count2 = p3 - p2;
			if (count > count2)
			{
				count = count2;
			}

			p2 = p3 - 1;
			while (count)
			{
				SwapByIndex(p1, p2);

				++p1;
				--p2;

				--count;
			}

		}

		void SwapByIndex(long p1, long p2)
		{
			if (p1 != p2)
			{
				T temp = _data[p1];
				_data[p1] = static_cast<T&&>(_data[p2]);
				_data[p2] = static_cast<T&&>(temp);
			}

		}


		long Partition(long begin, long end)
		{
			const T& pivot = _data[begin];

			long larger = FindLarger(begin, end, pivot);
			long smaller = FindSmaller(larger, end, pivot);
			long nextLarger = FindLarger(smaller, end, pivot);
			while (smaller < end)
			{
				SwapRange(larger, smaller, nextLarger);
				larger += (nextLarger - smaller);
				smaller = FindSmaller(nextLarger, end, pivot);
				nextLarger = FindLarger(smaller, end, pivot);
			}


			return larger;
		}

		void Sort(long begin, long end)
		{
			if (end - begin <= 1)
			{
				return;
			}

			long part = Partition(begin, end);
			SwapByIndex(begin, part - 1);
			Sort(begin, part - 1);
			Sort(part, end);
		}

	public:
		QuickSortAlg(vector<T>& data) :
			_data{ data }
		{
		}


		void Sort()
		{
			Sort(0, _data.size());
		}

	};
}