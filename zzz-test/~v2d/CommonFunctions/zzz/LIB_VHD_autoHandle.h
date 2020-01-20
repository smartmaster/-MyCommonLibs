#pragma once

#include "stdafx.h"

typedef BOOL(WINAPI *pfn_CSafeHandle_CloseHandle)(HANDLE hObj);

template <HANDLE TInvalidVal = INVALID_HANDLE_VALUE, pfn_CSafeHandle_CloseHandle TCloseHandle = ::CloseHandle>
class CSafeHandle
{
	HANDLE  m_hVal;

public:
	CSafeHandle()
	{
		m_hVal = TInvalidVal;
	}

	CSafeHandle(CSafeHandle& refVal)
	{
		m_hVal = refVal.m_hVal;
	}

	CSafeHandle(HANDLE hVal)
	{
		m_hVal = hVal;
	}

	virtual ~CSafeHandle()
	{
		if( m_hVal != TInvalidVal )
		{
			TCloseHandle(m_hVal);
			m_hVal = TInvalidVal;
		}
	}

	inline CSafeHandle& operator=(HANDLE hVal)
	{
		m_hVal = hVal;
		return *this;
	}

	inline bool operator==(HANDLE hVal) CONST
	{
		return m_hVal == hVal;
	}

	inline bool operator==(CSafeHandle& refVal) CONST
	{
		return m_hVal == refVal.m_hVal;
	}

	inline bool operator!=(HANDLE hVal) CONST
	{
		return m_hVal != hVal;
	}

	inline bool operator!=(CSafeHandle& refVal) CONST
	{
		return m_hVal != refVal.m_hVal;
	}

	inline operator HANDLE(VOID)
	{
		return m_hVal;
	}

	inline HANDLE* operator&(VOID)
	{
		return &m_hVal;
	}
};

template<class _TYPE>
class CHeapObj
{
protected:
	_TYPE*   m_pObj;

public:
	CHeapObj()
	{
		m_pObj = new _TYPE;
	}

	~CHeapObj()
	{
		if( m_pObj )
		{
			delete m_pObj;
			m_pObj = NULL;
		}
	}

	_TYPE* operator->()
	{
		return m_pObj;
	}

	operator _TYPE*()
	{
		return m_pObj;
	}

	operator CONST _TYPE*() CONST
	{
		return m_pObj;
	}

	operator VOID*()
	{
		return m_pObj;
	}
};

template<class _TYPE>
class CHeapArray
{
protected:
	_TYPE*   m_pArray;
public:
	CHeapArray(INT nCount)
	{
		m_pArray = new _TYPE[nCount];
	}

	~CHeapArray()
	{
		if( m_pArray )
		{
			delete[] m_pArray;
			m_pArray = NULL;
		}
	}

	inline _TYPE& operator[](INT nIndex)
	{
		return m_pArray[nIndex];
	}

	inline _TYPE& operator[](ULONG nIndex)
	{
		return m_pArray[nIndex];
	}

	inline _TYPE& operator[](size_t nIndex)
	{
		return m_pArray[nIndex];
	}

	operator _TYPE* () CONST
	{
		return m_pArray;
	}

	operator CONST _TYPE* () CONST
	{
		return m_pArray;
	}

	operator VOID*()
	{
		return m_pArray;
	};
};
