#pragma once

#include <iostream>
#include <cassert>
#include <vector>
using std::vector;

namespace SmartLib
{
	class ZigzagReview
	{
	public:
		class TreeNode
		{
		public:
			int _data{ 0 };
			TreeNode * _left{ nullptr };
			TreeNode * _right{ nullptr };

			TreeNode(int data = 0, TreeNode * left = nullptr, TreeNode * right = nullptr) :
				_data(data),
				_left(left),
				_right(right)
			{

			}
		};


		static void CreateTree(int count, vector<TreeNode*> & vec)
		{
			assert(count > 0);
			vec.resize(count);
			for (int ii = 0; ii < count; ++ ii)
			{
				TreeNode * temp = new TreeNode(ii);
				vec[ii] = temp;
			}


			for (int ii = 0; ii < count; ++ ii)
			{
				int left = 2 * ii + 1;
				int right = left + 1;

				if (left < count)
				{
					vec[ii]->_left = vec[left];
				}
				else
				{
					break;
				}

				if (right < count)
				{
					vec[ii]->_right = vec[right];
				}
				else
				{
					break;
				}
			}
		}

		static void Destroy(vector<TreeNode*> & vec)
		{
			for (auto node : vec)
			{
				delete node;
			}
			vec.clear();
		}


		static void Zigzag(TreeNode * node)
		{
			assert(node != nullptr);

			vector<TreeNode*> stack1;
			vector<TreeNode*> stack2;

			vector<TreeNode*> * current = &stack1;
			vector<TreeNode*> * next = &stack2;

			current->push_back(node);
			bool order = true;

			while (current->size())
			{
				TreeNode * top = current->back();
				std::cout << top->_data << ' '; //print
				current->pop_back();

				if (order)
				{
					if (top->_left)
					{
						next->push_back(top->_left);
					}
					
					if (top->_right)
					{
						next->push_back(top->_right);
					}
				}
				else
				{
					if (top->_right)
					{
						next->push_back(top->_right);
					}

					if (top->_left)
					{
						next->push_back(top->_left);
					}
				}

				if (current->size() == 0)
				{
					order = !order;
					auto temp = current;
					current = next;
					next = temp;
					std::cout << std::endl; //print line
				}

			}
		}


		static void Print(const vector<TreeNode*> & vec, int first, int last)
		{
			for (int ii = first; ii <= last; ++ ii)
			{
				printf("%d ", vec[ii]->_data);
			}

			printf("\r\n");
		}


		static void PrintReverse(const vector<TreeNode*> & vec, int first, int last)
		{
			for (int ii = last; ii >= first; --ii)
			{
				printf("%d ", vec[ii]->_data);
			}

			printf("\r\n");
		}

		static void ZigzagQueue(TreeNode * node)
		{
			assert(node != nullptr);

			vector<int> pos;
			vector<TreeNode*> queue;
			queue.push_back(node);

			queue.push_back(nullptr);
			pos.push_back(queue.size() - 1);

			int index = 0;

			while (index < queue.size())
			{
				if (queue[index] == nullptr)
				{
					if (index == queue.size() - 1)
					{
						break;
					}
					else
					{
						queue.push_back(nullptr);
						pos.push_back(queue.size() - 1);
					}
				}
				else
				{
					if (queue[index]->_left)
					{
						queue.push_back(queue[index]->_left);
					}

					if (queue[index]->_right)
					{
						queue.push_back(queue[index]->_right);
					}
				}

				++index;
			}


			int first = 0; 
			bool order = true;
			for (int ii = 0; ii < pos.size(); ++ ii)
			{
				int last = pos[ii] - 1;

				if (order)
				{
					Print(queue, first, last);
				}
				else
				{
					PrintReverse(queue, first, last);
				}

				order = !order;
				first = pos[ii] + 1;
			}
		}


		static void Case1()
		{
			vector<TreeNode*> vec;
			CreateTree(100, vec);
			Zigzag(vec[0]);
			ZigzagQueue(vec[0]);
			Destroy(vec);
		}
	};
}