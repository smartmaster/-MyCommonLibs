#pragma once
#include "wordBreak.h"
#include "log.h"

namespace SmartLib_WordBreak
{
	using namespace std;

	class SolutionTest
	{
	public:
		static void Case0()
		{
			//string str = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab";
			//string str = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			//vector<string> wordDict = { "a","aa","aaa","aaaa","aaaaa","aaaaaa","aaaaaaa","aaaaaaaa","aaaaaaaaa","aaaaaaaaaa" };


			string str = "Duringthe1990sarmedwithanOlympusStyluscamerahebecamethevisualchroniclerofHollywoodsyoungcelebritiesthelastpreinternetgenerationofmusiciansandactorslikeHilarySwankandCharlizeTheronwhoatthattimewerestilltryingtomakeitinthebusiness";
			vector<string> wordDict = { "During", "the", "1990s", "armed", "with", "an", "Olympus", "Stylus", "camera", "he", "became", "the", "visual", "chronicler", "of", "Hollywoods", "young", "celebrities", "the", "last", "pre", "internet", "generation", "of", "musicians", "and", "actors", "like", "Hilary", "Swank", "and", "Charlize", "Theron", "who", "at", "that", "time", "were", "still", "trying", "to", "make", "it", "in", "the", "business" };


			//string str = "leetcode";
			//vector<string> wordDict = { "code", "leet" };
					   
			SmlWordSplit split{ str, wordDict };
			bool result = split.Run();

			cout << "string: " << str << endl;
			cout << "Dictionary: ";
			for (const auto & word : wordDict)
			{
				cout << word << ",";
			}
			cout << endl;

			SML_LOG_LINE(TEXT("the string can %sbe splited completely, reuse count is %I64d."), result ? TEXT(" ") : TEXT("not "), split.ReuseCount());
			
			if (result)
			{
				vector<string> splitted;
				auto visitor = [&splitted](const string& str)
				{
					if (str.size())
					{
						splitted.push_back(str);
					}
				};


				split.Tree().Traverse(visitor);
				for (const auto & str : splitted)
				{
					cout << str << " ";
				}
			}
			cout << endl;
			return;
		}
	};
}