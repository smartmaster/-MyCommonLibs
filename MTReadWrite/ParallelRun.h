#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include <functional>
#include <vector>

namespace SmartLib
{
	struct ParallelRun
	{
	private:
		static DWORD WINAPI ThreadProc(
			_In_ LPVOID lpParameter
			)
		{
			std::function<void()> & func = *(std::function<void()>*)(lpParameter);
			func();
			return 0;
		}


	public:
		static void Run(const std::initializer_list<std::function<void()> > & funcs, std::vector<HRESULT> & results)
		{
			std::vector<HANDLE> handles;

			DWORD ThreadId = 0;
			HANDLE handle = nullptr;
			DWORD lastError = S_OK;
			HRESULT hr = S_OK;

			for (const auto & func : funcs)
			{
				ThreadId = 0;
				handle = CreateThread(
					nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES  lpThreadAttributes,
					0,//_In_      SIZE_T                 dwStackSize,
					ThreadProc,//_In_      LPTHREAD_START_ROUTINE lpStartAddress,
					(LPVOID)(&func),//_In_opt_  LPVOID                 lpParameter,
					0,//_In_      DWORD                  dwCreationFlags,
					&ThreadId//_Out_opt_ LPDWORD                lpThreadId
					);
				if (nullptr != handle)
				{
					handles.push_back(handle);
					results.push_back(S_OK);
				}
				else
				{
					lastError = GetLastError();
					hr = HRESULT_FROM_WIN32(lastError);
					results.push_back(hr);
				}
			}


			//////////////////////////////////////////////////////////////////////////
			//wait to end
			INT finished = 0;
			INT remains = (INT)handles.size();
			while (remains)
			{
				int current = (remains > MAXIMUM_WAIT_OBJECTS ? MAXIMUM_WAIT_OBJECTS : remains);
				WaitForMultipleObjects(current, &handles[0] + finished, TRUE, INFINITE);
				finished += current;
				remains -= current;
			}

			//////////////////////////////////////////////////////////////////////////
			for (auto & handle : handles)
			{
				CloseHandle(handle);
				handle = nullptr;
			}
			handles.clear();
		}

		static void Test()
		{
			volatile LONG percent = 0;
			CONST LONG MAX_LOOP = 1000;

			std::vector<HRESULT> results;
			Run(
			{
				[&percent, MAX_LOOP]()
			{
				while (percent < MAX_LOOP)
				{
					Sleep(150);
					wprintf_s(L"running %d" L"\r\n", percent);
				}
			},

				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(200);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},

				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},

				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},

#if 0
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},
				[&percent, MAX_LOOP]()
			{
				LONG val = 0;
				for (int ii = 0; ii < MAX_LOOP; ++ii)
				{
					Sleep(300);
					val = InterlockedIncrement(&percent);
					if (val > MAX_LOOP)
					{
						break;
					}
				}
			},

				[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(200);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},

				[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},

					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				},
					[&percent, MAX_LOOP]()
				{
					LONG val = 0;
					for (int ii = 0; ii < MAX_LOOP; ++ii)
					{
						Sleep(300);
						val = InterlockedIncrement(&percent);
						if (val > MAX_LOOP)
						{
							break;
						}
					}
				}
#endif
			},
				results
					);
		}
	};

}