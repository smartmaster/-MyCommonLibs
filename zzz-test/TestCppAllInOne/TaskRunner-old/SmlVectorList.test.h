#pragma once
#include "SmlVectorList.h"

#include <vector>
#include <string>

#include <iostream>

namespace SmartLib
{
	using namespace ::std;
	class VectorListTest
	{
	public:

		template<typename C>
		static void Print(const C& cc)
		{
			cout << "Size is " << cc.size() << endl;
			typename C::Enumerator ee{ cc };
			while (ee.HasNext())
			{
				cout << ee.Next().Data << ", ";
			}
			cout << endl;
		}


		template<typename C>
		static void ReversePrint(const C& cc)
		{
			cout << "Size is " << cc.size() << endl;
			typename C::ReverseEnumerator ee{ cc };
			while (ee.HasNext())
			{
				cout << ee.Next().Data << ", ";
			}
			cout << endl;
		}

		static void Case4()
		{
			vector<VectorList<string>::Node> buffer(10);
			VectorList<string> vl{ &buffer[0], (long)buffer.size() };
			//////////////////////////////////////////////////////////////////////////
			long data = 1;
			while (vl.push_front(to_string(data)) >= 0)
			{
				++data;
				if (data > 5)
				{
					break;
				}
			}
			Print(vl);
			ReversePrint(vl);

			VectorList<string> v2{ &buffer[0], 0, 0}; //attach constructor
			Print(vl);
			ReversePrint(vl);

		}

		static void Case3()
		{
			vector<VectorList<string>::Node> buffer(10);
			VectorList<string> vl{ &buffer[0], (long)buffer.size() };


			//////////////////////////////////////////////////////////////////////////
			long data = 1;
			while (vl.push_front(to_string(data)) >= 0)
			{
				++data;
			}
			Print(vl);
			ReversePrint(vl);

			for (;;)
			{
				string data;
				if (vl.size() <= 0)
				{
					break;
				}
				else
				{
					data = vl.pop_back();
					cout << data << ", ";
				}
			}
			cout << endl;
			Print(vl);
			ReversePrint(vl);


			//////////////////////////////////////////////////////////////////////////
			data = 1;
			while (vl.push_back(to_string(data)) >= 0)
			{
				++data;
			}
			Print(vl);
			ReversePrint(vl);

			for (;;)
			{
				string data;
				if (vl.size() <= 0)
				{
					break;
				}
				else
				{
					data = vl.pop_front();
					cout << data << ", ";
				}
			}
			cout << endl;
			Print(vl);
			ReversePrint(vl);
		}

		static void Case2()
		{
			vector<VectorList<string>::Node> buffer(10);
			VectorList<string> vl{ &buffer[0], (long)buffer.size() };

			//////////////////////////////////////////////////////////////////////////
			long data = 1;
			while (vl.push_front(to_string(data)) >= 0)
			{
				++data;
			}
			Print(vl);
			ReversePrint(vl);

			for (;;)
			{
				string data;
				if (vl.size() <= 0)
				{
					break;
				}
				else
				{
					data = vl.pop_back();
					cout << data << ", ";
				}
			}
			cout << endl;
			Print(vl);
			ReversePrint(vl);

			//////////////////////////////////////////////////////////////////////////
			data = 1;
			while (vl.push_back(to_string(data)) >= 0)
			{
				++data;
			}
			Print(vl);
			ReversePrint(vl);

			for (;;)
			{
				string data;
				if (vl.size() <= 0)
				{
					break;
				}
				else
				{
					data = vl.pop_front();
					cout << data << ", ";
				}
			}
			cout << endl;
			Print(vl);
			ReversePrint(vl);
		}


		static void Case1()
		{
			vector<VectorList<string>::Node> buffer(10);
			VectorList<string> vl{ &buffer[0], (long)buffer.size() };

			//////////////////////////////////////////////////////////////////////////
			long data = 1;
			while (vl.push_front(to_string(data)) >= 0)
			{
				++data;
			}
			Print(vl);
			ReversePrint(vl);

			for (;;)
			{
				string data;
				if (vl.size() <= 0)
				{
					break;
				}
				else
				{
					data = vl.pop_front();
					cout << data << ", ";
				}
			}
			cout << endl;
			Print(vl);
			ReversePrint(vl);

			//////////////////////////////////////////////////////////////////////////
			data = 1;
			while (vl.push_back(to_string(data)) >= 0)
			{
				++data;
			}
			Print(vl);
			ReversePrint(vl);

			for (;;)
			{
				string data;
				if (vl.size() <= 0)
				{
					break;
				}
				else
				{
					data = vl.pop_back();
					cout << data << ", ";
				}
			}
			cout << endl;
			Print(vl);
			ReversePrint(vl);
		}


		static void Case0()
		{
			VectorList<string>::Node buffer[10];
			VectorList<string> vl{&buffer[0], _countof(buffer)};


			//////////////////////////////////////////////////////////////////////////
			long data = 1;
			while (vl.push_back(to_string(data)) >= 0)
			{
				++data;
			}
			Print(vl);
			ReversePrint(vl);

			for (;;)
			{
				string data;
				if (vl.size() <= 0)
				{
					break;
				}
				else
				{
					data = vl.pop_back();
					cout << data << ", ";
				}
			}
			cout << endl;
			Print(vl);
			ReversePrint(vl);


			//////////////////////////////////////////////////////////////////////////
			data = 1;
			while (vl.push_front(to_string(data)) >= 0)
			{
				++data;
			}
			Print(vl);
			ReversePrint(vl);

			for (;;)
			{
				string data;
				if (vl.size() <= 0)
				{
					break;
				}
				else
				{
					data = vl.pop_front();
					cout << data << ", ";
				}
			}
			cout << endl;
			Print(vl);
			ReversePrint(vl);
		}
	};
}