#pragma once

#include <functional>

namespace SmartLib
{
	class ScopeExit
	{
	private:
		std::function<void()> _func;
	public:
		ScopeExit(std::function<void()> func) :
			_func(func)
		{
		}

		void Set(std::function<void()> func)
		{
			_func = func;
		}

		void Revoke()
		{
			_func = nullptr;
		}

		~ScopeExit()
		{
			if (nullptr != _func)
			{
				_func();
				_func = nullptr;
			}
		}
	};

#define SCOPE_EXIT(v) ::SmartLib::ScopeExit _SmartLib_ScopeExit_ ## v
#define SCOPE_EXIT_OBJECT(v) _SmartLib_ScopeExit_ ## v
}