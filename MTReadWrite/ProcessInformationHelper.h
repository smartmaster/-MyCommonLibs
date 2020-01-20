#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <winternl.h>

#include <vector>
#include <algorithm>

#ifndef PRINT_LINE
#define PRINT_LINE(str, ...) _ftprintf_s(stdout, str TEXT("\r\n"), __VA_ARGS__)
#endif


#ifndef BEGIN_BLOCK
#define BEGIN_BLOCK(level_number)			do{	int	_abc_xyz##level_number(level_number);
#define LEAVE_BLOCK(level_number)			{(_abc_xyz##level_number);}	break;
#define END_BLOCK(level_number)				{(_abc_xyz##level_number);}	}while(0);
#endif

namespace SML
{
	struct ProcessInformationHelper
	{
		static const INT INCR_DELTA = 2;

	private:
		typedef NTSTATUS(NTAPI * SML_NtQueryInformationProcess)(
			HANDLE ProcessHandle,
			DWORD ProcessInformationClass,
			PVOID ProcessInformation,
			DWORD ProcessInformationLength,
			PDWORD ReturnLength
			);

		template<typename T>
		static HRESULT ReadProcessStruct(IN HANDLE processHandle, LPCVOID lpBaseAddress, T & ss)
		{
			HRESULT hr = S_OK;
			DWORD lastError = 0;

			SIZE_T  NumberOfBytesRead = 0;
			BOOL bb = ::ReadProcessMemory(
				processHandle,//_In_  HANDLE  hProcess,
				lpBaseAddress,//_In_  LPCVOID lpBaseAddress,
				&ss,//_Out_ LPVOID  lpBuffer,
				sizeof(T),//_In_  SIZE_T  nSize,
				&NumberOfBytesRead//,//_Out_ SIZE_T  *lpNumberOfBytesRead
			);

			if (!bb)
			{
				lastError = GetLastError();
				hr = HRESULT_FROM_WIN32(lastError);
			}

			return hr;
		}

		static HRESULT ReadProcessBuffer(HANDLE processHandle, LPCVOID lpBaseAddress, SIZE_T  nSize, std::vector<LONGLONG> & buffer)
		{
			HRESULT hr = S_OK;
			DWORD lastError = 0;
			buffer.resize(nSize / sizeof(LONGLONG) + INCR_DELTA);

			SIZE_T  NumberOfBytesRead = 0;
			BOOL bb = ::ReadProcessMemory(
				processHandle,//_In_  HANDLE  hProcess,
				lpBaseAddress,//_In_  LPCVOID lpBaseAddress,
				&buffer[0],//_Out_ LPVOID  lpBuffer,
				nSize,//_In_  SIZE_T  nSize,
				&NumberOfBytesRead//,//_Out_ SIZE_T  *lpNumberOfBytesRead
			);

			if (!bb)
			{
				lastError = GetLastError();
				hr = HRESULT_FROM_WIN32(lastError);
			}

			return hr;
		}


		static HRESULT QueryPBI(IN HANDLE processHandle, OUT PROCESS_BASIC_INFORMATION & pbi)
		{
			HRESULT hr = S_OK;

			SML_NtQueryInformationProcess my_NtQueryInformationProcess =
				(SML_NtQueryInformationProcess)GetProcAddress(
					GetModuleHandleA("ntdll.dll"),
					"NtQueryInformationProcess");
			NTSTATUS nts = my_NtQueryInformationProcess(processHandle, ProcessBasicInformation, &pbi, sizeof(pbi), NULL);

			if (!NT_SUCCESS(nts))
			{
				hr = HRESULT_FROM_NT(nts);
			}

			return hr;
		}

		static HRESULT OpenProcessById(DWORD pid, HANDLE & handle)
		{

			HRESULT hr = S_OK;
			DWORD lastError = 0;

			handle = nullptr;
			if (0 == pid)
			{
				handle = ::GetCurrentProcess();
			}
			else
			{
				handle = ::OpenProcess(
					PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE,//_In_ DWORD dwDesiredAccess,
					FALSE,//_In_ BOOL  bInheritHandle,
					pid//,//_In_ DWORD dwProcessId
				);
				if (nullptr == handle)
				{
					lastError = GetLastError();
					hr = HRESULT_FROM_WIN32(lastError);
				}
			}

			return hr;
		}

	public:

		static HRESULT ClearCurrentProcessCommandLine()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);

			//////////////////////////////////////////////////////////////////////////
			HANDLE process = GetCurrentProcess();

			PROCESS_BASIC_INFORMATION pbi = { 0 };
			hr = QueryPBI(process, pbi);
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}

			
			UNICODE_STRING & CommandLine = pbi.PebBaseAddress->ProcessParameters->CommandLine;
			SecureZeroMemory(CommandLine.Buffer, CommandLine.Length);
			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}

		static HRESULT ChangeProcessCommandLine(DWORD pid)
		{

			HRESULT hr = S_OK;
			DWORD LastError = 0;

			HANDLE process = nullptr;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			
			hr = OpenProcessById(pid, process);
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			PROCESS_BASIC_INFORMATION pbi = { 0 };
			hr = QueryPBI(process, pbi);
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			PEB peb = { 0 };
			hr = ReadProcessStruct(process, pbi.PebBaseAddress, peb);
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			RTL_USER_PROCESS_PARAMETERS ProcessParameters = { 0 };
			hr = ReadProcessStruct(process, peb.ProcessParameters, ProcessParameters);
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			std::vector<LONGLONG> zeroBuffer;
			zeroBuffer.resize(ProcessParameters.CommandLine.Length / sizeof(LONGLONG) + INCR_DELTA);
			

			SIZE_T NumberOfBytesWritten = 0;
			BOOL bwpm = ::WriteProcessMemory(process, 
				ProcessParameters.CommandLine.Buffer, 
				&zeroBuffer[0],
				ProcessParameters.CommandLine.Length,
				&NumberOfBytesWritten);
			if (!bwpm)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			if (process != nullptr)
			{
				::CloseHandle(process);
				process = nullptr;
			}

			return hr;

		}

	};
}