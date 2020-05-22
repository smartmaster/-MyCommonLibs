#pragma once

#include <vector>

namespace SmartLib
{
	using namespace  ::std;

	class MergeSortAlg
	{
	private:
		vector<int>& _data;
		vector<int> _helper;

	public:
		MergeSortAlg(vector<int>& data) :
			_data{data}
		{
			_helper.resize(_data.size());
		}

	public:
		static int LocalInversionCount(const vector<int>& data)
		{
			int count = 0;
			for (int ii = 0; ii < data.size() - 1; ++ ii)
			{
				if (data[ii] > data[ii + 1])
				{
					++count;
				}
			}

			return count;
		}

	private:
		int Sort(int start, int end)
		{
			if (end - start <= 1)
			{
				return 0;
			}


			int halfSize = (end - start) / 2;
			int invers1 = Sort(start, start + halfSize);
			int invers2 = Sort(start + halfSize, end);
			int invers3 = Merge(start, start + halfSize, start + halfSize, end);

			return invers1 + invers2 + invers3;
		}

	private:
		int Merge(int left, int leftEnd, int right, int rightEnd)
		{
			int start = left;

			int kk = 0;
			int invers = 0;
			while (left < leftEnd && right < rightEnd)
			{
				if (_data[left] <= _data[right])
				{
					_helper[kk] = _data[left++];
				}
				else
				{
					_helper[kk] = _data[right++];
					invers += (leftEnd - left);
				}

				++kk;
			}

			while (left < leftEnd)
			{
				_helper[kk++] = _data[left++];
			}

			while (right < rightEnd)
			{
				_helper[kk++] = _data[right++];
			}

			memcpy(&_data[start], &_helper[0], kk * sizeof(_helper[0]));

			return invers;
		}

	public:
		int Sort()
		{
			return Sort(0, _data.size());
		}
	};



}