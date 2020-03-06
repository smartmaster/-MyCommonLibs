#include "stdafx.h"
#include "VolumeHelper.h"
#include "MiscHelper.h"

INT CVolumeHelper::GetAllVolumeGuid( vector<CString> & VolumeGuid )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	HANDLE hFindFirstVolume = INVALID_HANDLE_VALUE;
	do 
	{
		//******************************************************************************
		TCHAR szBuffer[1024] = {0};
		hFindFirstVolume = FindFirstVolume(
			szBuffer,//__out  LPTSTR lpszVolumeName,
			_countof(szBuffer)//__in   DWORD cchBufferLength
			);
		if (INVALID_HANDLE_VALUE == hFindFirstVolume)
		{
			dwLastError = GetLastError(); _OLD_DSETLASTSTATUS(dwLastError);
			nStatus = dwLastError;
			_OLD_DAPIERR((0, DRNPOS, TEXT("FindFirstVolume"), dwLastError));
			break;
		}

		//******************************************************************************
		do 
		{
			VolumeGuid.push_back(szBuffer);
			szBuffer[0] = 0;
		} while (FindNextVolume(
			hFindFirstVolume,//__in   HANDLE hFindVolume,
			szBuffer,//__out  LPTSTR lpszVolumeName,
			_countof(szBuffer)//__in   DWORD cchBufferLength
			));

	} while (FALSE);

	//******************************************************************************
	if (INVALID_HANDLE_VALUE != hFindFirstVolume)
	{
		FindVolumeClose(hFindFirstVolume);
		hFindFirstVolume = INVALID_HANDLE_VALUE;
	}

	return nStatus;
}

INT CVolumeHelper::GetVolumePathFromGuid( LPCTSTR szVolumeGuid, vector<CString> & Path )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		if (NULL == szVolumeGuid)
		{
			nStatus = E_INVALIDARG;
			_OLD_DAPIERR((0, DRNPOS, TEXT(__FUNCTION__), E_INVALIDARG));
			break;
		}

		//******************************************************************************
		DWORD cchReturnLength = 0;
		BOOL bGetVolumePathNamesForVolumeName = GetVolumePathNamesForVolumeName(
			szVolumeGuid,//__in   LPCTSTR lpszVolumeName,
			NULL,//__out  LPTSTR lpszVolumePathNames,
			0,//__in   DWORD cchBufferLength,
			&cchReturnLength//__out  PDWORD lpcchReturnLength
			);
		if (!bGetVolumePathNamesForVolumeName)
		{
			dwLastError = GetLastError(); _OLD_DSETLASTSTATUS(dwLastError);
			if (ERROR_MORE_DATA != dwLastError || 0 == cchReturnLength)
			{
				_OLD_DAPIERR((999, DRNPOS, TEXT("GetVolumePathNamesForVolumeName"), dwLastError));
				nStatus = dwLastError;
				break;
			}

			//******************************************************************************
			CHeapPtr<TCHAR> apBuffer;
			apBuffer.Allocate(cchReturnLength);
			ZeroMemory((TCHAR*)apBuffer, sizeof(TCHAR)*cchReturnLength);
			bGetVolumePathNamesForVolumeName = GetVolumePathNamesForVolumeName(
				szVolumeGuid,//__in   LPCTSTR lpszVolumeName,
				(TCHAR*)apBuffer,//__out  LPTSTR lpszVolumePathNames,
				cchReturnLength,//__in   DWORD cchBufferLength,
				&cchReturnLength//__out  PDWORD lpcchReturnLength
				);
			if (!bGetVolumePathNamesForVolumeName)
			{
				dwLastError = GetLastError(); _OLD_DSETLASTSTATUS(dwLastError);
				_OLD_DAPIERR((0, DRNPOS, TEXT("GetVolumePathNamesForVolumeName"), dwLastError));
				nStatus = dwLastError;
				break;
			}

			//******************************************************************************
			nStatus = CMiscHelper::MultiStringToStringVector((TCHAR*)apBuffer, cchReturnLength, Path);
			if (nStatus)
			{
				_OLD_DINFO((0, DRNPOS, TEXT("CUtilityHelper::MultiStringToStringVector [ERROR]")));
				break;
			}
		}

	} while (FALSE);

	return nStatus;
}

INT CVolumeHelper::GetVolumeRootAndGuidFromPath( LPCTSTR szPath, CString & strRoot, CString & strGuid )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		if (NULL == szPath)
		{
			nStatus = dwLastError;
			_OLD_DAPIERR((0, DRNPOS, TEXT(__FUNCTION__), E_INVALIDARG));
			break;
		}

		//******************************************************************************
		TCHAR szBuffer[1024] = {0};
		BOOL bGetVolumePathName = GetVolumePathName(
			szPath,//__in   LPCTSTR lpszFileName,
			szBuffer,//__out  LPTSTR lpszVolumePathName,
			_countof(szBuffer)//__in   DWORD cchBufferLength
			);
		if (!bGetVolumePathName)
		{
			dwLastError = GetLastError(); _OLD_DSETLASTSTATUS(dwLastError);
			_OLD_DAPIERR((0, DRNPOS, TEXT("GetVolumePathName"), dwLastError));
			nStatus = dwLastError;
			break;
		}
		strRoot = szBuffer;

		//******************************************************************************
		szBuffer[0] = 0;
		BOOL bGetVolumeNameForVolumeMountPoint = GetVolumeNameForVolumeMountPoint(
			strRoot.GetString(),//__in   LPCTSTR lpszVolumeMountPoint,
			szBuffer,//__out  LPTSTR lpszVolumeName,
			_countof(szBuffer)//__in   DWORD cchBufferLength
			);
		if (!bGetVolumeNameForVolumeMountPoint)
		{
			dwLastError = GetLastError(); _OLD_DSETLASTSTATUS(dwLastError);
			_OLD_DAPIERR((0, DRNPOS, TEXT("GetVolumeNameForVolumeMountPoint"), dwLastError));
			nStatus = dwLastError;
			break;
		}
		strGuid = szBuffer;

		//******************************************************************************
	} while (FALSE);

	return nStatus;
}

INT CVolumeHelper::GetVolumeMountPoint( LPCTSTR szRootPathName, vector<CString> & MountPoint )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	HANDLE hFindFirstVolumeMountPoint = INVALID_HANDLE_VALUE;
	do 
	{
		//******************************************************************************
		TCHAR szBuffer[1024] = {0};
		hFindFirstVolumeMountPoint = FindFirstVolumeMountPoint(
			szRootPathName,
			szBuffer,//__out  LPTSTR lpszVolumeName,
			_countof(szBuffer)//__in   DWORD cchBufferLength
			);
		if (INVALID_HANDLE_VALUE == hFindFirstVolumeMountPoint)
		{
			
			dwLastError = GetLastError(); _OLD_DSETLASTSTATUS(dwLastError);
			nStatus = dwLastError;
			_OLD_DAPIERR((999, DRNPOS, TEXT("FindFirstVolume"), dwLastError));
			break;
		}

		//******************************************************************************
		do 
		{
			MountPoint.push_back(szBuffer);
			szBuffer[0] = 0;
		} while (FindNextVolumeMountPoint(
			hFindFirstVolumeMountPoint,//__in   HANDLE hFindVolume,
			szBuffer,//__out  LPTSTR lpszVolumeName,
			_countof(szBuffer)//__in   DWORD cchBufferLength
			));

	} while (FALSE);

	//******************************************************************************
	if (INVALID_HANDLE_VALUE != hFindFirstVolumeMountPoint)
	{
		FindVolumeClose(hFindFirstVolumeMountPoint);
		hFindFirstVolumeMountPoint = INVALID_HANDLE_VALUE;
	}

	return nStatus;
}

INT CVolumeHelper::GetVolumeInfo( LPCTSTR lpRootPathName, CVolumeInfomation & volumeInfo )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		if (NULL == lpRootPathName)
		{
			nStatus = E_INVALIDARG;
			_OLD_DAPIERR((0, DRNPOS, TEXT(__FUNCTION__), E_INVALIDARG));
			break;
		}
		//******************************************************************************
		TCHAR szVolumeNameBuffer[256] = {0};
		DWORD dwVolumeSerialNumber = 0;
		DWORD dwMaximumComponentLength = 0;
		DWORD dwFileSystemFlags = 0;
		TCHAR szFileSystemNameBuffer[256] = {0};
		BOOL bGetVolumeInformation = GetVolumeInformation(
			lpRootPathName,//__in_opt   LPCTSTR lpRootPathName,
			szVolumeNameBuffer,//__out      LPTSTR lpVolumeNameBuffer,
			_countof(szVolumeNameBuffer),//__in       DWORD nVolumeNameSize,
			&dwVolumeSerialNumber,//__out_opt  LPDWORD lpVolumeSerialNumber,
			&dwMaximumComponentLength,//__out_opt  LPDWORD lpMaximumComponentLength,
			&dwFileSystemFlags,//__out_opt  LPDWORD lpFileSystemFlags,
			szFileSystemNameBuffer,//__out      LPTSTR lpFileSystemNameBuffer,
			_countof(szFileSystemNameBuffer)//__in       DWORD nFileSystemNameSize
			);
		if (!bGetVolumeInformation)
		{
			dwLastError = GetLastError(); _OLD_DSETLASTSTATUS(dwLastError);
			_OLD_DAPIERR((999, DRNPOS, TEXT("GetVolumeInformation"), dwLastError));
			nStatus = dwLastError;
			break;
		}

		volumeInfo.m_VolumeNameBuffer = szVolumeNameBuffer;
		volumeInfo.m_VolumeSerialNumber = dwVolumeSerialNumber;
		volumeInfo.m_MaximumComponentLength = dwMaximumComponentLength;
		volumeInfo.m_FileSystemFlags = dwFileSystemFlags;
		volumeInfo.m_FileSystemNameBuffer = szFileSystemNameBuffer;

	} while (FALSE);

	return nStatus;
}

HANDLE CVolumeHelper::CreateHandle( LPCTSTR szVolume, DWORD dwDesiredAccess )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
	HANDLE hDevice = INVALID_HANDLE_VALUE;

	do 
	{
		//******************************************************************************
		if (NULL == szVolume)
		{
			nStatus = E_INVALIDARG;
			_OLD_DAPIERR((0, DRNPOS, TEXT(__FUNCTION__), E_INVALIDARG));
			break;
		}

		//******************************************************************************
		CString strVolume = szVolume;
		strVolume.TrimRight(TEXT("\\/"));
		hDevice = CreateFile(
			strVolume.GetString(),//__in      LPCTSTR lpFileName,
			dwDesiredAccess,//__in      DWORD dwDesiredAccess,
			FILE_SHARE_READ | FILE_SHARE_WRITE,//__in      DWORD dwShareMode,
			NULL,//__in_opt  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			OPEN_EXISTING,//__in      DWORD dwCreationDisposition,
			0,//__in      DWORD dwFlagsAndAttributes,
			NULL//__in_opt  HANDLE hTemplateFile
			);
		if (INVALID_HANDLE_VALUE == hDevice)
		{
			dwLastError = GetLastError(); _OLD_DSETLASTSTATUS(dwLastError);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateFile"), dwLastError, szVolume));
			nStatus = dwLastError;
			break;
		}

	} while (FALSE);

	return hDevice;
}

INT CVolumeHelper::FSCTL_GET_NTFS_VOLUME_DATA_Helper(HANDLE hDevice, NTFS_VOLUME_DATA_BUFFER & NtfsVolumeDataBuffer )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		if (NULL == hDevice || INVALID_HANDLE_VALUE == hDevice)
		{
			nStatus = dwLastError;
			_OLD_DAPIERR((0, DRNPOS, TEXT(__FUNCTION__), E_INVALIDARG));
			break;
		}

		//******************************************************************************
		DWORD dwBytesReturned = 0;
		BOOL bDeviceIoControl = DeviceIoControl(
			hDevice,//(HANDLE) hDevice,           // handle to device
			FSCTL_GET_NTFS_VOLUME_DATA,//FSCTL_GET_NTFS_VOLUME_DATA, // dwIoControlCode
			NULL,//(LPVOID) lpInBuffer,        // lpInBuffer
			0,//(DWORD) nInBufferSize,      // size of input buffer
			&NtfsVolumeDataBuffer,//(LPVOID) lpOutBuffer,       // output buffer
			sizeof(NTFS_VOLUME_DATA_BUFFER),//(DWORD) nOutBufferSize,     // size of output buffer
			&dwBytesReturned,//(LPDWORD) lpBytesReturned,  // number of bytes returned
			NULL//(LPOVERLAPPED) lpOverlapped // OVERLAPPED structure
			);
		if (!bDeviceIoControl)
		{
			dwLastError = GetLastError(); _OLD_DSETLASTSTATUS(dwLastError);
			_OLD_DAPIERR((0, DRNPOS, TEXT("FSCTL_GET_NTFS_VOLUME_DATA"), dwLastError));
			nStatus = dwLastError;
			break;
		}

	} while (FALSE);

	return nStatus;
}

INT CVolumeHelper::FSCTL_GET_RETRIEVAL_POINTER_BASE_Helper( HANDLE hDevice, RETRIEVAL_POINTER_BASE & RetrievalPointerBase )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		if (NULL == hDevice || INVALID_HANDLE_VALUE == hDevice)
		{
			nStatus = E_INVALIDARG;
			_OLD_DAPIERR((0, DRNPOS, TEXT(__FUNCTION__), E_INVALIDARG));
			break;
		}

		//******************************************************************************
		DWORD dwBytesReturned = 0;
		BOOL bDeviceIoControl = DeviceIoControl(
			hDevice,//(HANDLE) hDevice,           // handle to device
			FSCTL_GET_RETRIEVAL_POINTER_BASE,//FSCTL_GET_RETRIEVAL_POINTER_BASE, // dwIoControlCode
			NULL,//(LPVOID) lpInBuffer,        // lpInBuffer
			0,//(DWORD) nInBufferSize,      // size of input buffer
			&RetrievalPointerBase,//(LPVOID) lpOutBuffer,       // output buffer
			sizeof(RETRIEVAL_POINTER_BASE),//(DWORD) nOutBufferSize,     // size of output buffer
			&dwBytesReturned,//(LPDWORD) lpBytesReturned,  // number of bytes returned
			NULL//(LPOVERLAPPED) lpOverlapped // OVERLAPPED structure
			);
		if (!bDeviceIoControl)
		{
			dwLastError = GetLastError(); _OLD_DSETLASTSTATUS(dwLastError);
			_OLD_DAPIERR((0, DRNPOS, TEXT("FSCTL_GET_RETRIEVAL_POINTER_BASE"), dwLastError));
			nStatus = dwLastError;
			break;
		}

	} while (FALSE);

	return nStatus;
}

INT CVolumeHelper::FSCTL_GET_RETRIEVAL_POINTERS_Helper( HANDLE hDevice, RETRIEVAL_POINTERS_BUFFER ** ppRetrievalPointersBuffer )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		if (NULL == hDevice || INVALID_HANDLE_VALUE == hDevice)
		{
			nStatus = E_INVALIDARG;
			_OLD_DAPIERR((0, DRNPOS, TEXT(__FUNCTION__), E_INVALIDARG));
			break;
		}

		//******************************************************************************
		STARTING_VCN_INPUT_BUFFER StartingVcn = {0};
		ZeroMemory(&StartingVcn, sizeof(STARTING_VCN_INPUT_BUFFER));
		CONST INT DELTA = 128 * (sizeof(RETRIEVAL_POINTERS_BUFFER) - FIELD_OFFSET(RETRIEVAL_POINTERS_BUFFER, Extents));
		DWORD dwBufferSize = sizeof(RETRIEVAL_POINTERS_BUFFER) +DELTA;
		CHeapPtr<BYTE> apBuffer;
		apBuffer.Allocate(dwBufferSize);
		ZeroMemory((BYTE*)apBuffer, dwBufferSize);
		do 
		{
			DWORD dwBytesReturned = 0;

			BOOL bDeviceIoControl = DeviceIoControl(
				hDevice,//(HANDLE) hDevice,              // handle to volume
				FSCTL_GET_RETRIEVAL_POINTERS,//FSCTL_GET_RETRIEVAL_POINTERS,  // dwIoControlCode
				&StartingVcn,//(LPVOID) lpInBuffer,           // input buffer
				sizeof(STARTING_VCN_INPUT_BUFFER),//(DWORD) nInBufferSize,         // size of input buffer
				(BYTE*)apBuffer,//(LPVOID) lpOutBuffer,          // output buffer
				dwBufferSize,//(DWORD) nOutBufferSize,        // size of output buffer
				&dwBytesReturned,//(LPDWORD) lpBytesReturned,     // number of bytes returned
				NULL//(LPOVERLAPPED) lpOverlapped  // OVERLAPPED structure
				);
			if (bDeviceIoControl)
			{
				*ppRetrievalPointersBuffer = (RETRIEVAL_POINTERS_BUFFER*)(apBuffer.Detach());
				break;
			}
			else
			{
				dwLastError = GetLastError(); _OLD_DSETLASTSTATUS(dwLastError);
				if (ERROR_MORE_DATA != dwLastError)
				{
					_OLD_DAPIERR((0, DRNPOS, TEXT("FSCTL_GET_RETRIEVAL_POINTERS"), dwLastError));
					nStatus = dwLastError;
					break;
				}

				dwBufferSize += DELTA;
				apBuffer.Reallocate(dwBufferSize);
				ZeroMemory((BYTE*)apBuffer, dwBufferSize);
			}

		} while (TRUE);
		if (nStatus)
		{
			break;
		}

	} while (FALSE);

	return nStatus;
}

INT CVolumeHelper::FSCTL_GET_VOLUME_BITMAP_Helper( HANDLE hDevice, VOLUME_BITMAP_BUFFER ** ppVolumeBitmapBuffer )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		if (NULL == hDevice || INVALID_HANDLE_VALUE == hDevice)
		{
			nStatus = dwLastError;
			_OLD_DAPIERR((0, DRNPOS, TEXT(__FUNCTION__), E_INVALIDARG));
			break;
		}

		//******************************************************************************
		STARTING_VCN_INPUT_BUFFER StartingVcn = {0};
		ZeroMemory(&StartingVcn, sizeof(STARTING_VCN_INPUT_BUFFER));
		CONST INT DELTA = 16*1024*1024; //512GB if cluster size is 4KB
		DWORD dwBufferSize = sizeof(VOLUME_BITMAP_BUFFER) + DELTA;
		CHeapPtr<BYTE> apBuffer;
		apBuffer.Allocate(dwBufferSize);
		ZeroMemory((BYTE*)apBuffer, dwBufferSize);
		do 
		{
			DWORD dwBytesReturned = 0;

			BOOL bDeviceIoControl = DeviceIoControl(
				hDevice,//(HANDLE) hDevice,              // handle to volume
				FSCTL_GET_VOLUME_BITMAP,//FSCTL_GET_RETRIEVAL_POINTERS,  // dwIoControlCode
				&StartingVcn,//(LPVOID) lpInBuffer,           // input buffer
				sizeof(STARTING_VCN_INPUT_BUFFER),//(DWORD) nInBufferSize,         // size of input buffer
				(BYTE*)apBuffer,//(LPVOID) lpOutBuffer,          // output buffer
				dwBufferSize,//(DWORD) nOutBufferSize,        // size of output buffer
				&dwBytesReturned,//(LPDWORD) lpBytesReturned,     // number of bytes returned
				NULL//(LPOVERLAPPED) lpOverlapped  // OVERLAPPED structure
				);
			if (bDeviceIoControl)
			{
				*ppVolumeBitmapBuffer = (VOLUME_BITMAP_BUFFER*)(apBuffer.Detach());
				break;
			}
			else
			{
				dwLastError = GetLastError(); _OLD_DSETLASTSTATUS(dwLastError);
				if (ERROR_MORE_DATA != dwLastError)
				{
					_OLD_DAPIERR((0, DRNPOS, TEXT("FSCTL_GET_VOLUME_BITMAP"), dwLastError));
					nStatus = dwLastError;
					break;
				}

				dwBufferSize += DELTA;
				apBuffer.Reallocate(dwBufferSize);
				ZeroMemory((BYTE*)apBuffer, dwBufferSize);
			}

		} while (TRUE);
		if (nStatus)
		{
			break;
		}

	} while (FALSE);

	return nStatus;
}

INT CVolumeHelper::FSCTL_QUERY_FILE_SYSTEM_RECOGNITION_Helper( HANDLE hDevice, FILE_SYSTEM_RECOGNITION_INFORMATION & FileSystemRecognitionInformation )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		if (NULL == hDevice || INVALID_HANDLE_VALUE == hDevice)
		{
			nStatus = dwLastError;
			_OLD_DAPIERR((0, DRNPOS, TEXT(__FUNCTION__), E_INVALIDARG));
			break;
		}

		//******************************************************************************
		DWORD dwBytesReturned = 0;
		BOOL bDeviceIoControl = DeviceIoControl(
			hDevice,//(HANDLE) hDevice,           // handle to device
			FSCTL_QUERY_FILE_SYSTEM_RECOGNITION,//FSCTL_GET_NTFS_VOLUME_DATA, // dwIoControlCode
			NULL,//(LPVOID) lpInBuffer,        // lpInBuffer
			0,//(DWORD) nInBufferSize,      // size of input buffer
			&FileSystemRecognitionInformation,//(LPVOID) lpOutBuffer,       // output buffer
			sizeof(FILE_SYSTEM_RECOGNITION_INFORMATION),//(DWORD) nOutBufferSize,     // size of output buffer
			&dwBytesReturned,//(LPDWORD) lpBytesReturned,  // number of bytes returned
			NULL//(LPOVERLAPPED) lpOverlapped // OVERLAPPED structure
			);
		if (!bDeviceIoControl)
		{
			dwLastError = GetLastError(); _OLD_DSETLASTSTATUS(dwLastError);
			_OLD_DAPIERR((0, DRNPOS, TEXT("FSCTL_QUERY_FILE_SYSTEM_RECOGNITION"), dwLastError));
			nStatus = dwLastError;
			break;
		}

	} while (FALSE);

	return nStatus;
}

INT CVolumeHelper::IOCTL_VOLUME_GET_GPT_ATTRIBUTES_Helper( HANDLE hDevice, VOLUME_GET_GPT_ATTRIBUTES_INFORMATION & GptAttributes )
{
	//DSTART(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		if (NULL == hDevice || INVALID_HANDLE_VALUE == hDevice)
		{
			nStatus = dwLastError;
			_OLD_DAPIERR((0, DRNPOS, TEXT(__FUNCTION__), E_INVALIDARG));
			break;
		}

		//******************************************************************************
		DWORD dwBytesReturned = 0;
		BOOL bDeviceIoControl = DeviceIoControl(
			hDevice,//(HANDLE) hDevice,           // handle to device
			IOCTL_VOLUME_GET_GPT_ATTRIBUTES,//FSCTL_GET_NTFS_VOLUME_DATA, // dwIoControlCode
			NULL,//(LPVOID) lpInBuffer,        // lpInBuffer
			0,//(DWORD) nInBufferSize,      // size of input buffer
			&GptAttributes,//(LPVOID) lpOutBuffer,       // output buffer
			sizeof(VOLUME_GET_GPT_ATTRIBUTES_INFORMATION),//(DWORD) nOutBufferSize,     // size of output buffer
			&dwBytesReturned,//(LPDWORD) lpBytesReturned,  // number of bytes returned
			NULL//(LPOVERLAPPED) lpOverlapped // OVERLAPPED structure
			);
		if (!bDeviceIoControl)
		{
			dwLastError = GetLastError(); _OLD_DSETLASTSTATUS(dwLastError);
			_OLD_DAPIERR((0, DRNPOS, TEXT("IOCTL_VOLUME_GET_GPT_ATTRIBUTES"), dwLastError));
			nStatus = dwLastError;
			break;
		}

	} while (FALSE);

	return nStatus;
}

INT CVolumeHelper::IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS_Helper( HANDLE hDevice, VOLUME_DISK_EXTENTS ** ppVolumeDiskExtents )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		if (NULL == hDevice || INVALID_HANDLE_VALUE == hDevice)
		{
			nStatus = dwLastError;
			_OLD_DAPIERR((0, DRNPOS, TEXT(__FUNCTION__), E_INVALIDARG));
			break;
		}

		//******************************************************************************
		CONST INT DELTA = sizeof(DISK_EXTENT);
		DWORD dwBufferSize = sizeof(VOLUME_DISK_EXTENTS);
		CHeapPtr<BYTE> apBuffer;
		apBuffer.Allocate(dwBufferSize);
		ZeroMemory((BYTE*)apBuffer, dwBufferSize);
		do 
		{
			DWORD dwBytesReturned = 0;

			BOOL bDeviceIoControl = DeviceIoControl(
				hDevice,//(HANDLE) hDevice,              // handle to volume
				IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,//FSCTL_GET_RETRIEVAL_POINTERS,  // dwIoControlCode
				NULL,//(LPVOID) lpInBuffer,           // input buffer
				0,//(DWORD) nInBufferSize,         // size of input buffer
				(BYTE*)apBuffer,//(LPVOID) lpOutBuffer,          // output buffer
				dwBufferSize,//(DWORD) nOutBufferSize,        // size of output buffer
				&dwBytesReturned,//(LPDWORD) lpBytesReturned,     // number of bytes returned
				NULL//(LPOVERLAPPED) lpOverlapped  // OVERLAPPED structure
				);
			if (bDeviceIoControl)
			{
				*ppVolumeDiskExtents = (VOLUME_DISK_EXTENTS*)(apBuffer.Detach());
				break;
			}
			else
			{
				dwLastError = GetLastError(); _OLD_DSETLASTSTATUS(dwLastError);
				if (ERROR_MORE_DATA != dwLastError)
				{
					_OLD_DAPIERR((999, DRNPOS, TEXT("IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS"), dwLastError));
					nStatus = dwLastError;
					break;
				}

				dwBufferSize += DELTA;
				apBuffer.Reallocate(dwBufferSize);
				ZeroMemory((BYTE*)apBuffer, dwBufferSize);
			}

		} while (TRUE);
		if (nStatus)
		{
			break;
		}

		//******************************************************************************
	} while (FALSE);

	return nStatus;
}

INT CVolumeHelper::IOCTL_VOLUME_LOGICAL_TO_PHYSICAL_Helper( HANDLE hDevice, VOLUME_LOGICAL_OFFSET & VolumeLogicalOffset, VOLUME_PHYSICAL_OFFSETS ** ppVolumePhysicalOffsets )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		if (NULL == hDevice || INVALID_HANDLE_VALUE == hDevice)
		{
			nStatus = dwLastError;
			_OLD_DAPIERR((0, DRNPOS, TEXT(__FUNCTION__), E_INVALIDARG));
			break;
		}

		//******************************************************************************
		CONST INT DELTA = sizeof(VOLUME_PHYSICAL_OFFSET);
		DWORD dwBufferSize = sizeof(VOLUME_PHYSICAL_OFFSETS);
		CHeapPtr<BYTE> apBuffer;
		apBuffer.Allocate(dwBufferSize);
		ZeroMemory((BYTE*)apBuffer, dwBufferSize);

		do 
		{
			DWORD dwBytesReturned = 0;

			BOOL bDeviceIoControl = DeviceIoControl(
				hDevice,//(HANDLE) hDevice,              // handle to volume
				IOCTL_VOLUME_LOGICAL_TO_PHYSICAL,//IOCTL_VOLUME_LOGICAL_TO_PHYSICAL,  // dwIoControlCode
				&VolumeLogicalOffset,//(LPVOID) lpInBuffer,           // input buffer
				sizeof(VOLUME_LOGICAL_OFFSET),//(DWORD) nInBufferSize,         // size of input buffer
				(BYTE*)apBuffer,//(LPVOID) lpOutBuffer,          // output buffer
				dwBufferSize,//(DWORD) nOutBufferSize,        // size of output buffer
				&dwBytesReturned,//(LPDWORD) lpBytesReturned,     // number of bytes returned
				NULL//(LPOVERLAPPED) lpOverlapped  // OVERLAPPED structure
				);
			if (bDeviceIoControl)
			{
				*ppVolumePhysicalOffsets = (VOLUME_PHYSICAL_OFFSETS*)(apBuffer.Detach());
				break;
			}
			else
			{
				dwLastError = GetLastError(); _OLD_DSETLASTSTATUS(dwLastError);
				if (ERROR_MORE_DATA != dwLastError)
				{
					_OLD_DAPIERR((0, DRNPOS, TEXT("IOCTL_VOLUME_LOGICAL_TO_PHYSICAL"), dwLastError));
					nStatus = dwLastError;
					break;
				}

				dwBufferSize += DELTA;
				apBuffer.Reallocate(dwBufferSize);
				ZeroMemory((BYTE*)apBuffer, dwBufferSize);
			}

		} while (TRUE);
		if (nStatus)
		{
			break;
		}

		//******************************************************************************
	} while (FALSE);


	return nStatus;
}

INT CVolumeHelper::IOCTL_VOLUME_PHYSICAL_TO_LOGICAL_Helper( HANDLE hDevice, VOLUME_PHYSICAL_OFFSET & VolumePhysicalOffset, VOLUME_LOGICAL_OFFSET & VolumeLogicalOffset )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		if (NULL == hDevice || INVALID_HANDLE_VALUE == hDevice)
		{
			nStatus = dwLastError;
			_OLD_DAPIERR((0, DRNPOS, TEXT(__FUNCTION__), E_INVALIDARG));
			break;
		}

		//******************************************************************************
		DWORD dwBytesReturned = 0;
		BOOL bDeviceIoControl = DeviceIoControl(
			hDevice,//(HANDLE) hDevice,           // handle to device
			IOCTL_VOLUME_PHYSICAL_TO_LOGICAL ,//FSCTL_GET_NTFS_VOLUME_DATA, // dwIoControlCode
			&VolumePhysicalOffset,//(LPVOID) lpInBuffer,        // lpInBuffer
			sizeof(VOLUME_PHYSICAL_OFFSET),//(DWORD) nInBufferSize,      // size of input buffer
			&VolumeLogicalOffset,//(LPVOID) lpOutBuffer,       // output buffer
			sizeof(VOLUME_LOGICAL_OFFSET),//(DWORD) nOutBufferSize,     // size of output buffer
			&dwBytesReturned,//(LPDWORD) lpBytesReturned,  // number of bytes returned
			NULL//(LPOVERLAPPED) lpOverlapped // OVERLAPPED structure
			);
		if (!bDeviceIoControl)
		{
			dwLastError = GetLastError(); _OLD_DSETLASTSTATUS(dwLastError);
			_OLD_DAPIERR((0, DRNPOS, TEXT("IOCTL_VOLUME_PHYSICAL_TO_LOGICAL"), dwLastError));
			nStatus = dwLastError;
			break;
		}

	} while (FALSE);

	return nStatus;	
}

INT TestVolume()
{
	INT nStatus = 0;

	do 
	{
		//********************************************
		vector<CString> VolumeGuid;
		nStatus = CVolumeHelper::GetAllVolumeGuid(VolumeGuid);
		if (nStatus)
		{
			break;
		}

		//********************************************
		for (size_t i=0; i<VolumeGuid.size(); ++i)
		{
			//********************************************
			vector<CString> Path;
			CVolumeHelper::GetVolumePathFromGuid(VolumeGuid[i], Path);

			//********************************************
			for (size_t j=0; j<Path.size(); ++j)
			{
				CString strRoot, strGuid;
				CVolumeHelper::GetVolumeRootAndGuidFromPath(Path[j], strRoot, strGuid);
			}

			//********************************************
			vector<CString> MountPoint;
			CVolumeHelper::GetVolumeMountPoint(VolumeGuid[i], MountPoint);

			//********************************************
			CVolumeHelper::CVolumeInfomation volumeInfo;
			CVolumeHelper::GetVolumeInfo(VolumeGuid[i], volumeInfo);

			//********************************************
			HANDLE hDeviceRaw = CVolumeHelper::CreateHandle(VolumeGuid[i], GENERIC_READ);
			if (INVALID_HANDLE_VALUE != hDeviceRaw)
			{
				CHandle hDevice(hDeviceRaw);
				hDeviceRaw = INVALID_HANDLE_VALUE;

				//********************************************
				NTFS_VOLUME_DATA_BUFFER NtfsVolumeDataBuffer;
				CVolumeHelper::FSCTL_GET_NTFS_VOLUME_DATA_Helper(hDevice, NtfsVolumeDataBuffer);

				//********************************************
				RETRIEVAL_POINTERS_BUFFER * pRetrievalPointersBuffer = NULL;
				CVolumeHelper::FSCTL_GET_RETRIEVAL_POINTERS_Helper(hDevice, &pRetrievalPointersBuffer);
				{
					CHeapPtr<BYTE> apBufferTemp((BYTE*)pRetrievalPointersBuffer);
					pRetrievalPointersBuffer = NULL;
				}

				//********************************************
				VOLUME_BITMAP_BUFFER * pVolumeBitmapBuffer = NULL;
				CVolumeHelper::FSCTL_GET_VOLUME_BITMAP_Helper(hDevice, &pVolumeBitmapBuffer);
				{
					CHeapPtr<BYTE> apBufferTemp((BYTE*)pVolumeBitmapBuffer);
					pVolumeBitmapBuffer = NULL;
				}

				//********************************************
				VOLUME_DISK_EXTENTS * pVolumeDiskExtents = NULL;
				CVolumeHelper::IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS_Helper(hDevice, &pVolumeDiskExtents);
				{
					CHeapPtr<BYTE> apBufferTemp((BYTE*)pVolumeDiskExtents);
					pVolumeDiskExtents = NULL;
				}

				//********************************************
				VOLUME_LOGICAL_OFFSET VolumeLogicalOffset = {0};
				VOLUME_PHYSICAL_OFFSETS * pVolumePhysicalOffsets = NULL;
				CVolumeHelper::IOCTL_VOLUME_LOGICAL_TO_PHYSICAL_Helper( hDevice, VolumeLogicalOffset, &pVolumePhysicalOffsets);
				
				if (pVolumePhysicalOffsets) 
				{
					VolumeLogicalOffset.LogicalOffset = (LONGLONG)(-1);
					CVolumeHelper::IOCTL_VOLUME_PHYSICAL_TO_LOGICAL_Helper(hDevice, pVolumePhysicalOffsets->PhysicalOffset[0], VolumeLogicalOffset);
				}
				{
					CHeapPtr<BYTE> apBufferTemp((BYTE*)pVolumePhysicalOffsets);
					pVolumePhysicalOffsets = NULL;
				}

				//********************************************
				RETRIEVAL_POINTER_BASE RetrievalPointerBase = {0};
				CVolumeHelper::FSCTL_GET_RETRIEVAL_POINTER_BASE_Helper(hDevice, RetrievalPointerBase);

				//********************************************
				FILE_SYSTEM_RECOGNITION_INFORMATION FileSystemRecognitionInformation = {0};
				CVolumeHelper::FSCTL_QUERY_FILE_SYSTEM_RECOGNITION_Helper(hDevice, FileSystemRecognitionInformation);

				//********************************************
				VOLUME_GET_GPT_ATTRIBUTES_INFORMATION GptAttributes = {0};
				CVolumeHelper::IOCTL_VOLUME_GET_GPT_ATTRIBUTES_Helper(hDevice, GptAttributes);
			}
		}


	} while (FALSE);

	return nStatus;
}
