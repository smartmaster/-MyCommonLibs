#pragma once

#include <vector>
#include <stack>
#include <functional>


//////////////////////////////////////////////////////////////////////////
template<typename data_t>
struct tree_node_t
{
	//////////////////////////////////////////////////////////////////////////
	data_t m_data;
	tree_node_t<data_t> * m_parent;
	size_t m_visited_child_count;
	std::vector<tree_node_t<data_t>*> m_children;

	//////////////////////////////////////////////////////////////////////////
	tree_node_t(const data_t & data) :
		m_data(data),
		m_parent(nullptr),
		m_visited_child_count(0)
	{
	}
};

template<typename data_t>
void add_child(tree_node_t<data_t> * root, tree_node_t<data_t> * child)
{
	if (child)
	{
		child->m_parent = root;
	}
	root->m_children.push_back(child);
}

template<typename data_t>
void free_tree(tree_node_t<data_t> * root)
{
	if (nullptr == root)
	{
		return;
	}

	for (auto & child : root->m_children)
	{
		free_tree(child);
		child = nullptr;
	}
	root->m_children.clear();
	delete root;
}

template<typename data_t>
void traverse_tree_s(tree_node_t<data_t> * root, std::function<void(tree_node_t<data_t>*)> pre_visit, std::function<void(tree_node_t<data_t>*)> mid_visit, std::function<void(tree_node_t<data_t>*)> post_visit)
{
	std::stack<tree_node_t<data_t>*> stck;
	stck.push(root);

	if (pre_visit)
	{
		pre_visit(root);
	}

	while (!stck.empty())
	{
		tree_node_t<data_t> * cur_top = stck.top();

		if (nullptr == cur_top)
		{
			if (mid_visit)
			{
				mid_visit(cur_top);
			}

			if (post_visit)
			{
				post_visit(cur_top);
			}
			stck.pop();
		}
		else if (cur_top->m_visited_child_count == cur_top->m_children.size())
		{
			if (0 == cur_top->m_children.size())
			{
				if (mid_visit)
				{
					mid_visit(cur_top);
				}
			}

			if (post_visit)
			{
				post_visit(cur_top);
			}
			cur_top->m_visited_child_count = 0;
			stck.pop();
		}
		else
		{
			if (cur_top->m_visited_child_count == cur_top->m_children.size() / 2)
			{
				if (mid_visit)
				{
					mid_visit(cur_top);
				}
			}

			tree_node_t<data_t> * new_top = cur_top->m_children[cur_top->m_visited_child_count];
			stck.push(new_top);
			++cur_top->m_visited_child_count;
			if (pre_visit)
			{
				pre_visit(new_top);
			}
		}
	}
}

template<typename data_t>
void traverse_tree_r(tree_node_t<data_t> * root, std::function<void(tree_node_t<data_t>*)> pre_visit, std::function<void(tree_node_t<data_t>*)> mid_visit, std::function<void(tree_node_t<data_t>*)> post_visit)
{
	if (pre_visit)
	{
		pre_visit(root);
	}

	if (root)
	{
		size_t half = root->m_children.size() / 2;
		for (size_t ii = 0; ii < half; ++ii)
		{
			traverse_tree_r(root->m_children[ii], pre_visit, mid_visit, post_visit);
		}
	}

	if (mid_visit)
	{
		mid_visit(root);
	}

	if (root)
	{
		size_t half = root->m_children.size() / 2;
		for (size_t ii = half; ii < root->m_children.size(); ++ii)
		{
			traverse_tree_r(root->m_children[ii], pre_visit, mid_visit, post_visit);
		}
	}

	if (post_visit)
	{
		post_visit(root);
	}
}

#if 0 //unit test
int _tmain_tree(int argc, _TCHAR* argv[])
{
	//////////////////////////////////////////////////////////////////////////
	int data = 1;
	typedef int data_t;
	auto root = new tree_node_t<data_t>(data++);
	for (int ii = 0; ii < 3; ++ii)
	{
		auto child = new tree_node_t<data_t>(data++);
		add_child<data_t>(root, child);
		add_child<data_t>(root, nullptr);

		for (int jj = 0; jj < 5; ++jj)
		{
			auto grand_child = new tree_node_t<data_t>(data++);
			add_child<data_t>(child, nullptr);
			add_child<data_t>(child, grand_child);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	auto pre_visit = [](tree_node_t<data_t> * node)
	{
		if (nullptr == node)
		{
			printf("F,");
		}
		else
		{
			printf("%d,", node->m_data);
		}
	};

	auto mid_visit = [](tree_node_t<data_t> * node)
	{
		if (nullptr == node)
		{
			printf("M,");
		}
		else
		{
			printf("%d,", node->m_data);
		}
	};

	auto post_visit = [](tree_node_t<data_t> * node)
	{
		if (nullptr == node)
		{
			printf("L,");
		}
		else
		{
			printf("%d,", node->m_data);
		}
	};

	//////////////////////////////////////////////////////////////////////////
	printf("pre\r\n");
	printf("s: "); traverse_tree_s<data_t>(root, pre_visit, nullptr, nullptr); printf("\r\n");
	printf("r: "); traverse_tree_r<data_t>(root, pre_visit, nullptr, nullptr); printf("\r\n");
	printf("\r\n"); printf("\r\n");


	//////////////////////////////////////////////////////////////////////////
	printf("pre\r\n");
	printf("s: "); traverse_tree_s<data_t>(root, pre_visit, nullptr, nullptr); printf("\r\n");
	printf("r: "); traverse_tree_r<data_t>(root, pre_visit, nullptr, nullptr); printf("\r\n");
	printf("\r\n"); printf("\r\n");


	//////////////////////////////////////////////////////////////////////////
	printf("mid\r\n");
	printf("s: "); traverse_tree_s<data_t>(root, nullptr, mid_visit, nullptr); printf("\r\n");
	printf("r: "); traverse_tree_r<data_t>(root, nullptr, mid_visit, nullptr); printf("\r\n");
	printf("\r\n"); printf("\r\n");

	//////////////////////////////////////////////////////////////////////////
	printf("mid\r\n");
	printf("s: "); traverse_tree_s<data_t>(root, nullptr, mid_visit, nullptr); printf("\r\n");
	printf("r: "); traverse_tree_r<data_t>(root, nullptr, mid_visit, nullptr); printf("\r\n");
	printf("\r\n"); printf("\r\n");

	//////////////////////////////////////////////////////////////////////////
	printf("post\r\n");
	printf("s: "); traverse_tree_s<data_t>(root, nullptr, nullptr, post_visit); printf("\r\n");
	printf("r: "); traverse_tree_r<data_t>(root, nullptr, nullptr, post_visit); printf("\r\n");
	printf("\r\n"); printf("\r\n");

	//////////////////////////////////////////////////////////////////////////
	printf("post\r\n");
	printf("s: "); traverse_tree_s<data_t>(root, nullptr, nullptr, post_visit); printf("\r\n");
	printf("r: "); traverse_tree_r<data_t>(root, nullptr, nullptr, post_visit); printf("\r\n");
	printf("\r\n"); printf("\r\n");

	//////////////////////////////////////////////////////////////////////////
	free_tree(root);

	return 0;
}
#endif