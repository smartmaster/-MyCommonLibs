#pragma once

#include <Windows.h>
#include <tchar.h>

#include <vector>
#include <string>
#include <algorithm>

#include "CommonDef.h"
#include "StringVersionHelpers.h"
#include "FileTreeOp.h"

namespace SmartLib
{
	class CFindOlderVersions
	{
	private:
		std::vector<std::wstring> m_processedPattern;
		std::vector<std::wstring> m_toKeep; // display purpose
		std::vector<std::wstring> m_toDelete;
		std::wstring m_rootDir;

	private:
		void ProcessSingleFile(const wchar_t * fileName)
		{
			BEGIN_BLOCK(0);

			std::wstring filePattern, fileVersion;
			SmartLib::CStringVersionHelpers::ConvertFileName(fileName, filePattern, fileVersion);
			if (filePattern.size() == 0 || fileVersion.size() == 0)
			{
				m_toKeep.push_back(fileName);
				LEAVE_BLOCK(0);
			}


			auto iter = std::find_if(std::begin(m_processedPattern), std::end(m_processedPattern), [&filePattern](const std::wstring & val) -> bool
			{
				return wcsicmp(val.c_str(), filePattern.c_str()) == 0;
			});
			if (iter != std::end(m_processedPattern))
			{
				LEAVE_BLOCK(0);
			}

			m_processedPattern.push_back(filePattern);

			ProcessSingleFilePattern(filePattern.c_str());
			
			END_BLOCK(0);
			
		}

		void ProcessSingleFilePattern(const wchar_t * filePattern)
		{
			HANDLE hFind = INVALID_HANDLE_VALUE; //to cleanup

			BEGIN_BLOCK(0);

			std::wstring fullPathPattern = m_rootDir + L"\\" + filePattern;
			WIN32_FIND_DATA FindFileData = { 0 };
			hFind = ::FindFirstFile(fullPathPattern.c_str(), &FindFileData); 
			if (INVALID_HANDLE_VALUE == hFind)
			{
				LEAVE_BLOCK(0);
			}

			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				LEAVE_BLOCK(0);
			}


			std::wstring maxVerFile = FindFileData.cFileName;

			while (::FindNextFile(hFind, &FindFileData))
			{
				if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
				{
					continue;
				}

				int cmp = SmartLib::CStringVersionHelpers::CompareFileNameVersion(maxVerFile.c_str(), FindFileData.cFileName);
				if (cmp < 0)
				{
					m_toDelete.push_back(maxVerFile);
					fwprintf_s(stdout, L"To Delete:\t[%s]" L"\r\n", maxVerFile.c_str());
					maxVerFile = FindFileData.cFileName;
				}
				else if (cmp > 0)
				{
					m_toDelete.push_back(FindFileData.cFileName);
					fwprintf_s(stdout, L"To Delete:\t[%s]" L"\r\n", FindFileData.cFileName);
				}
				else
				{
					fwprintf_s(stdout, L"Cannot dertermine, keep both:\t[%s] and [%s]" L"\r\n", maxVerFile.c_str(), FindFileData.cFileName);
				}
			}

			m_toKeep.push_back(maxVerFile);
			fwprintf_s(stdout, L"To Keep:\t[%s]" L"\r\n\r\n", maxVerFile.c_str());

			END_BLOCK(0);

			if (INVALID_HANDLE_VALUE != hFind)
			{
				::FindClose(hFind);
				hFind = INVALID_HANDLE_VALUE;
			}

		}
	public:

		void SetRoot(const wchar_t * rootDir)
		{
			m_rootDir = rootDir;
		}

		void Process()
		{
			HANDLE hFind = INVALID_HANDLE_VALUE; //to cleanup
			
			BEGIN_BLOCK(0);

			WIN32_FIND_DATA FindFileData = { 0 };
			std::wstring fullPath = m_rootDir + L"\\*";
			hFind = ::FindFirstFile(fullPath.c_str(), &FindFileData);
			if (INVALID_HANDLE_VALUE == hFind)
			{
				LEAVE_BLOCK(0);
			}

			do
			{
				if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
				{
					continue;
				}

				if (0 == wcscmp(FindFileData.cFileName, L".") || 0 == wcscmp(FindFileData.cFileName, L".."))
				{
					continue;
				}

				ProcessSingleFile(FindFileData.cFileName);

			} while (::FindNextFile(hFind, &FindFileData));

			END_BLOCK(0);

			if (INVALID_HANDLE_VALUE != hFind)
			{
				::FindClose(hFind);
				hFind = INVALID_HANDLE_VALUE;
			}
		}

		void Cleanup()
		{
			for (const auto & oldFolder : m_toDelete)
			{
				fwprintf_s(stdout, L"Deleting:\t[%s]" L"\r\n", oldFolder.c_str());
				std::wstring dir = m_rootDir + L"\\" + oldFolder;
				RemoveFileInFolder(dir.c_str(), L"*", TRUE, TRUE);

			}

			if (m_toDelete.size() == 0)
			{
				fwprintf_s(stdout, L"No old setup files cleaned up" L"\r\n");
			}
		}

	private:

	};

} //namespace SmartLib