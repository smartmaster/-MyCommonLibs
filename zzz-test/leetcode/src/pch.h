#pragma once


#if false


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


struct  SListNode
{
	int _val{ 0 };
	SListNode* _next{ nullptr };

	SListNode(int val) :
		_val{ val }
	{
	}
};


class SList
{
private:
	SListNode* _head{ nullptr };

public:
	void PushFront(int val)
	{
		SListNode* p = new SListNode{ val };

		do
		{
			p->_next = _head;
		} while (InterlockedCompareExchangePointer((VOID**)&_head, p, p->_next) != p->_next);
	}


	void PopFront()
	{
		SListNode* p = nullptr;
		do
		{
			p = _head;
		} while (InterlockedCompareExchangePointer((void**)&_head, p->_next, p) != p);

	}

};



#endif



