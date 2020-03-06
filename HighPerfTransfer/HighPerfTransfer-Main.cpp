// HighPerfTransfer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"






#include "HPTSendReceiver.h"
#include "EventHelper.h"
#include "CommandList.h"
#include "JobMainFlowbInterface.h"
#include "DowloadClient.h"
#include "UploadClient.h"
#include "RunAppClient.h"
#include "EnumClient.h"

#include "PluginFileFactoryConfig.h"

int _tmain_basic(int argc, _TCHAR* argv[])
{

	D_SETTINGS(999, FALSE, NULL, NULL);

	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CHPTSendReceive HPTSendReceive(NULL, NULL, FALSE, NULL, TRUE);
	hr = HPTSendReceive.CreateMember();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT(""), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	for (;;)
	{
		HPTSendReceive.WaitToEnd(5000);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = HPTSendReceive.StopProcess();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT(""), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = HPTSendReceive.DestroyMember();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT(""), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
		
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
	
}

int _tmain_event_helper(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IMultipleEventHelper> spIMultipleEventHelper;
	static CONST BOOL bManualReset[] = { TRUE, TRUE, TRUE, FALSE };
	hr = CreateInstanceCMultipleEventHelper(bManualReset, _countof(bManualReset), &spIMultipleEventHelper, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCMultipleEventHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = spIMultipleEventHelper->Set(2);
	hr = spIMultipleEventHelper->Reset(2);
	hr = spIMultipleEventHelper->Set(_countof(bManualReset) - 1);

	DWORD dwWait = spIMultipleEventHelper->Wait(2, 3000);
	DWORD dwWaitAll = spIMultipleEventHelper->WaitMultiple(TRUE, 3000);
	DWORD dwWaitAny = spIMultipleEventHelper->WaitMultiple(FALSE, 3000);
	dwWait = spIMultipleEventHelper->Wait(_countof(bManualReset) - 1, 0);
	ATLASSERT(dwWait == WAIT_TIMEOUT);

	spIMultipleEventHelper.Release();
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}


int _tmain_to_xml(int argc, _TCHAR* argv[])
{
	//////////////////////////////////////////////////////////////////////////
	{
		PluginFileFactoryConfigs configs;
		configs.Configs.resize(2);
		for (auto & var : configs.Configs)
		{
			var.FileType = 1;
			var.FileTypeString = TEXT(".vhdx");
			var.IsRelativePath = TRUE;
			var.ModulePath = TEXT("PluginFileFactoryVHDX.dll");
		}
		configs.ToFile(FIELD_NAME(PluginFileFactoryConfigs), TEXT("PluginFileFactoryConfigs.xml"));
	}

	//////////////////////////////////////////////////////////////////////////
	LONG aa = CMD_END_SESSION_REQ;
	LONG bb = CMD_END_ENUM_FILE_RSP;


	CString strXML;
	
	struct BODY_RESULT_RSP data_result_rsp;
	strXML.Empty();
	data_result_rsp.ToString(MethodString(BODY_RESULT_RSP), strXML);


	struct BODY_FILE_DESCRITION data_file_descrition;
	strXML.Empty();
	data_file_descrition.ToString(MethodString(BODY_FILE_DESCRITION), strXML);

	struct  BODY_CREATE_DOWNLOAD_SESSION_REQ data_creae_download_session_req;
	data_creae_download_session_req.m_CilentFiles.resize(10);
	data_creae_download_session_req.m_ServerFiles.resize(10);
	strXML.Empty();
	data_creae_download_session_req.ToString(MethodString(BODY_CREATE_DOWNLOAD_SESSION_REQ), strXML);
	struct  BODY_CREATE_DOWNLOAD_SESSION_REQ data_creae_download_session_req_1;
	data_creae_download_session_req_1.FromString(MethodString(BODY_CREATE_DOWNLOAD_SESSION_REQ), strXML);

	struct BODY_BEGIN_ENUM_FILE_REQ body_begin_enum_file_req;
	strXML.Empty();
	body_begin_enum_file_req.ToString(MethodString(BODY_BEGIN_ENUM_FILE_REQ), strXML);

	struct BODY_ENUM_FILE_DATA_RSP data_enum_file_data_rsp;
	data_enum_file_data_rsp.m_Files.resize(5);
	strXML.Empty();
	data_enum_file_data_rsp.ToString(MethodString(BODY_ENUM_FILE_DATA_RSP), strXML);
	struct  BODY_ENUM_FILE_DATA_RSP data_enum_file_data_rsp_1;
	data_enum_file_data_rsp_1.FromString(MethodString(BODY_ENUM_FILE_DATA_RSP), strXML);

	return 0;
}

//server port 
//sonmi01-hv12 14159 
static HRESULT CreateSocketClient(int argc, TCHAR ** argv, ISocketClient ** ppobj)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	//BOOL bUseSsl = TRUE;
	BOOL bUseSsl = FALSE;
	CString CertFile; // = TEXT("F:\\MyProjects\\~MyCommonLibs\\PublicLibExtern\\cert\\sslclientcert.pem");
	CString PrivateKeyFile; // = TEXT("F:\\MyProjects\\~MyCommonLibs\\PublicLibExtern\\cert\\sslclientkey.pem");
	CString VerifyPeerCert; // = TEXT("F:\\MyProjects\\~MyCommonLibs\\PublicLibExtern\\cert\\cacert.pem");
	CString KeyFilePwd; // = TEXT("1qaz");
	GetClientSslCertKeyFiles(CertFile, PrivateKeyFile, VerifyPeerCert, KeyFilePwd);
	if (bUseSsl)
	{
		hr = CreateInstanceCSslSocketClient(
			argv[1],//LPCTSTR pszServer,
			argv[2],//LPCTSTR pPort,
			ppobj,//ISocketClient ** ppObj,
			FALSE,//BOOL bSendAll,
			FALSE,//BOOL bReceiveAll,
			CertFile.GetString(),//LPCTSTR pCertFile,
			PrivateKeyFile.GetString(),//LPCTSTR PrivateKeyFile,
			VerifyPeerCert.GetString(),//LPCTSTR pVerifyPeerCert,
			KeyFilePwd.GetString(),
			TRUE//BOOL bCreateMember
			);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("CreateInstanceCSslSocketClient"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	else
	{
		hr = CreateInstanceCSocketClient(argv[1], argv[2], ppobj, FALSE, FALSE, TRUE);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("CreateInstanceCSocketClient"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

int _tmain_test_download_upload(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<ISocketClient> spISocketClient;
	hr = CreateSocketClient(argc, argv, &spISocketClient);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateSocketClient"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	BODY_BEGIN_ENUM_FILE_REQ body_begin_enum_file_req;
	body_begin_enum_file_req.m_Directory = TEXT("c:\\windows\\system32");
	body_begin_enum_file_req.m_FilePattern = TEXT("*.dll");
	body_begin_enum_file_req.m_NotFilePattern = TEXT("*s*");
	body_begin_enum_file_req.m_bRecursively = TRUE;

	LONG fileCount = 0;
	CNewEchoEumCallback NewEchoEumCallback(fileCount);
	hr = HPTEnumClient(spISocketClient.p, spISocketClient.p, &NewEchoEumCallback, body_begin_enum_file_req);
	if (FAILED(hr))
	{
		//DrainSocket(spISocketClient.p, 15 * 1000);
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("HPTEnumClient"), hr, TEXT(""));
		//LEAVE_BLOCK(0);

		spISocketClient.Release();
		CreateSocketClient(argc, argv, &spISocketClient);
	}

	//////////////////////////////////////////////////////////////////////////
	BODY_RUN_APP_REQ body_run_app_req;
	body_run_app_req.m_Username = TEXT("administrator");
	body_run_app_req.m_Domain = TEXT("sonmi01-790");
	body_run_app_req.m_Password = TEXT("Master16516336");
	body_run_app_req.m_CommandLine = TEXT("calc.exe");
	body_run_app_req.m_CurDir = TEXT("q:\\");
	body_run_app_req.m_TimeOut = 1000 * 60 * 3;
	body_run_app_req.m_TerminateOnTimeOut = TRUE;
	hr = HPTRunAppClient(spISocketClient.p, spISocketClient.p, body_run_app_req);
	if (FAILED(hr))
	{
		//DrainSocket(spISocketClient.p, 15 * 1000);
		D_API_ERR(0, TEXT("HPTRunAppClient"), hr, TEXT(""));
		//LEAVE_BLOCK(0);

		spISocketClient.Release();
		CreateSocketClient(argc, argv, &spISocketClient);
	}

	//quick download for test
	{
		BODY_CREATE_DOWNLOAD_SESSION_REQ data_creae_download_session_req;
		data_creae_download_session_req.m_CilentFiles.resize(1);
		data_creae_download_session_req.m_ServerFiles.resize(1);

		CString strFileName = TEXT("e:\\cccc\\fast-server-disk-volume-info.xml");
		CComPtr<IFileDevice> spRaw;
		HRESULT hr = CreateInstanceFileDeviceWin32(
			strFileName,//IN LPCTSTR strFileName,
			GENERIC_WRITE,//IN ULONG ulDesiredAccess,
			0,//IN ULONG ulShareMode,
			CREATE_ALWAYS,//IN ULONG ulCreationDisposition,
			0,//IN ULONG ulFlagsAndAttributes,
			&spRaw,//OUT IFileDevice ** ppFileDevice,
			TRUE//BOOL bCreateMember
			);

		CreateInstanceRawIFileDeviceXml RawIFileDeviceXml(spRaw);

		data_creae_download_session_req.m_CilentFiles[0].m_FileName = strFileName;
		data_creae_download_session_req.m_CilentFiles[0].m_FileType = PluginFileFactoryConfig_NS::rawIFileDevice.FileType;
		RawIFileDeviceXml.ToString(FIELD_NAME(CreateInstanceRawIFileDeviceXml), data_creae_download_session_req.m_CilentFiles[0].m_MoreContext);

		data_creae_download_session_req.m_ServerFiles[0].m_FileName = TEXT("server-disk-volume-info");
		data_creae_download_session_req.m_ServerFiles[0].m_FileType = PluginFileFactoryConfig_NS::fileCommandDiskVolumeInfo.FileType;

		hr = HPTDownloadClient(spISocketClient.p, spISocketClient.p, data_creae_download_session_req);
		if (FAILED(hr))
		{
			//DrainSocket(spISocketClient.p, 15 * 1000);
			D_API_ERR(0, TEXT("HPTDownloadClient"), hr, TEXT(""));
			//LEAVE_BLOCK(0);

			spISocketClient.Release();
			CreateSocketClient(argc, argv, &spISocketClient);
		}
	}

	//quick upload for test
	{
		BODY_CREATE_DOWNLOAD_SESSION_REQ data_creae_download_session_req;
		data_creae_download_session_req.m_CilentFiles.resize(1);
		data_creae_download_session_req.m_ServerFiles.resize(1);


		data_creae_download_session_req.m_CilentFiles[0].m_FileName = TEXT("client-disk-volume-info");
		data_creae_download_session_req.m_CilentFiles[0].m_FileType = PluginFileFactoryConfig_NS::fileCommandDiskVolumeInfo.FileType;


		data_creae_download_session_req.m_ServerFiles[0].m_FileName = TEXT("q:\\ssss\\fast-client-disk-volume-infop.xml");
		data_creae_download_session_req.m_ServerFiles[0].m_FileType = 0;// PluginFileFactoryConfig_NS::fileTest.FileType;


		//////////////////////////////////////////////////////////////////////////
		hr = HPTUploadClient(spISocketClient.p, spISocketClient.p, data_creae_download_session_req);
		if (FAILED(hr))
		{
			//DrainSocket(spISocketClient.p, 15 * 1000);
			D_API_ERR(0, TEXT("HPTUploadClient"), hr, TEXT(""));
			//LEAVE_BLOCK(0);

			spISocketClient.Release();
			CreateSocketClient(argc, argv, &spISocketClient);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	BODY_CREATE_DOWNLOAD_SESSION_REQ data_creae_download_session_req;
	data_creae_download_session_req.m_CilentFiles.resize(5);
	data_creae_download_session_req.m_CilentFiles[0].m_FileName = TEXT("e:\\cccc\\1.zip");
	data_creae_download_session_req.m_CilentFiles[0].m_FileType = 0; //PluginFileFactoryConfig_NS::fileTest.FileType;

	data_creae_download_session_req.m_CilentFiles[1].m_FileName = TEXT("e:\\cccc\\2.zip");
	data_creae_download_session_req.m_CilentFiles[1].m_FileType = PluginFileFactoryConfig_NS::fileTest.FileType;

	data_creae_download_session_req.m_CilentFiles[2].m_FileName = TEXT("e:\\cccc\\3.zip");
	data_creae_download_session_req.m_CilentFiles[2].m_FileType = PluginFileFactoryConfig_NS::fileTest.FileType;
	
	data_creae_download_session_req.m_CilentFiles[3].m_FileName = TEXT("e:\\cccc\\2.vhdx");
	data_creae_download_session_req.m_CilentFiles[3].m_FileType = PluginFileFactoryConfig_NS::vhdx.FileType;
	CreateInstanceCDiskImageVhdxParamsXml clientPraram  = 
	{
		nullptr,//parentVhdxFileName;
		TEXT("e:\\cccc\\2.vhdx"),//vhdxFileName;
		GENERIC_WRITE,//,//DesiredAccess;
		CREATE_ALWAYS,//CreationDisposition;
		64LL * 1024LL * 1024LL * 1024LL * 1024LL,//VirtualDiskSize;
		512,//LogicalSectorSize;
		4096,//PhysicalSectorSize;
		0,//BlockSize;
		0//VhdxDiskType;
	};
	clientPraram.ToString(FIELD_NAME(CreateInstanceCDiskImageVhdxParamsXml), data_creae_download_session_req.m_CilentFiles[3].m_MoreContext);

	data_creae_download_session_req.m_CilentFiles[4].m_FileName = TEXT("e:\\cccc\\server-disk-volume-info.xml");
	data_creae_download_session_req.m_CilentFiles[4].m_FileType = 0; //PluginFileFactoryConfig_NS::fileTest.FileType;

	//////////////////////////////////////////////////////////////////////////
	data_creae_download_session_req.m_ServerFiles.resize(5);
	data_creae_download_session_req.m_ServerFiles[0].m_FileName = TEXT("q:\\ssss\\1.zip");
	data_creae_download_session_req.m_ServerFiles[0].m_FileType = PluginFileFactoryConfig_NS::fileTest.FileType;

	data_creae_download_session_req.m_ServerFiles[1].m_FileName = TEXT("q:\\ssss\\2.zip");
	data_creae_download_session_req.m_ServerFiles[1].m_FileType = 0;// PluginFileFactoryConfig_NS::fileTest.FileType;

	data_creae_download_session_req.m_ServerFiles[2].m_FileName = TEXT("q:\\ssss\\3.zip");
	data_creae_download_session_req.m_ServerFiles[2].m_FileType = PluginFileFactoryConfig_NS::fileTest.FileType;

	if (0)
	{
		data_creae_download_session_req.m_ServerFiles[3].m_FileName = TEXT("q:\\ssss\\3.zip");
		data_creae_download_session_req.m_ServerFiles[3].m_FileType = PluginFileFactoryConfig_NS::fileTest.FileType;
	}
	else
	{
		data_creae_download_session_req.m_ServerFiles[3].m_FileName = TEXT("q:\\ssss\\2.vhdx");
		data_creae_download_session_req.m_ServerFiles[3].m_FileType = PluginFileFactoryConfig_NS::vhdx.FileType;
		CreateInstanceCDiskImageVhdxParamsXml serverPraram =
		{
			nullptr,//parentVhdxFileName;
			TEXT("q:\\ssss\\2.vhdx"),//vhdxFileName;
			GENERIC_READ,//DesiredAccess;
			OPEN_EXISTING,//CreationDisposition;
			0,//,//VirtualDiskSize;
			0,//LogicalSectorSize;
			0,//PhysicalSectorSize;
			0,//BlockSize;
			0//VhdxDiskType;
		};
		serverPraram.ToString(FIELD_NAME(CreateInstanceCDiskImageVhdxParamsXml), data_creae_download_session_req.m_ServerFiles[3].m_MoreContext);
	}

	

	data_creae_download_session_req.m_ServerFiles[4].m_FileName = TEXT("server-disk-volume-info");
	data_creae_download_session_req.m_ServerFiles[4].m_FileType = PluginFileFactoryConfig_NS::fileCommandDiskVolumeInfo.FileType;
	
	//////////////////////////////////////////////////////////////////////////
	hr = HPTDownloadClient(spISocketClient.p, spISocketClient.p, data_creae_download_session_req);
	if (FAILED(hr))
	{
		//DrainSocket(spISocketClient.p, 15 * 1000);
		D_API_ERR(0, TEXT("HPTDownloadClient"), hr, TEXT(""));
		//LEAVE_BLOCK(0);

		spISocketClient.Release();
		CreateSocketClient(argc, argv, &spISocketClient);
	}

	//////////////////////////////////////////////////////////////////////////
	data_creae_download_session_req.m_ServerFiles[0].m_FileName = TEXT("q:\\ssss-u\\1.zip");
	data_creae_download_session_req.m_ServerFiles[0].m_FileType = PluginFileFactoryConfig_NS::fileTest.FileType;

	data_creae_download_session_req.m_ServerFiles[1].m_FileName = TEXT("q:\\ssss-u\\2.zip");
	data_creae_download_session_req.m_ServerFiles[1].m_FileType = PluginFileFactoryConfig_NS::fileTest.FileType;

	data_creae_download_session_req.m_ServerFiles[2].m_FileName = TEXT("q:\\ssss-u\\3.zip");
	data_creae_download_session_req.m_ServerFiles[2].m_FileType = 0;// PluginFileFactoryConfig_NS::fileTest.FileType;

	if (0)
	{
		data_creae_download_session_req.m_ServerFiles[3].m_FileName = TEXT("q:\\ssss-u\\3.zip");
		data_creae_download_session_req.m_ServerFiles[3].m_FileType = 0;
	}
	else
	{
		data_creae_download_session_req.m_ServerFiles[3].m_FileName = TEXT("q:\\ssss-u\\2.vhdx");
		data_creae_download_session_req.m_ServerFiles[3].m_FileType = PluginFileFactoryConfig_NS::vhdx.FileType;
		CreateInstanceCDiskImageVhdxParamsXml serverPraram2 =
		{
			nullptr,//parentVhdxFileName;
			TEXT("q:\\ssss-u\\2.vhdx"),//vhdxFileName;
			GENERIC_WRITE,//DesiredAccess;
			CREATE_ALWAYS,//CreationDisposition;
			64LL * 1024LL * 1024LL * 1024LL * 1024LL,//VirtualDiskSize;
			512,//LogicalSectorSize;
			4096,//PhysicalSectorSize;
			0,//BlockSize;
			0//VhdxDiskType;
		};
		data_creae_download_session_req.m_ServerFiles[3].m_MoreContext.Empty();
		serverPraram2.ToString(FIELD_NAME(CreateInstanceCDiskImageVhdxParamsXml), data_creae_download_session_req.m_ServerFiles[3].m_MoreContext);
	}
	


	data_creae_download_session_req.m_CilentFiles[4].m_FileName = TEXT("client-disk-volume-info");
	data_creae_download_session_req.m_CilentFiles[4].m_FileType = PluginFileFactoryConfig_NS::fileCommandDiskVolumeInfo.FileType; //PluginFileFactoryConfig_NS::fileTest.FileType;

	data_creae_download_session_req.m_ServerFiles[4].m_FileName = TEXT("q:\\ssss-u\\client-disk-volume-info.xml");
	data_creae_download_session_req.m_ServerFiles[4].m_FileType = 0;

	//////////////////////////////////////////////////////////////////////////
	hr = HPTUploadClient(spISocketClient.p, spISocketClient.p, data_creae_download_session_req);
	if (FAILED(hr))
	{
		//DrainSocket(spISocketClient.p, 15 * 1000);
		D_API_ERR(0, TEXT("HPTDownloadClient"), hr, TEXT(""));
		//LEAVE_BLOCK(0);

		spISocketClient.Release();
		CreateSocketClient(argc, argv, &spISocketClient);
	}

	spISocketClient->Disconnect();
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}


int _tmain(int argc, _TCHAR* argv[])
{
	D_SETTINGS(99, FALSE, NULL, NULL);
	WSADATA WSAData = { 0 };
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	int testCase = 3;
	switch (testCase)
	{
	case 4:
	{

	}
	break;
	case 3:
	{
		return _tmain_test_download_upload(argc, argv);
	}
	break;
	case 2:
	{
		return _tmain_to_xml(argc, argv);
	}
	break;
	case 1:
	{
		return _tmain_event_helper(argc, argv);
	}
	break;
	case 0:
	{
		return  _tmain_basic(argc, argv);
	}
	break;
	}

	WSACleanup();
	return 0;
}

