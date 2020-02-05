#pragma once

#include <vector>

//https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-tree/

namespace SmartLib
{
	using namespace ::std;

	class lowest_common_ancestor_of_a_binary_tree
	{
	public:
		struct TreeNode {
			int val;
			TreeNode* left;
			TreeNode* right;
			TreeNode(int x) : 
				val(x), 
				left(nullptr), 
				right(nullptr) 
			{
			}
		};
	
	public:
		vector<TreeNode*> _stack1;
		//vector<TreeNode*> _stack2;
		vector<TreeNode*> _stack;
		long _find{ 0 };
		TreeNode* _pp;
		TreeNode* _qq;

		void DFS(TreeNode* root)
		{
			//if (_find == 2)
			//{
			//	return;
			//}

			if (root)
			{
				_stack.push_back(root);
				if (root == _pp || root == _qq)
				{
					if (_find == 0)
					{
						_stack1 = _stack;
						++_find;
					}
					else if (_find == 1)
					{
						//_stack2 = move(_stack);
						++_find;
						//throw 0L;
						return;
					}
				}

				DFS(root->left);
				DFS(root->right);

				if (_find != 2)
				{
					_stack.pop_back();
				}
			}
		}

		TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) 
		{
			if (p == q)
			{
				return p;
			}

			_pp = p;
			_qq = q;

			//try
			{
				DFS(root);
			}
			//catch (long)
			//{
			//}
			
			if (_find < 2)
			{
				return nullptr;
			}

			long ii = 0;

			for (; ii < _stack1.size() && ii < _stack.size(); ++ ii)
			{
				if (_stack1[ii] != _stack[ii])
				{
					break;
				}
			}


			return ii < _stack1.size() ? _stack1[ii - 1] : _stack[ii - 1];
		}
	};
}