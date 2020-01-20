#pragma once

#include "stdafx.h"
#include <vector>
#include <map>
using namespace std;

//******************************************
#define PTR_TO_STRING(ptr) ((ptr)? (ptr) : TEXT(""))
#define STRING_TO_PTR(str) (((NULL == (str)) || (0 == (str)[0]))? NULL : (str))

//******************************************
#define VALID_HANLE(h) ((NULL != h) && (INVALID_HANDLE_VALUE != h))

//******************************************
static CONST TCHAR STG_ITEM_COUNT_NAME[] = TEXT("StgItemCount");
static CONST TCHAR STG_ITEM_NAME[] = TEXT("StgItem[%u]");

//******************************************
struct CMiscHelper
{
	static VOID NumberToString( LONGLONG llNumber, CString & strNum );
	static INT MultiStringToStringVector( LPCTSTR szStrs, DWORD cchMax, vector<CString> & strs );
	static INT StringVectorToMultiString( LPCTSTR * ppszStrs, CONST vector<CString> & strs );

	static VOID AppendTail(CString & str, TCHAR chTail);
	static VOID RemoveTail(CString & str, TCHAR chTail);

	static HRESULT StringFromGuid(CString & strGuid, CONST GUID & Guid);
	static HRESULT GuidFromString(GUID & Guid, LPCTSTR pszGuid);
	static HRESULT CreateGuid(GUID & Guid, CString & strGuid);

	static BOOL IsVolumeGuidMatch(LPCTSTR pszVolGuid1, LPCTSTR pszVolGuid2);
	static BOOL IsDiskNumberMatch(ULONG ulDiskNumber, LPCTSTR pszDiskName);

	static VOID ReplaceInvalidFileNameChar(CString& str, TCHAR chNew);

	static BOOL MatchPattern(LPCTSTR pszFileName, LPCTSTR pszPattern, TCHAR chAll = TEXT('*'), TCHAR chSingle = TEXT('?'), BOOL bIgnoreCase = TRUE);
};

struct CInvertDataRange
{
	//start offset, end offset (Note OFFSET)
	static VOID InvertDataRangeHelper(CONST map<LONGLONG, LONGLONG> & dataRange, map<LONGLONG, LONGLONG> & invertedDataRange);
	static VOID GetDiskNonDataRange(DRIVE_LAYOUT_INFORMATION_EX * pDriveLayoutInformationEx, LONGLONG llDiskSize, map<LONGLONG, LONGLONG> & NonDataRange);
	static VOID AdjustDataRangeLengthLimit(CONST map<LONGLONG, LONGLONG> & DiskMetaDataRange, map<LONGLONG, LONGLONG> & AdjustedDiskMetaDataRange, LONGLONG llLengthLimit);
};


struct CRemoterInstanceHelper
{
	static VOID InitializeCOAUTHIDENTITY(
		COAUTHIDENTITY & AuthIdentity,
		LPTSTR pszUser,
		LPTSTR Domain,
		LPTSTR Password
		);

	static VOID InitializeCOAUTHINFO (
		COAUTHINFO & AuthInfo, 
		COAUTHIDENTITY * pAuthIdentityData,
		DWORD dwAuthnSvc = RPC_C_AUTHN_NONE,
		DWORD dwAuthzSvc= RPC_C_AUTHZ_NONE,
		LPWSTR pwszServerPrincName = NULL,
		DWORD dwAuthnLevel =  RPC_C_AUTHN_LEVEL_NONE,
		DWORD dwImpersonationLevel = RPC_C_IMP_LEVEL_IMPERSONATE,
		DWORD dwCapabilities = EOAC_NONE
		);

	static VOID InitializeCOSERVERINFO(COSERVERINFO & ServerInfo, COAUTHINFO * pAuthInfo, LPTSTR pszName);

	template<class QI>
	static HRESULT MyCoCreateInstance(
		REFCLSID rclsid,
		IUnknown * punkOuter,
		DWORD dwClsCtx,
		COSERVERINFO * pServerInfo,
		QI ** ppQI
		)
	{
		HRESULT hr = S_OK;
		do 
		{
			//******************************************
			MULTI_QI  mqiResults = {0};
			mqiResults.pIID = &__uuidof(QI);
			hr = ::CoCreateInstanceEx(
				rclsid,//REFCLSID rclsid,
				punkOuter,//IUnknown * punkOuter,
				dwClsCtx,//DWORD dwClsCtx,
				pServerInfo,//COSERVERINFO * pServerInfo,
				1,//ULONG cmq,
				&mqiResults//MULTI_QI * pResults
				);
			if (FAILED(hr) || FAILED(mqiResults.hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("CoCreateInstanceEx"), hr));
				break;
			}

			//******************************************
			CComPtr<IUnknown> spIUnknown;
			spIUnknown.Attach(mqiResults.pItf); mqiResults.pItf = NULL;
			hr = spIUnknown.QueryInterface(ppQI);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("QueryInterface"), hr));
				break;
			}

		} while (FALSE);

		return hr;
	}
};
