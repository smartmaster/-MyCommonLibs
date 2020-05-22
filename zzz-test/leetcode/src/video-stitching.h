#pragma once



#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>

#include <climits>

namespace SmartLib
{
	using namespace ::std;

	class video_stitching
	{
	private:
		const vector<vector<int>>& _raw;
		const long _T;


		vector<vector<long>> _clips;

		vector<vector<long>> _dp;


	private:
		inline constexpr static long SML_INFI{LONG_MAX / 2};


	private:
		video_stitching(const vector<vector<int>>& raw, const long T) :
			_raw{raw},
			_T{ T }//,
		{
		}

	private:
		void PreProcess() //reduce apparently useless ranges
		{

			//unordered_map<long, long> reducedES;
			map<long, long> reducedES;
			for (const auto& sub : _raw)
			{
				long start = sub[0];
				long end = sub[1];

				if (start >= _T)
				{
					continue;
				}


				if (end > _T)
				{
					end = _T;
				}

				auto iter = reducedES.find(end);
				if (iter == reducedES.end())
				{
					reducedES[end] = start;
				}
				else
				{
					if (iter->second > start)
					{
						iter->second = start;
					}
				}
			}



			//unordered_map<long, long> reducedSE;
			map<long, long> reducedSE;
			for (auto [end, start] : reducedES)
			{
				auto iter = reducedSE.find(start);
				if (iter == reducedSE.end())
				{
					reducedSE[start] = end;
				}
				else
				{
					if (iter->second < end)
					{
						iter->second = end;
					}
				}
			}



			for (const auto [start, end] : reducedSE)
			{
				_clips.push_back({ start, end });
			}

			sort(begin(_clips), end(_clips), [](const vector<long>& v1, const vector<long>& v2)
				{
					if (v1[0] != v2[0])
					{
						return v1[0] < v2[0];
					}

					return v1[1] > v2[1];
				});


			//for (const auto sub : _clips)
			//{
			//	long start = sub[0];
			//	long end = sub[1];
			//	cout << "{" << start << ", " << end << "}, " << endl;
 		//	}


			_dp.resize(_T + 1, vector<long>(_T + 1, SML_INFI));
		}


	private:
		static bool IsIntersect(const vector<long>& v1, const vector<long>& v2)
		{
			return max(v1[0], v2[0]) <= min(v1[1], v2[1]);
		}

	//private:
	//	static bool IsContain(const vector<long>& v1, const vector<long>& v2)
	//	{
	//		return v1[0] <= v2[0] && v1[1] >= v2[1];
	//	}

	//private:
	//	static vector<long> SetUnion(const vector<long>& v1, const vector<long>& v2)
	//	{
	//		return  {min(v1[0], v2[0]), max(v1[1], v2[1])};
	//	}

	private:
		long FindMaxEnd(const vector<long>& range, long startPos, long& endPos) //find the largest range in greedy
		{
			long maxEnd = range[1];
			long ii = startPos;
			for (; ii < _clips.size(); ++ ii)
			{
				if (IsIntersect(range, _clips[ii]))
				{
					if (maxEnd < _clips[ii][1])
					{
						maxEnd = _clips[ii][1];
					}
				}
				else
				{
					break;
				}
			}
			endPos = ii;
			return maxEnd;
		}

	private:
		long Count() //get the result
		{
			vector<long> range{ 0, 0 };
			long count = 0;
			long startPos = 0;
			for (;;)
			{
				long endPos = startPos;
				long maxEnd = FindMaxEnd(range, startPos, endPos);

				if (range[1] < maxEnd)
				{
					++count;
					range[1] = maxEnd;
				}

				if (startPos == endPos || endPos == _clips.size())
				{
					break;
				}
				startPos = endPos;
			}

			return range[1] == _T? count : -1;
		}

	private:
		void DP(long ss, long ee)
		{
			//for (long len = 1; len <= _T; ++len)
			for (long ii = 0; ii < _T; ++ ii)
			{
				//for (long ii = 0; ii <= _T - len; ++ ii)
				for (long jj = ii + 1; jj <= _T; ++ jj)
				{
					//long jj = ii + len;
					if (ss > jj || ee < ii) //no intersection
					{
						continue;
					}
					else if (ss <= ii && ee >= jj) //this covers all
					{
						_dp[ii][jj] = 1;
					}
					else if (ee >= jj) //this covers right part
					{
						_dp[ii][jj] = min(_dp[ii][jj], _dp[ii][ss] + 1);
					}
					else if (ss <= ii) //this covers left part
					{
						_dp[ii][jj] = min(_dp[ii][jj], _dp[ee][jj] + 1);
					}
					else // if(ss > ii && ee < jj) this is covered
					{
						_dp[ii][jj] = min(_dp[ii][jj], _dp[ii][ss] + _dp[ee][jj] + 1);
					}
				}
			}
		}

	private:
		long DPAll()
		{
			for (const auto& clip : _clips)
			{
				DP(clip[0], clip[1]);
			}

			return _dp[0][_T] == SML_INFI ? -1 : _dp[0][_T];
		}


	public:
		static long videoStitching(const vector<vector<int>>& raw, long T)
		{
			video_stitching alg{ raw, T };
			alg.PreProcess();
			//long res = alg.Count();
			long res = alg.DPAll();
			return res;
		}
	};
}