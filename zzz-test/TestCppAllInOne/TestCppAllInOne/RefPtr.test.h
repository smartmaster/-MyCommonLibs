#pragma once

#include <string>
#include "log.h"
#include "RefPtr.h"

namespace SmartLib 
{
	class MyObj
	{
		static int& Id()
		{
			static int id = 0;
			return id;
		}

		std::string _str;
		int _id;
	public:

		static int& CtorCount()
		{
			static int count = 0;
			return count;
		}


		static int& DtorCount()
		{
			static int count = 0;
			return count;
		}

		MyObj(const std::string& str) :
			_str(str),
			_id(++Id())
		{
			++CtorCount();
			SML_LOG_LINE(TEXT("ctor of [%d, %S]"), _id, _str.c_str());
		}

		~MyObj()
		{
			++DtorCount();
			SML_LOG_LINE(TEXT("detor of [%d, %S]"), _id, _str.c_str());
		}

		void Print()
		{
			SML_LOG_LINE(TEXT("I am [%d, %S]"), _id, _str.c_str());
		}
	};

	class RefPtrTest
	{
	public:
		static void Case1()
		{
			{
				auto dispose = [](WCHAR* & data)
				{
					::free(data);
					data = nullptr;
				};
				const int BUFFER_SIZE = 100;
				ObjWrap<WCHAR*> obj = ObjWrap<WCHAR*>::ScopedWithDispose(dispose, (WCHAR*)malloc(100*sizeof(WCHAR)));
				_tcscpy_s(obj.Ref(), BUFFER_SIZE, TEXT("hello world"));
				SML_LOG_LINE(TEXT("%s"), obj.Ref());
			}


			{
				//////////////////////////////////////////////////////////////////////////
				auto dispose = [](WCHAR*& data)
				{
					::free(data);
					data = nullptr;
				};
				const int BUFFER_SIZE = 100;
				ObjWrap<WCHAR*> obj = ObjWrap<WCHAR*>::Scoped((WCHAR*)malloc(100 * sizeof(WCHAR)));
				obj.SetDispose(dispose);
				_tcscpy_s(obj.Ref(), BUFFER_SIZE, TEXT("hello world"));
				SML_LOG_LINE(TEXT("%s"), obj.Ref());
			}
		}
		static void Case0()
		{
			//////////////////////////////////////////////////////////////////////////
			{
				RefCountPtr<MyObj> sp1 = RefCountPtr<MyObj>::Make(std::string{ "hello world!" });
				RefCountPtr<MyObj> sp2{ sp1 };
			}

			//////////////////////////////////////////////////////////////////////////
			{
				RefCountPtr<MyObj> sp1;
				sp1 = RefCountPtr<MyObj>::Make(std::string{ "hello world!" });
				RefCountPtr<MyObj> sp2;
				sp2 = sp1;
			}

			//////////////////////////////////////////////////////////////////////////
			{
				RefCountPtr<MyObj> sp1 = RefCountPtr<MyObj>::Make(std::string{ "hello world!" });
				RefCountPtr<MyObj> sp2 = RefCountPtr<MyObj>::Make(std::string{ "hello world!" });
				sp1 = sp2;
			}

			//////////////////////////////////////////////////////////////////////////
			{
				RefCountPtr<MyObj> sp1 = RefCountPtr<MyObj>::Make(std::string{ "hello world!" });
				RefCountPtr<MyObj> sp2 = RefCountPtr<MyObj>::Make(std::string{ "hello world!" });
				sp1.Attach(sp2.Detach());
			}

			{
				RefCountPtr<MyObj> sp1{RefCountPtr<MyObj>::Make(std::string{ "hello world!" })};
				sp1->Print();
				(*sp1).Print();
			}

			{
				RefCountPtr<MyObj> sp1{ RefCountPtr<MyObj>::Make(std::string{ "hello world!" }) };
				sp1.Attach(nullptr);
			}

			if (MyObj::CtorCount() == MyObj::DtorCount())
			{
				SML_LOG_LINE(TEXT("test passed, ctor and dtor count is %d"), MyObj::CtorCount());
			}
			else
			{
				SML_LOG_LINE(TEXT("test fail, ctor count is %d but dtor count is %d"), MyObj::CtorCount(), MyObj::DtorCount());
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
				RefCountPtr<CString> spdelfile{RefCountPtr<CString>::MakeWithDispose(delfile, fileName.GetString())};

				auto dispose = [](HANDLE& h)
				{
					::CloseHandle(h);
					h = INVALID_HANDLE_VALUE;
				};
				RefCountPtr<HANDLE> sp1 = RefCountPtr<HANDLE>::MakeWithDispose(dispose, hfile);
				RefCountPtr<HANDLE> sp2 = sp1;
				RefCountPtr<HANDLE> sp3;
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


				const int BUFFER_SIZE = 100;
				RefCountPtr<char*> sp1 = RefCountPtr<char*>::MakeWithDispose(dispose, (char*)::malloc(BUFFER_SIZE));

				strcpy_s(sp1.Ref(), BUFFER_SIZE, "hello world");
				SML_LOG_LINE(TEXT("%S"), sp1.Ref());

			}
		}
	};
}