#pragma once

#include <vector>

//https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/

namespace SmartLib_construct_binary_tree_from_preorder_and_inorder_traversal
{
	using namespace ::std;

	struct TreeNode {
		int val;
		TreeNode* left;
		TreeNode* right;
		TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
	};

	class Constructor
	{
	private:
		const vector<int>& _preorder;
		const vector<int>& _inorder;

	private:
		int Find(int data, int infirst, int instop)
		{
			while (infirst < instop)
			{
				if (data == _inorder[infirst])
				{
					break;
				}
				++infirst;
			}

			return infirst;
		}

		TreeNode* Construct(int prefirst, int prestop, int infirst, int instop)
		{
			if (prefirst == prestop)
			{
				return nullptr;
			}

			int data = _preorder[prefirst];
			int pos = Find(data, infirst, instop);

			int leftcount = pos - infirst;
			int rightcount = instop - pos - 1;

			TreeNode* left = Construct(prefirst + 1, prefirst + 1 + leftcount, infirst, pos);
			TreeNode* right = Construct(prefirst + 1 + leftcount, prestop, pos + 1, instop);

			auto root = new TreeNode{ data };
			root->left = left;
			root->right = right;

			return root;
		}

	public:

		Constructor(const vector<int>& preorder, const vector<int>& inorder) :
			_preorder{ preorder },
			_inorder{ inorder }
		{
		}

		TreeNode* Construct()
		{
			return Construct(0, _preorder.size(), 0, _inorder.size());
		}
	};


	class Solution 
	{
	public:
		TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder)
		{
			return Constructor{ preorder , inorder}.Construct();
		}
	};
}