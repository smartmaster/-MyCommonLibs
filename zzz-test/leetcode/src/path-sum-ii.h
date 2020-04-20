#pragma once

#include <vector>
#include <deque>
#include <iostream>

namespace SmartLib
{
	using namespace  ::std;

	class path_sum_ii
	{
	private:
		struct TreeNode
		{
			int val;
			TreeNode* left;
			TreeNode* right;
			TreeNode(int x) : val(x), left(NULL), right(NULL) {}
		};

	private:

		//TreeNode* _root; /***INCOMMING***/
		int _sum{ 0 }; /***INCOMMING***/

		int _currentSum{ 0 };
		vector<int > _currentStack;

		vector<vector<int>> _vvr;
	

	private:
		void DFS(TreeNode* root)
		{





			//if (nullptr == root->left && nullptr == root->right) //leaf node
			if (nullptr == root) 
			{
				if (_currentSum == _sum)
				{
					_vvr.push_back(_currentStack);
				}
				return;
			}



			_currentSum += root->val;
			_currentStack.push_back(root->val);


			if (_currentSum <= _sum)
			{

				DFS(root->left);
				DFS(root->right);
			}

			_currentSum -= root->val;
			_currentStack.pop_back();

		}


		path_sum_ii(int sum) :
			//_root{ root },
			_sum { sum }
		{
		}


		static void Print(const vector<int>& vec)
		{
			for (auto data : vec)
			{
				cout << data << ' ';
			}

			cout << endl;
		}

		static void Print(const vector<vector<int>>& vvec)
		{
			for (const auto& vec : vvec)
			{
				Print(vec);
			}
		}

		static vector<vector<int>> GetTreeNodes(TreeNode* root)
		{
			if (nullptr == root)
			{
				return {};
			}

			deque<TreeNode*> que;
			que.push_back(root);
			que.push_back(nullptr);

			vector<vector<int>> vvr{ 64 };
			vvr.resize(vvr.size() + 1);
			vvr.back().push_back(root->val);
			
			while (que.front())
			{
				vvr.resize(vvr.size() + 1);
				auto& vec = vvr.back();

				while (auto front = que.front())
				{
					que.pop_front();

					if (front->left)
					{
						que.push_back(front->left);
						vec.push_back(front->left->val);
					}

					if (front->right)
					{
						que.push_back(front->right);
						vec.push_back(front->right->val);
					}
				}

				que.pop_front();
				que.push_back(nullptr);
			} 
			

			return vvr;
		}


		static vector<TreeNode*> BuildTree(const vector<int>& data)
		{
			vector<TreeNode*> nodes(data.size());

			for (int ii = 0; ii < data.size(); ++ ii)
			{
				if (data[ii] > 0)
				{
					nodes[ii] = new TreeNode{ data[ii] };
				}
			}


			for (int ii = 0; ii < nodes.size(); ++ ii)
			{
				if (nodes[ii])
				{
					//////////////////////////////////////////////////////////////////////////
					int left = 2 * ii + 1;
					if (left >= nodes.size())
					{
						break;
					}
					nodes[ii]->left = nodes[left];

					//////////////////////////////////////////////////////////////////////////
					int right = left + 1;
					if (right >= nodes.size())
					{
						break;
					}
					nodes[ii]->right = nodes[right];
				}
			}


			return nodes;
		}


	public:
		static vector<vector<int>> pathSum(TreeNode* root, int sum) 
		{
			path_sum_ii ps{sum};
			ps.DFS(root);
			return move(ps._vvr);
		}


	public:
		static void Test()
		{
			vector<int> data{ 5,4,8,11,-1,13,4,7,2,-1,-1,5,1 };
			vector<TreeNode*> nodes = BuildTree(data);

			auto nodesData = GetTreeNodes(nodes[0]);
			Print(nodesData);

			auto vvr = pathSum(nodes[0], 22);
			Print(vvr);
		}

	};
}