#pragma once

#include <cassert>

namespace SmartLib
{
	using namespace ::std;

	class Sum123
	{
	public:
		static long long Calc(int nn)
		{
			long long count = 0;
			for (int cc = 0; cc <= nn/3; ++ cc)
			{
				int _3cc = (cc << 1) + cc; // cc + cc + cc
				int _2bb = nn - _3cc;
				count += (( _2bb >> 1) + 1); 
			}

			return count;
		}

	public:
		static void Case0()
		{
			long long count = Calc(20);
			assert(count == 44);

			count = Calc(10000);

			return;
		}
	};
}