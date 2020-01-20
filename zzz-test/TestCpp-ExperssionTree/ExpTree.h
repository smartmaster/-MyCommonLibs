#pragma once


#include <cassert>
#include <vector>
#include <string>
#include <functional>
#include <iostream>

#include "log.h"

namespace SmartLib
{
	enum class ExpNodeType
	{
		Unknown,
		Operator,
		Number,
		Tree,
	};

	enum class ExpOpPriority
	{
		Add = 1000,
		Mul = 2000,
		Bracket = 3000,
	};


	struct ExpNode
	{
	public:
		ExpNodeType _type{ ExpNodeType::Unknown };
		long _data{ 0 };
		//char _ch{ 0 }; //debug purpose only
		ExpNode * _left{ nullptr };
		ExpNode * _right{ nullptr };

	public:

		ExpNode()
		{

		}

		ExpNode(char ch) :
			_type(ExpNodeType::Operator),
			_data(ch) //,
			//_ch(ch)
		{
		}


		ExpNode(long num) :
			_type(ExpNodeType::Number),
			_data(num)
		{
		}

		const std::string & ToString(std::string & str) const
		{
			str.clear();
			if (IsOperator() || IsTree())
			{
				str += (char)(_data);
			}
			else
			{
				assert(IsNumber());
				char buffer[128] = { 0 };
				sprintf_s(buffer, "%ld", _data);
				str = buffer;
			}

			return str;
		}

		bool IsOperator() const
		{
			return _type == ExpNodeType::Operator;
		}


		bool IsNumber() const
		{
			return _type == ExpNodeType::Number;
		}


		bool IsTree() const
		{
			return _type == ExpNodeType::Tree;
		}

		bool IsBracket()
		{
			return
				_type == ExpNodeType::Operator &&
				(_data == '(' || _data == ')');
		}


		bool IsLeftBracket()
		{
			return
				_type == ExpNodeType::Operator &&
				(_data == '(');
		}


		bool IsRightBracket()
		{
			return
				_type == ExpNodeType::Operator &&
				(_data == ')');
		}




		ExpOpPriority Priority() const
		{
			assert(_type == ExpNodeType::Operator);

			switch (_data)
			{
			case '+':
			case '-':
				return ExpOpPriority::Add;
				break;
			case '*':
			case '/':
			case '%':
				return ExpOpPriority::Mul;
				break;
			case '(':
			case ')':
				return ExpOpPriority::Bracket;
				break;
			default:
				assert(false);
			}
		}

		static ExpNode Calc(const ExpNode & left, const ExpNode & op, const ExpNode & right)
		{
			assert(left.IsNumber());
			assert(op.IsOperator() || op.IsTree());
			assert(right.IsNumber());

			long rr = Calc(left._data, op._data, right._data);
			return ExpNode(rr);
		}


		static long Calc(long left, char op, long right)
		{
			long rr = -1;
			switch (op)
			{
			case '+':
				rr = left + right;
				break;
			case '-':
				rr = left - right;
				break;
			case '*':
				rr = left * right;
				break;
			case '/':
				rr = left / right;
				break;
			case '%':
				rr = left % right;
				break;
			default:
				assert(false);
			}

			return rr;
		}
	};


	class  ExpTree
	{

	public:
		typedef std::vector<ExpNode*> EXP_VECTOR;

		static ExpNode* BuildPriotityAdd(const EXP_VECTOR & exp, int first, int last) //only +-
		{
			if (first == last)
			{
				return exp[first];
			}

			assert(last - first >= 2);

			EXP_VECTOR stack;
			for (int nn = first; nn <= last; ++nn)
			{
				auto node = exp[nn];
				stack.push_back(node);
				int size = stack.size();
				if (size == 3)
				{
					auto left = stack[size - 3];
					auto middle = stack[size - 2];
					auto right = stack[size - 1];

					assert(left->IsNumber() || left->IsTree());
					assert(middle->Priority() == ExpOpPriority::Add);
					assert(right->IsNumber() || right->IsTree());

					middle->_left = left;
					middle->_right = right;
					middle->_type = ExpNodeType::Tree;

					for (int ii = 0; ii < 3; ++ii)
					{
						stack.pop_back();
					}
					stack.push_back(middle);
				}
			}

			assert(stack.size() == 1);
			return stack[0];
		}


		static ExpNode * BuildPriotityAddMul(const EXP_VECTOR & exp, int first, int last) //having +-*/%, but no ()
		{
			if (first == last)
			{
				return exp[first];
			}

			assert(last - first >= 2);
			EXP_VECTOR stack;
			for (int nn = first; nn <= last; ++nn)
			{
				auto node = exp[nn];
				stack.push_back(node);
				int size = stack.size();
				if (size >= 3)
				{
					auto left = stack[size - 3];
					auto middle = stack[size - 2];
					auto right = stack[size - 1];

					if (middle->IsOperator() && middle->Priority() == ExpOpPriority::Mul)
					{
						assert(left->IsNumber() || left->IsTree());
						//assert(middle->Priority() == ExpOpPriority::_2000);
						assert(right->IsNumber() || right->IsTree());

						middle->_left = left;
						middle->_right = right;
						middle->_type = ExpNodeType::Tree;

						for (int ii = 0; ii < 3; ++ii)
						{
							stack.pop_back();
						}
						stack.push_back(middle);
					}
				}
			}

			return BuildPriotityAdd(stack, 0, stack.size() - 1);
		}


		static ExpNode* BuildPriotityAddMulBracket(const EXP_VECTOR & exp, int first, int last)
		{
			if (first == last)
			{
				return exp[first];
			}

			EXP_VECTOR stack;
			std::vector<int> bracketsPos;
			for (int nn = first; nn <= last; ++nn)
			{
				auto node = exp[nn];
				stack.push_back(node);
				if (node->IsLeftBracket())
				{
					bracketsPos.push_back(stack.size() - 1); // save position of '(' 
				}
				else if (node->IsRightBracket())
				{
					int lll = stack.size() - 1; //position of ')' matching the one previously saved
					int fff = bracketsPos.back();
					bracketsPos.pop_back();

					ExpNode * tree = BuildPriotityAddMul(stack, fff + 1, lll - 1);
					for (int ii = 0; ii < lll - fff + 1; ++ii)
					{
						stack.pop_back();
					}

					stack.push_back(tree);
				}
			}

			assert(bracketsPos.size() == 0);
			return BuildPriotityAddMul(stack, 0, stack.size() - 1);
		}

		static const std::string & ExpTreeToString(const ExpNode * root, std::string & str)
		{
			auto pre = [&str](const ExpNode * node)
			{
				if (node->IsTree())
				{
					str += '(';
				}
				else
				{
					assert(node->IsNumber());
				}
			};

			auto middle = [&str](const ExpNode* node)
			{
				if (node->IsTree())
				{
					str += (char)(node->_data);
				}
				else
				{
					assert(node->IsNumber());
					char buffer[128] = { 0 };
					sprintf_s(buffer, "%ld", node->_data);
					str += buffer;
				}
			};

			auto post = [&str](const  ExpNode * node)
			{
				if (node->IsTree())
				{
					str += ')';
				}
				else
				{
					assert(node->IsNumber());
				}
			};


			Traverse(root, pre, middle, post);


			return str;
		}

		static long ExpTreeCalc(const ExpNode * root)
		{
			std::vector<long> stack;
			auto post = [&stack](const ExpNode * node)
			{
				if (node->IsNumber())
				{
					stack.push_back(node->_data);
				}
				else
				{
					assert(node->IsTree());
					int size = stack.size();
					long rr = ExpNode::Calc(stack[size - 2], node->_data, stack[size - 1]);
					for (int ii = 0; ii < 2; ++ ii)
					{
						stack.pop_back();
					}
					stack.push_back(rr);
				}
			};

			Traverse(root, nullptr, nullptr, post);

			assert(stack.size() == 1);
			return stack[0];
		}

		static void Traverse(const ExpNode * root,
			std::function<void(const ExpNode*)> pre,
			std::function<void(const ExpNode*)> middle,
			std::function<void(const ExpNode*)> post)
		{
			if (root == nullptr)
			{
				return;
			}


			if (pre)
			{
				pre(root);
			}

			Traverse(root->_left, pre, middle, post);


			if (middle)
			{
				middle(root);
			}

			Traverse(root->_right, pre, middle, post);

			if (post)
			{
				post(root);
			}
		}



		static void Parse(const char * str, EXP_VECTOR & expr)
		{
			while (*str)
			{
				switch (*str)
				{
				case '+':
					if (expr.size() == 0 || expr.back()->IsLeftBracket())
					{
						expr.push_back(new ExpNode(0L));
					}
					expr.push_back(new ExpNode('+'));
					break;
				case '-':
					if (expr.size() == 0 || expr.back()->IsLeftBracket())
					{
						expr.push_back(new ExpNode(0L));
					}
					expr.push_back(new ExpNode('-'));
					break;
				case '*':
					expr.push_back(new ExpNode('*'));
					break;
				case '/':
					expr.push_back(new ExpNode('/'));
					break;
				case '%':
					expr.push_back(new ExpNode('%'));
					break;
				case '(':
					expr.push_back(new ExpNode('('));
					break;
				case ')':
					expr.push_back(new ExpNode(')'));
					break;
				case '\t':
				case ' ':
					break;
				default:
					if (*str >= '0' && *str <= '9')
					{
						char * end = nullptr;
						long data = strtol(str, &end, 10);
						expr.push_back(new ExpNode(data));
						str = end - 1;
					}
					else
					{
						assert(false);
					}
				}

				++str;
			}
		}
	};


	class ExpTreeTest
	{
	public:
		static void Case0()
		{
			char str[] = "( (3 + 5 * 2) + 3) / 5 + (-6) / 4 * 2 + 3";
			//char str[] = "1 + 2 + 3 * 4";
			//char str[] = "1 + 2";
			//char str[] = "1 * 2";
			//char str[] = "(1 + 2 )";
			ExpTree::EXP_VECTOR expr;
			ExpTree::Parse(str, expr);
			ExpNode* rr = ExpTree::BuildPriotityAddMulBracket(expr, 0, expr.size() - 1);

			auto pre = [](const ExpNode * node)
			{
				if (node->IsTree())
				{
					std::cout << '(';
				}

			};

			std::string tempStr;
			auto middle = [&tempStr](const ExpNode * node)
			{
				std::cout << node->ToString(tempStr);
			};

			auto post = [](const ExpNode* node)
			{

				if (node->IsTree())
				{
					std::cout << ')';
				}
			};


			ExpTree::Traverse(rr, pre, middle, post);
			std::cout << std::endl;


			std::vector<long> exp;
			auto calc = [&exp](const ExpNode * node)
			{
				if (node->IsNumber())
				{
					exp.push_back(node->_data);
				}
				else
				{
					assert(node->IsTree());
					int size = exp.size();
					ExpNode rr = ExpNode::Calc(ExpNode(exp[size - 2]), *node, ExpNode(exp[size - 1]));
					for (int ii = 0; ii < 2; ++ii)
					{
						exp.pop_back();
					}
					exp.push_back(rr._data);
				}
			};


			ExpTree::Traverse(rr, nullptr, nullptr, calc);
			assert(exp.size() == 1);
			LOG_LINE_A("%s = %ld", str, exp[0]);


		}


		static void Case2()
		{
			char str[] = "( (3 + 5 * 2) + 3) / 5 + (-6) / 4 * 2 + 3";

			ExpTree::EXP_VECTOR exp;
			ExpTree::Parse(str, exp);
			ExpNode * root = ExpTree::BuildPriotityAddMulBracket(exp, 0, exp.size() - 1);
			long rr = ExpTree::ExpTreeCalc(root);
			LOG_LINE_A("%s = %ld", str, rr);
			
			std::string str2;
			ExpTree::ExpTreeToString(root, str2);
			ExpTree::EXP_VECTOR exp2;
			ExpTree::Parse(str2.c_str(), exp2);
			ExpNode * root2 = ExpTree::BuildPriotityAddMulBracket(exp2, 0, exp2.size() - 1);
			long rr2 = ExpTree::ExpTreeCalc(root2);
			LOG_LINE_A("%s = %ld", str2.c_str(), rr2);

		}

	};
}