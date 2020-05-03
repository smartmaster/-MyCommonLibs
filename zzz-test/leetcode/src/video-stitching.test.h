#pragma once
#include "video-stitching.h"

#include <iostream>

namespace SmartLib
{
	using namespace ::std;

	class video_stitching_test
	{
	public:
		static void Case0()
		{
			vector<vector<int>> clips =
			{
				//{0,1},{6,8},{0,2},{5,6},{0,4},{0,3},{6,7},{1,3},{4,7},{1,4},{2,5},{2,6},{3,4},{4,5},{5,7},{6,9}
				{{0, 5}, {2, 3} }

			};

			int T = 9;
			long res = ::SmartLib::video_stitching::videoStitching(clips, T);
			cout << "The result is " << res << endl;
		}
	};
}