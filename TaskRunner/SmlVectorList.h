#pragma once

#include <cassert>

//DEBUG START
//#include <iostream>
//using namespace ::std;
//DEBUG END

namespace SmartLib
{
	template<typename T>
	class VectorList
	{
	private:
		constexpr static const long _BUSY_ID{ -1 };
		constexpr static const long _FREE_ID{ -2 };
	public:
		constexpr static const long _INVALID_ID{ -3 };

	public:
		struct Node
		{
			T Data{};
			long Prev{ _FREE_ID };
			long Next{ _FREE_ID };


			Node() = default;

			Node(Node&& nn) :
				Data{ static_cast<T&&>(nn.Data) },
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
		};

	private:
		Node* const _first{ nullptr };
		long _size{ 0 };
		long _capacity{ 0 };

	private:


		void ConnectNode(long id1, long id2)
		{
			NodeRef(id1).Next = id2;
			NodeRef(id2).Prev = id1;

			//if (id1 != _INVALID_ID)
			//{
			//	NodeRef(id1).Next = id2;
			//}
			//
			//if (id2 != _INVALID_ID)
			//{
			//	NodeRef(id2).Prev = id1;
			//}
			//
		}

		void DettachNode(long id)
		{
			ConnectNode(NodeRef(id).Prev, NodeRef(id).Next);
			ConnectNode(id, id);
		}

		void InsertNodeAfter(long pos, long id)
		{
			ConnectNode(id, NodeRef(pos).Next);
			ConnectNode(pos, id);
		}

		long DisconnectAndCircle(long first, long stop)
		{
			long last = NodeRef(stop).Prev;
			//ConnectNode(last, _NULL_ID);
			ConnectNode(NodeRef(first).Prev, stop); //disconnect from list
			//ConnectNode(_NULL_ID, first);
			ConnectNode(last, first); //make circular linked nodes
			return last;
		}

	private:
		VectorList() = delete;
		VectorList(const VectorList&) = delete;
		VectorList(VectorList&&) = delete;
		const VectorList& operator=(const VectorList&) = delete;
		const VectorList& operator=(VectorList&&) = delete;

	public:
		VectorList(Node* memStart, long count) : //init constructor
			_first{ memStart + 2 },
			_size(0),
			_capacity{ count  -2 }
		{
			ConnectNode(_BUSY_ID, _BUSY_ID);
			ConnectNode(_FREE_ID, _FREE_ID);
			for (long ii = 0; ii < count - 2; ++ii)
			{
				//Connect(ii, ii);
				InsertNodeAfter(NodeRef(_FREE_ID).Prev, ii);
			}
		}


		VectorList(Node* memStart, long size, long count) : //attach constructor
			_first{ memStart + 2 },
			_size{ size },
			_capacity{ count - 2}
		{
			if (_size <= 0)
			{
				_size = 0;
				long id = NodeRef(_BUSY_ID).Next;
				while (id != _BUSY_ID)
				{
					++_size;
					id = NodeRef(id).Next;
				}
			}


			//////////////////////////////////////////////////////////////////////////
			if (_capacity <= 0)
			{
				long slotCount = 0;
				long id = NodeRef(_FREE_ID).Next;
				while (id != _FREE_ID)
				{
					++slotCount;
					id = NodeRef(id).Next;
				}

				_capacity = slotCount + _size;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		Node& NodeRef(long id)
		{
			return _first[id];
		}

		const Node& ConstNodeRef(long id) const
		{
			return _first[id];
		}


		//////////////////////////////////////////////////////////////////////////
		template<typename MAPPER>
		long FindEqual(long first, long stop, MAPPER&& mapper, long mappedValue, long& steps)
		{
			steps = 0;
			while (first != stop)
			{
				if (mapper(NodeRef(first)) == mappedValue)
				{
					break;
				}
				++steps;
				first = NodeRef(first).Next;
			}

			return first;
		}


		template<typename MAPPER>
		long FindNotEqual(long first, long stop, MAPPER&& mapper, long mappedValue, long& steps)
		{
			steps = 0;
			while (first != stop)
			{
				if (mapper(NodeRef(first)) != mappedValue)
				{
					break;
				}
				++steps;
				first = NodeRef(first).Next;
			}

			return first;
		}

		template<typename MAPPER>
		void Partition(long first, long stop, MAPPER&& mapper, long* arr, long count)
		{
			while (first != stop)
			{
				long mappedValue = mapper(NodeRef(first));
				long steps = 0;
				long next = FindNotEqual(first, stop, mapper, mappedValue, steps);
				if (steps > 0)
				{
					if (mappedValue < count)
					{
						_size -= steps;
						long last = DisconnectAndCircle(first, next);
						if (arr[mappedValue] < 0)
						{
							arr[mappedValue] = first;
						}
						else
						{
							long prevFirst = arr[mappedValue];
							long prevLast = NodeRef(prevFirst).Prev;
							ConnectNode(prevLast, first);
							ConnectNode(last, prevFirst);
						}
						
					}
				}
				first = next;
			}
		}

		template<typename LESS, typename EQUAL>
		long SortCircularNodes(long first, LESS&& less, EQUAL&& equal)
		{
			if (NodeRef(first).Next == first)
			{
				return first;
			}

			
			auto mapper = [this, first, &less, &equal](Node& nn) -> long
			{
				return less(nn, NodeRef(first)) ? 0
					: equal(nn, NodeRef(first)) ? 1
					: 2;
			};

			constexpr const long ARRAY_SIZE = 3;
			long arr[ARRAY_SIZE] = { _INVALID_ID, _INVALID_ID, _INVALID_ID};

			long newFirst = NodeRef(first).Next;
			Partition(newFirst, first, mapper, arr, ARRAY_SIZE);

			if (arr[1] < 0)
			{
				arr[1] = first;
			}
			else
			{
				InsertNodeAfter(NodeRef(arr[1]).Prev, first);
			}
			
			//////////////////////////////////////////////////////////////////////////DEBUG START
			//auto printParts = [](VectorList<long>& vl, long start)
			//{
			//	long first = start;
			//	while (start >= 0)
			//	{
			//		cout << vl.NodeRef(start).Data << ' ';
			//		start = vl.NodeRef(start).Next;
			//		if (start == first)
			//		{
			//			break;
			//		}
			//	}
			//	cout << endl;
			//};

			//for (long ii = 0; ii < ARRAY_SIZE; ++ii)
			//{
			//	cout << ii << " parted:\t";
			//	printParts(*this, arr[ii]);
			//}
			//cout << endl << endl;
			//////////////////////////////////////////////////////////////////////////DEBNUG END

			for (long ii = 0; ii < ARRAY_SIZE; ++ii)
			{
				if (ii != 1) //no need to sort "SAME" elements
				{
					if (arr[ii] >= 0)
					{
						long min = SortCircularNodes(arr[ii], less, equal);
						arr[ii] = min;
					}
				}

				//////////////////////////////////////////////////////////////////////////DEBNUG START
				//cout << ii << " sorted:\t";
				//printParts(*this, arr[ii]);
				//////////////////////////////////////////////////////////////////////////DEBNUG END
			}
			

			long minId = _INVALID_ID;
			for (int ii = 0; ii < ARRAY_SIZE; ++ii)
			{
				if (arr[ii] >= 0)
				{
					if (minId < 0)
					{
						minId = arr[ii];
					}
					else
					{
						long first1 = minId;
						long last1 = NodeRef(first1).Prev;

						long first2 = arr[ii];
						long last2 = NodeRef(first2).Prev;

						ConnectNode(last1, first2);
						ConnectNode(last2, first1);
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////DEBNUG START
			//cout << "final sorted:\t";
			//printParts(*this, minId);
			//////////////////////////////////////////////////////////////////////////DEBNUG END

			return minId;
		}

		template<typename LESS, typename EQUAL>
		void Sort(LESS&& less, EQUAL&& equal)
		{
			if (_size <= 1)
			{
				return;
			}

			long savedSize = _size; //to restore size because partition() will dec _size

			long first = NodeRef(_BUSY_ID).Next;
			DisconnectAndCircle(first, _BUSY_ID);
			long minId = SortCircularNodes(first, less, equal);

			long sortedFirst = minId; //make code "more" clear and readable
			long sortedLast = NodeRef(minId).Prev;
			ConnectNode(sortedLast, _BUSY_ID);
			ConnectNode(_BUSY_ID, sortedFirst);

			_size = savedSize; //restore size
		}

		template<typename LESS, typename EQUAL>
		bool VerifyOrder(LESS&& less, EQUAL&& equal)
		{
			if (0 == _size || 1 == _size)
			{
				return true;
			}

			long cur = NodeRef(_BUSY_ID).Next;
			long next = NodeRef(cur).Next;
			bool result = true;
			while (next != _BUSY_ID)
			{
				if (less(NodeRef(cur), NodeRef(next)) || equal(NodeRef(cur), NodeRef(next)))
				{
					cur = next;
					next = NodeRef(next).Next;
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
		void AppendData(T* first, long count, bool bMove)
		{
			long nodeId = _FREE_ID;
			while (count > 0)
			{
				if (bMove)
				{
					nodeId = push_back(static_cast<T&&>(*first));
				}
				else
				{
					nodeId = push_back(*first);
				}

				if (nodeId < 0)
				{
					break;
				}

				++first;
				--count;
			}
		}

		template<typename ITERATOR>
		void AppendData(ITERATOR first, ITERATOR stop, bool bMove)
		{
			long nodeId = _FREE_ID;
			while (first != stop)
			{
				if (bMove)
				{
					nodeId = push_back(static_cast<T&&>(*first));
				}
				else
				{
					nodeId = push_back(*first);
				}

				if (nodeId < 0)
				{
					break;
				}

				++first;
			}
		}


		template<typename D>
		long InsertAfter(long nodeId, D&& data)
		{
			long freeId = NodeRef(_FREE_ID).Next;
			if (freeId >= 0)
			{
				DettachNode(freeId);
				InsertNodeAfter(nodeId, freeId);
				NodeRef(freeId).Data = static_cast<D&&>(data);
				++_size;
			}

			return freeId;
		}


		long DeleteNode(long nodeId)
		{
			assert(nodeId >= 0);
			long nextId = NodeRef(nodeId).Next;
			DettachNode(nodeId);
			InsertNodeAfter(_FREE_ID, nodeId);
			--_size;
			T data = static_cast<T&&>(NodeRef(nodeId).Data); //in ordrt to call destructor ~T()
			return nextId;
		}

		template<typename TEST>
		long DeleteNodes(TEST&& test)
		{
			long result = _BUSY_ID;
			long first = NodeRef(_BUSY_ID).Next;
			while (first != _BUSY_ID)
			{
				if (test(NodeRef(first)))
				{
					first = DeleteNode(first);
					result = first;
				}
				else
				{
					first = NodeRef(first).Next;
				}
			}
			return result;
		}

		//////////////////////////////////////////////////////////////////////////
		template<typename D>
		long push_back(D&& data)
		{
			long freeId = NodeRef(_FREE_ID).Prev;
			if (freeId >= 0)
			{
				DettachNode(freeId);
				InsertNodeAfter(NodeRef(_BUSY_ID).Prev, freeId);
				NodeRef(freeId).Data = static_cast<D&&>(data);
				++_size;
			}

			return freeId;
		}


		long PopBackNode()
		{
			long busyId = NodeRef(_BUSY_ID).Prev;
			if (busyId >= 0)
			{
				DettachNode(busyId);
				InsertNodeAfter(NodeRef(_FREE_ID).Prev, busyId);
				--_size;
			}
			return busyId;
		}

		T pop_back()
		{
			long busyId = PopBackNode();
			assert(busyId >= 0);
			return static_cast<T&&>(NodeRef(busyId).Data);
		}

		//////////////////////////////////////////////////////////////////////////
		template<typename D>
		long push_front(D&& data)
		{
			long freeId = NodeRef(_FREE_ID).Next;
			if (freeId >= 0)
			{
				DettachNode(freeId);
				InsertNodeAfter(_BUSY_ID, freeId);
				NodeRef(freeId).Data = static_cast<D&&>(data);
				++_size;
			}

			return freeId;
		}


		long PopFrontNode()
		{
			long busyId = NodeRef(_BUSY_ID).Next;
			if (busyId >= 0)
			{
				DettachNode(busyId);
				InsertNodeAfter(_FREE_ID, busyId);
				--_size;
			}

			return busyId;
		}

		T pop_front()
		{
			long busyId = PopFrontNode();
			assert(busyId >= 0);
			return static_cast<T&&>(NodeRef(busyId).Data);
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
		long First() const
		{
			return ConstNodeRef(_BUSY_ID).Next;
		}


		long Last() const
		{
			return ConstNodeRef(_BUSY_ID).Prev;
		}

		long Stop() const
		{
			return _BUSY_ID;
		}

		//////////////////////////////////////////////////////////////////////////
		T& front()
		{
			long busyId = NodeRef(_BUSY_ID).Next;
			assert(busyId >= 0);
			return NodeRef(busyId).Data;
		}


		T& back()
		{
			long busyId = NodeRef(_BUSY_ID).Prev;
			assert(busyId >= 0);
			return NodeRef(busyId).Data;
		}


		const T& front() const
		{
			long busyId = NodeRef(_BUSY_ID).Next;
			assert(busyId >= 0);
			return NodeRef(busyId).Data;
		}


		const T& back() const
		{
			long busyId = NodeRef(_BUSY_ID).Prev;
			assert(busyId >= 0);
			return NodeRef(busyId).Data;
		}

		//////////////////////////////////////////////////////////////////////////
		class iterator
		{
		private:
			VectorList* _vecList{ nullptr };
			long _nodeId{ nullptr };
			long _step{ 0 };
		public:
			iterator(VectorList& vl, long nodeId) :
				_vecList{&vl},
				_nodeId{ nodeId }
			{
			}

			iterator& operator++()
			{
				++_step;
				_nodeId = _vecList->NodeRef(_nodeId).Next;
				return *this;
			}

			iterator& operator--()
			{
				--_step;
				_nodeId = _vecList->NodeRef(_nodeId).Prev;
				return *this;
			}

			T& operator*()
			{
				return _vecList->NodeRef(_nodeId).Data;
			}

			long Step()
			{
				return _step;

			}

			long Get()
			{
				return _nodeId;
			}

			bool operator==(const iterator& iter2)
			{
				return _nodeId == iter2._nodeId;
			}

			bool operator!=(const iterator& iter2)
			{
				return _nodeId != iter2._nodeId;
			}
		};

		iterator begin()
		{
			return iterator{ *this, NodeRef(_BUSY_ID).Next };
		}

		iterator end()
		{
			return iterator{ *this,  _BUSY_ID};
		}

		iterator rbegin()
		{
			return iterator{ *this, NodeRef(_BUSY_ID).Prev };
		}

		iterator rend()
		{
			return iterator{ *this,  _BUSY_ID };
		}

		//////////////////////////////////////////////////////////////////////////
	public:
		class Enumerator
		{
		private:
			const VectorList& _veclst;
			long _curId{ _BUSY_ID };
		public:
			Enumerator(const VectorList& veclst) :
				_veclst{ veclst },
				_curId{ veclst.ConstNodeRef(_BUSY_ID).Next }
			{}

			void Restart()
			{
				_curId = _veclst.NodeRef(_BUSY_ID).Next;
			}

			bool HasNext() const
			{
				return _curId != _BUSY_ID;
			}

			const Node& Next()
			{
				const Node& result = _veclst.ConstNodeRef(_curId);
				_curId = result.Next;
				return result;
			}
		};


		class ReverseEnumerator
		{
		private:
			const VectorList& _veclst;
			long _curId{ _BUSY_ID };
		public:
			ReverseEnumerator(const VectorList& veclst) :
				_veclst{ veclst },
				_curId{ veclst.ConstNodeRef(_BUSY_ID).Prev }
			{}

			void Restart()
			{
				_curId = _veclst.NodeRef(_BUSY_ID).Prev;
			}

			bool HasNext() const
			{
				return _curId != _BUSY_ID;
			}

			const Node& Next()
			{
				const Node& result = _veclst.ConstNodeRef(_curId);
				_curId = result.Prev;
				return result;
			}
		};

	};
}