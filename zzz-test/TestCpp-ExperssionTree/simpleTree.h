#pragma once

#include <cassert>

#include <vector>
#include <map>

#include "log.h"

using std::vector;
using std::map;

namespace SmartLib
{
	//template<typename T>
	class TreeNode
	{
	public:
		int _data{0};
		TreeNode * _left{ nullptr };
		TreeNode * _right{ nullptr };
	};

	class BFSNode
	{
	public:
		int _parentPos{ 0 };
		TreeNode * _node{ nullptr };


		BFSNode(int parentPos, TreeNode * node) :
			_parentPos(parentPos),
			_node(node)
		{

		}
	};
	

	//template<typename T>
	class SimpleTree
	{
	public:
		static TreeNode * CreateTree(int count, vector<TreeNode*> & vec)
		{
			assert(count > 0);

			vec.resize(count);
			for (int ii = 0; ii < count; ++ ii)
			{
				TreeNode * temp = new TreeNode();
				temp->_data = ii;
				vec[ii] = temp;
			}

			for (int ii = 0; ii < count; ++ ii)
			{
				int left = 2 * ii + 1;
				int right = left + 1;

				if (left < count)
				{
					vec[ii]->_left = vec[left];
				}
				else
				{
					break;
				}

				if (right < count)
				{
					vec[ii]->_right = vec[right];
				}
				else
				{
					break;
				}
			}


			return vec[0];
		}


		static void BFS(TreeNode * node, vector<BFSNode> & queue, map<TreeNode*, int> & lookup)
		{
			assert(node != nullptr);

			queue.push_back(BFSNode(-1, node));
			lookup.insert({ node, 0 });

			int indexParent = 0;
			while (indexParent < queue.size())
			{
				TreeNode * left = queue[indexParent]._node->_left;
				TreeNode * right = queue[indexParent]._node->_right;

				if (left)
				{
					queue.push_back(BFSNode(indexParent, left));
					lookup.insert({ left , queue.size() - 1});
				}
				
				if (right)
				{
					queue.push_back(BFSNode(indexParent, right));
					lookup.insert({ right , queue.size() - 1 });
				}
				

				++indexParent;
			}
		}

		static bool DFS(TreeNode * node, TreeNode * target, vector<TreeNode*> & stack, vector<TreeNode*> & resultStack)
		{
			if (node == nullptr)
			{
				return false;
			}

			bool rv = false;
			stack.push_back(node);
			if (node == target)
			{
				resultStack = stack;
				rv = true;
			}
			else
			{
				rv = DFS(node->_left, target, stack, resultStack);
				if (!rv)
				{
					rv = DFS(node->_right, target, stack, resultStack);
				}
			}
			
			stack.pop_back();


			return rv;
		}
	};


	class SimpleTreeTest
	{
	public:
		static void Case0()
		{
			vector<TreeNode*> vec;
			TreeNode * root = SimpleTree::CreateTree(100, vec);

			//////////////////////////////////////////////////////////////////////////
			vector<BFSNode> queue;
			map<TreeNode*, int> lookup;
			SimpleTree::BFS(root, queue, lookup);


			auto iter = lookup.find(vec.back());
			if (iter != lookup.end())
			{
				int index = iter->second;
				while (index >= 0)
				{
					LOG_A("%d ", queue[index]._node->_data);
					index = queue[index]._parentPos;
				}
				LOG_LINE_A("");
			}

			//////////////////////////////////////////////////////////////////////////
			vector<TreeNode*> stack, resultStack;
			SimpleTree::DFS(root, vec.back(), stack, resultStack);
			for (auto node : resultStack)
			{
				LOG_A("%d ", node->_data);
			}
			LOG_LINE_A("");
		}
	};
}