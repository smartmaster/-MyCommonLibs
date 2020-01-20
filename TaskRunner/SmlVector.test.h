#pragma once
#include "SmlVector.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <cassert>
#include <ctime>
#include <cstdlib>

#include <string>
#include <vector>
#include <iostream>
#include <memory>

namespace SmartLib
{
	using namespace ::std;

	class VectorTest
	{
	public:

		template<typename C>
		static void Print(const C& cc)
		{
			cout << "Size is " << cc.size() << endl;
			typename C::Enumerator ee{ cc };
			while (ee.HasNext())
			{
				cout << ee.Next() << ", ";
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


		

		static void Case16()
		{
			Vector<string, true, 1> vv;

			IterReversePrint(vv);
			IterPrint(vv);

			for (long ii = 0; ii < 20; ++ii)
			{
				string str = to_string(ii);
				if (ii & 1)
				{
					vv.push_back(move(str));
				}
				else
				{
					vv.push_back(str);
				}
			}
			IterReversePrint(vv);
			IterPrint(vv);



			while (vv.size())
			{
				string str = vv.pop_back();
				IterPrint(vv);
				//IterReversePrint(dq);
			}
		}

		static void Case15()
		{
			Vector<string, true, 1> vv;
			for (long ii = 0; ii < 20; ++ii)
			{
				vv.push_back(to_string(ii));
			}
			Print(vv);


			//////////////////////////////////////////////////////////////////////////
			string arr[] = { "100", "200", "300", "400", "500" };
			vv.Insert(5, arr, _countof(arr), true);
			Print(vv);

			vv.Delete(5, _countof(arr));
			Print(vv);
		}

		
		static void Case14()
		{
			Vector<long, false, 1> vv;
			for (long ii = 0; ii < 20; ++ ii)
			{
				vv.push_back(ii);
			}
			Print(vv);


			//////////////////////////////////////////////////////////////////////////
			long arr[] = {100, 200, 300, 400, 500};
			vv.Insert(5, arr, _countof(arr), false);
			Print(vv);

			vv.Delete(5, _countof(arr));
			Print(vv);


		}

		static void Case13()
		{
			Vector<int, false, 1> vv;
			srand(time(nullptr));

			for (long ii = 0; ii < 17; ++ii)
			{
				vv.push_back(rand() % 1000);
			}
			Print(vv);

			for (auto iter = vv.begin(); iter != vv.end(); ++ iter)
			{
				cout << *iter << " ";
			}
			cout << endl;
		}


		static void Case12()
		{
			Vector<int, false, 1> vv;
			srand(time(nullptr));

			for (long ii = 0; ii < 17; ++ii)
			{
				vv.push_back(rand() % 1000);
			}
			Print(vv);

			auto less = [](const int* aa, const int* bb)
			{
				return *aa > *bb;
			};

			auto equal = [](const int* aa, const int* bb)
			{
				return *aa == *bb;
			};


			bool useSort2 = true;
			if (useSort2)
			{
				vv.SortFast(less, equal);
			}
			else
			{
				vv.Sort(less, equal);
			}
			
			Print(vv);


			if (vv.VerifyOrder(less, equal))
			{
				cout << "Sort ok" << endl;
			}
			else
			{
				cout << "Sort error!!" << endl;
			}

		}

		template<long BYTE_SIZE>
		struct PlaceHolder
		{
			char mem[BYTE_SIZE];

			PlaceHolder() = default;
			~PlaceHolder()
			{
				memset(mem, 's', sizeof(mem));
			}
		};

		static void Case11()
		{

			{
				auto p = new PlaceHolder<17>[7];
				delete[] p;
			}

			//////////////////////////////////////////////////////////////////////////
			{
				PlaceHolder<133> ph;
				PlaceHolder<133> arr[17];

				cout << 133 << ", " << 133*17 << endl;
				cout << sizeof(ph) << ", " << sizeof(arr) << endl;
			}



			//////////////////////////////////////////////////////////////////////////
			{
				PlaceHolder<7> ph;
				PlaceHolder<7> arr[17];
				cout << 7 << ", " << 7 * 17 << endl;
				cout << sizeof(ph) << ", " << sizeof(arr) << endl;
			}


			//////////////////////////////////////////////////////////////////////////
			{
				PlaceHolder<5> ph;
				PlaceHolder<5> arr[17];

				cout << 5 << ", " << 5 * 17 << endl;
				cout << sizeof(ph) << ", " << sizeof(arr) << endl;
			}

			//////////////////////////////////////////////////////////////////////////
			{
				PlaceHolder<3> ph;
				PlaceHolder<3> arr[17];

				cout << 3 << 3*17 << endl;
				cout << sizeof(ph) << ", " << sizeof(arr) << endl;
			}


			//////////////////////////////////////////////////////////////////////////
			{
				PlaceHolder<1> ph;
				PlaceHolder<1> arr[17];

				cout << 1 << 1 * 17 << endl;
				cout << sizeof(ph) << ", " << sizeof(arr) << endl;
			}
			
		}


		static void Case10()
		{
			struct MyStruct
			{
				long aa;
				long bb;

				MyStruct(long a, long b) :
					aa{a},
					bb{ b }
				{

				}

				~MyStruct()
				{
					cout << aa << bb << endl;
				}
			};


			MyStruct* pss = (MyStruct*)(new PlaceHolder<sizeof(MyStruct)>[2]);
			new (pss) MyStruct{ 10, 20 };
			new (pss + 1) MyStruct{ 100, 200 };

			delete[] pss; //!!!!!!!not work!!!!!!!!!!
		}


		static void Case9()
		{
			

#if true
			//////////////////////////////////////////////////////////////////////////
			{
				Vector<string, true, 16> vec;
				for (long ii = 1; ii <= 20; ++ii)
				{
					vec.push_back(to_string(ii));
				}


				Vector<string, true, 16> copy{ vec };
				Vector<string, true, 16> moved{ move(vec) };
			}


			{
				Vector<string, true, 16> vec;
				for (long ii = 1; ii <= 20; ++ii)
				{
					vec.push_back(to_string(ii));
				}


				while (vec.size() > 15)
				{
					vec.pop_back();
				}

				Vector<string, true, 16> copy{vec};
				Vector<string, true, 16> moved{ move(copy) };
			}

			
			
			{
				Vector<string, true, 16> vec;
				for (long ii = 1; ii <= 20; ++ii)
				{
					vec.push_back(to_string(ii));
				}


				while (vec.size() > 6)
				{
					vec.pop_back();
				}

				Vector<string, true, 16> copy{ vec };
				Vector<string, true, 16> moved{ move(copy) };
			}
#endif
		}

		static void SetObjCount(void* block, long size) //trick
		{
			//((LONG_PTR*)(block))[-1] = size;
		}

		static void Case8()
		{
			

			//////////////////////////////////////////////////////////////////////////
			struct SomeObj
			{
				string str;
				long index;

				static long Count()
				{
					static long cc = 0;
					return ++cc;
				}

				SomeObj() :
					index{ Count() }
				{
				}
				
				SomeObj(string&& str_) :
					str{ static_cast<string&&>(str_) },
					index{ Count() }
				{
				}


				SomeObj(const string& str_) :
					str{ str_ },
					index{ Count() }
				{
				}

				~SomeObj()
				{
					cout << "destructor " << index << ", " << str << endl;
				}
			};


			//////////////////////////////////////////////////////////////////////////
			{
				char* temp = new char[111];
				delete[] temp;
				temp = nullptr;
			}


			//////////////////////////////////////////////////////////////////////////
			{
				SomeObj* temp = new SomeObj[111];
				delete[] temp;
				temp = nullptr;
			}


			//////////////////////////////////////////////////////////////////////////
			SomeObj* buffer = (SomeObj*)(new PlaceHolder<sizeof(SomeObj)>[2]);
			SetObjCount(buffer, 2); //play trick

			string longstr{ " hello, this is something ....hello, this is something ....hello, this is something ....hello, this is something ....hello, this is something ....hello, this is something ...." };
			new(buffer)SomeObj{ longstr };
			new(buffer + 1)SomeObj{ move(longstr) };
			delete[] buffer; //!!!!!!!not work!!!!!!!!!!!!
			//delete buffer;
			//buffer = nullptr;
			//Case_place_new(17, buffer, 10);
		}

		static void Case7()
		{
			string* buffer = (string*)(new PlaceHolder<sizeof(string)>[2]);
			SetObjCount(buffer, 2); //play trick

			string longstr{ " hello, this is something ....hello, this is something ....hello, this is something ....hello, this is something ....hello, this is something ....hello, this is something ...." };
			new(buffer)string{longstr};
			new(buffer + 1)string{ move(longstr) };
			delete[] buffer; //!!!!!!!not work!!!!!!!!!!!!
			//delete buffer;
			buffer = nullptr;
		}

		static void Case6()
		{
			vector<string> vec;
			for (long ii = 1; ii <= 10; ++ ii)
			{
				string longstr{ " hello, this is something ....hello, this is something ....hello, this is something ....hello, this is something ....hello, this is something ....hello, this is something ...." };
				vec.push_back(to_string(ii) + longstr);
			}

			Case_place_new(97, &vec[0], vec.size());
		}

		template<typename T>
		static void Case_place_new(long newCapacity, T* buffer, long size)
		{
			if (newCapacity & 7) //not integral times of 8
			{
				newCapacity = ((newCapacity + 8) & (~7UL));
			}

			T* newBuffer = (T*)(new PlaceHolder<sizeof(T)>[newCapacity]);
			SetObjCount(newBuffer, newCapacity); //play trick

			for (long ii = 0; ii < size; ++ii)
			{
				new(newBuffer + ii)T{ static_cast<T&&>(buffer[ii]) };
			}

			for (long ii = size; ii < newCapacity; ++ii)
			{
				new(newBuffer + ii)T{};
			}



			delete[] newBuffer; //!!!!!!!not work!!!!!!!!!!!!
		}

		static void Case5()
		{
			const long loop_count = 1'000'000L;
			{
				Vector<long, false, 4096> vec;
				ULONG start = GetTickCount();
				for (long ii = 0; ii < loop_count; ++ii)
				{
					vec.push_back(ii);
				}
				start = GetTickCount() - start;
				//cout << "SmlVector push long took " << (LONG)start << " milsecs" << endl;
				printf("SmlVector push long took %lu milsecs - %d ... %d" "\r\n", start, vec[0], vec[vec.size() - 1]);

				start = GetTickCount();
				while (vec.size())
				{
					vec.pop_back();
				}
				start = GetTickCount() - start;
				//cout << "SmlVector pop long took " << start << " milsecs" << endl;
				printf("SmlVector pop long took %lu milsecs - %d" "\r\n", start, vec.size());
			}

			{
				vector<long> vec;
				ULONG start = GetTickCount();
				for (long ii = 0; ii < loop_count; ++ii)
				{
					vec.push_back(ii);
				}
				start = GetTickCount() - start;
				//cout << "vector push long took " << start << " milsecs" << endl;
				printf("vector push long took %lu milsecs - %d ... %d" "\r\n", start, vec[0], vec[vec.size() - 1]);

				start = GetTickCount();
				while (vec.size())
				{
					vec.pop_back();
				}
				start = GetTickCount() - start;
				//cout << "vector pop long took " << start << " milsecs" << endl;
				printf("vector pop long took %lu milsecs - %d" "\r\n", start, vec.size());
			}



			{
				Vector<string, true, 4096> vec;
				ULONG start = GetTickCount();
				for (long ii = 0; ii < loop_count; ++ii)
				{
					vec.push_back(to_string(ii));
				}
				start = GetTickCount() - start;
				//cout << "SmlVector push string took " << start << " milsecs" << endl;
				printf("SmlVector push string took %lu milsecs - %s ... %s" "\r\n", start, vec[0].c_str(), vec[vec.size() - 1].c_str());

				start = GetTickCount();
				while (vec.size())
				{
					vec.pop_back();
				}
				start = GetTickCount() - start;
				//cout << "SmlVector pop string took " << start << " milsecs" << endl;
				printf("SmlVector pop string took %lu milsecs - %d" "\r\n", start, vec.size());
			}

			{
				vector<string> vec;
				ULONG start = GetTickCount();
				for (long ii = 0; ii < loop_count; ++ii)
				{
					vec.push_back(to_string(ii));
				}
				start = GetTickCount() - start;
				//cout << "vector push string took " << start << " milsecs" << endl;
				printf("vector push string took %lu milsecs - %s ... %s" "\r\n", start, vec[0].c_str(), vec[vec.size() - 1].c_str());

				start = GetTickCount();
				while (vec.size())
				{
					vec.pop_back();
				}
				start = GetTickCount() - start;
				//cout << "vector pop string took " << start << " milsecs" << endl;
				printf("vector pop string took %lu milsecs - %d" "\r\n", start, vec.size());
			}
		}

		static void Case4()
		{
			Vector<string, true, 2> vec;
			vector<string> strs{"fast", "vector" };

			for (long ii = 0; ii < 4; ++ii)
			{
				if (ii & 1) 
				{
					auto temp = strs;
					auto ff = &temp[0];
					auto ss = ff + temp.size();
					vec.Append(ff, ss - ff, true);
					vec.push_back("\r\n");
				}
				else
				{
					auto ff = &strs[0];
					auto ss = ff + strs.size();
					vec.Append(ff, ss - ff, false);
					vec.push_back("\r\n");
				}

			}

			Vector<string, true, 2>::Enumerator ee{vec};
			while (ee.HasNext())
			{
				cout << ee.Next() << " ";
			}
			cout << endl;


			Vector<string, true, 2>::ReverseEnumerator re{ vec };
			while (re.HasNext())
			{
				cout << re.Next() << " ";
			}
			cout << endl;

			return;
		}

		static void Case3()
		{
			Vector<string, true, 2> vec;
			vector<string> strs{"hello", "this", "is", "a", "fast", "vector"};

			for (long ii = 0; ii < 4; ++ ii)
			{
				if (ii & 1)
				{
					auto temp = strs;
					vec.Append(temp.begin(), temp.end(), true);
					vec.push_back("\r\n");
				}
				else
				{
					vec.Append(strs.begin(), strs.end(), false);
					vec.push_back("\r\n");
				}
				
			}

			auto first = vec.First();
			auto stop = vec.Stop();
			while (first != stop)
			{
				cout << *first << " ";
				++first;
			}
			cout << endl;

			return;
		}


		static void Case2()
		{
			Vector<long, false, 16> vec;

			try
			{
				//vec[100] = 100;
			}
			catch (const char* errorfunction)
			{
				cout << "error occurred in function " << errorfunction << endl;
			}
			catch (...)
			{
				cout << "abstract machine corrupted" << endl;
			}
		}

		static void Case1()
		{
			Vector<string, true, 2> vec;
			Case_string(vec);

			Vector<string, true, 2> vec1{ vec };
			Case_string(vec1);

			Vector<string, true, 2> vec2{ move(vec1) };
			Case_string(vec2);

			//////////////////////////////////////////////////////////////////////////
			{
				while (vec.size())
				{
					vec.pop_back();
				}
				
				Vector<string, true, 2> test{ vec };
				Case_string(test);


				Vector<string, true, 2> test1{ move(vec) };
				Case_string(test1);
			}

			//////////////////////////////////////////////////////////////////////////
			{
				while (vec.size())
				{
					vec.pop_back();
				}

				
				for (long ii = 0; ii < 2; ++ ii)
				{
					vec.push_back(to_string((ii + 1) * 1000));
				}

				Vector<string, true, 2> test{ vec };
				Case_string(test);


				Vector<string, true, 2> test1{ move(vec) };
				Case_string(test1);
			}

			//////////////////////////////////////////////////////////////////////////
			{
				while (vec.size())
				{
					vec.pop_back();
				}


				for (long ii = 0; ii < 8; ++ii)
				{
					vec.push_back(to_string((ii + 1) * 1000));
				}

				Vector<string, true, 2> test{ vec };
				Case_string(test);


				Vector<string, true, 2> test1{ move(vec) };
				Case_string(test1);
			}
		}

		static void Case_string(Vector<string, true, 2>& vec)
		{
			for (long ii = 0; ii < 16; ++ii)
			{
				if (ii & 1)
				{
					vec.push_back(to_string(100 * (ii + 1)));
				}
				else
				{
					string str = to_string(100 * (ii + 1));
					vec.push_back(str);
				}
			}

			//{
			//	SmlVector<string, true, 2> vec1{ vec };
			//	SmlVector<string, true, 2> vec2{ move(vec1) };
			//}
						

			long size = vec.size();
			for (long ii = 0; ii < size; ++ii)
			{
				auto data = vec[ii];
				cout << data << ", ";
			}
			cout << endl;

			while (vec.size())
			{
				vec.pop_back();
			}

			auto first = vec.First();
			auto stop = vec.Stop();
			assert(first == stop);

		}

		static void Case0()
		{
			Vector<long, false, 2> vec;
			Case_long(vec);

			Vector<long, false, 2> vec1{ vec };
			Case_long(vec1);

			Vector<long, false, 2> vec2{ move(vec1)};
			Case_long(vec2);


			//////////////////////////////////////////////////////////////////////////
			{
				while (vec.size())
				{
					vec.pop_back();
				}

				Vector<long, false, 2> test{ vec };
				Case_long(test);


				Vector<long, false, 2> test1{ move(vec) };
				Case_long(test1);
			}

			//////////////////////////////////////////////////////////////////////////
			{
				while (vec.size())
				{
					vec.pop_back();
				}


				for (long ii = 0; ii < 2; ++ii)
				{
					vec.push_back((ii + 1) * 1000);
				}

				Vector<long, false, 2> test{ vec };
				Case_long(test);


				Vector<long, false, 2> test1{ move(vec) };
				Case_long(test1);
			}

			//////////////////////////////////////////////////////////////////////////
			{
				while (vec.size())
				{
					vec.pop_back();
				}


				for (long ii = 0; ii < 8; ++ii)
				{
					vec.push_back((ii + 1) * 1000);
				}

				Vector<long, false, 2> test{ vec };
				Case_long(test);


				Vector<long, false, 2> test1{ move(vec) };
				Case_long(test1);
			}
		}

		static void Case_long(Vector<long, false, 2>& vec)
		{
			
			for (long ii = 0; ii < 16; ++ ii)
			{
				if (ii & 1)
				{
					long tt = 100 * (ii + 1);
					vec.push_back(tt);
				}
				else
				{
					vec.push_back(100 * (ii + 1));
				}
				
			}

			//{
			//	SmlVector<long, false, 2> vec1{vec};
			//	SmlVector<long, false, 2> vec2{ move(vec1) };
			//}

			long size = vec.size();
			for (long ii = 0; ii < size; ++ ii)
			{
				auto data = vec[ii];
				cout << data << ", ";
			}
			cout << endl;

			while (vec.size())
			{
				vec.pop_back();
			}


			auto first = vec.First();
			auto stop = vec.Stop();
			assert(first == stop);

		}
	};
}