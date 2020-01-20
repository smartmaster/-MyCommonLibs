#pragma once


#include "log.h"
#include "TestAlg2.h"
#include "ScopeExec.h"

namespace TestAlg2
{
	class TestPerm
	{
	private:
		int _total{ 0 }; /***INCOMMING***/
		int _count{ 0 }; /***INCOMMING***/
		int _resultCount{ 0 }; /***INTERNAL***/
		LinkedList<int> _stack; /***INTERNAL***/
		SCOPE_EXEC(_stack) {
			[this]()
			{
				int stackCount = _stack.GetCount();
				assert(0 == stackCount);
				_stack.Cleanup([](LinkedList<int>::Node * node)
				{
					LinkedList<int>::Node::Delete(node);
				});
			}
		};

		int* _arr{ nullptr }; /***INTERNAL***//***CLEANUP***/
		SCOPE_EXEC(_arr) {
			[this]() {
				if (_arr)
				{
					delete[] _arr;
					_arr = nullptr;
				}
			}
		};


	public:
		TestPerm(int total, int count) :
			_total(total),
			_count(count)
		{
			assert(count >= 1 && total >= count);
			_arr = new int[_total];
			for (int ii = 0; ii < _total; ++ii)
			{
				_arr[ii] = ii;
			}
		}

		void Perm()
		{
			for (int ii = 0; ii < _total; ++ii)
			{
				if (_arr[ii] >= 0)
				{
					_stack.PushBackData(_arr[ii]);
					_arr[ii] = -1;

					if (_count == _stack.GetCount())
					{
						++ _resultCount;
						LOG(TEXT("%05d:\t"), _resultCount);
						_stack.ForEach(_stack.GetFirst(), [](const int & val) {
							LOG(TEXT("%2d "), val + 1);
						});
						LOG_LINE(TEXT(""));
					}
					else
					{

						Perm();
					}

					int data = _stack.PopBackData();
					_arr[data] = data;
				}
			}
		}


		void Combine()
		{
			for (int ii = 0; ii < _total; ++ii)
			{
				if (0 == _stack.GetCount() || ii > _stack.GetLast()->_value)
				{
					_stack.PushBackData(ii);

					if (_count == _stack.GetCount())
					{
						++_resultCount;

						LOG(TEXT("%05d:\t"), _resultCount);
						_stack.ForEach(_stack.GetFirst(), [](const int & val) {
							LOG(TEXT("%2d "), val + 1);
						});
						LOG_LINE(TEXT(""));
					}
					else
					{

						Combine();
					}

					_stack.PopBackData();
				}
				
			}
		}

		virtual ~TestPerm()
		{

		}


		static void PermApi(int total, int count)
		{
			TestPerm pp(total, count);
			pp.Perm();
		}

		static void CombineApi(int total, int count)
		{
			TestPerm pp(total, count);
			pp.Combine();
		}

	};

} //namespace TestAlg2
