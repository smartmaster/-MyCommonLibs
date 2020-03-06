#include "stdafx.h"

#include "VcxprojDisableOpt.h"
#include "EnumWinFile.h"
#include "ReadWriteStringFile.h"


BOOL CVCProjectDisableOptimazationHandler::Compare(CONST XmlElement & xmlElement)
{
	static CONST TCHAR NAME_CLCOMPILE[] = TEXT("ClCompile");
	return 0 == xmlElement.m_Name.CompareNoCase(NAME_CLCOMPILE) &&
		0 == xmlElement.m_Attrs.size() &&
		xmlElement.m_Children.size();
}

HRESULT CVCProjectDisableOptimazationHandler::Operate(XmlElement & xmlElement)
{
	static CONST TCHAR NAME_OPTIMIZATION[] = TEXT("Optimization");
	static CONST TCHAR NAME_DISABLED[] = TEXT("Disabled");

	BOOL bFound = FALSE;
	for (size_t ii = 0; ii < xmlElement.m_Children.size(); ++ii)
	{
		XmlElement & ChildElement = xmlElement.m_Children[ii];
		if (0 == ChildElement.m_Name.CompareNoCase(NAME_OPTIMIZATION))
		{
			ChildElement.m_Value = NAME_DISABLED;
			bFound = TRUE;
			break;
		}
	}

	if (!bFound && xmlElement.m_Children.size())
	{
		AllocateOneMore(xmlElement.m_Children);
		XmlElement & ChildElement = xmlElement.m_Children.back();
		ChildElement.m_Name = NAME_OPTIMIZATION;
		ChildElement.m_Value = NAME_DISABLED;
	}

	return S_OK;
}

HRESULT VcxprojDisableOpt(LPCTSTR pSourceFile, LPCTSTR pNewFile)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	vector<CString> vecStr(1);
	hr = ReadSourceStringFile(pSourceFile, vecStr[0]);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadSourceStringFile"), hr, TEXT("%s"), pSourceFile);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	XmlElement RootElement;
	INT Start = 0;
	LONG ElementCount = RootElement.FromString(vecStr[0], Start);
	if (ElementCount <= 0)
	{
		hr = E_FAIL;
		D_API_ERR(0, TEXT("BadFormatXml"), ERROR_BAD_FORMAT, TEXT("%s"), pSourceFile);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CVCProjectDisableOptimazationHandler EnumHandler;
	EnumXmlElement(RootElement, &EnumHandler);

	//////////////////////////////////////////////////////////////////////////
	vecStr[0].Empty();
	RootElement.ToStringWithFormat(vecStr[0]);


	//////////////////////////////////////////////////////////////////////////
	hr = WriteTargetStringFile(pNewFile, vecStr);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTargetStringFile"), hr, TEXT("%s"), pNewFile);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

static INT __RemoveFileHR(LPCTSTR szName)
{
	INT nStatus = ERROR_SUCCESS;

	do
	{
		DWORD dwAttributes = GetFileAttributes(
			szName//__in  LPCTSTR lpFileName
			);
		if (INVALID_FILE_ATTRIBUTES == dwAttributes)
		{
			nStatus = GetLastError();
			D_API_ERR(0, TEXT("GetFileAttributes"), nStatus, TEXT("%s"), szName);
			nStatus = -1;
			break;
		}

		dwAttributes &= (~(FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY));
		BOOL bSetFileAttributes = SetFileAttributes(
			szName,//__in  LPCTSTR lpFileName,
			dwAttributes//__in  DWORD dwFileAttributes
			);
		if (!bSetFileAttributes)
		{
			nStatus = GetLastError();
			D_API_ERR(0, TEXT("SetFileAttributes"), nStatus, TEXT("%s"), szName);
			nStatus = -1;
			break;
		}

	} while (FALSE);

	return nStatus;
}

HRESULT VcxprojDisableOptRecurively(LPCTSTR pRootFolder)
{
	class CEnumFolderFileOperation : public IEnumFolderFileOperation
	{
	public:
		CEnumFolderFileOperation() :
			m_ProcessedFileCount(0),
			m_ErrorFileCount(0)
		{
		}
		//struct IEnumFolderFileOperation
		//{
		virtual INT Operate(CONST CString & strFile)
		{
			D_INFO(0, TEXT("Process file [%s]"), strFile.GetString());
			__RemoveFileHR(strFile.GetString());
			HRESULT hr = VcxprojDisableOpt(strFile.GetString(), strFile.GetString());
			if (SUCCEEDED(hr))
			{
				++m_ProcessedFileCount;
			}
			else
			{
				++m_ErrorFileCount;
			}
			return S_OK;
		}
		//};

		LONG GetFileCount()
		{
			return m_ProcessedFileCount;
		}

		LONG GetErrorFileCount()
		{
			return m_ErrorFileCount;
		}

	private:
		LONG m_ProcessedFileCount;
		LONG m_ErrorFileCount;

	};

	CEnumFolderFileOperation FileOper;
	static CONST TCHAR NAME_VCXPROJ[] = TEXT("*.vcxproj");
	EnumDirectoryFileTree(pRootFolder, NAME_VCXPROJ, NULL, &FileOper);
	D_INFO(0, TEXT("Total processed file count [%d], Total error file count [%d]"), FileOper.GetFileCount(), FileOper.GetErrorFileCount());

	return 0;
}
