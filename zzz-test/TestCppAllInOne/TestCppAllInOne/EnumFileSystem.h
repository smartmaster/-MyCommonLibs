#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <locale>
#include <codecvt>

#include "StringOf.h"
#include "CodeBlock.h"
#include "log.h"



namespace SmartLib
{
	class Helper
	{
	public:
		template<typename OSTREAM>
		static OSTREAM& Print(OSTREAM & os)
		{
			return os;
		}

		template<typename OSTREAM, typename ARG1, typename ...ARGS>
		static OSTREAM& Print(OSTREAM & os, ARG1 && arg1, ARGS && ...args)
		{
			os << std::forward<ARG1>(arg1) << std::endl;
			Print(os, std::forward<ARGS>(args)...);
			return os;
		}
	};

	class IEnumOp
	{
	public:
		virtual HRESULT Begin(LPCTSTR parentDir, LPCTSTR filePattern) = 0;
		virtual HRESULT End(LPCTSTR parentDir, LPCTSTR filePattern) = 0;
		virtual HRESULT PreDir(LPCTSTR parentPath, LPCTSTR dirName, LPCTSTR shortName) = 0;
		virtual HRESULT PostDir(LPCTSTR parentPath, LPCTSTR dirName, LPCTSTR shortName) = 0;
		virtual HRESULT OnFile(LPCTSTR parentPath, LPCTSTR fileName, LPCTSTR shortName) = 0;
	};

	class CEnumDir
	{
	private:
		IEnumOp * _enumOp{ nullptr };
		

	private:
		static const WCHAR PathDelimeter{TEXT('\\')};

		static BOOL IsDotDir(LPCTSTR dir)
		{
			return dir && dir[0] && (0 == _tcsicmp(TEXT("."), dir) || 0 == _tcsicmp(TEXT(".."), dir));
		}

		static void AppendChar(std::wstring & str, WCHAR ch)
		{
			if (str.size() == 0)
			{
				str += ch;
			}
			else
			{
				if (str.back() != ch)
				{
					str += ch;
				}
			}
		}

	private:
		HRESULT EnumProc(LPCTSTR parentDir, LPCTSTR filePattern)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			HANDLE hfind = INVALID_HANDLE_VALUE; /***CLEANUP***/

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			std::wstring strPattern = parentDir;
			AppendChar(strPattern, PathDelimeter);
			strPattern += filePattern;

			WIN32_FIND_DATA findData = { 0 };
			//hfind = ::FindFirstFileEx(
			//	strPattern.c_str(),//_In_        LPCTSTR lpFileName,
			//	FindExInfoStandard,//_In_        FINDEX_INFO_LEVELS fInfoLevelId,
			//	&findData,//_Out_       LPVOID lpFindFileData,
			//	FindExSearchNameMatch,//_In_        FINDEX_SEARCH_OPS fSearchOp,
			//	nullptr,//_Reserved_  LPVOID lpSearchFilter,
			//	FIND_FIRST_EX_LARGE_FETCH//,//_In_        DWORD dwAdditionalFlags
			//);

			hfind = ::FindFirstFile(
				strPattern.c_str(),
				&findData
			);

			if (INVALID_HANDLE_VALUE == hfind)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				LEAVE_BLOCK(0);
			}

			for (;;)
			{
				if (FILE_ATTRIBUTE_DIRECTORY == (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					if (!IsDotDir(findData.cFileName))
					{
						if (_enumOp)
						{
							_enumOp->PreDir(parentDir, findData.cFileName, findData.cAlternateFileName);
						}

						std::wstring newParent = parentDir;
						AppendChar(newParent, PathDelimeter);
						newParent += findData.cFileName;
						EnumProc(newParent.c_str(), filePattern); //recursive call

						if (_enumOp)
						{
							_enumOp->PostDir(parentDir, findData.cFileName, findData.cAlternateFileName);
						}
					}
				}
				else
				{
					if (_enumOp)
					{
						_enumOp->OnFile(parentDir, findData.cFileName, findData.cAlternateFileName);
					}

				}


				BOOL fnf = ::FindNextFile(hfind, &findData);
				if (!fnf)
				{
					break;
				}

			}


			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			if (INVALID_HANDLE_VALUE != hfind)
			{
				::FindClose(hfind);
				hfind = INVALID_HANDLE_VALUE;
			}

			return hr;
		}

	public:

		CEnumDir(IEnumOp * enumOp) :
			_enumOp(enumOp)
		{
			
		}


		HRESULT Enum(LPCTSTR parentDir, LPCTSTR filePattern)
		{
			if (_enumOp)
			{
				_enumOp->Begin(parentDir, filePattern);
			}
			
			HRESULT hr = EnumProc(parentDir, filePattern);

			if (_enumOp)
			{
				_enumOp->End(parentDir, filePattern);
			}

			return hr;
		}
	
	};


	class CEnumOpPrint : 
		public IEnumOp
	{
	private:
		LONGLONG _dirCount{ 0LL };
		LONGLONG _fileCount{ 0LL };
		
		std::wstring _strSpaces;
		static const WCHAR _SPACES{TEXT(' ')};
	public:

		virtual HRESULT Begin(LPCTSTR parentDir, LPCTSTR filePattern) override
		{
			return S_OK;
		}

		virtual HRESULT End(LPCTSTR parentDir, LPCTSTR filePattern) override
		{
			return S_OK;
		}

		virtual HRESULT PreDir(LPCTSTR parentPath, LPCTSTR dirName, LPCTSTR shortName) override
		{
			++_dirCount;
			SML_LOG_LINE(TEXT("%s+%s\\%s (%s)"), _strSpaces.c_str(), parentPath, dirName, shortName);
			_strSpaces.push_back(_SPACES);
			return S_OK;
		}
		virtual HRESULT PostDir(LPCTSTR parentPath, LPCTSTR dirName, LPCTSTR shortName) override
		{
			//_strSpaces.re
			_strSpaces.pop_back();
			return S_OK;
		}
		virtual HRESULT OnFile(LPCTSTR parentPath, LPCTSTR fileName, LPCTSTR shortName) override
		{
			++_fileCount;
			SML_LOG_LINE(TEXT("%s-%s\\%s (%s)"), _strSpaces.c_str(), parentPath, fileName, shortName);
			return S_OK;
		}

		void PrintCount()
		{
			SML_LOG_LINE(TEXT("Dir count is %I64d, file count is %I64d"), _dirCount, _fileCount);
		}

	};


	class CEnumOpForWixSetup :
		public IEnumOp
	{
	private:
		LONGLONG _dirCount{ 0LL };
		LONGLONG _fileCount{ 0LL };
		std::wofstream _of;

		LPCTSTR _dirIdPrefix{ TEXT("DirId_###@@@NULL@@@###_") };
		LPCTSTR _componetIdPrefix{ TEXT("CompId_###@@@NULL@@@###_") };
		LPCTSTR _fileIdPrefix{ TEXT("FileId_###@@@NULL@@@###_") };

		std::wstring _strSpaces;
		static const WCHAR _SPACES{ TEXT(' ') };

		WCHAR _guidBuffer[64]{ 0 };

		std::vector<std::wstring> _compIds;
		std::wstring _curDir;

		static void CreateGuid(LPTSTR buffer, int count)
		{
			GUID guid{ 0 };
			::CoCreateGuid(&guid);
			::StringFromGUID2(guid, buffer, count);
		}

		static std::wstring To8Dot3Name(const std::wstring & fileName)
		{
			std::wstring  ret;
			
			auto pos = fileName.find_last_of(TEXT('.'));
			if (pos == std::wstring::npos)
			{
				ret = fileName.substr(0, 8 + 3 + 1);
			}
			else
			{
				if (pos >= 8)
				{
					ret = fileName.substr(0, 8);
					ret += fileName.substr(pos, 4);
				}
				else
				{
					ret = fileName.substr(0, pos + 4);
				}
			}


			return ret;
		}
	public:

		CEnumOpForWixSetup(LPCTSTR outFileName)
		{
			//std::wstring strtemp =  To8Dot3Name(L"123456789012345678");
			//To8Dot3Name(L"1234567890.12345678");
			//To8Dot3Name(L"12.12345678");
			//To8Dot3Name(L"12");
			//To8Dot3Name(L"123456.123");
			_of.open(outFileName);
			_of.imbue(std::locale(_of.getloc(), new std::codecvt_utf8_utf16<WCHAR>()));
		}

		virtual HRESULT Begin(LPCTSTR parentDir, LPCTSTR filePattern) override
		{
			CString str = parentDir;
			str.TrimRight(TEXT('\\'));
			int pos = str.ReverseFind(TEXT('\\'));
			if (pos >= 0)
			{
				str = str.Mid(pos + 1);
			}

			_of << L"<" << str.GetString() << L">" << std::endl;
			
			return S_OK;
		}

		virtual HRESULT End(LPCTSTR parentDir, LPCTSTR filePattern) override
		{
			OutputCompIds();

			CString str = parentDir;
			str.TrimRight(TEXT('\\'));
			int pos = str.ReverseFind(TEXT('\\'));
			if (pos >= 0)
			{
				str = str.Mid(pos + 1);
			}

			_of << L"</" << str.GetString() << L">" << std::endl;
			return S_OK;
		}


		virtual HRESULT PreDir(LPCTSTR parentPath, LPCTSTR dirName, LPCTSTR shortName) override
		{
			++_dirCount;

			_curDir = dirName;

			SML_LOG_LINE(TEXT("%s+%s\\%s (%s)"), _strSpaces.c_str(), parentPath, dirName, shortName);
			//<Directory Id = "DirId_NULL_dir" Name = "dir">

			CString strDirId = _dirIdPrefix;
			strDirId += dirName;
			strDirId.Replace(TEXT('.'), TEXT('_'));
			static const LPCTSTR formatStr = SML_WSTRING_OF(%s<Directory Id="%s" Name="%s">);
			CString str;
			str.Format(formatStr, _strSpaces.c_str(), strDirId.GetString(), dirName);
			_of << str.GetString() << std::endl;

			_strSpaces.push_back(_SPACES);
			return S_OK;
		}
		virtual HRESULT PostDir(LPCTSTR parentPath, LPCTSTR dirName, LPCTSTR shortName) override
		{
			//_strSpaces.re
			_strSpaces.pop_back();

			_curDir.clear();

			//<Directory Id = "DirId_NULL_dir" Name = "dir">
			static const LPCTSTR formatStr = SML_WSTRING_OF(%s</Directory>);
			CString str;
			str.Format(formatStr, _strSpaces.c_str());
			_of << str.GetString() << std::endl;

			return S_OK;
		}
		virtual HRESULT OnFile(LPCTSTR parentPath, LPCTSTR fileName, LPCTSTR shortName) override
		{
			++_fileCount;
			SML_LOG_LINE(TEXT("%s-%s\\%s (%s)"), _strSpaces.c_str(), parentPath, fileName, shortName);

			CString str;

			/*
			<Component Id="C_Udpw8cat" Guid="{C1DFB329-457A-40D3-0010-4345160E373E}">
			  <File Id="F_Udpw8cat" Name="AFFlt.cat" KeyPath="yes" ShortName="AFFLT.CAT" DiskId="1" Source="$(var.SolutionDir)..\tools\afflt\common_driver\AMD64\w8\AFFlt.cat" />
			</Component>
			*/

			//<Component Id="C_Udpw8cat" Guid="{C1DFB329-457A-40D3-0010-4345160E373E}">

			CString compId;
			compId.Format(TEXT("%s%s_%s"), _componetIdPrefix, _curDir.c_str(), fileName);
			compId.Replace(TEXT('.'), TEXT('_'));
			static const LPCTSTR fmt1 = SML_WSTRING_OF(%s<Component Id="%s" Guid="%s">);


			std::wstringstream ss;
			ss << L"<ComponentRef Id=\"" << compId.GetString() << "\" />";
			_compIds.push_back(ss.str());

			_guidBuffer[0] = 0;
			CreateGuid(_guidBuffer, _countof(_guidBuffer));
			str.Format(fmt1, _strSpaces.c_str(), compId.GetString(), _guidBuffer);
			_of << str.GetString() << std::endl;


			static const LPCTSTR fmt2 = SML_WSTRING_OF(%s%c<File Id="%s" Name="%s" KeyPath="yes" ShortName="%s" DiskId="1" Source="%s\%s" />);
			std::wstring shortname = To8Dot3Name(fileName);
			//::GetShortPathName(fileName, shortname, _countof(shortname));

			CString fileId;
			fileId.Format(TEXT("%s%s_%s"), _fileIdPrefix, _curDir.c_str(), fileName);
			fileId.Replace(TEXT("."), TEXT("_"));

			str.Format(fmt2, _strSpaces.c_str(), _SPACES, fileId.GetString(), fileName, shortname.c_str(), parentPath, fileName);
			_of << str.GetString() << std::endl;


			static const LPCTSTR fmt3 = SML_WSTRING_OF(%s</Component>);
			str.Format(fmt3, _strSpaces.c_str());
			_of << str.GetString() << std::endl;

			return S_OK;
		}


		void OutputCompIds()
		{
			for (const auto & item : _compIds)
			{
				_of << item.c_str() << std::endl;
			}
		}

		void PrintCount()
		{
			SML_LOG_LINE(TEXT("Dir count is %I64d, file count is %I64d"), _dirCount, _fileCount);
		}

	};

	class TestEnumDir
	{
	public:
		static void Case_CEnumOpPrint(LPCTSTR parent, LPCTSTR filePattern)
		{
			CEnumOpPrint print;
			CEnumDir enumDir(&print);
			enumDir.Enum(parent, filePattern);
			print.PrintCount();
		}


		static void Case_CEnumOpForWixSetup(LPCTSTR parent, LPCTSTR filePattern, LPCTSTR outfileName)
		{
			//{
			//	std::wstringstream wss;
			//	Helper::Print(wss, 1, 2, 3.4, 56LL, L"hello, world");
			//}
			
			CEnumOpForWixSetup  Wix(outfileName);
			CEnumDir enumDir(&Wix);
			enumDir.Enum(parent, filePattern);
			//Wix.OutputCompIds();			
			Wix.PrintCount();
		}
	};
}