#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <typeinfo>
#include <thread>
#include <functional>
#include <iostream>
#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/pool/simple_segregated_storage.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <boost/pool/pool_alloc.hpp>


namespace
{
	//template<typename T>
	static void intrusive_ptr_add_ref(IDispatch* p)
	{
		p->AddRef();
	}

	//template<typename T>
	static void intrusive_ptr_release(IDispatch* p)
	{
		p->Release();
	}
}
#include <boost/intrusive_ptr.hpp>





namespace SmartLib
{
	using namespace ::boost;
	using namespace ::std;

	class Eg0103SharedPtrArray
	{
	public:

		static void Case5()
		{
			{
				std::vector<int, boost::pool_allocator<int>> v;
				for (int i = 0; i < 1000; ++i)
				{
					v.push_back(i);
				}
				v.clear();
				boost::singleton_pool<boost::pool_allocator_tag, sizeof(int)>::purge_memory();
			}
			{
				struct int_pool {};
				typedef boost::singleton_pool<int_pool, sizeof(int)> singleton_int_pool;

				int* i = static_cast<int*>(singleton_int_pool::malloc());
				*i = 1;

				int* j = static_cast<int*>(singleton_int_pool::ordered_malloc(10));
				j[9] = 2;

				singleton_int_pool::release_memory();
				singleton_int_pool::purge_memory();
			}
			//////////////////////////////////////////////////////////////////////////
			{
				boost::object_pool<int> pool{ 32, 0 };
				int* data = pool.construct(100);
				std::cout << pool.get_next_size() << endl;
				pool.set_next_size(8);
				std::cout << pool.get_next_size() << endl;
			}

			//////////////////////////////////////////////////////////////////////////
			{
				boost::object_pool<string> pool;

				
				string* j = pool.construct("world~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

				pool.destroy(j);


			}
			//////////////////////////////////////////////////////////////////////////
			{
				boost::object_pool<int> pool;

				int* i = pool.malloc();
				*i = 100;// "hello~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";

				int* j = pool.construct(200/*"world~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"*/);

				pool.destroy(i);
				pool.destroy(j);

			}

			
		//////////////////////////////////////////////////////////////////////////
			{
				boost::simple_segregated_storage<std::size_t> storage;
				std::vector<char> v(1024);
				storage.add_block(&v[0], v.size(), 256);

				int* i = static_cast<int*>(storage.malloc());
				*i = 1;

				int* j = static_cast<int*>(storage.malloc_n(1, 512));
				j[10] = 2;

				storage.free(i);
				storage.free_n(j, 1, 512);
			}
			
		}


		static void Case4()
		{
			
			::CoInitialize(0);
			std::unique_ptr<void, std::function<void(void*)>> couninit{
				(void*)(1),
				[](void*)
				{
				::CoUninitialize();
				}
			};

			CLSID clsid;
			::CLSIDFromProgID(CComBSTR{TEXT("Scripting.FileSystemObject")}, &clsid);
			void* p;
			::CoCreateInstance(clsid, 0, CLSCTX_INPROC_SERVER, __uuidof(IDispatch), &p);

			boost::intrusive_ptr<IDispatch> disp{ static_cast<IDispatch*>(p), false };
			
			CComDispatchDriver dd{ disp.get() };
			CComVariant arg{ "C:\\Windows" };
			CComVariant ret{ false };
			dd.Invoke1(CComBSTR{ TEXT("FolderExists") }, & arg, & ret);
			std::cout << std::boolalpha << (ret.boolVal != 0) << '\n';
		}

		static void Reset(boost::shared_ptr<int>& sp)
		{
			::Sleep(100);
			sp.reset();
		}
		static void Print(boost::weak_ptr<int>& wp)
		{
			auto sp = wp.lock();
			if (sp)
			{
				cout << *sp << endl;
			}
		}

		static void Case3()
		{
			boost::shared_ptr<int> sp{ new int{99} };
			boost::weak_ptr<int> wp{ sp };
			
			std::thread t1{Reset, std::ref(sp)};
			std::thread t2{Print, std::ref(wp) };

			t1.join();
			t2.join();
			return;
		}

		static void Case2()
		{
			//auto p1 = boost::make_shared<int>(1);
			//std::cout << typeid(p1).name() << '\n';
			//auto p2 = boost::make_shared<int[]>(10);
			//std::cout << typeid(p2).name() << '\n';

			//auto sp1 = boost::make_shared<int>(1);
			//cout << typeid(sp1).name() << endl;

			//auto sp2 = boost::make_shared<int[]>(10);
			//cout << typeid(sp2).name() << endl;

			//auto p1 = boost::make_shared<int>(1);
			//std::cout << typeid(p1).name() << '\n';
			//auto p2 = boost::make_shared<int[]>(10);
			//std::cout << typeid(p2).name() << '\n';

			boost::shared_array<int> sa{new int[10]};
			{
				boost::shared_array<int> sa2{sa};
				sa2[3] = 300;
				sa2.reset();
			}

			cout << sa[3] << endl;
		}
		static void Case1()
		{
			boost::shared_ptr<HANDLE> handle{
				new HANDLE{::OpenProcess(PROCESS_SET_INFORMATION, FALSE, ::GetCurrentProcessId())}, 
				[](HANDLE* ph)
				{
					::CloseHandle(*ph);
					delete ph;
					ph = nullptr;
				}
			};


			boost::shared_ptr<void> sphandle{
				(void*)::OpenProcess(PROCESS_SET_INFORMATION, FALSE, ::GetCurrentProcessId()),
				[](void* hh)
				{
					::CloseHandle((HANDLE)(hh));
				}
			};
			return;
		}

		static void Case0()
		{
			boost::shared_ptr<int> sp1{ new int{100} };
			boost::shared_ptr<int> sp2{sp1};
			cout << *sp1 << endl;
			cout << *sp2 << endl;
			cout << *sp2.get() << endl;

			boost::weak_ptr<int> wp{sp2};
			auto locked = wp.lock();
			if (locked)
			{
				cout << *locked << endl;
			}


			sp1.reset();
			cout << boolalpha << static_cast<bool>(sp1) << endl;
			cout << boolalpha << static_cast<bool>(sp2) << endl;
		}
	};
}