#pragma once

#include <cassert>
#include <vector>
#include <iostream>

#include "log.h"

namespace SmartLib
{

	enum class  OpType
	{
		First,
		Add,
		Sub,
		OpDelimeter,
		Mul,
		Div,
		Mod,
		Last,
		LeftBracket,
		RightBracket,
		Data,
		Unknown
	};



	template<typename T>
	struct ExprElement
	{
		OpType _type{ OpType::Unknown };
		
		T _data{ default };

		ExprElement* _left{ nullptr };
		ExprElement* _right{ nullptr };
	};

	class ExpressionTree
	{
	public:
		template<typename T>
		static	ExprElement<T> Calc(const ExprElement<T> & left, const ExprElement<T> & op, const ExprElement<T> & right)
		{
			assert(left._type == OpType::Data);
			assert(op._type > OpType::First && op._type < OpType::Last);
			assert(right._type == OpType::Data);

			T rr;
			switch (op._type)
			{
			case OpType::Add:
				rr = left._data + right._data;
				break;
			case OpType::Sub:
				rr = left._data - right._data;
				break;
			case OpType::Mul:
				rr = left._data * right._data;
				break;
			case OpType::Div:
				rr = left._data / right._data;
				break;
			case OpType::Mod:
				rr = left._data % right._data;
				break;
			default:
				assert(false);
			}

			return ExprElement<T>{OpType::Data, rr};
		}

		template<typename T>
		static ExprElement<T> CalcAdditive(const std::vector<ExprElement<T>> & expr, int first, int last) //only + -
		{
			if (first == last)
			{
				return expr[first];
			}

			assert(last - first >= 2);

			std::vector<ExprElement<T>> stack;
			for (int ii = first; ii <= last; ++ii)
			{
				stack.push_back(expr[ii]);
				if (stack.size() == 3)
				{
					const auto & left = stack[stack.size() - 3];
					const auto & op = stack[stack.size() - 2];
					const auto & right = stack[stack.size() - 1];

					assert(op._type > OpType::First && op._type < OpType::OpDelimeter);

					auto rr = Calc(left, op, right);
					while (stack.size())
					{
						stack.pop_back();
					}
					stack.push_back(rr);
				}
			}

			assert(stack.size() == 1);
			return stack[0];
		}


		template<typename T>
		static ExprElement<T> CalcMultiply(const std::vector<ExprElement<T>> & expr, int first, int last) // + - x / %, but no bracket
		{
			if (first == last)
			{
				return expr[first];
			}

			assert(last - first >= 2);

			std::vector<ExprElement<T>> stack;
			for (int ii = first; ii <= last; ++ii) //process mul/dive/mod
			{
				stack.push_back(expr[ii]);

				if (stack.size() >= 3 && stack.back()._type == OpType::Data)
				{
					const auto & left = stack[stack.size() - 3];
					const auto & op = stack[stack.size() - 2];
					const auto & right = stack[stack.size() - 1];

					if (op._type > OpType::OpDelimeter && op._type < OpType::Last) // if * / %
					{
						auto rr = Calc(left, op, right);
						for (int jj = 0; jj < 3; ++jj)
						{
							stack.pop_back();
						}
						stack.push_back(rr);
					}
				}
			}

			return CalcAdditive(stack, 0, stack.size() - 1);
		}


		template<typename T>
		static ExprElement<T> CalcExpr(const std::vector<ExprElement<T>> & expr, int first, int last) //+-*/%()
		{
			if (first == last)
			{
				return expr[0];
			}

			assert(last - first >= 2);

			std::vector<ExprElement<T>> stack;
			std::vector<int> bracketPos;
			for (int ii = first; ii <= last; ++ii)
			{
				stack.push_back(expr[ii]);
				if (stack.back()._type == OpType::LeftBracket)
				{
					bracketPos.push_back(stack.size() - 1);
				}
				else if (stack.back()._type == OpType::RightBracket)
				{
					int lll = stack.size() - 1;
					int fff = bracketPos.back();
					bracketPos.pop_back();

					assert(fff >= 0);
					auto rr = CalcMultiply(stack, fff + 1, lll - 1);
					for (int kk = 0; kk < lll - fff + 1; ++kk)
					{
						stack.pop_back();
					}
					stack.push_back(rr);
				}
			}


			assert(bracketPos.size() == 0);

			return CalcMultiply(stack, 0, stack.size() - 1);
		}

		static void Parse(const wchar_t * str, std::vector<ExprElement<int>> & expr)
		{
			while (*str)
			{
				switch (*str)
				{
				case L'+':
					if (expr.size() == 0 || expr.back()._type == OpType::LeftBracket)
					{
						expr.push_back(ExprElement<int>{OpType::Data, 0});
					}
					expr.push_back(ExprElement<int>{OpType::Add, 0});
					break;
				case L'-':
					if (expr.size() == 0 || expr.back()._type == OpType::LeftBracket)
					{
						expr.push_back(ExprElement<int>{OpType::Data, 0});
					}
					expr.push_back(ExprElement<int>{OpType::Sub, 0});
					break;
				case L'*':
					expr.push_back(ExprElement<int>{OpType::Mul, 0});
					break;
				case L'/':
					expr.push_back(ExprElement<int>{OpType::Div, 0});
					break;
				case L'%':
					expr.push_back(ExprElement<int>{OpType::Mod, 0});
					break;
				case L'(':
					expr.push_back(ExprElement<int>{OpType::LeftBracket, 0});
					break;
				case L')':
					expr.push_back(ExprElement<int>{OpType::RightBracket, 0});
					break;
				case  L'\t':
				case  L' ':
					break;
				default:
					if (*str >= L'0' && *str <= L'9')
					{
						wchar_t * end = nullptr;
						int data = wcstol(str, &end, 10);
						expr.push_back(ExprElement<int>{OpType::Data, data});
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

		template<class T>
		static void PrintExpr(const std::vector<ExprElement<T>> & expr)
		{
			for (const auto & ee : expr)
			{
				switch (ee._type)
				{
				case OpType::Add:
					std::wcout << L'+';
					break;
				case OpType::Sub:
					std::wcout << L'-';
					break;
				case OpType::Mul:
					std::wcout << L'*';
					break;
				case OpType::Div:
					std::wcout << L'/';
					break;
				case OpType::Mod:
					std::wcout << L'%';
					break;
				case OpType::LeftBracket:
					std::wcout << L'(';
					break;
				case OpType::RightBracket:
					std::wcout << L')';
					break;
				case OpType::Data:
					std::wcout << ee._data;
					break;
				default:
					assert(false);
				}
			}

			std::wcout << std::endl;
		}
	};

	class ExpressionTreeTest
	{
	public:
		static void Case0()
		{
			wchar_t str[] = L"( (3 + 5 * 2) + 3) / 5 + (-6) / 4 * 2 + 3";
			std::vector<ExprElement<int>> expr;
			ExpressionTree::Parse(str, expr);
			ExpressionTree::PrintExpr(expr);
			ExprElement<int> rr = ExpressionTree::CalcExpr(expr, 0, expr.size() - 1);
			LOG_LINE(L"%s = %d", str, rr._data);
		}

	};

}

