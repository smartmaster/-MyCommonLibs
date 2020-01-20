#pragma once

#include "log.h"
#include <functional>

namespace SmartLib
{
	class StringTest
	{
	public:
		static char * Find(char* start, std::function<bool(char)> func)
		{
			while (*start && !func(*start))
			{
				++start;
			}

			return start;
		}


		static void CopyString(char * & start1, char * & start2, std::function<bool(char ch)> func)
		{
			while (*start2 && func(*start2))
			{
				if (start1 != start2)
				{
					*start1 = *start2;
				}
				++start1;
				++start2;
			}
		}


		static	bool IsSpace(char ch)
		{
			return ch == ' ' || ch == '\t';
		}


		static	bool NotSpace(char ch)
		{
			return !IsSpace(ch);
		}


		static	char * RemoveSpaces(char * str)
		{

			char * start1 = Find(str, IsSpace);
			if (start1 != str && *start1)
			{
				++start1;
			}
			char * start2 = Find(start1, NotSpace);

			while (*start2)
			{
				CopyString(start1, start2, NotSpace);
				if (*start2) //must be space or 0
				{
					*start1++ = *start2++;
				}
				start2 = Find(start2, NotSpace);
			}

			*start1 = '\0';
			if (start1 > str)
			{
				if (IsSpace(*(start1 - 1)))
				{
					*(start1 - 1) = '\0';
				}
			}

			return str;
		}
	};


	class StringTestCase
	{
	public:
		static void Case0()
		{
			char str[] = "  char    * sta   rt1 = Fi   nd(s   tr, Is    Space);    ";
			//char str[] = "a a a a a a a a a a a";
			StringTest::RemoveSpaces(str);
			LOG_LINE_A("%s", str);
		}


	};

}