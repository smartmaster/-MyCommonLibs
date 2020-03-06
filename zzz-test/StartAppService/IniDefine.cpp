#include "stdafx.h"
#include "IniDefine.h"
#include "ConstDef.h"


VOID ReadIniFile( IN CONST WCHAR * pFile, TINIFile & Ini )
{
	CONST INT TEMP_BUFFER_LEN = 4096;
	CHeapPtr<WCHAR> apTempBuffer;
	apTempBuffer.Allocate(TEMP_BUFFER_LEN);
	WCHAR * pTempBuffer = apTempBuffer.m_pData;

	//CString INIServiceName;			//#define INI_ServiceName				L"ServiceName"
	pTempBuffer[0] = 0;
	GetPrivateProfileString(INI_Section_Settings,INI_ServiceName,L"StartAppService", pTempBuffer, TEMP_BUFFER_LEN, pFile);
	Ini.SectionSettings.INIServiceName = pTempBuffer;

	//CString INICheckProcessMinutes;	//#define INI_CheckProcessMinutes		L"CheckProcessMinutes"
	pTempBuffer[0] = 0;
	GetPrivateProfileString(INI_Section_Settings,INI_CheckProcessMinutes, L"0", pTempBuffer, TEMP_BUFFER_LEN, pFile);
	Ini.SectionSettings.INICheckProcessMinutes = pTempBuffer;


	//CString INICheckProcessSeconds;	//#define INI_CheckProcessSeconds		L"CheckProcessSeconds"
	pTempBuffer[0] = 0;
	GetPrivateProfileString(INI_Section_Settings,INI_CheckProcessSeconds, L"600", pTempBuffer, TEMP_BUFFER_LEN, pFile);
	Ini.SectionSettings.INICheckProcessSeconds = pTempBuffer;

	//CString INIEndProcessOnServiceStop;			//#define INI_EndProcessOnServiceStop	L"EndProcessOnServiceStop"
	pTempBuffer[0] = 0;
	GetPrivateProfileString(INI_Section_Settings,INI_EndProcessOnServiceStop, L"Yes", pTempBuffer, TEMP_BUFFER_LEN, pFile);
	Ini.SectionSettings.INIEndProcessOnServiceStop = pTempBuffer;

	//CString INIChangeToStartType;			//#define INI_ChangeToStartType		L"ChangeToStartType"
	pTempBuffer[0] = 0;
	GetPrivateProfileString(INI_Section_Settings, INI_ChangeToStartType, L"", pTempBuffer, TEMP_BUFFER_LEN, pFile);
	Ini.SectionSettings.INIChangeToStartType = pTempBuffer;

	//<sonmi01>2012-8-1 ###???
	//CString INIStartProcessInOrder;				//#define INI_StartProcessInOrder				L"StartProcessInOrder"
	pTempBuffer[0] = 0;
	GetPrivateProfileString(INI_Section_Settings, INI_StartProcessInOrder, L"yes", pTempBuffer, TEMP_BUFFER_LEN, pFile);
	Ini.SectionSettings.INIStartProcessInOrder = pTempBuffer;

	//CString INIWaitPreviousProcessTimeOut;		//#define INI_WaitPreviousProcessTimeOut		L"WaitPreviousProcessTimeOut"
	pTempBuffer[0] = 0;
	GetPrivateProfileString(INI_Section_Settings, INI_WaitPreviousProcessTimeOut, L"600", pTempBuffer, TEMP_BUFFER_LEN, pFile); //<sonmi01>2012-9-24 #enlarge 5 min to 10 min
	Ini.SectionSettings.INIWaitPreviousProcessTimeOut = pTempBuffer;

	//CString INIEncryptionEnabled;				//#define INI_EncryptionEnabled					L"EncryptionEnabled"
	pTempBuffer[0] = 0;
	GetPrivateProfileString(INI_Section_Settings, INI_EncryptionEnabled, L"no", pTempBuffer, TEMP_BUFFER_LEN, pFile);
	Ini.SectionSettings.INIEncryptionEnabled = pTempBuffer;

	//CString INIDependencies;					//#define INI_Dependencies						L"Dependencies"
	pTempBuffer[0] = 0;
	GetPrivateProfileString(INI_Section_Settings, INI_Dependencies, L"", pTempBuffer, TEMP_BUFFER_LEN, pFile);
	Ini.SectionSettings.INIDependencies = pTempBuffer;

	//////////////////////////////////////////////////////////////////////////
	Ini.SectionProcessItems.resize(G_MAX_PROC_COUNT);
	for (INT ii = 0; ii<G_MAX_PROC_COUNT; ++ii)
	{
		INISectionProcess & SectionProcess = Ini.SectionProcessItems[ii];

		//#define INI_CommandLine				L"CommandLine"
		CString strSectionProcess;
		strSectionProcess.Format(INI_Section_Process, ii);

		pTempBuffer[0] = 0;
		GetPrivateProfileString(strSectionProcess.GetString(), INI_CommandLine, L"", pTempBuffer, TEMP_BUFFER_LEN, pFile);
		SectionProcess.INICommandLine = pTempBuffer;

		if (SectionProcess.INICommandLine.GetLength() > 4)
		{
			//#define INI_CurrentDirectory		L"CurrentDirectory"
			pTempBuffer[0] = 0;
			GetPrivateProfileString(strSectionProcess.GetString(), INI_CurrentDirectory, L"", pTempBuffer, TEMP_BUFFER_LEN, pFile);
			SectionProcess.INICurrentDirectory = pTempBuffer;

			//#define INI_UserName				L"UserName"
			pTempBuffer[0] = 0;
			GetPrivateProfileString(strSectionProcess.GetString(), INI_UserName, L"", pTempBuffer, TEMP_BUFFER_LEN, pFile);
			SectionProcess.INIUserName = pTempBuffer;

			//#define INI_Password				L"Password"
			pTempBuffer[0] = 0;
			GetPrivateProfileString(strSectionProcess.GetString(), INI_Password, L"", pTempBuffer, TEMP_BUFFER_LEN, pFile);
			SectionProcess.INIPassword = pTempBuffer;

			//#define INI_Domain					L"Domain"
			pTempBuffer[0] = 0;
			GetPrivateProfileString(strSectionProcess.GetString(), INI_Domain, L"", pTempBuffer, TEMP_BUFFER_LEN, pFile);
			SectionProcess.INIDomain = pTempBuffer;

			//#define INI_UserInterface			L"UserInterface"
			pTempBuffer[0] = 0;
			GetPrivateProfileString(strSectionProcess.GetString(), INI_UserInterface, L"N", pTempBuffer, TEMP_BUFFER_LEN, pFile);
			SectionProcess.INIUserInterface = pTempBuffer;

			//#define INI_UserInterfaceOnActiveSession			L"UserInterfaceOnActiveSession"
			pTempBuffer[0] = 0;
			GetPrivateProfileString(strSectionProcess.GetString(), INI_UserInterfaceOnActiveSession, L"N", pTempBuffer, TEMP_BUFFER_LEN, pFile);
			SectionProcess.INIUserInterfaceOnActiveSession = pTempBuffer;

			//#define INI_PauseStart				L"PauseStart"
			pTempBuffer[0] = 0;
			GetPrivateProfileString(strSectionProcess.GetString(), INI_PauseStart,L"100", pTempBuffer, TEMP_BUFFER_LEN, pFile);
			SectionProcess.INIPauseStart = pTempBuffer;

			//#define INI_PauseEnd				L"PauseEnd"
			pTempBuffer[0] = 0;
			GetPrivateProfileString(strSectionProcess.GetString(), INI_PauseEnd,L"100", pTempBuffer, TEMP_BUFFER_LEN, pFile);
			SectionProcess.INIPauseEnd = pTempBuffer;

			//CString INIRestart;				//#define INI_Restart				L"Restart"
			pTempBuffer[0] = 0;
			GetPrivateProfileString(strSectionProcess.GetString(), INI_Restart,L"No", pTempBuffer, TEMP_BUFFER_LEN, pFile);
			SectionProcess.INIRestart = pTempBuffer;
		}
	}
}

VOID WriteIniFile( IN OUT CONST WCHAR * pFile, IN CONST TINIFile & Ini )
{
	//WCHAR pTempBuffer[1024] = {0};
	CString strTempBuffer;

	//CString INIServiceName;			//#define INI_ServiceName				L"ServiceName"
	strTempBuffer.Format(L"\"%s\"", Ini.SectionSettings.INIServiceName.GetString());
	WritePrivateProfileString(INI_Section_Settings, INI_ServiceName, strTempBuffer.GetString(), pFile);

	//CString INICheckProcessMinutes;	//#define INI_CheckProcessMinutes		L"CheckProcessMinutes"
	if (Ini.SectionSettings.INICheckProcessMinutes.GetLength())
	{
		strTempBuffer.Format(L"\"%s\"", Ini.SectionSettings.INICheckProcessMinutes.GetString());
		WritePrivateProfileString(INI_Section_Settings, INI_CheckProcessMinutes, strTempBuffer.GetString(), pFile);
	}
	

	//CString INICheckProcessSeconds;	//#define INI_CheckProcessSeconds		L"CheckProcessSeconds"
	if (Ini.SectionSettings.INICheckProcessSeconds.GetLength())
	{
		strTempBuffer.Format(L"\"%s\"", Ini.SectionSettings.INICheckProcessSeconds.GetString());
		WritePrivateProfileString(INI_Section_Settings, INI_CheckProcessSeconds, strTempBuffer.GetString(), pFile);
	}

	//CString INIEndProcessOnServiceStop;			//#define INI_EndProcessOnServiceStop	L"EndProcessOnServiceStop"
	if (Ini.SectionSettings.INIEndProcessOnServiceStop.GetLength())
	{
		strTempBuffer.Format(L"\"%s\"", Ini.SectionSettings.INIEndProcessOnServiceStop.GetString());
		WritePrivateProfileString(INI_Section_Settings, INI_EndProcessOnServiceStop, strTempBuffer.GetString(), pFile);
	}


	//CString INIChangeToStartType;			//#define INI_ChangeToStartType		L"ChangeToStartType"
	if (Ini.SectionSettings.INIChangeToStartType.GetLength())
	{
		strTempBuffer.Format(L"\"%s\"", Ini.SectionSettings.INIChangeToStartType.GetString());
		WritePrivateProfileString(INI_Section_Settings, INI_ChangeToStartType, strTempBuffer.GetString(), pFile);
	}


	//<sonmi01>2012-8-1 ###???
	//CString INIStartProcessInOrder;				//#define INI_StartProcessInOrder				L"StartProcessInOrder"
	if (Ini.SectionSettings.INIStartProcessInOrder.GetLength())
	{
		strTempBuffer.Format(L"\"%s\"", Ini.SectionSettings.INIStartProcessInOrder.GetString());
		WritePrivateProfileString(INI_Section_Settings, INI_StartProcessInOrder, strTempBuffer.GetString(), pFile);
	}

	//CString INIWaitPreviousProcessTimeOut;		//#define INI_WaitPreviousProcessTimeOut		L"WaitPreviousProcessTimeOut"
	if (Ini.SectionSettings.INIWaitPreviousProcessTimeOut.GetLength())
	{
		strTempBuffer.Format(L"\"%s\"", Ini.SectionSettings.INIWaitPreviousProcessTimeOut.GetString());
		WritePrivateProfileString(INI_Section_Settings, INI_WaitPreviousProcessTimeOut, strTempBuffer.GetString(), pFile);
	}


	//CString INIEncryptionEnabled;				//#define INI_EncryptionEnabled					L"EncryptionEnabled"
	if (Ini.SectionSettings.INIEncryptionEnabled.GetLength())
	{
		strTempBuffer.Format(L"\"%s\"", Ini.SectionSettings.INIEncryptionEnabled.GetString());
		WritePrivateProfileString(INI_Section_Settings, INI_EncryptionEnabled, strTempBuffer.GetString(), pFile);
	}


	//CString INIDependencies;					//#define INI_Dependencies						L"Dependencies"
	if (Ini.SectionSettings.INIDependencies.GetLength())
	{
		strTempBuffer.Format(L"\"%s\"", Ini.SectionSettings.INIDependencies.GetString());
		WritePrivateProfileString(INI_Section_Settings, INI_Dependencies, strTempBuffer.GetString(), pFile);
	}


	//////////////////////////////////////////////////////////////////////////
	for (SIZE_T ii = 0; ii<Ini.SectionProcessItems.size(); ++ii)
	{
		CONST INISectionProcess & SectionProcess = Ini.SectionProcessItems[ii];
		
		CString strSectionProcess;
		strSectionProcess.Format(INI_Section_Process, ii);

		if (SectionProcess.INICommandLine.GetLength() > 4)
		{
			//#define INI_CommandLine				L"CommandLine"
			strTempBuffer.Format(L"\"%s\"", SectionProcess.INICommandLine.GetString());
			WritePrivateProfileString(strSectionProcess.GetString(), INI_CommandLine, strTempBuffer.GetString(), pFile);

			//#define INI_CurrentDirectory		L"CurrentDirectory"
			if (SectionProcess.INICurrentDirectory.GetLength() > 0)
			{
				strTempBuffer.Format(L"\"%s\"", SectionProcess.INICurrentDirectory.GetString());
				WritePrivateProfileString(strSectionProcess.GetString(), INI_CurrentDirectory, strTempBuffer.GetString(), pFile);
			}

			//#define INI_UserName				L"UserName"
			if (SectionProcess.INIUserName.GetLength() > 0)
			{
				strTempBuffer.Format(L"\"%s\"", SectionProcess.INIUserName.GetString());
				WritePrivateProfileString(strSectionProcess.GetString(), INI_UserName, strTempBuffer.GetString(), pFile);
			}

			//#define INI_Password				L"Password"
			if (SectionProcess.INIPassword.GetLength() > 0)
			{
				strTempBuffer.Format(L"\"%s\"", SectionProcess.INIPassword.GetString());
				WritePrivateProfileString(strSectionProcess.GetString(), INI_Password, strTempBuffer.GetString(), pFile);
			}

			//#define INI_Domain					L"Domain"
			if (SectionProcess.INIDomain.GetLength() > 0)
			{
				strTempBuffer.Format(L"\"%s\"", SectionProcess.INIDomain.GetString());
				WritePrivateProfileString(strSectionProcess.GetString(), INI_Domain, strTempBuffer.GetString(), pFile);
			}

			//#define INI_UserInterface			L"UserInterface"
			if (SectionProcess.INIUserInterface.GetLength() > 0)
			{
				strTempBuffer.Format(L"\"%s\"", SectionProcess.INIUserInterface.GetString());
				WritePrivateProfileString(strSectionProcess.GetString(), INI_UserInterface, strTempBuffer.GetString(), pFile);
			}

			//#define INI_UserInterfaceOnActiveSession			L"UserInterfaceOnActiveSession"
			if (SectionProcess.INIUserInterfaceOnActiveSession.GetLength() > 0)
			{
				strTempBuffer.Format(L"\"%s\"", SectionProcess.INIUserInterfaceOnActiveSession.GetString());
				WritePrivateProfileString(strSectionProcess.GetString(), INI_UserInterfaceOnActiveSession, strTempBuffer.GetString(), pFile);
			}

			//#define INI_PauseStart				L"PauseStart"
			if (SectionProcess.INIPauseStart.GetLength() > 0)
			{
				strTempBuffer.Format(L"\"%s\"", SectionProcess.INIPauseStart.GetString());
				WritePrivateProfileString(strSectionProcess.GetString(), INI_PauseStart, strTempBuffer.GetString(), pFile);
			}

			//#define INI_PauseEnd				L"PauseEnd"
			if (SectionProcess.INIPauseEnd.GetLength() > 0)
			{
				strTempBuffer.Format(L"\"%s\"", SectionProcess.INIPauseEnd.GetString());
				WritePrivateProfileString(strSectionProcess.GetString(), INI_PauseEnd, strTempBuffer.GetString(), pFile);
			}

			//CString INIRestart;				//#define INI_Restart				L"Restart"
			if (SectionProcess.INIRestart.GetLength() > 0)
			{
				strTempBuffer.Format(L"\"%s\"", SectionProcess.INIRestart.GetString());
				WritePrivateProfileString(strSectionProcess.GetString(), INI_Restart, strTempBuffer.GetString(), pFile);
			}
		}
	}
}
