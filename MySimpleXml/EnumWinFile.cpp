#include "stdafx.h"
#include "EnumWinFile.h"


INT EnumDirectoryFileFlat(LPCTSTR szDirectory, LPCTSTR szFileSpec, IEnumFolderFileOperation * pFileOperation)
{
	INT nStatus = 0;
	HANDLE hFindFile = INVALID_HANDLE_VALUE;

	do
	{
		CString cstrFileSpec = szDirectory;
		cstrFileSpec.TrimRight(TEXT("\\/"));
		cstrFileSpec += TEXT('\\');
		cstrFileSpec += szFileSpec;

		WIN32_FIND_DATA FindFileData = { 0 };
		hFindFile = FindFirstFile(
			cstrFileSpec.GetString(),//__in          LPCTSTR lpFileName,
			&FindFileData//__out         LPWIN32_FIND_DATA lpFindFileData
			);
		if (INVALID_HANDLE_VALUE == hFindFile)
		{
			nStatus = -1;
			break;
		}

		do
		{
			if (0 == (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				CString cstrFile = szDirectory;
				cstrFile.TrimRight(TEXT("\\/"));
				cstrFile += TEXT('\\');
				cstrFile += FindFileData.cFileName;
				if (pFileOperation)
				{
					pFileOperation->Operate(cstrFile);
				}
			}

		} while (FindNextFile(hFindFile, &FindFileData));

		FindClose(hFindFile);
		hFindFile = INVALID_HANDLE_VALUE;

	} while (FALSE);

	return nStatus;
}

INT EnumDirectoryFileTree(LPCTSTR szDirectory, LPCTSTR szFileSpec, IEnumFolderFileOperation * pFolderOperation, IEnumFolderFileOperation * pFileOperation)
{
	INT nStatus = 0;
	HANDLE hFindFile = INVALID_HANDLE_VALUE;

	nStatus = EnumDirectoryFileFlat(szDirectory, szFileSpec, pFileOperation);

	do
	{
		CString cstrSubDirectorySpec = szDirectory;
		cstrSubDirectorySpec.TrimRight(TEXT("\\/"));
		cstrSubDirectorySpec += TEXT("\\*");

		WIN32_FIND_DATA FindFileData = { 0 };
		hFindFile = FindFirstFile(
			cstrSubDirectorySpec.GetString(),//__in          LPCTSTR lpFileName,
			&FindFileData//__out         LPWIN32_FIND_DATA lpFindFileData
			);
		if (INVALID_HANDLE_VALUE == hFindFile)
		{
			nStatus = -1;
			break;
		}

		do
		{
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
				_tcsicmp(FindFileData.cFileName, TEXT(".")) &&
				_tcsicmp(FindFileData.cFileName, TEXT(".."))
				)
			{
				CString cstrSubDirectory = szDirectory;
				cstrSubDirectory.TrimRight(TEXT("\\/"));
				cstrSubDirectory += TEXT('\\');
				cstrSubDirectory += FindFileData.cFileName;
				EnumDirectoryFileTree(cstrSubDirectory.GetString(), szFileSpec, pFolderOperation, pFileOperation);
				if (pFolderOperation)
				{
					pFolderOperation->Operate(cstrSubDirectory);
				}
			}

		} while (FindNextFile(hFindFile, &FindFileData));

		FindClose(hFindFile);
		hFindFile = INVALID_HANDLE_VALUE;

	} while (FALSE);

	return nStatus;
}
