#pragma once

#include <cassert>
#include <mutex>
#include <condition_variable>

#include "SmlList.h"


namespace SmartLib
{
	using namespace ::std;

	enum class MTQ_STATUS : long
	{
		_ERROR = -1,
		_TIMEOUT = -2,
	};

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

		//bool IsEmpty()
		//{
		//	unique_lock<mutex> lock{ _mutex };
		//	return 0 == _queque.size();
		//}


		//bool IsFull()
		//{
		//	unique_lock<mutex> lock{ _mutex };
		//	return MAX_COUNT == _queque.size();
		//}


		template<typename D>
		long push_back(D&& data, long millsec = 0)
		{
			unique_lock<mutex> lock{ _mutex };

			while (MAX_COUNT == _queque.size() && !_stopProvider)
			{
				if (millsec > 0)
				{
					auto status = _cvSlots.wait_for(lock, chrono::milliseconds(millsec));
					if (cv_status::timeout == status)
					{
						return (long)MTQ_STATUS::_TIMEOUT;
					}
				}
				else
				{
					_cvSlots.wait(lock);
				}
			}

			if (_stopProvider)
			{
				return (long)MTQ_STATUS::_ERROR;
			}

			_queque.push_back(static_cast<D&&>(data));


			lock.unlock();

			_cvData.notify_all();

			return 1;
		}

		long pop_front(T& data, long millsec = 0)
		{
			unique_lock<mutex> lock{ _mutex };

			while (0 == _queque.size() && !_stopConsumer)
			{
				if (millsec > 0)
				{
					auto status = _cvData.wait_for(lock, chrono::milliseconds(millsec));
					if (cv_status::timeout == status)
					{
						return (long)MTQ_STATUS::_TIMEOUT;
					}
				}
				else
				{
					_cvData.wait(lock);
				}
			}

			if (_stopConsumer)
			{
				return (long)MTQ_STATUS::_ERROR;
			}

			data = _queque.pop_front();

			lock.unlock();

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
			lock.unlock();
			_cvSlots.notify_all();
		}

		void StopConsumer()
		{
			unique_lock<mutex> lock{ _mutex };
			_stopConsumer = true;
			lock.unlock();
			_cvData.notify_all();
		}
	};
}