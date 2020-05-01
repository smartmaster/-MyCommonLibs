#pragma once

#include <map>
#include <vector>
#include <set>
#include <functional>

#include <climits>

namespace SmartLib
{
	using namespace ::std;

	class Graph2
	{
	private:
		vector<map<long, long>> _graph;

	public:
		Graph2(long nodeCount = 0)
		{
			if (nodeCount > 0)
			{
				_graph.resize(nodeCount);
			}
		}
		void AddEdge(long startNode, long nextNode, long edgeLen)
		{

			if (startNode >= _graph.size())
			{
				_graph.resize(startNode + 1);
			}


			_graph[startNode].insert({ nextNode, edgeLen});
		}


		void RemoveEdge(long startNode, long nextNode)
		{
			if (startNode < _graph.size())
			{
				_graph[startNode].erase(nextNode);
			}
		}


		long EdgeLength(long startNode, long nextNode)
		{
			return _graph[startNode][nextNode];
		}

		const auto& NextNodes(long startNode) const
		{
			return _graph[startNode];
		}

		long NodesCount() const
		{
			return _graph.size();
		}

	};

	class FastPriorityQueue2
	{
	public:
		struct NodeDistance
		{
			long _node{ 0 };
			long _distance{ 0 };

			NodeDistance(long node = 0, long distance = 0) :
				_node{ node },
				_distance{ distance }//,
			{

			}
		};

	private:

		function<bool(const NodeDistance&, const NodeDistance&)> _compare = 
			[](const NodeDistance& nd1, const NodeDistance& nd2)
		{
			return nd1._distance < nd2._distance;
		};
		multiset<NodeDistance, decltype(_compare)> _nodeDistance{_compare};
		map<long, decltype(_nodeDistance.begin()) > _nodePos;

	public:
		void Insert(long node, long distance)
		{
			auto pos = _nodeDistance.insert(NodeDistance{node, distance});
			_nodePos.insert({ node, pos });
		}

		void Delete(long node)
		{
			auto iter = _nodePos.find(node);
			if (iter != _nodePos.end())
			{
				auto pos = iter->second;
				_nodeDistance.erase(pos);
				_nodePos.erase(iter);
			}
		}

		NodeDistance GetMin()
		{
			return *_nodeDistance.begin();
		}

		long Size()
		{
			return _nodeDistance.size();
		}
	};

	class Dijkstra2
	{
		inline static const long MAX_INFI = LONG_MAX / 2;

		const Graph2& _graph;

		vector<long> _distances;
		vector<bool> _visited;

		FastPriorityQueue2 _que;

	public:
		Dijkstra2(const Graph2& graph) :
			_graph{graph}
		{
			_distances.resize(_graph.NodesCount(), MAX_INFI);
			_visited.resize(_graph.NodesCount(), false);
		}

	private: 
		void UpdateNextNodes(long startNode)
		{
			for (auto [node, edgeLen] : _graph.NextNodes(startNode))
			{
				if (!_visited[node])
				{
					long tempDistance = _distances[startNode] + edgeLen;
					if (_distances[node] > tempDistance)
					{
						_que.Delete(node);
						_distances[node] = tempDistance;
						_que.Insert(node, tempDistance);
					}
				}
			}
		}

		void ShortestPath(long startNode)
		{
			_distances[startNode] = 0;
			_que.Insert(startNode, 0);

			while (_que.Size())
			{
				auto [node, dis] = _que.GetMin();

				//_distances[node] = dis;
				_visited[node] = true;
				_que.Delete(node);

				UpdateNextNodes(node);
			}
		}



	};
}