#pragma once

#include <windows.h>
#include <type_traits>
#include <cassert>


namespace SmartLib
{

	static const int SmartLib_counted_ptr_counted_obj_pattern =  'rtpc' ;


	template<typename TypeObject>
	class counted_ptr
	{
	private:
		template<typename _TypeObject>
		class counted_obj
		{
		public:
			_TypeObject _obj{};
			volatile long _ref{ 0 };
			volatile long _pattern{ 0 };

		public:
			int add_ref()
			{
				return ::InterlockedIncrement(&_ref);
			}

			int dec_ref()
			{
				int ref = ::InterlockedDecrement(&_ref);
				if (0 == ref)
				{
					delete this;
				}
				return ref;
			}

			template<typename ... TypeArgs>
			counted_obj(TypeArgs&& ... args) :
				_obj{ std::forward<TypeArgs>(args)... },
				_ref{ 1 },
				_pattern{ SmartLib_counted_ptr_counted_obj_pattern }//,
			{
			}

			~counted_obj()
			{
				_pattern = 0;
				assert(_ref == 0);
			}
		};

	private:
		counted_obj<TypeObject>* _ptr{ nullptr };

	private:
		explicit counted_ptr(counted_obj<TypeObject>* ptr) :
			_ptr{ptr}
		{
		}

	public:
		template<typename ... _TypeArgs>
		static counted_ptr<TypeObject> make(_TypeArgs&& ... args)
		{
			return counted_ptr<TypeObject>
			{
				new counted_obj<TypeObject>
				{
					std::forward<_TypeArgs>(args)...
				}
			};
		}

		counted_ptr()
		{
			return;
		}

		counted_ptr(const counted_ptr& other) :
			_ptr(other._ptr)
		{
			
			if (_ptr)
			{
				_ptr->add_ref();
			}
		}

		const counted_ptr& operator=(const counted_ptr& other)
		{
			if (_ptr)
			{
				_ptr->dec_ref();
			}

			_ptr = other._ptr;
			if (_ptr)
			{
				_ptr->add_ref();
			}

			return *this;
		}

		
		TypeObject* dettach()
		{
			auto ptr = _ptr;
			_ptr = nullptr;
			return ptr? &ptr->_obj : nullptr;
		}


		void attach(TypeObject* dettached)
		{
			if (_ptr)
			{
				_ptr->dec_ref();
				_ptr = nullptr;
			}

			if (dettached)
			{
				counted_obj<TypeObject>* newptr = CONTAINING_RECORD(dettached, counted_obj<TypeObject>, _obj);
				assert(newptr->_pattern == SmartLib_counted_ptr_counted_obj_pattern);
				assert(newptr->_ref > 0);
				_ptr = newptr;
			}
		}

		operator TypeObject*()
		{
			return _ptr ? &_ptr->_obj : nullptr;
		}

		TypeObject* operator->()
		{
			return _ptr ? &_ptr->_obj : nullptr;
		}

		~counted_ptr()
		{
			if (_ptr)
			{
				_ptr->dec_ref();
				_ptr = nullptr;
			}
		}
	};

	
}