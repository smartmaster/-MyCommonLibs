#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

namespace TestCpp
{
	class TestNullObject
	{
	public:
		void  Func()
		{
			_ftprintf(stdout, TEXT(__FUNCTION__) TEXT("\r\n"));
		}


		virtual void VFunc()
		{
			_ftprintf(stdout, TEXT(__FUNCTION__) TEXT("\r\n"));
		}
	};



	class TestNullObjectCases
	{
	public:
		static void Case0()
		{

			TestNullObject * pobj = nullptr;
			pobj->Func();
			pobj->VFunc();
			
		}
	};
}