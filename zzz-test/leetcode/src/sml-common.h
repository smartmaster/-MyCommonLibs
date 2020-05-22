#pragma once

#include <ctime>
#include <cstdlib>


#ifdef _WIN32

#else

#define _countof(arr) (sizeof(arr)/sizeof(arr[0])) //linux

#endif


namespace SmartLib
{
	using namespace ::std;

	class Common
	{
	public:
		template<typename TOUT, typename T, typename... TARGS>
		static TOUT& Print(TOUT& out, T&& arg, TARGS&&... args)
		{
			out << static_cast<T&&>(arg);
			Print(out, static_cast<TARGS&&>(args)...);
			return out;
		}

		template<typename TOUT>
		static TOUT& Print(TOUT& out)
		{
			return out;
		}


		template<typename TOUT, typename TITER>
		static TOUT& PrintIterator(TOUT& out, TITER begin, TITER end)
		{
			while (begin != end)
			{
				out << *begin << ' ';
				++begin;
			}

			return out;
		}

		template<typename TITER>
		static void FillRandom(TITER begin, TITER end, long min, long max)
		{
			long data = max - min;
			srand(time(nullptr));
			while (begin != end)
			{
				*begin = rand() % data + min;
				++begin;
			}
		}

		template<typename TITER>
		static bool VerifyOrder(TITER begin, TITER end)
		{
			if (begin == end)
			{
				return true;
			}

			TITER next = begin;
			++next;
			bool result = true;
			while (next != end)
			{
				if (*begin > *next)
				{
					result = false;
					break;
				}

				++begin;
				++next;
			}

			return result;
		}
	};

	
}