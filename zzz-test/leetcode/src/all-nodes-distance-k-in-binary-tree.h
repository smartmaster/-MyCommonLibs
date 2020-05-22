#pragma once

#include <vector>
#include <deque>

namespace SmartLib::all_nodes_distance_k_in_binary_tree
{
	using namespace ::std;


	struct TreeNode
	{
		int val;
		TreeNode* left;
		TreeNode* right;
		TreeNode(int x) : val(x), left(NULL), right(NULL) {}
	};




	class Graph
	{
	private:
		vector<vector<int>> _graph;

	public:
		void AddEdge(int nodeId1, int nodeId2)
		{
			if (nodeId1 >= _graph.size())
			{
				_graph.resize(nodeId1 + 1);
			}

			_graph[nodeId1].push_back(nodeId2);
		}


		const auto& NextNodes(int nodeId)
		{
			return _graph[nodeId];
		}

		int Size()
		{
			return _graph.size();
		}
	};

	class Alg
	{
	public:
		//unordered_map<int, TreeNode*> _map;
		Graph _gg;
		vector<bool> _visited;

	public:
		void DFS(TreeNode* root)
		{
			if (nullptr == root)
			{
				return;
			}

			int rootNode = root->val;
			//_map.insert({ rootNode, root});


			if (root->left)
			{
				int childNode = root->left->val;
				_gg.AddEdge(rootNode, childNode);
				_gg.AddEdge(childNode, rootNode);


				DFS(root->left);
			}

			if (root->right)
			{
				int childNode = root->right->val;
				_gg.AddEdge(rootNode, childNode);
				_gg.AddEdge(childNode, rootNode);

				DFS(root->right);
			}
		}


		vector<int> BFS(int node, int len)
		{
			if (_gg.Size() == 0)
			{
				return {};
			}

			_visited.resize(_gg.Size(), false);

			deque<int> que;
			que.push_back(node);
			_visited[node] = true;

			int level = 0;

			while (que.size())
			{
				if (level == len)
				{
					break;
				}
				

				int size = que.size();
				while (size)
				{
					--size;
					int front = que.front();
					que.pop_front();

					for (int next : _gg.NextNodes(front))
					{
						if (!_visited[next])
						{
							_visited[next] = true;
							que.push_back(next);
						}
					}
				}

				++level;
			}


			vector<int> res;
			for (int node : que)
			{
				res.push_back(node);
			}

			return res;
		}
	};


	class Solution {
	public:
		static vector<int> distanceK(TreeNode* root, TreeNode* target, int K)
		{

			Alg alg;
			alg.DFS(root);
			return alg.BFS(target->val, K);

		}

	public:
		static void Case0()
		{
			TreeNode* root = new TreeNode{ 1 };
			vector<int> res = distanceK(root, root, 3);
		}
	};
}