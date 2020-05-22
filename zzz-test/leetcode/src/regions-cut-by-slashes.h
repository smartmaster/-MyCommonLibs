#pragma once

//https://leetcode.com/problems/regions-cut-by-slashes/

#include <vector>
#include <string>
#include <cassert>

namespace SmartLib
{
	using namespace  ::std;

	class DJSet
	{
	private:
		vector<long> _parent;

	public:
		DJSet(long nodeCount)
		{
			_parent.resize(nodeCount);
			for (long ii = 0; ii < nodeCount; ++ ii)
			{
				_parent[ii] = ii;
			}
		}

		long FindRoot(long nodeId)
		{
			//long cur = nodeId;
			//while (cur != _parent[cur])
			//{
			//	cur = _parent[cur];
			//}
			//_parent[nodeId] = cur;
			//return cur;


			if (nodeId != _parent[nodeId])
			{
				_parent[nodeId] = FindRoot(_parent[nodeId]); //reduce tree height
			}
			return _parent[nodeId];
		}

		void Join(long nodeId1, long nodeId2)
		{
			_parent[FindRoot(nodeId1)] = FindRoot(nodeId2);
		}

		long Count()
		{
			long count = 0;
			for (long ii = 0; ii < _parent.size(); ++ ii)
			{
				if (FindRoot(ii) == ii)
				{
					++count;
				}
			}
			return count;
		}
	};

	class regions_cut_by_slashes
	{
	private:
		const vector<string>& _grid;
		const long _edgeLen{ 0 };
		DJSet _djset;
		
	private:
		regions_cut_by_slashes(vector<string>& grid) :
			_grid{ grid },
			_edgeLen{ (long)grid.size() },
			_djset{_edgeLen * _edgeLen * 4}//,
		{
		}


	private:
		void Process(long row, long col)
		{
			char ch = _grid[row][col];

			long node_0 = (row * _edgeLen + col) * 4;
			long node_1 = node_0 + 1;
			long node_2 = node_1 + 1;
			long node_3 = node_2 + 1;

			switch (ch)
			{
			case '/':
			{
				_djset.Join(node_0, node_1);
				_djset.Join(node_2, node_3);
			}
			break;
			case '\\':
			{
				_djset.Join(node_0, node_3);
				_djset.Join(node_1, node_2);
			}
			break;
			case ' ':
			{
				_djset.Join(node_0, node_1);
				_djset.Join(node_1, node_2);
				_djset.Join(node_2, node_3);
				_djset.Join(node_3, node_0);
			}
			break;
			default:
			{
				assert(false);
			}
			break;
			}

			if (col + 1 < _edgeLen)
			{
				long rightNode = node_3 + 1;
				_djset.Join(node_2, rightNode);
			}

			if (row + 1 < _edgeLen)
			{
				long lowerNode = ((row + 1) * _edgeLen + col) * 4 + 1;
				_djset.Join(node_3, lowerNode);
			}

		}
	private:
		long Solution()
		{
			for (long ii = 0; ii < _edgeLen; ++ ii)
			{
				for (long jj = 0; jj < _edgeLen; ++ jj)
				{
					Process(ii, jj);
				}
			}


			return _djset.Count();
		}


	public:
		static int regionsBySlashes(vector<string>& grid) 
		{
			regions_cut_by_slashes alg{grid};
			long res = alg.Solution();
			return res;
		}
	};
}