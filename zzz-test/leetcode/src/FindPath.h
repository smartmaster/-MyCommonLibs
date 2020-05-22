#pragma once

#include <vector>
#include <iostream>
#include "get-input.h"

namespace SmartLib
{

	using namespace ::std;

	class FindPath
	{
	private:
		vector<vector<int>>& _grath;
		const int _TOTAL_ROW{ 0 };
		const int _TOTAL_COL{ 0 };


		int _curStep{ 0 };


		int _minStep{ INT_MAX };
		int _minRow{ INT_MAX };
		int _minCol{ INT_MAX };

		vector<vector<bool>> _visited;

	public:
		FindPath(vector<vector<int>>& grath) :
			_grath{ grath },
			_TOTAL_ROW{ (int)(grath.size()) },
			_TOTAL_COL{ (int)(grath[0].size()) }//,
		{
			_visited.resize(_TOTAL_ROW);
			for (auto& sub : _visited)
			{
				sub.resize(_TOTAL_COL);
			}
		}

		void DFS(int row, int col)
		{
			if (row == 0 || row == _TOTAL_ROW - 1 || col == 0 || col == _TOTAL_COL - 1)
			{
				if (_minStep > _curStep)
				{
					_minStep = _curStep;
					_minRow = row;
					_minCol = col;
				}
				return;
			}


			_visited[row][col] = true;
			int saved = _grath[row][col];
			++_curStep;

			for (int ii = 0; ii < 4; ++ii)
			{
				int nextRow = row;
				int nextCol = col;
				switch (ii)
				{
				case 0:
				{
					--nextRow;

				}
				break;
				case 1:
				{
					++nextRow;

				}
				break;
				case 2:
				{
					--nextCol;

				}
				break;
				case 3:
				{
					++nextCol;
				}
				break;
				}

				if (nextRow >= 0 && nextRow < _TOTAL_ROW && nextCol >= 0 && nextCol < _TOTAL_COL)
				{
					if (_grath[row][col] >= _grath[nextRow][nextCol] && _visited[nextRow][nextCol] == false)
					{
						DFS(nextRow, nextCol);
					}
				}
			}

			_visited[row][col] = false;
			--_curStep;
		}

		void Print()
		{
			cout << _minRow << " " << _minCol << " " << _minStep << " " << endl;
		}


	public:
		static void Case0()
		{
			vector<string> lines
			{
				"4 4",
				"2 2",
				"9 2 9 9 \
				9 4 9 9 \
				9 5 5 9 \
				9 9 9 9"
			};

			auto values = Helper::StringsToInts(lines);
			for (const auto& sub : values)
			{
				Helper::PrintContainer(begin(sub), end(sub));
			}

			int row = values[0][0];
			int col = values[0][1];
			vector<vector<int>> graph(row, vector<int>(col));
			int kk = 0;
			for (int ii = 0; ii < row; ++ii)
			{
				for (int jj = 0; jj < col; ++jj)
				{
					graph[ii][jj] = values[2][ii * row + jj];
				}
			}


			int startRow = values[1][0];
			int startCol = values[1][1];

			FindPath alg{ graph };

			alg.DFS(startRow, startCol);
			alg.Print();
		}
	};

}