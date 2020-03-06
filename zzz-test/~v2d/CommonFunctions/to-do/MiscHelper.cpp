#include "stdafx.h"
#include "MiscHelper.h"

VOID CMiscHelper::NumberToString( LONGLONG llNumber, CString & strNum )
{
	CONST INT BASE = 1000;

	do 
	{
		//********************************************************************
		if (llNumber == 0)
		{
			strNum = TEXT("0");
			break;
		}

		//********************************************************************
		if (llNumber < 0)
		{
			strNum += TEXT('-');
			llNumber = -llNumber;
		}

		//********************************************************************
		vector<INT> vecNum;
		while (llNumber)
		{
			vecNum.push_back(llNumber%BASE);
			llNumber /= BASE;
		}

		for (size_t i=0; i<vecNum.size(); ++i)
		{
			if (i==0)
			{
				strNum.AppendFormat(TEXT("%d,"), vecNum[vecNum.size()-i-1]);
			}
			else
			{
				strNum.AppendFormat(TEXT("%03d,"), vecNum[vecNum.size()-i-1]);
			}			
		}

	} while (FALSE);

	strNum.Trim(TEXT(", "));
}

INT CMiscHelper::MultiStringToStringVector( LPCTSTR szStrs, DWORD cchMax, vector<CString> & strs )
{
	INT nStatus = 0;

	do 
	{
		//******************************************************************************
		if (NULL == szStrs || 0 == cchMax)
		{
			nStatus = E_INVALIDARG;
			_OLD_DAPIERR((999, DRNPOS, TEXT(__FUNCTION__), E_INVALIDARG));
			break;
		}

		//******************************************************************************
		LPCTSTR pch = szStrs;
		while (pch[0])
		{
			strs.push_back(pch);

			size_t cch = 0;
			cch = _tcsnlen(pch, cchMax);

			pch += (cch + 1);
		}
		if (nStatus)
		{
			break;
		}

	} while (FALSE);

	return nStatus;
}

HRESULT CMiscHelper::StringFromGuid( CString & strGuid, CONST GUID & Guid )
{
	LPTSTR szBuffer = strGuid.GetBuffer(256);
	INT nChars = ::StringFromGUID2(
		Guid,//REFGUID rguid,
		szBuffer,//LPOLESTR lpsz,
		256//int cchMax
		);
	strGuid.ReleaseBuffer();

	return (0==nChars? E_FAIL : S_OK);
}

HRESULT CMiscHelper::GuidFromString( GUID & Guid, LPCTSTR pszGuid )
{
	CComBSTR bstrGuid = pszGuid;
	HRESULT hr = ::IIDFromString(
		bstrGuid,//LPOLESTR lpsz,
		&Guid//LPIID lpiid
		);
	return hr;
}

HRESULT CMiscHelper::CreateGuid( GUID & Guid, CString & strGuid )
{
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		hr = ::CoCreateGuid(
			&Guid//GUID * pguid
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CoCreateGuid"), hr));
			break;
		}

		//******************************************
		hr = CMiscHelper::StringFromGuid(strGuid, Guid);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CMiscHelper::StringFromGuid"), hr));
			break;
		}

	} while (FALSE);

	return hr;
}

VOID CMiscHelper::AppendTail( CString & str, TCHAR chTail )
{
	str.TrimRight(chTail);
	str += chTail;
}

INT CMiscHelper::StringVectorToMultiString( LPCTSTR * ppszStrs, CONST vector<CString> & strs )
{
	INT nStatus = 0;

	do 
	{
		INT nCharCount = 1;
		for (size_t ii=0; ii<strs.size(); ++ii)
		{
			nCharCount += (strs[ii].GetLength() + 1);
		}

		CHeapPtr<TCHAR> apBuffer;
		apBuffer.Allocate(nCharCount);
		if (NULL == (TCHAR*)apBuffer)
		{
			//_OLD_DSETLASTSTATUS(E_OUTOFMEMORY);
			nStatus = E_OUTOFMEMORY;
			//_OLD_DAPIERR((0, DRNPOS, TEXT("Allocate"), E_OUTOFMEMORY));
			break;
		}


		LPTSTR pszBuffer = (TCHAR*)apBuffer;
		ZeroMemory(pszBuffer, nCharCount*sizeof(TCHAR));
		for (size_t ii=0; ii<strs.size(); ++ii)
		{
			CONST CString & str = strs[ii];
			size_t nCurrentCount = str.GetLength() + 1;
			_tcscpy_s(pszBuffer, nCurrentCount, str.GetString());
			pszBuffer +=nCurrentCount;
		}

		*ppszStrs = apBuffer.Detach();

	} while (FALSE);

	return nStatus;
}

VOID CMiscHelper::RemoveTail( CString & str, TCHAR chTail )
{
	str.TrimRight(chTail);
}

BOOL CMiscHelper::IsVolumeGuidMatch( LPCTSTR pszVolGuid1, LPCTSTR pszVolGuid2 )
{
	CString strGuid1 = pszVolGuid1;
	CString strGuid2 = pszVolGuid2;
	CONST TCHAR SZ_TO_TRIM[] = TEXT("\\/?.");
	strGuid1.Trim(SZ_TO_TRIM);
	strGuid2.Trim(SZ_TO_TRIM);
	return (0 == strGuid1.CompareNoCase(strGuid2));
}

BOOL CMiscHelper::IsDiskNumberMatch( ULONG ulDiskNumber, LPCTSTR pszDiskName )
{
	CONST TCHAR SZ_TO_TRIM[] = TEXT("\\/?.");
	CONST TCHAR SZ_PHYSICAL_DRIVE[] = TEXT("PhysicalDrive%u");

	CString strDiskName = pszDiskName;
	strDiskName.Trim(SZ_TO_TRIM);

	CString strDiskNumber;
	strDiskName.Format(SZ_PHYSICAL_DRIVE, ulDiskNumber);

	return (0 == strDiskNumber.CompareNoCase(strDiskName) );
}

VOID CMiscHelper::ReplaceInvalidFileNameChar( CString& str, TCHAR chNew )
{
	CONST TCHAR SZ_INVALID_CHARS[] = TEXT("<>:\"/\\|?*");
	for (INT ii=0; ii<_countof(SZ_INVALID_CHARS)-1; ++ii)
	{
		str.Replace(SZ_INVALID_CHARS[ii], chNew);
	}
}

BOOL CMiscHelper::MatchPattern( LPCTSTR pszFileNameParam, LPCTSTR pszPatternParam, TCHAR chAll, TCHAR chSingle, BOOL bIgnoreCase)
{
	LPCTSTR pszFileName = pszFileNameParam;
	LPCTSTR pszPattern = pszPatternParam;

	CString strFileName;
	CString strPattern;
	if (bIgnoreCase)
	{
		strFileName = pszFileNameParam;
		strFileName.MakeUpper();

		strPattern = pszPatternParam;
		strPattern.MakeUpper();

		pszFileName = strFileName.GetString();
		pszPattern = strPattern.GetString();
	}

	// Compare until pattern is not '*' and not '?'
	while (*pszFileName && *pszPattern)
	{
		if (*pszPattern == chAll || *pszPattern == chSingle)
			break;

		if (*pszFileName != *pszPattern)
			return FALSE;

		pszFileName ++;
		pszPattern ++;
	}

	if (*pszPattern == 0)
	{
		if (*pszFileName == 0)
			return TRUE;	// Exactly matched
		else
			return FALSE;
	}
	else if (*pszPattern == chSingle)
	{
		do
		{
			if (*pszFileName == 0)
				return FALSE;
			else
			{
				pszFileName ++;	// Skip to next
				pszPattern ++;
			}
		} while (*pszPattern == chSingle);

		return MatchPattern(pszFileName, pszPattern);
	}
	else if (*pszPattern == chAll)
	{
		LPCTSTR p = pszPattern;

		// Skip to next character, not '?' and not '*"
		pszPattern ++;
		while (*pszPattern)
		{
			if (*pszPattern == chAll || *pszPattern == chSingle)
				pszPattern ++;
			else
				break;
		}

		while (*pszFileName && *pszFileName != *pszPattern)
			pszFileName ++;
		if (*pszFileName != *pszPattern)
			return FALSE;
		else
		{
			if (*pszPattern)
			{
				LPCTSTR pszFN = pszFileName;
				LPCTSTR pszPT = pszPattern;

				while (*pszFileName == *pszPT)
					pszFileName ++;
				while (*pszPattern == *pszPT)
					pszPattern ++;

				if (*pszFileName == *pszPattern && *pszFileName == 0 && (pszFileName-pszFN) >= (pszPattern-pszPT))
					return TRUE;

				if (MatchPattern(pszFN+1, p))
					return TRUE;
				else
					return MatchPattern(pszFN, pszPT);
			}
			else
				return TRUE;
		}
	}
	else
		return FALSE;
}


VOID CInvertDataRange::InvertDataRangeHelper( CONST map<LONGLONG, LONGLONG> & dataRange, map<LONGLONG, LONGLONG> & invertedDataRange )
{
	for (map<LONGLONG, LONGLONG>::const_iterator citer = dataRange.begin();
		citer != dataRange.end();
		++citer)
	{
		map<LONGLONG, LONGLONG>::const_iterator citer2 = citer;
		++citer2;
		if (dataRange.end() == citer2)
		{
			break;
		}
		invertedDataRange.insert(make_pair(citer->second, citer2->first));
	}
}

VOID CInvertDataRange::GetDiskNonDataRange( DRIVE_LAYOUT_INFORMATION_EX * pDriveLayoutInformationEx, LONGLONG llDiskSize, map<LONGLONG, LONGLONG> & NonDataRange )
{
	map<LONGLONG, LONGLONG> dataRange;
	dataRange.insert(make_pair(0, 0));
	dataRange.insert(make_pair(llDiskSize, llDiskSize));

	ULONG nPartitionCount = pDriveLayoutInformationEx->PartitionCount;
	PPARTITION_INFORMATION_EX pPartitionEntry = pDriveLayoutInformationEx->PartitionEntry;
	for (ULONG ii=0; ii<nPartitionCount; ++ii)
	{
		CONST PARTITION_INFORMATION_EX & PartitionEntry = pPartitionEntry[ii];
		if (0 != PartitionEntry.PartitionNumber && 0 != PartitionEntry.PartitionLength.QuadPart)
		{
			dataRange.insert(make_pair(PartitionEntry.StartingOffset.QuadPart, PartitionEntry.StartingOffset.QuadPart + PartitionEntry.PartitionLength.QuadPart));
		}
	}

	InvertDataRangeHelper(dataRange, NonDataRange);
}

VOID CInvertDataRange::AdjustDataRangeLengthLimit( CONST map<LONGLONG, LONGLONG> & DiskMetaDataRange, map<LONGLONG, LONGLONG> & AdjustedDiskMetaDataRange, LONGLONG llLengthLimit )
{
	for (map<LONGLONG, LONGLONG>::const_iterator citer = DiskMetaDataRange.begin(); 
		citer != DiskMetaDataRange.end();
		++citer)
	{
		LONGLONG llLength = citer->second - citer->first;
		if (llLength <= llLengthLimit)
		{
			AdjustedDiskMetaDataRange.insert(*citer);
		}
		else
		{
			AdjustedDiskMetaDataRange.insert(make_pair(citer->first, citer->first + llLengthLimit/2));
			AdjustedDiskMetaDataRange.insert(make_pair(citer->second - llLengthLimit/2, citer->second));
		}
	}
}

VOID CRemoterInstanceHelper::InitializeCOAUTHINFO( COAUTHINFO & AuthInfo, COAUTHIDENTITY * pAuthIdentityData, DWORD dwAuthnSvc /*= RPC_C_AUTHN_NONE*/, DWORD dwAuthzSvc/*= RPC_C_AUTHZ_NONE*/, LPWSTR pwszServerPrincName /*= NULL*/, DWORD dwAuthnLevel /*= RPC_C_AUTHN_LEVEL_NONE*/, DWORD dwImpersonationLevel /*= RPC_C_IMP_LEVEL_IMPERSONATE*/, DWORD dwCapabilities /*= EOAC_NONE */ )
{
	AuthInfo.dwAuthnSvc = dwAuthnSvc;
	AuthInfo.dwAuthzSvc = dwAuthzSvc;
	AuthInfo.pwszServerPrincName = pwszServerPrincName;
	AuthInfo.dwAuthnLevel = dwAuthnLevel;
	AuthInfo.dwImpersonationLevel = dwImpersonationLevel;
	AuthInfo.pAuthIdentityData = pAuthIdentityData;
	AuthInfo.dwCapabilities = dwCapabilities;
}

VOID CRemoterInstanceHelper::InitializeCOSERVERINFO( COSERVERINFO & ServerInfo, COAUTHINFO * pAuthInfo, LPTSTR pszName )
{
	ZeroMemory(&ServerInfo, sizeof(ServerInfo));
	ServerInfo.pwszName = pszName;
	ServerInfo.pAuthInfo = pAuthInfo;
}

VOID CRemoterInstanceHelper::InitializeCOAUTHIDENTITY( COAUTHIDENTITY & AuthIdentity, LPTSTR pszUser, LPTSTR Domain, LPTSTR Password )
{
	ZeroMemory(&AuthIdentity, sizeof(COAUTHIDENTITY));

	AuthIdentity.User = (USHORT*)pszUser; 
	if (pszUser)
	{
		AuthIdentity.UserLength = _tcslen(pszUser);
	}

	AuthIdentity.Domain = (USHORT*)Domain; 
	if (Domain)
	{
		AuthIdentity.DomainLength = _tcslen(Domain);
	}

	AuthIdentity.Password = (USHORT*)Password; 
	if (Password)
	{
		AuthIdentity.PasswordLength = _tcslen(Password); 
	}

	AuthIdentity.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;
}