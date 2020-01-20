#pragma once
#include <windows.h>
#include <Winsvc.h>
#include <string>
#include <vector>
#include <AccCtrl.h>
#include <Aclapi.h>
#include <Sddl.h>
#include <shlwapi.h>
using namespace std;

//==================================Type Defines===============================
typedef vector<wstring> VSTRLIST;

typedef struct _FILE_VER_
{
    DWORD	dwMaj1;
    DWORD	dwMaj2;
    DWORD	dwMin1;
    DWORD	dwMin2;
}FILE_VER, *PFILE_VER;

typedef enum _OSVER_
{
    OSV_UNKNOW,
    OSV_W2K,
    OSV_WXP,
    OSV_W2K3,
    OSV_VISTA,
    OSV_W2K8,
    OSV_WIN7,
    OSV_W2K8R2,
    OSV_WIN8,
	OSV_W2K12,
	OSV_WIN81,
	OSV_W2K12R2,
	OSV_WIN10
}OS_VER, *POS_VER;

typedef enum _CPUARC_
{
    CPU_UNKNOW,
    CPU_X86,
    CPU_X64,
    CPU_IA64
}CPUARC, *PCPUARC;

typedef struct _OS_INFO_
{
    OS_VER osVer;
    CPUARC cpuArc;
} OS_INFO,*POS_INFO;


class CAdrTime
{
public:
    CAdrTime(void);
    virtual ~CAdrTime(void);
public:
    std::wstring	GetDate() const;
    std::wstring	GetTime() const;

    WORD	Year()		const;
    WORD	Month()		const;
    WORD	Day()		const;

    WORD	Hour()		const;
    WORD	Minute()	const;
    WORD	Second()	const;

private:
    SYSTEMTIME m_SysTime;
};


class CFileVer
{
public:
    CFileVer(void);
    ~CFileVer(void);
public:
    static BOOL GetFileVer(LPCWSTR lpFile, FILE_VER& FileVer);
    /*
    Retrieve the file version string info
    lpItemName, its value is :
    Comments
    CompanyName
    FileDescription
    FileVersion
    InternalName
    LegalCopyright
    LegalTrademarks
    OriginalFileName
    PrivateBuild
    ProductName
    ProductVersion
    SpecialBuild
    */
    static BOOL GetFileVerInfo(LPCWSTR lpFile, LPCWSTR lpItemName, wstring& strValue);

    static BOOL GetFileVerInfoOriginalName(LPCWSTR lpFile, wstring& strValue);
};


class CIxStr
{
public:
    CIxStr(void);
    ~CIxStr(void);

public:
    static BOOL		I2Str(__int64 value, wstring& strValue, int radix = 10);
    static BOOL		Guid2Str(const GUID& Guid, wstring& strGuid);
    static __int64	Str2I64(const wstring& strValue);
    static GUID		Str2Guid(const wstring& strValue);
    static GUID     NewGuid();
    static DWORD    GenSigFromGUID(const GUID &guid);

private:
    static void BuildCRCTable();

private:
    static DWORD m_CRCTable[256];

    static BOOL m_bCrcTableInit;
};

class CADRFileW
{
public:
    CADRFileW(void);
public:
    ~CADRFileW(void);

public:
    static BOOL		DeleteDirecotry(LPCWSTR lpPath, BOOL bRecursion);
    static BOOL		CopyDirectory(LPCWSTR lpExit, LPCWSTR lpNew);
    static BOOL		CopyDirectory2(LPCWSTR lpExit, LPCWSTR lpNew);
    static BOOL		CreateDirectory(LPCWSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes=NULL);
    static BOOL		CreateAllDirecotries(LPCWSTR lpszFullPath, LPSECURITY_ATTRIBUTES lpSecurityAttributes=NULL);
    static BOOL		IsFileExist(LPCWSTR lpFileName);
    static BOOL		IsDirectoryExist(LPCWSTR lpDirectory);
    static BOOL		DeleteFile(LPCWSTR lpFileName);
    static BOOL		CopyFile(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, BOOL bFailIfExists = TRUE);
    static DWORD	GetFullPathNameEx(LPCWSTR lpFileName, DWORD nBufferLength, LPWSTR lpBuffer, LPWSTR* lpFilePart, LPWSTR* lpExe = NULL);
    static BOOL		GetFullPathNameEx(LPCWSTR lpFileName, std::wstring &strPath, std::wstring& strName, std::wstring &strExe);

    static BOOL		GetSizeEx(LPCWSTR lpFileName, LARGE_INTEGER& liFileSize);
public:
    BOOL FindFirstFile(LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData);
    BOOL FindNextFile(LPWIN32_FIND_DATAW lpFindFileData);
    BOOL FindClose();

public:
    BOOL Open(
        LPCWSTR lpFileName, 
        BOOL bCreate = FALSE,  
        DWORD dwDesiredAccess = FILE_ALL_ACCESS,
        DWORD dwShareMode = FILE_SHARE_WRITE,
        DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
        );

    BOOL Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped = NULL);
    BOOL Write( LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped = NULL);
    BOOL Close();
    BOOL IsExist();
    BOOL Delete(LPCWSTR lpFileName = NULL);
    DWORD GetSize(LPDWORD lpFileSizeHigh = NULL);

    DWORD SetPointer(DWORD dwMoveMethod = FILE_BEGIN, LONG lDistanceToMove = 0, PLONG lpDistanceToMoveHigh = NULL);

private:
    BOOL CopyDirectory(LPCWSTR lpExistPath, LPCWSTR lpNewPath, BOOL bRecursion);
private:
    HANDLE	m_hFile;
    HANDLE	m_hFind;
    WCHAR	m_szFileName[MAX_PATH+1];
};

class CRegistry
{
public:
    CRegistry(void);
    virtual ~CRegistry(void);
public:
    LONG	Open(LPCWSTR lpszKeyName, HKEY hKeyParent = HKEY_LOCAL_MACHINE, REGSAM samDesired = KEY_READ);
	LONG	OpenWrite( LPCWSTR lpszKeyName, HKEY hKeyParent = HKEY_LOCAL_MACHINE );
    LONG	Create(LPCWSTR lpszKeyName, HKEY hKeyParent = HKEY_LOCAL_MACHINE, 
				   LPWSTR lpszClass = REG_NONE, DWORD dwOptions = REG_OPTION_NON_VOLATILE,
				   REGSAM samDesired =KEY_READ | KEY_WRITE, LPSECURITY_ATTRIBUTES lpSecAttr = NULL, 
				   LPDWORD lpdwDisposition = NULL);
    LONG	Close();

    LONG	QueryStringValue(LPCWSTR pszValueName, LPWSTR pszValue, ULONG* pnChars);
    LONG	QueryStringValue(LPCWSTR pszValueName, wstring &strValue);
    LONG    QueryMultiStringValue(LPCWSTR pszValueName, vector<wstring> &vStr);
    LONG	QueryDWORDValue(LPCWSTR pszValueName, DWORD& dwValue);
	LONG	QueryQWORDValue(LPCWSTR pszValueName, __int64& iiValue);
    LONG	QueryValue(LPCWSTR pszValueName, DWORD* pdwType, void* pData, ULONG* pnBytes);

    LONG	SetStringValue(LPCWSTR pszValueName, LPCWSTR pszValue);
    LONG	SetDWORDValue(LPCWSTR pszValueName, DWORD dwValue);
	LONG	SetQWORDValue( LPCWSTR pszValueName, __int64 iiValue );
    LONG	SetValue(LPCWSTR pszValueName, DWORD dwType, const void* pValue, ULONG nBytes); 
	LONG    SetMultiStringValue( LPCWSTR pszValueName, const vector<wstring>& vStr );

    LONG	EnumKeyNames(VSTRLIST& NameList);
    LONG	EnumValueNames(VSTRLIST& NameList);

    LONG	DeleteValue(LPCWSTR lpValueName);
    LONG	DeleteKey(LPCWSTR lpKeyName);
	
    LONG	SaveKey(LPCWSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
    LONG	SaveKeyEx(LPCWSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, DWORD Flags = REG_LATEST_FORMAT);
    LONG	RestoreKey(LPCWSTR lpFile, DWORD dwFlags = REG_NO_LAZY_FLUSH);

    LONG	CopyKeyValues(HKEY hDstKey);

    static	DWORD LoadRegKey(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpFile);
    static	DWORD UnLoadRegKey(HKEY hKey, LPCWSTR lpSubKey);

private:    
    static BOOL IsFileExist(LPCWSTR lpFileName);
private:
    HKEY m_hKey;

};


class CSystemInfo
{
public:
    CSystemInfo(void);
    virtual ~CSystemInfo(void);

public:
    BOOL	GetHostName(std::wstring& strName);
    BOOL	GetBootVolumeName(wstring& strVolName);
    DWORD	GetSystemVolumeName(wstring& strVolName);
    DWORD   GetSystemVolumeNameEx(wstring &strVolName, wstring &strDosDeviceName);
    BOOL	GetSystemDirectory(wstring& strSysDir);

    LANGID	GetLanguageId();
    BOOL    IsUEFIBoot();

	DWORD	GetCPUCount();
private:
    LONG	GetSystemPartitionDosDeviceName(wstring& strVolName);
    BOOL	QueryDosNameByVolumeName(LPCWSTR lpVolName, wstring& strDosName);
};

namespace UTILS
{
    void U2A( LPCWSTR ws, LPSTR s, int nSizeInChar );
    void A2U( LPCSTR s, LPWSTR ws, int nSizeInWChar );
    void GetTimeStrA( char* szTime, int nSize );
    void GetTimeStrW( WCHAR* szTime, int nSize );
    DWORD CopyCharToWChar( const CHAR *ptr, WCHAR *wptr, int len, UINT uCodePage = CP_ACP);

    DWORD CopyWCharToChar( const WCHAR *wptr, CHAR *ptr, int len, UINT uCodePage = CP_ACP);

    /***********************************************************************
    *@Purpose: convert wide characters into string container
    *@pBuf: buffer which contains wide characters
    *@theString: string contaier which contains converted characters
    *@Return value: If function succeeds, 0 will be returned.
    If function fails, system error code will be returned
    ***********************************************************************/

    DWORD AssignWCharToString( const WCHAR *pBuf, string &theString );

    /***********************************************************************
    *@Purpose: convert ANSI characters into UNICODE wstring
    *@pBuf: buffer which contains ANSI characters
    *@str: wstring contaier which contains converted UNICODE characters
    *@Return value: If function succeeds, 0 will be returned.
    If function fails, system error code will be returned
    ***********************************************************************/

   DWORD AssignCharToWString(const char *pBuf, wstring &str);

   wstring FileTime2HexString(const FILETIME& ft);

   FILETIME HexString2FileTime(const wstring& strHexTime);

   wstring FileTime2String( const FILETIME& ft );

   FILETIME String2FileTime( const wstring& strTime );
}

class CWinVer
{
public:
    CWinVer(void);
public:
    virtual ~CWinVer(void);

public:
    DWORD	GetMajVer() const;
    DWORD	GetMinVer() const;
    DWORD	GetProductType() const;
    wstring GetCSDVersion() const;
	BOOL	IsSBS() const;
	BOOL	IsEBS() const;
	BOOL	IsEssentials() const;
    WORD    GetSPMajVer();
    WORD    GetSPMinVer();
    BOOL	IsWin2k();
    BOOL	IsWinXp();
    BOOL	IsWin2k3();
    BOOL	IsVista();
    BOOL	IsLongHorn();
    BOOL	IsWin7();
    BOOL	IsW2k8();
    BOOL	IsW2k8R2();
    BOOL	IsWin8();
	BOOL	IsW2K12();
	BOOL	IsWin81();
	BOOL	IsW2K12R2();
	BOOL	IsWin10();

    BOOL	GetVer(OS_VER& OsVer);

private:
    BOOL GetVer();
private:
    OSVERSIONINFOEX m_OsVer;
    BOOL m_bGet;
};


class CSysInfo
{
public:
    CSysInfo(void);
    virtual ~CSysInfo(void);
public:
    BOOL IsX64();
    BOOL IsIA64();
    BOOL IsX86();
    BOOL GetSysInfo(LPSYSTEM_INFO pSysInfo);
	BOOL IsWow64();
private:
    BOOL GetInfo();
private:
    SYSTEM_INFO m_SysInfo;
    BOOL m_bGet;

};

class CWowFun
{
public:
    CWowFun(void);
    virtual ~CWowFun(void);

public:
    BOOL DisableFsRedirection(PVOID* ppOldValue);
    BOOL RevertFsRedirection(PVOID pOldValue);
private:
    HMODULE m_hMod;
private:
    typedef BOOL(WINAPI *WOWDISABLEFSREDIRECTION)(PVOID *pOld );
    typedef BOOL(WINAPI *WOWREVERTFSREDIRECTION)(PVOID old );
};

//Added by zouyu01 on 5/8/2009
#define COMMAND_LENGTH 4096
//For unsignature driver, if user canceled the installation, the return value for 64-bit w2k3
//is 2148204800(for 32-bit w2k3 is 2148204801)
#define ERROR_CANCEL_INSTALL_DRIVER_X64 0x800b0100
#define ERROR_CANCEL_INSTALL_DRIVER 0x800b0101

typedef BOOL (WINAPI *PfnWow64DisableWow64FsRedirection)(PVOID *OldValue);
typedef BOOL (WINAPI *PfnWow64RevertWow64FsRedirection)(PVOID OldValue);
typedef UINT (WINAPI *PfnGetSystemWow64DirectoryW)(LPWSTR lpBuffer,UINT size);

class RunDllWow
{
public:
    RunDllWow(void);

    ~RunDllWow(void);

    DWORD RunDll64( const std::wstring &strDll,const std::wstring &strFunc,const std::wstring &strParam, const wchar_t *pDir=NULL );

private:
    DWORD Log( DWORD errNo, int level, IN wchar_t *Str, ... ); 

    DWORD InitFuncAddrFromDll();

private:

    //CDbgLog *m_pLog;

    HMODULE m_hDll;

    PfnWow64DisableWow64FsRedirection m_pfnDisableWow64Redir;

    PfnWow64RevertWow64FsRedirection m_pfnRevertWow64Redir;

    PfnGetSystemWow64DirectoryW m_pfnGetSystemWow64Dir;

};

class CService  
{
public:
    CService();
    virtual ~CService();
    BOOL Create(
        LPCWSTR lpServiceName,
        LPCWSTR lpBinaryPathName,
        LPCWSTR lpDisplayName=NULL,
        LPCWSTR lpLoadOrderGroup=NULL,
        DWORD dwDesiredAccess=SC_MANAGER_ALL_ACCESS,
        DWORD dwServiceType=SERVICE_WIN32_OWN_PROCESS,
        DWORD dwStartType=SERVICE_DEMAND_START,
        DWORD dwErrorControl=SERVICE_ERROR_NORMAL,
        LPDWORD lpdwTagId=NULL,
        LPCWSTR lpDependencies=NULL,
        LPCWSTR lpServiceStartName=NULL,
        LPCWSTR lpPassword=NULL
        );
	BOOL Create2(
        LPCWSTR lpServiceName,
        LPCWSTR lpBinaryPathName,
        LPCWSTR lpDisplayName=NULL,
		LPWSTR	lpServiceDescription = NULL,
        LPCWSTR lpLoadOrderGroup=NULL,			
        DWORD dwDesiredAccess=SC_MANAGER_ALL_ACCESS,
        DWORD dwServiceType=SERVICE_WIN32_OWN_PROCESS,
        DWORD dwStartType=SERVICE_DEMAND_START,
        DWORD dwErrorControl=SERVICE_ERROR_NORMAL,
        LPDWORD lpdwTagId=NULL,
        LPCWSTR lpDependencies=NULL,
        LPCWSTR lpServiceStartName=NULL,
        LPCWSTR lpPassword=NULL
        );
    BOOL Delete(LPCWSTR lpServicName=NULL);
    BOOL Start(LPCWSTR lpServicName);
    BOOL Stop(LPCWSTR lpServicName);
	BOOL Exists( LPCWSTR lpServicName );
    BOOL QueryStatus(LPCWSTR lpServicName, DWORD dwQueryStatus);
private:
    BOOL OpenScm(
        LPCWSTR lpMachineName=NULL,
        LPCWSTR lpDatabaseName=NULL,
        DWORD dwDesiredAccess=SC_MANAGER_ALL_ACCESS
        );
    BOOL GetBinPathName(LPWSTR lpOutPath, LPCWSTR lpInPath);
    BOOL QueryStatus(SC_HANDLE hService, DWORD dwQueryStatus, DWORD dwStatusPending);
private:
    SC_HANDLE  m_hScm;
    SC_HANDLE  m_hSvr;
    WCHAR m_strSvrName[MAX_PATH];
};

class DR_CRC
{
public:
    DR_CRC(void);
    ~DR_CRC(void);
public:
    DWORD CalcCRC(LPVOID buffer, UINT size);
    ULONG LDM_Priv_header_checksum(char *buf);

private:
    void Calculate (const LPBYTE buffer, UINT size, ULONG &crc);
    ULONG Reflect(ULONG ref, char ch);
    ULONG CheckSum(CONST char *buf, ULONG len);
private:
    ULONG Table[256];
};

#define CRC32_POLYNOMIAL		0XEDB88320L
class ASAG_CRC
{
public:
	ASAG_CRC( );
	
	~ASAG_CRC( );

	VOID				CSSCheckSum(PCHAR buffer, DWORD count, DWORD *pdwCRC);

protected:
	VOID				BuildCRCTable();

protected:
	DWORD				CRCTable[256];
};

//===================================Functions====================================
//
//Functions
//
BOOL ExeCmd(LPCWSTR lpCmd, WORD uCmdShow, BOOL bAsync = FALSE, DWORD dwTimeOut = 30000);
BOOL ExeCmd(LPCWSTR lpCmd, std::vector<std::wstring>& vecOutputs,BOOL bShowWindow=FALSE );
//Added by zouyu01 on 10/19/2009
BOOL WINAPI IsInDREnv();

void EliminatebackslashW(LPWSTR lpInPath);
void EliminatebackslashStrW(std::wstring &strInPath);

//=========================================================
//Description:
//	adjust the process priviledge
//Parameters:
//	lpPrivName:priviledge name, it value referrence MSDN
//	bEnablePriv, TRUE, enable the priviledge; FALSE, disable the priviledge
//Return Values:
//	TRUE, success
//	FALSE, failed, use GetLastError() to get the error code
//
BOOL AdjustPriviledge(LPTSTR lpPrivName, BOOL bEnablePriv, PTOKEN_PRIVILEGES pTokOldState = NULL);

BOOL RestorePriviledge(LPWSTR lpPrivName, PTOKEN_PRIVILEGES pTokOldState);

//Added by zouyu01 on 5/20/2009
/*
*Desciption:
*  Check whether the give process exists.
*@pszProcessName: Process name.
*@bExist: Boolean to show whether the give process exists.
*@return value: zero for success. Otherwise, system standard error code will be returned.
*/
DWORD CheckProcessExistByName(const WCHAR *pszProcessName, BOOL &bExist);

/*
*Desciption:
*  Check whether the give process exists.
*@pszProcessName: Process name.
*@bExist: Boolean to show whether the give process exists.
*@dwProcessID: The process ID when the given process exists.
*@return value: zero for success. Otherwise, system standard error code will be returned.
*/
DWORD CheckProcessExistByNameEx(const WCHAR *pszProcessName, BOOL &bExist, DWORD &dwProcessID);


DWORD WINAPI DRGetVddkInstallPath(std::wstring &strPath);

DWORD WINAPI GetVddkInstallBinPath(std::wstring &strPath);

DWORD WINAPI GetVddkPathFromD2D(std::wstring &strPath);
//
//Give a full file path, it will return the logical sector size of the disk which
//the file locates on. the file can locate on a locate volume or a remote volume
//
DWORD WINAPI GetMediaLogicalSectorSize(LPCWSTR lpFileName);

DWORD AddAceToObjectsSecurityDescriptor (
    LPWSTR pszObjName,          // name of object
    SE_OBJECT_TYPE ObjectType,  // type of object
    LPWSTR pszTrustee,          // trustee for new ACE
    TRUSTEE_FORM TrusteeForm,   // format of trustee structure
    DWORD dwAccessRights,       // access mask for new ACE
    ACCESS_MODE AccessMode,     // type of ACE
    DWORD dwInheritance         // inheritance flags for new ACE
    );

BOOL IsUserHasRight(
                    LPWSTR pszObjName,          // name of object
                    SE_OBJECT_TYPE ObjectType,  // type of object
                    LPWSTR pszTrustee,          // trustee for new ACE
                    TRUSTEE_FORM TrusteeForm,   // format of trustee structure
                    DWORD dwAccessRights        // access mask for new ACE
                    );


//<xuvji01 added for getting locale info>
/*
 * @param lpShortDatePattern: A pointer to the buffer that receives the short date pattern.
 * @param dwShortDatePatternLen: The size of the buffer pointed to by the lpShortDatePattern parameter.
 * @return 0-success, others-error.
 */
DWORD WINAPI GetShortDatePattern(LPWSTR lpShortDatePattern, DWORD dwShortDatePatternLen);

DWORD WINAPI GetLongDatePattern(LPWSTR lpLongDatePattern, DWORD dwLongDatePatternLen);

DWORD WINAPI GetShortTimePattern(LPWSTR lpLongTimePattern, DWORD dwLongTimePatternLen);

DWORD WINAPI GetLongTimePattern(LPWSTR lpLongTimePattern, DWORD dwLongTimePatternLen);

DWORD WINAPI GetFirstDayOfWeek(DWORD &dwFirstDayOfWeek);

DWORD WINAPI GetAMDesignator(LPWSTR lpAMDesignator, DWORD dwAMDesignatorLen);

DWORD WINAPI GetPMDesignator(LPWSTR lpPMDesignator, DWORD dwPMDesignatorLen);

DWORD WINAPI GetDisplayLanguage(DWORD &dwDisplayLanguageID);

DWORD WINAPI GetThousandSeparator(LPWSTR lpThousandSeparator, DWORD dwThousandSeparatorLen);

DWORD WINAPI GetDecimalSeparator(LPWSTR lpDecimalSeparator, DWORD dwDecimalSeparatorLen);

/*Desciption:
*  Formats a number string as a number string customized for a locale specified by identifier. For example, given "-12345.67", returns "-12,345.67"
*@lpValue: Pointer to a null-terminated string containing the number string to format.
*			This string can only contain the following characters, else the function will return an error.
*				Characters "0" through "9".
*				One decimal point (dot) if the number is a floating-point value.
*				A minus sign in the first character position if the number is a negative value.
*@lpNumberStr: Pointer to a buffer in which this function retrieves the formatted number string.
*@dwNumberStrLen: The size for the number string buffer indicated by lpNumberStr.
*/
DWORD WINAPI GetNumberFormatString(LPCWSTR lpValue, LPWSTR lpNumberStr, DWORD dwNumberStrLen);

//@lpDate: Pointer to a SYSTEMTIME structure that contains the date information to format. If it is NULL, use the current local system date.
DWORD WINAPI GetShortDateFormatString(const SYSTEMTIME *lpDate, LPWSTR lpDateStr, DWORD dwDateStrLen);

DWORD WINAPI GetLongDateFormatString(const SYSTEMTIME *lpDate, LPWSTR lpDateStr, DWORD dwDateStrLen);

//@lpTime: Pointer to a SYSTEMTIME structure that contains the time information to format. If it is NULL, use the current local system time.
DWORD WINAPI GetLongTimeFormatString(const SYSTEMTIME *lpTime, LPWSTR lpTimeStr, DWORD dwTimeStrLen);
//</xuvji01 added for getting locale info>

//xuvji01 added for loading resource 
HMODULE WINAPI LoadResourceModule( LPCWSTR lpszResource, LANGID lid = 0 );


//xuvji01 added for getting install path by product type from register table
typedef enum _PRODUCT_TYPE_
{
    PRODUCT_D2D,
	PRODUCT_CENTRAL,
	PRODUCT_RPS
}PRODUCT_TYPE;

DWORD WINAPI GetInstallPathByProduct(int nProductType, LPWSTR pInstallPath, DWORD dwLen);

//This function is getting install path post dr, because after bmr, the drive letter may be changed.
DWORD WINAPI GetInstallPathByProductPostDR(int nProductType, LPWSTR pInstallPath, DWORD dwLen);

//get register root path by product type, the format is like "SOFTWARE\CA\CA ARCserve D2D"
DWORD WINAPI GetRegRootPathByProduct(int nProductType, LPWSTR pRegRootPath, DWORD dwLen);

// detect if the server is RPS server or D2D agent only
BOOL  WINAPI IsRPSServerEnabled( );

// get the relative path of a specified session number, like "VStore\S0000000008"
DWORD WINAPI RelativePathOfSession( DWORD dwSessionNumber, LPWSTR pszRelativePath, DWORD dwSizeInCharacters );


// get the UI language
/*Desciption:
*  Get the display language name of UDP GUI.
*  The available languages are: en, zh_CN, zh_TW, ja, fr, de, it, es, pt
* @strInLanguage : Given a default language, detect if this language is available. If not, return an available language.
*/
std::wstring WINAPI GetUILanguage(const std::wstring& strInLanguage = L"");

/*Desciption:
*  Get the display language ID of UDP GUI.
*  The available languages are: 0x409(en), 0x804(zh_CN), 0x404(zh_TW), 0x411(ja), 0x40c(fr), 0x407(de), 0x410(it), 0xc0a(es), 0x416(pt)
* @lid : Given a default language id, detect if this language is available. If not, return an available language id.
*/
LANGID	WINAPI GetUILanguage( LANGID lid=0);