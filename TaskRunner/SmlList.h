#pragma once

//there should be no c++ std includes here

#include "SmlVector.h"

namespace SmartLib
{
	template<typename T, long CACHE_SIZE = 128>
	class List
	{
	public:
		class Node
		{
		public:
			T Data;
			Node* Prev{ nullptr };
			Node* Next{ nullptr };

			Node() :
				Prev{ nullptr },
				Next{ nullptr }
			{
			}

			Node(Node&& nn) :
				Data{static_cast<T&&>(nn.Data)},
				Prev{ nn.Prev },
				Next{ nn.Next }
			{
			}

			Node(const Node& nn) :
				Data{ nn.Data },
				Prev{ nn.Prev },
				Next{ nn.Next }
			{
			}

			template<typename D>
			Node(D&& data, Node* prev, Node* next) :
				Data{ static_cast<D&&>(data) },
				Prev{ prev },
				Next{ next }
			{
			}

			static void Connect(Node* p1, Node* p2)
			{
				if (p1)
				{
					p1->Next = p2;
				}

				if (p2)
				{
					p2->Prev = p1;
				}
			}

			static Node* DisconnectRange(Node* first, Node* stop)
			{
				Node* last = stop->Prev;
				Connect(last, nullptr);
				Connect(first->Prev, stop);
				Connect(nullptr, first);
				return last;
			}
		};


		

	private:
		template<long MAX_COUNT>
		class CacheAllocator
		{
		private:
			Vector<Node*, false, 16> _cache;
		public:
			~CacheAllocator()
			{
				Node** first = _cache.First();
				Node** stop = _cache.Stop();

				while (first != stop)
				{
					delete (*first);
					++first;
				}
				_cache.resize(0);
			}

			template<typename D>
			Node* New(D&& data)
			{
				Node* nn = nullptr;
				if (_cache.size())
				{
					nn = _cache.back();
					nn->Data = static_cast<D&&>(data);
					_cache.pop_back();
				}
				else
				{
					nn = new Node{ static_cast<D&&>(data), nullptr, nullptr };
				}
				return nn;
			}



			void Delete(Node* nn)
			{
				if (_cache.size() >= MAX_COUNT)
				{
					delete nn;
				}
				else
				{
					if (nullptr != nn)
					{
						nn->Prev = nullptr;
						nn->Next = nullptr;
						_cache.push_back(nn);
					}
				}
			}
		};


	private:
		long _size{ 0 };
		Node _start;
		CacheAllocator<CACHE_SIZE> _cacheAlloc;

	private:
		//////////////////////////////////////////////////////////////////////////
		void Assign(const List& right, bool construct)
		{
			if (!construct)
			{
				while (Node* nn = PopBackNode())
				{
					_cacheAlloc.Delete(nn);
				}
			}

			_size = 0;
			Node::Connect(&_start, &_start);

			Node* first = right._start.Next;
			while (first != &right._start)
			{
				push_back(first->Data);
				first = first->Next;
			}
		}

		void Assign(List&& right, bool construct)
		{
			if (!construct)
			{
				while (Node* nn = PopBackNode())
				{
					_cacheAlloc.Delete(nn);
				}
			}

			_size = 0;
			Node::Connect(&_start, &_start);

			Node* first = right._start.Next;
			if (first != &right._start)
			{
				Node* last = right._start.Prev;
				SpliceLinkedNodes(first, last, right._size);
			}

			right._size = 0;
			Node::Connect(&right._start, &right._start);
		}

	public:

		//////////////////////////////////////////////////////////////////////////
		List()
		{
			Node::Connect(&_start, &_start);
		}

		List(const List& right)
		{
			Assign(right, true);
		}

		List(List&& right)
		{
			Assign(static_cast<List&&>(right), true);
		}

		const List& operator=(const List& right)
		{
			Assign(right, false);
			return *this;
		}

		const List& operator=(List&& right)
		{
			Assign(static_cast<List&&>(right), false);
			return *this;
		}




		//////////////////////////////////////////////////////////////////////////
		~List()
		{
			while (Node* nn = PopBackNode())
			{
				if (nn != &_start)
				{
					_cacheAlloc.Delete(nn);
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		Node* PushBackNode(Node* nn)
		{
			Node::Connect(_start.Prev, nn);
			Node::Connect(nn, &_start);
			++_size;
			return nn;
		}


		template<typename D>
		Node* push_back(D&& data)
		{
			Node* nn = _cacheAlloc.New(static_cast<D&&>(data));
			PushBackNode(nn);
			return nn;
		}

		//////////////////////////////////////////////////////////////////////////
		Node* PopBackNode()
		{
			Node* last = _start.Prev;
			if (last != &_start)
			{
				Node::Connect(last->Prev, &_start);
				last->Prev = nullptr;
				last->Next = nullptr;
				--_size;
				return last;
			}

			return nullptr;
		}

		T pop_back()
		{
			Node* nn = PopBackNode();
			T result{ nn ? static_cast<T&&>(nn->Data) : T() };
			_cacheAlloc.Delete(nn);
			return result;
		}

		//////////////////////////////////////////////////////////////////////////
		Node* PushFrontNode(Node* nn)
		{
			Node::Connect(nn, _start.Next);
			Node::Connect(&_start, nn);
			++_size;
			return nn;
		}


		template<typename D>
		Node* push_front(D&& data)
		{
			Node* nn = _cacheAlloc.New(static_cast<D&&>(data));
			PushFrontNode(nn);
			return nn;
		}

		//////////////////////////////////////////////////////////////////////////
		Node* PopFrontNode()
		{
			Node* first = _start.Next;
			if (first != &_start)
			{
				Node::Connect(&_start, first->Next);
				first->Prev = nullptr;
				first->Next = nullptr;
				--_size;
				return first;
			}

			return nullptr;
		}

		T pop_front()
		{
			Node* nn = PopFrontNode();
			T result{ nn ? static_cast<T&&>(nn->Data) : T() };
			_cacheAlloc.Delete(nn);
			return result;
		}


		//////////////////////////////////////////////////////////////////////////
		Node* DeleteNode(Node* nn)
		{
			Node* next = nn->Next;
			if (nn != &_start)
			{
				Node::Connect(nn->Prev, nn->Next);
				T data = static_cast<T&&>(nn->Data); //in ordrt to call destructor ~T()
				_cacheAlloc.Delete(nn);
				--_size;
			}
			return next;
		}

		template<typename TEST>
		Node* DeleteNodes(TEST&& test)
		{
			Node* result = nullptr;
			Node* first = _start.Next;
			while (first != &_start)
			{
				if (test(first))
				{
					first = DeleteNode(first);
					result = first;
				}
				else
				{
					first = first->Next;
				}
			}
			return result;
		}


		void RecycleDelete(Node* nn)
		{
			_cacheAlloc.Delete(nn);
		}

		template<typename D>
		Node* RecycleNew(D&& data)
		{
			return _cacheAlloc.New(static_cast<D&&>(data));
		}

		template<typename MAPPER>
		Node* FindEqual(Node* first, Node* stop, MAPPER&& mapper, long mappedValue, long& steps)
		{
			steps = 0;
			while (first != stop)
			{
				if (mapper(first) == mappedValue)
				{
					break;
				}
				++steps;
				first = first->Next;
			}

			return first;
		}


		template<typename MAPPER>
		Node* FindNotEqual(Node* first, Node* stop, MAPPER&& mapper, long mappedValue, long& steps)
		{
			steps = 0;
			while (first != stop)
			{
				if (mapper(first) != mappedValue)
				{
					break;
				}
				++steps;
				first = first->Next;
			}

			return first;
		}

		template<typename MAPPER>
		void Partition(Node* first, Node* stop, MAPPER&& mapper, List* arr, long count)
		{
			while (first != stop)
			{
				long mappedValue = mapper(first);
				long steps = 0;
				Node* next = FindNotEqual(first, stop, mapper, mappedValue, steps);
				if (steps > 0)
				{
					if (mappedValue < count)
					{
						_size -= steps;
						Node* last = Node::DisconnectRange(first, next);
						arr[mappedValue].SpliceLinkedNodes(first, last, steps);
					}
				}
				first = next;
			}
		}

		

		template<typename LESS, typename EQUAL>
		void Sort(LESS&& less, EQUAL&& equal)
		{
			if (0 == _size || 1 == _size)
			{
				return;
			}

			Node* first = _start.Next;
			auto mapper = [first, &less, &equal](const Node* nn) -> long
			{
				return less(nn, first) ? 0 
					: equal(nn, first) ?  1
					: 2;
			};

			constexpr const long ARRAY_SIZE = 3;
			List arr[ARRAY_SIZE];
			Partition(_start.Next, &_start, mapper, arr, ARRAY_SIZE);

			for (long ii = 0; ii < ARRAY_SIZE; ++ii)
			{
				if (ii != 1) //no need to sort "SAME" elements
				{
					arr[ii].Sort(less, equal);
				}
				
			}

			for (int ii = 0; ii < ARRAY_SIZE; ++ii)
			{
				SpliceList(arr[ii]);
			}
		}

		template<typename LESS, typename EQUAL>
		bool VerifyOrder(LESS&& less, EQUAL&& equal)
		{
			if (0 == _size || 1 == _size)
			{
				return true;
			}

			Node* cur = _start.Next;
			Node* next = cur->Next;
			bool result = true;
			while (next != &_start)
			{
				if (less(cur, next) || equal(cur, next))
				{
					cur = next;
					next = next->Next;
				}
				else
				{
					result = false;
					break;
				}
			}

			return result;
		}

		//////////////////////////////////////////////////////////////////////////

		void SpliceLinkedNodesAfter(Node* pos, Node* first, Node* last, long count)
		{
			Node::Connect(last, pos->Next);
			Node::Connect(pos, first);
			_size += count;
		}


		void SpliceLinkedNodes(Node* first, Node* last, long count)
		{
			SpliceLinkedNodesAfter(_start.Prev, first, last, count);
			//Node::Connect(_start.Prev, first);
			//Node::Connect(last, &_start);
			//_size += count;
		}


		void SpliceListAfter(Node* pos, List& other)
		{
			if (other._size)
			{
				SpliceLinkedNodesAfter(pos, other._start.Next, other._start.Prev, other._size);
				other._size = 0;
				Node::Connect(&other._start, &other._start);
			}
		}


		void SpliceList(List& other)
		{
			SpliceListAfter(_start.Prev, other);
			//if (other.size())
			//{
			//	SpliceLinkedNodes(other._start.Next, other._start.Prev, other.size());
			//	other._size = 0;
			//	Node::Connect(&other._start, &other._start);
			//}
		}


		void SpliceLinkedNodesAfter(Node* pos, Node* first, Node* last)
		{
			long count = 1;
			Node* cur = first;
			while (cur != last)
			{
				++count;
				cur = cur->Next;
			}
			SpliceLinkedNodesAfter(pos, first, last, count);
		}


		void SpliceLinkedNodes(Node* first, Node* last)
		{
			SpliceLinkedNodesAfter(_start.Prev, first, last);
			//long count = 1;
			//Node* cur = first;
			//while (cur != last)
			//{
			//	++count;
			//	cur = cur->Next;
			//}
			//SpliceLinkedNodes(first, last, count);
		}

		//////////////////////////////////////////////////////////////////////////
		void AppendData(T* first, long count, bool bMove)
		{
			while (count > 0)
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
				--count;
			}
		}

		template<typename ITERATOR>
		void AppendData(ITERATOR first, ITERATOR stop, bool bMove)
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


		//void AppendDataMove(T* first, long count)
		//{
		//	while (count > 0)
		//	{
		//		push_back(static_cast<T&&>(*first));
		//		++first;
		//		--count;
		//	}
		//}

		//template<typename ITERATOR>
		//void AppendDataMove(ITERATOR first, ITERATOR stop)
		//{
		//	while (first != stop)
		//	{
		//		push_back(static_cast<T&&>(*first));
		//		++first;
		//	}
		//}

		//////////////////////////////////////////////////////////////////////////
		//void InsertNodeBefore(Node* pos, Node* nn)
		//{
		//	Node::Connect(pos->Prev, nn);
		//	Node::Connect(nn, pos);
		//	++_size;
		//}




		//template<typename D>
		//Node* InsertBefore(Node* pos, D&& data)
		//{
		//	Node* nn = _cacheAlloc.New(static_cast<D&&>(data));
		//	InsertNodeBefore(pos, nn);
		//	return nn;
		//}

		void InsertNodeAfter(Node* pos, Node* nn)
		{
			Node::Connect(nn, pos->Next);
			Node::Connect(pos, nn);
			++_size;
		}



		template<typename D>
		Node* InsertAfter(Node* pos, D&& data)
		{
			Node* nn = _cacheAlloc.New(static_cast<D&&>(data));
			InsertNodeAfter(pos, nn);
			return nn;
		}

		//////////////////////////////////////////////////////////////////////////
		long size() const
		{
			return _size;
		}

		//////////////////////////////////////////////////////////////////////////
		Node* First()
		{
			return _start.Next;
		}

		Node* Last()
		{
			return _start.Prev;
		}

		Node* Stop()
		{
			return &_start;
		}

		const Node* First() const
		{
			return _start.Next;
		}

		const Node* Last() const
		{
			return _start.Prev;
		}

		const Node* Stop() const
		{
			return &_start;
		}


		//////////////////////////////////////////////////////////////////////////
		T& front()
		{
			return _start.Next->Data;
		}

		T& back()
		{
			return _start.Prev->Data;
		}

		const T& front() const
		{
			return _start.Next->Data;
		}

		const T& back() const
		{
			return _start.Prev->Data;
		}


		//////////////////////////////////////////////////////////////////////////

		class iterator
		{
		private:
			Node* _node{ nullptr };
			long _step{ 0 };
		public:
			iterator(Node* node) :
				_node{ node }
			{
			}

			iterator& operator++()
			{
				++_step;
				_node = _node->Next;
				return *this;
			}

			iterator& operator--()
			{
				--_step;
				_node = _node->Prev;
				return *this;
			}

			T& operator*()
			{
				return _node->Data;
			}

			long Step()
			{
				return _step;

			}

			Node* Get()
			{
				return _node;
			}

			bool operator==(const iterator& iter2)
			{
				return _node == iter2._node;
			}

			bool operator!=(const iterator& iter2)
			{
				return _node != iter2._node;
			}
		};

		iterator begin()
		{
			return iterator{_start.Next};
		}

		iterator end()
		{
			return iterator{ &_start };
		}

		iterator rbegin()
		{
			return iterator{_start.Prev};
		}


		iterator rend()
		{
			return iterator{ &_start };
		}

		//////////////////////////////////////////////////////////////////////////
	public:
		class Enumerator
		{
		private:
			const List& _lst;
			const Node* _cur{ nullptr };
		public:
			Enumerator(const List& lst) :
				_lst{ lst },
				_cur{ lst._start.Next }
			{}

			void Restart()
			{
				_cur = _lst._start.Next;
			}

			bool HasNext() const
			{
				return _cur != &_lst._start;
			}

			const Node* Next()
			{
				const Node* result = _cur;
				_cur = _cur->Next;
				return result;
			}
		};


		class ReverseEnumerator
		{
		private:
			const List& _lst;
			const Node* _cur{ nullptr };
		public:
			ReverseEnumerator(const List& lst) :
				_lst{ lst },
				_cur{ lst._start.Prev }
			{}

			void Restart()
			{
				_cur = _lst._start.Prev;
			}

			bool HasNext() const
			{
				return _cur != &_lst._start;
			}

			const Node* Next()
			{
				const Node* result = _cur;
				_cur = _cur->Prev;
				return result;
			}
		};

	};
}