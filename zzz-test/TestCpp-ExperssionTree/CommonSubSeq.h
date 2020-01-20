#pragma once

#include <vector>

#include "log.h"
#include "MultiDimArray.h"

namespace SmartLib
{
	class CommonSubSeqAlg
	{
		std::vector<int> _list1;
		std::vector<int> _list2;
		MDArray<int> _mda;
	public:

		CommonSubSeqAlg(const std::vector<int>  & list1, const std::vector<int>  & list2) :
			_list1(list1),
			_list2(list2),
			_mda({ (int)list1 .size() + 1, (int)list2.size() + 1 })
		{
			for (int ii = 0; ii < list1.size() + 1; ++ ii)
			{
				for (int jj = 0; jj < list2.size() + 1; ++ jj)
				{
					_mda({ ii, jj }) = -1;
				}
			}
		}

		int Length()
		{
			return Length(_list1.size(), _list2.size());
		}


		int Length(int mm, int nn)
		{
			int saved = _mda({ mm, nn });
			if (saved > 0)
			{
				return saved;
			}

			int len = 0;
			if (mm == 0 || nn == 0)
			{
				len = 0;
			}
			else if (_list1[mm - 1] == _list2[nn - 1])
			{
				len = Length(mm - 1, nn - 1) + 1;
			}
			else
			{
				int len1 = Length(mm, nn - 1);
				int len2 = Length(mm - 1, nn);

				len = len1 > len2 ? len1 : len2;
			}

			_mda({ mm, nn }) = len;

			return len;
		}
		
	};

	class CommonSubSeqAlgTest
	{
	public:
		static void Case0()
		{
			std::vector<int> l1 = {1, 2, 3, 4, 1};
			std::vector<int> l2 = { 3, 4, 1, 2, 1, 3};

			CommonSubSeqAlg alg(l1, l2);
			int len = alg.Length();

			LOG_LINE(L"Length = %d", len);
		}
	};
}
