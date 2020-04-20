#pragma once

#include <vector>
#include <iostream>

#include "sml-common.h"

namespace SmartLib
{
	using namespace ::std;

	class SegmentArrayAlg
	{
	public:
		struct Node
		{
			long _sum{ 0 };
			Node* _left{ nullptr };
			Node* _right{ nullptr };

			Node(long sum, Node* left = nullptr, Node* right = nullptr) :
				_sum{sum},
				_left{ left },
				_right{ right }
			{
			}
		};

	private:
		const vector<long> _data;

	public:
		SegmentArrayAlg(const vector<long>& data) :
			_data{data}
		{
		}


		Node* Builld(long start, long end)
		{
			if (end - start <= 0)
			{
				return nullptr;
			}

			if (end - start == 1)
			{
				return new Node{_data[start]};
			}

			long mid = (start + end) / 2;

			Node* left = Builld(start, mid);
			Node* right = Builld(mid, end);

			long sum = 0;
			if (left)
			{
				sum += left->_sum;
			}
			if (right)
			{
				sum += right->_sum;
			}

			return new Node{sum, left, right};
		}
	};
}