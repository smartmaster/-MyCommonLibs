#pragma once

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#include <atlbase.h>
#include <atlstr.h>
#include <windows.h>

class CVMemAllocator
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


template <class T, class Allocator = CVMemAllocator>
class CVMemPtrBase
{
protected:
	CVMemPtrBase() throw() :
		 m_pData(NULL)
		 {
		 }
		 CVMemPtrBase(_Inout_ CVMemPtrBase<T, Allocator>& p) throw()
		 {
			 m_pData = p.Detach();  // Transfer ownership
		 }
		 explicit CVMemPtrBase(_In_ T* pData) throw() :
		 m_pData(pData)
		 {
		 }

public:
	~CVMemPtrBase() throw()
	{
		Free();
	}

protected:
	CVMemPtrBase<T, Allocator>& operator=(_Inout_ CVMemPtrBase<T, Allocator>& p) throw()
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

template <typename T, class Allocator = CVMemAllocator>
class CVMemPtr :
	public CVMemPtrBase<T, Allocator>
{
public:
	CVMemPtr() throw()
	{
	}
	CVMemPtr(_Inout_ CVMemPtr<T, Allocator>& p) throw() :
	CVMemPtrBase<T, Allocator>(p)
	{
	}
	explicit CVMemPtr(_In_ T* p) throw() :
	CVMemPtrBase<T, Allocator>(p)
	{
	}

	CVMemPtr<T, Allocator>& operator=(_Inout_ CVMemPtr<T, Allocator>& p) throw()
	{
		CVMemPtrBase<T, Allocator>::operator=(p);

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
		CVMemPtr<LONGLONG> apLL;
		apLL.Allocate(1024);
		apLL.Reallocate(2048);
		apLL.Free();
	}
};
