#pragma once

#include <vector>
#include <iostream>

namespace SmartLib
{
	using namespace ::std;

	class MySList
	{
	public:
		class Node
		{
		public:
			long _data{0L};
			Node* _next{ nullptr };


			Node()
			{

			}


			Node(long data, Node* next) :
				_data{ data, },
				_next{ next }//,
			{
			}
		};


	public:

		static Node* CreateList(long count, vector<Node*>* nodes)
		{
			Node head;
			Node* cur = &head;

			for (long ii = 0; ii < count; ++ ii)
			{
				cur->_next = new Node{ii, nullptr};
				cur = cur->_next;

				if (nodes)
				{
					nodes->push_back(cur);
				}
			}


			return head._next;
		}


		static void Destroy(Node* head)
		{
			while (head)
			{
				Node* next = head->_next;
				delete head;
				head = next;
			}
		}


		static void Print(Node* head)
		{
			while (head)
			{
				cout << head->_data << ' ';
				head = head->_next;
			}

			cout << endl;
		}

		static Node* HaveLoop(Node* head)
		{
			Node* slow = head;
			Node* fast = head;

			while (fast)
			{
				slow = slow->_next;
				fast = fast->_next;
				fast = fast ? fast->_next : nullptr;

				if (slow == fast)
				{
					break;
				}
			}

			return fast;
		}

		static long LoopLength(Node* node)
		{
			long len = 0;
			Node* start = node;

			do 
			{
				++len;
				start = start->_next;

			} while (start != node);

			return len;
		}


		static Node* FirstJoint(Node* head)
		{
			Node* joint = HaveLoop(head);
			if (joint)
			{
				long len = LoopLength(joint);

				Node* p1 = head;
				Node* p2 = head;
				while (len)
				{
					p2 = p2->_next;
					--len;
				}

				while (p1 != p2)
				{
					p1 = p1->_next;
					p2 = p2->_next;
				}



				return p1;
			}


			return nullptr;
		}


		static Node* PushFront(Node* head, Node* node, long kk)
		{
			while (kk && node)
			{
				auto next = node->_next;

				node->_next = head->_next;
				head->_next = node;

				node = next;

				--kk;

			}

			return node;
		}


		static Node* ReverseListK(Node* head, long kk)
		{
			Node newHead{};

			auto node = head;
			head = &newHead;
			while (node)
			{
				auto nextnode = PushFront(head, node, kk);
				head = node;
				node = nextnode;
			}
			

			return newHead._next;
		}

	};
}