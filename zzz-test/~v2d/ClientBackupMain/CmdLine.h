#pragma once

#include "stdafx.h"

struct CCommandLine 
{
	CString m_strDestination;
	CString m_strMachine;
	CString m_strUser;
	CString m_strDomain;
	CString m_strPassword;
	vector<CString> m_strVolumes;

	//******************************************
	INT Parse(INT argc, TCHAR ** argv);
};