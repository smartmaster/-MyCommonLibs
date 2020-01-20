#pragma once
#include "SmlVectorList.h"

#include <vector>
#include <string>
#include <ctime>

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

		template<typename CC>
		static void IterPrint(typename CC& cc)
		{
			for (auto iter = cc.begin(); iter != cc.end(); ++ iter)
			{
				cout << *iter << " ";
			}
			cout << endl;
		}

		template<typename CC>
		static void IterReversePrint(typename CC& cc)
		{
			for (auto iter = cc.rbegin(); iter != cc.rend(); --iter)
			{
				cout << *iter << " ";
			}
			cout << endl;
		}


		static void PrintLinkedNode(VectorList<long>& vl, long start)
		{
			long first = start;
			while (start >= 0)
			{
				cout << vl.NodeRef(start).Data << ' ';
				start = vl.NodeRef(start).Next;
				if (start == first)
				{
					break;
				}
			}
			cout << endl;
		}


		static void Case13()
		{
			vector<VectorList<long>::Node> buffer{ 30 };
			VectorList<long> vl{ &buffer[0], (long)buffer.size() };

			srand(time(nullptr));
			//long data = 100;
			while (vl.push_back(rand()%20) >= 0)
			{
			}
			IterPrint(vl);

			auto less = [](/*const*/ VectorList<long>::Node& nn1, /*const*/ VectorList<long>::Node& nn2)
			{
				return nn1.Data < nn2.Data;
			};

			auto equal = [](/*const*/ VectorList<long>::Node& nn1, /*const*/ VectorList<long>::Node& nn2)
			{
				return nn1.Data == nn2.Data;
			};

			vl.Sort(less, equal);
			IterPrint(vl);

			if (vl.VerifyOrder(less, equal))
			{
				cout << "Sotred ok!" << endl;
			}
			else
			{
				cout << "Sotred error!!!" << endl;
			}
		}

		static void Case12()
		{
			vector<VectorList<long>::Node> buffer{ 5 };
			VectorList<long> vl{ &buffer[0], (long)buffer.size() };

			long data = 100;
			while (vl.push_back(data--) >= 0)
			{
			}
			IterPrint(vl);

			auto less = [](/*const*/ VectorList<long>::Node& nn1, /*const*/ VectorList<long>::Node& nn2)
			{
				return nn1.Data < nn2.Data;
			};

			auto equal = [](/*const*/ VectorList<long>::Node& nn1, /*const*/ VectorList<long>::Node& nn2)
			{
				return nn1.Data == nn2.Data;
			};

			vl.Sort(less, equal);
			IterPrint(vl);

			if (vl.VerifyOrder(less, equal))
			{
				cout << "Sotred ok!" << endl;
			}
			else
			{
				cout << "Sotred error!!!" << endl;
			}
		}

		static void Case11()
		{
			vector<VectorList<long>::Node> buffer{ 105 };
			VectorList<long> vl{ &buffer[0], (long)buffer.size() };

			srand(time(nullptr));
			long data = 1;
			while (vl.push_back(data++) >= 0)
			{
			}

			IterPrint(vl);


			auto mapper = [](const VectorList<long>::Node& node)
			{
				return node.Data / 10;
			};

			vector<long> arr;
			arr.resize(10, VectorList<long>::_INVALID_ID);
			vl.Partition(vl.First(), vl.Stop(), mapper, &arr[0], arr.size());

			for (auto start : arr)
			{
				PrintLinkedNode(vl, start);
			}

			IterPrint(vl);
		}

		static void Case10()
		{
			vector<VectorList<long>::Node> buffer{ 105 };
			VectorList<long> vl{ &buffer[0], (long)buffer.size() };

			srand(time(nullptr));
			while (vl.push_back(rand() % 100) >= 0)
			{
			}

			IterPrint(vl);

			
			auto mapper = [](const VectorList<long>::Node& node)
			{
				return node.Data / 10;
			};

			vector<long> arr;
			arr.resize(10, VectorList<long>::_INVALID_ID);
			vl.Partition(vl.First(), vl.Stop(), mapper, &arr[0], arr.size());

			for (auto start : arr)
			{
				PrintLinkedNode(vl, start);
			}

			IterPrint(vl);
		}

		static void Case9()
		{
			vector<VectorList<string>::Node> buffer{ 10 };
			VectorList<string> vl{ &buffer[0], (long)buffer.size() };
			

			vector<string> vecstr;
			for (long ii = 0; ii < 100; ++ii)
			{
				vecstr.push_back(to_string(ii));
			}

			vl.AppendData(&vecstr.front(), (long)vecstr.size(), true);
			IterPrint(vl);

			auto mapper = [](const VectorList<string>::Node& node)
			{
				return stol(node.Data) > 3? 0L : 1L;
			};

			long step = 0;
			long nodeid = vl.FindEqual(vl.First(), vl.Stop(), mapper, 0, step);
			if (nodeid >= 0)
			{
				cout << vl.NodeRef(nodeid).Data << endl;
			}
			
			nodeid = vl.FindNotEqual(vl.First(), vl.Stop(), mapper, 1, step);
			if (nodeid >= 0)
			{
				cout << vl.NodeRef(nodeid).Data << endl;
			}

		}


		static void Case8()
		{
			vector<VectorList<string>::Node> buffer{ 10 };
			VectorList<string> vl{ &buffer[0], (long)buffer.size() };

			vector<string> vecstr;
			for (long ii = 0; ii < 100; ++ii)
			{
				vecstr.push_back(to_string(ii));
			}

			vl.AppendData(&vecstr.front(), (long)vecstr.size(), true);

			IterPrint(vl);

		}

		static void Case7()
		{
			vector<VectorList<string>::Node> buffer{ 10 };
			VectorList<string> vl{ &buffer[0], (long)buffer.size() };

			vector<string> vecstr;
			for (long ii = 0; ii < 100; ++ ii)
			{
				vecstr.push_back(to_string(ii));
			}

			vl.AppendData(vecstr.begin(), vecstr.end(), true);

			IterPrint(vl);

		}

		static void Case6()
		{
			vector<VectorList<long>::Node> buffer{ 10 };
			VectorList<long> vl{ &buffer[0], (long)buffer.size() };

			long data = 1;
			for (;;)
			{
				long nodeid = vl.push_back(data);
				if (nodeid < 0)
				{
					break;
				}
				++data;
			}

			IterReversePrint(vl);
			IterPrint(vl);

			auto even = [](const VectorList<long>::Node& node)
			{
				return 0 == (node.Data & 1);
			};

			auto odd = [](const VectorList<long>::Node& node)
			{
				return 0 != (node.Data & 1);
			};

			vl.DeleteNodes(even);
			IterPrint(vl);

			vl.DeleteNodes(odd);
			IterPrint(vl);
		}

		static void Case5()
		{
			vector<VectorList<string>::Node> buffer{ 10 };
			VectorList<string> vl{ &buffer[0], (long)buffer.size() };

			long data = 1;
			for (;;)
			{
				long nodeid = vl.InsertAfter(vl.Stop(), to_string(data));
				if (nodeid < 0)
				{
					break;
				}
				++data;
			}

			IterReversePrint(vl);
			IterPrint(vl);

			long delId = 0;
			while (vl.size())
			{
				long nextid = vl.DeleteNode(delId);
				IterPrint(vl);
				++delId;
			}
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

			IterPrint(vl);
			IterReversePrint(vl);

			VectorList<string> v2{ &buffer[0], 0, 0}; //attach constructor
			Print(v2);
			ReversePrint(v2);

			IterPrint(v2);
			IterReversePrint(v2);

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