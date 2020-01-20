#pragma once

#include "stdafx.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

struct MyProviderInfo
{
	GUID m_ProviderId;
	CString m_pwszProviderName;
	VSS_PROVIDER_TYPE m_eProviderType;
	CString m_pwszProviderVersion;
	GUID m_ProviderVersionId;
	GUID m_ClassId;

	MyProviderInfo(
		const GUID & ProviderId,
		LPCTSTR pwszProviderName,
		VSS_PROVIDER_TYPE eProviderType,
		LPCTSTR pwszProviderVersion,
		const GUID & ProviderVersionId,
		const GUID & ClassId
	) :
		m_ProviderId(ProviderId),
		m_pwszProviderName(pwszProviderName),
		m_eProviderType(eProviderType),
		m_pwszProviderVersion(pwszProviderVersion),
		m_ProviderVersionId(ProviderVersionId),
		m_ClassId(ClassId)
	{
	}
};