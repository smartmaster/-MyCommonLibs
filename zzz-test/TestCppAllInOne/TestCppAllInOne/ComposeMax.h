#pragma once

//give some numbers, find the max number after linking them all
//for example
// 12, 122 --> 12212
// 100, 80, 9 --> 980100

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

namespace SmartLib
{
	using namespace std;

	class FindMaxSeq
	{
	private:
		const vector<wstring> & _numbers;
	

		wstring MakeStr(const vector<int> & pos)
		{
			wstring str;
			for (auto pp : pos)
			{
				str += _numbers[pp];
			}

			return str;
		}

		vector<int> Sort(int count)
		{
			if (count == 0)
			{
				return {};
			}

			//if (count == 1)
			//{
			//	return { 0 };
			//}

			vector<int> result = Sort(count - 1);
			result.push_back(count - 1);
			wstring maxstr;
			vector<int> maxresult;
			for (int ii = count - 1; ii >= 0; --ii)
			{
				wstring str = MakeStr(result);
				if (str > maxstr)
				{
					maxstr = move(str);
					maxresult = result;
				}

				if (ii > 0)
				{
					std::swap(result[ii], result[ii - 1]);
				}
			}

			return maxresult;
		}

	public:
		FindMaxSeq(const vector<wstring>& numbers) :
			_numbers(numbers)
		{
		}

		wstring Solution()
		{
			vector<int> result = Sort(_numbers.size());
			
			for (const auto& number : _numbers)
			{
				wcout << number << L" ";
			}
			wcout << endl;

			wstring strResult;
			for (auto pos : result)
			{
				strResult += _numbers[pos];
				strResult += L' ';
			}


			wcout << strResult << endl;

			return strResult;
		}

		wstring Solution2()
		{
			auto compare = [this](int ii, int jj) -> bool
			{
				return (_numbers[ii] + _numbers[jj]) > (_numbers[jj] + _numbers[ii]);
			};

			vector<int> result;
			for (int ii = 0; ii < _numbers.size(); ++ ii)
			{
				result.push_back(ii);
			}

			std::sort(result.begin(), result.end(), compare);

			for (const auto& number : _numbers)
			{
				wcout << number << L" ";
			}
			wcout << endl;

			wstring strResult;
			for (auto pos : result)
			{
				strResult += _numbers[pos];
				strResult += L' ';
			}
			wcout << strResult << endl;

			return strResult;
		}
		
	};
}