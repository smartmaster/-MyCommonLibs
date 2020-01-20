#pragma once

////#include "stdafx.h"////
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"

//******************************************
#define PTR_TO_STRING(ptr) ((ptr)? (ptr) : TEXT(""))
#define STRING_TO_PTR(str) (((NULL == (str)) || (0 == (str)[0]))? NULL : (str))

//******************************************
#define VALID_HANLE(h) ((NULL != h) && (INVALID_HANDLE_VALUE != h))

//******************************************
static CONST TCHAR STG_ITEM_COUNT_NAME[] = TEXT("StgItemCount");
static CONST TCHAR STG_ITEM_NAME[] = TEXT("StgItem[%u]");

//////////////////////////////////////////////////////////////////////////
#define PRINT_LINE(x, ...) _ftprintf_s(stdout, x L"\r\n", __VA_ARGS__)

//******************************************
struct CMiscHelper
{
	//////////////////////////////////////////////////////////////////////////
private:
	static BOOL IsDecNumber(TCHAR ch);
	static LPCTSTR FindDecNumber(LPCTSTR p);
public:
	static VOID ExtractDecNumbers(LPCTSTR psz, std::vector<LONGLONG> & numbers, BOOL includeSign);

private:
	static BOOL IsHexNumber(TCHAR ch);
	static LPCTSTR FindHexNumber(LPCTSTR p);
public:
	static VOID ExtractHexNumbers(LPCTSTR psz, std::vector<LONGLONG> & numbers);

	//////////////////////////////////////////////////////////////////////////
	static CONST CString & NumberToString( LONGLONG llNumber, CString & strNum );
	static INT MultiStringToStringVector( LPCTSTR szStrs, DWORD cchMax, vector<CString> & strs );
	static INT StringVectorToMultiString( LPCTSTR * ppszStrs, CONST vector<CString> & strs );
	static VOID SplitString(CONST CString & StrList, CONST CString & Delim, vector<CString> & vec);
	static HRESULT GetStringParentByLevel( OUT CString & strParent, IN LPCTSTR pString, IN INT Level, TCHAR Delimiter = TEXT('\\') );

	//////////////////////////////////////////////////////////////////////////
	static BOOL IsSameLevelPath(int start, CONST CString & path);
	static BOOL IsParentLevelPath(int start, CONST CString & path);
	//return value: BOOL
	//true - is relative path; false - not a relative path
	static BOOL FindLevel(OUT INT & Level, IN OUT INT & Start, CONST CString & path);
	static HRESULT ConvertRelativeToAbsolutePath(CONST CString & absPath1, CONST CString & relPath, CString & absPath2);

	static VOID AbsoluteWin32PathToVolumepath(LPCTSTR pWin32Path, CString & VolumePath);

	static VOID GetParentRelativePath(CONST CString & strParent, CONST CString & strChild, CString & strRelative);

	//////////////////////////////////////////////////////////////////////////
	static BOOL IsFileExists( LPCTSTR szFile );
	static BOOL IsDirExists(LPCTSTR szFile);
private:	static HRESULT CreateDirectoryRecursivelyOld(LPCTSTR szFullPath);
private:	static HRESULT CreateDirectoryRecursivelyExPrivate(LPCTSTR szFullPath);
public:		static HRESULT CreateDirectoryRecursively(LPCTSTR szFullPath);

	//////////////////////////////////////////////////////////////////////////
	/****************************
	pDestDir must be created/existing first
	resulting same folder structure pSourceDir\*.* pDestDir\*.*
	****************************/
	static HRESULT CopyFilesFlat(LPCTSTR pSourceDir, LPCTSTR pDestDir, LPCTSTR pFilePattern);
	static HRESULT CopyFileTree(LPCTSTR pSourceDir, LPCTSTR pDestDir, LPCTSTR pFilePattern);
	/****************************
	pDestDir need NOT be created/existing
	resulting same folder structure pSourceDir\*.* pDestDir\*.* i.e. copy pSourceDir AS pDestDir
	****************************/
	static HRESULT CopyFileTreeEx(LPCTSTR pSourceDir, LPCTSTR pDestDir, LPCTSTR pFilePattern);


	//////////////////////////////////////////////////////////////////////////
	static HRESULT __AddFileAttrs(LPCTSTR szName, DWORD dwAttr);
	static HRESULT __RemoveFileAttrs(LPCTSTR szName, DWORD dwAttr);
	static HRESULT __DeleteFile(LPCTSTR strFile);
	static HRESULT __RemoveDirectory(LPCTSTR strFile);

	//////////////////////////////////////////////////////////////////////////
	static VOID ReplaceInvalidFileNameChar(CString& str, TCHAR chNew);

	//////////////////////////////////////////////////////////////////////////
	static BOOL CStringEqualNoCase(CONST CString & str1, CONST CString & str2);
	static VOID AppendTail(CString & str, TCHAR chTail);
	static VOID RemoveTail(CString & str, TCHAR chTail);

	//////////////////////////////////////////////////////////////////////////
	static HRESULT StringFromGuid(CString & strGuid, CONST GUID & Guid);
	static HRESULT GuidFromString(GUID & Guid, LPCTSTR pszGuid);
	static HRESULT CreateGuid(GUID & Guid, CString & strGuid);

	//////////////////////////////////////////////////////////////////////////
	static BOOL IsVolumeGuidMatch(LPCTSTR pszVolGuid1, LPCTSTR pszVolGuid2);
	static BOOL IsDiskNumberMatch(ULONG ulDiskNumber, LPCTSTR pszDiskName);

	//////////////////////////////////////////////////////////////////////////
	static BOOL MatchPattern(LPCTSTR pszFileName, LPCTSTR pszPattern, TCHAR chAll = TEXT('*'), TCHAR chSingle = TEXT('?'), BOOL bIgnoreCase = TRUE);

	//////////////////////////////////////////////////////////////////////////
	static VOID CommandLineAddArgs(CString & strCmdLine, LPCTSTR arg, BOOL bAddQuote = TRUE);

	//////////////////////////////////////////////////////////////////////////
	static ULONG_PTR PtrBytesDiff(const void * hi, const void * low);
	static void * PtrBytesAdvance(void * ptr, LONG_PTR diff);
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
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("CoCreateInstanceEx"), hr, TEXT("")); /*0004*/
				break;
			}

			//******************************************
			CComPtr<IUnknown> spIUnknown;
			spIUnknown.Attach(mqiResults.pItf); mqiResults.pItf = NULL;
			hr = spIUnknown.QueryInterface(ppQI);
			if (FAILED(hr))
			{	
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("QueryInterface"), hr, TEXT("")); /*0004*/
				break;
			}

		} while (FALSE);

		return hr;
	}
};
