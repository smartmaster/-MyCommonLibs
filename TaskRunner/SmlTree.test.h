#pragma once

#include <string>
#include <iostream>

#include "SmlTree.h"

namespace SmartLib
{
	using namespace ::std;

	class TreeTest
	{
	public:

		static void Case1()
		{
			string pre[] = { "3", "9", "20", "15", "7" };
			string mid[] = { "9", "3", "15", "20", "7" };

			RefPtr<Tree<string>> root = Tree<string>::BuildBinaryTree(pre, mid, _countof(pre));

			{
				Tree<string>::PreOrderEnumerator ee{ root };
				while (ee.HasNext())
				{
					cout << ee.Next() << ", ";
				}
				cout << endl;
			}


			{
				Tree<string>::PostOrderEnumerator ee{ root };
				while (ee.HasNext())
				{
					cout << ee.Next() << ", ";
				}
				cout << endl;
			}


			{
				Tree<string>::InOrderEnumerator ee{ root };
				while (ee.HasNext())
				{
					cout << ee.Next() << ", ";
				}
				cout << endl;
			}

		}



		static void Case0()
		{
			RefPtr<Tree<string>> root = RefPtr<Tree<string>>::Make();
			root->SetData(to_string(100));
			for (long ii = 1; ii <= 5; ++ ii)
			{
				auto sub = RefPtr<Tree<string>>::Make();
				sub->SetData(to_string(ii * 1000));
				root->SetSubTree(sub, ii);
			}

			{
				Tree<string>::PreOrderEnumerator ee{ root };
				while (ee.HasNext())
				{
					cout << ee.Next() << ", ";
				}
				cout << endl;
			}


			{
				Tree<string>::PostOrderEnumerator ee{ root };
				while (ee.HasNext())
				{
					cout << ee.Next() << ", ";
				}
				cout << endl;
			}


			{
				Tree<string>::InOrderEnumerator ee{ root };
				while (ee.HasNext())
				{
					cout << ee.Next() << ", ";
				}
				cout << endl;
			}

		}
		
	};
}