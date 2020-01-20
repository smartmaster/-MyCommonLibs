#pragma once

#include "ScopeExec.h"

namespace TestCpp
{
	class Inner
	{
	public:
		int v{ 0 };


		Inner(int seed) :
			v(seed)
		{
		}
	};


	class Outer
	{
	public:
		int v{ 0 };
		Inner o;
		Inner * p{ nullptr };
		SCOPE_EXEC(p) {
			[this]() -> void
			{
				if (p)
				{
					delete p;
					p = nullptr;
				}
			}
		};


		Outer() :
			v(1000),
			o(1000),
			p(new Inner(1000))
		{
		}
	};

	class  TestConstCases
	{
	public:
		static void Case0(const Outer * p)
		{
			//p->v = 100;
			//p->o.v = 100;
			//p->p = nullptr;
			p->p->v = 100;
		}


		static void Case1()
		{
			Outer * p = new Outer();
			SCOPE_EXEC(p) {
				nullptr, 

				[&p]()
				{
					if (p)
					{
						delete p;
						p = nullptr;
					}
				}
			};


			Case0(p);
		}
	};

}