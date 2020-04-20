#pragma once

//https://leetcode.com/problems/binary-tree-level-order-traversal/description/

#include <vector>
#include <deque>

/*


// Definition for a binary tree node.
  struct TreeNode 
  {
      int val;
      TreeNode *left;
      TreeNode *right;
      TreeNode(int x) : val(x), left(NULL), right(NULL) {}
  };
 
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {

    }
};

*/


namespace SmartLib
{
    using namespace ::std;

    class binary_tree_level_order_traversal
    {
    private:
        struct TreeNode
        {
            int val;
            TreeNode* left;
            TreeNode* right;
            TreeNode(int x) : val(x), left(NULL), right(NULL) {}
        };

    public:
        static vector<vector<int>> levelOrder(TreeNode* root)
        {
            deque<TreeNode*> queue;
            queue.push_back(root);
            queue.push_back(nullptr);


            vector<vector<int>> vvr;
            vvr.reserve(64);

            while (queue.size() && queue.front())
            {
                vvr.resize(vvr.size() + 1);
                for (;;)
                {
                    auto front = queue.front();
                    queue.pop_front();
                    if (front)
                    {
                        vvr.back().push_back(front->val);
                        if (front->left)
                        {
                            queue.push_back(front->left);
                        }

                        if (front->right)
                        {
                            queue.push_back(front->right);
                        }

                    }
                    else
                    {
                        queue.push_back(nullptr);
                        break;
                    }
                }
            }

            return vvr;
        }

    };
}