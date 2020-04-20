#pragma once
#include "my-tree.h"


#include <cassert>

namespace SmartLib
{
	using namespace ::std;

	class MyTreeTest
	{
	public:
		static void Case2()
		{
			//vector<int> data{0, 1, 2, -1, -1, 3, 4};


			vector<int> data{ 0, -1, 1, -1, 2, -1, 3, -1, 4, -1, 5, 100, 300, 400, 500, 800, 900 };

			auto root = MyTree::Load(data);
			

			MyTree::Traverse(root, MyTree::Order::IN);
			cout << endl;

			
			auto head = MyTree::ConvertToList(root);
			MyTree::PrintTreeList(head);

		}


		static void Case1()
		{
			//vector<int> data{0, 1, 2, -1, -1, 3, 4};


			vector<int> data{ 0, -1, 1, -1, 2, -1, 3, -1, 4, -1, 5};

			auto root = MyTree::Load(data);
			auto data1 = MyTree::Save(root);


			while (data1.size() && data1.back() == -1)
			{
				data1.pop_back();
			}
			assert(data == data1);

			auto root1 = MyTree::Load(data1);



			MyTree::Traverse(root, MyTree::Order::PRE);
			MyTree::Traverse(root1, MyTree::Order::PRE);
			cout << endl;

			MyTree::Traverse(root, MyTree::Order::IN);
			MyTree::Traverse(root1, MyTree::Order::IN);
			cout << endl;


			MyTree::Traverse(root, MyTree::Order::POST);
			MyTree::Traverse(root1, MyTree::Order::POST);
			cout << endl;

			
		}
	};

}