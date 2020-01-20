#ifndef _HADT_HALOG_H_
#define _HADT_HALOG_H_
#pragma once

#include "..\DTraceLib\DTraceLib.h"

#define HALogError(x, ...)			DTraceInfo(0, DRNPOS, x, __VA_ARGS__)
#define HALogWarning(x, ...)		DTraceInfo(1, DRNPOS, x, __VA_ARGS__) 
#define HALogMsg(x, ...)				DTraceInfo(9, DRNPOS, x, __VA_ARGS__) 
#define HALogDebug(x, ...)			DTraceInfo(999, DRNPOS, x, __VA_ARGS__) 

#if 0
#include <windows.h>
#include <string>
#include <memory>
using std::wstring;
using std::auto_ptr;

class CHALog
{
protected:
    CHALog(void);
    ~CHALog(void);
    CHALog(const CHALog&); //forbidden
    CHALog& operator=(const CHALog&); //forbidden
public:
    static CHALog* Instance(void);
    static void    DestroyInstance(void);  //a explicitly release method, NOT required to call it.
    friend class std::auto_ptr<CHALog>;
public:
    // Init log's name and set if will append to the log file with the same name.
    int Init(const wchar_t* pwszLogName, bool bAppend, bool bShowPTID=false);

    // Set the max log size in bytes
    void SetMaxLogSize(unsigned long ulInBytes);

    // Set the registry value that control the log level, this registry value should be a DWORD value
    // Only LogDbg which's level <= the log level set in registry will be output (default is 0).
    // @hKeyParent [in] - such as HKEY_LOCAL_MACHINE
    // @pwszKeyName[in] - such as REG_KEY_ARC_OFFLINECOPY L"\\DebugLevel"
    // @pwszValueName[in] - such as L"HATransClient"
    void SetDebugLevelReg(HKEY hKeyParent, const wchar_t* pwszKeyName, const wchar_t* pwszValueName);

    //
    // Set the predifined debug level, if have set this value to non-zero value
    // LogDebug will not read the debug level from the registry table
    void SetPredefinedDebugLevel(unsigned int nLevel);

    void LogMsg(const wchar_t* format, ...);
    void LogWarning(const wchar_t* format, ...);
    void LogError(const wchar_t* format, ...);
    void LogDebug(unsigned int nLevel, const wchar_t* format, ...);
protected:
    enum e_log_level {LOG_MSG = 0, LOG_WARNING = 1, LOG_ERROR = 2, LOG_DEBUG = 3};
    void WriteOut(enum e_log_level level, const wchar_t* pwszCurrTime, wchar_t* pwszText);
    int Init(const wchar_t* pwszLogName, bool bAppend, bool bShowPTID, bool bRenamed);
    unsigned long GetDebugLevel();
    int SwitchFileIfExceedMaxSize();
    int InitWriteMutex(const wchar_t* pwszLogName);
protected:
    static auto_ptr<CHALog> m_auto_ptr;
    static CHALog* m_pInst;
    wstring m_strLogName; // D:\folder\aaa.log
    wstring m_strLogNameInput;
    int     m_nPredefDbgLvl;
    bool    m_bPredefDbgLvl;
    HANDLE  m_hFile;
    bool    m_bShowPTID;
    __int64 m_llFileSize;
    unsigned long m_ulMaxSize;
    wchar_t m_wszPrefix[256];
    CRITICAL_SECTION m_cs;
    HANDLE           m_hWriteMutex;

    HKEY m_hKeyParent;
    wstring m_wstrKeyName;
    wstring m_wstrValueName;
};

#define HALogInit                   CHALog::Instance()->Init
#define HALogSetMaxSize             CHALog::Instance()->SetMaxLogSize
#define HALogSetDbgLevelReg         CHALog::Instance()->SetDebugLevelReg
#define HALogSetPredefDebugLevel    CHALog::Instance()->SetPredefinedDebugLevel

#define HALogError      CHALog::Instance()->LogError
#define HALogWarning    CHALog::Instance()->LogWarning
#define HALogMsg        CHALog::Instance()->LogMsg
#define HALogDebug      CHALog::Instance()->LogDebug

#endif //_HADT_HALOG_H_
#endif //end #if 0