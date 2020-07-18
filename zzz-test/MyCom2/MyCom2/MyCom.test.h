#pragma once


#include <iostream>
#include <string>
#include <cassert>


#include "MyCom.h"



namespace SmartLib::Test
{
	using namespace ::std;


	class Sample
	{
		//sample
		//component C1 implements IA and IB
		//component C2 implements IC and ID, and aggregates component C1
		//component C3 implements IE and IF
		//component CAgg aggregates C2 and C3
		MIDL_INTERFACE("9B8EB8E1-B00E-406F-8457-887A2C8D4C05")
			IA : public IUnknown
		{
		public:
			virtual void fa1() = 0;
			virtual void fa2() = 0;
		};


		MIDL_INTERFACE("FB4A623E-6108-44EC-8D7E-258EC9B6FE7F")
			IB : public IUnknown
		{
		public:
			virtual void fb1() = 0;
			virtual void fb2() = 0;
		};

		MIDL_INTERFACE("74A60500-E0AA-4860-B24B-0163155A686C")
			IC : public IUnknown
		{
		public:
			virtual void fc1() = 0;
			virtual void fc2() = 0;
		};

		MIDL_INTERFACE("FB386C21-3E8E-478C-B249-6CF6C54D6B2F")
			ID : public IUnknown
		{
		public:
			virtual void fd1() = 0;
			virtual void fd2() = 0;
		};


		MIDL_INTERFACE("21F0E742-D953-43F7-B797-D92C392CDB6C")
			IE : public IUnknown
		{
		public:
			virtual void fe1() = 0;
			virtual void fe2() = 0;
		};

		MIDL_INTERFACE("873E5822-4AE2-489D-BC3D-E864925CBDC4")
			IF : public IUnknown
		{
		public:
			virtual void ff1() = 0;
			virtual void ff2() = 0;
		};


		class C1 :
			public CUnknown<IA, IB>
		{
		private:
			C1()
			{
				cout << "Constructor " << __FUNCTION__ << endl;
			}

		private:
			~C1() override
			{
				cout << "Destructor " << __FUNCTION__ << endl;
			}

		private:
			void fa1() override
			{
				cout << __FUNCTION__ << endl;
			}

		private:
			void fa2() override
			{
				cout << __FUNCTION__ << endl;
			}

		private:
			void fb1() override
			{
				cout << __FUNCTION__ << endl;
			}

		private:
			void fb2() override
			{
				cout << __FUNCTION__ << endl;
			}

		private:
			HRESULT InternalQueryInterface(REFIID iid, void** ppv) override
			{
				return InternalQueryInterfaceImpl(iid, ppv);
			}


		public:
			CREATE_INSTANCE(C1);
		};


		class C2 :
			public CUnknown<IC, ID>
		{
		private:
			IInternalUnknown* _pc1{ nullptr };

		private:
			C2()
			{
				cout << "Constructor " << __FUNCTION__ << endl;
				_pc1 = Helper::CreateIInternalUnknown<C1>(IUnknownPtr());

			}

			~C2() override
			{
				_pc1->InternalRelease();
				cout << "Destructor " << __FUNCTION__ << endl;
			}

		private:
			HRESULT STDMETHODCALLTYPE InternalQueryInterface(REFIID riid, void** ppvObject) override
			{
				InternalQueryInterfaceImpl(riid, ppvObject);
				if (nullptr == *ppvObject)
				{
					_pc1->InternalQueryInterface(riid, ppvObject);
				}

				return *ppvObject ? S_OK : E_NOINTERFACE;
			}

		private:
			void fc1() override
			{
				cout << __FUNCTION__ << endl;
			}

		private:
			void fc2() override
			{
				cout << __FUNCTION__ << endl;
			}

		private:
			void fd1() override
			{
				cout << __FUNCTION__ << endl;
			}

		private:
			void fd2() override
			{
				cout << __FUNCTION__ << endl;
			}


		public:
			CREATE_INSTANCE(C2);
		};


		class C3 :
			public CUnknown<IE, IF>
		{
		private:
			C3()
			{
				cout << "Constructor " << __FUNCTION__ << endl;
			}

		private:
			~C3() override
			{
				cout << "Destructor " << __FUNCTION__ << endl;
			}

		private:
			void fe1() override
			{
				cout << __FUNCTION__ << endl;
			}

		private:
			void fe2() override
			{
				cout << __FUNCTION__ << endl;
			}

		private:
			void ff1() override
			{
				cout << __FUNCTION__ << endl;
			}

		private:
			void ff2() override
			{
				cout << __FUNCTION__ << endl;
			}

		private:
			HRESULT InternalQueryInterface(REFIID iid, void** ppv) override
			{
				return InternalQueryInterfaceImpl(iid, ppv);
			}


		public:
			CREATE_INSTANCE(C3);
		};


		class  CAgg :
			protected CUnknown<IUnknown, IInternalUnknown>
		{
		private:
			IInternalUnknown* _pc2{ nullptr }; //aggregate component c2
			IInternalUnknown* _pc3{ nullptr }; //aggregate component c3


		public:
			CAgg()
			{
				cout << "Constructor " << __FUNCTION__ << endl;
				_pc2 = Helper::CreateIInternalUnknown<C2>(IUnknownPtr());
				_pc3 = Helper::CreateIInternalUnknown<C3>(IUnknownPtr());
				

			}

			~CAgg() override
			{
				_pc3->InternalRelease();
				_pc2->InternalRelease();
				cout << "Destructor " << __FUNCTION__ << endl;
			}

		private:
			HRESULT InternalQueryInterface(REFIID riid, void** ppvObject) override
			{
				InternalQueryInterfaceImpl(riid, ppvObject);
				if (nullptr == *ppvObject)
				{
					_pc2->InternalQueryInterface(riid, ppvObject);
				}

				if (nullptr == *ppvObject)
				{
					_pc3->InternalQueryInterface(riid, ppvObject);
				}

				return *ppvObject ? S_OK : E_NOINTERFACE;
			}

		public:
			CREATE_INSTANCE(CAgg);
		};

	public:
		static void Case1()
		{
			//using namespace SmartLib;

			CComPtr<IUnknown> sp;
			sp.Attach(C1::CreateInstance());

			CComPtr<IA> spa;
			HRESULT hr = sp.QueryInterface(&spa);

			if (spa)
			{
				spa->fa1();
				spa->fa2();
			}

			CComPtr<IB> spb;
			hr = spa.QueryInterface(&spb);
			if (spb)
			{
				spb->fb1();
				spb->fb2();
			}

			spa.Release();
			spb.QueryInterface(&spa);
			if (spa)
			{
				spa->fa1();
				spa->fa2();
			}
		}

		

	public:
		template<typename T>
		static void CaseT()
		{
			using namespace SmartLib;

			CComPtr<IUnknown> spI;
			spI.Attach(CAgg::CreateInstance());


			CComPtr<T> sp;
			spI.QueryInterface(&sp);


			CComPtr<IA> spa;
			CComPtr<IB> spb;
			CComPtr<IC> spc;
			CComPtr<ID> spd;
			CComPtr<IE> spe;
			CComPtr<IF> spf;

			sp.QueryInterface(&spa);
			if (spa)
			{
				spa->fa1();
				spa->fa2();
			}

			sp.QueryInterface(&spb);
			if (spb)
			{
				spb->fb1();
				spb->fb2();
			}


			sp.QueryInterface(&spc);
			if (spc)
			{
				spc->fc1();
				spc->fc2();
			}

			sp.QueryInterface(&spd);
			if (spd)
			{
				spd->fd1();
				spd->fd2();
			}

			sp.QueryInterface(&spe);
			if (spe)
			{
				spe->fe1();
				spe->fe2();
			}

			sp.QueryInterface(&spf);
			if (spf)
			{
				spf->ff1();
				spf->ff2();
			}
		}


	public:
		static void CaseAgg()
		{
			CaseT<IUnknown>();
			cout << endl << endl;

			CaseT<IA>();
			cout << endl << endl;

			CaseT<IB>();
			cout << endl << endl;

			CaseT<IC>();
			cout << endl << endl;

			CaseT<ID>();
			cout << endl << endl;

			CaseT<IE>();
			cout << endl << endl;

			CaseT<IF>();
			cout << endl << endl;

		}


	};

	class TestCases
	{
	public:
		static void Case1()
		{
			class DECLSPEC_UUID("1677B990-17FC-4A3B-8F39-BDEEDCF6251E") AA
			{

			};

			//CUnknown<AA> test;
		}

	public:
		static void Case0()
		{
			class DECLSPEC_UUID("7D0647CF-F395-42A2-BEF7-B27C41127880")
				A
			{
			public:
				void a()
				{
					cout << __FUNCTION__ << endl;
				}
			};

			class DECLSPEC_UUID("1DD28CBF-730F-4E3D-A3A6-85D23079C285")
				B
			{
			public:
				void b()
				{
					cout << __FUNCTION__ << endl;
				}
			};

			class DECLSPEC_UUID("88636E31-EC20-46AB-BA43-CA4422BDB9DE")
				C
			{
			public:
				void c()
				{
					cout << __FUNCTION__ << endl;
				}
			};

			class DECLSPEC_UUID("447220D5-A607-40EC-A3EF-22019F7F80E5")
				D
			{
			public:
				void d()
				{
					cout << __FUNCTION__ << endl;
				}
			};


			class DECLSPEC_UUID("447220D5-A618-40EC-A3EF-22019F7F80E5")
				E
			{
			public:
				void e()
				{
					cout << __FUNCTION__ << endl;
				}
			};


			CImplenment<A, B, C, D> t;
			//t.a();
			//t.b();
			//t.c();
			//t.d();


			/*{

				auto pobj = (A*)t.Query(__uuidof(A));
				pobj->a();
			}

			{

				auto pobj = (B*)t.Query(__uuidof(B));
				pobj->b();
			}

			{

				auto pobj = (C*)t.Query(__uuidof(C));
				pobj->c();

			}

			{

				auto pobj = (D*)t.Query(__uuidof(D));
				pobj->d();
			}

			{

				auto pobj = (E*)t.Query(__uuidof(E));
				assert(nullptr == pobj);
			}*/

		}
	};
}