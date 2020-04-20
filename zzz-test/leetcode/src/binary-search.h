#pragma once


#include <vector>
#include <functional>

namespace SmartLib
{
	using namespace ::std;

	class BinarySearchALg 
	{
	private:
		const vector<long>& _data;

	public:
		long BinarySearchLastSmaller(long val, long left, long right)
		{
			while (left <= right)
			{
				long mid = (left + right) / 2;

				if (_data[mid] > val)
				{
					right = mid - 1;
				}
				else
				{
					if (mid == right || val < _data[mid + 1])
					{
						return mid;
					}

					left = mid + 1;
				}
			}

			return -1;
		}

		long BinarySearchFirtLarger(long val, long left, long right)
		{
			//while (left <= right)
			//{
			//	long mid = (left + right) / 2;
			//	if (_data[mid] < val)
			//	{
			//		left = mid + 1;
			//	}
			//	else
			//	{
			//		if (mid == left || val > _data[mid - 1])
			//		{
			//			return mid;
			//		}
			//		right = mid - 1;
			//	}
			//}

			//return -1;

			while (left <= right)
			{
				if (left == right)
				{
					if (_data[left] > val)
					{
						return left;
					}
					else
					{
						return -1;
					}
				}

				long mid = (left + right) / 2;
				if (_data[mid] > val)
				{
					right = mid;
				}
				else
				{
					left = mid + 1;
				}
			}

			return -1;
		}

		long BinSearch(function<bool(long)> test, long left, long right)
		{
			while (left <= right)
			{
				if (left == right)
				{
					if (test(_data[left]))
					{
						return left;
					}
					else
					{
						return -1;
					}
				}
				else
				{
					long mid = (left + right) / 2;
					if (test(_data[mid]))
					{
						right = mid;
					}
					else
					{
						left = mid + 1;
					}
				}

			}
			
		}


		BinarySearchALg(const vector<long>& data) :
			_data{data}
		{
		}
	};
}