#pragma once

#include <memory>

#include "log.h"

namespace SML
{
	class MyCcc
	{
	private:
		static int s_index;

	private:
		int _index;

	public:
		std::weak_ptr<MyCcc> Friend;

		MyCcc()
		{
			_index = s_index++;
			LOG_LINE(TEXT("Created %d"), _index);
		}

		~MyCcc()
		{
			LOG_LINE(TEXT("Detroyed %d"), _index);
		}

	};


	struct TestSharedPtr
	{
	public:
		static void Case0()
		{
			MyCcc* p0 = nullptr;
			MyCcc* p1 = nullptr;

			{
				std::shared_ptr<MyCcc> sp0 = std::make_shared<MyCcc>();
				std::shared_ptr<MyCcc> sp1 = std::make_shared<MyCcc>();
				sp0->Friend = sp1; //make circular reference
				sp1->Friend = sp0;

				p0 = sp0.get();
				p1 = sp1.get();

				LOG_LINE(TEXT("%d, %d"), sp0.use_count(), sp1.use_count());
			}


			LOG_LINE(TEXT("%d, %d"), p0->Friend.use_count(), p1->Friend.use_count());

			//app crash
			//how to break the circular reference?
			//if shared_ptr has semantics of detach/attach, it will be easy.
			delete p0; 
			delete p1;

		}
	};
}//namespace SML