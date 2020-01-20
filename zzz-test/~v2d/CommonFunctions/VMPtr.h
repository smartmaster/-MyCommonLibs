#pragma once

#include "stdafx.h"

class CVMAllocator
{
public:
	static void* Reallocate(
		_In_ void* p,
		_In_ size_t nBytes) throw()
	{
		Free(p);
		return Allocate(nBytes);
	}

	static void* Allocate(_In_ size_t nBytes) throw()
	{
		HRESULT hr = S_OK;
		DWORD dwLastError = 0;

		LPVOID pBuffer = VirtualAlloc(
			NULL,//__in_opt  LPVOID lpAddress,
			nBytes,//__in      SIZE_T dwSize,
			MEM_COMMIT | MEM_RESERVE,//__in      DWORD flAllocationType,
			PAGE_READWRITE//__in      DWORD flProtect
			);
		if (NULL == pBuffer)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
		}

		return pBuffer;
	}

	static void Free(_In_ void* pBuffer) throw()
	{
		VirtualFree(
			pBuffer,//__in  LPVOID lpAddress,
			0,//__in  SIZE_T dwSize,
			MEM_RELEASE//__in  DWORD dwFreeType
			);
	}
};


template <class T, class Allocator = CVMAllocator>
class CVMPtrBase
{
protected:
	CVMPtrBase() throw() :
		 m_pData(NULL)
		 {
		 }
		 CVMPtrBase(_Inout_ CVMPtrBase<T, Allocator>& p) throw()
		 {
			 m_pData = p.Detach();  // Transfer ownership
		 }
		 explicit CVMPtrBase(_In_ T* pData) throw() :
		 m_pData(pData)
		 {
		 }

public:
	~CVMPtrBase() throw()
	{
		Free();
	}

protected:
	CVMPtrBase<T, Allocator>& operator=(_Inout_ CVMPtrBase<T, Allocator>& p) throw()
	{
		if(m_pData != p.m_pData)
			Attach(p.Detach());  // Transfer ownership
		return *this;
	}

public:
	operator T*() const throw()
	{
		return m_pData;
	}

	T* operator->() const throw()
	{
		ATLASSERT(m_pData != NULL);
		return m_pData;
	}

	T** operator&() throw()
	{
		ATLASSUME(m_pData == NULL);
		return &m_pData;
	}

	// Allocate a buffer with the given number of bytes
	bool AllocateBytes(_In_ size_t nBytes) throw()
	{
		ATLASSERT(m_pData == NULL);
		m_pData = static_cast<T*>(Allocator::Allocate(nBytes));
		if (m_pData == NULL)
			return false;

		return true;
	}

	// Attach to an existing pointer (takes ownership)
	void Attach(_In_ T* pData) throw()
	{
		Allocator::Free(m_pData);
		m_pData = pData;
	}

	// Detach the pointer (releases ownership)
	T* Detach() throw()
	{
		T* pTemp = m_pData;
		m_pData = NULL;
		return pTemp;
	}

	// Free the memory pointed to, and set the pointer to NULL
	void Free() throw()
	{
		Allocator::Free(m_pData);
		m_pData = NULL;
	}

	// Reallocate the buffer to hold a given number of bytes
	bool ReallocateBytes(_In_ size_t nBytes) throw()
	{
		T* pNew;

		pNew = static_cast<T*>(Allocator::Reallocate(m_pData, nBytes));
		if (pNew == NULL)
			return false;
		m_pData = pNew;

		return true;
	}

public:
	T* m_pData;
};

template <typename T, class Allocator = CVMAllocator>
class CVMPtr :
	public CVMPtrBase<T, Allocator>
{
public:
	CVMPtr() throw()
	{
	}
	CVMPtr(_Inout_ CVMPtr<T, Allocator>& p) throw() :
	CVMPtrBase<T, Allocator>(p)
	{
	}
	explicit CVMPtr(_In_ T* p) throw() :
	CVMPtrBase<T, Allocator>(p)
	{
	}

	CVMPtr<T, Allocator>& operator=(_Inout_ CVMPtr<T, Allocator>& p) throw()
	{
		CVMPtrBase<T, Allocator>::operator=(p);

		return *this;
	}

	// Allocate a buffer with the given number of elements
	bool Allocate(_In_ size_t nElements = 1) throw()
	{
		size_t nBytes=0;
		if(FAILED(::ATL::AtlMultiply(&nBytes, nElements, sizeof(T))))
		{
			return false;
		}
		return AllocateBytes(nBytes);
	}

	// Reallocate the buffer to hold a given number of elements
	bool Reallocate(_In_ size_t nElements) throw()
	{
		size_t nBytes=0;
		if(FAILED(::ATL::AtlMultiply(&nBytes, nElements, sizeof(T))))
		{
			return false;
		}
		return ReallocateBytes(nBytes);
	}
};


struct TestVMPtr
{
	static VOID Main()
	{
		CVMPtr<LONGLONG> apLL;
		apLL.Allocate(1024);
		apLL.Reallocate(2048);
		apLL.Free();
	}
};
