#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>
#include <assert.h>


#include "log.h"
#include "ScopeExit.h"

namespace SmartLib
{
	class CircurlarBufferCV
	{
	private:
		CONDITION_VARIABLE _cvBuffer; /***INTERNAL***/
		
		CONDITION_VARIABLE _cvData; /***INTERNAL***/
		
		CRITICAL_SECTION _cs; /***INTERNAL***//***CLEANUP***/
		SCOPE_EXIT(_cs) {
			[this]()
			{
				DeleteCriticalSection(&_cs);
			}
		};

		int _bufferTotalSize{ 0 };

		BYTE * _buffer{ nullptr };
		SCOPE_EXIT(_buffer) {
			[this]()
			{
				if (_buffer)
				{
					VirtualFree(_buffer, 0, MEM_RELEASE);
				}
			}
		};


		int _dataOffset{ 0 };
		int _dataLen{ 0 };

		volatile bool _stop{ false };


	private:
		static void WriteBuffer(BYTE * buffer, int bufferLen, int bufferOffset, const BYTE * data, int dataLen)
		{
			if (bufferOffset >= bufferLen)
			{
				bufferOffset %= bufferLen;
			}

			if (bufferOffset + dataLen > bufferLen)
			{
				int firstPart = bufferLen - bufferOffset;
				int secondPart = dataLen - firstPart;
				CopyMemory(buffer + bufferOffset, data, firstPart);
				CopyMemory(buffer, data + firstPart, secondPart);
			}
			else
			{
				CopyMemory(buffer + bufferOffset, data, dataLen);
			}
		}


		static void ReadBuffer(const BYTE * buffer, int bufferLen, int bufferOffset, BYTE * data, int dataLen)
		{
			assert(bufferOffset < bufferLen);
			if (bufferOffset + dataLen > bufferLen)
			{
				int firstPart = bufferLen - bufferOffset;
				int secondPart = dataLen - firstPart;
				CopyMemory(data, buffer + bufferOffset, firstPart);
				CopyMemory(data + firstPart, buffer, secondPart);
			}
			else
			{
				CopyMemory(data, buffer + bufferOffset, dataLen);
			}
		}

	public:

		CircurlarBufferCV(int bufferSize) :
			_bufferTotalSize(bufferSize)
		{
			InitializeConditionVariable(&_cvBuffer);
			InitializeConditionVariable(&_cvData);

			const DWORD SPIN_COUNT = 0x400;
			InitializeCriticalSectionAndSpinCount(&_cs, SPIN_COUNT);

			_buffer = (BYTE*)VirtualAlloc(
				nullptr,//_In_opt_ LPVOID lpAddress,
				_bufferTotalSize,//_In_     SIZE_T dwSize,
				MEM_COMMIT | MEM_RESERVE,//_In_     DWORD  flAllocationType,
				PAGE_READWRITE//,//_In_     DWORD  flProtect
			);
		}

		BOOL Write(
			_In_        LPCVOID      lpBuffer,
			_In_        DWORD        nNumberOfBytesToWrite,
			_Out_opt_   LPDWORD      lpNumberOfBytesWritten
		)
		{
			BOOL rr = TRUE;
			EnterCriticalSection(&_cs);
			
			while (_dataLen == _bufferTotalSize && !_stop)
			{
				SleepConditionVariableCS(&_cvBuffer, &_cs, INFINITE);
			}

			if (_stop)
			{
				rr = FALSE;
			}
			else
			{
				int bufferLen = _bufferTotalSize - _dataLen;
				if (nNumberOfBytesToWrite > bufferLen)
				{
					nNumberOfBytesToWrite = bufferLen;
				}

				WriteBuffer(_buffer, _bufferTotalSize, _dataOffset + _dataLen, (CONST BYTE*)lpBuffer, nNumberOfBytesToWrite);
				
				if (lpNumberOfBytesWritten)
				{
					*lpNumberOfBytesWritten = nNumberOfBytesToWrite;
				}

				_dataLen += nNumberOfBytesToWrite;
			}

			LeaveCriticalSection(&_cs);


			WakeAllConditionVariable(&_cvData);

			return rr;
		}

		BOOL WriteAll(
			_In_        LPCVOID      lpBuffer,
			_In_        DWORD        nNumberOfBytesToWrite,
			_Out_opt_   LPDWORD      lpNumberOfBytesWritten
		)
		{
			BOOL rr = FALSE;
			DWORD transffered = 0;
			DWORD currentTransffered = 0;
			while (transffered < nNumberOfBytesToWrite)
			{
				currentTransffered = 0;
				rr = Write((const BYTE*)lpBuffer + transffered, nNumberOfBytesToWrite - transffered, &currentTransffered);
				if (rr)
				{
					transffered += currentTransffered;
				}
				else
				{
					break;
				}
			}

			if (lpNumberOfBytesWritten)
			{
				*lpNumberOfBytesWritten = transffered;
			}

			return rr;
		}


		BOOL Read(
			_Out_        LPVOID lpBuffer,
			_In_         DWORD nNumberOfBytesToRead,
			_Out_opt_    LPDWORD lpNumberOfBytesRead
			)
		{
			BOOL rr = TRUE;
			EnterCriticalSection(&_cs);

			while (0 == _dataLen && !_stop)
			{
				SleepConditionVariableCS(&_cvData, &_cs, INFINITE);
			}

			if (_stop)
			{
				rr = FALSE;
			}
			else
			{
				int dataLen = _dataLen;
				if (nNumberOfBytesToRead > dataLen)
				{
					nNumberOfBytesToRead = dataLen;
				}

				ReadBuffer(_buffer, _bufferTotalSize, _dataOffset, (BYTE*)lpBuffer, nNumberOfBytesToRead);

				if (lpNumberOfBytesRead)
				{
					*lpNumberOfBytesRead = nNumberOfBytesToRead;
				}

				_dataLen -= nNumberOfBytesToRead;
				_dataOffset += nNumberOfBytesToRead;
				if (_dataOffset >= _bufferTotalSize)
				{
					_dataOffset %= _bufferTotalSize;
				}
			}

			LeaveCriticalSection(&_cs);

			WakeAllConditionVariable(&_cvBuffer);

			return rr;
		}


		BOOL ReadAll(
			_Out_        LPVOID lpBuffer,
			_In_         DWORD nNumberOfBytesToRead,
			_Out_opt_    LPDWORD lpNumberOfBytesRead
		)
		{
			BOOL rr = FALSE;
			DWORD transffered = 0;
			DWORD currentTransffered = 0;
			while (transffered < nNumberOfBytesToRead)
			{
				currentTransffered = 0;
				rr = Read((BYTE*)lpBuffer + transffered, nNumberOfBytesToRead - transffered, &currentTransffered);
				if (rr)
				{
					transffered += currentTransffered;
				}
				else
				{
					break;
				}
			}


			if (lpNumberOfBytesRead)
			{
				*lpNumberOfBytesRead = transffered;
			}

			return rr;
		}



		BOOL WriteMessage(
			_In_        LPCVOID      lpBuffer,
			_In_        DWORD        nNumberOfBytesToWrite
		)
		{
			assert(nNumberOfBytesToWrite <= _bufferTotalSize);

			BOOL rr = TRUE;
			EnterCriticalSection(&_cs);

			while (_bufferTotalSize - _dataLen < nNumberOfBytesToWrite && !_stop)
			{
				SleepConditionVariableCS(&_cvBuffer, &_cs, INFINITE);
			}

			if (_stop)
			{
				rr = FALSE;
			}
			else
			{
				WriteBuffer(_buffer, _bufferTotalSize, _dataOffset + _dataLen, (CONST BYTE*)lpBuffer, nNumberOfBytesToWrite);
				_dataLen += nNumberOfBytesToWrite;
			}

			LeaveCriticalSection(&_cs);


			WakeAllConditionVariable(&_cvData);

			return rr;
		}


		BOOL ReadMessage(
			_Out_        LPVOID lpBuffer,
			_In_         DWORD nNumberOfBytesToRead
		)
		{

			assert(nNumberOfBytesToRead <= _bufferTotalSize);

			BOOL rr = TRUE;
			EnterCriticalSection(&_cs);

			while (_dataLen < nNumberOfBytesToRead && !_stop)
			{
				SleepConditionVariableCS(&_cvData, &_cs, INFINITE);
			}

			if (_stop)
			{
				rr = FALSE;
			}
			else
			{
				
				ReadBuffer(_buffer, _bufferTotalSize, _dataOffset, (BYTE*)lpBuffer, nNumberOfBytesToRead);

				_dataLen -= nNumberOfBytesToRead;
				_dataOffset += nNumberOfBytesToRead;
				if (_dataOffset >= _bufferTotalSize)
				{
					_dataOffset %= _bufferTotalSize;
				}
			}

			LeaveCriticalSection(&_cs);

			WakeAllConditionVariable(&_cvBuffer);

			return rr;
		}

		
		void GetRemains(std::vector<BYTE> & data)
		{

			EnterCriticalSection(&_cs);
			data.resize(_dataLen);
			//for (int ii = 0; ii < _dataLen; ++ ii)
			//{
			//	data[ii] = _buffer[(_dataOffset + ii) % _bufferTotalSize];
			//}
			ReadBuffer(_buffer, _bufferTotalSize, _dataOffset, (BYTE*)(&data[0]), _dataLen);
			_dataOffset = 0;
			_dataLen = 0;
			LeaveCriticalSection(&_cs);
		}

		void Stop()
		{
			_stop = TRUE;
			WakeAllConditionVariable(&_cvData);
			WakeAllConditionVariable(&_cvBuffer);
		}
	};


	class CircurlarBufferCVTest
	{
	private:
		const int BUFFER_SIZE = 997; //prime number less than 1000
		LONGLONG _data{ 0 };


	public:
		CircurlarBufferCV _cb{ BUFFER_SIZE };

	public:
		void Provide()
		{
			const int COUNT = 17;
			LONGLONG data[COUNT];
			for (int ii = 0; ii < COUNT; ++ ii)
			{
				data[ii] = _data++;
			}

			//while (_cb.WriteAll(data, sizeof(data), nullptr))
			while (_cb.WriteMessage(data, sizeof(data)))
			{
				for (int ii = 0; ii < COUNT; ++ii)
				{
					data[ii] = _data++;
				}
			}
		}

		

		void Consume()
		{
			LONGLONG prevData = -1;

			const int COUNT = 17;
			LONGLONG data[COUNT];
			//while (_cb.ReadAll(&data, sizeof(data), nullptr))
			while (_cb.ReadMessage(&data, sizeof(data)))
			{
				assert(VerifyData(prevData, data, COUNT));
				for (int ii = 0; ii < COUNT; ++ ii)
				{
					LOG_LINE(TEXT("%08I64d"), data[ii]);
				}
				
				prevData = data[COUNT - 1];
			}
		}


		static bool VerifyData(LONGLONG prev, LONGLONG * data, int count)
		{
			bool rr = (prev + 1 == data[0]);
			if (!rr)
			{
				return rr;
			}


			for (int ii = 0; ii < count - 1; ++ii)
			{
				if (data[ii] + 1 != data[ii + 1])
				{
					rr = FALSE;
					break;
				}
			}

			return rr;
		}


		void Stop()
		{
			_cb.Stop();
		}

		static DWORD WINAPI ThreadProcProvider(
			_In_ LPVOID lpParameter
		)
		{
			CircurlarBufferCVTest * obj = (CircurlarBufferCVTest*)lpParameter;
			obj->Provide();
			return 0;
		}


		static DWORD WINAPI ThreadProcConsumer(
			_In_ LPVOID lpParameter
		)
		{
			CircurlarBufferCVTest * obj = (CircurlarBufferCVTest*)lpParameter;
			obj->Consume();
			return 0;
		}


		static void TestCase()
		{
			CircurlarBufferCVTest test;
			HANDLE thread[2] = { 0 };

			DWORD ThreadId = 0;
			thread[0] = CreateThread(
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES  lpThreadAttributes,
				0,//_In_      SIZE_T                 dwStackSize,
				ThreadProcConsumer,//_In_      LPTHREAD_START_ROUTINE lpStartAddress,
				&test,//_In_opt_  LPVOID                 lpParameter,
				0,//_In_      DWORD                  dwCreationFlags,
				&ThreadId//,//_Out_opt_ LPDWORD                lpThreadId
			);


			ThreadId = 0;
			thread[1] = CreateThread(
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES  lpThreadAttributes,
				0,//_In_      SIZE_T                 dwStackSize,
				ThreadProcProvider,//_In_      LPTHREAD_START_ROUTINE lpStartAddress,
				&test,//_In_opt_  LPVOID                 lpParameter,
				0,//_In_      DWORD                  dwCreationFlags,
				&ThreadId//,//_Out_opt_ LPDWORD                lpThreadId
			);



			int time = 30;
			for (int ii = 0; ii < time; ++ ii)
			{
				Sleep(1000);
			}


			test.Stop();

			WaitForMultipleObjects(_countof(thread), thread, TRUE, INFINITE);
			for (HANDLE h : thread)
			{
				CloseHandle(h);
			}

			std::vector<BYTE> remain;
			test._cb.GetRemains(remain);
			LONGLONG * ll = (LONGLONG*)&remain[0];
			int count = remain.size();
			//assert(0 == count % sizeof(LONGLONG));
			LOG_LINE(TEXT("remains are:"));
			for (int ii = 0; ii < count/sizeof(LONGLONG); ++ ii)
			{
				LOG_LINE(TEXT("%08I64d"), ll[ii]);
			}
		}
	};
}