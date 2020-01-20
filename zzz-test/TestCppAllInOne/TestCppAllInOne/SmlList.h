#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <string>
#include <functional>

#include "log.h"

namespace SmartLib
{
	template<typename T>
	class MyList;

	template<typename T>
	class MyListEntry
	{
		friend class MyList<T>;
	private:
		MyListEntry * _prev{ nullptr };
		MyListEntry * _next{ nullptr };
		T _data{default};

	public:
		template<typename TE>
		MyListEntry(TE && data, MyListEntry * prev = nullptr, MyListEntry * next = nullptr) :
			_data(std::forward<TE>(data)),
			_prev(prev),
			_next(next)
		{
		}

		MyListEntry * Prev()
		{
			return _prev;
		}

		MyListEntry * Next()
		{
			return _next;
		}


		T& Data()
		{
			return _data;
		}

		static void Connect(MyListEntry * p1, MyListEntry * p2)
		{
			if (p1)
			{
				p1->_next = p2;
			}

			if (p2)
			{
				p2->_prev = p1;
			}
		}
	};

	template<typename T>
	class MyList
	{
	private:
		MyListEntry<T> * _head{ nullptr };
		MyListEntry<T> * _tail{ nullptr };
		int _count{ 0 };


	public:
		template<typename TE>
		MyListEntry<T> * AddTail(TE && data)
		{
			MyListEntry<T> * pe = new MyListEntry<T>(std::forward<TE>(data));
			if (_tail == nullptr)
			{
				_head = pe;
				_tail = pe;
			}
			else
			{
				MyListEntry<T>::Connect(_tail, pe);
				_tail = pe;
			}

			++_count;
			return pe;
		}

		template<typename TE>
		MyListEntry<T> * AddHead(TE && data)
		{
			MyListEntry<T> * pe = new MyListEntry<T>(std::forward<TE>(data));
			if (_head == nullptr)
			{
				_head = pe;
				_tail = pe;
			}
			else
			{
				MyListEntry<T>::Connect(pe, _head);
				_head = pe;
			}

			++_count;
			return pe;
		}


		template<typename TE>
		MyListEntry<T>* InsertBefore(MyListEntry<T>* pos, TE && data)
		{
			auto newEntry = new MyListEntry<T>(std::forward<TE>(data));

			if (nullptr == _head)
			{
				_head = _tail = newEntry;
			}
			else
			{
				MyListEntry<T>::Connect(pos->_prev, newEntry);
				MyListEntry<T>::Connect(newEntry, pos);

				if (pos == _head)
				{
					_head = newEntry;
				}
			}

			++_count;
			return newEntry;
		}


		template<typename TE>
		MyListEntry<T>* InsertAfter(MyListEntry<T>* pos, TE && data)
		{
			auto newEntry = new MyListEntry<T>(std::forward<TE>(data));

			if (nullptr == _tail)
			{
				_head = _tail = newEntry;
			}
			else
			{
				MyListEntry<T>::Connect(newEntry, pos->_next);
				MyListEntry<T>::Connect(pos, newEntry);

				if (pos == _tail)
				{
					_tail = newEntry;
				}
			}

			++_count;
			return newEntry;
		}


		MyListEntry<T> * Head()
		{
			return _head;
		}

		MyListEntry<T> * Tail()
		{
			return _tail;
		}

		MyListEntry<T> * Remove(MyListEntry<T> * start, std::function<bool(const T&)> pred)
		{
			MyListEntry<T> * rv = nullptr;
			while (start)
			{
				if (pred(start->_data))
				{
					if (start == _head)
					{
						_head = start->_next;
					}
					if (start == _tail)
					{
						_tail = start->_prev;
					}

					MyListEntry<T>::Connect(start->_prev, start->_next);
					rv = start->_next;
					delete start;
					start = nullptr;
					--_count;

					break;
				}
				start = start->_next;
			}

			return rv;
		}


		MyListEntry<T>* Remove(MyListEntry<T>* entry)
		{
			MyListEntry<T>* rv =  nullptr;
			if (entry)
			{
				if (entry == _head)
				{
					_head = entry->_next;
				}

				if (entry == _tail)
				{
					_tail = entry->_prev;
				}

				MyListEntry<T>::Connect(entry->_prev, entry->_next);
				rv = entry->_next;
				delete entry;
				entry = nullptr;
				--_count;
			}

			return rv;
		}


		void Enum(MyListEntry<T> * start, std::function<void(const T&)> pred) const
		{
			while (start)
			{
				pred(start->_data);
				start = start->_next;
			}
		}

		MyListEntry<T> * Find(MyListEntry<T> * start, std::function<bool(const T&)> pred) const
		{
			//if (nullptr == start)
			//{
			//	start = _head;
			//}

			while (start)
			{
				if (pred(start->_data))
				{
					break;
				}
				start = start->_next;
			}

			return start;
		}


		int Count() const
		{
			return _count;
		}

		~MyList()
		{
			MyListEntry<T> * cur = _head;
			while (cur)
			{
				MyListEntry<T> * next = cur->_next;
				delete cur;
				cur = next;
			}

			_head = _tail = nullptr;
			_count = 0;
		}
	};



	class MyListTest
	{
	public:

		static void Case2()
		{
			MyList<int> lst;


			auto print = [](const int & val)
			{
				SML_LOG(TEXT("%d "), val);
			};

			for (int ii = 5; ii >= 0; --ii)
			{
				lst.AddHead(ii);
			}
			lst.Enum(lst.Head(), print);
			SML_LOG_LINE(TEXT(""));

			for (int ii = 6; ii < 10; ++ii)
			{
				lst.AddTail(ii);
			}

			lst.Enum(lst.Head(), print);
			SML_LOG_LINE(TEXT(""));

			int count = lst.Count() / 2;
			while (count)
			{
				lst.Remove(lst.Head());
				--count;
			}

			while (lst.Count())
			{
				lst.Remove(lst.Tail());
			}

		}


		static void Case1()
		{
			MyList<int> lst;


			auto print = [](const int & val)
			{
				SML_LOG(TEXT("%d "), val);
			};

			for (int ii = 5; ii >= 0; --ii)
			{
				lst.InsertBefore(lst.Head(), ii);
			}
			lst.Enum(lst.Head(), print);
			SML_LOG_LINE(TEXT(""));

			for (int ii = 6; ii < 10; ++ ii)
			{
				lst.InsertAfter(lst.Tail(), ii);
			}

			lst.Enum(lst.Head(), print);
			SML_LOG_LINE(TEXT(""));

			int count = lst.Count() / 2;
			while (count)
			{
				lst.Remove(lst.Head());
				--count;
			}

			while (lst.Count())
			{
				lst.Remove(lst.Tail());
			}

		}
		static void Case0()
		{
			auto printE = [](const std::wstring& str)
			{
				SML_LOG(TEXT("%s "), str.c_str());
			};

			MyList<std::wstring> lst;
			WCHAR chars[] = L"A";
			for (int ii = 0; ii < 26; ++ ii)
			{
				lst.AddTail(std::wstring(chars));
				++chars[0];
			}
			lst.AddTail(TEXT("\r\n"));
			lst.Enum(lst.Head(), printE);

			

			auto predOdd = [](const std::wstring & val)
			{
				return val.size() && ((int)(val[0]) & 1);
			};


			auto predEven = [](const std::wstring & val)
			{
				return val.size() && 0 == ((int)(val[0]) & 1);
			};

			MyListEntry<std::wstring> * iter = lst.Find(lst.Head(), predOdd);
			while (iter)
			{
				SML_LOG_LINE(TEXT("Found '%s'"), iter->Data().c_str());
				iter = lst.Find(iter->Next(), predOdd);
			}

			iter = lst.Remove(lst.Head(), predEven);
			while (iter)
			{
				iter = lst.Remove(iter, predEven);
			}
			lst.Enum(lst.Head(), printE);


			//iter = lst.Remove(lst.Head(), predOdd);
			//while (iter)
			//{
			//	iter = lst.Remove(iter, predOdd);
			//}
			//lst.Enum(lst.Head(), printE);

			while (lst.Remove(lst.Head()))
			{
				lst.Enum(lst.Head(), printE);
			}
			
		}
	};

} //namespace SmartLib