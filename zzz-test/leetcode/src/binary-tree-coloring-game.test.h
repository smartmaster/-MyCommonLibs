#pragma once
#include "binary-tree-coloring-game.h"

namespace SmartLib
{
	class binary_tree_coloring_game_test
	{
	public:
		static void Case0()
		{
			auto root = new binary_tree_coloring_game::TreeNode{ 1 };
			auto left = new binary_tree_coloring_game::TreeNode{ 2 };
			auto right = new binary_tree_coloring_game::TreeNode{ 3 };

			root->left = left;
			root->right = right;
			bool ok = SmartLib::binary_tree_coloring_game::Solution(root, 3, 1);
			return;
		}
	};
}