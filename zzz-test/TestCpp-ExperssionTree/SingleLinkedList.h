#pragma once

#include <cassert>

#include "log.h"

namespace SmartLib
{
	struct SLLNode
	{
		int _data{ 0 };
		SLLNode * _next{ nullptr };
	};


	class SLL
	{
	public:
		static SLLNode * Create(int count)
		{
			assert(count > 0);

			SLLNode * head = new SLLNode();
			SLLNode * current = head;
			current->_data = 0;
			for (int ii = 1; ii < count; ++ii)
			{
				current->_next = new SLLNode();
				current = current->_next;
				current->_data = ii;
			}

			return head;
		}


		static void Print(SLLNode * node)
		{
			while (node)
			{
				LOG_A("%d ", node->_data);
				node = node->_next;
			}
			LOG_LINE_A("");
		}


		static void Free(SLLNode * node)
		{
			SLLNode * curret = node;
			SLLNode * next = nullptr;
			while (curret)
			{
				next = curret->_next;
				delete curret;
				curret = next;
			}
		}

		static SLLNode * Reverse(SLLNode * node)
		{
			SLLNode * p1 = node;
			SLLNode * p2 = p1 ? p1->_next : nullptr;
			SLLNode * p3 = p2 ? p2->_next : nullptr;

			p1->_next = nullptr;
			while (p2)
			{
				p2->_next = p1;
				p1 = p2;
				p2 = p3;
				p3 = p3 ? p3->_next : nullptr;
			}


			return p1;
		}
	};

	class SLLTest
	{
	public:
		static void Case0()
		{
			SLLNode * sll = SLL::Create(7);
			SLL::Print(sll);

			SLLNode * reversed = SLL::Reverse(sll);
			SLL::Print(reversed);


			SLL::Free(reversed);
			reversed = nullptr;
		}
	};
}
