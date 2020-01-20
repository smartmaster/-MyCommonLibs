#pragma once

#include <functional>
#include <assert.h>

#include "log.h"

namespace TestAlg
{

	template<typename T>
	class LinkedList;


	template<typename T>
	class LinkedListNode
	{
		friend class LinkedList<T>;

	private:
		LinkedListNode(const T & value, LinkedListNode* prev = nullptr, LinkedListNode* next = nullptr) :
			_value(value),
			_prev(prev),
			_next(next)

		{
		}

		LinkedList<T> * _owner{ nullptr };

	public:
		T _value{};
		LinkedListNode * _prev{ nullptr };
		LinkedListNode * _next{ nullptr };


		static LinkedListNode* New(const T & value, LinkedListNode* prev = nullptr, LinkedListNode* next = nullptr)
		{
			return new LinkedListNode(value, prev, next);
		}

		static void Delete(LinkedListNode* node)
		{
			delete node;
		}


		static void Link(LinkedListNode * prev, LinkedListNode * next)
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

	};


	template<typename T>
	class LinkedList
	{
	private:
		LinkedListNode<T> * _first{ nullptr };
		LinkedListNode<T> * _last{ nullptr };
		int _count{ 0 };


	public:
		LinkedListNode<T> * Find(LinkedListNode<T>* begin, std::function<bool(const T&)> predict)
		{
			assert(begin && this == begin->_owner);
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

		void AddBefore(LinkedListNode<T>* newNode, LinkedListNode<T> * pos)
		{
			assert(newNode && nullptr == newNode->_owner);
			assert(nullptr == pos || this == pos->_owner);


			if (0 == _count) //empty list
			{
				_first = newNode;
				_last = newNode;
				
			}
			else
			{
				if (pos == nullptr) //add to last
				{
					LinkedListNode<T>::Link(_last, newNode);
					_last = newNode;
				}
				else  if (pos == _first) //add to first
				{
					LinkedListNode<T>::Link(newNode, _first);
					_first = newNode;
				}
				else
				{
					LinkedListNode<T>::Link(pos->_prev, newNode);
					LinkedListNode<T>::Link(newNode, pos);
				}
			}

			newNode->_owner = this;
			++_count;
		}

		void Remove(LinkedListNode<T> * node)
		{
			assert(node && this == node->_owner);

			if (1 == _count)
			{
				_first = nullptr;
				_last = nullptr;
			}
			else
			{
				if (node == _last)
				{
					_last = node->_prev;
					LinkedListNode<T>::Link(_last, nullptr);

				}
				else if (node == _first)
				{
					_first = node->_next;
					LinkedListNode<T>::Link(nullptr, _first);
				}
				else
				{
					LinkedListNode<T>::Link(node->_prev, node->_next);
				}
			}

			//LinkedListNode::Delete(node);
			node->_prev = node->_next = nullptr;
			node->_owner = nullptr;
			--_count;
		}

		void Enumerate(LinkedListNode<T>* begin, std::function<void(const T&)> func)
		{
			assert(begin && this == begin->_owner);

			while (begin)
			{
				func(begin->_value);
				begin = begin->_next;
			}
		}

		void PushBack(LinkedListNode<T>* node)
		{
			AddBefore(node, nullptr);
		}

		LinkedListNode<T> * PopBack()
		{
			LinkedListNode<T> * ret = _last;
			if (_last)
			{
				Remove(_last);
			}
			return ret;
		}

		void PushFront(LinkedListNode<T> * node)
		{
			AddBefore(node, _first);
		}

		LinkedListNode<T> * PopFront()
		{
			LinkedListNode<T> * ret = _first;
			if (_first)
			{
				Remove(_first);
			}
			return ret;
		}



		LinkedListNode<T> * GetFirst()
		{
			return _first;
		}

		LinkedListNode<T> * GetLast()
		{
			return _last;
		}

		int GetCount()
		{
			return _count;
		}
	};


	class LinkedListTest
	{
	public:
		static void Case0()
		{
			LinkedList<int> ll;
			const int mid = 5;
			const int max = 10;
			const int min = 0;
			ll.AddBefore(LinkedListNode<int>::New(mid), nullptr);
			for (int ii = mid + 1; ii <= max; ++ii)
			{
				ll.PushBack(LinkedListNode<int>::New(ii));
			}

			for (int ii = mid - 1; ii >= min; --ii)
			{
				ll.PushFront(LinkedListNode<int>::New(ii));
			}


			LOG_LINE(TEXT("Enumerate:"));
			ll.Enumerate(ll.GetFirst(), [](const int & val)
			{
				LOG_LINE(TEXT("%d"), val);
			});


			LOG_LINE(TEXT("pop back all:"));
			while (auto p = ll.PopBack())
			{
				LOG_LINE(TEXT("%d"), p->_value);
				LinkedListNode<int>::Delete(p);
			}


			for (int ii = 100; ii <= 200; ii += 10)
			{
				ll.PushBack(LinkedListNode<int>::New(ii));
			}

			
			LOG_LINE(TEXT("find:"));
			auto node = ll.Find(ll.GetFirst(), [](const int & val)
			{
				return (val == 150);
			});
			if (node)
			{
				LOG_LINE(TEXT("Found node with value [%d]"), node->_value);

				ll.AddBefore(LinkedListNode<int>::New(145), node);

				LOG_LINE(TEXT("Enumerate after adding:"));
				ll.Enumerate(ll.GetFirst(), [](const int & val)
				{
					LOG_LINE(TEXT("%d"), val);
				});


				ll.Remove(node);
				LinkedListNode<int>::Delete(node);
				node = nullptr;
				LOG_LINE(TEXT("Enumerate after removing:"));
				ll.Enumerate(ll.GetFirst(), [](const int & val)
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
				LinkedListNode<int>::Delete(p);
			}
		}
	};
}