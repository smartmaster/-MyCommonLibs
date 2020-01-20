#pragma once

#include <string>
#include <iostream>

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "SmlRefPtr.h"
#include "SmlVector.h"

namespace SmartLib
{
	using namespace ::std;

	class MyRCObj
	{
		static long& Id()
		{
			static long id = 0;
			return id;
		}

		std::string _str;
		long _id;
	public:

		static long& CtorCount()
		{
			static long count = 0;
			return count;
		}


		static long& DtorCount()
		{
			static long count = 0;
			return count;
		}

		MyRCObj(const std::string& str) :
			_str(str),
			_id(++Id())
		{
			++CtorCount();
			//SML_LOG_LINE(TEXT("ctor of [%d, %S]"), _id, _str.c_str());
		}

		~MyRCObj()
		{
			++DtorCount();
			//SML_LOG_LINE(TEXT("detor of [%d, %S]"), _id, _str.c_str());
		}

		void Print()
		{
			//SML_LOG_LINE(TEXT("I am [%d, %S]"), _id, _str.c_str());
		}
	};

	class RefPtrTest
	{
	public:

		struct MyObj
		{
		private:
			static inline int _count{ 0 };
		private:
			string _str;
		public:
			MyObj()
			{
				_str = to_string(++_count);
			}

			~MyObj()
			{
				cout << "desttctor " << _str << endl;
			}
		};

		static void Case4()
		{
			struct MyStruct
			{
				MyObj obj;
				RefPtr<MyStruct> self;
			};

			RefPtr<MyStruct> sp = RefPtr<MyStruct>::Make();
			sp->self = sp;

		}

		static void Case3()
		{
			MyObj* p = nullptr;
			MyObj* p1 = nullptr;

			{
				p = new MyObj{};
				ScopeResource sp{ [&p]()
				{
					if (p)
					{
						delete p;
						p = nullptr;
					}
				} };



				p1 = new MyObj{};

				auto disp = [&p1]()
				{
					if (p1)
					{
						delete p1;
						p1 = nullptr;
					}
				};
				ScopeResource sp1{ disp };
			}


			assert(nullptr == p);
			assert(nullptr == p1);

			return;
		}

		static void Case2()
		{
			RefPtr<MyRCObj>::ObjectBlock ob1{"111"};
			RefPtr<MyRCObj>::ObjectBlock ob2{"222"};
			RefPtr<MyRCObj>::ObjectBlock ob3{ "333" };

			//ob2 = ob1;
			//ob3 = move(ob3);

		}
		
		static void Case1()
		{
			{
				Vector<RefPtr<string>, true, 4> vec;
				string temp{ "hello, this is a long strong .................$$$$$" };
				RefPtr<string> sp1 = RefPtr<string>::Make(temp);

				RefPtr<string> sp2;
				sp2 = sp1;
				sp2 = sp1;
				sp2 = move(sp2);
			}


			{
				Vector<RefPtr<string>, true, 4> vec;
				string temp{ "hello, this is a long strong .................$$$$$" };
				RefPtr<string> sp1 = RefPtr<string>::Make(move(temp));

				RefPtr<string> sp2;
				sp2 = sp1;
				sp2 = sp1;
				sp2 = move(sp2);
			}

			{
				Vector<RefPtr<string>, true, 4> vec;
				string temp{ "hello, this is a long strong .................$$$$$" };
				RefPtr<string> sp1 = RefPtr<string>::Make(move(temp));

				RefPtr<string> sp2;
				sp2 = sp1;
				sp2 = sp1;
				sp2 = move(sp1);
			}
			{
				Vector<RefPtr<string>, true, 4> vec;
				string temp{ "hello, this is a long strong .................$$$$$" };
				RefPtr<string> sp1 = RefPtr<string>::Make(move(temp));
				for (long ii = 0; ii < 100; ++ ii)
				{
					vec.push_back(sp1);
				}

				while (vec.size())
				{
					vec.pop_back();
				}
			}
			{
				string temp{"hello, this is a long strong .................$$$$$"};
				RefPtr<string> sp1 = RefPtr<string>::Make(move(temp));
				RefPtr<string> sp2;
				sp2 = sp1;
				sp2 = sp1;
				sp2 = sp1;
			}
			{
				auto dispose = [](WCHAR*& data)
				{
					::free(data);
					data = nullptr;
				};
				const long BUFFER_SIZE = 100;
				RefPtr<WCHAR*>::ObjectBlock obj{ (WCHAR*)malloc(100 * sizeof(WCHAR)) };
				obj.SetDispose(dispose);
				_tcscpy_s(obj.Ref(), BUFFER_SIZE, TEXT("hello world"));
				//SML_LOG_LINE(TEXT("%s"), obj.Ref());
			}


			{
				//////////////////////////////////////////////////////////////////////////
				auto dispose = [](WCHAR*& data)
				{
					::free(data);
					data = nullptr;
				};
				const long BUFFER_SIZE = 100;
				RefPtr<WCHAR*>::ObjectBlock obj{(WCHAR*)malloc(100 * sizeof(WCHAR))};
				obj.SetDispose(dispose);
				_tcscpy_s(obj.Ref(), BUFFER_SIZE, TEXT("hello world"));
				//SML_LOG_LINE(TEXT("%s"), obj.Ref());
			}
		}
		static void Case0()
		{
			//////////////////////////////////////////////////////////////////////////
			{
				RefPtr<MyRCObj> sp1 = RefPtr<MyRCObj>::Make(std::string{ "hello world!" });
				RefPtr<MyRCObj> sp2{ sp1 };
			}

			//////////////////////////////////////////////////////////////////////////
			{
				RefPtr<MyRCObj> sp1;
				sp1 = RefPtr<MyRCObj>::Make(std::string{ "hello world!" });
				RefPtr<MyRCObj> sp2;
				sp2 = sp1;
			}

			//////////////////////////////////////////////////////////////////////////
			{
				RefPtr<MyRCObj> sp1 = RefPtr<MyRCObj>::Make(std::string{ "hello world!" });
				RefPtr<MyRCObj> sp2 = RefPtr<MyRCObj>::Make(std::string{ "hello world!" });
				sp1 = sp2;
			}

			//////////////////////////////////////////////////////////////////////////
			{
				RefPtr<MyRCObj> sp1 = RefPtr<MyRCObj>::Make(std::string{ "hello world!" });
				RefPtr<MyRCObj> sp2 = RefPtr<MyRCObj>::Make(std::string{ "hello world!" });
				sp1.Attach(sp2.Detach());
			}

			{
				RefPtr<MyRCObj> sp1{ RefPtr<MyRCObj>::Make(std::string{ "hello world!" }) };
				sp1->Print();
				(*sp1).Print();
			}

			{
				RefPtr<MyRCObj> sp1{ RefPtr<MyRCObj>::Make(std::string{ "hello world!" }) };
				sp1.Attach(nullptr);
			}

			if (MyRCObj::CtorCount() == MyRCObj::DtorCount())
			{
				//SML_LOG_LINE(TEXT("test passed, ctor and dtor count is %d"), MyObj::CtorCount());
			}
			else
			{
				//SML_LOG_LINE(TEXT("test fail, ctor count is %d but dtor count is %d"), MyObj::CtorCount(), MyObj::DtorCount());
			}


			{
				CString fileName = TEXT("f:\\000-test-refptr.txt");
				HANDLE hfile = ::CreateFile(
					fileName.GetString(),//_In_      LPCTSTR lpFileName,
					GENERIC_ALL,//_In_      DWORD dwDesiredAccess,
					0,//_In_      DWORD dwShareMode,
					nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
					CREATE_ALWAYS,//_In_      DWORD dwCreationDisposition,
					0,//_In_      DWORD dwFlagsAndAttributes,
					nullptr//,//_In_opt_  HANDLE hTemplateFile
				);

				auto delfile = [](CString& file)
				{
					::DeleteFile(file.GetString());
				};
				RefPtr<CString> spdelfile{ RefPtr<CString>::Make(fileName.GetString()) };
				spdelfile.SetDispose(delfile);

				auto dispose = [](HANDLE& h)
				{
					::CloseHandle(h);
					h = INVALID_HANDLE_VALUE;
				};
				RefPtr<HANDLE> sp1 = RefPtr<HANDLE>::Make(hfile);
				sp1.SetDispose(dispose);
				RefPtr<HANDLE> sp2 = sp1;
				RefPtr<HANDLE> sp3;
				sp3 = sp1;
			}


			{
				auto dispose = [](char*& data)
				{
					if (data)
					{
						::free(data);
						data = nullptr;
					}
				};


				const long BUFFER_SIZE = 100;
				RefPtr<char*> sp1 = RefPtr<char*>::Make((char*)::malloc(BUFFER_SIZE));
				sp1.SetDispose(dispose);

				strcpy_s(sp1.Ref(), BUFFER_SIZE, "hello world");
				//SML_LOG_LINE(TEXT("%S"), sp1.Ref());

			}
		}
	};
}