#pragma once


//https://leetcode.com/problems/binary-tree-coloring-game/

#include <map>
#include <unordered_map>

namespace SmartLib
{
	using namespace ::std;

	class binary_tree_coloring_game
	{

	public:
		struct TreeNode
		{
			int val;
			TreeNode* left;
			TreeNode* right;
			TreeNode(int x) : val(x), left(NULL), right(NULL) {}
		};


	private:
		unordered_map<TreeNode*, long> _node_count;
		//map<TreeNode*, long> _node_count;


	private:
		long DFSCount(TreeNode* root)
		{

			if (root == nullptr)
			{
				return 0L;
			}


			long left = DFSCount(root->left);

			long right = DFSCount(root->right);


			_node_count.insert({root, 1 + left + right});

			return (1 + left + right);
			
		}


	private:
		TreeNode* DFSFind(TreeNode* root, int val)
		{
			if (root == nullptr)
			{
				return nullptr;
			}

			if (root->val == val)
			{
				return root;
			}


			auto node = DFSFind(root->left, val);
			if (node == nullptr)
			{
				node = DFSFind(root->right, val);
			}

			return node;
		}

	private:
		long MaxNodes(TreeNode* root, int x)
		{
			DFSCount(root);
			auto node = DFSFind(root, x);

			long leftSize = node->left ? _node_count[node->left] : 0;
			long rightSize = node->right ? _node_count[node->right] : 0;
			long otherSize = _node_count[root] - leftSize - rightSize - 1;

			long max = leftSize;
			if (max < rightSize)
			{
				max = rightSize;
			}

			if (max < otherSize)
			{
				max = otherSize;
			}
			return max;
		}


	public:
		static bool Solution(TreeNode* root, int n, int x)
		{
			return (binary_tree_coloring_game{}.MaxNodes(root, x) << 1) > n;
		}

	};

}