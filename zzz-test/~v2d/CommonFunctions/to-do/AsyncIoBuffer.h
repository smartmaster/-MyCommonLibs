#pragma once 

#include "stdafx.h"
#include <vector>
using namespace std;

struct CAsyncIoBufferItem
{
	OVERLAPPED m_OverLapped; //MUST be first member for easily convert from OVERLAPPED* to CBufferItemSimpleNTFS*
	ULONG m_nMetaInformationLength;
	ULONG m_nMaximumDataLength;
	BOOL m_bAllocated;
	BYTE m_Buffer[1];

	//******************************************
	static CAsyncIoBufferItem * Allocate(ULONG nMetaDataLength, ULONG nMaxDataLength);
	static VOID Free(CAsyncIoBufferItem * pAsyncIoBufferItem);
};

class CAsyncIoSharedBuffer
{
private: //******************************************
	INT m_nItemCount;
	INT m_nAllocatedItemCount;
	static CONST INT DELTA = 2;

private: //******************************************
	HANDLE m_hSemaphore; //created inside
	CComCriticalSection m_cs; 
	vector<CAsyncIoBufferItem*> m_SharedBuffers; //created inside

	//******************************************
	CAsyncIoBufferItem * FindNotUsedBufferItem();

public: //******************************************
	CAsyncIoSharedBuffer();
	virtual ~CAsyncIoSharedBuffer();

	INT CreateMember(ULONG nMetaDataLength, ULONG nMaxDataLength, INT nItemCount);
	INT DestroyMember();

	CAsyncIoBufferItem * AllocateItem(DWORD dwTimeout);
	INT RecycleItem( CAsyncIoBufferItem * pBufferItem );
	INT RecycleAllItems();

private: //******************************************
	CAsyncIoSharedBuffer(CONST CAsyncIoSharedBuffer&);
	CAsyncIoSharedBuffer& operator=(CONST CAsyncIoSharedBuffer&);
};