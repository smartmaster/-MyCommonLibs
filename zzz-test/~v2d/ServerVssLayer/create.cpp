//******************************************
//  
// 
//  This file may contain preliminary information or inaccuracies, 
//  and may not correctly represent any associated Microsoft 
//  Product as commercially released. All Materials are provided entirely 
//   To the extent permitted by law, MICROSOFT MAKES NO 
//  WARRANTY OF ANY KIND, DISCLAIMS ALL EXPRESS, IMPLIED AND STATUTORY 
//  WARRANTIES, AND ASSUMES NO LIABILITY TO YOU FOR ANY DAMAGES OF 
//  ANY TYPE IN CONNECTION WITH THESE MATERIALS OR ANY INTELLECTUAL PROPERTY IN THEM. 
// 


// Main header
#include "stdafx.h"
#include "VssCommon.h"



void VssClient::CreateSnapshotSet(
    vector<wstring> volumeList, 
    wstring outputXmlFile,     
    vector<wstring> excludedWriterList,
    vector<wstring> includedWriterList
    )
{
    FunctionTracer ft(DBG_INFO);

    bool bSnapshotWithWriters = ((m_dwContext & VSS_VOLSNAP_ATTR_NO_WRITERS) == 0);

    // Gather writer metadata
    if (bSnapshotWithWriters)
        GatherWriterMetadata();

    // Select writer components based on the given shadow volume list
    if (bSnapshotWithWriters)
        SelectComponentsForBackup(volumeList, excludedWriterList, includedWriterList);

    // Start the shadow set
    CHECK_COM(m_pVssObject->StartSnapshotSet(&m_latestSnapshotSetID))
    ft.WriteLine(L"Creating shadow set " WSTR_GUID_FMT L" ...", GUID_PRINTF_ARG(m_latestSnapshotSetID));

    // Add the specified volumes to the shadow set
    AddToSnapshotSet(volumeList);

    // Prepare for backup. 
    // This will internally create the backup components document with the selected components
    if (bSnapshotWithWriters)
        PrepareForBackup();

    // Creates the shadow set 
    DoSnapshotSet();

    // Do not attempt to continue with delayed snapshot ...
    if (m_dwContext & VSS_VOLSNAP_ATTR_DELAYED_POSTSNAPSHOT)
    {
        ft.WriteLine(L"\nFast snapshot created. Exiting... \n");
        return;
    }

    // Saves the backup components document, if needed
    if (outputXmlFile.length() > 0 || m_pIStorageXmls)
	{
        SaveBackupComponentsDocument(outputXmlFile);
	}

    // List all the created shadow copies
    if ((m_dwContext & VSS_VOLSNAP_ATTR_TRANSPORTABLE) == 0)
    {
        ft.WriteLine(L"\nList of created shadow copies: \n");
        QuerySnapshotSet(m_latestSnapshotSetID);
    }
}


// Prepare the shadow for backup
void VssClient::PrepareForBackup()
{
    FunctionTracer ft(DBG_INFO);

    ft.WriteLine(L"Preparing for backup ... ");

    CComPtr<IVssAsync>  pAsync;
    CHECK_COM(m_pVssObject->PrepareForBackup(&pAsync));

    // Waits for the async operation to finish and checks the result
    WaitAndCheckForAsyncOperation(pAsync);

    // Check selected writer status
    CheckSelectedWriterStatus();
}


// Add volumes to the shadow set
void VssClient::AddToSnapshotSet(vector<wstring> volumeList)
{
    FunctionTracer ft(DBG_INFO);

    // Preserve the list of volumes for script generation 
    m_latestVolumeList = volumeList;

    _ASSERTE(m_latestSnapshotIdList.size() == 0);

    // Add volumes to the shadow set 
    for (unsigned i = 0; i < volumeList.size(); i++)
    {
        wstring volume = volumeList[i];
        ft.WriteLine(L"- Adding volume %s [%s] to the shadow set...", 
            volume.c_str(),
            GetDisplayNameForVolume(volume).c_str());

        VSS_ID SnapshotID;
        CHECK_COM(m_pVssObject->AddToSnapshotSet((LPWSTR)volume.c_str(), GUID_NULL, &SnapshotID));

        // Preserve this shadow ID for script generation 
        m_latestSnapshotIdList.push_back(SnapshotID);
    }
}



// Effectively creating the shadow (calling DoSnapshotSet)
void VssClient::DoSnapshotSet()
{
    FunctionTracer ft(DBG_INFO);

    ft.WriteLine(L"Creating the shadow (DoSnapshotSet) ... ");

    CComPtr<IVssAsync>  pAsync;
    CHECK_COM(m_pVssObject->DoSnapshotSet(&pAsync));

    // Waits for the async operation to finish and checks the result
    WaitAndCheckForAsyncOperation(pAsync);

    // Do not attempt to continue with delayed snapshot ...
    if (m_dwContext & VSS_VOLSNAP_ATTR_DELAYED_POSTSNAPSHOT)
    {
        ft.WriteLine(L"\nFast DoSnapshotSet finished. \n");
        return;
    }

    // Check selected writer status
    CheckSelectedWriterStatus();

    ft.WriteLine(L"Shadow copy set succesfully created.");
}


// Ending the backup (calling BackupComplete)
void VssClient::BackupComplete(bool succeeded)
{
    FunctionTracer ft(DBG_INFO);

    unsigned cWriters = 0;
    CHECK_COM(m_pVssObject->GetWriterComponentsCount(&cWriters));

    if (cWriters == 0)
    {
        ft.WriteLine(L"- There were no writer components in this backup");
        return;
    } else if (succeeded)
        ft.WriteLine(L"- Mark all writers as succesfully backed up... ");
    else
        ft.WriteLine(L"- Backup failed. Mark all writers as not succesfully backed up... ");

    SetBackupSucceeded(succeeded);

    ft.WriteLine(L"Completing the backup (BackupComplete) ... ");

    CComPtr<IVssAsync>  pAsync;
    CHECK_COM(m_pVssObject->BackupComplete(&pAsync));

    // Waits for the async operation to finish and checks the result
    WaitAndCheckForAsyncOperation(pAsync);

    // Check selected writer status
    CheckSelectedWriterStatus();

}

static VOID SaveBSTRAsFileHelper(LPCTSTR szFileName, BSTR bstr)
{
	FunctionTracer ft(DBG_INFO);

	DWORD dwLastEror = 0;

	HANDLE hFileXML = CreateFile(
		szFileName,//__in      LPCTSTR lpFileName,
		GENERIC_WRITE,//__in      DWORD dwDesiredAccess,
		0,//__in      DWORD dwShareMode,
		NULL,//__in_opt  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		CREATE_ALWAYS,//__in      DWORD dwCreationDisposition,
		0,//__in      DWORD dwFlagsAndAttributes,
		NULL//__in_opt  HANDLE hTemplateFile
		);
	if (INVALID_HANDLE_VALUE == hFileXML)
	{
		dwLastEror = GetLastError();
		CHECK_WIN32_ERROR(dwLastEror, TEXT("CreateFile"));
	}

	CHandle ahFileXML(hFileXML); hFileXML = INVALID_HANDLE_VALUE;

	CONST BYTE ucTag[] = {0xff, 0xfe};
	DWORD dwNumberOfBytesWritten = 0;
	BOOL bRet = WriteFile(
		ahFileXML,//__in         HANDLE hFile,
		ucTag,//__in         LPCVOID lpBuffer,
		sizeof(ucTag),//__in         DWORD nNumberOfBytesToWrite,
		&dwNumberOfBytesWritten,//__out_opt    LPDWORD lpNumberOfBytesWritten,
		NULL//__inout_opt  LPOVERLAPPED lpOverlapped
		);
	if (!bRet)
	{
		dwLastEror = GetLastError();
		CHECK_WIN32_ERROR(dwLastEror, TEXT("WriteFile"));
	}

	CComBSTR bstrXML;
	bstrXML.Attach(bstr);

	dwNumberOfBytesWritten = 0;
	bRet = WriteFile(
		ahFileXML,//__in         HANDLE hFile,
		(LPCTSTR)bstrXML,//__in         LPCVOID lpBuffer,
		bstrXML.ByteLength(),//__in         DWORD nNumberOfBytesToWrite,
		&dwNumberOfBytesWritten,//__out_opt    LPDWORD lpNumberOfBytesWritten,
		NULL//__inout_opt  LPOVERLAPPED lpOverlapped
		);

	bstrXML.Detach();
	
	if (!bRet)
	{
		dwLastEror = GetLastError();
		CHECK_WIN32_ERROR(dwLastEror, TEXT("WriteFile"));
	}
}

static VOID SaveBCDStorageHelper(
	LPCTSTR szStreamName,
	BSTR bstrXml,
	IStorage * pIStorage
	)
{
	FunctionTracer ft(DBG_INFO);

	CComPtr<IStream> spIStream;
	CHECK_COM(pIStorage->CreateStream(
		szStreamName,//[in]                 const WCHAR* pwcsName,
		STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,//[in]                 DWORD grfMode,
		0,//[in]                 DWORD reserved1,
		0,//[in]                 DWORD reserved2,
		&spIStream//[out]                IStream** ppstm
		));

	CComBSTR bstr;
	bstr.Attach(bstrXml);
	CHECK_COM(bstr.WriteToStream(spIStream));
	bstr.Detach();
}

// Save the backup components document
void VssClient::SaveBackupComponentsDocument(wstring fileName)
{
    FunctionTracer ft(DBG_INFO);

    ft.WriteLine(L"Saving the backup components document ... ");

    // Get the Backup Components in XML format
    CComBSTR bstrXML;
    CHECK_COM(m_pVssObject->SaveAsXML(&bstrXML));

	if (m_pIStorageXmls) //<mc> 2010-7-25 11:50
	{
		CONST TCHAR szBcdStream[] = TEXT("BackupComponentsDocument.xml");
		SaveBCDStorageHelper(
			szBcdStream,//LPCTSTR szStreamName,
			bstrXML,//BSTR bstrXml,
			m_pIStorageXmls//IStorage * pIStorage
			);
	}

    // Save the XML string to the file
    //WriteFile(fileName, BSTR2WString(bstrXML));
	if (fileName.length() > 0)
	{
		SaveBSTRAsFileHelper(fileName.c_str(), bstrXML);
	}
}


// Generate the SETVAR script
// This is useful for management operations
void VssClient::GenerateSetvarScript(wstring stringFileName)
{
    FunctionTracer ft(DBG_INFO);

    ft.WriteLine(L"Generating the SETVAR script (%s) ... ", stringFileName.c_str());

    wofstream ofile;
    ofile.open(WString2String(stringFileName).c_str());
    ofile << L"@echo.\n";
    ofile << L"@echo [This script is generated by VSHADOW.EXE for the shadow set " << Guid2WString(m_latestSnapshotSetID).c_str() << L"]\n";
    ofile << L"@echo.\n\n";

    wstring snapshotSetID = Guid2WString(m_latestSnapshotSetID);
    ofile << L"SET SHADOW_SET_ID=" << snapshotSetID.c_str() << L"\n\n";
    
    // For each added volume add the VSHADOW.EXE exposure command
    for (unsigned i = 0; i < m_latestSnapshotIdList.size(); i++)
    {
        wstring snapshotID = Guid2WString(m_latestSnapshotIdList[i]);
        ofile << L"SET SHADOW_ID_" << i+1 << L"=" << snapshotID.c_str() << L"\n";

        // Get shadow copy device (if the snapshot is there)
        if ((m_dwContext & VSS_VOLSNAP_ATTR_TRANSPORTABLE) == 0)
        {
            VSS_SNAPSHOT_PROP Snap;
            CHECK_COM(m_pVssObject->GetSnapshotProperties(WString2Guid(snapshotID), &Snap));

            // Automatically call VssFreeSnapshotProperties on this structure at the end of scope
            CAutoSnapPointer snapAutoCleanup(&Snap);

            ofile << L"SET SHADOW_DEVICE_" << i+1 << L"=" << Snap.m_pwszSnapshotDeviceObject << L"\n";
			ofile << L"SET VOLUME_DEVICE_" << i+1 << L"=" << Snap.m_pwszOriginalVolumeName << L"\n";
			wstring volumeNameCanon;
			GetDisplayNameForVolumeNoThrow(Snap.m_pwszOriginalVolumeName, volumeNameCanon);
			ofile << L"REM VOLUME_DISPLAY_" << i+1 << L"=" << volumeNameCanon << L"\n\n";
        }
    }

    ofile.close();
}


// Import the shadow set
void VssClient::ImportSnapshotSet()
{
    FunctionTracer ft(DBG_INFO);

    ft.WriteLine(L"Importing the transportable snapshot set ... ");

    CComPtr<IVssAsync>  pAsync;
    CHECK_COM(m_pVssObject->ImportSnapshots(&pAsync));

    // Waits for the async operation to finish and checks the result
    WaitAndCheckForAsyncOperation(pAsync);

    ft.WriteLine(L"Shadow copy set succesfully imported.");
}


// Marks all selected components as succeeded for backup
void VssClient::SetBackupSucceeded(bool succeeded)
{
    FunctionTracer ft(DBG_INFO);

    // Enumerate writers
    for (unsigned iWriter = 0; iWriter < m_writerList.size(); iWriter++)
    {
        VssWriter & writer = m_writerList[iWriter];

        // Enumerate components
        for(unsigned iComponent = 0; iComponent < writer.m_components.size(); iComponent++)
        {
            VssComponent & component = writer.m_components[iComponent]; 

            // Test that the component is explicitely selected and requires notification
            if (!component.m_isExplicitlyIncluded)
                continue;

            // Call SetBackupSucceeded for this component
            CHECK_COM(m_pVssObject->SetBackupSucceeded(
                WString2Guid(writer.m_instanceId),
                WString2Guid(writer.m_id),
                component.m_type,
                component.m_logicalPath.c_str(),
                component.m_name.c_str(),
                succeeded));
        }
    }
}


