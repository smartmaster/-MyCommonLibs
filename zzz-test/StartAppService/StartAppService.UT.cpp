#include "stdafx.h"


#ifdef ___UNIT_TEST___

#include "IniDefine.h"
#include "DebugLog.h"

#include "StartEndApp.h"
#include "ServiceHandlerAndMain.h"
#include "ServiceManager.h"
#include "RestartWorker.h"
#include "UtililityAndHelper.h"

#include "StartAppOnActiveSession.h"

LONG GetEncPassword(OUT CString & StrPassword);

INT _tmain_002(INT argc, _TCHAR* argv[])
{

	//StartAppOnActiveSession(argv[1]);
	//DWORD ProcessID = -1;
	//DWORD SessID = _ttoi(argv[2]);

	//do 
	//{
	//	HRESULT hr = FindProcessOnSession(argv[1], SessID, ProcessID);
	//	if (FAILED(hr))
	//	{
	//		break;
	//	}

	//	HANDLE hTemp = NULL;
	//	hr = DuplicateProcessToken(ProcessID, hTemp);
	//	if (FAILED(hr))
	//	{
	//		break;
	//	}

	//	CHandle ahProcessTokenDup(hTemp); hTemp = NULL;
	//	hr = AdjustProcessToken(ahProcessTokenDup, SE_DEBUG_NAME, SessID);
	//	if (FAILED(hr))
	//	{
	//		break;
	//	}

	//	CString strCmdLine = L"\"F:\\temp temp temp temp\\~~~notepad.exe\"";
	//	hr = StartAppAsUser(ahProcessTokenDup, strCmdLine.GetBuffer());
	//	if (FAILED(hr))
	//	{
	//		break;
	//	}

	//} while (FALSE);


	return 0;
}

INT _tmain_001(INT argc, _TCHAR* argv[])
{
//	MyChangeServiceStartType(argv[1], SERVICE_DEMAND_START);
	return 0;
}

INT _tmain_004(INT argc, _TCHAR* argv[])
{
	TINIFile ini;
	ReadIniFile(argv[1], ini);
	WriteIniFile(argv[2], ini);
	return 0;
}

#include "EncryptString.h"

INT _tmain_003(INT argc, _TCHAR* argv[])
{
	
	CString StrPassword;
	GetEncPassword(StrPassword);

	wstring EncryptedText;
	EncryptString(StrPassword.GetString(), argv[2], EncryptedText);	

	wstring PlainText;
	DecryptString(StrPassword.GetString(), EncryptedText, PlainText);
	return 0;
}


INT _tmain_005(int argc, TCHAR * agv[])
{
	CString StrPassword;
	GetEncPassword(StrPassword);
	return 0;
}

INT _tmain_006(INT argc, _TCHAR* argv[])
{
	if (argc < 2)
	{
		_ftprintf_s(stdout, TEXT("Usage: this.exe <PlainText>") TEXT("\r\n"));
		return -1;
	}

	CString StrPassword;
	GetEncPassword(StrPassword);

	wstring EncryptedText;
	EncryptString(StrPassword.GetString(), argv[1], EncryptedText);	

	wstring PlainText;
	DecryptString(StrPassword.GetString(), EncryptedText, PlainText);

	_ftprintf_s(stdout, TEXT("Input:\t%s") TEXT("\r\n"), argv[1]);
	_ftprintf_s(stdout, TEXT("Encrypted:\t%s") TEXT("\r\n"), EncryptedText.c_str());
	_ftprintf_s(stdout, TEXT("Decrypted:\t%s") TEXT("\r\n"), PlainText.c_str());
	return 0;
}

#include "UtililityAndHelper.h"
INT _tmain_007(INT argc, _TCHAR* argv[])
{
	LPCTSTR psz = TEXT("\"111\"  \"222\"  \"333\"  \"444\"  \"555\"  \"\"  \"zzz\"");
	LPTSTR pBuffer = NULL;
	QuotedStringsToBuffer(psz, &pBuffer);
	FreeQuotedStringsBuffer(pBuffer); pBuffer = NULL;
	return 0;
}

//<sonmi01>2012-9-24 ###???
#include "RunCount.h"
INT _tmain_008(INT argc, _TCHAR* argv[])
{
	//CString strPath;
	//__GetModulePath(NULL, TRUE, strPath);

	//CString strFilePath;
	//__GetModulePath(NULL, FALSE, strFilePath);


	//LPCTSTR pIni = TEXT("c:\\xxx.ini");
	//__ReadRestartCount(pIni);
	//__WriteRestartCount(pIni, 9);

	DecrementRestartCount(3);

	return 0;
}

INT _tmain(INT argc, _TCHAR* argv[])
{
	return _tmain_008(argc, argv);
	return _tmain_007(argc, argv);
	return _tmain_006(argc, argv);
	//return _tmain_005(argc, argv);
	return _tmain_003(argc, argv);
	return _tmain_004(argc, argv);
}


#endif //___UNIT_TEST___