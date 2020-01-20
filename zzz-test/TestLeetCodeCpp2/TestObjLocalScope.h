#pragma once

#include <iostream>
using namespace std;

namespace SmartLib
{
	class TempObj
	{
	private:
		static int & Index()
		{
			static int index{ 0 };
			return index;
		}

	private:
		int _index{ 0 };

	public:
		TempObj() :
			_index(++Index())
		{
			cout << "Create " << _index << endl;
		}


		~TempObj()
		{
			cout << "Destroy " << _index << endl;
		}

		TempObj(const TempObj & rr)
		{
			_index = ++Index();
			cout << "Copy constructed " << _index << endl;
		}

		void Print(const char * msg)
		{
			cout << msg << _index << endl;
		}
	};

	class TempObjTest
	{
	public:
		static void SubProc(TempObj obj)
		{
			cout << "enter sub proc" << endl;
			obj.Print("In sub proc ");
			cout << "going to exit sub proc" << endl;
		}

		static void Case2()
		{
			cout << "enter main proc" << endl;
			//{
			//TempObj obj;
			SubProc(TempObj{});
			//}
			cout << "going to exit main proc" << endl << endl;
			return;
		}

		static void Case1()
		{
			cout << "enter main proc" << endl;
			//{
				TempObj obj;
				SubProc(obj);
			//}
				cout << "going to exit main proc" << endl << endl;
			return;
		}

		static void Case0()
		{
			cout << "enter main proc" << endl;
			{
				TempObj obj;
				SubProc(obj);
			}
			cout << "going to exit main proc" << endl << endl;
			return;
		}
	};
}