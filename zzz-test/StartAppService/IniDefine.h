#pragma once

#include "stdafx.h"

#include <vector>
using namespace std;

//////////////////////////////////////////////////////////////////////////
#define INI_Section_Settings				L"Settings"

#define INI_ServiceName						L"ServiceName"
#define INI_CheckProcessMinutes				L"CheckProcessMinutes"
#define INI_CheckProcessSeconds				L"CheckProcessSeconds"
#define INI_EndProcessOnServiceStop			L"EndProcessOnServiceStop"
#define INI_ChangeToStartType				L"ChangeToStartType"
#define INI_StartProcessInOrder				L"StartProcessInOrder"
#define INI_WaitPreviousProcessTimeOut		L"WaitPreviousProcessTimeOut"
#define INI_EncryptionEnabled				L"EncryptionEnabled"
#define INI_Dependencies					L"Dependencies" //<sonmi01>2012-8-8 ###???

struct INISectionSettings	//#define INI_Section_Settings		L"Settings"
{
	CString INIServiceName;						//#define INI_ServiceName						L"ServiceName"
	CString INICheckProcessMinutes;				//#define INI_CheckProcessMinutes				L"CheckProcessMinutes"
	CString INICheckProcessSeconds;				//#define INI_CheckProcessSeconds				L"CheckProcessSeconds"
	CString INIEndProcessOnServiceStop;			//#define INI_EndProcessOnServiceStop			L"EndProcessOnServiceStop"
	CString INIChangeToStartType;				//#define INI_ChangeToStartType					L"ChangeToStartType"
	CString INIStartProcessInOrder;				//#define INI_StartProcessInOrder				L"StartProcessInOrder"
	CString INIWaitPreviousProcessTimeOut;		//#define INI_WaitPreviousProcessTimeOut		L"WaitPreviousProcessTimeOut"
	CString INIEncryptionEnabled;				//#define INI_EncryptionEnabled					L"EncryptionEnabled"
	CString INIDependencies;					//#define INI_Dependencies						L"Dependencies"
};


//////////////////////////////////////////////////////////////////////////
#define INI_Section_Process						L"Process%u\0"

#define INI_CommandLine							L"CommandLine"
#define INI_CurrentDirectory					L"CurrentDirectory"
#define INI_UserName							L"UserName"
#define INI_Password							L"Password"
#define INI_Domain								L"Domain"
#define INI_UserInterface						L"UserInterface"
#define INI_UserInterfaceOnActiveSession		L"UserInterfaceOnActiveSession"
#define INI_PauseStart							L"PauseStart"
#define INI_PauseEnd							L"PauseEnd"
#define INI_Restart								L"Restart"


struct INISectionProcess	//#define INI_Section_Process			L"Process%u\0"
{
	CString INICommandLine;							//#define INI_CommandLine			L"CommandLine"
	CString INICurrentDirectory;					//#define INI_CurrentDirectory		L"CurrentDirectory"
	CString INIUserName;							//#define INI_UserName				L"UserName"
	CString INIPassword;							//#define INI_Password				L"Password"
	CString INIDomain;								//#define INI_Domain				L"Domain"
	CString INIUserInterface;						//#define INI_UserInterface			L"UserInterface"
	CString INIUserInterfaceOnActiveSession;		//#define INI_UserInterfaceOnActiveSession			L"UserInterfaceOnActiveSession"
	CString INIPauseStart;							//#define INI_PauseStart			L"PauseStart"
	CString INIPauseEnd;							//#define INI_PauseEnd				L"PauseEnd"
	CString INIRestart;								//#define INI_Restart				L"Restart"
};


//////////////////////////////////////////////////////////////////////////
struct TINIFile
{
	INISectionSettings SectionSettings;
	vector<INISectionProcess> SectionProcessItems;
};


//////////////////////////////////////////////////////////////////////////
VOID ReadIniFile(IN CONST WCHAR * pFile, OUT TINIFile & Ini);
VOID WriteIniFile(IN OUT CONST WCHAR * pFile, IN CONST TINIFile & Ini);