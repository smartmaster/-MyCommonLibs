#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include "log.h"
#include "ScopeExec.h"

namespace TestCpp
{
	class ISome
	{
	public:
		virtual void Eat() = 0;
		virtual	~ISome() {};
	};


	class Csome : public ISome
	{
	private:
		TCHAR * _data{ nullptr };
		SmartLib::ScopeExec ScopeExit__data{ [this]() 
		{
			if (_data)
			{
				LOG_LINE(TEXT("_data is being cleaned up"));
				delete[] _data;
				_data = nullptr;
			}
		} };


		TCHAR * _data1{ nullptr };
		SmartLib::ScopeExec ScopeExit__data1{ [this]()
		{
			if (_data1)
			{
				LOG_LINE(TEXT("_data1 is being cleaned up"));
				delete[] _data1;
				_data1 = nullptr;
			}
		} };

	public:

		Csome()
		{
			_data = new TCHAR[20];
			_tcscpy_s(_data, 20, TEXT("data"));
			_data1 = new TCHAR[20];
			_tcscpy_s(_data1, 20, TEXT("data1"));
		}
		virtual void Eat() override
		{
			LOG_LINE(TEXT("eat something.. [%s, %s]"), _data, _data1);
		}

		~Csome()
		{
			//delete[] _data1;
			//_data1 = nullptr;

			//delete[] _data;
			//_data = nullptr;
			LOG_LINE(TEXT("destructor called"));
		}
	

	};

	class CsomeTest
	{
	public:
		static void  Case0()
		{
			ISome* ss = new Csome();
			SCOPE_EXEC(ss)(nullptr, 
				[&ss] ()
			{
				if (ss)
				{
					delete ss;
					ss = nullptr;
				}
			});

			ss->Eat();
		}
	};

}