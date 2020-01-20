#pragma once


#include <vector>
using std::vector;

namespace SmartLib
{
	class FindMedianSortedArrays
	{
	public:
		static vector<int> Merge(const vector<int> & arr1, const vector<int> & arr2)
		{
			int nn1 = 0;
			int nn2 = 0;
			int rr = 0;
			vector<int> rv(arr1.size() + arr2.size());
			while (nn1 < arr1.size() && nn2 < arr2.size())
			{
				if (arr1[nn1] < arr2[nn2])
				{
					rv[rr] = arr1[nn1++];
				}
				else
				{
					rv[rr] = arr2[nn2++];
				}
				++rr;
			}

			while (nn1 < arr1.size())
			{
				rv[rr++] = arr1[nn1++];
			}

			while (nn2 < arr2.size())
			{
				rv[rr++] = arr2[nn2++];
			}

			return rv;
		}
	};
}