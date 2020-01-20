#pragma once

//there should be no c++ std includes here
#include <cassert>
#include <memory.h>

namespace SmartLib
{
	template<typename T, bool NEED_CONSTRUCT_ELEMENT = true, long INITIAL_CAPACITY = 8>
	class Vector
	{
	private:
		struct PlaceHolder
		{
			char mem[sizeof(T)];

			PlaceHolder() = default;
			~PlaceHolder() //will not be called, play trick on compiler 
			{
				memset(mem, 's', sizeof(mem));
			}
		};
	private:
		T _array[INITIAL_CAPACITY];

		long _size{ 0 };
		long _capacity{ INITIAL_CAPACITY };
		T* _buffer{ _array };

	public:

		static void CopyBufferMove(T* newBuffer, T* oldBuffer, long size)
		{
			if constexpr (NEED_CONSTRUCT_ELEMENT)
			{
				for (long ii = 0; ii < size; ++ii)
				{
					newBuffer[ii] = static_cast<T&&>(oldBuffer[ii]);
				}
			}
			else
			{
				memcpy(newBuffer, oldBuffer, size * sizeof(T));
			}
		}

		static void CopyBufferCopy(T* newBuffer, T* oldBuffer, long size)
		{
			if constexpr (NEED_CONSTRUCT_ELEMENT)
			{
				for (long ii = 0; ii < size; ++ii)
				{
					newBuffer[ii] = oldBuffer[ii];
				}
			}
			else
			{
				memcpy(newBuffer, oldBuffer, size * sizeof(T));
			}
		}


		//////////////////////////////////////////////////////////////////////////
		static void MoveBufferMove(T* newBuffer, T* oldBuffer, long size)
		{
			if constexpr (NEED_CONSTRUCT_ELEMENT)
			{
				for (long ii = size - 1; ii >= 0; --ii)
				{
					newBuffer[ii] = static_cast<T&&>(oldBuffer[ii]);
				}
			}
			else
			{
				memmove(newBuffer, oldBuffer, size * sizeof(T));
			}
		}

		static void MoveBufferCopy(T* newBuffer, T* oldBuffer, long size)
		{
			if constexpr (NEED_CONSTRUCT_ELEMENT)
			{
				for (long ii = size - 1; ii >= 0; --ii)
				{
					newBuffer[ii] = oldBuffer[ii];
				}
			}
			else
			{
				memmove(newBuffer, oldBuffer, size * sizeof(T));
			}
		}




	public:
		//////////////////////////////////////////////////////////////////////////
		Vector()
		{
			static_assert(INITIAL_CAPACITY >= 1);
		}

		Vector(const Vector& right)
		{
			_size = right._size;

			if (_size <= INITIAL_CAPACITY)
			{
				_capacity = INITIAL_CAPACITY;
				_buffer = _array;
			}
			else
			{
				_capacity = right._capacity;
				_buffer = new T[_capacity];
			}

			CopyBufferCopy(_buffer, right._buffer, _size);
		}

		Vector(Vector&& right)
		{
			_size = right._size;
			_capacity = right._capacity;

			if (right._buffer != right._array)
			{
				_buffer = right._buffer;
				right._buffer = right._array;
			}
			else
			{
				_buffer = _array;
				CopyBufferMove(_buffer, right._buffer, _size);
			}

			right._size = 0;
			right._capacity = INITIAL_CAPACITY;
		}

		~Vector()
		{
			if (_buffer != _array)
			{
				delete[] _buffer;
				_buffer = _array;
			}

			_capacity = INITIAL_CAPACITY;
			_size = 0;
		}

		//////////////////////////////////////////////////////////////////////////
		const Vector operator=(const Vector& right)
		{
			resize(right._size);
			CopyBufferCopy(_buffer, right._buffer, _size);
			return *this;
		}

		const Vector operator=(Vector&& right)
		{
			if (_buffer != _array)
			{
				delete[] _buffer;
				_buffer = _array;
			}


			_size = right._size;
			_capacity = right._capacity;

			if (right._buffer != right._array)
			{
				_buffer = right._buffer;
				right._buffer = right._array;
			}
			else
			{
				_buffer = _array;
				CopyBufferMove(_buffer, right._buffer, _size);
			}

			right._size = 0;
			right._capacity = INITIAL_CAPACITY;

			return *this;
		}

		//////////////////////////////////////////////////////////////////////////
		void resize(long newSize)
		{
			if (newSize > _capacity)
			{
				long newCapacity = newSize / INITIAL_CAPACITY * INITIAL_CAPACITY;
				newCapacity += (INITIAL_CAPACITY + INITIAL_CAPACITY);

				long tempCap = _capacity + _capacity / 2;
				if (newCapacity < tempCap)
				{
					newCapacity = tempCap;
				}

#if true
				T* newBuffer = nullptr;
				if constexpr (NEED_CONSTRUCT_ELEMENT)
				{
					newBuffer = (T*)(new PlaceHolder[newCapacity]);
					for (long ii = 0; ii < _size; ++ii)
					{
						new(newBuffer + ii) T{ static_cast<T&&>(_buffer[ii]) };
					}

					for (long ii = _size; ii < newCapacity; ++ii)
					{
						new(newBuffer + ii) T{};
					}
				}
				else
				{
					newBuffer = new T[newCapacity];
					CopyBufferMove(newBuffer, _buffer, _size);
				}
#else
				T* newBuffer = new T[newCapacity];
				CopyBufferMove(newBuffer, _buffer, _size);
#endif

				if (_buffer != _array)
				{
					delete[] _buffer;
				}

				_capacity = newCapacity;
				_buffer = newBuffer;
			}
			else if (newSize <= INITIAL_CAPACITY / 2 && _buffer != _array)
			{
				CopyBufferMove(_array, _buffer, newSize);

				delete[] _buffer;

				_capacity = INITIAL_CAPACITY;
				_buffer = _array;
			}

			_size = newSize;
		}

		//////////////////////////////////////////////////////////////////////////
		template<typename D>
		void push_back(D&& element)
		{
			resize(_size + 1);
			_buffer[_size - 1] = static_cast<D&&>(element);
		}


		T pop_back()
		{
			T data = static_cast<T&&>(_buffer[_size - 1]);
			resize(_size - 1);
			return data;
		}

		//////////////////////////////////////////////////////////////////////////
		void Append(T* first, long count, bool bMove)
		{
			long oldSize = _size;
			resize(oldSize + count);
			if (bMove)
			{
				CopyBufferMove(_buffer + oldSize, first, count);
			}
			else
			{
				CopyBufferCopy(_buffer + oldSize, first, count);
			}
			
		}

		template<typename ITERATOR>
		void Append(ITERATOR first, ITERATOR stop, bool bMove)
		{
			while (first != stop)
			{
				if (bMove)
				{
					push_back(static_cast<T&&>(*first));
				}
				else
				{
					push_back(*first);
				}
				
				++first;
			}
		}


		//void AppendMove(T* first, long count)
		//{
		//	long oldSize = _size;
		//	resize(oldSize + count);
		//	CopyBufferMove(_buffer + oldSize, first, count);
		//}

		//template<typename ITERATOR>
		//void AppendMove(ITERATOR first, ITERATOR stop)
		//{
		//	while (first != stop)
		//	{
		//		push_back(static_cast<T&&>(*first));
		//		++first;
		//	}
		//}

		void Insert(long pos, T* data, long count, bool bMove) //insert before pos
		{
			resize(_size + count);
			MoveBufferMove(_buffer + pos + count, _buffer + pos, _size - pos - count);
			if (bMove)
			{
				CopyBufferMove(_buffer + pos, data, count);
			}
			else
			{
				CopyBufferCopy(_buffer + pos, data, count);
			}
		}


		void Delete(long pos, long count)
		{
			if (count < 0 || pos + count > _size)
			{
				count = _size - pos;
			}
			CopyBufferMove(_buffer + pos, _buffer + pos + count, _size - pos - count);
			resize(_size - count);
		}

		//////////////////////////////////////////////////////////////////////////
		long size() const
		{
			return _size;
		}

		long capacity() const
		{
			return _capacity;
		}

		//////////////////////////////////////////////////////////////////////////
		T* First()
		{
			return _buffer;
		}


		T* Last()
		{
			return _buffer + _size - 1;
		}

		T* Stop()
		{
			return _buffer + _size;
		}

		const T* First() const
		{
			return _buffer;
		}

		const T* Last() const
		{
			return _buffer + _size - 1;
		}

		const T* Stop() const
		{
			return _buffer + _size;
		}


		//////////////////////////////////////////////////////////////////////////
		T& front()
		{
			return _buffer[0];
		}

		T& back()
		{
			return _buffer[_size - 1];
		}

		const T& front() const
		{
			return _buffer[0];
		}

		const T& back() const
		{
			return _buffer[_size - 1];
		}

		//////////////////////////////////////////////////////////////////////////
		T& operator[](long index)
		{
			assert(index >= 0 && index < _size);
			return _buffer[index];
		}

		const T& operator[](long index) const
		{
			assert(index >= 0 && index <= _size);
			return _buffer[index];
		}

		//////////////////////////////////////////////////////////////////////////
		template<typename MAPPER>
		static T* FindEqual(T* first, T* stop, MAPPER&& mapper, long value)
		{
			while (first != stop)
			{
				if (mapper(first) == value)
				{
					break;
				}
				++first;
			}
			return first;
		}

		template<typename MAPPER>
		static T* FindNotEqual(T* first, T* stop, MAPPER&& mapper, long value)
		{
			while (first != stop)
			{
				if (mapper(first) != value)
				{
					break;
				}
				++first;
			}
			return first;
		}


		static void SwapRange(T* first, long count1, long count2)
		{
			long count = count1 < count2 ? count1 : count2;
			T* last = first + count1 + count2 - 1;
			while (count > 0)
			{
				T temp = static_cast<T&&>(*first);
				*first = static_cast<T&&>(*last);
				*last = static_cast<T&&>(temp);
				++first;
				--last;
				--count;
			}
		}

		static void SwapData(T* aa, T* bb)
		{
			T temp = static_cast<T&&>(*aa);
			*aa = static_cast<T&&>(*bb);
			*bb = static_cast<T&&>(temp);
		}

#if true
		//////////////////////////////////////////////////////////////////////////
		template<typename MAPPER>
		static T* Partition(T* first, T* stop, MAPPER&& mapper, long value)
		{
			T* notEqual = FindNotEqual(first, stop, mapper, value);
			T* equal = FindEqual(notEqual, stop, mapper, value);
			T* nextNE = FindNotEqual(equal, stop, mapper, value);

			while (equal != stop)
			{
				long count1 = equal - notEqual;
				long count2 = nextNE - equal;
				SwapRange(notEqual, count1, count2);
				notEqual += count2;
				equal = FindEqual(nextNE, stop, mapper, value);
				nextNE = FindNotEqual(equal, stop, mapper, value);
			}

			if (notEqual - first > 1)
			{
				SwapData(first, notEqual - 1);
			}

			return notEqual;
		}


		template<typename LESS, typename EQUAL>
		static void Sort(T* first, T* stop, LESS&& less, EQUAL&& equal)
		{
			if ((long)(stop - first) <= 1)
			{
				return;
			}

			T firstData = *first;
			auto mapper = [&firstData, &less, &equal](const T* pdata)
			{
				return (less(pdata, &firstData) || equal(pdata, &firstData)) ? 0 : 1;
			};

			T* half = Partition(first, stop, mapper, 0);
			Sort(first, half - 1, less, equal);
			Sort(half, stop, less, equal);
		}


		template<typename LESS, typename EQUAL>
		void Sort(LESS&& less, EQUAL&& equal)
		{
			if (_size <= 1)
			{
				return;
			}

			Sort(_buffer, _buffer + _size, less, equal);
		}
#endif
		//////////////////////////////////////////////////////////////////////////
		template<typename MAPPER>
		static T* PartitionFast(T* first, T* stop, MAPPER&& mapper, long value)
		{
			T* notEqual = FindNotEqual(first, stop, mapper, value);
			T* equal = FindEqual(notEqual, stop, mapper, value);
			T* nextNE = FindNotEqual(equal, stop, mapper, value);

			while (equal != stop)
			{
				long count1 = equal - notEqual;
				long count2 = nextNE - equal;
				SwapRange(notEqual, count1, count2);
				notEqual += count2;
				equal = FindEqual(nextNE, stop, mapper, value);
				nextNE = FindNotEqual(equal, stop, mapper, value);
			}

			return notEqual;
		}


		template<typename LESS, typename EQUAL>
		static void SortFast(T* first, T* stop, LESS&& less, EQUAL&& equal)
		{
			if ((long)(stop - first) <= 1)
			{
				return;
			}

			T firstData = *first;
			auto mapperLess = [&firstData, &less](const T* pdata)
			{
				return less(pdata, &firstData) ? 0 : 1;
			};
			T* half1 = PartitionFast(first, stop, mapperLess, 0);

			auto mapperEqual = [&firstData, &equal](const T* pdata)
			{
				return equal(pdata, &firstData) ? 0 : 1;
			};
			T* half2 = PartitionFast(half1, stop, mapperEqual, 0);

			SortFast(first, half1, less, equal);
			SortFast(half2, stop, less, equal);
		}

		template<typename LESS, typename EQUAL>
		void SortFast(LESS&& less, EQUAL&& equal)
		{
			if (_size <= 1)
			{
				return;
			}

			SortFast(_buffer, _buffer + _size, less, equal);
		}


		template<typename LESS, typename EQUAL>
		bool VerifyOrder(LESS&& less, EQUAL&& equal)
		{
			if (_size <= 1)
			{
				return true;
			}

			bool result = true;
			T* cur = _buffer;
			for (long ii = 0; ii <= _size - 2; ++ii)
			{
				if (less(cur, cur + 1) || equal(cur, cur + 1))
				{
					//do nothing
				}
				else
				{
					result = false;
					break;
				}
			}

			return result;
		}

	public:
		class iterator
		{
		private:
			Vector* _vec{ nullptr };
			long _pos{ 0 };
			long _step{ 0 };
		public:
			iterator(Vector& vec, long pos) :
				_vec{ &vec },
				_pos{ pos }
			{
				//if (_vec->_size == 0)
				//{
				//	_pos = -1;
				//}
			}

			iterator& operator++()
			{
				++_step;
				++_pos;

				//if (_pos > _vec->_size)
				//{
				//	_pos = _vec->_size;
				//}


				return *this;
			}

			iterator& operator--()
			{
				--_step;
				--_pos;

				//if (_pos < 0)
				//{
				//	_pos = _vec->_size;
				//}

				
				return *this;
			}

			T& operator*()
			{
				return (*_vec)[_pos];
			}

			long Step()
			{
				return _step;

			}

			long Get()
			{
				return _pos;
			}

			bool operator==(const iterator& iter2)
			{
				return _pos  == iter2._pos;
			}

			bool operator!=(const iterator& iter2)
			{
				return _pos != iter2._pos;
			}
		};

	public:
		iterator begin()
		{
			return iterator{ *this, 0 };
		}

		iterator end()
		{
			return iterator{ *this, _size };
		}

		iterator rbegin()
		{
			return iterator{ *this, _size -1 };
		}


		iterator rend()
		{
			return iterator{ *this, -1 };
		}

		//////////////////////////////////////////////////////////////////////////
	public:
		class Enumerator
		{
		private:
			const Vector& _vec;
			long _index{ 0 };
		public:
			Enumerator(const Vector& vec) :
				_vec{ vec }
			{}

			void Restart()
			{
				_index = 0;
			}

			bool HasNext() const
			{
				return _index < _vec._size;
			}

			const T& Next()
			{
				const T& result = _vec[_index];
				++_index;
				return result;
			}
		};


		class ReverseEnumerator
		{
		private:
			const Vector& _vec;
			long _count{ 0 };
		public:
			ReverseEnumerator(const Vector& vec) :
				_vec{ vec },
				_count{ _vec._size }
			{}

			void Restart()
			{
				_count = _vec._size;
			}

			bool HasNext() const
			{
				return _count > 0;
			}

			const T& Next()
			{
				const T& result = _vec[_count - 1];
				--_count;
				return result;
			}
		};
	};
}