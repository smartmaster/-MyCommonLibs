#pragma once
#include "SmlList.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include <ctime>
#include <cstdlib>

#include <string>
#include <vector>
#include <list>
#include <iostream>

namespace SmartLib
{
	using namespace ::std;

	class ListTest
	{
	public:

		template<typename C>
		static void Print(const C& cc)
		{
			cout << "Size is " << cc.size() << endl;
			typename C::Enumerator ee{cc};
			while (ee.HasNext())
			{
				cout << ee.Next()->Data << ", ";
			}
			cout << endl;
		}

		template<typename CC>
		static void IterPrint(typename CC& cc)
		{
			for (auto iter = cc.begin(); iter != cc.end(); ++iter)
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


		static void Case19()
		{
			List<string> LL;
			IterPrint(LL);
			IterReversePrint(LL);

			for (long ii = 0; ii < 20; ++ii)
			{
				string str = to_string(ii);
				if (ii & 1)
				{
					LL.push_front(move(str));
				}
				else
				{
					LL.push_front(str);
				}
			}
			IterPrint(LL);
			IterReversePrint(LL);




			while (LL.size())
			{
				string str = LL.pop_front();
				IterReversePrint(LL);
				//IterReversePrint(dq);
			}
		}

		static void Case18()
		{
			List<string> LL;
			IterPrint(LL);
			IterReversePrint(LL);

			for (long ii = 0; ii < 20; ++ii)
			{
				string str = to_string(ii);
				if (ii & 1)
				{
					LL.push_back(move(str));
				}
				else
				{
					LL.push_back(str);
				}
			}
			IterReversePrint(LL);
			IterPrint(LL);



			while (LL.size())
			{
				string str = LL.pop_back();
				IterPrint(LL);
				//IterReversePrint(dq);
			}
		}


		static void Case17()
		{

			int arr[] = { 796, 796, };
			List<int> LL;
			LL.AppendData(arr, _countof(arr), false);
			Print(LL);

			auto less = [](const List<int>::Node* aa, const List<int>::Node* bb)
			{
				return aa->Data > bb->Data;
			};

			auto equal = [](const List<int>::Node* aa, const List<int>::Node* bb)
			{
				return aa->Data == bb->Data;
			};


			LL.Sort(less, equal);
			Print(LL);


			if (LL.VerifyOrder(less, equal))
			{
				cout << "Sort ok" << endl;
			}
			else
			{
				cout << "Sort error!!" << endl;
			}

		}

		static void Case16()
		{

			int arr[] = { 281, 893, 796, 208, 361, 21, 134, 237, 38, 709, 522, 853, 796, 26, 84, 572, 636, };
			List<int> LL;
			LL.AppendData(arr, _countof(arr), false);
			Print(LL);

			auto less = [](const List<int>::Node* aa, const List<int>::Node* bb)
			{
				return aa->Data > bb->Data;
			};

			auto equal = [](const List<int>::Node* aa, const List<int>::Node* bb)
			{
				return aa->Data == bb->Data;
			};


			LL.Sort(less, equal);
			Print(LL);


			if (LL.VerifyOrder(less, equal))
			{
				cout << "Sort ok" << endl;
			}
			else
			{
				cout << "Sort error!!" << endl;
			}

		}

		static void Case15()
		{
			List<int> LL;
			srand(time(nullptr));

			for (long ii = 0; ii < 17; ++ii)
			{
				LL.push_back(rand() % 100);
			}
			Print(LL);


			for (auto iter = LL.begin(); iter != LL.end(); ++ iter)
			{
				cout << *iter << " ";
			}
			cout << endl;
		}

		static void Case14()
		{
			List<int> LL;
			srand(time(nullptr));

			for (long ii = 0; ii < 17; ++ii)
			{
				LL.push_back(rand() % 3);
			}
			Print(LL);


			auto mapper = [](const List<int>::Node* nn)
			{
				return nn->Data % 3;
			};

			List<int> arr[3];
			LL.Partition(LL.First(), LL.Stop(), mapper, arr, _countof(arr));
			Print(LL);


			for (long ii = 0; ii < _countof(arr); ++ ii)
			{
				Print(arr[ii]);
			}
			Print(LL);

			//auto less = [](const List<int>::Node* nn1, const List<int>::Node* nn2)
			//{
			//	return (nn1->Data % 3) < (nn2->Data % 3);
			//};


			//auto equal = [](const List<int>::Node* nn1, const List<int>::Node* nn2)
			//{
			//	return (nn1->Data % 3) == (nn2->Data % 3);
			//};


			//if (LL.VerifyOrder(less, equal))
			//{
			//	cout << "Sort ok" << endl;
			//}
			//else
			//{
			//	cout << "Sort error!!" << endl;
			//}

		}

		static void Case13()
		{
			List<int> LL;
			srand(time(nullptr));

			for (long ii = 0; ii < 17; ++ ii)
			{
				LL.push_back(rand()%1000);
			}
			Print(LL);

			auto less = [](const List<int>::Node* aa, const List<int>::Node* bb)
			{
				return aa->Data > bb->Data;
			};

			auto equal = [](const List<int>::Node* aa, const List<int>::Node* bb)
			{
				return aa->Data == bb->Data;
			};


			LL.Sort(less, equal);
			Print(LL);


			if (LL.VerifyOrder(less, equal))
			{
				cout << "Sort ok" << endl;
			}
			else
			{
				cout << "Sort error!!" << endl;
			}

		}

		static void Case12()
		{
			const long loop_count = 1'000'000L;
			{
				List<long> vec;
				ULONG start = GetTickCount();
				for (long ii = 0; ii < loop_count; ++ii)
				{
					vec.push_back(ii);
				}
				start = GetTickCount() - start;
				//cout << "SmlVector push long took " << (LONG)start << " milsecs" << endl;
				printf("SmlList push long took %lu milsecs - %d ... %d" "\r\n", start, vec.First()->Data, vec.Last()->Data);

				start = GetTickCount();
				while (vec.size())
				{
					vec.PopBackNode();
				}
				start = GetTickCount() - start;
				//cout << "SmlVector pop long took " << start << " milsecs" << endl;
				printf("SmlList pop long took %lu milsecs - %d" "\r\n", start, vec.size());
			}

			{
				list<long> vec;
				ULONG start = GetTickCount();
				for (long ii = 0; ii < loop_count; ++ii)
				{
					vec.push_back(ii);
				}
				start = GetTickCount() - start;
				//cout << "vector push long took " << start << " milsecs" << endl;
				printf("list push long took %lu milsecs - %d ... %d" "\r\n", start, vec.front(), vec.back());

				start = GetTickCount();
				while (vec.size())
				{
					vec.pop_back();
				}
				start = GetTickCount() - start;
				//cout << "vector pop long took " << start << " milsecs" << endl;
				printf("list pop long took %lu milsecs - %d" "\r\n", start, vec.size());
			}



			{
				List<string> vec;
				ULONG start = GetTickCount();
				for (long ii = 0; ii < loop_count; ++ii)
				{
					vec.push_back(to_string(ii));
				}
				start = GetTickCount() - start;
				//cout << "SmlVector push string took " << start << " milsecs" << endl;
				printf("SmlList push string took %lu milsecs - %s ... %s" "\r\n", start, vec.First()->Data.c_str(), vec.Last()->Data.c_str());

				start = GetTickCount();
				while (vec.size())
				{
					vec.PopBackNode();
				}
				start = GetTickCount() - start;
				//cout << "SmlVector pop string took " << start << " milsecs" << endl;
				printf("SmlList pop string took %lu milsecs - %d" "\r\n", start, vec.size());
			}

			{
				list<string> vec;
				ULONG start = GetTickCount();
				for (long ii = 0; ii < loop_count; ++ii)
				{
					vec.push_back(to_string(ii));
				}
				start = GetTickCount() - start;
				//cout << "vector push string took " << start << " milsecs" << endl;
				printf("list push string took %lu milsecs - %s ... %s" "\r\n", start, vec.front().c_str(), vec.back().c_str());

				start = GetTickCount();
				while (vec.size())
				{
					vec.pop_back();
				}
				start = GetTickCount() - start;
				//cout << "vector pop string took " << start << " milsecs" << endl;
				printf("list pop string took %lu milsecs - %d" "\r\n", start, vec.size());
			}
		}


		static void Case11()
		{
			List<string> lst;

			//////////////////////////////////////////////////////////////////////////
			for (long ii = 1; ii <= 3; ++ii)
			{
				lst.push_back(to_string(ii));
			}
			Print(lst);


			//////////////////////////////////////////////////////////////////////////
			for (long ii = 100; ii <= 300; ii += 100)
			{
				lst.InsertAfter(lst.Stop(), to_string(ii));
			}
			Print(lst);
			
			//////////////////////////////////////////////////////////////////////////
			for (long ii = 1000; ii <= 3000; ii += 1000)
			{
				lst.InsertAfter(lst.Last(), to_string(ii));
			}
			Print(lst);
		}

		static void Case10()
		{
			//////////////////////////////////////////////////////////////////////////
			{
				vector<string> vec;
				for (long ii = 0; ii < 10; ++ii)
				{
					vec.push_back(to_string(ii));
				}
				List<string> lst;
				lst.AppendData(&vec[0], vec.size(), false);
				Print(lst);
				cout << endl;
			}
			

			//////////////////////////////////////////////////////////////////////////
			{
				vector<string> vec;
				for (long ii = 0; ii < 10; ++ii)
				{
					vec.push_back(to_string(ii));
				}
				List<string> lst;
				lst.AppendData(vec.begin(), vec.end(), false);
				Print(lst);
				cout << endl;
			}

			//////////////////////////////////////////////////////////////////////////
			{
				vector<string> vec;
				for (long ii = 0; ii < 10; ++ii)
				{
					vec.push_back(to_string(ii));
				}
				List<string> lst;
				lst.AppendData(&vec[0], vec.size(), true);
				Print(lst);
				cout << endl;
			}


			//////////////////////////////////////////////////////////////////////////
			{
				vector<string> vec;
				for (long ii = 0; ii < 10; ++ii)
				{
					vec.push_back(to_string(ii));
				}
				List<string> lst;
				lst.AppendData(vec.begin(), vec.end(), true);
				Print(lst);
				cout << endl;
			}
		}

		static void Case9()
		{
			List<string> lst;

			//////////////////////////////////////////////////////////////////////////
			for (long ii = 0; ii < 10; ++ii)
			{
				lst.push_back(to_string(ii));
			}
			Print(lst);

			List<string> cc1{ lst }; //copy ctor
			cout << "----------------------" << endl;
			Print(cc1);
			//Print(cc2);
			cout << endl;

			List<string> cc2{ move(cc1) }; //move ctor
			cout << "----------------------" << endl;
			Print(cc1);
			Print(cc2);
			cout << endl;

			cc1 = cc2; //copy =
			cout << "----------------------" << endl;
			Print(cc1);
			Print(cc2);
			cout << endl;

			cc2 = move(cc1); //move =
			cout << "----------------------" << endl;
			Print(cc1);
			Print(cc2);
			cout << endl;

		}

		static void Case8()
		{
			List<string> lst;

			//////////////////////////////////////////////////////////////////////////
			for (long ii = 0; ii < 120; ++ii)
			{
				lst.push_back(to_string(ii));
			}

			//////////////////////////////////////////////////////////////////////////
			auto mapper = [](const List<string>::Node* nn)
			{
				return (stol(nn->Data) / 10);
			};

			Vector<List<string>, true, 2> arr;
			arr.resize(10);
			lst.Partition(lst.First(), lst.Stop(), mapper, arr.First(), arr.size());

			List<string>::Enumerator eelst{ lst };
			while (eelst.HasNext())
			{
				cout << eelst.Next()->Data << ", ";
			}
			cout << endl;

			for (long ii = 0; ii < arr.size(); ++ii)
			{
				List<string>::Enumerator ee{ arr[ii] };
				while (ee.HasNext())
				{
					cout << ee.Next()->Data << ", ";
				}
				cout << endl;
			}
		}

		static void Case7()
		{
			List<string> lst;

			//////////////////////////////////////////////////////////////////////////
			for (long ii = 0; ii < 120; ++ii)
			{
				lst.push_back(to_string(ii));
			}

			//////////////////////////////////////////////////////////////////////////
			auto mapper = [](const List<string>::Node* nn)
			{
				return (stol(nn->Data) / 10);
			};

			List<string> arr[10];
			lst.Partition(lst.First(), lst.Stop(), mapper, arr, _countof(arr));

			List<string>::Enumerator eelst{ lst };
			while (eelst.HasNext())
			{
				cout << eelst.Next()->Data << ", ";
			}
			cout << endl;

			for (long ii = 0; ii < _countof(arr); ++ ii)
			{
				List<string>::Enumerator ee{arr[ii]};
				while (ee.HasNext())
				{
					cout << ee.Next()->Data << ", ";
				}
				cout << endl;
			}
		}

		static void Case6()
		{
			List<string> lst;

			//////////////////////////////////////////////////////////////////////////
			for (long ii = 0; ii < 10; ++ ii)
			{
				lst.push_back(to_string(ii));
			}

			//////////////////////////////////////////////////////////////////////////
			List<string>::Enumerator ee{ lst };
			while (ee.HasNext())
			{
				cout << ee.Next()->Data << " ";
			}
			cout << endl;

			//////////////////////////////////////////////////////////////////////////
			List<string>::ReverseEnumerator re{ lst };
			while (re.HasNext())
			{
				cout << re.Next()->Data << " ";
			}
			cout << endl;

			//////////////////////////////////////////////////////////////////////////
			auto odd = [](const List<string>::Node* node)
			{
				return (stol(node->Data) & 1) != 0;
			};

			//////////////////////////////////////////////////////////////////////////
			auto even = [](const  List<string>::Node* node)
			{
				return (stol(node->Data) & 1) == 0;
			};


			//////////////////////////////////////////////////////////////////////////
			lst.DeleteNodes(odd);
			
			ee.Restart();
			while (ee.HasNext())
			{
				cout << ee.Next()->Data << " ";
			}
			cout << endl;

			re.Restart();
			while (re.HasNext())
			{
				cout << re.Next()->Data << " ";
			}
			cout << endl;

			//////////////////////////////////////////////////////////////////////////
			lst.DeleteNodes(even);

			ee.Restart();
			while (ee.HasNext())
			{
				cout << ee.Next()->Data << " ";
			}
			cout << endl;

			re.Restart();
			while (re.HasNext())
			{
				cout << re.Next()->Data << " ";
			}
			cout << endl;
		}


		static void Case5()
		{
			List<string> lst;
			lst.PushFrontNode(new List<string>::Node{ to_string(1000), nullptr, nullptr });
			lst.push_front(to_string(2000));
			{
				string str = to_string(3000);
				lst.push_front(str);
			}

			while (lst.size())
			{
				cout << lst.pop_front() << " ";
			}
			cout << endl;
		}

		static void Case4()
		{
			List<string> lst;
			lst.PushBackNode(new List<string>::Node{ to_string(1000), nullptr, nullptr });
			lst.push_back(to_string(2000));
			{
				string str = to_string(3000);
				lst.push_back(str);
			}

			while (lst.size())
			{
				cout << lst.pop_back() << " ";
			}
			cout << endl;
		}

		static void Case3()
		{
			List<string> lst;
			for (long ii = 1; ii <= 6; ++ii)
			{
				lst.PushFrontNode(new List<string>::Node{ to_string(ii * 1000), nullptr, nullptr });
			}

			while (List<string>::Node* node = lst.PopFrontNode())
			{
				cout << node->Data << " ";
			}
			cout << endl;
		}

		static void Case2()
		{
			List<string> lst;
			for (long ii = 1; ii <= 6; ++ii)
			{
				lst.PushBackNode(new List<string>::Node{ to_string(ii * 1000), nullptr, nullptr });
			}

			while (List<string>::Node* node = lst.PopBackNode())
			{
				cout << node->Data << " ";
			}
			cout << endl;
		}

		static void Case1()
		{
			List<string> lst;
			for (long ii = 1; ii <= 6; ++ii)
			{
				lst.PushFrontNode(new List<string>::Node{ to_string(ii * 1000), nullptr, nullptr });
			}

			while (lst.size())
			{
				List<string>::Node* node = lst.PopFrontNode();
				cout << node->Data << " ";
			}
			cout << endl;
		}

		static void Case0()
		{
			List<string> lst;
			for (long ii = 1; ii <= 6; ++ ii)
			{
				lst.PushBackNode(new List<string>::Node{to_string(ii * 1000), nullptr, nullptr});
			}

			while (lst.size())
			{
				List<string>::Node* node = lst.PopBackNode();
				cout << node->Data << " ";
			}
			cout << endl;
		}
	};
}
