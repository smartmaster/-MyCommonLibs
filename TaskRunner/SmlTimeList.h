#pragma once

#include <mutex>
#include <thread>
#include <functional>

#include "SmlList.h"
#include "SmlRefPtr.h"

namespace SmartLib
{
	using namespace ::std;

	template<typename T, typename DISPOSE = function<void(T&)>, typename MUTEX = mutex, typename LOCK = unique_lock<mutex>>
	class TimeList
	{
	private:
		struct TimeObject
		{
			T Data{};
			long Life{ 0 };


			TimeObject() = default;

			template<typename D>
			TimeObject(D&& data) :
				Data{ static_cast<D&&>(data) }
			{
			}

			TimeObject(TimeObject&& to) :
				Data{ static_cast<T&&>(to.Data) },
				Life{ to.Life }
			{
			}


			TimeObject(TimeObject& to) :
				Data{ to.Data },
				Life{ to.Life }
			{
			}

			const TimeObject& operator=(TimeObject&& to)
			{
				Data = static_cast<T&&>(to.Data);
				Life = to.Life;
				return *this;
			}


			const TimeObject& operator=(TimeObject& to)
			{
				Data = to.Data;
				Life = to.Life;
				return *this;
			}
		};


	private:
		long _scanInterval{ 0 }; //in millsec
		long _maxLife{ 0 };//in millsec
		List<TimeObject> _list;

		DISPOSE _dispose;
		MUTEX _mtx;
		volatile bool _stop{ false };

		RefPtr<thread> _thread;

	private:
		void ScanOnePass(bool force)
		{

			LOCK lock{ _mtx };

			auto first = _list.First();
			auto stop = _list.Stop();
			while (first != stop)
			{
				TimeObject& to = first->Data;
				to.Life += _scanInterval;
				if (to.Life >= _maxLife || force)
				{
					if (_dispose)
					{
						_dispose(to.Data);
					}
					else
					{
						T data = static_cast<T&&>(to.Data);
					}
					first = _list.DeleteNode(first);
				}
				else
				{
					first = first->Next;
				}
			}
		}

		void ScanProc()
		{
			while (!_stop)
			{
				this_thread::sleep_for(chrono::milliseconds(_scanInterval));
				ScanOnePass(false);
			}
		}

		void StartScan()
		{
			_thread = RefPtr<thread>::Make([this]()
			{
				this->ScanProc();
			});
		}

	public:

		TimeList(long scanInterval, long maxLife) :
			_scanInterval{ scanInterval },
			_maxLife{ maxLife }
		{
			StartScan();
		}

		~TimeList()
		{
			StopScan();
		}

		void StopScan()
		{
			if (!_stop)
			{
				_stop = true;
				_thread->join();
			}
			ScanOnePass(true);
		}

		template<typename D>
		void SetDispose(D&& dispose)
		{
			LOCK lock{ _mtx };

			_dispose = static_cast<D&&>(dispose);
		}

		template<typename D>
		void push_back(D&& data)
		{
			LOCK lock{ _mtx };

			_list.push_back(TimeObject{ static_cast<D&&>(data) });
		}
	};
}