#pragma once

#include "SmlVector.h"
#include "SmlRefPtr.h"

namespace SmartLib
{
	template<typename T>
	class Tree
	{
	private:
		T _data{};
		Vector<RefPtr<Tree>, true, 2> _subTrees;

	public:
		template<typename D>
		void SetData(D&& data)
		{
			_data = static_cast<D&&>(data);
		}

		const T& GetData() const
		{
			return _data;
		}

		long GetSubTreeCount() const
		{
			return _subTrees.size();
		}

		void SetSubTree(RefPtr<Tree> sub, long pos)
		{
			if (pos >= _subTrees.size())
			{
				_subTrees.resize(pos + 1);
			}

			_subTrees[pos] = static_cast<RefPtr<Tree>&&>(sub);
		}

		RefPtr<Tree> GetSubTree(long pos) const
		{
			if (pos >= _subTrees.size())
			{
				return RefPtr<Tree>{};
			}

			return _subTrees[pos];
		}

		static RefPtr<Tree> BuildBinaryTree(T* pre, T* mid, long count)
		{
			if (count <= 0)
			{
				return RefPtr<Tree>{};
			}


			long midPos = -1;
			for (long ii = 0; ii < count; ++ ii)
			{
				if (pre[0] == mid[ii])
				{
					midPos = ii;
					break;
				}
			}

			if (midPos < 0)
			{
				return RefPtr<Tree>{};
			}


			RefPtr<Tree> left = BuildBinaryTree(pre + 1, mid, midPos);
			RefPtr<Tree> right = BuildBinaryTree(pre + 1 + midPos, mid + 1 + midPos, count - midPos - 1);

			RefPtr<Tree> root = RefPtr<Tree>::Make();
			root->SetData(pre[0]);
			root->SetSubTree(left, 0);
			root->SetSubTree(right, 1);
			return root;
		}
	

		//////////////////////////////////////////////////////////////////////////
	private:
		struct TreeFlag
		{
			const Tree* _tree{ nullptr };
			bool _isTree{ false };

			TreeFlag()
			{
			}

			TreeFlag(const Tree* tree, bool isTree) :
				_tree{ tree },
				_isTree{ isTree }
			{
			}
		};

		template<typename VECTOR, typename STACK>
		static void PushSubTreesToStack(const VECTOR& subTrees, long start, long stop, STACK& stack)
		{
			for (long pos = stop - 1; pos >= start; --pos) //in reverse order
			{
				auto tree = subTrees[pos];
				if (tree)
				{
					stack.push_back(TreeFlag{ tree, true });
				}
			}
		}

	public:
		class PreOrderEnumerator
		{
		private:
			const Tree* _tree;
			Vector<TreeFlag, false, 8> _stack;
		
		public:
			PreOrderEnumerator(const Tree* tree) :
				_tree{tree}
			{
				if (_tree)
				{
					_stack.push_back(TreeFlag{ _tree, true });
				}
			}

			void Restart()
			{
				while (_stack.size())
				{
					_stack.pop_back();
				}

				if (_tree)
				{
					_stack.push_back(TreeFlag{ _tree, true });
				}
			}

			bool HasNext()
			{
				if (_stack.size() == 0)
				{
					return false;
				}

				while (_stack.size())
				{
					TreeFlag top = _stack.back();
					if (!top._isTree)
					{
						return true;
					}

					_stack.pop_back();
					PushSubTreesToStack(top._tree->_subTrees, 0, top._tree->_subTrees.size(), _stack);
					_stack.push_back(TreeFlag{top._tree, false});
				}

				return false;
			}

			const T& Next()
			{
				const T& data = _stack.back()._tree->GetData();
				_stack.pop_back();
				return data;
			}
		};


		class PostOrderEnumerator
		{
		private:
			const Tree* _tree;
			Vector<TreeFlag, false, 8> _stack;

		public:

			PostOrderEnumerator(const Tree* tree) :
				_tree{ tree }
			{
				_stack.push_back(TreeFlag{ tree, true });
			}

			void Restart()
			{
				while (_stack.size())
				{
					_stack.pop_back();
				}

				if (_tree)
				{
					_stack.push_back(TreeFlag{ _tree, true });
				}
			}

			bool HasNext()
			{
				if (_stack.size() == 0)
				{
					return false;
				}

				while (_stack.size())
				{
					TreeFlag top = _stack.back();
					if (!top._isTree)
					{
						return true;
					}

					_stack.pop_back();
					_stack.push_back(TreeFlag{ top._tree, false });
					PushSubTreesToStack(top._tree->_subTrees, 0, top._tree->_subTrees.size(), _stack);
				}

				return false;
			}

			const T& Next()
			{
				const T& data = _stack.back()._tree->GetData();
				_stack.pop_back();
				return data;
			}
		};


		class InOrderEnumerator
		{
		private:
			const Tree* _tree;
			Vector<TreeFlag, false, 8> _stack;

	
		public:

			InOrderEnumerator(const Tree* tree) :
				_tree{ tree }
			{
				_stack.push_back(TreeFlag{ tree, true });
			}

			void Restart()
			{
				while (_stack.size())
				{
					_stack.pop_back();
				}

				if (_tree)
				{
					_stack.push_back(TreeFlag{ _tree, true });
				}
			}

			bool HasNext()
			{
				if (_stack.size() == 0)
				{
					return false;
				}

				while (_stack.size())
				{
					TreeFlag top = _stack.back();
					if (!top._isTree)
					{
						return true;
					}
					_stack.pop_back();
					long halflen = top._tree->_subTrees.size() / 2;
					PushSubTreesToStack(top._tree->_subTrees, halflen, top._tree->_subTrees.size(), _stack);
					_stack.push_back(TreeFlag{ top._tree, false });
					PushSubTreesToStack(top._tree->_subTrees, 0, halflen, _stack);
				}

				return false;
			}

			const T& Next()
			{
				const T& data = _stack.back()._tree->GetData();
				_stack.pop_back();
				return data;
			}
		};

	};
}