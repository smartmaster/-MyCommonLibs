#pragma once

#include "stdafx.h"

class CBinTree;

struct IVisitor
{
	virtual VOID Operate(CBinTree * pTree) = NULL;
};

struct IEnterNode
{
	virtual VOID Operate(CBinTree * pTree) = NULL;
};

struct ILeaveNode
{
	virtual VOID Operate(CBinTree * pTree) = NULL;
};

class CBinTree
{
public:
	INT m_Value;
	CBinTree * m_Left;
	CBinTree * m_Right;

private:
	CBinTree()
	{
	}

public:
	static CBinTree* NewItem(INT Value, CBinTree * Left, CBinTree * Right, ULONG * pNewCount)
	{
		CBinTree * pTree = new CBinTree;
		pTree->m_Value = Value;
		pTree->m_Left = Left;
		pTree->m_Right = Right;

		if (pNewCount)
		{
			++ (*pNewCount);
		}

		return pTree;
	}

	static VOID DeleteItem(CBinTree* pTree, ULONG * pDeleteCount)
	{
		pTree->m_Left = NULL;
		pTree->m_Right = NULL;
		delete pTree;

		if (pDeleteCount)
		{
			++(*pDeleteCount);
		}
	}

	static VOID DeleteTree(CBinTree* pTree, ULONG * pDeleteCount)
	{
		if (pTree->m_Left)
		{
			DeleteTree(pTree->m_Left, pDeleteCount);
		}
		if (pTree->m_Right)
		{
			DeleteTree(pTree->m_Right, pDeleteCount);
		}
		DeleteItem(pTree, pDeleteCount);
	}

	VOID InserTree(CBinTree * pTree)
	{
		if (pTree->m_Value > m_Value)
		{
			if (NULL == m_Right)
			{
				m_Right = pTree;
			}
			else
			{
				m_Right->InserTree(pTree);
			}
		}
		else if (pTree->m_Value < m_Value)
		{
			if (NULL == m_Left)
			{
				m_Left = pTree;
			}
			else
			{
				m_Left->InserTree(pTree);
			}
		}
		else
		{
			_ftprintf_s(stdout, TEXT("value %d already exsit") TEXT("\r\n"), pTree->m_Value);
		}
	}

	VOID Pre(IVisitor * pVisitor, IEnterNode * pEnter, ILeaveNode * pLeave)
	{
		if (pEnter)
		{
			pEnter->Operate(this);
		}

		if (pVisitor)
		{
			pVisitor->Operate(this);
		}

		if (m_Left)
		{
			m_Left->Pre(pVisitor, pEnter, pLeave);
		}
		if (m_Right)
		{
			m_Right->Pre(pVisitor, pEnter, pLeave);
		}

		if (pLeave)
		{
			pLeave->Operate(this);
		}
	}

	VOID Post(IVisitor * pVisitor, IEnterNode * pEnter, ILeaveNode * pLeave)
	{
		if (pEnter)
		{
			pEnter->Operate(this);
		}

		if (m_Left)
		{
			m_Left->Post(pVisitor, pEnter, pLeave);
		}

		if (m_Right)
		{
			m_Right->Post(pVisitor, pEnter, pLeave);
		}

		if (pVisitor)
		{
			pVisitor->Operate(this);
		}

		if (pLeave)
		{
			pLeave->Operate(this);
		}
	}

	VOID Mid(IVisitor * pVisitor, IEnterNode * pEnter, ILeaveNode * pLeave)
	{
		if (pEnter)
		{
			pEnter->Operate(this);
		}

		if (m_Left)
		{
			m_Left->Mid(pVisitor, pEnter, pLeave);
		}

		if (pVisitor)
		{
			pVisitor->Operate(this);
		}

		if (m_Right)
		{
			m_Right->Mid(pVisitor, pEnter, pLeave);
		}

		if (pLeave)
		{
			pLeave->Operate(this);
		}
	}

	CBinTree * Find(INT Value)
	{
		CBinTree * pTree = NULL;
		if (Value == m_Value)
		{
			pTree = this;
		}
		else if (Value < m_Value)
		{
			if (m_Left)
			{
				pTree = m_Left->Find(Value);
			}
		}
		else 
		{
			if (m_Right)
			{
				pTree = m_Right->Find(Value);
			}
		}

		return pTree;
	}
};

class CConvertListVisitor : public IVisitor
{
public:
	CBinTree * m_pHead;
	CBinTree * m_pCurrent;
public:

	CConvertListVisitor() :
	  m_pHead(NULL),
		  m_pCurrent(NULL)
	  {
	  }

	  virtual VOID Operate(CBinTree * pTree)
	  {
		  if (m_pCurrent)
		  {
			  m_pCurrent->m_Right = pTree;
			  pTree->m_Left = m_pCurrent;
			  //pTree->m_Right = NULL;
		  }
		  else
		  {
			  //pTree->m_Left = NULL;
			  //pTree->m_Right = NULL;
			  m_pHead = pTree;
		  }

		  m_pCurrent = pTree;
	  }
};

class CSimpleVisitor : public IVisitor
{
public:
	virtual VOID Operate(CBinTree * pTree)
	{
		_ftprintf_s(stdout, TEXT("%d") TEXT(", "), pTree->m_Value);
	}
};

class CEnterNode : public IEnterNode
{
	ULONG & m_Depth;
public:
	CEnterNode(ULONG & Depth) :
	  m_Depth(Depth)
	  {
	  }

	  virtual VOID Operate(CBinTree * pTree)
	  {
		  for (ULONG ii=0; ii<m_Depth; ++ii)
		  {
			  _ftprintf_s(stdout, TEXT("\t"));
		  }
		  _ftprintf_s(stdout, TEXT("%d") TEXT("\r\n"), pTree->m_Value);

		  ++m_Depth;
	  }
};

class CLeaveNode : public ILeaveNode
{
	ULONG & m_Depth;
public:

	CLeaveNode(ULONG & Depth) :
	  m_Depth(Depth)
	  {
	  }

	  virtual VOID Operate(CBinTree * pTree)
	  {
		  --m_Depth;
	  }
};


struct NS_CONSTRUCT_TREE
{
	static INT * SplitMid(INT * midBegin, INT* midEnd, INT RootValue)
	{
		INT *p = NULL;
		for (INT* ptr=midBegin; ptr<midEnd; ++ptr)
		{
			if (RootValue == *ptr)
			{
				p  = ptr;
				break;
			}
		}
		return p;
	}

	static VOID ConstructTree(
		CBinTree ** ppRoot, 
		INT * preBegin, INT* preEnd,
		INT * midBegin, INT* midEnd)
	{
		if (preBegin && preBegin < preEnd)
		{
			if (NULL == *ppRoot)
			{
				*ppRoot = CBinTree::NewItem(0, NULL, NULL, NULL);
			}
			(**ppRoot).m_Value = *preBegin;

			INT * pMid = SplitMid(midBegin, midEnd, *preBegin);
			if (pMid)
			{
				INT_PTR leftCount = pMid - midBegin;
				INT_PTR rightCount = midEnd - pMid -1;
				if (leftCount)
				{
					ConstructTree(&((**ppRoot).m_Left), preBegin + 1, preBegin + 1 + leftCount, midBegin, pMid);
				}
				if (rightCount)
				{
					ConstructTree(&((**ppRoot).m_Right), preEnd - rightCount, preEnd, pMid + 1, midEnd);
				}
			}
		}
	}
};

struct NS_BINTREE_HELPER
{
	static VOID PrintTree(CBinTree * pRoot)
	{
		ULONG Depth = 0;
		CEnterNode Enter(Depth);
		CLeaveNode Leave(Depth);
		pRoot->Pre(NULL, &Enter, &Leave);
		_ftprintf_s(stdout, TEXT("\r\n\r\n\r\n"));

		//Depth = 0;
		//pRoot->Post(NULL, &Enter, &Leave);
		//_ftprintf_s(stdout, TEXT("\r\n\r\n\r\n"));

		//Depth = 0;
		//pRoot->Mid(NULL, &Enter, &Leave);
		//_ftprintf_s(stdout, TEXT("\r\n\r\n\r\n"));


		CSimpleVisitor Visitor;

		pRoot->Pre(&Visitor, NULL,  NULL);
		_ftprintf_s(stdout, TEXT("\r\n\r\n\r\n"));

		pRoot->Post(&Visitor, NULL,  NULL);
		_ftprintf_s(stdout, TEXT("\r\n\r\n\r\n"));

		pRoot->Mid(&Visitor, NULL,  NULL);
		_ftprintf_s(stdout, TEXT("\r\n\r\n\r\n"));
	}
};