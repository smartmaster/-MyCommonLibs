#pragma once


#include <assert.h>
#include <functional>

#include "log.h"

namespace TestAlg2
{
	template<typename T>
	class LinkedList
	{
	public:
		class Node
		{

		public:
			T _value{};
			Node * _prev{ nullptr };
			Node * _next{ nullptr };

		private:
			friend class LinkedList;
			LinkedList * _owner{ nullptr };

		private:
			Node(const T & value, Node * prev = nullptr, Node * next = nullptr) :
				_value(value),
				_prev(prev),
				_next(next)
			{
			}

		public:

			static void Link(Node* prev, Node * next)
			{
				if (prev)
				{
					prev->_next = next;
				}

				if (next)
				{
					next->_prev = prev;
				}
			}

			static Node * New(const T & value, Node * prev = nullptr, Node * next = nullptr)
			{
				return new Node(value, prev, next);
			}


			static void Delete(Node * node)
			{
				assert(nullptr == node || nullptr == node->_owner);
				delete node;
			}
		};

	private:
		Node * _first{ nullptr };
		Node * _last{ nullptr };
		int _count{ 0 };


	public:
		void AddDataBefore(const T & data, Node * pos)
		{
			AddBefore(Node::New(data), pos);
		}

		void AddBefore(Node * node, Node * pos) //if pos is null, add to back
		{
			assert(node && nullptr == node->_owner);
			assert(nullptr == pos || this == pos->_owner);

			if (0 == _count)
			{
				node->_prev = node->_next = nullptr;
				_first = _last = node;
			}
			else
			{
				if (nullptr == pos) //add to back
				{
					Node::Link(_last, node);
					Node::Link(node, nullptr);
					_last = node;
				}
				else if (_first == pos) //add to front
				{
					Node::Link(nullptr, node);
					Node::Link(node, _first);
					_first = node;
				}
				else //add in middle somewhere
				{
					Node::Link(pos->_prev, node);
					Node::Link(node, pos);
				}
			}


			node->_owner = this;
			++_count;
		}

		void Remove(Node * node, bool deleteNode)
		{
			assert(node && this == node->_owner);

			if (1 == _count)
			{
				_first = _last = nullptr;
			}
			else
			{
				if (_last == node)
				{
					_last = _last->_prev;
					Node::Link(_last, nullptr);
				}
				else if (_first == node)
				{
					_first = _first->_next;
					Node::Link(nullptr, _first);
				}
				else
				{
					Node::Link(node->_prev, node->_next);
				}
			}

			node->_prev = node->_next = nullptr;
			node->_owner = nullptr;
			--_count;

			if (deleteNode)
			{
				Node::Delete(node);
			}
		}

		Node * Find(Node * begin, std::function<bool(const T&)> predict)
		{
			assert(nullptr == begin || this == begin->_owner);
			while (begin)
			{
				if (predict(begin->_value))
				{
					break;
				}
				begin = begin->_next;
			}

			return begin;
		}


		void ForEach(Node * begin, std::function<void(const T &)> func)
		{
			assert(nullptr == begin || this == begin->_owner);
			while (begin)
			{
				func(begin->_value);
				begin = begin->_next;
			}
		}

		void PushBackData(const T & data)
		{
			PushBack(Node::New(data));
		}

		void PushBack(Node * node)
		{
			AddBefore(node, nullptr);
		}

		T PopBackData()
		{
			Node * node = PopBack();
			T rr = node ? node->_value : T{};
			Node::Delete(node);
			return rr;
		}

		Node * PopBack()
		{
			Node * rr = _last;
			if (rr)
			{
				Remove(rr, false);
			}
			return rr;
		}

		//Node * PeekBack()
		//{
		//	return _last;
		//}


		void PushFrontData(const T & data)
		{
			PushFront(Node::New(data));
		}

		void PushFront(Node * node)
		{
			AddBefore(node, _first);
		}


		T PopFrontData()
		{
			Node * node = PopFront();
			T rr = node ? node->_value : T{};
			Node::Delete(node);
			return rr;
		}

		Node * PopFront()
		{
			Node * rr = _first;
			if (rr)
			{
				Remove(rr, false);
			}
			return rr;
		}

		//Node * PeekFront()
		//{
		//	return _first;
		//}

		Node * GetFirst()
		{
			return _first;
		}

		Node * GetLast()
		{
			return _last;
		}

		int GetCount()
		{
			return _count;
		}

		void Cleanup(std::function<void(Node*)> cleaner)
		{
			Node * cur = _first;
			while (cur)
			{
				Node * next = cur->_next;
				cur->_prev = cur->_next = nullptr;
				cur->_owner = nullptr;
				cleaner(cur);
				cur = next;
			}

			_first = _last = nullptr;
			_count = 0;
		}

	}; //class LinkedList


	class LinkedListTest
	{
	public:
		static void Case0()
		{
			LinkedList<int> ll;
			const int mid = 5;
			const int max = 10;
			const int min = 0;
			ll.AddBefore(LinkedList<int>::Node::New(mid), nullptr);
			for (int ii = mid + 1; ii <= max; ++ii)
			{
				ll.PushBack(LinkedList<int>::Node::New(ii));
			}

			for (int ii = mid - 1; ii >= min; --ii)
			{
				ll.PushFront(LinkedList<int>::Node::New(ii));
			}


			LOG_LINE(TEXT("Enumerate:"));
			ll.ForEach(ll.GetFirst(), [](const int & val)
			{
				LOG_LINE(TEXT("%d"), val);
			});


			LOG_LINE(TEXT("pop back all:"));
			while (auto p = ll.PopBack())
			{
				LOG_LINE(TEXT("%d"), p->_value);
				LinkedList<int>::Node::Delete(p);
			}


			for (int ii = 100; ii <= 200; ii += 10)
			{
				ll.PushBack(LinkedList<int>::Node::New(ii));
			}


			LOG_LINE(TEXT("find:"));
			auto node = ll.Find(ll.GetFirst(), [](const int & val)
			{
				return (val == 150);
			});
			if (node)
			{
				LOG_LINE(TEXT("Found node with value [%d]"), node->_value);

				ll.AddBefore(LinkedList<int>::Node::New(145), node);

				LOG_LINE(TEXT("Enumerate after adding:"));
				ll.ForEach(ll.GetFirst(), [](const int & val)
				{
					LOG_LINE(TEXT("%d"), val);
				});


				ll.Remove(node, false);
				LinkedList<int>::Node::Delete(node);
				node = nullptr;
				LOG_LINE(TEXT("Enumerate after removing:"));
				ll.ForEach(ll.GetFirst(), [](const int & val)
				{
					LOG_LINE(TEXT("%d"), val);
				});

			}
			else
			{
				LOG_LINE(TEXT("did not find value"));
			}




			LOG_LINE(TEXT("pop front all:"));
			while (auto p = ll.PopFront())
			{
				LOG_LINE(TEXT("%d"), p->_value);
				LinkedList<int>::Node::Delete(p);
			}


			//////////////////////////////////////////////////////////////////////////
			for (int ii = 1000; ii <= 1005; ii++)
			{
				ll.PushBack(LinkedList<int>::Node::New(ii));
			}

			ll.Cleanup([](LinkedList<int>::Node * p) {
				LOG_LINE(TEXT("to cleanup %d"), p->_value);
				LinkedList<int>::Node::Delete(p);
			});
		}
	}; //class LinkedListTest 
}//namespace TestAlg2