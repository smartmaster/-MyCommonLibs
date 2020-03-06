#include "stdafx.h"

#include "MyAllSystemInfo.h"

#include "MyAllSystemInfo.h.xml.Output.hpp"

CMyAllSystemInfo::CMyAllSystemInfo(LPCTSTR machine /*= nullptr*/, LPCTSTR user /*= nullptr*/, LPCTSTR domain /*= nullptr*/, LPCTSTR password /*= nullptr */) :
System_Info(machine, user, domain, password),
MSFT_DisksPartitionsVolumes(machine, user, domain, password) //,
//m_machine(machine),
//m_user(user),
//m_domain(domain),
//m_password(password)
{

}

HRESULT CMyAllSystemInfo::Retrieve()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	System_Info.Retrieve();
	MSFT_DisksPartitionsVolumes.Retrieve();
	GetAllVolumeDiskInfoXml(AllVolumeDiskInfoXml, FALSE);
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}
