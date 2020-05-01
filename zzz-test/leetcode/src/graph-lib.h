#pragma once

//https://leetcode.com/problems/minimum-cost-to-make-at-least-one-valid-path-in-a-grid/

#include <vector>
#include <map>
#include <set>
#include <stack>
#include <functional>

#include <cassert>
#include <climits>



namespace SmartLib
{

	using namespace ::std;

	class Graph
	{
	private:
		vector<map<long, long>> _graphStore;

		vector<bool> _visited;
		function<void(long)> _visit;

	public:
		Graph(long nodeCount = 0)
		{
			if (nodeCount > 0)
			{
				_graphStore.resize(nodeCount);
			}
		}

		void AddEdge(long startNode, long nextNode, long lengh)
		{
			if (startNode >= _graphStore.size())
			{
				_graphStore.resize(startNode + 1);
			}

			_graphStore[startNode][nextNode] = lengh;
		}

		void RemoveEdge(long startNode, long nextNode)
		{
			if (startNode < _graphStore.size())
			{
				auto& edges = _graphStore[startNode];

				auto iter = edges.find(nextNode);
				if (iter != edges.end())
				{
					edges.erase(iter);
				}
			}

			while (_graphStore.size())
			{
				if (_graphStore.back().size() == 0)
				{
					_graphStore.pop_back();
				}
				else
				{
					break;
				}
			}
		}


		long EdgeLength(long startNode, long nextNode)
		{
			return _graphStore[startNode][nextNode];
		}

		auto& Adjacents(long startNode)
		{
			return _graphStore[startNode];
		}

		long NodeCount()
		{
			return _graphStore.size();
		}

		void BFS(long node, function<void(long)> visit)
		{
			_visit = visit;
			_visited.clear();
			_visited.resize(NodeCount(), false);
			BFS_R(node);
		}

		void BFS_R(long node)
		{
			deque<long> que;

			que.push_back(node);
			_visited[node] = true;
			_visit(node);

			while (que.size())
			{
				long head = que.front();
				que.pop_front();

				for (auto [next, len] : Adjacents(head))
				{
					if (!_visited[next])
					{
						que.push_back(next);
						_visited[next] = true;
						_visit(next);
					}
				}
			}
		}

		void DFS(long node, function<void(long)> visit)
		{
			_visit = visit;
			_visited.clear();
			_visited.resize(NodeCount(), false);
			DFS_R(node);
		}

		void DFS_R(long node)
		{
			stack<long> stack;

			stack.push(node);
			_visited[node] = true;
			_visit(node);
			
			while (stack.size())
			{
				long top = stack.top();
				stack.pop();

				for (auto [next, len] : Adjacents(top))
				{
					if (!_visited[next])
					{
						stack.push(next);
						_visited[next] = true;
						_visit(next);
					}
					
				}
			}
			
		}
	};


	class FastPriorityQueue
	{
	private:
		multiset<long, function<bool(long id1, long id2)>> _order;
		map<long, decltype(_order.begin())> _pos;

	public:

		FastPriorityQueue(function<bool(long id1, long id2)> comapre) :
			_order{ comapre }
		{
		}

		void Insert(long id)
		{
			auto iter = _order.insert(id);
			_pos.insert({ id, iter });
		}

		void Remove(long id)
		{
			auto iter = _pos.find(id);
			if (iter != _pos.end())
			{
				assert(id == *(iter->second));
				_order.erase(iter->second);
				_pos.erase(iter);
			}
		}

		long GetFirstId()
		{
			return (*_order.begin());
		}


		long Size()
		{
			return _order.size();
		}
	};

	class Dijkstra
	{
	private:

		inline static const long SML_MAX_DISTANCE{ LONG_MAX / 2 };
		
	
	private:
		Graph& _graph; /***INCOMMING***/

		vector<long> _distances;
		vector<bool> _visited;


		function<bool(long id1, long id2)> _compare = [this](long id1, long id2)
		{
			return _distances[id1] < _distances[id2];
		};
		FastPriorityQueue _queue{ _compare };


	private:
		long FindShortestNode()
		{
#if false
			long node = -1;
			long distance = SML_MAX_DISTANCE;

			for (long ii = 0; ii < _visited.size(); ++ii)
			{
				if (!_visited[ii])
				{
					if (distance > _distances[ii])
					{
						distance = _distances[ii];
						node = ii;
					}
				}
			}

			return node;
#else
			return _queue.Size()? _queue.GetFirstId() : -1;
#endif
		}


		void UpdateNextNodes(long node)
		{
			for (auto [next, len] : _graph.Adjacents(node))
			{
				if (!_visited[next])
				{
					if (_distances[next] > _distances[node] + len)
					{
						_queue.Remove(next);
						_distances[next] = _distances[node] + len;
						_queue.Insert(next);
					}
				}
			}
		}

	public:
		Dijkstra(Graph& graph) :
			_graph{ graph }
		{
			_distances.resize(graph.NodeCount(), SML_MAX_DISTANCE);
			_visited.resize(graph.NodeCount(), false);
		}

		void ShortestPath(long startNode)
		{
			_distances[startNode] = 0;
			_queue.Insert(startNode);

			while (_queue.Size())
			{
				long node = FindShortestNode();
				_queue.Remove(node);
				_visited[node] = true;
				UpdateNextNodes(node);
				//--count;
			}
		}

		long GetDistance(long nextNode)
		{
			return _distances[nextNode];
		}
	};

	class Floyd
	{
	private:

		inline static const long SML_MAX_DISTANCE{ LONG_MAX / 2 };


	private:
		Graph& _graph; /***INCOMMING***/

		vector<vector<long>> _distances; /***INTERNAL***/

	public:
		Floyd(Graph& graph) :
			_graph{graph}
		{
			_distances.resize(_graph.NodeCount());
			for (auto& sub : _distances)
			{
				sub.resize(_graph.NodeCount(), SML_MAX_DISTANCE);
			}

			for (long startNode = 0; startNode < _graph.NodeCount(); ++startNode)
			{
				_distances[startNode][startNode] = 0;
				for (auto [nextNode, len] : _graph.Adjacents(startNode))
				{
					_distances[startNode][nextNode] = len;
				}
			}
		}

		void ShortestPath()
		{
			long totalCount = _graph.NodeCount();
			for (long midNode = 0; midNode < totalCount; ++midNode)
			{
				for (long srcNode = 0; srcNode < totalCount; ++srcNode)
				{
					long d1 = _distances[srcNode][midNode];
					if (d1 != SML_MAX_DISTANCE)
					{
						for (long destNode = 0; destNode < totalCount; ++ destNode)
						{
							long d2 = _distances[midNode][destNode];
							if (d2 != SML_MAX_DISTANCE)
							{
								if (_distances[srcNode][destNode] > d1 + d2)
								{
									_distances[srcNode][destNode] = d1 + d2;
								}
							}
						}
					}
				}
			}
		}


		long GetDistance(long startNode, long nextNode)
		{
			return _distances[startNode][nextNode];
		}
	};

	class MinCostDirectionGridAlg3
	{
	public:
		inline static const int  UP{ 4 };
		inline static const int  DOWN{ 3 };
		inline static const int  LEFT{ 2 };
		inline static const int  RIGHT{ 1 };

	private:
		const vector<vector<int>>& _direction; /***INCOMMING***/
		const int _ROW; /***INCOMMING***/
		const int _COL; /***INCOMMING***/
		const int _NODE_COUNT; /***INCOMMING***/

		Graph _graph;

	private:
		MinCostDirectionGridAlg3(const vector<vector<int>>& direction) :
			_direction{ direction },
			_ROW{ (int)direction.size() },
			_COL{ (int)direction[0].size() },
			_NODE_COUNT{ (int)(_ROW * _COL) },
			_graph{ _NODE_COUNT }
		{

		}

		int NodeNumber(int row, int col)
		{
			return row * _COL + col;
		}

		int UpNodeNumber(int row, int col)
		{
			return (row - 1) * _COL + col;
		}

		int DownNodeNumber(int row, int col)
		{
			return (row + 1) * _COL + col;
		}

		int LeftNodeNumber(int row, int col)
		{
			return row * _COL + col - 1;
		}

		int RightNodeNumber(int row, int col)
		{
			return row * _COL + col + 1;
		}


		void MakeEdge(int row, int col)
		{
			int start = NodeNumber(row, col);

			if (row - 1 >= 0)
			{
				int next = NodeNumber(row - 1, col);
				if (_direction[row][col] == UP)
				{
					//_edges[start][next] = 0;
					_graph.AddEdge(start, next, 0);
				}
				else
				{
					//_edges[start][next] = 1;
					_graph.AddEdge(start, next, 1);
				}
			}

			if (row + 1 < _ROW)
			{
				int next = NodeNumber(row + 1, col);
				if (_direction[row][col] == DOWN)
				{
					//_edges[start][next] = 0;
					_graph.AddEdge(start, next, 0);
				}
				else
				{
					//_edges[start][next] = 1;
					_graph.AddEdge(start, next, 1);
				}
			}


			if (col - 1 >= 0)
			{
				int next = NodeNumber(row, col - 1);
				if (_direction[row][col] == LEFT)
				{
					//_edges[start][next] = 0;
					_graph.AddEdge(start, next, 0);
				}
				else
				{
					//_edges[start][next] = 1;
					_graph.AddEdge(start, next, 1);
				}
			}


			if (col + 1 < _COL)
			{
				int next = NodeNumber(row, col + 1);
				if (_direction[row][col] == RIGHT)
				{
					//_edges[start][next] = 0;
					_graph.AddEdge(start, next, 0);
				}
				else
				{
					//_edges[start][next] = 1;
					_graph.AddEdge(start, next, 1);
				}
			}
		}


		void MakeEdges()
		{
			for (int ii = 0; ii < _ROW; ++ii)
			{
				for (int jj = 0; jj < _COL; ++jj)
				{
					MakeEdge(ii, jj);
				}
			}

#if false
			{
				set<long> nodes;
				auto visit = [&nodes, this](long node)
				{
					nodes.insert(node);
					cout << node << " ";
				};
				_graph.BFS(0, visit);
				cout << endl;

				for (auto node : nodes)
				{
					cout << node << " ";
				}
				cout << endl;
			}

			{
				set<long> nodes;
				auto visit = [&nodes, this](long node)
				{
					nodes.insert(node);
					cout << node << " ";
				};
				_graph.DFS(0, visit);
				cout << endl;

				for (auto node : nodes)
				{
					cout << node << " ";
				}
				cout << endl;
			}
#endif			
		}

		long SolutionD()
		{
			MakeEdges();
			Dijkstra dj{ _graph };
			dj.ShortestPath(0);
			return dj.GetDistance(_NODE_COUNT - 1);
		}

		long SolutionF()
		{
			MakeEdges();
			Floyd fl{_graph};
			fl.ShortestPath();
			return fl.GetDistance(0, _NODE_COUNT - 1);
		}

	public:
		static long SolutionD(const vector<vector<int>>& direction)
		{
			int r1 = MinCostDirectionGridAlg3{ direction }.SolutionD();
			return r1;
		}

		static long SolutionF(const vector<vector<int>>& direction)
		{
			int r1 = MinCostDirectionGridAlg3{ direction }.SolutionF();
			return r1;
		}

	};
}