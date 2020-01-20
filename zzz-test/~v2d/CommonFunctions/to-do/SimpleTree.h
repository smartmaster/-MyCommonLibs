#pragma once

#include "stdafx.h"
#include <vector>

namespace NS_TREE
{
	using std::vector;

	template<class NODE_TYPE>
	class CSimpleTree;

	template<class NODE_TYPE>
	class ITraverseCallback
	{
	public:
		virtual INT Enter(CSimpleTree<NODE_TYPE> & Tree) = NULL;
		virtual INT Operate(CSimpleTree<NODE_TYPE> & Tree) = NULL;
		virtual INT Leave(CSimpleTree<NODE_TYPE> & Tree) = NULL;
	};

	template<class NODE_TYPE>
	struct TraverseStackItem
	{
		CSimpleTree<NODE_TYPE> * m_pTree;
		LONG m_nCountPushedChild;

		TraverseStackItem(CSimpleTree<NODE_TYPE> * pTree) :
		m_pTree(pTree),
		m_nCountPushedChild(0)
		{
		}
	};

	template<class NODE_TYPE>
	class CSimpleTree
	{
	private:
		NODE_TYPE m_Value;
		CSimpleTree * m_pParent;
		vector<CSimpleTree*> m_ChildTrees;

	public:
		static CSimpleTree * AllocateNode(CONST NODE_TYPE & Value, LONG * pCountNew)
		{
			CSimpleTree * pTree = new CSimpleTree(Value);

			if (pTree)
			{
				if (pCountNew)
				{
					++(*pCountNew);
				}
			}

			return pTree;
		}

		static VOID FreeNode(CSimpleTree * pTtree,  LONG * pCountDelete)
		{
			if (NULL == pTtree)
			{
				return /*E_INVALIDARG*/;
			}

			if (pCountDelete)
			{
				++(*pCountDelete);
			}

			delete pTtree;
		}

		static VOID FreeTree(CSimpleTree * pTtree,  LONG * pCountDelete)
		{
			if (NULL == pTtree)
			{
				return /*E_INVALIDARG*/;
			}

			vector<CSimpleTree*> & ChildTrees = pTtree->GetChildTrees();
			for (size_t ii=0; ii<ChildTrees.size(); ++ii)
			{
				FreeTree(ChildTrees[ii], pCountDelete); //recursive call
			}
			ChildTrees.clear();

			FreeNode(pTtree,  pCountDelete);
		}

	private:
		CSimpleTree(CONST NODE_TYPE & Value/*, CSimpleTree * pParent = NULL*/) :
		m_Value(Value),
		m_pParent(NULL/*pParent*/)
		{
		}

		~CSimpleTree()
		{
		}

	public:
		CSimpleTree * GetParent()
		{
			return m_pParent;
		}

		NODE_TYPE & GetValue()
		{
			return m_Value;
		}

		vector<CSimpleTree*> & GetChildTrees()
		{
			return m_ChildTrees;
		}

		size_t GetChildCount()
		{
			return m_ChildTrees.size();
		}

		VOID AddChildTree(CSimpleTree * pSubTree)
		{
			pSubTree->m_pParent = this;
			m_ChildTrees.push_back(pSubTree);
		}
		
		VOID PreOrderTraverse(ITraverseCallback<NODE_TYPE> * pOperate)
		{
			if (pOperate) 
			{
				pOperate->Enter(*this);
				pOperate->Operate(*this);
			}

			for (size_t ii=0; ii<m_ChildTrees.size(); ++ii)
			{
				m_ChildTrees[ii]->PreOrderTraverse(pOperate); //recursive call
			}

			if (pOperate) 
			{
				pOperate->Leave(*this);
			}
		}

		VOID PostOrderTraverse(ITraverseCallback<NODE_TYPE> * pOperate)
		{
			if (pOperate) 
			{
				pOperate->Enter(*this);
			}

			for (size_t ii=0; ii<m_ChildTrees.size(); ++ii)
			{
				m_ChildTrees[ii]->PostOrderTraverse(pOperate); //recursive call
			}

			if (pOperate)
			{
				pOperate->Operate(*this);
				pOperate->Leave(*this);
			}
		}

		VOID Traverse(ITraverseCallback<NODE_TYPE> * pOperate, BOOL bPreOrder)
		{
			typedef TraverseStackItem<NODE_TYPE> StackItem;

			vector<StackItem> vecStack;
			vecStack.push_back(StackItem(this));

			if (pOperate)
			{
				StackItem & Top = vecStack.back();
				pOperate->Enter(*Top.m_pTree);
				if (bPreOrder)
				{
					pOperate->Operate(*Top.m_pTree);
				}
			}

			while (vecStack.size())
			{
				StackItem & OldTopItem = vecStack.back();
				vector<CSimpleTree*> & ChildTrees = OldTopItem.m_pTree->GetChildTrees();

				if (OldTopItem.m_nCountPushedChild < OldTopItem.m_pTree->GetChildCount())
				{
					++OldTopItem.m_nCountPushedChild;
					vecStack.push_back(StackItem(ChildTrees[OldTopItem.m_nCountPushedChild-1]));
					//++OldTopItem.m_nCountPushedChild; //can crash  because OldTopItem can be an invalid reference after push_back()

					if (pOperate)
					{
						StackItem & Top = vecStack.back();
						pOperate->Enter(*Top.m_pTree);
						if (bPreOrder)
						{
							pOperate->Operate(*Top.m_pTree);
						}
					}

				}
				else
				{
					if (pOperate)
					{
						StackItem & Top = vecStack.back();

						if (!bPreOrder)
						{
							pOperate->Operate(*Top.m_pTree);
						}

						pOperate->Leave(*Top.m_pTree);
					}
					
					vecStack.pop_back();
				}
			}
		}
	};
}