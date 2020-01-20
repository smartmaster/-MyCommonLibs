#pragma once

#include <unordered_map>;

namespace SmartLib
{
	class CheckBalancedBinaryTree
	{
	public:
		struct TreeNode {
		     int val;
		     TreeNode *left;
		     TreeNode *right;
		     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
		 };


	private:
		std::unordered_map<TreeNode*, int> _height;

		int Height(TreeNode * node)
		{
			if (node == nullptr)
			{
				return 0;
			}
			else
			{
				auto iter = _height.find(node);
				if (iter != _height.end())
				{
					return iter->second;
				}

				int left = Height(node->left);
				int right = Height(node->right);
				int hh = (left > right ? left : right) + 1;
				_height.insert({node, hh});
				return hh;
			}
		}

		static int Diff(int val1, int val2)
		{
			return val1 > val2 ? val1 - val2 : val2 - val1;
		}


	public:
		bool IsBalanceTree(TreeNode * root)
		{
			if (nullptr == root)
			{
				return true;
			}

			int leftHeight = Height(root->left);
			int rightHeight = Height(root->right);
			if (Diff(leftHeight, rightHeight) > 1)
			{
				return false;
			}

			return IsBalanceTree(root->left) && IsBalanceTree(root->right);
		}
		
	};
}