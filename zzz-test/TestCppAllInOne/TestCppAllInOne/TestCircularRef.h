#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <string>
#include <memory>

#include "log.h"

namespace SmartLib
{
	class Player
	{
	private:
		std::wstring _name;
		//std::shared_ptr<Player> _friend;
		std::weak_ptr<Player> _friend;

	public:
		Player(LPCWSTR name) :
			_name(name)
		{
			SML_LOG_LINE("%s created", name);
		}

		void SetFriend(std::shared_ptr<Player> & ff)
		{
			_friend = ff;
		}

		~Player()
		{
			SML_LOG_LINE("%s destroyed", _name.c_str());
		}
	};

	class TestCRef
	{
	public:
		static void Case0()
		{
			std::shared_ptr<Player> p1 = make_shared<Player>(L"Mike");
			std::shared_ptr<Player> p2 = make_shared<Player>(L"Jordan");
			p1->SetFriend(p2);
			p2->SetFriend(p1);
			SML_LOG_LINE(L"p1 refcount = %d, p2 refcount = %d", p1.use_count(), p2.use_count());
		}
	};
}