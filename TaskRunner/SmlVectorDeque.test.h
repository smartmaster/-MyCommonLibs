#pragma once

#include <string>
#include <iostream>

#include "SmlVectorDeque.h"


namespace SmartLib
{
	using namespace ::std;

	class DequeTest
	{
	public:



		template<typename CC>
		static void IterPrint(typename CC& cc)
		{
			for (auto iter = cc.begin(); iter != cc.end(); ++iter)
			{
				cout << *iter << " ";
			}
			cout << endl;
		}

		template<typename CC>
		static void IterReversePrint(typename CC& cc)
		{
			for (auto iter = cc.rbegin(); iter != cc.rend(); --iter)
			{
				cout << *iter << " ";
			}
			cout << endl;
		}


		static void Case3()
		{
			VectorDeque<string, true, 1> dq;
			for (long ii = 0; ii < 20; ++ii)
			{
				string str = to_string(ii);
				if (ii & 1)
				{
					dq.push_front(move(str));
				}
				else
				{
					dq.push_front(str);
				}
			}
			IterPrint(dq);
			IterReversePrint(dq);


			VectorDeque<string, true, 1> dqCopy;// { dq };
			dqCopy = dq;
			IterPrint(dqCopy);
			IterReversePrint(dqCopy);

			VectorDeque<string, true, 1> dqMove;// { move(dq) };
			dqMove = move(dq);
			IterPrint(dqMove);
			IterReversePrint(dqMove);
		}

		static void Case2()
		{
			VectorDeque<string, true, 1> dq;
			for (long ii = 0; ii < 20; ++ii)
			{
				string str = to_string(ii);
				if (ii & 1)
				{
					dq.push_front(move(str));
				}
				else
				{
					dq.push_front(str);
				}
			}
			IterPrint(dq);
			IterReversePrint(dq);


			VectorDeque<string, true, 1> dqCopy{ dq };
			IterPrint(dqCopy);
			IterReversePrint(dqCopy);

			VectorDeque<string, true, 1> dqMove{ move(dq) };
			IterPrint(dqMove);
			IterReversePrint(dqMove);
		}

		static void Case1()
		{
			VectorDeque<string, true, 1> dq;
			for (long ii = 0; ii < 20; ++ii)
			{
				string str = to_string(ii);
				if (ii & 1)
				{
					dq.push_front(move(str));
				}
				else
				{
					dq.push_front(str);
				}
			}
			IterPrint(dq);
			IterReversePrint(dq);
			



			while (dq.size())
			{
				string str = dq.pop_front();
				IterReversePrint(dq);
				//IterReversePrint(dq);
			}
		}

		static void Case0()
		{
			VectorDeque<string, true, 1> dq;
			for (long ii = 0; ii < 20; ++ ii)
			{
				string str = to_string(ii);
				if (ii & 1)
				{
					dq.push_back(move(str));
				}
				else
				{
					dq.push_back(str);
				}
			}
			IterReversePrint(dq);
			IterPrint(dq);
			


			while (dq.size())
			{
				string str = dq.pop_back();
				IterPrint(dq);
				//IterReversePrint(dq);
			}
		}
	};
}