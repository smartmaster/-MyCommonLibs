#pragma once

#include <winsock2.h>
#include <tchar.h>
#include <windows.h>
#include <atlbase.h>




static CONST TCHAR DUP_SOCK_EVENT_NAME[] = TEXT("Global\\dup_sock_event_E443645A3D0547A9B451030561A2F581_pid_%u");
static CONST TCHAR DUP_SOCK_PIPE_NAME[] = TEXT("\\\\.\\pipe\\dup_sock_pipe_8C53DB893714471C9163C1D76D25DADA_pid_%u");

HRESULT DuplicateSocketToTargetProcess(IN LPCTSTR pCommandLine, IN OUT SOCKET & ConnectionSocket);

typedef DWORD (WINAPI * pfnSocketProc_t)(_In_  LPVOID lpParameter);
HRESULT TargetProcessMain(pfnSocketProc_t pfnSocketProc);
