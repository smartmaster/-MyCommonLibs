#pragma once

#include <memory.h>

#include <cassert>
#include <mutex>
#include <condition_variable>

#include "SmlVector.h"

namespace SmartLib
{
	using namespace ::std;

	enum class CB_STATUS : long
	{
		_ERROR = -1,
		_TIMEOUT = -2,
	};

	class CircularBuffer
	{
	private:
		Vector<char, false, 2> _buffer;
		mutex _mutex;
		condition_variable _cvSlots;
		condition_variable _cvData;
		long _start{ 0 };
		long _stop{ 0 };

		volatile bool _stopProvider{ false };
		volatile bool _stopConsumer{ false };

	private:
		void PutBuffer(long pos, const char* data, long len)
		{
			assert(pos < _buffer.size());

			if (pos + len <= _buffer.size())
			{
				memcpy(_buffer.First() + pos, data, len);
			}
			else
			{
				long len1 = _buffer.size() - pos;
				memcpy(_buffer.First() + pos, data, len1);
				memcpy(_buffer.First(), data + len1, len - len1);
			}
		}

		void GetBuffer(long pos, char* data, long len)
		{
			assert(pos < _buffer.size());

			if (pos + len <= _buffer.size())
			{
				memcpy(data, _buffer.First() + pos, len);
			}
			else
			{
				long len1 = _buffer.size() - pos;
				memcpy(data, _buffer.First() + pos, len1);
				memcpy(data + len1, _buffer.First(), len - len1);
			}
		}

	public:
		CircularBuffer(long bufferSize)
		{
			_buffer.resize(bufferSize);
		}

		//bool IsEmpty()
		//{
		//	unique_lock<mutex> lock{ _mutex };
		//	return _start == _stop;
		//}
		//
		//bool IsFull()
		//{
		//	unique_lock<mutex> lock{ _mutex };
		//	return _buffer.size() == (_stop - _start);
		//}
		//	

		//bool IsStopped()
		//{
		//	unique_lock<mutex> lock{ _mutex };
		//	return _stopProvider || _stopConsumer;
		//}

		//bool IsEmptyNoLock()
		//{
		//	//unique_lock<mutex> lock{ _mutex };
		//	return _start == _stop;
		//}

		//bool IsFullNoLock()
		//{
		//	//unique_lock<mutex> lock{ _mutex };
		//	return _buffer.size() == (_stop - _start);
		//}

		//bool IsStoppedNoLock()
		//{
		//	//unique_lock<mutex> lock{ _mutex };
		//	return _stopProvider || _stopConsumer;
		//}

		long push_back(const char* data, long len, long millsec = 0)
		{
			unique_lock<mutex> lock{ _mutex };

			while ((_stop - _start) == _buffer.size() && !_stopProvider)
			{
				if (millsec > 0)
				{
					auto status = _cvSlots.wait_for(lock, chrono::milliseconds(millsec));
					if (cv_status::timeout == status)
					{
						return (long)CB_STATUS::_TIMEOUT;
					}
				}
				else
				{
					_cvSlots.wait(lock);
				}
				
			}

			if (_stopProvider)
			{
				return (long)CB_STATUS::_ERROR;
			}


			long slots = _buffer.size() - (_stop - _start);
			slots = slots < len ? slots : len;

			PutBuffer(_stop % _buffer.size(), data, slots);
			_stop += slots;

			lock.unlock();

			_cvData.notify_all();

			return slots;
		}




		long pop_front(char* data, long len, long millsec = 0)
		{
			unique_lock<mutex> lock{ _mutex };

			while (_stop == _start && !_stopConsumer)
			{
				if (millsec > 0)
				{
					auto status = _cvData.wait_for(lock, chrono::milliseconds(millsec));
					if (cv_status::timeout == status)
					{
						return (long)CB_STATUS::_TIMEOUT;
					}
				}
				else
				{
					_cvData.wait(lock);
				}
			}

			if (_stopConsumer)
			{
				return (long)CB_STATUS::_ERROR;
			}

			long dataLen = _stop - _start;
			dataLen = dataLen < len ? dataLen : len;
			GetBuffer(_start % _buffer.size(), data, dataLen);
			_start += dataLen;
			while (_start >= _buffer.size())
			{
				_start -= _buffer.size();
				_stop -= _buffer.size();
			}

			lock.unlock();
			_cvSlots.notify_all();

			return dataLen;
		}

		long push_back_all(const char* data, long len)
		{
			long count = 0;
			while (len > 0)
			{
				long curlen = push_back(data, len);
				if (curlen < 0)
				{
					break;
				}
				count += curlen;
				data += curlen;
				len -= curlen;
			}

			return count;
		}

		long pop_front_all(char* data, long len)
		{
			long count = 0;
			while (len > 0)
			{
				long curlen = pop_front(data, len);
				if (curlen < 0)
				{
					break;
				}
				count += curlen;
				data += curlen;
				len -= curlen;
			}
			return count;
		}

		//////////////////////////////////////////////////////////////////////////
		long GetRemainingDataCount() //no concurrent threads running
		{
			unique_lock<mutex> lock{ _mutex };
			return _stop - _start;
		}
		

		//////////////////////////////////////////////////////////////////////////
		void StopProvider()
		{
			unique_lock<mutex> lock{ _mutex };
			_stopProvider= true;
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