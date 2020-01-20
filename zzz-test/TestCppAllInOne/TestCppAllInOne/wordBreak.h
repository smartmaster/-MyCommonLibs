#pragma once


#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>

//https://leetcode.com/problems/word-break/

namespace SmartLib_WordBreak
{
	using namespace std;

	template<class T>
	class TreeNode
	{
	public:
		T _data{};
		vector<TreeNode> _subTree;
		

		void Traverse(function<void(const T&)> _func) const
		{
			if (_subTree.size() > 0)
			{
				_subTree[0].Traverse(_func);
			}

			if (_func)
			{
				_func(_data);
			}

			if (_subTree.size() > 1)
			{
				_subTree[1].Traverse(_func);
			}
		}
	};


	class SmlWordSplit
	{
	private:
		const string& _str;				/***INCOMMING***/
		const vector<string>& _dict;	/***INCOMMING***/

		vector<vector<char>> _savedResults;		/***INTERNAL***/
		long long _reuseCount{ 0 };				/***INTERNAL***/

		enum TriBool : char
		{
			TB_NULL = -1,
			TB_FALSE = 0,
			TB_TRUE = 1
		};

		TreeNode<string> _root;

	private:
		bool SplitBySingle(int first, int last, const string& word, TreeNode<string>& node)
		{

			int count = last - first + 1;
			if (count == word.size() && 0 == memcmp(&_str[first], word.c_str(), count))
			{
				node._data = word;
				return true;
			}

			auto pos = _str.find(word, first);
			if (string::npos == pos || (pos + word.size()) > (last + 1))
			{
				return false;
			}

			node._data = word;
			node._subTree.resize(2);

			if (pos != first)
			{
				
				bool b1 = Split(first, pos - 1, node._subTree[0]);
				if (!b1)
				{
					return false;
				}
			}

			if (pos + word.size() <= last)
			{
				bool b2 = Split(pos + word.size(), last, node._subTree[1]);
				if (!b2)
				{
					return false;
				}
			}

			return true;

		}


		bool Split(int first, int last, TreeNode<string>& node)
		{
			if (_savedResults[first][last] != TriBool::TB_NULL)
			{
				++_reuseCount;
				return _savedResults[first][last];
			}


			bool result = false;

			for (const auto& word : _dict)
			{
				result = SplitBySingle(first, last, word, node);
				if (result)
				{
					break;
				}
				node._data.clear();
				node._subTree.clear();
			}

			_savedResults[first][last] = result;
			return result;
		}

	public:

		SmlWordSplit(const string& str, vector<string>& dict) :
			_str(str),
			_dict(dict)
		{
			sort(dict.begin(), dict.end(), [](const string& str1, const string& str2) {return str1.size() > str2.size(); });

			_savedResults.resize(_str.size());
			for (auto& vec : _savedResults)
			{
				vec.resize(_str.size(), TB_NULL);
			}
		}

		bool Run()
		{
			return Split(0, _str.size() - 1, _root);
		}


		long long ReuseCount() const
		{
			return _reuseCount;
		}

		TreeNode<string>& Tree()
		{
			return _root;
		}
	};

	class Solution
	{
	public:

		bool wordBreak(const string& str, vector<string>& wordDict)
		{
			//std::ios::sync_with_stdio(false);
			//cin.tie(nullptr);

			SmlWordSplit split{ str, wordDict };
			return split.Run();
		}
	};
}

