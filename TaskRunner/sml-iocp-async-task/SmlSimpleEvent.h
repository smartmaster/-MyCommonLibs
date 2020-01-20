#pragma once

#include <mutex>
#include <condition_variable>

namespace SmartLib
{
	using namespace ::std;

	//////////////////////////////////////////////////////////////////////////
	class SimpleEvent
	{
	private:
		volatile bool _signaled{ false };
		mutex _mutex;
		condition_variable _cv;

	public:
		void Reset()
		{
			unique_lock<mutex> lock{ _mutex };
			_signaled = false;
		}

		void Set()
		{
			unique_lock<mutex> lock{ _mutex };
			_signaled = true;
			lock.unlock();
			_cv.notify_all();
		}

		void Wait()
		{
			unique_lock<mutex> lock{ _mutex };
			while (!_signaled)
			{
				_cv.wait(lock);
			}
		}
	};

}