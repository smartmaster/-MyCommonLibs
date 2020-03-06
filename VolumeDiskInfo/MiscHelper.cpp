#include "stdafx.h"

#include "..\PublicHeader\PublicCodeBlock.h"

#include "MiscHelper.h"
#include "VolumeHelper.h"

BOOL CMiscHelper::IsDecNumber(TCHAR ch)
{
	return ch >= TEXT('0') && ch <= TEXT('9');
}

LPCTSTR CMiscHelper::FindDecNumber(LPCTSTR p)
{
	ATLASSERT(nullptr != p);
	while (*p)
	{
		if (IsDecNumber(*p))
		{
			break;
		}
		++p;
	}
	return p;
}

VOID CMiscHelper::ExtractDecNumbers(LPCTSTR psz, std::vector<LONGLONG> & numbers, BOOL includeSign)
{
	if (nullptr == psz)
	{
		return;
	}

	LPCTSTR p = psz;

	LONGLONG number = 0;

	for (;;)
	{
		p = FindDecNumber(p);
		if (0 == p[0])
		{
			break;
		}

		if (includeSign && p > psz && p[-1] == TEXT('-'))
		{
			--p;
		}

		number = _tcstoll(p, (TCHAR**)(&p), 10);
		numbers.push_back(number);
	}
}

BOOL CMiscHelper::IsHexNumber(TCHAR ch)
{
	return
		(ch >= TEXT('0') && ch <= TEXT('9')) ||
		(ch >= TEXT('a') && ch <= TEXT('f')) ||
		(ch >= TEXT('A') && ch <= TEXT('F'));
}

LPCTSTR CMiscHelper::FindHexNumber(LPCTSTR p)
{
	ATLASSERT(nullptr != p);
	while (*p)
	{
		if (IsHexNumber(*p))
		{
			break;
		}
		++p;
	}
	return p;
}

VOID CMiscHelper::ExtractHexNumbers(LPCTSTR psz, std::vector<LONGLONG> & numbers)
{
	if (nullptr == psz)
	{
		return;
	}

	LPCTSTR p = psz;

	LONGLONG number = 0;

	for (;;)
	{
		p = FindHexNumber(p);
		if (0 == p[0])
		{
			break;
		}

		number = _tcstoll(p, (TCHAR**)(&p), 16);
		numbers.push_back(number);
	}
}

CONST CString & CMiscHelper::NumberToString(LONGLONG llNumber, CString & strNum)
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

	return strNum;
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
			D_API_ERR(999, /*DRNPOS,*/ TEXT(__FUNCTION__), E_INVALIDARG, TEXT("")); /*0010*/
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
			D_SET_LAST_STATUS(hr, -1);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("CoCreateGuid"), hr, TEXT("")); /*0004*/
			break;
		}

		//******************************************
		hr = CMiscHelper::StringFromGuid(strGuid, Guid);
		if (FAILED(hr))
		{	
			D_SET_LAST_STATUS(hr, -1);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("CMiscHelper::StringFromGuid"), hr, TEXT("")); /*0004*/
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
			//D_API_ERR(0, /*DRNPOS,*/ TEXT("Allocate"), E_OUTOFMEMORY));
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

HRESULT CMiscHelper::GetStringParentByLevel( OUT CString & strParent, IN LPCTSTR pString, IN INT Level, TCHAR Delimiter)
{
	HRESULT hr = S_OK;

	strParent = pString;
	strParent.TrimRight(Delimiter);
	LPTSTR pBuffer = strParent.GetBuffer();
	LPTSTR pTemp = NULL;
	for (INT ii = 0; ii < Level; ++ ii)
	{
		pTemp = _tcsrchr(pBuffer, Delimiter);
		if (NULL == pTemp)
		{
			break;
		}

		while (pTemp >= pBuffer && Delimiter == pTemp[0])
		{
			pTemp[0] = 0;
			--pTemp;
		}
	}
	strParent.ReleaseBuffer();

	if (NULL == pTemp)
	{
		strParent.Empty();
		hr = E_INVALIDARG;
	}

	return hr;
}

BOOL CMiscHelper::IsSameLevelPath( int start, CONST CString & path )
{
	/*
	.\ == str
	*/
	if (start + 2 > path.GetLength())
	{
		return FALSE;
	}
	return TEXT('.') == path[start] && TEXT('\\') == path[start + 1];
}

BOOL CMiscHelper::IsParentLevelPath( int start, CONST CString & path )
{
	/*
	..\ == str
	*/
	if (start + 3 > path.GetLength())
	{
		return FALSE;
	}
	return TEXT('.') == path[start] && TEXT('.') == path[start + 1] && TEXT('\\') == path[start + 2];
}

BOOL CMiscHelper::FindLevel( OUT INT & Level, IN OUT INT & Start, CONST CString & path )
{
	BOOL bret = FALSE;
	Level = 0;
	for (;;)
	{
		if (IsSameLevelPath(Start, path))
		{
			bret = TRUE;
			Start += 2;
		}
		else if (IsParentLevelPath(Start, path))
		{
			bret = TRUE;
			Start += 3;
			++ Level;
		}
		else
		{
			break;
		}
	}

	return bret;
}

HRESULT CMiscHelper::ConvertRelativeToAbsolutePath( CONST CString & absPath1, CONST CString & relPath, CString & absPath2 )
{
	HRESULT hr  = S_OK;

	int level = 0;
	int start = 0;

	BOOL bRelative = FindLevel(level, start, relPath);
	if (!bRelative)
	{
		absPath2 = relPath;
	}
	else
	{
		hr = GetStringParentByLevel(absPath2, absPath1.GetString(), level + 1, TEXT('\\'));
		if (SUCCEEDED(hr))
		{
			absPath2 += TEXT('\\');
			absPath2 += relPath.Mid(start);
		}
	}

	return hr;
}

BOOL CMiscHelper::IsFileExists( LPCTSTR szFile )
{
	BOOL bExist = FALSE;
	WIN32_FIND_DATA FindFileData = {0};
	HANDLE hFindFirstFile = FindFirstFile(
		szFile,//__in          LPCTSTR lpFileName,
		&FindFileData//__out         LPWIN32_FIND_DATA lpFindFileData
		);
	if (INVALID_HANDLE_VALUE != hFindFirstFile)
	{
		FindClose(hFindFirstFile);
		hFindFirstFile = INVALID_HANDLE_VALUE;
		bExist = (0 == (FILE_ATTRIBUTE_DIRECTORY & FindFileData.dwFileAttributes));
	}
	return bExist;
}

VOID CMiscHelper::AbsoluteWin32PathToVolumepath( LPCTSTR pWin32Path, CString & VolumePath )
{
	CString strRoot;
	CVolumeHelper::GetVolumeRootAndGuidFromPath(pWin32Path, strRoot, VolumePath);
	if (VolumePath.GetLength())
	{
		VolumePath += (pWin32Path + strRoot.GetLength());
	}
}

VOID CMiscHelper::GetParentRelativePath( CONST CString & strParent, CONST CString & strChild, CString & strRelative )
{
	CString strRootParent;
	CString strVolumeGuidParent;
	CVolumeHelper::GetVolumeRootAndGuidFromPath(strParent, strRootParent, strVolumeGuidParent);

	CString strRootChild;
	CString strVolumeGuidChild;
	CVolumeHelper::GetVolumeRootAndGuidFromPath(strChild, strRootChild, strVolumeGuidChild);

	if (strRootParent.IsEmpty() || strRootChild.IsEmpty() || !CStringEqualNoCase(strRootParent, strRootChild))
	{
		strRelative.Empty();
		return;
	}

	CONST TCHAR DELIMITER = TEXT('\\');
	int startParent = strRootParent.GetLength();
	int startChild = startParent;
	for (;;)
	{
		int p_start = strParent.Find(DELIMITER, startParent);
		int c_start = strChild.Find(DELIMITER, startChild);
		
		if (p_start != c_start || -1 == p_start || -1 == c_start)
		{
			break;
		}

		if (!CStringEqualNoCase(strParent.Left(p_start + 1), strChild.Left(c_start + 1)))
		{
			break;
		}

		startParent = p_start + 1;
		startChild = c_start + 1;
	}

	CString MaxEqSubstr = strParent.Left(startParent);
	MaxEqSubstr.TrimRight(DELIMITER);
	
	int level = 1;
	CString strChildLeft;
	for (;;)
	{
		GetStringParentByLevel( strChildLeft, strChild.GetString(), level, DELIMITER);
		if (CStringEqualNoCase(strChildLeft, MaxEqSubstr))
		{
			break;
		}

		++ level;
	}

	if (1 == level)
	{
		strRelative = TEXT(".\\");
	}
	else 
	{
		strRelative = TEXT("..\\");
		for (int ii = 0; ii < level - 2; ++ ii)
		{
			strRelative += TEXT("..\\");
		}
	}
	strRelative += strParent.Mid(startParent);
}

BOOL CMiscHelper::CStringEqualNoCase( CONST CString & str1, CONST CString & str2 )
{
	return 0 == str1.CompareNoCase(str2);
}

BOOL CMiscHelper::IsDirExists(LPCTSTR szFile)
{
	BOOL bExist = FALSE;
	WIN32_FIND_DATA FindFileData = { 0 };
	HANDLE hFindFirstFile = FindFirstFile(
		szFile,//__in          LPCTSTR lpFileName,
		&FindFileData//__out         LPWIN32_FIND_DATA lpFindFileData
		);
	if (INVALID_HANDLE_VALUE != hFindFirstFile)
	{
		FindClose(hFindFirstFile);
		hFindFirstFile = INVALID_HANDLE_VALUE;
		bExist = (FILE_ATTRIBUTE_DIRECTORY & FindFileData.dwFileAttributes);
	}
	return bExist;
}

HRESULT CMiscHelper::CreateDirectoryRecursivelyOld(LPCTSTR szFullPath)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	TCHAR szVolumePathName[512] = { 0 };
	BOOL bGetVolumePathName = GetVolumePathName(
		szFullPath,//__in          LPCTSTR lpszFileName,
		szVolumePathName,//__out         LPTSTR lpszVolumePathName,
		_countof(szVolumePathName)//__in          DWORD cchBufferLength
		);
	if (!bGetVolumePathName)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("GetVolumePathName"), hr, TEXT("%s"), szFullPath);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CString cstrDirectory = szVolumePathName;
	if (cstrDirectory[cstrDirectory.GetLength() - 1] != TEXT('\\'))
	{
		cstrDirectory += TEXT('\\');
	}

	//////////////////////////////////////////////////////////////////////////
	CONST TCHAR szDelimeter[] = TEXT("\\/");

	CString cstrDirectoryToken;
	BOOL bDirectoryToken = FALSE;
	INT nCountDirectoryToken = 0;
	INT iStartDirectory = 0;
	do
	{
		cstrDirectoryToken = cstrDirectory.Tokenize(szDelimeter, iStartDirectory);
		bDirectoryToken = (cstrDirectoryToken.GetLength() && -1 != iStartDirectory);
		if (bDirectoryToken)
		{
			++nCountDirectoryToken;
		}
	} while (bDirectoryToken);


	//////////////////////////////////////////////////////////////////////////
	CString cstrFullPathToken;
	INT iStartFullPath = 0;
	CString cstrFullPath = szFullPath;
	BOOL bFullPathToken = FALSE;
	INT nCountFullPathToken = 0;
	BOOL bCreateDirectory = TRUE;
	do
	{
		cstrFullPathToken = cstrFullPath.Tokenize(szDelimeter, iStartFullPath);
		bFullPathToken = (cstrFullPathToken.GetLength() && -1 != iStartFullPath);
		if (bFullPathToken)
		{
			++nCountFullPathToken;
			if (nCountFullPathToken > nCountDirectoryToken)
			{
				cstrDirectory += cstrFullPathToken;
				if (!IsDirExists(cstrDirectory.GetString()))
				{
					cstrDirectory += TEXT('\\');
					bCreateDirectory = CreateDirectory(cstrDirectory.GetString(), NULL);
					if (!bCreateDirectory)
					{
						LastError = GetLastError();
						if (ERROR_ALREADY_EXISTS != LastError)
						{
							hr = HRESULT_FROM_WIN32(LastError);
							D_API_ERR(0, TEXT("CreateDirectory"), hr, TEXT("%s"), cstrDirectory.GetString());
							break;
						}
					}
				}
				else
				{
					cstrDirectory += TEXT('\\');
				}
			}
		}
	} while (bFullPathToken);

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

VOID CMiscHelper::SplitString(CONST CString & StrList, CONST CString & Delim, vector<CString> & vec)
{
	int start = 0;
	int end = 0;
	for (;;)
	{
		end = StrList.Find(Delim, start);
		if (-1 == end)
		{
			if (start < StrList.GetLength())
			{
				vec.push_back(StrList.Mid(start));
			}
			break;
		}

		if (end > start)
		{
			vec.push_back(StrList.Mid(start, end - start));
		}

		start = end + Delim.GetLength();
	}
}

VOID CMiscHelper::CommandLineAddArgs(CString & strCmdLine, LPCTSTR arg, BOOL bAddQuote /*= TRUE*/)
{
	if (strCmdLine.IsEmpty())
	{
		if (bAddQuote)
		{
			strCmdLine.AppendFormat(TEXT("\"%s\""), arg);
		}
		else
		{
			strCmdLine.AppendFormat(TEXT("%s"), arg);
		}
	}
	else
	{
		if (bAddQuote)
		{
			strCmdLine.AppendFormat(TEXT(" \"%s\""), arg);
		}
		else
		{
			strCmdLine.AppendFormat(TEXT(" %s"), arg);
		}
	}
}

ULONG_PTR CMiscHelper::PtrBytesDiff(const void * hi, const void * low)
{
	return (ULONG_PTR)(const BYTE*)(hi)-(ULONG_PTR)(const BYTE*)(low);
}

void * CMiscHelper::PtrBytesAdvance(void * ptr, LONG_PTR diff)
{
	return (BYTE*)(ptr)+diff;
}


//////////////////////////////////////////////////////////////////////////
static VOID __AppendTail(CString & str, TCHAR ch)
{
	str.TrimRight(ch);
	str += ch;
}

HRESULT CMiscHelper::CopyFilesFlat(LPCTSTR pSourceDir, LPCTSTR pDestDir, LPCTSTR pFilePattern)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CString strSourceDir = pSourceDir;
	__AppendTail(strSourceDir, TEXT('\\'));

	CString strDestDir = pDestDir;
	__AppendTail(strDestDir, TEXT('\\'));

	//////////////////////////////////////////////////////////////////////////
	WIN32_FIND_DATA FindFileData = { 0 };
	CString strFilePattern = strSourceDir;
	strFilePattern += pFilePattern;
	HANDLE hFind = FindFirstFile(
		strFilePattern.GetString(),//_In_   LPCTSTR lpFileName,
		&FindFileData//_Out_  LPWIN32_FIND_DATA lpFindFileData
		);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CString strSourceFile;
	CString strDestFile;
	BOOL bCopyFile = FALSE;
	do
	{
		if (!(FILE_ATTRIBUTE_DIRECTORY  & FindFileData.dwFileAttributes))
		{
			strSourceFile = strSourceDir;
			strSourceFile += FindFileData.cFileName;

			strDestFile = strDestDir;
			strDestFile += FindFileData.cFileName;

			__DeleteFile(strDestFile.GetString());
			bCopyFile = CopyFile(
				strSourceFile.GetString(),//_In_  LPCTSTR lpExistingFileName,
				strDestFile.GetString(),//_In_  LPCTSTR lpNewFileName,
				FALSE//_In_  BOOL bFailIfExists
				);
			if (!bCopyFile)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				D_API_ERR(0, TEXT("CopyFile"), hr, TEXT("CopyFile failed [%s] --> [%s], hr = 0x%08x"), strSourceFile.GetString(), strDestFile.GetString(), hr);
			}
			else
			{
				D_INFO(0, TEXT("CopyFile [%s] --> [%s]"), strSourceFile.GetString(), strDestFile.GetString());
			}
		}

	} while (FindNextFile(hFind, &FindFileData));

	//////////////////////////////////////////////////////////////////////////
	FindClose(hFind);
	hFind = INVALID_HANDLE_VALUE;
	END_BLOCK(0);

	return hr;
}

HRESULT CMiscHelper::CopyFileTree(LPCTSTR pSourceDir, LPCTSTR pDestDir, LPCTSTR pFilePattern)
{
	HRESULT hr = S_OK;
	DWORD	LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = CopyFilesFlat(pSourceDir, pDestDir, pFilePattern);

	//////////////////////////////////////////////////////////////////////////
	CString strSubSourceDirPattern = pSourceDir;
	__AppendTail(strSubSourceDirPattern, TEXT('\\'));
	strSubSourceDirPattern += TEXT('*');

	WIN32_FIND_DATA FindFileData = { 0 };
	HANDLE hFind = FindFirstFile(
		strSubSourceDirPattern.GetString(),//_In_   LPCTSTR lpFileName,
		&FindFileData//_Out_  LPWIN32_FIND_DATA lpFindFileData
		);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CString strSubSource;
	CString strSubDest;
	BOOL bCreateDirectory = FALSE;
	do
	{
		if ((FILE_ATTRIBUTE_DIRECTORY & FindFileData.dwFileAttributes) &&
			_tcsicmp(TEXT("."), FindFileData.cFileName) &&
			_tcsicmp(TEXT(".."), FindFileData.cFileName))
		{
			strSubSource = pSourceDir;
			__AppendTail(strSubSource, TEXT('\\'));
			strSubSource += FindFileData.cFileName;

			strSubDest = pDestDir;
			__AppendTail(strSubDest, TEXT('\\'));
			strSubDest += FindFileData.cFileName;

			bCreateDirectory = CreateDirectory(strSubDest.GetString(), NULL);
			if (!bCreateDirectory)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				if (ERROR_ALREADY_EXISTS == LastError)
				{
					bCreateDirectory = TRUE;
				}
				else
				{
					D_API_ERR(0, TEXT("CreateDirectory"), hr, TEXT("CreateDirectory failed [%s], hr = 0x%08x"), strSubDest.GetString(), hr);
				}

			}

			if (bCreateDirectory)
			{
				hr = CopyFileTree(strSubSource.GetString(), strSubDest.GetString(), pFilePattern);
			}
		}

	} while (FindNextFile(hFind, &FindFileData));

	//////////////////////////////////////////////////////////////////////////
	FindClose(hFind);
	hFind = INVALID_HANDLE_VALUE;
	END_BLOCK(0);

	return hr;
}

HRESULT CMiscHelper::CopyFileTreeEx(LPCTSTR pSourceDir, LPCTSTR pDestDir, LPCTSTR pFilePattern)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = CreateDirectoryRecursively(pDestDir);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateDirectoryRecursively"), hr, TEXT("%s"), pDestDir);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = CopyFileTree(pSourceDir, pDestDir, pFilePattern);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CopyFileTree"), hr, TEXT("SourceDir=%s, DestDir=%s, FilePattern=%s"), pSourceDir, pDestDir, pFilePattern);
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CMiscHelper::__AddFileAttrs(LPCTSTR szName, DWORD dwAttr)
{
	HRESULT nStatus = ERROR_SUCCESS;

	do
	{
		DWORD dwAttributes = GetFileAttributes(
			szName//__in  LPCTSTR lpFileName
			);
		if (INVALID_FILE_ATTRIBUTES == dwAttributes)
		{
			nStatus = HRESULT_FROM_WIN32(GetLastError());
			D_API_ERR(0, TEXT("GetFileAttributes"), nStatus, TEXT("%s"), szName);
			//nStatus = -1;
			break;
		}

		dwAttributes |= dwAttr;
		BOOL bSetFileAttributes = SetFileAttributes(
			szName,//__in  LPCTSTR lpFileName,
			dwAttributes//__in  DWORD dwFileAttributes
			);
		if (!bSetFileAttributes)
		{
			nStatus = HRESULT_FROM_WIN32(GetLastError());
			D_API_ERR(0, TEXT("SetFileAttributes"), nStatus, TEXT("%s"), szName);
			//nStatus = -1;
			break;
		}

	} while (FALSE);

	return nStatus;
}

HRESULT CMiscHelper::__RemoveFileAttrs(LPCTSTR szName, DWORD dwAttr)
{
	HRESULT nStatus = ERROR_SUCCESS;

	do
	{
		DWORD dwAttributes = GetFileAttributes(
			szName//__in  LPCTSTR lpFileName
			);
		if (INVALID_FILE_ATTRIBUTES == dwAttributes)
		{
			nStatus = HRESULT_FROM_WIN32(GetLastError());
			D_API_ERR(0, TEXT("GetFileAttributes"), nStatus, TEXT("%s"), szName);
			//nStatus = -1;
			break;
		}

		dwAttributes &= (~dwAttr);
		BOOL bSetFileAttributes = SetFileAttributes(
			szName,//__in  LPCTSTR lpFileName,
			dwAttributes//__in  DWORD dwFileAttributes
			);
		if (!bSetFileAttributes)
		{
			nStatus = HRESULT_FROM_WIN32(GetLastError());
			D_API_ERR(0, TEXT("SetFileAttributes"), nStatus, TEXT("%s"), szName);
			//nStatus = -1;
			break;
		}

	} while (FALSE);

	return nStatus;
}

HRESULT CMiscHelper::__DeleteFile(LPCTSTR strFile)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	DWORD FileAttr = (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN);

	hr = __RemoveFileAttrs(strFile, FileAttr);
	if (SUCCEEDED(hr))
	{
		D_INFO(0, TEXT("Remove attributes %s, 0x%08x"), strFile, FileAttr);
	}
	else
	{
		D_API_ERR(0, TEXT("__RemoveFileAttrs"), hr, TEXT("Failed to Remove attributes %s, 0x%08x"), strFile, FileAttr);
	}

	BOOL bDF = ::DeleteFile(strFile);
	if (!bDF)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("DeleteFile"), hr, TEXT("%s"), strFile);
		//LEAVE_BLOCK(0);
	}
	else
	{
		hr = S_OK;
	}

	return hr;
}

HRESULT CMiscHelper::__RemoveDirectory(LPCTSTR strFile)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	DWORD FileAttr = (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN);

	hr = __RemoveFileAttrs(strFile, FileAttr);
	if (SUCCEEDED(hr))
	{
		D_INFO(0, TEXT("Remove attributes %s, 0x%08x"), strFile, FileAttr);
	}
	else
	{
		D_API_ERR(0, TEXT("__RemoveFileAttrs"), hr, TEXT("Failed to Remove attributes %s, 0x%08x"), strFile, FileAttr);
	}

	BOOL bDF = ::RemoveDirectory(strFile);
	if (!bDF)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("RemoveDirectory"), hr, TEXT("%s"), strFile);
		//LEAVE_BLOCK(0);
	}
	else
	{
		hr = S_OK;
	}

	return hr;
}

HRESULT CMiscHelper::CreateDirectoryRecursivelyExPrivate(LPCTSTR szFullPath)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (nullptr == szFullPath || 0 == szFullPath[0])
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("CreateDirectoryRecursivelyExPrivate"), hr, TEXT("%s"), szFullPath);
		LEAVE_BLOCK(0);
	}

	BOOL bRet = ::CreateDirectory(szFullPath, nullptr);
	if (bRet)
	{
		hr = S_OK;
		LEAVE_BLOCK(0);
	}
	else
	{
		LastError = GetLastError();
		if (ERROR_ALREADY_EXISTS == LastError)
		{
			hr = S_OK;
			LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	CString strParent;
	GetStringParentByLevel(strParent, szFullPath, 1, TEXT('\\'));
	hr = CreateDirectoryRecursivelyExPrivate(strParent);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateDirectoryRecursivelyExPrivate"), hr, TEXT("%s"), strParent.GetString());
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	bRet = ::CreateDirectory(szFullPath, nullptr);
	if (!bRet)
	{
		LastError = GetLastError();
		if (ERROR_ALREADY_EXISTS == LastError)
		{
			hr = S_OK;
		}
		else
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("CreateDirectory"), hr, TEXT("%s"), szFullPath);
			LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CMiscHelper::CreateDirectoryRecursively(LPCTSTR szFullPath)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	TCHAR szVolumePathName[512] = { 0 };
	BOOL bGetVolumePathName = GetVolumePathName(
		szFullPath,//__in          LPCTSTR lpszFileName,
		szVolumePathName,//__out         LPTSTR lpszVolumePathName,
		_countof(szVolumePathName)//__in          DWORD cchBufferLength
		);
	if (!bGetVolumePathName)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("GetVolumePathName"), hr, TEXT("%s"), szFullPath);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = CreateDirectoryRecursivelyExPrivate(szFullPath);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateDirectoryRecursivelyExPrivate"), hr, TEXT("%s"), szFullPath);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
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

