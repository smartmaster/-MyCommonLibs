#pragma once


#include <vector>
#include <deque>
#include <iostream>

#include "logger.h"

namespace SmartLib
{

	using namespace ::std;

	class binary_tree_zigzag_level_order_traversal
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
		template<typename TC>
		static void Print(TC& c)
		{
			for (auto& item : c)
			{
				SmartLib::Logger::Print(cout, item, ' ');
			}

			SmartLib::Logger::Print(cout) << endl;
		}

		static void Test1()
		{
			vector<TreeNode*> vec(6);

			for (int ii = 1; ii < vec.size(); ++ ii)
			{
				vec[ii] = new TreeNode{ ii };
			}


			vec[1]->left = vec[2];
			vec[1]->right = vec[3];

			vec[2]->left = vec[4];

			vec[3]->right = vec[5];

			auto vvr = zigzagLevelOrder(vec[1]);

			for (const auto& vec : vvr)
			{
				Print(vec);
			}
			
		}


		static vector<vector<int>> zigzagLevelOrder(TreeNode* root) 
		{
			if (root == nullptr)
			{
				return {};
			}


			vector<vector<int>> vvr;
			vvr.reserve(64);
			vvr.resize(1);

			vector<TreeNode*> queque1;
			vector<TreeNode*> queque2;

			bool flag = true;;
	
			queque1.push_back(root);
			vvr.back().push_back(root->val);


			for (;;)
			{
				if (queque1.empty() && queque2.empty())
				{
					break;
				}

				vvr.resize(vvr.size() + 1);
				auto& vec = vvr.back();

				if (flag)
				{
					while (queque1.size())
					{
						auto back = queque1.back();
						queque1.pop_back();

						if (back->right)
						{
							queque2.push_back(back->right);
							vec.push_back(back->right->val);
						}

						if (back->left)
						{
							queque2.push_back(back->left);
							vec.push_back(back->left->val);
						}
					}
				}
				else
				{
					while (queque2.size())
					{
						auto back = queque2.back();
						queque2.pop_back();

						if (back->left)
						{
							queque1.push_back(back->left);
							vec.push_back(back->left->val);
						}

						if (back->right)
						{
							queque1.push_back(back->right);
							vec.push_back(back->right->val);
						}
					}
				}


				flag = !flag;
			}
			
		
			if (vvr.back().empty())
			{
				vvr.resize(vvr.size() - 1);
			}
			
			return vvr;
		}

	};
}