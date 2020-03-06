// testminhook.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "SocketConnect.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "HookedAPIList.h"



int _tmain(int argc, TCHAR ** argv)
{
	int testCase = 0;
	if (argc > 1)
	{
		testCase = _tcstol(argv[1], nullptr, 10);
	}


	SmartLib::HookedAPIList* hookedApi = SmartLib::HookedAPIList::GetHookedAPIList();
	hookedApi->Hook();


	switch (testCase)
	{
	case 2:
	{
		LPCVOID pv1 = ::CreateFileW;
		LPCVOID pv2 = &::CreateFileW;
		ULONG_PTR diff = (ULONG_PTR)pv1 - (ULONG_PTR)pv2;
		WSADATA wsadat = { 0 };
		WSAStartup(MAKEWORD(2, 2), &wsadat);
		SmartLib::CSocketConnect::Case3();
		SmartLib::CSocketConnect::Case2();
		SmartLib::CSocketConnect::Case1();
		WSACleanup();
	}
	break;
	case 1:
	{
		SmartLib::CSocketConnect::Connect(argv[2], argv[3]);
	}
	break;
	case 0:
	{
		HANDLE handle = ::CreateFile(
			L"e:\\000.txt",//_In_      LPCTSTR lpFileName,
			GENERIC_ALL,//_In_      DWORD dwDesiredAccess,
			0,//_In_      DWORD dwShareMode,
			nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			CREATE_ALWAYS,//_In_      DWORD dwCreationDisposition,
			0,//_In_      DWORD dwFlagsAndAttributes,
			nullptr//,//_In_opt_  HANDLE hTemplateFile
		);

		if (INVALID_HANDLE_VALUE != handle)
		{
			::CloseHandle(handle);
			handle = INVALID_HANDLE_VALUE;
		}
	}
	break;
	case -1:
	default:
		break;
	}
	
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
