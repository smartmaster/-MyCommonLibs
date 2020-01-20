#pragma once

#include <functional>

namespace SmartLib
{
	class ScopeExec
	{
	private:
		std::function<void()> _init;
		std::function<void()> _exit;
	public:
		ScopeExec(std::function<void()> init, std::function<void()> exit) :
			_init(init),
			_exit(exit)
		{
			if (nullptr != _init)
			{
				_init();
			}
		}

		void Set(std::function<void()> exit)
		{
			_exit = exit;
		}

		void Revoke()
		{
			_exit = nullptr;
		}

		~ScopeExec()
		{
			if (nullptr != _exit)
			{
				_exit();
				_exit = nullptr;
			}
		}
	};

#define SCOPE_EXEC(v) ::SmartLib::ScopeExec _SmartLib_ScopeExec_ ## v
#define SCOPE_EXEC_OBJECT(v) _SmartLib_ScopeExec_ ## v
}