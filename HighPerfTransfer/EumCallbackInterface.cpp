#include "stdafx.h"

#include "..\PublicHeader\PublicDTraceLib.h"

#include "EumCallbackInterface.h"


CEchoEumCallback::CEchoEumCallback(LONG & fileCount) :
m_fileCount(fileCount)
{

}

HRESULT CEchoEumCallback::OnEnumedFiles(CONST BODY_ENUM_FILE_DATA_RSP & body_enum_file_data_rsp)
{
	CONST vector<BODY_FILE_DESCRITION> & Files = body_enum_file_data_rsp.m_Files;
	for (size_t ii = 0; ii < Files.size(); ++ii)
	{
		D_INFO(0, TEXT("%08d\t%s"), m_fileCount, Files[ii].m_FileName.GetString());
		++m_fileCount;
	}
	return S_OK;
}


CNewEchoEumCallback::CNewEchoEumCallback(LONG & fileCount) :
m_fileCount(fileCount)
{

}

HRESULT CNewEchoEumCallback::OnEnumedFiles(CONST BODY_ENUM_FILE_DATA_RSP & body_enum_file_data_rsp)
{
	CONST vector<BODY_FILE_DESCRITION> & Files = body_enum_file_data_rsp.m_Files;
	for (size_t ii = 0; ii < Files.size(); ++ii)
	{
		D_INFO(0, TEXT("new echo %08d\t%s"), m_fileCount, Files[ii].m_FileName.GetString());
		++m_fileCount;
	}
	return S_OK;
}