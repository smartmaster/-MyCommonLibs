#pragma once
#include "maximum-length-of-a-concatenated-string-with-unique-characters.h"


namespace SmartLib
{
	using namespace  ::std;
	class CombineNMTest
	{

	public:
		static void Case2()
		{
			vector<string> arr{ "cha","r","act","ers"};
			long max = SmartLib::Solution_maximum_length_of_a_concatenated_string_with_unique_characters::maxLength(arr);
			return;
		}

		static void Case1()
		{
			PermNM{ 5, 2, nullptr }.Perm();
		}

		static void Case0()
		{
			CombineNM{ 5, 2, nullptr }.Combine();
		}
	};
}