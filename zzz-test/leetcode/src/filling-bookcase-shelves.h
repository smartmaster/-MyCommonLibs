#pragma once


#include <vector>
#include <climits>

namespace SmartLib
{
	using namespace ::std;

	class filling_bookcase_shelves
	{
	private:
		vector<vector<int>>& _books; /***INCOMMING***/
		long _shelf_width; /***INCOMMING***/

		vector<long> _heights;

	private:
		filling_bookcase_shelves(vector<vector<int>>& books, int shelf_width) :
			_books{ books },
			_shelf_width{ shelf_width }
		{
			_heights.resize(_books.size() + 1, LONG_MAX);
			_heights[0] = 0;
		}

	private:
		long MinHeightDP()
		{
			vector<long> dp(_books.size() + 1, LONG_MAX);
			dp[0] = 0;
			
			for (long size = 1; size <= _books.size(); ++size)
			{
				long count = 1;
				long thick = _books[size - count][0];
				long height = _books[size - count][1];
				for (;;)
				{
					long temp = dp[size - count] + height;
					if (dp[size] > temp)
					{
						dp[size] = temp;
					}

					++count;
					if (count > size)
					{
						break;
					}

					thick += _books[size - count][0];
					height = height > _books[size - count][1]? height : _books[size - count][1];
					if (thick > _shelf_width)
					{
						break;
					}
				}
			}
			return dp.back();
		}


		long MinHeight(long size)
		{
			long hh = 0;
			if ((hh = _heights[size]) != LONG_MAX)
			{
				return hh;
			}

			long thick = _books[size - 1][0];
			long height = _books[size - 1][1];
			long count = 1;
			long mh = LONG_MAX;
			for (;;)
			{
				long tempMh = MinHeight(size - count) + height;
				if (mh > tempMh)
				{
					mh = tempMh;
				}

				count++;
				if (count > size)
				{
					break;
				}

				thick += _books[size - count][0];
				if (height < _books[size - count][1])
				{
					height = _books[size - count][1];
				}

				if (thick > _shelf_width)
				{
					break;
				}
				
			}

			_heights[size] = mh;
			return mh;
		}

	public:
		static int minHeightShelves(vector<vector<int>>& books, int shelf_width) 
		{
			filling_bookcase_shelves alg{books, shelf_width};
			//long mh = alg.MinHeight(books.size());
			long mh = alg.MinHeightDP();
			return mh;
		}
	};
}