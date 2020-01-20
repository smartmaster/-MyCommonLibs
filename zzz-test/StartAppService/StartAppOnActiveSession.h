#pragma once

#include "stdafx.h"

//HRESULT FindProcessOnSession(IN LPCWSTR pExeName, IN DWORD SessionID, OUT DWORD & ProcessID);
//HRESULT DuplicateProcessToken(IN DWORD ProcessID, OUT HANDLE & hProcessTokenDup);
//HRESULT AdjustProcessToken(IN HANDLE hToken, IN LPCWSTR pPrivilegeName, IN DWORD SessID);
//HRESULT StartAppAsUser(HANDLE hToken, LPWSTR pCommandLine);
HRESULT StartAppOnActiveSession(
								LPCWSTR pUsername,
								LPCWSTR pDomain,
								LPCWSTR pPassword,
								LPCWSTR pCommandLine, 
								LPCWSTR pCurrentDirectory,
								PPROCESS_INFORMATION pPI);