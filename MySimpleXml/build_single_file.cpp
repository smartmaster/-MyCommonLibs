#include "stdafx.h"

#include <fstream>
#include <map>

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicMiscHelper.h"

#include "build_single_file.h"
#include "EnumElement.h"
#include "ReadWriteStringFile.h"
#include "EnumWinFile.h"

//////////////////////////////////////////////////////////////////////////
static HRESULT SimpleRun(LPTSTR commanLine);
//////////////////////////////////////////////////////////////////////////

struct  BuildSingleFileConfig
{
	CString m_ProjectFullPathFile;
	CString m_plfatform;
	CString m_configuration;
	CString m_outDir;
};


enum class E_PLATFORM
{
	E_UNKNOWN,
	E_X64,
	E_WIN32,
};

static CONST LPCTSTR VC_VAR[] =
{
	TEXT("unknown"),	//E_UNKNOWN,
	TEXT("amd64"),		//E_X64,
	TEXT("x86")			//E_WIN32,
};

static CONST LPCTSTR _X64[] =
{
	TEXT("x64")
};

static CONST LPCTSTR _X86[] =
{
	TEXT("Win32")
};




E_PLATFORM DetectPlatForm(const BuildSingleFileConfig & config)
{
	

	E_PLATFORM e_platform = E_PLATFORM::E_UNKNOWN;
	for (INT ii = 0; ii < _countof(_X64); ++ii)
	{
		if (0 == _tcsicmp(_X64[ii], config.m_plfatform.GetString()))
		{
			e_platform = E_PLATFORM::E_X64;
			return e_platform;
		}
	}

	for (INT ii = 0; ii < _countof(_X86); ++ii)
	{
		if (0 == _tcsicmp(_X86[ii], config.m_plfatform.GetString()))
		{
			e_platform = E_PLATFORM::E_WIN32;
			return e_platform;
		}
	}

	return e_platform;
}

HRESULT BuildPlatFormConfigration(LPCTSTR vsInstallPath, const BuildSingleFileConfig & config, LPCTSTR libDir)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	E_PLATFORM e_platform = DetectPlatForm(config);
	if (e_platform == E_PLATFORM::E_UNKNOWN)
	{
		//LastError = WSAGetLastError();
		LastError = E_INVALIDARG;
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("DetectPlatForm"), hr, TEXT("%s"), config.m_plfatform.GetString());
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	LPCTSTR vcvar = VC_VAR[(INT)(e_platform)];

	INT indexPath = config.m_ProjectFullPathFile.ReverseFind(TEXT('\\'));
	if (indexPath < 0)
	{
		//LastError = WSAGetLastError();
		LastError = E_INVALIDARG;
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("E_INVALIDARG"), hr, TEXT("%s"), config.m_ProjectFullPathFile.GetString());
		LEAVE_BLOCK(0);
	}

	CString Path = config.m_ProjectFullPathFile.Mid(0, indexPath);
	CString File = config.m_ProjectFullPathFile.Mid(indexPath + 1);

	CONST INT BUFFER_SIZE = 1024;
	CString strModule;
	LPTSTR Buffer = strModule.GetBuffer(BUFFER_SIZE);
	GetModuleFileName(
		nullptr,//_In_opt_  HMODULE hModule,
		Buffer,//_Out_     LPTSTR lpFilename,
		BUFFER_SIZE//_In_      DWORD nSize
		);
	strModule.ReleaseBuffer();

	int indexModule = strModule.ReverseFind(TEXT('\\'));
	CString strModulePath = strModule.Mid(0, indexModule);
	CString strModuleFile = strModule.Mid(indexModule + 1);

	//////////////////////////////////////////////////////////////////////////
	static CONST TCHAR BUILD_SCRIPTS[] = TEXT("BuildScripts");
	CString strBuildScripts = strModulePath;
	strBuildScripts += TEXT('\\');
	strBuildScripts += BUILD_SCRIPTS;
	CreateDirectory(strBuildScripts.GetString(), nullptr);

	static CONST LPCTSTR LINE[] = 
	{
		//call "E:\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64
		//TEXT("call \"%s\\VC\\vcvarsall.bat\" %s") TEXT("\n"),

		//D:\Microsoft Visual Studio 2017 Enterprise\VC\Auxiliary\Build\vcvarsall.bat
		TEXT("call \"%s\\vcvarsall.bat\" %s") TEXT("\n"),
		//pushd "F:\~~~workspace\ad2d-r17-oolong\Native\Common\AFCommFunc"
		TEXT("pushd \"%s\"") TEXT("\n"),
		//msbuild /p:UseEnv=true /p:plfatform=x64 /p:configuration="Release" "AFCommFunc.vcxproj" /flp:Summary;LogFile="F:\~~~workspace\D2DBuildAll\Bin\_output_\_log\AFCommFunc.vcxproj_Release_x64.log"
		TEXT("msbuild /p:UseEnv=true /p:plfatform=%s /p:configuration=\"%s\" \"%s\" /flp:Summary;LogFile=\"%s\\%s.%s.%s.%s.log\"") TEXT("\n"),
		//popd
		TEXT("popd") TEXT("\n"),
	};

	CString strContent;
	int line_index = 0;
	strContent.AppendFormat(LINE[line_index++], vsInstallPath, vcvar);
	strContent.AppendFormat(LINE[line_index++], Path.GetString());
	strContent.AppendFormat(LINE[line_index++], 
		config.m_plfatform.GetString(), config.m_configuration.GetString(),	File.GetString(), 
		strBuildScripts.GetString(), strModuleFile.GetString(), File.GetString(), config.m_plfatform.GetString(), config.m_configuration.GetString()
		);
	strContent.AppendFormat(LINE[line_index++], TEXT(""));



	CString strCmFile;
	strCmFile.Format(TEXT("%s\\%s.%s.%s.%s.cmd"), strBuildScripts.GetString(), strModuleFile.GetString(), File.GetString(), config.m_plfatform.GetString(), config.m_configuration.GetString());
	{
		using namespace std;
		wofstream fs(strCmFile.GetString());
		fs << strContent.GetString();
	}


	//////////////////////////////////////////////////////////////////////////

	// Start the child process. 
	CString strCommandLine;
	strCommandLine.Format(TEXT("\"%s\""), strCmFile.GetString());
	hr = SimpleRun(strCommandLine.GetBuffer());
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("SimpleRun"), hr, TEXT("%s"), strCommandLine.GetString());
		LEAVE_BLOCK(0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	CString strBinSource;
	if (config.m_outDir.GetLength())
	{
		strBinSource = config.m_outDir;
	}
	else
	{
		strBinSource = Path;
		if (e_platform == E_PLATFORM::E_WIN32)
		{
			strBinSource.AppendFormat(TEXT("\\%s"), config.m_configuration.GetString());
		}
		else if (e_platform == E_PLATFORM::E_X64)
		{
			strBinSource.AppendFormat(TEXT("\\x64\\%s"), config.m_configuration.GetString());
		}
	}
	


	CString strBinDest = libDir;
	if (e_platform == E_PLATFORM::E_WIN32)
	{
		strBinDest.AppendFormat(TEXT("\\x86\\%s"), config.m_configuration.GetString());
	}
	else if (e_platform == E_PLATFORM::E_X64)
	{
		strBinDest.AppendFormat(TEXT("\\x64\\%s"), config.m_configuration.GetString());
	}

	//////////////////////////////////////////////////////////////////////////
	CString strCmCopyFile;
	static CONST TCHAR strCmCopyFile_format[] =
		TEXT("md \"%s\"") TEXT("\n")
		TEXT("robocopy \"%s\" \"%s\" *.lib *.lib.static *.pdb *.dll *.exe *.sys  /s /e /is") TEXT("\n");
	CString strCmCopyFileContent;
	strCmCopyFileContent.Format(strCmCopyFile_format, strBinDest.GetString(), strBinSource.GetString(), strBinDest.GetString());
	strCmCopyFile.Format(TEXT("%s\\%s.%s.%s.%s.copy.cmd"), strBuildScripts.GetString(), strModuleFile.GetString(), File.GetString(), config.m_plfatform.GetString(), config.m_configuration.GetString());
	{
		using namespace std;
		wofstream fs(strCmCopyFile.GetString());
		fs << strCmCopyFileContent.GetString();
	}


	// Start the child process. 
	//CString strCommandLine;
	strCommandLine.Format(TEXT("\"%s\""), strCmCopyFile.GetString());
	hr = SimpleRun(strCommandLine.GetBuffer());
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("SimpleRun"), hr, TEXT("%s"), strCommandLine.GetString());
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

namespace
{
	struct ThreadProcParam
	{
		CString					vsInstallPath;
		BuildSingleFileConfig	config;
		CString					libDir;
	};
} //end namespace

DWORD WINAPI BuildPlatFormConfigrationThreadProc(_In_  LPVOID lpParameter)
{
	CAutoPtr<ThreadProcParam> param((ThreadProcParam*)lpParameter);
	return BuildPlatFormConfigration(param->vsInstallPath, param->config, param->libDir);
}

HRESULT BuildPlatFormConfigrationThread(HANDLE & hThread, LPCTSTR vsInstallPath, const BuildSingleFileConfig & config, LPCTSTR libDir)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	ThreadProcParam * p = new ThreadProcParam{ vsInstallPath, config, libDir };
	DWORD ThreadId = 0;
	hThread = CreateThread(
		nullptr,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
		0,//_In_       SIZE_T dwStackSize,
		(LPTHREAD_START_ROUTINE)(BuildPlatFormConfigrationThreadProc),//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
		(LPVOID)(p),//_In_opt_   LPVOID lpParameter,
		0,//_In_       DWORD dwCreationFlags,
		&ThreadId//_Out_opt_  LPDWORD lpThreadId
		);
	if (nullptr == hThread)
	{
		delete p; p = nullptr;
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateThread"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}
	return hr;
}

HRESULT BuildFile(LPCTSTR vsInstallPath, LPCTSTR projectFullPathFile, LPCTSTR libDir)
{

	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	vector<CString> vecStr(1);
	hr = ReadSourceStringFile(projectFullPathFile, vecStr[0]);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadSourceStringFile"), hr, TEXT("%s"), projectFullPathFile);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	XmlElement RootElement;
	INT Start = 0;
	LONG ElementCount = RootElement.FromString(vecStr[0], Start);
	if (ElementCount <= 0)
	{
		hr = E_FAIL;
		D_API_ERR(0, TEXT("BadFormatXml"), ERROR_BAD_FORMAT, TEXT("%s"), projectFullPathFile);
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	static CONST TCHAR xPropertyGroup[] = TEXT("PropertyGroup");
	static CONST TCHAR xOutDir[] = TEXT("OutDir");
	static CONST TCHAR xCondition[] = TEXT("Condition");
	static CONST TCHAR xConfiguration_Platform[] = TEXT("'$(Configuration)|$(Platform)'=='%s|%s'");
	std::map<CString, CString> outDir;
	for (CONST auto & c1 : RootElement.m_Children)
	{
		if (0 == c1.m_Name.CompareNoCase(xPropertyGroup))
		{
			for (CONST auto & c2 : c1.m_Children)
			{
				if (0 == c2.m_Name.CompareNoCase(xOutDir))
				{
					CString strOurDir = c2.m_Value;
					for (CONST auto & attr : c2.m_Attrs)
					{
						if (0 == attr.m_AttrName.CompareNoCase(xCondition))
						{
							CString strCondition = attr.m_AttrValue;
							strCondition.MakeUpper();
							if (strOurDir.GetLength() && strOurDir[0] == TEXT('.'))
							{
								auto strOurDirTemp = strOurDir;
								CMiscHelper::ConvertRelativeToAbsolutePath(
									projectFullPathFile,//CONST CString & absPath1, 
									strOurDirTemp,//CONST CString & relPath, 
									strOurDir//CString & absPath2
									);
							}
							strOurDir.TrimRight(TEXT("\\/"));
							strOurDir.MakeUpper();
							outDir.insert(make_pair(strCondition, strOurDir));
							break;
						}
					}
				}
			}
		}
	}

#if 0
	///test only
	{
		CONST LPCTSTR Paths[] =
		{
			TEXT("PropertyGroup"),
			TEXT("OutDir xxxx")
		};
		CONST XmlElement * containingEle = nullptr;
		vector<CONST XmlElement *> vecChildren;
		INT pathsCount = _countof(Paths);
		BOOL bFound = XmlElement::FindByPath(RootElement, Paths, pathsCount, containingEle, vecChildren);
		for (CONST auto & var : vecChildren)
		{
			CString str;
			var->ToString(str);
			//_ftprintf_s(stdout, TEXT("%s") TEXT("\r\n"), str.GetString());
		}
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	static CONST TCHAR _Project[] = TEXT("Project");
	static CONST TCHAR _ItemGroup[] = TEXT("ItemGroup");
	static CONST TCHAR _ProjectConfiguration[] = TEXT("ProjectConfiguration");
	static CONST TCHAR _Configuration[] = TEXT("Configuration");
	static CONST TCHAR _Platform[] = TEXT("Platform");

	vector<HANDLE> hThreads; //<sonmi01>2015-5-21 ###???
	for (size_t ii = 0; ii < RootElement.m_Children.size(); ++ ii)
	{
		CONST XmlElement & ele_ItemGroup = RootElement.m_Children[ii];
		if (0 == ele_ItemGroup.m_Name.CompareNoCase(_ItemGroup))
		{
			for (size_t jj = 0; jj < ele_ItemGroup.m_Children.size(); ++ jj)
			{
				CONST XmlElement & ele__ProjectConfiguration = ele_ItemGroup.m_Children[jj];
				if (0 == ele__ProjectConfiguration.m_Name.CompareNoCase(_ProjectConfiguration))
				{
					BuildSingleFileConfig config;
					for (size_t kk = 0; kk < ele__ProjectConfiguration.m_Children.size(); ++ kk)
					{
						CONST auto & eleTemp = ele__ProjectConfiguration.m_Children[kk];
						if (0 == eleTemp.m_Name.CompareNoCase(_Configuration))
						{
							config.m_configuration = eleTemp.m_Value;
						}
						else if (0 == eleTemp.m_Name.CompareNoCase(_Platform))
						{
							config.m_plfatform = eleTemp.m_Value;
						}
					}
					config.m_ProjectFullPathFile = projectFullPathFile;

					CString strCP;
					strCP.Format(xConfiguration_Platform, config.m_configuration.GetString(), config.m_plfatform.GetString());
					strCP.MakeUpper();
					auto iter = outDir.find(strCP);
					if (outDir.end() != iter)
					{
						config.m_outDir = iter->second;
					}

					HANDLE hThread = nullptr;
					BuildPlatFormConfigrationThread(hThread, vsInstallPath, config, libDir);
					if (nullptr != hThread)
					{
						hThreads.push_back(hThread);
						hThread = nullptr;
					}
				}
			}
		}
	}

	ATLASSERT(hThreads.size() <= MAXIMUM_WAIT_OBJECTS);
	if (hThreads.size() > 0)
	{
		DWORD dwWait = WaitForMultipleObjects(hThreads.size(), &hThreads[0], TRUE, INFINITE);
		if (WAIT_FAILED == dwWait)
		{
			//LastError = WSAGetLastError();
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
	}

	for (auto & item : hThreads)
	{
		::CloseHandle(item);
		item = nullptr;
	}
	hThreads.clear();
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
	
}



struct CBuildEnumFolderFileOperation : public IEnumFolderFileOperation
{
private:
	CString m_strVSInstallPath;
	CString m_strLibDir;
	vector<CString> m_excludedDir;
public:
	CBuildEnumFolderFileOperation(LPCTSTR VSInstallPath, LPCTSTR libDir, CONST vector<CString> & excludedDir) :
		m_strVSInstallPath(VSInstallPath),
		m_strLibDir(libDir),
		m_excludedDir(excludedDir)
	{
		for (auto & var : m_excludedDir)
		{
			var.MakeUpper();
		}
	}

	virtual INT Operate(CONST CString & strFile)
	{
		BOOL bFind = FALSE;

		CString strFileTmp = strFile;
		strFileTmp.MakeUpper();
		for (CONST auto & var : m_excludedDir)
		{
			int index = strFileTmp.Find(var.GetString());
			if (index >= 0)
			{
				bFind = TRUE;
				break;
			}
		}

		if (bFind)
		{
			D_INFO(0, TEXT("%s is excluded"), strFile.GetString());
		}
		else
		{
			BuildFile(m_strVSInstallPath, strFile, m_strLibDir);
		}
		return 0;
	}
};

//////////////////////////////////////////////////////////////////////////
INT BuildFilePath(LPCTSTR VSInstallPath, LPCTSTR sourceRootDir, LPCTSTR libDir, CONST vector<CString> & excludedDir)
{
	CBuildEnumFolderFileOperation op(VSInstallPath, libDir, excludedDir);
	return EnumDirectoryFileTree(
		sourceRootDir,//LPCTSTR szDirectory, 
		TEXT("*.vcxproj"),//LPCTSTR szFileSpec, 
		nullptr,//IEnumFolderFileOperation * pFolderOperation, 
		&op//IEnumFolderFileOperation * pFileOperation
		);
}


//////////////////////////////////////////////////////////////////////////
static HRESULT SimpleRun(LPTSTR commanLine)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process. 

	DWORD creationFlags = CREATE_NEW_CONSOLE;
	BOOL bCreateProcess = CreateProcess(NULL,   // No module name (use command line)
		commanLine,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		creationFlags,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi);           // Pointer to PROCESS_INFORMATION structure
	if (!bCreateProcess)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateProcess"), hr, TEXT("%s"), commanLine);
		LEAVE_BLOCK(0);
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);
	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}