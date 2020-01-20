#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <list>
#include <string>

#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <iostream>

namespace SmartLib
{
	using namespace std;


	class TestUnique
	{
	public:
		static void Case0()
		{
			list<string> lst;
			srand(time(nullptr));
			for (long ii = 0; ii < 20; ++ ii)
			{
				lst.push_back(to_string(rand() % 10));
			}

			for (const auto& str : lst)
			{
				cout << str << " ";
			}
			cout << endl;


			lst.sort();
			lst.unique();

			for (const auto& str : lst)
			{
				cout << str << " ";
			}
			cout << endl;


		}

	};

	class TestUsing : vector<int>
	{
		vector<int> pp;
	public:
		using vector<int>::push_back;
		//using vector<int>::*;
		//using pp::pop_back;

		void Print()
		{
			for (auto data : *this)
			{
				cout << data << " ";
			}
			cout << endl;
		}

	public:
		static void Case0()
		{
			TestUsing tu;
			tu.push_back(100);
			tu.push_back(200);
			tu.Print();
		}
	};


	class STdSort
	{
	public:
		static void Case0()
		{
			vector<long> pp;
			srand(time(nullptr));
			for (long ii = 0; ii < 1000; ++ ii)
			{
				pp.push_back(rand()%1000);
			}

			::std::sort(pp.begin(), pp.end());

			for (const long data : pp)
			{
				cout << data << " ";
			}
			cout << endl;
		}
	};

	class CMiscSmall
	{
	public:
		static HRESULT ExitOnWin32Error(DWORD e, HRESULT& x)
		{
			if (ERROR_SUCCESS != e) 
			{ 
				x = HRESULT_FROM_WIN32(e); 
				if (!FAILED(x)) 
				{ 
					x = E_FAIL; 
				} 
				//Dutil_RootFailure(__FILE__, __LINE__, x); 
				//ExitTrace(x, s, __VA_ARGS__); 
				//goto LExit; 
			}

			return x;
		}

		static void Case0()
		{

			HRESULT hr = S_OK;
			LONG lval = ExitOnWin32Error(3010, hr);

		}

		static void AdjustTaskOrder()
		{
			list<string> tasks;
			int count = 0;
			for (int ii = 0; ii < 3; ++ii)
			{
				tasks.push_back("aaa" + to_string(count++));
			}

			tasks.push_back("Create Recovery Point");

			for (int ii = 0; ii < 3; ++ii)
			{
				tasks.push_back("bbb" + to_string(count++));
			}

			tasks.push_back("Manifest");
			tasks.push_back("Manifest 1");
			tasks.push_back("Manifest 2");
			tasks.push_back("Manifest 3");

			for (int ii = 0; ii < 3; ++ii)
			{
				tasks.push_back("ccc" + to_string(count++));
			}

			auto iterCreateRecoveryPoint = tasks.end();
			int indexCreateRecoveryPoint = -1;
			auto iterSyncManifest = tasks.end();
			int indexSyncManifest = -1;


			int index = -1;

			for (auto iter = tasks.begin(); iter != tasks.end(); ++iter)
			{
				++index;

				if ((*iter) == "Create Recovery Point")
				{
					indexCreateRecoveryPoint = index;
					iterCreateRecoveryPoint = iter;
				}


				if ((*iter) == "Manifest" || 0 == (*iter).find("Manifest ")) //<sonmi01>2019-11-26 #multiple manifest issues
				{
					indexSyncManifest = index;
					iterSyncManifest = iter;
				}
			}


			if (indexCreateRecoveryPoint >= 0 && indexSyncManifest >= 0 && indexCreateRecoveryPoint < indexSyncManifest)
			{
				auto taskCreateRecoveryPoint = *iterCreateRecoveryPoint;
				++iterSyncManifest;
				tasks.erase(iterCreateRecoveryPoint);
				tasks.insert(iterSyncManifest, taskCreateRecoveryPoint);
			}
		}
	};


	
}