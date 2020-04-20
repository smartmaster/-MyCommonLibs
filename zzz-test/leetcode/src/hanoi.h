#pragma once

#include <stack>
#include <iostream>

#include "sml-common.h"

namespace SmartLib
{
	using namespace ::std;

	class HannoiAlg
	{
	public:
		static void Hannoi(long count, char A, char B, char C)
		{
			if (0 == count)
			{
				return;
			}

			Hannoi(count - 1, A, C, B);
			Common::Print(cout, A, " -> ", C) << endl;
			Hannoi(count - 1, B, A, C);
		}


		static void HanoiNR(long count, char A, char B, char C)
		{
			struct Problem
			{
				long _count;
				char _A;
				char _B;
				char _C;
				char _{ '\0' };

				Problem(long count, char A, char B, char C) :
					_count{count},
					_A{ A },
					_B{ B },
					_C{ C }
				{
				}

			};


			stack<Problem> stack;
			stack.push(Problem{ count, A, B, C});
			while (stack.size())
			{
				auto top = stack.top();
				stack.pop();
				if (top._count == 1)
				{
					Common::Print(cout, top._A, " -> ", top._C) << endl;
				}
				else
				{
					stack.push(Problem{ top._count - 1, top._B, top._A, top._C });
					stack.push(Problem{ 1, top._A, top._B, top._C });
					stack.push(Problem{ top._count - 1, top._A, top._C, top._B });
				}
			}

		}
	};
}