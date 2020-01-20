#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include "CommandList.h"


struct IEumCallback
{
	virtual HRESULT OnEnumedFiles(CONST BODY_ENUM_FILE_DATA_RSP & body_enum_file_data_rsp) = 0;
};


class CEchoEumCallback : public IEumCallback
{
private:
	LONG & m_fileCount;
public:
	CEchoEumCallback(LONG & fileCount);
	//struct IEumCallback
	//{
	//	virtual HRESULT OnEnumedFiles(CONST BODY_ENUM_FILE_DATA_RSP & body_enum_file_data_rsp) = 0;
	virtual HRESULT OnEnumedFiles(CONST BODY_ENUM_FILE_DATA_RSP & body_enum_file_data_rsp);
	//};

};


class CNewEchoEumCallback : public IEumCallback
{
private:
	LONG & m_fileCount;
public:
	CNewEchoEumCallback(LONG & fileCount);
	//struct IEumCallback
	//{
	//	virtual HRESULT OnEnumedFiles(CONST BODY_ENUM_FILE_DATA_RSP & body_enum_file_data_rsp) = 0;
	virtual HRESULT OnEnumedFiles(CONST BODY_ENUM_FILE_DATA_RSP & body_enum_file_data_rsp);
	//};

};