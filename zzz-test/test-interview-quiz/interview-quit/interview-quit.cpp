// interview-quit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"



//////////////////////////////////////////////////////////////////////////
int test_ipp()
{
	int i = 1;
	printf("%d\r\n", ++i/*, ++i, i++*/);
	printf("%d\r\n", i++/*, i++, ++i*/);


	return 0;
}

//////////////////////////////////////////////////////////////////////////
int & f_ref(int & x)
{
	return (++x);
}

int & f_ref2(int & x)
{
	//return (x++);
	return x;
}

//////////////////////////////////////////////////////////////////////////
class CMyInt
{
	int m_val;
public:
	CMyInt(int val) : m_val(val)
	{
	}

	const CMyInt & operator= (const CMyInt & r)
	{
		m_val = r.m_val;
	}

	CMyInt(const CMyInt & r)
	{
		m_val = r.m_val;
	}

	const CMyInt operator ++(int)
	{
		CMyInt temp(m_val);
		m_val ++;
		return temp;
	}

	CMyInt & operator ++ ()
	{
		++m_val;
		return *this;
	}

	const CMyInt operator+(const CMyInt & r)
	{
		CMyInt temp(m_val + r.m_val);
		return temp;
	}


	void print() const
	{
		printf("%d \r\n", m_val);
	}
};

int test_inc()
{
	CMyInt x(10);
	(++ x).print();
	CMyInt x1 = (x ++);
	x1.print();
	return 0;
}

//////////////////////////////////////////////////////////////////////////
int test_sizeof()
{
#define my_text "123456"

	char * p = my_text;
	char p1[] = my_text;

	printf("%d, %d, %d \r\n", sizeof(p), sizeof(p1), sizeof(my_text));

	return 0;
}


//////////////////////////////////////////////////////////////////////////
int test_macro()
{
#define my_square(x) (x * x)
	int a = 10;
	int b = 10;
	printf("%d \r\n", my_square(a + b));
	return 0; 
}

//////////////////////////////////////////////////////////////////////////
/*
c++ cast
pointer value change after cast
*/

//////////////////////////////////////////////////////////////////////////
/*
member function const
*/
int test_const()
{
	int x = 999;
	int y = 888;
	const int * p1 = &x;
	//*p1 = y;

	int * const p2 = &x;
	*p2 = y;
	//p2 = &y;

	const int * const p3 = &x;
	//*p3 = y;
	//p3 = &y;

	return 0;
}

//////////////////////////////////////////////////////////////////////////
void (*Foo)();
void foo();

//////////////////////////////////////////////////////////////////////////
int sum(int n)
{
	if (0 == n)
	{
		return 0;
	}
	return n + sum(n - 1);
}

int test_sum()
{
	printf("%d \r\n", sum(10));
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
single linked list 
-build
-print
-free
- traverse recursive and non-recursive
*/

struct element
{
	int		Data;
	element *	Next;

	element(int __data, element * __next) :
		Data(__data),
		Next(__next)
	{
	}
};


element * BuildList(int count)
{
	element * head = NULL;
	element * p = NULL;
	for (int ii = 0; ii < count; ++ ii)
	{

		if (0 == ii)
		{
			p = new element(ii, NULL);
			head = p;
		}
		else
		{
			p->Next = new element(ii, NULL);
			p = p->Next;
		}
	}
	return head;
}

void FreeList(element * head)
{
	element * p = NULL;
	while (head)
	{
		p = head->Next;
		delete head;
		head = p;
	}
}

void PrintList(element * head)
{
	while (head)
	{
		_ftprintf(stdout, TEXT("%d, "), head->Data);
		head = head->Next;
	}
}

element * ReverseList(element * head)
{
	if (NULL == head)
	{
		return NULL;
	}

	element * p1 = NULL;
	element * p2 = head;
	element * p3 = p2->Next;
	while (p3)
	{
		p2->Next = p1;
		p1 = p2;
		p2 = p3;
		p3 = p3->Next;
	}
	p2->Next = p1;
	return p2;
}


int test_list()
{
	element * head = BuildList(17);
	PrintList(head);
	_ftprintf(stdout, TEXT("\r\n"));

	head = ReverseList(head);
	PrintList(head);
	_ftprintf(stdout, TEXT("\r\n"));

	FreeList(head);

	return 0;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//binary tree
//////////////////////////////////////////////////////////////////////////
#include <stack>
using namespace std;

struct Node
{
	INT		m_ID;
	BOOL	m_IsVisited;
	Node *	m_LeftChild;
	Node *	m_RightChild;

	Node(INT ID, Node * LeftChild = NULL, Node *	RightChild  = NULL) :
		m_ID(ID),
		m_IsVisited(FALSE),
		m_LeftChild(LeftChild),
		m_RightChild(RightChild)
	{
	}
};

Node * GetNotVisistedChild(Node * pNode)
{
	ATLASSERT(pNode);

	Node * p = NULL;

	if (pNode->m_LeftChild && !pNode->m_LeftChild->m_IsVisited)
	{
		p = pNode->m_LeftChild;
	}

	if (p == NULL && pNode->m_RightChild && !pNode->m_RightChild->m_IsVisited)
	{
		p = pNode->m_RightChild;
	}

	return p;
}

struct IVisitor
{
	virtual VOID Visit(Node * pNode) = 0;
};

//non recursive post order traverse
VOID Traverse(Node * pRoot, IVisitor * pPreVistitor, IVisitor * pPostVistitor)
{
	ATLASSERT(pRoot);

	stack<Node*> s;
	s.push(pRoot);
	if (pPreVistitor)	{pPreVistitor->Visit(pRoot);}
	while (!s.empty())
	{
		Node * pTop = s.top();
		Node * pNotVisitedChild = GetNotVisistedChild(pTop);
		if (pNotVisitedChild)
		{
			s.push(pNotVisitedChild);
			if (pPreVistitor)	{pPreVistitor->Visit(pNotVisitedChild);}
		}
		else
		{
			if (pPostVistitor)	{pPostVistitor->Visit(pTop);}
			pTop->m_IsVisited = TRUE;
			s.pop();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
VOID RecursiveTraverse(Node * pRoot, IVisitor * pPreVistitor, IVisitor * pPostVistitor)
{
	if (pRoot)
	{
		if (pPreVistitor)	{pPreVistitor->Visit(pRoot);}
		RecursiveTraverse(pRoot->m_LeftChild, pPreVistitor, pPostVistitor);
		RecursiveTraverse(pRoot->m_RightChild, pPreVistitor, pPostVistitor);
		if (pPostVistitor)	{pPostVistitor->Visit(pRoot);}
	}
}

//////////////////////////////////////////////////////////////////////////
INT GetMid(INT Data, INT * v, INT Begin, INT End)
{
	INT Mid = -1;
	for (INT ii = Begin; ii <= End; ++ ii)
	{
		if (Data == v[ii])
		{
			Mid = ii;
			break;
		}
	}

	return Mid;
}

VOID BuildTree(Node ** ppNode, 
			   INT * v1, INT v1_begin, INT v1_end,
			   INT * v2, INT v2_begin, INT v2_end)
{
	if (v1_begin <= v1_end)
	{
		*ppNode = new Node(v1[v1_begin]);
		INT Mid = GetMid(v1[v1_begin], v2, v2_begin, v2_end);
		ATLASSERT(Mid != -1);

		INT LeftCount = Mid - v2_begin;
		INT RightCount = v2_end - Mid;

		if (LeftCount > 0)
		{
			BuildTree(&((**ppNode).m_LeftChild), 
				v1, v1_begin + 1, v1_begin + LeftCount,
				v2, v2_begin, Mid - 1);
		}

		if (RightCount > 0)
		{
			BuildTree(&((**ppNode).m_RightChild), 
				v1, v1_begin + LeftCount + 1, v1_end,
				v2, Mid + 1, v2_end);
		}
	}
}

int test_bintree(int argc, _TCHAR* argv[])
{
	int v1[] = {1, 2, 4, 5, 3, 6, 7};
	int v2[] = {4, 2, 5, 1, 6, 3, 7};

	//////////////////////////////////////////////////////////////////////////
	Node * root = NULL;
	BuildTree(&root, v1, 0, _countof(v1) - 1, v2, 0, _countof(v2) - 1);

	//////////////////////////////////////////////////////////////////////////
	class  CVisitor : public IVisitor
	{
	public:
		virtual VOID Visit(Node * pNode)
		{
			_ftprintf_s(stdout, TEXT("%d, "), pNode->m_ID);
		}
	};
	CVisitor visitor;

	_ftprintf_s(stdout, TEXT("RecursivePreOrder:\t"));
	RecursiveTraverse(root, &visitor, NULL);
	_ftprintf_s(stdout, TEXT("\r\n"));


	_ftprintf_s(stdout, TEXT("RecursivePostOrder:\t"));
	RecursiveTraverse(root, NULL, &visitor);
	_ftprintf_s(stdout, TEXT("\r\n"));


	_ftprintf_s(stdout, TEXT("NonRecursivePreOrder:\t"));
	Traverse(root, &visitor, NULL);
	_ftprintf_s(stdout, TEXT("\r\n"));

	class CClearVisitFlag : public IVisitor
	{
	public:
		virtual VOID Visit(Node * pNode)
		{
			pNode->m_IsVisited = FALSE;
		}
	};
	CClearVisitFlag ClearVisitFlag;
	RecursiveTraverse(root, &ClearVisitFlag, NULL);

	_ftprintf_s(stdout, TEXT("NonRecursivePostOrder:\t"));
	Traverse(root, NULL, &visitor);
	_ftprintf_s(stdout, TEXT("\r\n"));


	return 0;
}

//////////////////////////////////////////////////////////////////////////
class AA
{
public:
	AA()
	{
		_ftprintf_s(stdout, TEXT("1 %s") TEXT("\r\n"), TEXT(__FUNCTION__));
	}

	~AA()
	{
		_ftprintf_s(stdout, TEXT("2 %s") TEXT("\r\n"), TEXT(__FUNCTION__));
	}

	AA(const AA & r)
	{
		_ftprintf_s(stdout, TEXT("3 %s") TEXT("\r\n"), TEXT(__FUNCTION__));
	}
};

VOID f(AA a)
{
	_ftprintf_s(stdout, TEXT("endter %s") TEXT("\r\n"), TEXT(__FUNCTION__));
	AA aa = a;
	_ftprintf_s(stdout, TEXT("leve %s") TEXT("\r\n"), TEXT(__FUNCTION__));
}

int test_obj_constructor()
{
	AA a;
	f(a);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
struct  IAA
{
	virtual VOID fa() = 0;
};

struct  IBB : public IAA
{
	//struct  IAA
	//{
	//	virtual VOID fa() = 0;
	//};
	virtual VOID fb() = 0;
};

struct ICC
{
	virtual VOID fc() = 0;
};



class  CC : public IBB
{
public:
//	struct  IBB : public IAA
//	{
//		//struct  IAA
//		//{
	virtual VOID fa() 
	{
		_ftprintf_s(stdout, TEXT("%s") TEXT("\r\n"), TEXT(__FUNCTION__));
	}
//		//};
	virtual VOID fb()
	{
		_ftprintf_s(stdout, TEXT("%s") TEXT("\r\n"), TEXT(__FUNCTION__));
	}
//	};
};

class  CC2 :public ICC, public IBB
{
public:
	//struct ICC
	//{
	virtual VOID fc()
	{
		_ftprintf_s(stdout, TEXT("%s") TEXT("\r\n"), TEXT(__FUNCTION__));
	}
	//};


	//	struct  IBB : public IAA
	//	{
	//		//struct  IAA
	//		//{
	virtual VOID fa()
	{
		_ftprintf_s(stdout, TEXT("%s") TEXT("\r\n"), TEXT(__FUNCTION__));
	}
	//		//};
	virtual VOID fb()
	{
		_ftprintf_s(stdout, TEXT("%s") TEXT("\r\n"), TEXT(__FUNCTION__));
	}
	//	};
};

int test_sizeof_class()
{
	_ftprintf_s(stdout, TEXT("%d") TEXT("\r\n"), sizeof(CC));
	_ftprintf_s(stdout, TEXT("%d") TEXT("\r\n"), sizeof(CC2));
	return 0;
}


//////////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
	return test_sizeof_class();
	return test_obj_constructor();
	return test_list();
	return test_sum();
	return test_const();
	return test_macro();
	return test_sizeof();
	return test_inc();
	return test_ipp();
	return 0;
}

