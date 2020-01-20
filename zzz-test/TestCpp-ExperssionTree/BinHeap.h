#pragma once

#include <vector>

namespace SmartLib
{
	class BinHeap
	{
	private:
		std::vector<int> _heap;
		int _size{ 0 };

	private:

		void Swap(int index1, int index2)
		{
			if (index1 != index2)
			{
				int temp = _heap[index1];
				_heap[index1] = _heap[index2];
				_heap[index2] = temp;
			}
			
		}

		void Up(int index)
		{
			int parent = -1;
			while (index > 0)
			{
				 parent = ((index - 1) << 1);
				 if (_heap[index] > _heap[parent])
				 {
					 Swap(index, parent);
				 }
				 else
				 {
					 break;
				 }
				 index = parent;
			}
		}


		void Down(int lastIndex)
		{
			if (lastIndex == 0)
			{
				return;
			}


			Swap(0, lastIndex);

			int index = 0;
			int left = (index << 1) + 1;
			int right = left + 1;
			while(index < lastIndex)
			{
				int down = -1;
				if (left < lastIndex && right < lastIndex) // having left and right child
				{
					if (_heap[index] >= _heap[left] && _heap[index] >= _heap[right])
					{
						break; //ok
					}

					down = _heap[left] > _heap[right] ? left : right;
					
				}
				else if (left < lastIndex) //having left child only
				{
					if (_heap[index] >= _heap[left])
					{
						break; //ok
					}


					down = left;
				}
				else
				{
					break;
				}

				Swap(index, down);
				index = down;
				left = (index << 1) + 1;
				right = left + 1;
			}
		}
	public:
		void Add(int val)
		{
			_heap.push_back(val);
			Up(_heap.size() - 1);
		}


		int Remove()
		{
			int result = _heap[0];
			Swap(0, _heap.size() - 1);
			_heap.pop_back();
			Down(_heap.size() - 1);
			return result;
		}

		int Size()
		{
			return _heap.size();
		}
	};
}