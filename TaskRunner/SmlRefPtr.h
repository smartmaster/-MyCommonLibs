#pragma once

#include <functional>
#include <atomic>
#include <cassert>

namespace SmartLib
{
	using namespace ::std;

	template<typename START = function<void()> >
	class ScopeStart
	{
	public:
		ScopeStart(START&& start)
		{
			if (start)
			{
				start();
			}
		}
	};

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

	class RefPtrMakeTag {};


	template<typename T, typename DISPOSE = function<void(T&)>, typename ATOMIC = atomic<long> >
	class RefPtr
	{
	public:
		//template<typename T, typename DISPOSE = function<void(T&)>, typename ATOMIC = atomic<long> >
		struct ObjectBlock
		{
		private:
			constexpr static T* _DESTRUCTING()
			{
				T* const ptr = (T*)(void*)(-1);
				return ptr;
			}

			class ObjMemory
			{
				char _mem[sizeof(T)];
			};
			

		private:
			ObjMemory _objMem; //in order to not to call ~T() more than once
			T* /*const*/ _shadow{ nullptr };
			typename ATOMIC _refcount{ 1 };
			typename ATOMIC _weakRefcount{ 0 };
			typename DISPOSE _dispose;

		private:
			static constexpr long ObjOffset()
			{
				constexpr static ObjectBlock* const base = nullptr;
				return ((char*)(&base->_objMem) - (char*)(base));
			}

			//T& MemToObj()
			//{
			//	return *(reinterpret_cast<T*>(&_objMem));
			//}


			void DestroyObject()
			{
				if (_shadow)
				{
					T* shadow = _shadow;
					_shadow = _DESTRUCTING();

					if (_dispose)
					{
						_dispose(*shadow/*MemToObj()*/);
						_dispose = nullptr;
					}
					shadow->~T();
					_shadow = nullptr;
				}
			}

		public:
			static ObjectBlock* ContainingRecord(T* obj)
			{
				ObjectBlock* ptr = (ObjectBlock*)((char*)obj + ObjOffset());
				assert(ptr->IsValid());
				return ptr;
			}

		private:
			//////////////////////////////////////////////////////////////////////////
			ObjectBlock(const ObjectBlock& other) = delete;
			ObjectBlock(ObjectBlock&& other) = delete;
			const ObjectBlock& operator=(const ObjectBlock& other) = delete;
			const ObjectBlock& operator=(ObjectBlock&& other) = delete;

		public:
			//////////////////////////////////////////////////////////////////////////
			template<typename ... TARGS>
			static ObjectBlock* Make(TARGS&& ... args)
			{
				return new ObjectBlock{ static_cast<TARGS&&>(args) ... };
			}


			template<typename... TARGS>
			ObjectBlock(TARGS&& ... args) :
				//_obj{ static_cast<TARGS&&>(args)... },
				_shadow{ reinterpret_cast<T*>(&_objMem) },
				_refcount{ 1 },
				_weakRefcount{ 0 }
			{
				new (&_objMem)T{ static_cast<TARGS&&>(args)... };
			}

			//////////////////////////////////////////////////////////////////////////
			template<typename TDispose>
			void SetDispose(TDispose&& dispose)
			{
				_dispose = static_cast<TDispose&&>(dispose);
			}

			~ObjectBlock()
			{
				DestroyObject();
			}

			//////////////////////////////////////////////////////////////////////////
			T& Ref()
			{
				return *_shadow;
			}

			T* Ptr()
			{
				return _shadow;
			}

			//////////////////////////////////////////////////////////////////////////
			long AddRef()
			{
				return ++_refcount;
			}

			long Release()
			{
				long ref = --_refcount;


				if (0 == ref) //destroy if shared ref count reaches zero
				{
					DestroyObject();
				}

				if (0 == ref &&  0 == _weakRefcount)
				{
					assert(nullptr == _shadow);
					delete this;
				}
				return ref;
			}

			long RefCount() const
			{
				return _refcount;
			}

			//////////////////////////////////////////////////////////////////////////
			long WeakAddRef()
			{
				return ++_weakRefcount;
			}

			long WeakRelease()
			{
				long ref = --_weakRefcount;
				if (0 == ref && 0 == _refcount /*&& _shadow*/) //2020-2-8 //!!@@## done: how to handle self point to self
				{
					if (_shadow != _DESTRUCTING())
					{
						assert(nullptr == _shadow);
						delete this;
					}
				}
				return ref;
			}

			long WeakRefCount() const
			{
				return _weakRefcount;
			}

			//////////////////////////////////////////////////////////////////////////
			bool IsValid() const
			{
				return (const char*)_shadow == (const char*)&_objMem;
			}
		};

	private:
		ObjectBlock* _objBlockPtr{ nullptr };

	private:
		RefPtr(ObjectBlock* blockPtr) :
			_objBlockPtr{ blockPtr }
		{
		}

	public:
		//////////////////////////////////////////////////////////////////////////
		template<typename ... TAGRS>
		static RefPtr Make(TAGRS&& ... args)
		{
			ObjectBlock* blockPtr = ObjectBlock::Make(static_cast<TAGRS&&>(args)...);
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
		template<typename ... TAGRS>
		RefPtr(RefPtrMakeTag&& /*tag*/, TAGRS&& ... args) :
			_objBlockPtr{ ObjectBlock::Make(static_cast<TAGRS&&>(args)...) }
		{
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

		RefPtr(T* other) //T* other must be dettached or nullptr
		{
			Attach(other);
		}

		//////////////////////////////////////////////////////////////////////////
		const RefPtr& operator=(RefPtr&& other)
		{
			if (this != &other) //correct!! no need to move from self to self
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
			if (_objBlockPtr != other._objBlockPtr) //correct!! no need to assign if they point to same object
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

		const RefPtr& operator=(T* other) //T* other must be dettached or nullptr
		{
			Attach(other);
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
			if (nullptr == obj)
			{
				if (_objBlockPtr)
				{
					_objBlockPtr->Release();
					_objBlockPtr = nullptr;
				}
			}
			else
			{
				ObjectBlock* blockPtr = ObjectBlock::ContainingRecord(obj);
				if (_objBlockPtr != blockPtr)
				{
					if (_objBlockPtr)
					{
						_objBlockPtr->Release();
					}
					_objBlockPtr = blockPtr;
				}
			}
			
		}

		//////////////////////////////////////////////////////////////////////////
		T& Ref()
		{
			assert(_objBlockPtr);
			return _objBlockPtr->Ref();
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


		T& operator*()
		{
			assert(_objBlockPtr);
			return _objBlockPtr->Ref();
		}

		//////////////////////////////////////////////////////////////////////////
		bool operator==(const RefPtr& other)
		{
			return _objBlockPtr == other._objBlockPtr;
		}

		bool operator!=(const RefPtr& other)
		{
			return _objBlockPtr != other._objBlockPtr;
		}

		bool operator==(T* other)
		{
			return Ptr() == other;
		}

		bool operator!=(T* other)
		{
			return Ptr() != other;
		}


		//////////////////////////////////////////////////////////////////////////
		class Weak
		{
		private:
			ObjectBlock* _sharedObjBlockPtr{ nullptr };

		public:

			Weak()
			{

			}

			Weak(RefPtr& shared) :
				_sharedObjBlockPtr{shared._objBlockPtr}
			{
				if (_sharedObjBlockPtr)
				{
					_sharedObjBlockPtr->WeakAddRef();
				}
			}


			Weak(Weak& other) :
				_sharedObjBlockPtr{other._sharedObjBlockPtr }
			{
				if (_sharedObjBlockPtr)
				{
					_sharedObjBlockPtr->WeakAddRef();
				}
			}

			Weak(Weak&& other) :
				_sharedObjBlockPtr{ other._sharedObjBlockPtr }
			{
				other._sharedObjBlockPtr = nullptr;
			}

			~Weak()
			{
				if (_sharedObjBlockPtr)
				{
					_sharedObjBlockPtr->WeakRelease();
					_sharedObjBlockPtr = nullptr;
				}
			}

			//////////////////////////////////////////////////////////////////////////
			const Weak& operator=(Weak&& other)
			{
				if (this != &other) //correct!! no need to move from self to self
				{
					if (_sharedObjBlockPtr)
					{
						_sharedObjBlockPtr->WeakRelease();
					}
					_sharedObjBlockPtr = other._sharedObjBlockPtr;
					other._sharedObjBlockPtr = nullptr;
				}

				return *this;
			}

			const Weak& operator=(const Weak& other)
			{
				if (_sharedObjBlockPtr != other._sharedObjBlockPtr) //correct!! no need to assign if they point to same object
				{
					if (_sharedObjBlockPtr)
					{
						_sharedObjBlockPtr->WeakRelease();
					}
					_sharedObjBlockPtr = other._sharedObjBlockPtr;
					if (_sharedObjBlockPtr)
					{
						_sharedObjBlockPtr->WeakAddRef();
					}
				}

				return *this;
			}


			const Weak operator=(RefPtr& shared)
			{
				if (_sharedObjBlockPtr)
				{
					_sharedObjBlockPtr->WeakRelease();
				}
				_sharedObjBlockPtr = shared._objBlockPtr;
				if (_sharedObjBlockPtr)
				{
					_sharedObjBlockPtr->WeakAddRef();
				}
				return *this;
			}

			RefPtr lock()
			{
				if (_sharedObjBlockPtr && 0 != _sharedObjBlockPtr->RefCount())
				{
					_sharedObjBlockPtr->AddRef();
					return RefPtr{ _sharedObjBlockPtr->Ptr() };
				}

				return RefPtr{};
			}
		};
	};
}