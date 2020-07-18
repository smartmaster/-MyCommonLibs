#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

namespace SmartLib
{
	using namespace  ::std;


#define  CREATE_INSTANCE(cls) \
 \
	static IUnknown* CreateInstance() \
	{ \
		auto pobj = new cls{}; \
		if (pobj) \
		{ \
			pobj->AddRef(); \
		} \
 \
		return pobj ? pobj->IUnknownPtr() : nullptr; \
	}


	MIDL_INTERFACE("0B155B9C-2228-44A4-B017-83F2B0E00138")
		IInternalUnknown : public IUnknown
	{
		virtual HRESULT STDMETHODCALLTYPE InternalQueryInterface(
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR * __RPC_FAR * ppvObject) = 0;

		virtual ULONG STDMETHODCALLTYPE InternalAddRef(void) = 0;

		virtual ULONG STDMETHODCALLTYPE InternalRelease(void) = 0;

		virtual void STDMETHODCALLTYPE SetOutter(IUnknown* outer) = 0;
	};


	class Helper
	{
	public:
		static bool EqualGuid(const GUID& id1, const GUID& id2)
		{
			return 0 == memcmp(&id1, &id2, sizeof(GUID));
		}

	public:
		template<typename T>
		static IInternalUnknown* CreateIInternalUnknown(IUnknown* outter)
		{
			CComPtr<IUnknown> spunk;
			spunk.Attach(T::CreateInstance()); //create internal object instance
			IInternalUnknown* piunk{ nullptr };
			if (spunk)
			{
				spunk.QueryInterface(&piunk);
				spunk.Release(); //must call Release() before SetOutter(...), or it will dec reft count of the outter

				if (piunk)
				{
					piunk->SetOutter(outter);
				}
			}

			return piunk;
		}
	};


	template<typename T, typename ... TARGS>
	class CImplenment :
		protected T,
		protected CImplenment<TARGS...>
	{

	protected:
		void* Query(REFIID iid)
		{
			if (Helper::EqualGuid(__uuidof(T), iid))
			{
				return (void*)(T*)(this);
			}
			else
			{
				return CImplenment<TARGS...>::Query(iid);
			}
		}
	};

	template<typename T>
	class CImplenment<T> :
		protected T
	{
	protected:
		void* Query(REFIID iid)
		{
			if (Helper::EqualGuid(__uuidof(T), iid))
			{
				return (void*)(T*)(this);
			}
			else
			{
				return nullptr;
			}
		}
	};




	template<typename ... TARGS>
	class CUnknown :
		protected CImplenment<IUnknown, IInternalUnknown, TARGS ...>
	{
	private:
		IUnknown* _outer{ nullptr };
		LONG _refCount{ 0 };

	private:
		LONG _id{ 0 };

	private:
		static LONG GetId()
		{
			static LONG id{ 0 };
			return InterlockedIncrement(&id);
		}

	protected:
		CUnknown()
		{
			_id = GetId();
			cout << "Constructor " << __FUNCTION__ << " " << _id << endl;
		}

	protected:
		virtual ~CUnknown()
		{
			cout << "Destructor " << __FUNCTION__ << " " << _id << endl;
		}



	protected:
		HRESULT STDMETHODCALLTYPE QueryInterface(
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override
		{
			return
				_outer ?
				_outer->QueryInterface(riid, ppvObject) :
				InternalQueryInterface(riid, ppvObject);
		}

	protected:
		ULONG STDMETHODCALLTYPE AddRef(void) override
		{
			return
				_outer ?
				_outer->AddRef() :
				InternalAddRef();
		}

	protected:
		ULONG STDMETHODCALLTYPE Release(void) override
		{
			return
				_outer ?
				_outer->Release() :
				InternalRelease();
		}

	protected:
		HRESULT STDMETHODCALLTYPE InternalQueryInterfaceImpl( //helper function used to implement InternalQueryInterface
			REFIID riid,
			void** ppvObject)
		{
			*ppvObject = CImplenment<IUnknown, IInternalUnknown, TARGS ...>::Query(riid);
			if (*ppvObject)
			{
				AddRef(); // call AddRef() not InternalAddRef()
			}

			return *ppvObject ? S_OK : E_NOINTERFACE;
		}

	protected:
		virtual HRESULT STDMETHODCALLTYPE InternalQueryInterface(
			REFIID riid,
			void** ppvObject) = 0;

	protected:
		ULONG STDMETHODCALLTYPE InternalAddRef(void) override
		{
			return InterlockedIncrement(&_refCount);
		}

	protected:
		ULONG STDMETHODCALLTYPE InternalRelease(void) override
		{
			LONG count = InterlockedDecrement(&_refCount);
			if (0 == count)
			{
				delete this;
			}

			return count;
		}

	protected:
		void STDMETHODCALLTYPE SetOutter(IUnknown* outer)  override
		{
			_outer = outer;
		}


	protected:
		IUnknown* IUnknownPtr()
		{
			return (IUnknown*)(CImplenment<IUnknown, IInternalUnknown, TARGS ...>*)(this);
		}
	};

	
}

