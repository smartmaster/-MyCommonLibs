#pragma once
#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <Shlobj.h>

namespace SmartLib
{
	class CGetSpectialFolder
	{
	public:
		static FILE* GetLoggerStream()
		{
			CONST INT BUFFER_SIZE = 1024;
			CString strPath;
			LPTSTR buffer = strPath.GetBuffer(BUFFER_SIZE);
			//DWORD dwRet = GetTempPath(
			//	BUFFER_SIZE,//_In_  DWORD  nBufferLength,
			//	buffer//_Out_ LPTSTR lpBuffer
			//);

			FILE* pFile = nullptr;
			HRESULT hr = ::SHGetFolderPath(
				nullptr,//_In_  HWND   hwndOwner,
				CSIDL_WINDOWS,//_In_  int    nFolder,
				nullptr,//_In_  HANDLE hToken,
				0,//_In_  DWORD  dwFlags,
				buffer//,//_Out_ LPTSTR pszPath
			);
			strPath.ReleaseBuffer();
			if (SUCCEEDED(hr))
			{
				strPath += TEXT("\\temp\\arcserve.GetApplicationDetails.log");
				::DeleteFile(strPath.GetString());
				errno_t err = _wfopen_s(
					&pFile,//FILE** pFile,
					strPath.GetString(),//const wchar_t *filename,
					L"a+"//const wchar_t *mode
				);
				//char errmsg[128] = { 0 };
				//strerror_s(errmsg, err);
				_ftprintf(stdout, TEXT("Log file path is [%s]") TEXT("\r\n"), strPath.GetString());
			}
			
			if (nullptr == pFile)
			{
				pFile = stdout;
			}
			return pFile;
		}
	};
}