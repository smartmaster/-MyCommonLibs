#include <memory>
#include <vector>
#include <deque>
#include <cstdlib>


namespace SmartLib::TreeBFS
{
	using namespace ::std;


	struct Node
	{
		using SPNODE = shared_ptr<Node>;

		int _val;
		char* _mem;
		vector<SPNODE> _children;

		Node()
		{
			_val = rand();
			_mem = new char[rand() % 128 + 1];
		}

		~Node()
		{
			delete[] _mem;
			_mem = nullptr;
		}


		static vector<SPNODE> CreateNodes(int count)
		{
			vector<SPNODE> vec;
			while (count)
			{
				vec.push_back(make_shared<Node>());
				--count;
			}
			return vec;
		}

		static void AppendNodesToQue(deque<SPNODE>& que, const vector<SPNODE>& nodes)
		{
			for (auto node : nodes)
			{
				que.push_back(node);
			}

		}

		static void AddChild(SPNODE Node, const vector<SPNODE>& nodes)
		{
			auto& childNodes = Node->_children;
			for (auto Node : nodes)
			{
				childNodes.push_back(Node);
			}
		}

		static SPNODE Create(int maxHeight)
		{
			auto root = make_shared<Node>();

			int height = 1;
			deque<SPNODE> que;
			que.push_back(root);

			while (que.size())
			{
				int size = que.size();

				while (size)
				{
					--size;
					auto head = que.front();
					que.pop_front();

					auto childNodes = CreateNodes(rand() % 16 + 1);
					AddChild(head, childNodes);
					AppendNodesToQue(que, childNodes);
				}



				++height;

				if (height > maxHeight)
				{
					break;
				}
			}


			return root;
		}



		static void Case0()
		{
			int height = rand() % 19 + 1;
			auto sp = Create(height);
			return;
		}

	};


	


	
}