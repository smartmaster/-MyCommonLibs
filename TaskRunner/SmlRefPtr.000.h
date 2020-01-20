#pragma once

#include <functional>
#include <atomic>
#include <cassert>

namespace SmartLib
{
	using namespace ::std;

	template<typename DISPOSE = function<void()> >
	class ScopeResource
	{
	private:
		DISPOSE _dispose;
		bool _enabled{ true };

	public:
		ScopeResource(DISPOSE&& dispose) :
			_dispose{static_cast<DISPOSE&&>(dispose)}
		{
		}

		ScopeResource(DISPOSE& dispose) :
			_dispose{ dispose }
		{
		}

		~ScopeResource()
		{
			if (_enabled)
			{
				_dispose();
				_enabled = false;
			}
		}

		void Invoke()
		{
			if (_enabled)
			{
				_dispose();
				_enabled = false;
			}
		}

		void Enable()
		{
			_enabled = true;
		}

		void Disable()
		{
			_enabled = false;
		}
	};

	template<typename T, typename DISPOSE = function<void(T&)>, typename ATOMIC = atomic<long> >
	struct RefObjectBlock
	{
	private:
		T _obj{};
		T* _cookie{ nullptr };
		typename ATOMIC _refcount{ 1 };
		typename DISPOSE _dispose;

	private:
		static constexpr long ObjOffset()
		{
			constexpr static RefObjectBlock* const base = nullptr;
			return ((char*)(&base->_obj) - (char*)(base));
		}

	public:
		static RefObjectBlock* ContainingRecord(T* obj)
		{
			RefObjectBlock* ptr = (RefObjectBlock*)((char*)obj + ObjOffset());
			assert(ptr->IsValid());
			return ptr;
		}

	private:
		//////////////////////////////////////////////////////////////////////////
		RefObjectBlock(const RefObjectBlock& other) = delete;
		RefObjectBlock(RefObjectBlock&& other) = delete;
		const RefObjectBlock& operator=(const RefObjectBlock& other) = delete;
		const RefObjectBlock& operator=(RefObjectBlock&& other) = delete;

	public:
		//////////////////////////////////////////////////////////////////////////
		template<typename ... TARGS>
		static RefObjectBlock* Make(TARGS&& ... args)
		{
			return new RefObjectBlock{ static_cast<TARGS&&>(args) ... };
		}


		template<typename... TARGS>
		RefObjectBlock(TARGS&& ... args) :
			_obj{ static_cast<TARGS&&>(args)... },
			_cookie{ &_obj },
			_refcount{ 1 }
		{
		}

		//////////////////////////////////////////////////////////////////////////
		template<typename TDispose>
		void SetDispose(TDispose&& dispose)
		{
			_dispose = static_cast<TDispose&&>(dispose);
		}
		
		~RefObjectBlock()
		{
			if (_dispose)
			{
				_dispose(_obj);
				_dispose = nullptr;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		T& Object()
		{
			return _obj;
		}

		T* Ptr()
		{
			return &_obj;
		}

		//////////////////////////////////////////////////////////////////////////
		long AddRef()
		{
			return ++_refcount;
		}

		long Release()
		{
			long ref = --_refcount;
			if (0 == ref)
			{
				delete this;
			}
			return ref;
		}

		long RefCount() const
		{
			return _refcount;
		}

		//////////////////////////////////////////////////////////////////////////
		bool IsValid() const
		{
			return _cookie == &_obj;
		}
	};

	template<typename T, typename DISPOSE = function<void(T&)>, typename ATOMIC = atomic<long> >
	class RefPtr
	{
	private:
		RefObjectBlock<T, DISPOSE, ATOMIC>* _objBlockPtr{ nullptr };

	private:
		RefPtr(RefObjectBlock<T, DISPOSE, ATOMIC>* blockPtr) :
			_objBlockPtr{ blockPtr }
		{
		}

	public:
		//////////////////////////////////////////////////////////////////////////
		template<typename ... TAGRS>
		static RefPtr Make(TAGRS&& ... args)
		{
			RefObjectBlock<T, DISPOSE, ATOMIC>* blockPtr = RefObjectBlock<T, DISPOSE, ATOMIC>::Make(static_cast<TAGRS&&>(args)...);
			return RefPtr{ blockPtr };
		}

		template<typename TDispose>
		void SetDispose(TDispose&& dispose)
		{
			assert(_objBlockPtr);
			_objBlockPtr->SetDispose(static_cast<TDispose&&>(dispose));
		}

		~RefPtr()
		{
			if (_objBlockPtr)
			{
				_objBlockPtr->Release();
				_objBlockPtr = nullptr;
			}
		}
		
		//////////////////////////////////////////////////////////////////////////
		RefPtr() :
			_objBlockPtr{ nullptr }
		{
		}


		RefPtr(RefPtr&& other) :
			_objBlockPtr{ other._objBlockPtr }
		{
			other._objBlockPtr = nullptr;
		}


		RefPtr(const RefPtr& other) :
			_objBlockPtr{ other._objBlockPtr }
		{
			if (_objBlockPtr)
			{
				_objBlockPtr->AddRef();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		const RefPtr& operator=(RefPtr&& other)
		{
			if (this != &other) //correct!!
			{
				if (_objBlockPtr)
				{
					_objBlockPtr->Release();
				}
				_objBlockPtr = other._objBlockPtr;
				other._objBlockPtr = nullptr;
			}

			return *this;
		}

		const RefPtr& operator=(const RefPtr& other)
		{
			if (_objBlockPtr != other._objBlockPtr) //correct!!
			{
				if (_objBlockPtr)
				{
					_objBlockPtr->Release();
				}
				_objBlockPtr = other._objBlockPtr;
				if (_objBlockPtr)
				{
					_objBlockPtr->AddRef();
				}
			}

			return *this;
		}


		//////////////////////////////////////////////////////////////////////////
		T* Detach()
		{
			T* ret = _objBlockPtr ? _objBlockPtr->Ptr() : nullptr;
			_objBlockPtr = nullptr;
			return ret;
		}

		void Attach(T* obj)
		{
			if (_objBlockPtr)
			{
				_objBlockPtr->Release();
				_objBlockPtr = nullptr;
			}
			if (obj)
			{
				_objBlockPtr = RefObjectBlock<T, DISPOSE, ATOMIC>::ContainingRecord(obj);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		T& Object()
		{
			assert(_objBlockPtr);
			return _objBlockPtr->Object();
		}

		T* Ptr()
		{
			return _objBlockPtr ? _objBlockPtr->Ptr() : nullptr;
		}

		//////////////////////////////////////////////////////////////////////////
		operator T* ()
		{
			return _objBlockPtr ? _objBlockPtr->Ptr() : nullptr;
		}

		T* operator->()
		{
			return _objBlockPtr ? _objBlockPtr->Ptr() : nullptr;
		}
	};
}