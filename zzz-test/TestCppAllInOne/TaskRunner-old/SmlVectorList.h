#pragma once


namespace SmartLib
{
	template<typename T>
	class VectorList
	{
	private:
		constexpr static const long _BUSY_ID{ -1 };
		constexpr static const long _FREE_ID{ -2 };

	public:
		struct Node
		{
			T Data{};
			long Prev{ _FREE_ID };
			long Next{ _FREE_ID };
		};

	private:
		Node* const _first{ nullptr };
		long _size{ 0 };
		long _capacity{ 0 };

	private:
		Node& NodeRef(long id)
		{
			return _first[id];
		}
		
		const Node& NodeRef(long id) const
		{
			return _first[id];
		}

		void Connect(long id1, long id2)
		{
			NodeRef(id1).Next = id2;
			NodeRef(id2).Prev = id1;
		}

		void Dettach(long id)
		{
			Connect(NodeRef(id).Prev, NodeRef(id).Next);
			Connect(id, id);
		}

		void InsertAfter(long pos, long id)
		{
			Connect(id, NodeRef(pos).Next);
			Connect(pos, id);
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
			Connect(_BUSY_ID, _BUSY_ID);
			Connect(_FREE_ID, _FREE_ID);
			for (long ii = 0; ii < count - 2; ++ii)
			{
				//Connect(ii, ii);
				InsertAfter(NodeRef(_FREE_ID).Prev, ii);
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
		template<typename D>
		long push_back(D&& data)
		{
			long freeId = NodeRef(_FREE_ID).Prev;
			if (freeId >= 0)
			{
				Dettach(freeId);
				InsertAfter(NodeRef(_BUSY_ID).Prev, freeId);
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
				Dettach(busyId);
				InsertAfter(NodeRef(_FREE_ID).Prev, busyId);
				--_size;
			}
			return busyId;
		}

		T pop_back()
		{
			long busyId = PopBackNode();
			if (busyId >= 0)
			{
				return static_cast<T&&>(NodeRef(busyId).Data);
			}

			return T{};
		}

		//////////////////////////////////////////////////////////////////////////
		template<typename D>
		long push_front(D&& data)
		{
			long freeId = NodeRef(_FREE_ID).Next;
			if (freeId >= 0)
			{
				Dettach(freeId);
				InsertAfter(_BUSY_ID, freeId);
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
				Dettach(busyId);
				InsertAfter(_FREE_ID, busyId);
				--_size;
			}

			return busyId;
		}

		T pop_front()
		{
			long busyId = PopFrontNode();
			if (busyId >= 0)
			{
				return static_cast<T&&>(NodeRef(busyId).Data);
			}

			return T{};
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
			return NodeRef(_BUSY_ID).Next;
		}


		long Last() const
		{
			return NodeRef(_BUSY_ID).Prev;
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
	public:
		class Enumerator
		{
		private:
			const VectorList& _veclst;
			long _curId{ _BUSY_ID };
		public:
			Enumerator(const VectorList& veclst) :
				_veclst{ veclst },
				_curId{ veclst.NodeRef(_BUSY_ID).Next }
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
				const Node& result = _veclst.NodeRef(_curId);
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
				_curId{ veclst.NodeRef(_BUSY_ID).Prev }
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
				const Node& result = _veclst.NodeRef(_curId);
				_curId = result.Prev;
				return result;
			}
		};

	};
}