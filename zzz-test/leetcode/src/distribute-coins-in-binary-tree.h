#pragma once

//https://leetcode.com/problems/distribute-coins-in-binary-tree/

#include <map>
#include <unordered_map>
#include <tuple>


namespace SmartLib
{
	using namespace ::std;

	class distribute_coins_in_binary_tree
	{
	public:
		struct TreeNode
		{
			int val;
			TreeNode* left;
			TreeNode* right;
			TreeNode() : val(0), left(nullptr), right(nullptr) {}
			TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
			TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
		};

	private:
		map<TreeNode*, tuple<long, long>> _saved;

	private:
		tuple<long, long> DFS_Sum(TreeNode* root)
		{
			if (nullptr == root)
			{
				_saved[nullptr] = make_tuple(0, 0);
				return {0, 0};
			}

			long nodeCuount = 1;
			long coinCount = root->val;

			if (root->left)
			{
				auto [nodeCountLL, coinCountLL] = DFS_Sum(root->left);
				nodeCuount += nodeCountLL;
				coinCount += coinCountLL;
			}

			if (root->right)
			{
				auto [nodeCountRR, coinCountRR] = DFS_Sum(root->right);
				nodeCuount += nodeCountRR;
				coinCount += coinCountRR;
			}

			_saved[root] = make_tuple(nodeCuount, coinCount);
			return { nodeCuount, coinCount };
		}

		static long AbsDiff(long val1, long val2)
		{
			return val1 > val2 ? val1 - val2 : val2 - val1;
		}
		

		long DFS_Move(TreeNode* root)
		{
			if (nullptr == root)
			{
				return 0;
			}

			auto [nodeCountLL, coinCountLL] = _saved[root->left];
			auto [nodeCountRR, coinCountRR] = _saved[root->right];


			long moveLeft = DFS_Move(root->left);
			long moveRight = DFS_Move(root->right);


			return AbsDiff(nodeCountLL, coinCountLL) + AbsDiff(nodeCountRR, coinCountRR) + moveLeft + moveRight;
			
		}

	public:

		static tuple<long, long, long> DFS(TreeNode* root)
		{
			if (nullptr == root)
			{
				return { 0, 0, 0 };
			}

			auto [nodeCountLL, coinCountLL, moveCountLL] = DFS(root->left);
			auto [nodeCountRR, coinCountRR, moveCountRR] = DFS(root->right);

			return 
			{
				1 + nodeCountLL + nodeCountRR,
				root->val + coinCountLL + coinCountRR,
				AbsDiff(nodeCountLL, coinCountLL) + AbsDiff(nodeCountRR, coinCountRR) + moveCountLL + moveCountRR
			};
		}

		static int distributeCoins(TreeNode* root)
		{
			//DFS_Sum(root);
			//long moveCount = DFS_Move(root);
			auto [LL, RR, moveCount] = DFS(root);
			return moveCount;
		}
	};
}


class Solution {
public:
	static int distributeCoins(SmartLib::distribute_coins_in_binary_tree::TreeNode* root)
	{
		SmartLib::distribute_coins_in_binary_tree alg;
		return alg.distributeCoins(root);
	}
};
