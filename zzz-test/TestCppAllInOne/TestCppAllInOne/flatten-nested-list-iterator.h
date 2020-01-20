#pragma once

#include <vector>
#include <memory>

//https://leetcode.com/problems/flatten-nested-list-iterator/


namespace SmartLib_NestedInteger2
{
	using namespace ::std;

	// This is the interface that allows for creating nested lists.
	// You should not implement it, or speculate about its implementation
	class NestedInteger
	{
	public:
		bool _isInt{ false };
		int _ivalue{ 0 };
		vector<NestedInteger> _vec;

	public:
		// Return true if this NestedInteger holds a single integer, rather than a nested list.
		bool isInteger() const
		{
			return _isInt;
		}

		// Return the single integer that this NestedInteger holds, if it holds a single integer
		// The result is undefined if this NestedInteger holds a nested list
		int getInteger() const
		{
			return _ivalue;
		}

		// Return the nested list that this NestedInteger holds, if it holds a nested list
		// The result is undefined if this NestedInteger holds a single integer
		const vector<NestedInteger>& getList() const
		{
			return _vec;
		}
	};



	class NestedIterator
	{
	private:
		vector<const NestedInteger*> _stack; /***INTERNAL***/


	private:
		void PushListToStack(const vector<NestedInteger>& nestedList)
		{
			if (nestedList.size())
			{
				const NestedInteger* last = &nestedList.back();
				const NestedInteger* first = &nestedList.front();

				while (last >= first) //push in reverse order
				{
					_stack.push_back(last);
					--last;
				}
			}
		}

	public:
		NestedIterator(vector<NestedInteger>& nestedList)
		{
			PushListToStack(nestedList);
		}

		int next()
		{
			int result = _stack.back()->getInteger();
			_stack.pop_back();
			return result;
		}

		bool hasNext()
		{
			if (_stack.empty())
			{
				return false;
			}

			while (!_stack.empty())
			{
				const NestedInteger* top = _stack.back();
				if (top->isInteger())
				{
					return true;
				}

				_stack.pop_back();
				PushListToStack(top->getList());
			}

			return false;
		}
	};
}


namespace SmartLib_NestedInteger
{
	using namespace ::std;

	// This is the interface that allows for creating nested lists.
	// You should not implement it, or speculate about its implementation
	class NestedInteger
	{
	public:
		bool _isInt{ false };
		int _ivalue{ 0 };
		vector<NestedInteger> _vec;

	public:
		// Return true if this NestedInteger holds a single integer, rather than a nested list.
		bool isInteger() const
		{
			return _isInt;
		}

		// Return the single integer that this NestedInteger holds, if it holds a single integer
		// The result is undefined if this NestedInteger holds a nested list
		int getInteger() const
		{
			return _ivalue;
		}

		// Return the nested list that this NestedInteger holds, if it holds a nested list
		// The result is undefined if this NestedInteger holds a single integer
		const vector<NestedInteger>& getList() const
		{
			return _vec;
		}
	};


	class SingleNestedIntegerIterator
	{
	private:
		const NestedInteger* _ni{ nullptr }; /***INCOMMING***/

		int _index{ 0 }; /***INTERNAL***/
		vector<unique_ptr<SingleNestedIntegerIterator>> _subIter; /***INTERNAL***/

	public:

		SingleNestedIntegerIterator(SingleNestedIntegerIterator&& obj) noexcept :
			_ni{ obj._ni },
			_index{ obj._index },
			_subIter{ move(obj._subIter) }
		{
		}

		SingleNestedIntegerIterator(const NestedInteger* ni) :
			_ni{ ni }
		{
			if (!_ni->isInteger())
			{
				for (const auto& subni : _ni->getList())
				{
					_subIter.push_back(make_unique<SingleNestedIntegerIterator>(&subni));
				}
			}
		}

		int next()
		{
			if (_ni->isInteger())
			{
				_index = 1;
				return _ni->getInteger();
			}
			else
			{
				int result = _subIter[_index]->next();;
				if (!_subIter[_index]->hasNext())
				{
					++_index;
				}
				return result;
			}
		}

		bool hasNext()
		{
			if (_ni->isInteger())
			{
				return _index == 0;
			}
			else
			{
				bool result = false;
				while (_index < _subIter.size())
				{
					if (_subIter[_index]->hasNext())
					{
						result = true;
						break;
					}

					++_index;
				}

				return result;
			}
		}
	};


	class NestedIterator
	{
		const vector<NestedInteger>& _nestedList; /***INCOMMING***/

		int _index{ 0 }; /***INTERNAL***/
		vector<unique_ptr<SingleNestedIntegerIterator>> _subiter; /***INTERNAL***/

	public:
		NestedIterator(vector<NestedInteger>& nestedList) :
			_nestedList(nestedList)
		{
			for (const auto& ni : _nestedList)
			{
				_subiter.push_back(make_unique<SingleNestedIntegerIterator>(&ni));
			}
		}

		int next()
		{
			int result = _subiter[_index]->next();
			if (!_subiter[_index]->hasNext())
			{
				++_index;
			}
			return result;
		}

		bool hasNext()
		{
			bool result = false;
			while (_index < _subiter.size())
			{
				if (_subiter[_index]->hasNext())
				{
					result = true;
					break;
				}
				++_index;
			}

			return result;
		}
	};
}


namespace SmartLib
{

	using namespace ::std;

	// This is the interface that allows for creating nested lists.
	// You should not implement it, or speculate about its implementation
	class NestedInteger
	{
	public:
		int _ivalue;
		vector<NestedInteger> _vec;

	public:
		// Return true if this NestedInteger holds a single integer, rather than a nested list.
		bool isInteger() const
		{
			return !_vec.empty();
		}

		// Return the single integer that this NestedInteger holds, if it holds a single integer
		// The result is undefined if this NestedInteger holds a nested list
		int getInteger() const
		{
			return _ivalue;
		}

		// Return the nested list that this NestedInteger holds, if it holds a nested list
		// The result is undefined if this NestedInteger holds a single integer
		const vector<NestedInteger>& getList() const
		{
			return _vec;
		}
	};


	class SingleNestedIntegerIterator
	{
	private:
		const NestedInteger* _ni{ nullptr }; /***INCOMMING***/

		int _index{ 0 }; /***INTERNAL***/
		vector<SingleNestedIntegerIterator> _subIter; /***INTERNAL***/

	public:

		SingleNestedIntegerIterator(SingleNestedIntegerIterator&& obj) noexcept :
			_ni{ obj._ni },
			_index{ obj._index },
			_subIter{ move(obj._subIter) }
		{
		}

		SingleNestedIntegerIterator(const NestedInteger* ni) :
			_ni{ ni }
		{
			if (!_ni->isInteger())
			{
				for (const auto& subni : _ni->getList())
				{
					_subIter.push_back(SingleNestedIntegerIterator{ &subni });
				}
			}
		}

		int next()
		{
			if (_ni->isInteger())
			{
				++_index;
				return _ni->getInteger();
			}
			else
			{
				int result = _subIter[_index].next();;
				if (!_subIter[_index].hasNext())
				{
					++_index;
				}
				return result;
			}
		}

		bool hasNext()
		{
			if (_ni->isInteger())
			{
				return _index == 0;
			}
			else
			{
				bool result = false;
				while (_index < _subIter.size())
				{
					if (_subIter[_index].hasNext())
					{
						result = true;
						break;
					}

					++_index;
				}

				return result;
			}
		}
	};


	class NestedIterator
	{
		const vector<NestedInteger>& _nestedList; /***INCOMMING***/

		int _index{ 0 }; /***INTERNAL***/
		vector<SingleNestedIntegerIterator> _subiter; /***INTERNAL***/

	public:
		NestedIterator(vector<NestedInteger>& nestedList) :
			_nestedList(nestedList)
		{
			for (const auto& ni : _nestedList)
			{
				_subiter.push_back(SingleNestedIntegerIterator{ &ni });
			}
		}

		int next()
		{
			int result = _subiter[_index].next();
			if (!_subiter[_index].hasNext())
			{
				++_index;
			}
			return result;
		}

		bool hasNext()
		{
			bool result = false;
			while (_index < _subiter.size())
			{
				if (_subiter[_index].hasNext())
				{
					result = true;
					break;
				}
				++_index;
			}

			return result;
		}
	};
}