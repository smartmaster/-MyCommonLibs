#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "SimpleUnknown.h"
#include "StringOf.h"

namespace SmartLib
{
	class IAllocator : public ISimpleUnknown
	{
		//struct ISimpleUnknown : public IUnknown
		//{
		//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
		//	//	IUnknown
		//	//{
		//	//public:
		//	//	BEGIN_INTERFACE
		//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
		//	//		/* [in] */ REFIID riid,
		//	//		/* [iid_is][out] */ void **ppvObject) = 0;

		//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

		//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

		//	//	END_INTERFACE
		//	//};

		//	virtual LPCTSTR WhoAmI() = 0;
		//	virtual HRESULT CreateMember() = 0;
		//	virtual HRESULT DestroyMember() = 0;
		//};
	public:
		virtual void* Allocate(size_t size) = 0;
		virtual void Free(void * pmem) = 0;
	};

	class VMemAllocator : public IAllocator
	{
		SML_IMP_CLASS_REF(VMemAllocator);

	public:
		virtual HRESULT CreateMember() { return S_OK; }
		virtual HRESULT DestroyMember() { return S_OK; }

	public:
		virtual void* Allocate(size_t size) override
		{
			return ::VirtualAlloc(
				nullptr,//_In_opt_ LPVOID lpAddress,
				size,//_In_     SIZE_T dwSize,
				MEM_COMMIT | MEM_RESERVE,//_In_     DWORD  flAllocationType,
				PAGE_READWRITE //_In_     DWORD  flProtect
			);
		}
		virtual void Free(void * pmem)
		{
			::VirtualFree(
				pmem,//_In_ LPVOID lpAddress,
				0,//_In_ SIZE_T dwSize,
				MEM_RELEASE//_In_ DWORD  dwFreeType
			);
		}
	};


	class HeapAllocator : public IAllocator
	{
		SML_IMP_CLASS_REF(HeapAllocator);

	public:
		virtual HRESULT CreateMember() { return S_OK; }
		virtual HRESULT DestroyMember() { return S_OK; }

	public:
		virtual void* Allocate(size_t size) override
		{
			return ::HeapAlloc(GetProcessHeap(), 0, size);
		}
		virtual void Free(void * pmem)
		{
			::HeapFree(GetProcessHeap(), 0, pmem);
		}
	};

	class CrtAllocator : public IAllocator
	{
		SML_IMP_CLASS_REF(CrtAllocator);

	public:
		virtual HRESULT CreateMember() { return S_OK; }
		virtual HRESULT DestroyMember() { return S_OK; }
	public:
		virtual void* Allocate(size_t size) override
		{
			return ::malloc(size);
		}
		virtual void Free(void * pmem)
		{
			::free(pmem);
		}
	};

	class CppAllocator : public IAllocator
	{
		SML_IMP_CLASS_REF(CppAllocator);

	public:
		virtual HRESULT CreateMember() { return S_OK; }
		virtual HRESULT DestroyMember() { return S_OK; }
	public:
		virtual void* Allocate(size_t size) override
		{
			return new BYTE[size];
		}
		virtual void Free(void * pmem) override
		{
			return delete[] pmem;
		}
	};


	class IAllocatorTest
	{
	public:
		static void Case0()
		{
			auto  temp = SML_EXPANDED_MACRO_STRING(SML_IMP_CLASS_REF(IAllocatorTest));

			CComPtr<IAllocator> myAlloc;
			CREATE_INSTCANCE_COBJECT(new VMemAllocator, &myAlloc, TRUE);

			void * mem = myAlloc->Allocate(100);
			myAlloc->Free(mem);
			mem = nullptr;

			myAlloc.Release();

		}
	};
}