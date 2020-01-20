#pragma once

#include <windows.h>

#include <memory>
#include <functional>
#include <cassert>

namespace SmartLib
{
	template<typename T>
	struct ObjWrap
	{
	private:
		T _obj;
		T* _cookie;
		long _refcount;
		std::function<void(T&)> _dispose;

	private:
		template<typename... TARGS>
		ObjWrap(TARGS&& ... args) :
			_obj{ std::forward<TARGS>(args)... },
			_cookie(&this->Ref()),
			_refcount(1)
		{
		}

	public:
		ObjWrap(ObjWrap&& other) :
			_obj{std::move(other._obj)},
			_cookie{&this->Ref()},
			_refcount{1},
			_dispose{std::move(other._dispose)}
		{
		}


		static ObjWrap* ContainingRecord(T* obj)
		{
			ObjWrap* ptr = CONTAINING_RECORD(obj, ObjWrap, _obj);
			assert(ptr->IsValid());
			return ptr;
		}

		

	public:
		template<typename ... TARGS>
		static ObjWrap* New(TARGS&& ... args)
		{
			return new ObjWrap{ std::forward<TARGS>(args) ... };
		}


		template<typename TDispose, typename ... TARGS>
		static ObjWrap* NewWithDispose(TDispose&& dispose, TARGS&& ... args)
		{
			auto ptr = new ObjWrap{ std::forward<TARGS>(args) ... };
			ptr->_dispose = std::forward<TDispose>(dispose);
			return ptr;
		}

		template<typename ... TARGS>
		static ObjWrap Scoped(TARGS&& ... args)
		{
			return ObjWrap{ std::forward<TARGS>(args)... };
		}

		template<typename TDispose, typename ... TARGS>
		static ObjWrap ScopedWithDispose(TDispose&& dispose, TARGS&& ... args)
		{
			auto obj{ ObjWrap(std::forward<TARGS>(args)...) };
			obj._dispose = std::forward<TDispose>(dispose);
			//return std::move(obj);
			return obj; //compiler is clever, no need to call move() explicitly
		}


		~ObjWrap()
		{
			if (_dispose)
			{
				_dispose(_obj);
			}
		}

		LONG AddRef()
		{
			return ::InterlockedIncrement(&_refcount);
		}

		LONG Release()
		{
			LONG ref = ::InterlockedDecrement(&_refcount);
			if (0 == ref)
			{
				delete this;
			}
			return ref;
		}

		LONG RefCount() const
		{
			return _refcount;
		}

		bool IsValid() const
		{
			return _cookie == &_obj;
		}

		template<typename TDispose>
		void SetDispose(TDispose && dispose)
		{
			_dispose = std::forward<TDispose>(dispose);
		}


		T& Ref()
		{
			return _obj;
		}
	};

	template<typename T>
	class RefCountPtr
	{
	private:
		ObjWrap<T>* _ptr{ nullptr };

	private:
		RefCountPtr(ObjWrap<T>* ptr) :
			_ptr{ ptr }
		{
		}

	public:

		template<typename ... TAGRS>
		static RefCountPtr Make(TAGRS&& ... args)
		{
			ObjWrap<T>* ptr = ObjWrap<T>::New( std::forward<TAGRS>(args)... );
			return RefCountPtr{ ptr };
		}


		template<typename TDispose, typename ... TAGRS>
		static RefCountPtr MakeWithDispose(TDispose && dispose, TAGRS&& ... args)
		{
			ObjWrap<T>* ptr = ObjWrap<T>::NewWithDispose(std::forward<TDispose>(dispose), std::forward<TAGRS>(args)...);
			//ptr->SetDispose(std::forward<TDispose>(dispose));
			return RefCountPtr{ ptr };
		}

		RefCountPtr() :
			_ptr{ nullptr }
		{
		}


		RefCountPtr(RefCountPtr&& other) :
			_ptr{ other._ptr }
		{
			other._ptr = nullptr;
		}


		RefCountPtr(const RefCountPtr& other) :
			_ptr{ other._ptr }
		{
			if (_ptr)
			{
				_ptr->AddRef();
			}
		}


		const RefCountPtr& operator=(RefCountPtr&& other)
		{
			if (_ptr)
			{
				_ptr->Release();
			}
			_ptr = other._ptr;
			other._ptr = nullptr;

			return *this;
		}

		const RefCountPtr& operator=(const RefCountPtr& other)
		{
			if (_ptr != other._ptr)
			{
				if (_ptr)
				{
					_ptr->Release();
				}
				_ptr = other._ptr;
				if (_ptr)
				{
					_ptr->AddRef();
				}
			}

			return *this;
		}

		~RefCountPtr()
		{
			if (_ptr)
			{
				_ptr->Release();
				_ptr = nullptr;
			}
		}

		T* Detach()
		{
			T* ret = _ptr ? &_ptr->Ref() : nullptr;
			_ptr = nullptr;
			return ret;
		}

		void Attach(T* obj)
		{
			if (_ptr)
			{
				_ptr->Release();
				_ptr = nullptr;
			}
			if (obj)
			{
				_ptr = ObjWrap<T>::ContainingRecord(obj);
			}

		}

		T& Ref()
		{
			assert(_ptr);
			return _ptr->Ref();
		}

		T* Ptr()
		{
			return _ptr ? &_ptr->Ref() : nullptr;
		}

		operator T* ()
		{
			return _ptr ? &_ptr->Ref() : nullptr;
		}

		T* operator->()
		{
			return _ptr ? &_ptr->Ref() : nullptr;
		}
	};
}