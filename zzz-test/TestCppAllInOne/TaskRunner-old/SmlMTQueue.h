#pragma once

#include <cassert>
#include <mutex>
#include <condition_variable>

#include "SmlList.h"


namespace SmartLib
{
	using namespace ::std;

	template<typename T, long MAX_COUNT> 
	class MTQueue
	{
	private:
		List<T, MAX_COUNT> _queque;
		mutex _mutex;
		condition_variable _cvSlots;
		condition_variable _cvData;
		volatile bool _stopProvider{ false };
		volatile bool _stopConsumer{ false };


	public:
		template<typename D>
		long push_back(D&& data)
		{
			unique_lock<mutex> lock{ _mutex };

			while (MAX_COUNT == _queque.size() && !_stopProvider)
			{
				_cvSlots.wait(lock);
			}

			if (_stopProvider)
			{
				return -1;
			}

			_queque.push_back(static_cast<D&&>(data));

			_cvData.notify_all();

			return 1;
		}

		long pop_front(T& data)
		{
			unique_lock<mutex> lock{ _mutex };

			while (0 == _queque.size() && !_stopConsumer)
			{
				_cvData.wait(lock);
			}

			if (_stopConsumer)
			{
				return -1;
			}

			data = _queque.pop_front();

			_cvSlots.notify_all();

			return 1;
		}

		//////////////////////////////////////////////////////////////////////////
		long GetRemainingDataCount() //no concurrent threads running
		{
			unique_lock<mutex> lock{ _mutex };
			return _queque.size();
		}


		//////////////////////////////////////////////////////////////////////////
		void StopProvider()
		{
			unique_lock<mutex> lock{ _mutex };
			_stopProvider = true;
			_cvSlots.notify_all();
		}

		void StopConsumer()
		{
			unique_lock<mutex> lock{ _mutex };
			_stopConsumer = true;
			_cvData.notify_all();
		}
	};
}