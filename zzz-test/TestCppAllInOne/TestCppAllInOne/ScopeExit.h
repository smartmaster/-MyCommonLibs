#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include <functional>
#include <vector>
#include <memory>

#include "log.h"

namespace SmartLib
{
	class CSopeExit
	{
	private:
		std::vector<std::function<void()>> _vec;

	public:

		template<typename T>
		int Add(T && func)
		{
			_vec.push_back(std::forward<T>(func));
			return (int)_vec.size() - 1;
		}

		void Remove(int index)
		{
			_vec[index] = nullptr;
		}

		~CSopeExit()
		{
			for (int index = (int)_vec.size() - 1; index >= 0; --index)
			{
				if (_vec[index])
				{
					_vec[index]();
					_vec[index] = nullptr;
				}
			}
			_vec.clear();
		}
	};


	class CSopeExitTest
	{
	public:
		static void Case0()
		{
			CSopeExit scope_exit;
			int index = 0;
			scope_exit.Add([index]()
			{
				SML_LOG_LINE(TEXT("%d"), index);
			});

			++index;
			auto func = [index]()
			{
				SML_LOG_LINE(TEXT("%d"), index);
			};
			scope_exit.Add(func);

		}

	};
}//namespace SmartLib

