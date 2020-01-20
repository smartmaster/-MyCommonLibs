#pragma once

#include "SmlVector.h"

namespace SmartLib
{
	template<typename T, bool NEED_CONSTRUCT_ELEMENT = true, long INITIAL_CAPACITY = 8>
	class VectorDeque
	{
	private:
		Vector<T, NEED_CONSTRUCT_ELEMENT, INITIAL_CAPACITY> _vec;
		long _first{ 0 };
		long _stop{ 0 };


	private:
		void resize()
		{
			long count = _stop - _first;
			assert(count == _vec.size());
			_vec.resize(count << 1);
			if (_first != 0)
			{
				_vec.CopyBufferMove(&_vec[count], &_vec[0], _first);
				_stop = _first + count;
			}
		}

	public:
		VectorDeque()
		{
			_vec.resize(INITIAL_CAPACITY);
		}

		VectorDeque(VectorDeque&& que) :
			_vec{ static_cast<Vector<T, NEED_CONSTRUCT_ELEMENT, INITIAL_CAPACITY>&&>(que._vec)},
			_first{ que._first },
			_stop{ que._stop }
		{
			que._vec.resize(INITIAL_CAPACITY);
			que._first = 0;
			que._stop = 0;
		}


		VectorDeque(const VectorDeque& que) :
			_vec{ que._vec },
			_first{ que._first },
			_stop{ que._stop }
		{
		}

		const VectorDeque& operator=(VectorDeque&& que)
		{
			_vec = static_cast<Vector<T, NEED_CONSTRUCT_ELEMENT, INITIAL_CAPACITY>&&>(que._vec);
			_first = que._first ;
			_stop = que._stop ;

			que._vec.resize(INITIAL_CAPACITY);
			que._first = 0;
			que._stop = 0;

			return *this;
		}


		const VectorDeque& operator=(const VectorDeque& que)
		{
			_vec = que._vec;
			_first = que._first;
			_stop = que._stop;
			return *this;
		}

		long size() const
		{
			return _stop - _first;
		}


		template<typename DATA>
		void push_back(DATA&& data)
		{
			if (_stop - _first == _vec.size())
			{
				resize();
			}

			_vec[_stop % _vec.size()] = static_cast<DATA&&>(data);
			++_stop;
		}
		
		T pop_back()
		{
			assert(_stop - _first > 0);
			--_stop;
			return static_cast<T&&>(_vec[_stop % _vec.size()]);
		}

		template<typename DATA>
		void push_front(DATA&& data)
		{
			if (_stop - _first == _vec.size())
			{
				resize();
			}

			--_first;
			while (_first < 0)
			{
				_first += _vec.size();
				_stop += _vec.size();
			}

			_vec[_first] = static_cast<DATA&&>(data);
		}

		T pop_front()
		{
			assert(_stop - _first > 0);
			T data = static_cast<T&&>(_vec[_first]);
			++_first;
			while (_first >= _vec.size())
			{
				_first -= _vec.size();
				_stop -= _vec.size();
			}
			return data;
		}


		//////////////////////////////////////////////////////////////////////////
	public:
		class iterator
		{
		private:
			VectorDeque* _deque{ nullptr };
			long _pos{ 0 };
			long _step{ 0 };
		public:
			iterator(VectorDeque& deque, long pos) :
				_deque{ &deque },
				_pos{ pos }
			{
				//while (_pos < 0)
				//{
				//	_pos += _deque->_vec.size();
				//}

				//while (_pos >= _deque->_vec.size())
				//{
				//	_pos -= _deque->_vec.size();
				//}
			}

			iterator& operator++()
			{
				++_step;
				++_pos;

				//while (_pos >= _deque->_vec.size())
				//{
				//	_pos -= _deque->_vec.size();
				//}
				return *this;
			}

			iterator& operator--()
			{
				--_step;
				--_pos;
				//while (_pos < 0)
				//{
				//	_pos += _deque->_vec.size();
				//}
				return *this;
			}

			T& operator*()
			{
				//return (_deque->_vec)[_pos];

				long pos = _pos;
				while (pos < 0)
				{
					pos += _deque->_vec.size();
				}
				while (pos >= _deque->_vec.size())
				{
					pos -= _deque->_vec.size();
				}
				return (_deque->_vec)[pos];
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
				//return (_pos % _deque->_vec.size()) == (iter2._pos % _deque->_vec.size());
				return _pos == iter2._pos;
			}

			bool operator!=(const iterator& iter2)
			{
				//return (_pos % _deque->_vec.size()) != (iter2._pos % _deque->_vec.size());
				return _pos != iter2._pos;
			}
		};

	public:
		iterator begin()
		{
			return iterator{ *this, _first };
		}

		iterator end()
		{
			return iterator{ *this, _stop };
		}

		iterator rbegin()
		{
			return iterator{ *this, _stop - 1 };
		}

		iterator rend()
		{
			return iterator{ *this, _first - 1 };
		}
	};
}