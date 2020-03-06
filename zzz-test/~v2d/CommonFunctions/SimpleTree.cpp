#include "stdafx.h"
#include "SimpleTree.h"
#include <vector>
using namespace  std;

namespace /*NS_NONAME*/
{
	typedef NS_TREE::CSimpleTree<CString> MY_TREE;

	class CTraverseCallback :
		public NS_TREE::ITraverseCallback<CString>
	{
		vector<CString> m_Paths;
		CString m_strCurrent;

		virtual INT Enter(MY_TREE & Tree)
		{
			m_strCurrent = Tree.GetValue();
			m_Paths.push_back(m_strCurrent);
			return 0;
		}

		virtual INT Operate(MY_TREE & Tree)
		{
			CString strPath;
			for (size_t ii=0; ii<m_Paths.size(); ++ ii)
			{
				if (0 == ii)
				{
					strPath = m_Paths[ii];
				}
				else
				{
					strPath += TEXT("\\");
					strPath += m_Paths[ii];
				}
			}

			_ftprintf_s(stdout, TEXT("%s") TEXT("\r\n"), strPath.GetString());
			return 0;
		}

		virtual INT Leave(MY_TREE & Tree)
		{
			m_Paths.pop_back();
			return 0;
		}
	};
}


VOID TestSimpleTree()
{
	LONG nCountNew = 0;
	LONG nCountDelete = 0;

	MY_TREE * psub0 = MY_TREE::AllocateNode(TEXT("!!dest"), &nCountNew);

	MY_TREE * psub10 = MY_TREE::AllocateNode(TEXT("DSC_3407"), &nCountNew);
	MY_TREE * psub11 = MY_TREE::AllocateNode(TEXT("DSC_3411"), &nCountNew);
	MY_TREE * psub12 = MY_TREE::AllocateNode(TEXT("DSC_3426"), &nCountNew);
	
	MY_TREE * psub1 = MY_TREE::AllocateNode(TEXT("xxxxxx_20090614L"), &nCountNew);
	psub1->AddChildTree(psub10);
	psub1->AddChildTree(psub11);
	psub1->AddChildTree(psub12);

	MY_TREE * psub2 = MY_TREE::AllocateNode(TEXT("zzz.vhd"), &nCountNew);


	MY_TREE * proot = MY_TREE::AllocateNode(TEXT("D:\\###Temp"), &nCountNew);
	proot->AddChildTree(psub0);
	proot->AddChildTree(psub1);
	proot->AddChildTree(psub2);

	CTraverseCallback TraverseCallback;
	proot->PreOrderTraverse(&TraverseCallback);

	_ftprintf_s(stdout, TEXT("\r\n\r\n\r\n\r\n"));
	proot->PostOrderTraverse(&TraverseCallback);

	_ftprintf_s(stdout, TEXT("\r\n\r\n\r\n\r\n"));
	proot->Traverse(&TraverseCallback, TRUE);

	_ftprintf_s(stdout, TEXT("\r\n\r\n\r\n\r\n"));
	proot->Traverse(&TraverseCallback, FALSE);

	MY_TREE::FreeTree(proot, &nCountDelete);
	proot = NULL;

	_ftprintf_s(stdout, TEXT("\r\n\r\n\r\n\r\n"));
	_ftprintf_s(stdout, TEXT("new=%u, delete=%u"), nCountNew, nCountDelete);
}