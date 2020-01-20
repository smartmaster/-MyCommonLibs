#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>
#include <functional>

#include "CodeBlock.h"


namespace SmartLib
{
	class VolumeHepler
	{
	public:
		static HRESULT FindAllVolumes(std::vector<CString> & volumes, std::function<BOOL(const CString &)> predict)
		{
			TCHAR vol[256] = { 0 };
			HANDLE hfind = FindFirstVolume(vol, _countof(vol));
			if (hfind != INVALID_HANDLE_VALUE)
			{
				if (predict == nullptr || predict(vol))
				{
					volumes.push_back(vol);
				}


				for (;;)
				{
					vol[0] = 0;
					BOOL fnv = FindNextVolume(hfind, vol, _countof(vol));
					if (!fnv)
					{
						break;
					}

					if (predict == nullptr || predict(vol))
					{
						volumes.push_back(vol);
					}
				}

				FindVolumeClose(hfind);
			}
		}


		static HRESULT FindAllVolumes(std::vector<CString> & volumes)
		{
			auto precit = [](const CString & vol) -> BOOL
			{
				BOOL ok = TRUE;

				
				BEGIN_BLOCK(0);
				//////////////////////////////////////////////////////////////////////////
				UINT dt = GetDriveType(vol.GetString());
				if (dt != DRIVE_FIXED)
				{
					ok = FALSE;
					LEAVE_BLOCK(0);
				}


				TCHAR VolumeNameBuffer[128] = { 0 };
				DWORD VolumeSerialNumber = 0;
				DWORD MaximumComponentLength = 0;
				DWORD FileSystemFlags = 0;
				TCHAR FileSystemNameBuffer[128] = { 0 };
				BOOL gvi = GetVolumeInformation(
					vol.GetString(),//_In_opt_  LPCTSTR lpRootPathName,
					VolumeNameBuffer,//_Out_opt_ LPTSTR  lpVolumeNameBuffer,
					_countof(VolumeNameBuffer),//_In_      DWORD   nVolumeNameSize,
					&VolumeSerialNumber,//_Out_opt_ LPDWORD lpVolumeSerialNumber,
					&MaximumComponentLength,//_Out_opt_ LPDWORD lpMaximumComponentLength,
					&FileSystemFlags,//_Out_opt_ LPDWORD lpFileSystemFlags,
					FileSystemNameBuffer,//_Out_opt_ LPTSTR  lpFileSystemNameBuffer,
					_countof(FileSystemNameBuffer)//,//_In_      DWORD   nFileSystemNameSize
				);
				if (!gvi)
				{
					ok = FALSE;
					LEAVE_BLOCK(0);
				}


				if (_tcsicmp(TEXT("NTFS"), FileSystemNameBuffer) && 
					_tcsicmp(TEXT("REFS"), FileSystemNameBuffer) && 
					_tcsicmp(TEXT("CVFS"), FileSystemNameBuffer))
				{
					ok = FALSE;
					LEAVE_BLOCK(0);
				}


				DWORD  cchReturnLength = 0;
				BOOL gvv = GetVolumePathNamesForVolumeName(
					vol.GetString(),//_In_  LPCTSTR lpszVolumeName,
					nullptr,//_Out_ LPTSTR  lpszVolumePathNames,
					0,//_In_  DWORD   cchBufferLength,
					&cchReturnLength//_Out_ PDWORD  lpcchReturnLength
				);
				if (cchReturnLength < 4) //C:\<null><null>
				{
					ok = FALSE;
					LEAVE_BLOCK(0);
				}


				//if (0 == _tcsicmp(L"Recovery", VolumeNameBuffer))
				//{
				//	ok = FALSE;
				//	LEAVE_BLOCK(0);
				//}

				//////////////////////////////////////////////////////////////////////////
				END_BLOCK(0);
				
				
				return ok;
			};


			return FindAllVolumes(volumes, precit);
		}
	};
}