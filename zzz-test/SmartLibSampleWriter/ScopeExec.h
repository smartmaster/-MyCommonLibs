#pragma once

#include <functional>

namespace SmartLib
{
	class ScopeExec
	{
	private:
		std::function<void()> _end{ nullptr };

	public:
		ScopeExec(std::function<void()> begin, std::function<void()> end)
		{
			if (begin)
			{
				begin();
			}
			_end = end;
		}


		~ScopeExec()
		{
			if (_end)
			{
				_end();
			}
		}

		void Revoke()
		{
			_end = nullptr;
		}
	};
}