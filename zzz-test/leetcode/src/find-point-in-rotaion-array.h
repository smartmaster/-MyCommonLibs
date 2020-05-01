#pragma once

#include <cassert>

namespace SmartLib
{
	class FindUpperPointAlg
	{
	private:
		long* _array{ nullptr };
		long _size{ 0 };

	public:


		FindUpperPointAlg(long* arr, long size) :
			_array{arr},
			_size{ size }
		{
		}

		long FindUpperPoint(long left, long right)
		{
			while (left <= right)
			{
				if (left == right)
				{
					return left;
				}
				else if (left + 1 == right)
				{
					return _array[left] > _array[right] ? left : right;
				}
				else
				{
					long mid = (left + right) / 2;
					if (_array[mid] > _array[0])
					{
						left = mid;
					}
					else
					{
						right = mid - 1;
					}
				}
			}

			assert(false);
			return -1; //
		}
		
	};
}
