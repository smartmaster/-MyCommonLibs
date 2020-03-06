#include "StdAfx.h"
#include "HALog.h"
#include "HATransErrorCode.h"
#include <atlbase.h>
#include <time.h>

#if 0

static const int MAX_LOGFILE_COUNT = 10000; //max log file count
static const int s_default_log_size = 10*1024*1024; //10M
static const wchar_t* s_logheader_PTID = L"[Time                  Severity    PID    TID]\r\n";
static const wchar_t* s_msg_prefix_PTID = L"[%s    %s    %04d    %04d] ";
// [time    severity    ]
static const wchar_t* s_logheader = L"[Time                  Severity]\r\n";
static const wchar_t* s_msg_prefix = L"[%s    %s    ] ";
static const wchar_t* s_time_format = L"%.4d-%.2d-%.2d %.2d:%.2d:%.2d";
static wchar_t* s_log_severity[] = {L"Message ", L"Warning ", L"Error   ", L"Debug   "};
static void GetCurrTime(wchar_t* pwszCurrTime, int nCount);

static const int HALOG_WRITEWAITTIME = 1000*5;
CHALog* CHALog::m_pInst = NULL;
auto_ptr<CHALog> CHALog::m_auto_ptr;

//
//The critical section lock
//
class CLocalCsLock
{
public:
    explicit CLocalCsLock(CRITICAL_SECTION* pCs) : m_pCs(pCs) {::EnterCriticalSection(pCs);};
    ~CLocalCsLock() {Release();};
public:
    void Release()
    {
        if (m_pCs)
            ::LeaveCriticalSection(m_pCs); 
        m_pCs = NULL;
    }
private:
    CLocalCsLock(const CLocalCsLock&); //forbidden
    CLocalCsLock& operator=(const CLocalCsLock&); //forbidden
private:
    CRITICAL_SECTION* m_pCs;
};

CHALog::CHALog(void)
:m_strLogName(L"")
, m_hFile(INVALID_HANDLE_VALUE)
, m_bShowPTID(false)
, m_llFileSize(0)
, m_ulMaxSize(s_default_log_size)
, m_hKeyParent(0)
, m_wstrKeyName(L"")
, m_wstrValueName(L"")
, m_strLogNameInput(L"")
, m_nPredefDbgLvl(0)
, m_bPredefDbgLvl(false)
, m_hWriteMutex(NULL)
{
    m_auto_ptr = auto_ptr<CHALog>(this);
    InitializeCriticalSection(&m_cs);
    ZeroMemory(m_wszPrefix, sizeof(m_wszPrefix));
}

CHALog::~CHALog(void)
{
    if (m_hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
    }
    m_strLogName = L"";
    m_llFileSize = 0;
    DeleteCriticalSection(&m_cs);
    if (m_hWriteMutex != NULL)
    {
        ::CloseHandle(m_hWriteMutex);
        m_hWriteMutex = NULL;
    }
}

CHALog* 
CHALog::Instance(void)
{
    if (m_pInst == NULL)
        m_pInst = new CHALog;
    return m_pInst;
}


int 
CHALog::Init(const wchar_t* pwszLogName, bool bAppend, bool bShowPTID)
{
    return Init(pwszLogName, bAppend, bShowPTID, false);
}

int 
CHALog::Init(const wchar_t* pwszLogName, bool bAppend, bool bShowPTID, bool bRenamed)
{
    CLocalCsLock lock(&m_cs);
    DWORD dwCreateDisp = bAppend ? OPEN_ALWAYS : CREATE_ALWAYS;
    if (pwszLogName == NULL)
        return E_INVALIDARG;

    if (m_hFile != INVALID_HANDLE_VALUE)
    {
//satmu02
/*        if (_wcsicmp(pwszLogName, m_strLogName.c_str()) == 0)
            return 0;
        else
            return E_HA_LOG_CANNOT_RENAMED;*/
    }
    m_bShowPTID = bShowPTID;

    m_hFile = CreateFileW(pwszLogName, 
                        GENERIC_WRITE, 
                        FILE_SHARE_READ | FILE_SHARE_WRITE, 
                        NULL, 
                        dwCreateDisp, 
                        FILE_ATTRIBUTE_NORMAL, 
                        NULL);

    if (m_hFile == INVALID_HANDLE_VALUE)
        return E_HA_LOG_CREATE_FILE_FAILED;

    if (int nRet = InitWriteMutex(pwszLogName))
        return nRet;

    if (WAIT_OBJECT_0 != ::WaitForSingleObject(m_hWriteMutex, HALOG_WRITEWAITTIME))
    {
        return ::GetLastError();
    }

    LARGE_INTEGER liSize = {0};
    GetFileSizeEx(m_hFile, &liSize);
    if (liSize.QuadPart == 0)
    {
        DWORD dwWritten = 0;
        char caMark[] = {(char)0xFF, (char)0xFE};
        ::WriteFile(m_hFile, caMark, sizeof(caMark), &dwWritten, 0);
        if (m_bShowPTID)
            ::WriteFile(m_hFile, s_logheader_PTID, (DWORD)wcslen(s_logheader_PTID)*2, &dwWritten, 0);
        else
            ::WriteFile(m_hFile, s_logheader, (DWORD)wcslen(s_logheader)*2, &dwWritten, 0);
    }
    m_strLogName = pwszLogName;
    if (!bRenamed)
        m_strLogNameInput = pwszLogName;
    if (bAppend)
        SetFilePointer(m_hFile, 0, 0, FILE_END);
    GetFileSizeEx(m_hFile, &liSize);
    m_llFileSize = liSize.QuadPart;

    if (m_hWriteMutex)
        ::ReleaseMutex(m_hWriteMutex);

    return 0;
}

void
CHALog::SetMaxLogSize(unsigned long ulInBytes)
{
    if (ulInBytes > s_default_log_size
        && ulInBytes < (unsigned long)2*1024*1024*1024)
    {
        m_ulMaxSize = ulInBytes;
    }
}

void 
CHALog::DestroyInstance(void)
{
    if (m_pInst != NULL)
    {
        delete m_pInst;
        m_pInst = NULL;
    }
    m_auto_ptr.release();
}

void 
CHALog::LogMsg(const wchar_t* format, ...)
{
    //get time
    wchar_t wszCurrTime[255] = {0};
    GetCurrTime(wszCurrTime, _countof(wszCurrTime));

 
    //get input format
    wchar_t wszText[2048] = {0};
    va_list arglist;

    va_start(arglist, format);
    vswprintf_s(wszText, _countof(wszText), format, arglist);
    va_end(arglist);

    //writeout
    return WriteOut(LOG_MSG, wszCurrTime, wszText);
}

void 
CHALog::LogWarning(const wchar_t* format, ...)
{
    //get time
    wchar_t wszCurrTime[255] = {0};
    GetCurrTime(wszCurrTime, _countof(wszCurrTime));


    //get input format
    wchar_t wszText[2048] = {0};
    va_list arglist;

    va_start(arglist, format);
    vswprintf_s(wszText, _countof(wszText), format, arglist);
    va_end(arglist);

    //writeout
    return WriteOut(LOG_WARNING, wszCurrTime, wszText);
}

void 
CHALog::LogError(const wchar_t* format, ...)
{
    //get time
    wchar_t wszCurrTime[255] = {0};
    GetCurrTime(wszCurrTime, _countof(wszCurrTime));


    //get input format
    wchar_t wszText[2048] = {0};
    va_list arglist;

    va_start(arglist, format);
    vswprintf_s(wszText, _countof(wszText), format, arglist);
    va_end(arglist);

    //writeout
    return WriteOut(LOG_ERROR, wszCurrTime, wszText);
}

void 
CHALog::LogDebug(unsigned int nLevel, const wchar_t* format, ...)
{
    unsigned long nLevelSet = GetDebugLevel();
    if (nLevel > nLevelSet)
        return;

    //get time
    wchar_t wszCurrTime[255] = {0};
    GetCurrTime(wszCurrTime, _countof(wszCurrTime));


    //get input format
    wchar_t wszText[2048] = {0};
    va_list arglist;

    va_start(arglist, format);
    vswprintf_s(wszText, _countof(wszText), format, arglist);
    va_end(arglist);

    //writeout
    return WriteOut(LOG_DEBUG, wszCurrTime, wszText);
}

static __int64 _HA_GetFileSize(const wchar_t* pwszFileName)
{
	HANDLE hFile  = CreateFile(pwszFileName, 
		GENERIC_READ, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, 
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN, 
		NULL
		);
	if( hFile == INVALID_HANDLE_VALUE )
		return 0;

    LARGE_INTEGER li = {0};
	GetFileSizeEx(hFile, &li);
	CloseHandle(hFile);

    return li.QuadPart;
}

int
CHALog::SwitchFileIfExceedMaxSize()
{
    int nRet = 0;
    CLocalCsLock lock(&m_cs);
    if (m_llFileSize >= m_ulMaxSize)
    {
        if (m_hFile != INVALID_HANDLE_VALUE)
        { ::CloseHandle(m_hFile); m_hFile = INVALID_HANDLE_VALUE; } 
        if (m_hWriteMutex != NULL)
        { ::CloseHandle(m_hWriteMutex); m_hWriteMutex = NULL; }

        wstring wstrBase;
        wstring::size_type pos = m_strLogNameInput.rfind('.');
        if (pos != wstring::npos)
            wstrBase = m_strLogNameInput.substr(0, pos);
        else
            wstrBase = m_strLogNameInput;

        WCHAR wszNumLog[MAX_PATH] = {0}; //"000N.log"
        wstring wstrTempFile;
        for (int nIndex = 1; nIndex<MAX_LOGFILE_COUNT; nIndex++)
        {
            _snwprintf_s( wszNumLog, _countof(wszNumLog), _TRUNCATE, L"_%04d.log", nIndex );
            wstrTempFile = wstrBase + wszNumLog;
            if (_HA_GetFileSize(wstrTempFile.c_str()) < m_ulMaxSize)
                break;
        }
        lock.Release();
        nRet = Init(wstrTempFile.c_str(), true, m_bShowPTID, true);
    }

    return nRet;
}

int 
CHALog::InitWriteMutex(const wchar_t* pwszLogName)
{
    if (pwszLogName == NULL || pwszLogName[0] == L'\0')
        return E_INVALIDARG;
    if( m_hWriteMutex != NULL )
		return 0;

    DWORD dwRet = 0;
	wchar_t wszMutexName[MAX_PATH] = L"Global\\";	// "Global\" is case sensitive
	wchar_t wszTemp[MAX_PATH] = {0};
	
	wcsncpy_s(wszTemp, _countof(wszTemp), pwszLogName, _TRUNCATE);	
	for (size_t i = 0; i < wcslen(wszTemp); i++)
	{
		if (wszTemp[i] == L'\\')
			wszTemp[i] = L'/';
	}
	
	_wcsupr_s(wszTemp );
	wcsncat_s(wszMutexName, _countof(wszMutexName), wszTemp, _TRUNCATE);

    SECURITY_DESCRIPTOR sd; 
    SECURITY_ATTRIBUTES sa; 
    InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION); 
    SetSecurityDescriptorDacl(&sd,TRUE,(PACL)NULL,FALSE);   
    sa.nLength=sizeof(SECURITY_ATTRIBUTES); 
    sa.bInheritHandle=FALSE; 
    sa.lpSecurityDescriptor=&sd;

	m_hWriteMutex = CreateMutex(&sa, FALSE, wszMutexName);
	if (NULL == m_hWriteMutex)
    {
        dwRet = ::GetLastError();
        if (dwRet == 0) dwRet = (DWORD)-1;
    }

    return dwRet;
}

void
CHALog::WriteOut(enum e_log_level level, const wchar_t* pwszCurrTime, wchar_t* pwszText)
{
    if (int nRet = SwitchFileIfExceedMaxSize())
        return;

    //writeout them
    if (m_hWriteMutex &&
        WAIT_OBJECT_0 != ::WaitForSingleObject(m_hWriteMutex, HALOG_WRITEWAITTIME))
    {
        return;
    }

    DWORD dwBytesWritten = 0;
    //write out
    if (m_bShowPTID)
    {
        _snwprintf_s(m_wszPrefix, _countof(m_wszPrefix), _TRUNCATE, s_msg_prefix_PTID,
        pwszCurrTime, s_log_severity[level], GetCurrentProcessId(), GetCurrentThreadId());
    }
    else
    {
        _snwprintf_s(m_wszPrefix, _countof(m_wszPrefix), _TRUNCATE, s_msg_prefix,
            pwszCurrTime, s_log_severity[level]);
    }
    WriteFile(m_hFile, m_wszPrefix, (DWORD)wcslen(m_wszPrefix)*sizeof(wchar_t), &dwBytesWritten, NULL);
    m_llFileSize += dwBytesWritten;
    WriteFile(m_hFile, pwszText, (DWORD)wcslen(pwszText)*sizeof(wchar_t), &dwBytesWritten, NULL);
    m_llFileSize += dwBytesWritten;
    WriteFile(m_hFile, L"\r\n", (DWORD)wcslen(L"\r\n")*sizeof(wchar_t), &dwBytesWritten, NULL);
    m_llFileSize += dwBytesWritten;
    
    if (m_hWriteMutex)
        ::ReleaseMutex(m_hWriteMutex);
    
}

static void GetCurrTime(wchar_t* pwszCurrTime, int nCount)
{
    errno_t err;
    struct tm newtime;
    __time64_t long_time;

    _time64( &long_time);
    err = _localtime64_s( &newtime, &long_time );
    if (err == 0)
    {
        _snwprintf_s(pwszCurrTime, 
            nCount, 
            _TRUNCATE,
            s_time_format,
            newtime.tm_year+1900,
            newtime.tm_mon+1,
            newtime.tm_mday,
            newtime.tm_hour,
            newtime.tm_min,
            newtime.tm_sec);
    }
}

void 
CHALog::SetDebugLevelReg(HKEY hKeyParent, const wchar_t* pwszKeyName, const wchar_t* pwszValueName)
{
    m_hKeyParent = hKeyParent;
    m_wstrKeyName = pwszKeyName;
    m_wstrValueName = pwszValueName;
}

void 
CHALog::SetPredefinedDebugLevel(unsigned int nLevel)
{
    m_nPredefDbgLvl = nLevel;
    m_bPredefDbgLvl = true;
}

unsigned long 
CHALog::GetDebugLevel()
{
    if (m_bPredefDbgLvl)
        return m_nPredefDbgLvl;

    if (m_hKeyParent == 0 || m_wstrKeyName.empty() || m_wstrValueName.empty())
        return 0;

    DWORD dwLevel = 0;
    CRegKey regKey;
    LONG lRet = ERROR_SUCCESS;

    lRet = regKey.Create(m_hKeyParent, m_wstrKeyName.c_str());
    if (ERROR_SUCCESS == lRet)
    {
        lRet = regKey.QueryDWORDValue(m_wstrValueName.c_str(), dwLevel);
    }

    if (ERROR_SUCCESS != lRet)
    {
        dwLevel = 0;
    }

    return dwLevel;
}

#endif //end #if 0