#include "stdafx.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>
#include <fstream>

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"

#include "SingleAndAllVolumeInfo.h"
#include "MiscHelper.h"



//////////////////////////////////////////////////////////////////////////
static LPCTSTR DriveTypeString(UINT DriveType);
static VOID GetVssVolumes(std::vector<CString> & volumes);
//////////////////////////////////////////////////////////////////////////


#define RAW_STRING_LINE(x) #x "\n"
#define RAW_STRING_NEW_LINE "\n"

static const char file_part1[] =
RAW_STRING_NEW_LINE
RAW_STRING_LINE(delete shadows all)
RAW_STRING_NEW_LINE
RAW_STRING_LINE(# creates metadata cab file)
RAW_STRING_LINE(SET METADATA C:\\as-hbbu-vmwarebackup\\vss-shadow.cab)
RAW_STRING_NEW_LINE
RAW_STRING_LINE(# Use a persistent context so we can "map" a drive)
RAW_STRING_LINE(set context persistent)
RAW_STRING_NEW_LINE
RAW_STRING_LINE(# Log everything)
RAW_STRING_LINE(set verbose on)
RAW_STRING_NEW_LINE
RAW_STRING_LINE(# exclude writers)
RAW_STRING_LINE(# ADAM(VMwareVCMSDS) Writer)
RAW_STRING_LINE(writer exclude {dd846aaa-a1b6-42a8-aaf8-03dcb6114bfd})
RAW_STRING_LINE(# System Writer)
RAW_STRING_LINE(# writer exclude {e8132975-6f93-4464-a53e-1050253ae220})
RAW_STRING_NEW_LINE
RAW_STRING_LINE(# Take snapshot uncomment disk you need)
RAW_STRING_LINE(begin backup)
RAW_STRING_NEW_LINE
RAW_STRING_NEW_LINE
;

static const WCHAR file_part2_format[] = 
L"add volume %s alias VM_%d" L"\n";


static const char file_part3[] =
RAW_STRING_NEW_LINE
RAW_STRING_NEW_LINE
RAW_STRING_LINE(# Create the shadow copy)
RAW_STRING_LINE(create)
RAW_STRING_NEW_LINE
RAW_STRING_LINE(# Expose each shadowed volume as a drive)
RAW_STRING_LINE(# ***TODO*** Make sure the exposed drive letter is available and used)
RAW_STRING_LINE(# in the backup script)
RAW_STRING_LINE(#expose %VM_C% S:)
RAW_STRING_LINE(#expose %VM_D% T:)
RAW_STRING_LINE(#expose %VM_E% U:)
RAW_STRING_NEW_LINE
RAW_STRING_LINE(end backup)
RAW_STRING_NEW_LINE
RAW_STRING_NEW_LINE
;


HRESULT ModifyFileForDiskShadowHelper(LPCTSTR tagetFile, const std::vector<CString> & volumes)
{
	using namespace std;

	HRESULT hr = S_OK;
	
	wofstream ofile(tagetFile);
	if (ofile)
	{
		//////////////////////////////////////////////////////////////////////////
		ofile << file_part1;

		//////////////////////////////////////////////////////////////////////////
		for (size_t ii = 0; ii < volumes.size(); ++ ii)
		{
			CString str;
			str.Format(file_part2_format, volumes[ii].GetString(), (int)(ii));
			ofile << str.GetString();
		}

		//////////////////////////////////////////////////////////////////////////
		ofile << file_part3;

		ofile.close();
	}
	else
	{
		hr = E_FAIL;
	}
	
	return hr;
}


HRESULT ModifyFileForDiskShadow(LPCTSTR tagetFile)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	std::vector<CString> volumes;
	GetVssVolumes(volumes);

	//////////////////////////////////////////////////////////////////////////
	CMiscHelper::__RemoveFileAttrs(tagetFile, FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN);
	hr = ModifyFileForDiskShadowHelper(tagetFile, volumes);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("ModifyFileForDiskShadowHelper"), hr, TEXT("cannot open file %s"), tagetFile);
		LEAVE_BLOCK(0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

//////////////////////////////////////////////////////////////////////////
static const char vs_file_part1[] =
RAW_STRING_NEW_LINE
RAW_STRING_LINE(set as_tool_root=C:\\as-hbbu-vmwarebackup)
RAW_STRING_NEW_LINE
RAW_STRING_LINE(md "%as_tool_root%\log")
RAW_STRING_NEW_LINE
RAW_STRING_LINE(echo "#######################" >> "%as_tool_root%\log\000-vm-backuplog.txt")
RAW_STRING_LINE(date /t >> "%as_tool_root%\log\000-vm-backuplog.txt")
RAW_STRING_LINE(time /t >> "%as_tool_root%\log\000-vm-backuplog.txt")
RAW_STRING_NEW_LINE
RAW_STRING_LINE(md "%as_tool_root%\created-before-freeze")
RAW_STRING_NEW_LINE
RAW_STRING_LINE(del "%as_tool_root%\*.cab" /Q)
RAW_STRING_LINE(del "%as_tool_root%\WritersXml\*.xml" /Q)
RAW_STRING_LINE(md "%as_tool_root%\WritersXml")
RAW_STRING_LINE("%as_tool_root%\vshadow.exe" -da >> "%as_tool_root%\log\000-vm-backuplog.txt")
RAW_STRING_NEW_LINE
;

//%s is SPACE separated volumes
static const WCHAR vs_file_part2_format[] = 
L"\"%%as_tool_root%%\\vshadow.exe\" -bc=\"%%as_tool_root%%\\WritersXml\\bcd.xml\" -p %s >> \"%%as_tool_root%%\\log\\000-vm-backuplog.txt\"" L"\n";

static const char vs_file_part3[] =
RAW_STRING_NEW_LINE
RAW_STRING_LINE(date /t >> "%as_tool_root%\log\000-vm-backuplog.txt")
RAW_STRING_LINE(time /t >> "%as_tool_root%\log\000-vm-backuplog.txt")
RAW_STRING_LINE(echo "$$$$$$$$$$$$$$$$$$$$$$$" >> "%as_tool_root%\log\000-vm-backuplog.txt")
RAW_STRING_NEW_LINE
RAW_STRING_NEW_LINE
;





HRESULT ModifyFileForVShadowHelper(LPCTSTR tagetFile, const std::vector<CString> & volumes)
{
	using namespace std;

	HRESULT hr = S_OK;

	wofstream ofile(tagetFile);
	if (ofile)
	{
		//////////////////////////////////////////////////////////////////////////
		ofile << vs_file_part1;

		//////////////////////////////////////////////////////////////////////////
		CString strVols;
		CString strFile;
		for (size_t ii = 0; ii < volumes.size(); ++ii)
		{
			strVols.AppendFormat(TEXT(" %s"), volumes[ii].GetString());
		}
		strFile.Format(vs_file_part2_format, strVols.GetString());
		ofile << strFile.GetString();
		//////////////////////////////////////////////////////////////////////////
		ofile << vs_file_part3;

		ofile.close();
	}
	else
	{
		hr = E_FAIL;
	}

	return hr;
}


HRESULT ModifyFileForVShadow(LPCTSTR tagetFile)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	std::vector<CString> volumes;
	GetVssVolumes(volumes);

	//////////////////////////////////////////////////////////////////////////
	CMiscHelper::__RemoveFileAttrs(tagetFile, FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN);
	hr = ModifyFileForVShadowHelper(tagetFile, volumes);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("ModifyFileForDiskShadowHelper"), hr, TEXT("cannot open file %s"), tagetFile);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}


//////////////////////////////////////////////////////////////////////////
static LPCTSTR DriveTypeString(UINT DriveType)
{
	static CONST LPCTSTR strDesc[] = 
	{
		TEXT("DRIVE_UNKNOWN"),
		TEXT("DRIVE_NO_ROOT_DIR"),
		TEXT("DRIVE_REMOVABLE"),
		TEXT("DRIVE_FIXED"),
		TEXT("DRIVE_REMOTE"),
		TEXT("DRIVE_CDROM"),
		TEXT("DRIVE_RAMDISK")
	};

	if (DriveType >= _countof(strDesc))
	{
		return strDesc[0];
	}
	else
	{
		return strDesc[DriveType];
	}
}

static VOID GetVssVolumes(std::vector<CString> & volumes)
{
	CAllVolumeInfo avi;
	avi.Retrieve(FALSE);

	
	for (size_t ii = 0; ii < avi.m_pVolumeInfos.size(); ++ii)
	{
		CString str =
			avi.m_pVolumeInfos[ii]->m_Paths.size() ?
			avi.m_pVolumeInfos[ii]->m_Paths[0] :
			avi.m_pVolumeInfos[ii]->m_strVolumeGUID;
		UINT driveType = GetDriveType(str.GetString());
		if (DRIVE_FIXED == driveType)
		{
			D_INFO(0, TEXT("%s is %s, included from VSS snapshot"), str.GetString(), DriveTypeString(driveType));
			str.TrimRight(TEXT("\\/"));
			volumes.push_back(str);
		}
		else
		{
			D_INFO(0, TEXT("%s is %s, excluded from VSS snapshot"), str.GetString(), DriveTypeString(driveType));
		}
	}
}