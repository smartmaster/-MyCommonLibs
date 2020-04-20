#pragma once

#include <iostream>
#include "sml-common.h"

namespace SmartLib
{
	using namespace ::std;

	//class A
	//{
	//	int _a;
	//};

	//class B
	//{
	//	int _b;
	//};

	//class C : 
	//	public A,
	//	public B
	//{
	//	int _c;
	//};

	//class D :
	//	 public virtual A,
	//	public virtual B
	//{
	//	int _d;
	//};


	//class A
	//{
	//};
	//class B
	//{
	//};

	//class C
	//{
	//};
	//class D
	//{
	//};
	//class E : public virtual A, public virtual B, public virtual C, public virtual D
	//{
	//};


	class A
	{
	public:
		long long _a;

		virtual void f() {}
	};
	
	class B : public virtual A
	{
	public:
		long long _b;
	};

	class C: public virtual A
	{
	public:
		long long _c;
	};


	class D : public B, public C
	{
	public:
		long long _d;

	};

	class SizeOfClass
	{
	public:
		
		static long Diff(const void* p1, const void* p2)
		{
			return (long)((const char*)p1 - (const char*)p2);
		}

		static void Case0()
		{
			Common::Print(cout, "sizeof(long long) = ", sizeof(long long)) << endl;
			Common::Print(cout, "sizeof(long) = ", sizeof(long)) << endl;
			Common::Print(cout, "sizeof(A) = ", sizeof(A)) << endl;
			Common::Print(cout, "sizeof(B) = ", sizeof(B)) << endl;
			Common::Print(cout, "sizeof(C) = ", sizeof(C)) << endl;
			Common::Print(cout, "sizeof(D) = ", sizeof(D)) << endl;

			D dd;

			D* d = &dd;
			d->_d = 'dddd';

			C* c = d;
			c->_c = 'cccc';


			B* b = d;
			b->_b = 'bbbb';

			A* a = d;
			a->_a = 'aaaa';



			Common::Print(cout, Diff(a, d), " ", Diff(b, d), " ", Diff(c, d)) << endl;
		}
	};
}