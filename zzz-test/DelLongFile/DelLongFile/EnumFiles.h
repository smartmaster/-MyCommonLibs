#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

namespace SmartLib
{

	class ILogger
	{
	public:
		virtual VOID Log(int level, LPCTSTR message, ...) = 0;
	};

	class CConsoleLogger : public ILogger
	{
	public:
		virtual VOID Log(int level, LPCTSTR message, ...) override
		{
			va_list valist;
			va_start(valist, message);
			_vftprintf_s(stdout, message, valist);
			_ftprintf_s(stdout, TEXT("\r\n"));
			va_end(valist);
		}
	};

	class StaticLoggerFactory
	{
	public:
		static ILogger * GetSingleton()
		{
			static CConsoleLogger s_logger;
			return &s_logger;
		}
	};

	class IFileFolderOperation
	{
	public:
		virtual HRESULT Do(LPCTSTR path) = 0;
	};

	class StaticHelper
	{
	public:
		static HRESULT RemoveRHS(LPCTSTR path)
		{
			DWORD dwAttr = GetFileAttributes(path);
			dwAttr &= ~(FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
			SetFileAttributes(path, dwAttr);

			return S_OK;
		}
	};


	class  CFolderOperation : public IFileFolderOperation
	{
	private:
		int m_count{ 0 };
		int m_failedCount{ 0 };

	public:
		// Inherited via IFileFolderOperation
		virtual HRESULT Do(LPCTSTR path) override
		{
			StaticHelper::RemoveRHS(path);
			if (RemoveDirectory(path))
			{
				++m_count;
				StaticLoggerFactory::GetSingleton()->Log(0, TEXT("Delete Folder [index=%08d, name=%s]"), m_count, path);
			}
			else
			{
				++m_failedCount;
				StaticLoggerFactory::GetSingleton()->Log(0, TEXT("Fail to delete Folder [index=%08d, name=%s]"), m_failedCount, path);
			}
			

			return S_OK;
		}

		VOID PrintResult()
		{
			StaticLoggerFactory::GetSingleton()->Log(0, TEXT("Totally deleted [%d] folders, failed with [%d] folders"), m_count, m_failedCount);
		}
	};

	class CFileOperation : public IFileFolderOperation
	{
	private:
		int m_count{ 0 };
		int m_failedCount{ 0 };

	public:
		// Inherited via IFileFolderOperation
		virtual HRESULT Do(LPCTSTR path) override
		{
			StaticHelper::RemoveRHS(path);
			if (DeleteFile(path))
			{
				++m_count;
				StaticLoggerFactory::GetSingleton()->Log(0, TEXT("Delete File [index=%08d, name=%s]"), m_count, path);
			}
			else
			{
				++m_failedCount;
				StaticLoggerFactory::GetSingleton()->Log(0, TEXT("Fail to delete File [index=%08d, name=%s]"), m_failedCount, path);
			}
			
			
			return S_OK;
		}

		VOID PrintResult()
		{
			StaticLoggerFactory::GetSingleton()->Log(0, TEXT("Totally deleted [%d] files, failed with [%d] files"), m_count, m_failedCount);
		}
	};

	class CEnumFiles
	{
	private:
		IFileFolderOperation * m_preFolderOp{ nullptr };
		IFileFolderOperation * m_postFolderOp{ nullptr };
		IFileFolderOperation * m_fileOp{ nullptr };



	public:
		CEnumFiles(
			IFileFolderOperation * preFolderOp,
			IFileFolderOperation * postFolderOp,
			IFileFolderOperation * fileOp
		) :
			m_preFolderOp(preFolderOp),
			m_postFolderOp(postFolderOp),
			m_fileOp(fileOp)
		{
		}

		HRESULT Enum(LPCTSTR rootFolder, LPCTSTR filePattern)
		{
			CString fullPath;
			FolderFileCat(fullPath, rootFolder, filePattern);

			WIN32_FIND_DATA FindFileData = { 0 };
			HANDLE hFind = FindFirstFile( /***CLEANUP***/
				fullPath.GetString(),//_In_  LPCTSTR           lpFileName,
				&FindFileData//_Out_ LPWIN32_FIND_DATA lpFindFileData
			);


			if (INVALID_HANDLE_VALUE != hFind)
			{
				do
				{
					if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						if (0 == _tcscmp(TEXT("."), FindFileData.cFileName) ||
							0 == _tcscmp(TEXT(".."), FindFileData.cFileName))
						{
							//do nothing
						}
						else
						{
							CString strFolderPath;
							FolderFileCat(strFolderPath, rootFolder, FindFileData.cFileName);
							if (m_preFolderOp)
							{
								m_preFolderOp->Do(strFolderPath);
							}
							Enum(strFolderPath, filePattern);
							if (m_postFolderOp)
							{
								m_postFolderOp->Do(strFolderPath);
							}
						}
					}
					else
					{
						if (m_fileOp)
						{
							CString strFilePath;
							FolderFileCat(strFilePath, rootFolder, FindFileData.cFileName);
							m_fileOp->Do(strFilePath);
						}
					}
				} while (FindNextFile(hFind, &FindFileData));
			}


			FindClose(hFind); /***CLEANUP***/
			hFind = INVALID_HANDLE_VALUE;

			return S_OK;
		}

	private:
		static VOID AppendChar(CString & str, TCHAR ch)
		{
			if (str.GetLength() == 0)
			{
				str = ch;
			}
			else if (str[str.GetLength() - 1] != ch)
			{
				str += ch;
			}
		}

		static VOID FolderFileCat(CString & strFullPath, LPCTSTR folder, LPCTSTR file)
		{
			strFullPath = folder;
			AppendChar(strFullPath, TEXT('\\'));
			strFullPath += file;
		}
	};

	class StaticAPI
	{
	public:
		static HRESULT RemoveFile(LPCTSTR folder, LPCTSTR filePattern)
		{
			CFolderOperation folderOp;
			CFileOperation fileOp;

			CEnumFiles ef(nullptr, &folderOp, &fileOp);
			ef.Enum(folder, filePattern);

			folderOp.PrintResult();
			fileOp.PrintResult();

			return S_OK;
		}
	};

}//namespace SmartLib