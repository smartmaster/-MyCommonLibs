#pragma once
#include "SmlTaskRunnner.h"

namespace SmartLib
{
	class TaskRunnerTest
	{
	public:
		static void Case0()
		{
			RefPtr<TaskRunner> tr = RefPtr<TaskRunner>::Make();
			TaskRunner* trshadow = tr.Ptr();
			trshadow->Create();
			trshadow->StartThreads();
			trshadow->StopThreads();
		}
	};
}