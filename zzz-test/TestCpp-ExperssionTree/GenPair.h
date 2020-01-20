#pragma once

#include <string>
#include <vector>
#include <iostream>

namespace SmartLib
{
	class GenPair
	{
	private:
		std::vector<std::string> _vec;
	public:
		 void DFS(std::vector<char> & stack, int left, int right, int max)
		{
			if (left > max || right > max)
			{
				return;
			}

			if (left < right)
			{
				return;
			}


			if (right == max)
			{
				_vec.push_back(std::string{ stack.begin(), stack.end() });
				std::cout << _vec.back()  << std::endl;
			}
			else
			{
				if (left == right)
				{
					stack.push_back('(');
					DFS(stack, left + 1, right, max);
					stack.pop_back();
				}
				else if (left > right)
				{
					if (left < max)
					{
						stack.push_back('(');
						DFS(stack, left + 1, right, max);
						stack.pop_back();
					}

					stack.push_back(')');
					DFS(stack, left, right + 1, max);
					stack.pop_back();
				}
			}

			
		}

		const std::vector<std::string> & Run(int max)
		{
			std::vector<char> stack;
			DFS(stack, 0, 0, max);
			return _vec;
		}
	};


	class GenPairTest
	{
	public:
		static void Case0()
		{
			GenPair gp;
			gp.Run(3);
		}
	};
}