#pragma once

#include <vector>
#include <list>

#include "log.h"

namespace SmartLib
{
	struct Node
	{
		int _data{ 0 };
		Node * _left{ nullptr };
		Node * _right{ nullptr };
	};

	class ZigZagAlg
	{
	public:

		static void Traverse(Node * root)
		{
			if (nullptr == root)
			{
				return;
			}

			std::list<Node*> queque;
			queque.push_back(root);
			queque.push_back(nullptr);

			while (queque.size())
			{
				auto head = *queque.begin();
				queque.pop_front();

				if (head == nullptr)
				{
					if (queque.size() == 0)
					{
						break;
					}
					queque.push_back(nullptr);
					LOG_LINE(L"");
				}
				else
				{
					LOG(L"%d ", head->_data);

					if (head->_left) queque.push_back(head->_left);
					if (head->_right) queque.push_back(head->_right);
				}
			}
		}

		static void Print(const std::vector<const Node*> & vec, int first, int last)
		{
			for (int ii = first; ii <= last; ++ii)
			{
				LOG(L"%d ", vec[ii]->_data);
			}

			LOG_LINE(L"");
		}


		static void PrintReverse(const std::vector<const Node*> & vec, int first, int last)
		{
			for (int ii = last; ii >= first; --ii)
			{
				LOG(L"%d ", vec[ii]->_data);
			}

			LOG_LINE(L"");
		}


		static void ZigZagNodes(const Node * root)
		{
			std::vector<const Node*> vec1;
			std::vector<const Node*> vec2;

			auto current = &vec1;
			auto next = &vec2;

			if (root)
			{
				current->push_back(root);
			}

			bool order = true;

			while (current->size())
			{
				for (auto node : *current)
				{
					if(node->_left) next->push_back(node->_left);
					if(node->_right) next->push_back(node->_right);
				}

				if (order)
				{
					Print(*current, 0, current->size() - 1);
				}
				else
				{
					PrintReverse(*current, 0, current->size() - 1);
				}


				current->clear();
				auto temp = current;
				current = next;
				next = temp;

				order = !order;
			}
		}


		static void ZigZagOneQueque(const Node * root)
		{
			if (nullptr == root)
			{
				return;
			}


			
			std::vector<int> savedPos;
			savedPos.push_back(-1);

			std::vector<const Node*> queque;
			queque.push_back(root);
			queque.push_back(nullptr);
			savedPos.push_back(queque.size() - 1);

			int pos = 0;
			while (pos < queque.size())
			{
				if (queque[pos] == nullptr)
				{
					if (pos == queque.size() - 1)
					{
						break;
					}
					else
					{
						queque.push_back(nullptr);
						savedPos.push_back(queque.size() - 1);
					}
				}
				else
				{
					if (queque[pos]->_left) queque.push_back(queque[pos]->_left);
					if (queque[pos]->_right) queque.push_back(queque[pos]->_right);
				}

				++pos;
			}


			bool order = true;

			for (int ii = 0; ii < savedPos.size() - 1; ++ ii)
			{
				if (order)
				{
					Print(queque, savedPos[ii] + 1, savedPos[ii + 1] - 1);
				}
				else
				{
					PrintReverse(queque, savedPos[ii] + 1, savedPos[ii + 1] - 1);
				}


				order = !order;
			}
		}

		static void ZigZag(const Node * root)
		{
			if (nullptr == root)
			{
				return;
			}


			std::vector<const Node*> stack1;
			std::vector<const Node*> stack2;

			auto current = &stack1;
			auto next = &stack2;

			bool order = true;

			current->push_back(root);
			while (current->size())
			{
				auto top = current->back();
				LOG(L"%d ", top->_data);
				current->pop_back();

				if (order)
				{
					if(top->_left) next->push_back(top->_left);
					if(top->_right) next->push_back(top->_right);
				}
				else
				{
					if (top->_right) next->push_back(top->_right);
					if (top->_left) next->push_back(top->_left);
				}

				if (current->size() == 0)
				{
					LOG_LINE(L"");
					order = !order;
					auto temp = current;
					current = next;
					next = temp;
				}
			}
		}
	};

	class ZigZagAlgTest
	{
	public:
		static void Case0()
		{
			int count = 30;
			std::vector<Node*> nodes;
			for (int ii = 0; ii < count; ++ ii)
			{
				Node * node = new Node();
				node->_data = ii + 1;
				nodes.push_back(node);
			}


			for (int ii = 0; ii < nodes.size(); ++ ii)
			{
				int left = 2 * ii + 1;
				int right = left + 1;

				if (left < nodes.size())
				{
					nodes[ii]->_left = nodes[left];
				}
				else
				{
					break;
				}
				
				if (right < nodes.size())
				{
					nodes[ii]->_right = nodes[right];
				}
				else
				{
					break;
				}
			}

			ZigZagAlg::ZigZagNodes(nodes[0]);

			ZigZagAlg::ZigZagOneQueque(nodes[0]);

			ZigZagAlg::ZigZag(nodes[0]);

			ZigZagAlg::Traverse(nodes[0]);
		}
	};

}