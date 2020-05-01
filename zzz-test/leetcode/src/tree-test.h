#pragma once

#include <vector>
#include <deque>
#include <stack>
#include <iostream>

namespace SmartLib
{
	using namespace ::std;

	class TreeTest
	{
	public:
		struct Node
		{
			long _val{ 0 };
			Node* _left{ nullptr };
			Node* _right{ nullptr };


			Node(long val, Node* LL = nullptr, Node* RR = nullptr) :
				_val{val},
				_left{ LL},
				_right{ RR }//,
			{
			}
		};


	public:
		static void CreateTree(vector<Node*>& nodes, long nodeCount)
		{
			nodes.resize(nodeCount);
			for (long ii = 0; ii < nodeCount; ++ ii)
			{
				nodes[ii] = new Node{ ii };
			}

			for (long ii = 0; ii < nodeCount; ++ ii)
			{
				long left = 2 * ii + 1;
				long right = left + 1;

				if (left >= nodeCount)
				{
					break;
				}
				nodes[ii]->_left = nodes[left];

				if (right >= nodeCount)
				{
					break;
				}
				nodes[ii]->_right = nodes[right];
			}
		}

	public:
		static void BFS(Node* root)
		{
			if (root == nullptr)
			{
				return;
			}

			deque<Node*> que;
			que.push_back(root);
			long currentSize = 1;
			long nextSize = 0;

			while (que.size())
			{
				auto front = que.front();
				que.pop_front();
				--currentSize;
				cout << front->_val << " ";

				if (front->_left)
				{
					que.push_back(front->_left);
					++nextSize;
				}

				if (front->_right)
				{
					que.push_back(front->_right);
					++nextSize;
				}


				if (currentSize == 0)
				{
					currentSize = nextSize;
					nextSize = 0;

					cout << endl;
				}
			}
		}


	public:
		static void ZigZag(Node* root)
		{
			if (root == nullptr)
			{
				return;
			}

			stack<Node*> s1;
			s1.push(root);

			stack<Node*> s2;

			for (;;)
			{
				if (s1.empty() && s2.empty())
				{
					break;
				}

				while (s1.size())
				{
					auto top = s1.top();
					s1.pop();

					cout << top->_val << " ";

					if (top->_left)
					{
						s2.push(top->_left);
					}

					if (top->_right)
					{
						s2.push(top->_right);
					}
				}
				cout << endl;

				while (s2.size())
				{
					auto top = s2.top();
					s2.pop();

					cout << top->_val << " ";

					if (top->_right)
					{
						s1.push(top->_right);
					}

					if (top->_left)
					{
						s1.push(top->_left);
					}
				}
				cout << endl;
			}
		}

	};
}