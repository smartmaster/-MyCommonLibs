#pragma once

#include <cassert>
#include <vector>
#include <iostream>
#include <stack>

namespace SmartLib
{
	using namespace ::std;
	
	class MyTree
	{
	public:
		struct TreeNode
		{
			int val;
			TreeNode* left;
			TreeNode* right;
			TreeNode(int x) : 
				val{ x },
				left{ nullptr },
				right{ nullptr }
			{}
		};


		
	public:

		enum class Order
		{
			PRE,
			IN,
			POST
		};

		static void Traverse(TreeNode* root, Order order)
		{
			struct NodeIndicator 
			{
				TreeNode* _node{ nullptr };
				bool _isData{ false };

				NodeIndicator(TreeNode* node, bool isData) :
					_node{ node },
					_isData{ isData }
				{
				}
			};


			stack<NodeIndicator> stack;
			stack.push(NodeIndicator{ root , false});

			while (stack.size())
			{
				auto top = stack.top();
				stack.pop();
				if (top._node)
				{
					if (top._isData)
					{
						cout << top._node->val << ' ';
					}
					else
					{
						switch (order)
						{
						case Order::PRE:
							stack.push(NodeIndicator{ top._node->right, false });
							stack.push(NodeIndicator{ top._node->left, false });
							stack.push(NodeIndicator{ top._node, true });
							break;
						case Order::IN:
							stack.push(NodeIndicator{ top._node->right, false});
							stack.push(NodeIndicator{ top._node, true});
							stack.push(NodeIndicator{ top._node->left, false });
							break;
						case Order::POST:
							stack.push(NodeIndicator{ top._node, true });
							stack.push(NodeIndicator{ top._node->right, false });
							stack.push(NodeIndicator{ top._node->left, false });
							break;
						default:
							assert(false);
							break;
						}
					}
				}

			}

			cout << endl;

		}

		//-1 means nullptr
		static vector<int> Save(TreeNode* root)
		{
			vector<TreeNode*> nodes;

			nodes.push_back(root);
			long nextIndex = 0;
			while (nextIndex < nodes.size())
			{
				TreeNode* next = nodes[nextIndex];
				if (next == nullptr)
				{
					break;
				}

				nodes.push_back(next->left);
				nodes.push_back(next->right);

				++nextIndex;

				while (nextIndex < nodes.size() && nullptr == nodes[nextIndex])
				{
					++nextIndex;
				}
			}


			vector<int> data(nodes.size());
			for (long ii = 0; ii < nodes.size(); ++ ii)
			{
				data[ii] = nodes[ii] ? nodes[ii]->val : -1;
			}

			return data;

		}

		static TreeNode* Load(const vector<int>& data)
		{
			vector<TreeNode*> nodes(data.size());
			for (long ii = 0; ii < nodes.size(); ++ ii)
			{
				if (data[ii] != -1)
				{
					nodes[ii] = new TreeNode{ data[ii] };
				}
			}

			long nextIndex = 0;
			long childIndex = 1;
			while (nextIndex < nodes.size() && childIndex < nodes.size())
			{
				if (nodes[nextIndex] == nullptr)
				{
					break;
				}

				nodes[nextIndex]->left = nodes[childIndex++];
				nodes[nextIndex]->right = nodes[childIndex++];

				++nextIndex;
				while (nextIndex < nodes.size() && nullptr == nodes[nextIndex])
				{
					++nextIndex;
				}
			}


			return nodes[0];
		}


		static void Connect(TreeNode* node1, TreeNode* node2)
		{
			if (node1)
			{
				node1->right = node2;
			}

			if (node2)
			{
				node2->left= node1;
			}
		}

		static TreeNode* RightMost(TreeNode* root)
		{
			if (root == nullptr)
			{
				return nullptr;
			}

			while (root->right)
			{
				root = root->right;
			}

			return root;
		}

		static TreeNode* LeftMost(TreeNode* root)
		{
			if (root == nullptr)
			{
				return nullptr;
			}

			while (root->left)
			{
				root = root->left;
			}

			return root;
		}

		static TreeNode* ConvertToList(TreeNode* root)
		{
			if (root == nullptr)
			{
				return nullptr;
			}

			if (root->left == nullptr && root->right == nullptr)
			{
				return root;
			}

			TreeNode* prev = RightMost(root->left);

			TreeNode* head1 = ConvertToList(root->left);
			TreeNode* head2 = ConvertToList(root->right);

			if (prev)
			{
				Connect(prev, root);
			}
			else
			{
				head1 = root;
			}

			Connect(root, head2);

			return head1;
		}


		static void PrintTreeList(TreeNode* root)
		{
			while (root)
			{
				cout << root->val << ' ';
				root = root->right;
			}

			cout << endl;
		}


	};
}