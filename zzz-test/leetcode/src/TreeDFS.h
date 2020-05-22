#pragma once

#include <cstring>
#include <cstdlib>
#include <iostream>

namespace SmartLib::TreeDFS
{
	using namespace ::std;

	struct Node
	{
		int _val{ 0 };
		char* _mem{ nullptr };

		int _childCount{ 0 };
		Node** _child{ nullptr };


		inline static const int MAX_MEM_SIZE = 17;
		inline static const int MAX_CHILD_SIZE = 3;
		inline static const int MAX_TREE_HEIGHT = 7;

		Node()
		{

			_val = rand();
			_mem = new char[_val % MAX_MEM_SIZE + 1];
			_childCount = _val % MAX_CHILD_SIZE + 1;
			_child = new Node * [_childCount];
			memset(_child, 0, _childCount * sizeof(Node*));

		}

		Node(Node& node, bool moveOdd) //deep clone and split even odd numbers
		{
			_val = rand();

			_mem = new char[_val % MAX_MEM_SIZE + 1];
			_childCount = node._childCount;
			_child = new Node * [_childCount];
			memcpy(_child, node._child, _childCount * sizeof(Node*));
			//memset(_child, 0, _childCount * sizeof(Node*));

			if (moveOdd)
			{
				if (_val & 1)
				{
					_val ^= 1; //make an even
				}

				for (int ii = 0; ii < _childCount; ++ii)
				{
					if (_child[ii] && 0 == (_child[ii]->_val & 1)) //remove even, keep odd
					{
						_child[ii] = nullptr;
					}
				}


				for (int ii = 0; ii < node._childCount; ++ii)
				{
					if (node._child[ii] && 0 != (node._child[ii]->_val & 1)) //remove odd, keep even
					{
						node._child[ii] = nullptr;
					}
				}
			}

		}

		//4. delete the tree
		static void Delete(Node* root)
		{
			for (int ii = 0; ii < root->_childCount; ++ii)
			{
				if (root->_child[ii])
				{
					Delete(root->_child[ii]);
					root->_child[ii] = nullptr;
				}
			}

			delete[] root->_mem;
			delete[] root->_child;
			delete root;
		}

		//1. create the tree
		static void BuildTree(Node** root, int height)
		{
			if (height == 0)
			{
				return;
			}

			*root = new Node{};

			for (int ii = 0; ii < (**root)._childCount; ++ii)
			{
				BuildTree(&(**root)._child[ii], height - 1);
			}

		}

		//2. traverse the tree
		static void Traverse(Node* root)
		{
			cout << root->_val << ' ';
			for (int ii = 0; ii < root->_childCount; ++ii)
			{
				if (root->_child[ii])
				{
					Traverse(root->_child[ii]);
				}
			}
		}

		//3.1 add even node between odds
		static void ProcessNode(Node* node)
		{
			if (nullptr == node || 0 == (node->_val & 1))
			{
				return;
			}

			int OddIndex = -1;
			for (int ii = 0; ii < node->_childCount; ++ii)
			{
				if (node->_child[ii] && 0 != (node->_child[ii]->_val & 1))
				{
					OddIndex = ii;
					break;
				}
			}

			if (OddIndex >= 0)
			{
				Node* evenNode = new Node{ *node, true };
				node->_child[OddIndex] = evenNode;
			}
		}

		//3.2 add even node between odds
		static void ProcessNodeDFS(Node* root)
		{
			if (root == nullptr)
			{
				return;
			}

			//ProcessNode(root);


			for (int ii = 0; ii < root->_childCount; ++ii)
			{
				ProcessNodeDFS(root->_child[ii]);
			}

			ProcessNode(root);

		}

		//test cases
		static void Test()
		{
			Node* root = nullptr;
			int height = rand() % MAX_TREE_HEIGHT + 1;
			cout << endl << "Build Tree....." << endl;
			BuildTree(&root, height);

			cout << endl << "Traverse Tree....." << endl;
			Traverse(root);

			cout << endl << "Insert even nodes....." << endl;
			ProcessNodeDFS(root);

			cout << endl << "Traverse Tree....." << endl;
			Traverse(root);

			cout << endl << "Delete Tree....." << endl;
			Delete(root);
			root = nullptr;
		}
	};
}
