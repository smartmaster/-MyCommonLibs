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
#include "..\CommonFunctions\MiscHelper.h"


//******************************************
//  Main routines forwriter metadata/status gathering. 
//


// Gather writers metadata
void VssClient::GatherWriterMetadata()
{
	FunctionTracer ft(DBG_INFO);

	ft.WriteLine(L"(Gathering writer metadata...)");

	// Gathers writer metadata
	// WARNING: this call can be performed only once per IVssBackupComponents instance!
	CComPtr<IVssAsync>  pAsync;
	CHECK_COM(m_pVssObject->GatherWriterMetadata(&pAsync));

	// Waits for the async operation to finish and checks the result
	WaitAndCheckForAsyncOperation(pAsync);

	ft.WriteLine(L"Initialize writer metadata ...");

	// Initialize the internal metadata data structures
	InitializeWriterMetadata();
}


void VssClient::GatherWriterMetadataToScreen()
{
	FunctionTracer ft(DBG_INFO);

	ft.WriteLine(L"(Gathering writer metadata...)");

	// Gathers writer metadata
	// WARNING: this call can be performed only once per IVssBackupComponents instance!
	CComPtr<IVssAsync>  pAsync;
	CHECK_COM(m_pVssObject->GatherWriterMetadata(&pAsync));

	// Waits for the async operation to finish and checks the result
	WaitAndCheckForAsyncOperation(pAsync);

	// Get the list of writers in the metadata  
	unsigned cWriters = 0;
	CHECK_COM(m_pVssObject->GetWriterMetadataCount(&cWriters));

	// Enumerate writers
	for (unsigned iWriter = 0; iWriter < cWriters; iWriter++)
	{
		CComBSTR bstrXML;
		// Get the metadata for this particular writer
		VSS_ID idInstance = GUID_NULL;
		CComPtr<IVssExamineWriterMetadata> pMetadata;
		CHECK_COM(m_pVssObject->GetWriterMetadata(iWriter, &idInstance, &pMetadata));

		CHECK_COM(pMetadata->SaveAsXML(&bstrXML));
		wprintf(L"\n--[Writer %d]--\n%s\n", iWriter, (LPCWSTR)(bstrXML));
	}

	wprintf(L"--[end of data]--\n");
}

// Gather writers status
void VssClient::GatherWriterStatus()
{
	FunctionTracer ft(DBG_INFO);

	// Gathers writer status
	// WARNING: GatherWriterMetadata must be called before
	CComPtr<IVssAsync>  pAsync;
	CHECK_COM(m_pVssObject->GatherWriterStatus(&pAsync));

	// Waits for the async operation to finish and checks the result
	WaitAndCheckForAsyncOperation(pAsync);
}


void VssClient::InitializeWriterMetadata()
{
	FunctionTracer ft(DBG_INFO);

	// Get the list of writers in the metadata  
	unsigned cWriters = 0;
	CHECK_COM(m_pVssObject->GetWriterMetadataCount (&cWriters));

	// Enumerate writers
	m_writerList.resize(cWriters); //sonmi01 avoid copy too much56
	for (unsigned iWriter = 0; iWriter < cWriters; iWriter++)
	{
		// Get the metadata for this particular writer
		VSS_ID idInstance = GUID_NULL;
		CComPtr<IVssExamineWriterMetadata> pMetadata;
		CHECK_COM(m_pVssObject->GetWriterMetadata(iWriter, &idInstance, &pMetadata));

		VssWriter  & writer = m_writerList[iWriter]; //sonmi01 avoid copy too much56
		writer.InitializeStorage(m_pIStorageXmls, iWriter);
		writer.Initialize(pMetadata);

		// Add this writer to the list 
		//m_writerList.push_back(writer); //sonmi01 avoid copy too much55
	}
}


// Initialize the list of writers and components for restore
void VssClient::InitializeWriterComponentsForRestore()
{
	FunctionTracer ft(DBG_INFO);

	ft.WriteLine(L"Initializing writer components for restore ...");

	// Get the list of writers in the metadata  
	unsigned cWriters = 0;
	CHECK_COM(m_pVssObject->GetWriterComponentsCount(&cWriters));

	// Enumerate writers
	m_writerComponentsForRestore.reserve(cWriters); //sonmi01 avoid copy too much58
	for (unsigned iWriter = 0; iWriter < cWriters; iWriter++)
	{
		// Get the selected components for this particular writer
		CComPtr<IVssWriterComponentsExt> pWriterComponents;
		CHECK_COM(m_pVssObject->GetWriterComponents(iWriter, &pWriterComponents));

		// Get writer identity. 
		// Ignore this writer if the real writer is not present in the system
		VSS_ID idInstance = GUID_NULL;
		VSS_ID idWriter = GUID_NULL;
		CHECK_COM(pWriterComponents->GetWriterInfo(
			&idInstance, 
			&idWriter
			));

		wstring id = Guid2WString(idWriter);
		wstring instanceId = Guid2WString(idInstance);

		// Try to discover the name based on an existing writer (identical ID and instance ID).
		// Otherwise, ignore it!
		bool bFound = false;
		for (unsigned i = 0; i < m_writerList.size(); i++)
		{
			// Do Not check for instance ID ... 
			if (id != m_writerList[i].m_id)
				continue; 

			// Copy the information from the existing writer in the system
			m_writerComponentsForRestore.push_back(m_writerList[i]); //sonmi01 avoid copy too much59
			VssWriter &  writer = m_writerComponentsForRestore[m_writerComponentsForRestore.size() - 1]; //sonmi01 avoid copy too much00

			ft.WriteLine(L"- Writer %s is present in the Backup Components document and on the system. Considering for restore ...",
				writer.m_name.c_str());

			// Adding components
			writer.InitializeComponentsForRestore(pWriterComponents);

			// Add this writer object to the writer components list 
			//m_writerComponentsForRestore.push_back(writer); //sonmi01 avoid copy too much58

			// We found the writer!
			bFound = true;
		}

		if (!bFound)
		{
			ft.WriteLine(L"- Writer with ID %s is not present in the system! Ignoring ...", id.c_str());
		}
	}
}


// Lists the writer metadata
void VssClient::ListWriterMetadata(bool bListDetailedInfo)
{
	FunctionTracer ft(DBG_INFO);

	ft.WriteLine(L"Listing writer metadata ...");   

	// Enumerate writers
	for (unsigned iWriter = 0; iWriter < m_writerList.size(); iWriter++)
		m_writerList[iWriter].Print(bListDetailedInfo);
}



// Lists the status for all writers
void VssClient::ListWriterStatus()
{
	FunctionTracer ft(DBG_INFO);

	ft.WriteLine(L"Listing writer status ..."); 

	// Gets the number of writers in the gathered status info
	// (WARNING: GatherWriterStatus must be called before)
	unsigned cWriters = 0;
	CHECK_COM(m_pVssObject->GetWriterStatusCount(&cWriters));
	ft.WriteLine(L"- Number of writers that responded: %u", cWriters);  

	// Enumerate each writer
	for(unsigned iWriter = 0; iWriter < cWriters; iWriter++)
	{
		VSS_ID idInstance = GUID_NULL;
		VSS_ID idWriter= GUID_NULL;
		VSS_WRITER_STATE eWriterStatus = VSS_WS_UNKNOWN;
		CComBSTR bstrWriterName;
		HRESULT hrWriterFailure = S_OK;

		// Get writer status
		CHECK_COM(m_pVssObject->GetWriterStatus(iWriter,
			&idInstance,
			&idWriter,
			&bstrWriterName,
			&eWriterStatus,
			&hrWriterFailure));

		// Print writer status
		ft.WriteLine(L"\n"
			L"* WRITER \"%s\"\n"
			L"   - Status: %d (%s)\n" 
			L"   - Writer Failure code: 0x%08lx (%s)\n" 
			L"   - Writer ID: " WSTR_GUID_FMT L"\n"
			L"   - Instance ID: " WSTR_GUID_FMT L"\n",
			(PWCHAR)bstrWriterName,
			eWriterStatus, GetStringFromWriterStatus(eWriterStatus).c_str(), 
			hrWriterFailure, FunctionTracer::HResult2String(hrWriterFailure).c_str(),
			GUID_PRINTF_ARG(idWriter),
			GUID_PRINTF_ARG(idInstance)
			);
	}
}


// Pre-restore 
void VssClient::PreRestore()
{
	FunctionTracer ft(DBG_INFO);

	ft.WriteLine(L"\nSending the PreRestore event ... \n");

	// Gathers writer status
	// WARNING: GatherWriterMetadata must be called before
	CComPtr<IVssAsync>  pAsync;
	CHECK_COM(m_pVssObject->PreRestore(&pAsync));

	// Waits for the async operation to finish and checks the result
	WaitAndCheckForAsyncOperation(pAsync);
}



// Post-restore 
void VssClient::PostRestore()
{
	FunctionTracer ft(DBG_INFO);

	ft.WriteLine(L"\nSending the PostRestore event ... \n");

	// Gathers writer status
	// WARNING: GatherWriterMetadata must be called before
	CComPtr<IVssAsync>  pAsync;
	CHECK_COM(m_pVssObject->PostRestore(&pAsync));

	// Waits for the async operation to finish and checks the result
	WaitAndCheckForAsyncOperation(pAsync);
}



// Convert a writer status into a string
wstring VssClient::GetStringFromWriterStatus(VSS_WRITER_STATE eWriterStatus)
{
	FunctionTracer ft(DBG_INFO);

	ft.Trace(DBG_INFO, L"Interpreting constant %d", (int)eWriterStatus);
	switch (eWriterStatus)
	{
		CHECK_CASE_FOR_CONSTANT(VSS_WS_STABLE);
		CHECK_CASE_FOR_CONSTANT(VSS_WS_WAITING_FOR_FREEZE);
		CHECK_CASE_FOR_CONSTANT(VSS_WS_WAITING_FOR_THAW);
		CHECK_CASE_FOR_CONSTANT(VSS_WS_WAITING_FOR_POST_SNAPSHOT);
		CHECK_CASE_FOR_CONSTANT(VSS_WS_WAITING_FOR_BACKUP_COMPLETE);
		CHECK_CASE_FOR_CONSTANT(VSS_WS_FAILED_AT_IDENTIFY);
		CHECK_CASE_FOR_CONSTANT(VSS_WS_FAILED_AT_PREPARE_BACKUP);
		CHECK_CASE_FOR_CONSTANT(VSS_WS_FAILED_AT_PREPARE_SNAPSHOT);
		CHECK_CASE_FOR_CONSTANT(VSS_WS_FAILED_AT_FREEZE);
		CHECK_CASE_FOR_CONSTANT(VSS_WS_FAILED_AT_THAW);
		CHECK_CASE_FOR_CONSTANT(VSS_WS_FAILED_AT_POST_SNAPSHOT);
		CHECK_CASE_FOR_CONSTANT(VSS_WS_FAILED_AT_BACKUP_COMPLETE);
		CHECK_CASE_FOR_CONSTANT(VSS_WS_FAILED_AT_PRE_RESTORE);
		CHECK_CASE_FOR_CONSTANT(VSS_WS_FAILED_AT_POST_RESTORE);

	default:
		ft.WriteLine(L"Unknown constant: %d",eWriterStatus);
		_ASSERTE(false);
		return wstring(L"Undefined");
	}
}










//******************************************
//  VssWriter
//

VOID SaveWriterMetaDataHelper(
	LPCTSTR Name,
	LPCTSTR id,
	LPCTSTR instanceId,
	IVssExamineWriterMetadata * pMetadata
	)
{
	FunctionTracer ft(DBG_INFO);

	CComBSTR bstrWMXml;
	CHECK_COM(pMetadata->SaveAsXML(
		&bstrWMXml
		));

	wstring strWMName = TEXT("VSSWM==");
	strWMName +=	Name;
	strWMName += TEXT("==");
	strWMName += id;
	strWMName += TEXT("==");
	strWMName += instanceId;
	strWMName += TEXT(".xml");

	DWORD dwLastEror = 0;

	HANDLE hFileXML = CreateFile(
		strWMName.c_str(),//__in      LPCTSTR lpFileName,
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

	dwNumberOfBytesWritten = 0;
	bRet = WriteFile(
		ahFileXML,//__in         HANDLE hFile,
		(LPCTSTR)bstrWMXml,//__in         LPCVOID lpBuffer,
		bstrWMXml.ByteLength(),//__in         DWORD nNumberOfBytesToWrite,
		&dwNumberOfBytesWritten,//__out_opt    LPDWORD lpNumberOfBytesWritten,
		NULL//__inout_opt  LPOVERLAPPED lpOverlapped
		);
	if (!bRet)
	{
		dwLastEror = GetLastError();
		CHECK_WIN32_ERROR(dwLastEror, TEXT("WriteFile"));
	}
}

VOID SaveWriterMetaDataStorageHelper(
	LPCTSTR Name,
	LPCTSTR id,
	LPCTSTR instanceId,
	IVssExamineWriterMetadata * pMetadata,
	IStorage * pIStorage,
	INT iWriterIndex
	)
{
	FunctionTracer ft(DBG_INFO);

	CComBSTR bstrWMXml;
	CHECK_COM(pMetadata->SaveAsXML(
		&bstrWMXml
		));

	//wstring strWMName;// = TEXT("VSSWM==");
	//strWMName +=	Name;
	//strWMName += TEXT("==");
	//strWMName += id;
	//strWMName += TEXT("==");
	//strWMName += instanceId;
	//strWMName += TEXT(".xml");

	CString strWmName;
	strWmName.Format(STG_ITEM_NAME, iWriterIndex);

	DWORD dwLastEror = 0;

	CComPtr<IStream> spIStream;
	CHECK_COM(pIStorage->CreateStream(
		strWmName.GetString(),//[in]                 const WCHAR* pwcsName,
		STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,//[in]                 DWORD grfMode,
		0,//[in]                 DWORD reserved1,
		0,//[in]                 DWORD reserved2,
		&spIStream//[out]                IStream** ppstm
		));

	CHECK_COM(bstrWMXml.WriteToStream(spIStream));
}

// Initialize from a IVssWMFiledesc
void VssWriter::Initialize(IVssExamineWriterMetadata * pMetadata)
{
	FunctionTracer ft(DBG_INFO);

	// Get writer identity information
	VSS_ID idInstance = GUID_NULL;
	VSS_ID idWriter = GUID_NULL;
	CComBSTR bstrWriterName;
	VSS_USAGE_TYPE usage = VSS_UT_UNDEFINED;
	VSS_SOURCE_TYPE source= VSS_ST_UNDEFINED;
	CComBSTR bstrService;
	CComBSTR bstrUserProcedure;
	UINT iMappings;

	// Get writer identity
	CHECK_COM(pMetadata->GetIdentity (
		&idInstance,
		&idWriter,
		&bstrWriterName,
		&usage,
		&source
		));

	// Get the restore method 
	CHECK_COM(pMetadata->GetRestoreMethod(
		&m_restoreMethod,
		&bstrService,
		&bstrUserProcedure,
		&m_writerRestoreConditions,
		&m_rebootRequiredAfterRestore,
		&iMappings
		));

	// Initialize local members
	m_name = (LPWSTR)bstrWriterName;
	m_id = Guid2WString(idWriter);
	m_instanceId = Guid2WString(idInstance);
	m_supportsRestore = (m_writerRestoreConditions != VSS_WRE_NEVER);

	//******************************************
	//SaveWriterMetaDataHelper(
	//	m_name.c_str(),//LPCTSTR Name,
	//	m_id.c_str(),//LPCTSTR id,
	//	m_instanceId.c_str(),//LPCTSTR instanceId,
	//	pMetadata//IVssExamineWriterMetadata * pMetadata
	//	);
	if (m_pIStorageXmls)
	{
		SaveWriterMetaDataStorageHelper(
			m_name.c_str(),//LPCTSTR Name,
			m_id.c_str(),//LPCTSTR id,
			m_instanceId.c_str(),//LPCTSTR instanceId,
			pMetadata,//IVssExamineWriterMetadata * pMetadata,
			m_pIStorageXmls,//IStorage * pIStorage
			m_iWriterIndex
			);
	}

	// Get file counts
	unsigned cIncludeFiles = 0;
	unsigned cExcludeFiles = 0;
	unsigned cComponents = 0;
	CHECK_COM(pMetadata->GetFileCounts(&cIncludeFiles, &cExcludeFiles, &cComponents));

	// Get exclude files
	m_excludedFiles.resize(cExcludeFiles); //sonmi01 avoid copy too much01
	for(unsigned i = 0; i < cExcludeFiles; i++)
	{
		CComPtr<IVssWMFiledesc> pFileDesc;
		CHECK_COM(pMetadata->GetExcludeFile(i, &pFileDesc));

		// Add this descriptor to the list of excluded files
		VssFileDescriptor & excludedFile = m_excludedFiles[i];
		excludedFile.Initialize(pFileDesc, VSS_FDT_EXCLUDE_FILES);
		//excludedFiles.push_back(excludedFile); //sonmi01 avoid copy too much00
	}

	// Enumerate components
	m_components.resize(cComponents); //sonmi01 avoid copy too much02
	for(unsigned iComponent = 0; iComponent < cComponents; iComponent++)
	{
		// Get component
		CComPtr<IVssWMComponent> pComponent;
		CHECK_COM(pMetadata->GetComponent(iComponent, &pComponent));

		// Add this component to the list of components
		VssComponent & component = m_components[iComponent];
		component.Initialize(m_name, pComponent);
		//components.push_back(component); //sonmi01 avoid copy too much02
	}

	// Discover toplevel components
	for(unsigned i = 0; i < cComponents; i++)
	{
		m_components[i].m_isTopLevel = true;
		for(unsigned j = 0; j < cComponents; j++)
			if (m_components[j].IsAncestorOf(m_components[i]))
				m_components[i].m_isTopLevel = false;
	}
}


// Initialize from a IVssWriterComponentsExt
void VssWriter::InitializeComponentsForRestore(IVssWriterComponentsExt * pWriterComponents)
{
	FunctionTracer ft(DBG_INFO);

	// Erase the current list of components for this writer
	m_components.clear();

	// Enumerate the components from the BC document
	unsigned cComponents = 0;
	CHECK_COM(pWriterComponents->GetComponentCount(&cComponents));

	// Enumerate components
	m_components.resize(cComponents); //sonmi01 avoid copy too much04
	for(unsigned iComponent = 0; iComponent < cComponents; iComponent++)
	{
		// Get component
		CComPtr<IVssComponent> pComponent;
		CHECK_COM(pWriterComponents->GetComponent(iComponent, &pComponent));

		// Add this component to the list of components
		VssComponent & component = m_components[iComponent]; //sonmi01 avoid copy too much04
		component.Initialize(m_name, pComponent);

		ft.WriteLine(L"- Found component available for restore: \"%s\"", component.m_fullPath.c_str());

		//components.push_back(component); //sonmi01 avoid copy too much03
	}
}


// Prints the writer to the console
void VssWriter::Print(bool bListDetailedInfo)
{
	FunctionTracer ft(DBG_INFO);

	// Print writer identity information
	ft.WriteLine(L"\n"
		L"* WRITER \"%s\"\n"
		L"    - WriterId   = %s\n"
		L"    - InstanceId = %s\n"
		L"    - Supports restore events = %s\n"
		L"    - Writer restore conditions = %s\n"
		L"    - Restore method = %s\n"
		L"    - Requires reboot after restore = %s\n",
		m_name.c_str(),
		m_id.c_str(),
		m_instanceId.c_str(),
		BOOL2TXT(m_supportsRestore),
		GetStringFromRestoreConditions(m_writerRestoreConditions).c_str(),
		GetStringFromRestoreMethod(m_restoreMethod).c_str(),
		BOOL2TXT(m_rebootRequiredAfterRestore)
		);

	// Print exclude files
	ft.WriteLine(L"    - Excluded files:");
	for(unsigned i = 0; i < m_excludedFiles.size(); i++)
		m_excludedFiles[i].Print();

	// Enumerate components
	for(unsigned i = 0; i < m_components.size(); i++)
		m_components[i].Print(bListDetailedInfo);
}


// Convert a component type into a string
inline wstring VssWriter::GetStringFromRestoreMethod(VSS_RESTOREMETHOD_ENUM eRestoreMethod)
{
	FunctionTracer ft(DBG_INFO);

	ft.Trace(DBG_INFO, L"Interpreting constant %d", (int)eRestoreMethod);
	switch (eRestoreMethod)
	{
		CHECK_CASE_FOR_CONSTANT(VSS_RME_UNDEFINED);
		CHECK_CASE_FOR_CONSTANT(VSS_RME_RESTORE_IF_NOT_THERE);
		CHECK_CASE_FOR_CONSTANT(VSS_RME_RESTORE_IF_CAN_REPLACE);
		CHECK_CASE_FOR_CONSTANT(VSS_RME_STOP_RESTORE_START);
		CHECK_CASE_FOR_CONSTANT(VSS_RME_RESTORE_TO_ALTERNATE_LOCATION);
		CHECK_CASE_FOR_CONSTANT(VSS_RME_RESTORE_AT_REBOOT);
#ifdef VSS_SERVER
		CHECK_CASE_FOR_CONSTANT(VSS_RME_RESTORE_AT_REBOOT_IF_CANNOT_REPLACE);
#endif
		CHECK_CASE_FOR_CONSTANT(VSS_RME_CUSTOM);
		CHECK_CASE_FOR_CONSTANT(VSS_RME_RESTORE_STOP_START);

	default:
		ft.WriteLine(L"Unknown constant: %d",eRestoreMethod);
		_ASSERTE(false);
		return wstring(L"Undefined");
	}
}


// Convert a component type into a string
inline wstring VssWriter::GetStringFromRestoreConditions(VSS_WRITERRESTORE_ENUM eRestoreEnum)
{
	FunctionTracer ft(DBG_INFO);

	ft.Trace(DBG_INFO, L"Interpreting constant %d", (int)eRestoreEnum);
	switch (eRestoreEnum)
	{
		CHECK_CASE_FOR_CONSTANT(VSS_WRE_UNDEFINED);
		CHECK_CASE_FOR_CONSTANT(VSS_WRE_NEVER);
		CHECK_CASE_FOR_CONSTANT(VSS_WRE_IF_REPLACE_FAILS);
		CHECK_CASE_FOR_CONSTANT(VSS_WRE_ALWAYS);

	default:
		ft.WriteLine(L"Unknown constant: %d",eRestoreEnum);
		_ASSERTE(false);
		return wstring(L"Undefined");
	}
}

VOID VssWriter::InitializeStorage( IStorage * pIStorageXmls, INT  iWriterIndex)
{
	m_pIStorageXmls = pIStorageXmls;
	m_iWriterIndex = iWriterIndex;
}

//******************************************
//  VssComponent
//


// Initialize from a IVssWMComponent
void VssComponent::Initialize(wstring writerNameParam, IVssWMComponent * pComponent)
{
	FunctionTracer ft(DBG_INFO);

	m_writerName = writerNameParam;

	// Get the component info
	PVSSCOMPONENTINFO pInfo = NULL;
	CHECK_COM(pComponent->GetComponentInfo (&pInfo));

	// Initialize local members
	m_name = BSTR2WString(pInfo->bstrComponentName);
	m_logicalPath = BSTR2WString(pInfo->bstrLogicalPath);
	m_caption = BSTR2WString(pInfo->bstrCaption);
	m_type = pInfo->type;
	m_isSelectable = pInfo->bSelectable;
	m_notifyOnBackupComplete = pInfo->bNotifyOnBackupComplete;

	// Compute the full path
	m_fullPath = AppendBackslash(m_logicalPath) + m_name;
	if (m_fullPath[0] != L'\\')
		m_fullPath = wstring(L"\\") + m_fullPath;

	// Get file list descriptors
	m_descriptors.resize(pInfo->cFileCount + pInfo->cDatabases + pInfo->cLogFiles); //sonmi01 avoid copy too much04
	INT nIndexFile = 0;
	for(unsigned i = 0; i < pInfo->cFileCount; i++)
	{
		CComPtr<IVssWMFiledesc> pFileDesc;
		CHECK_COM(pComponent->GetFile (i, &pFileDesc));

		VssFileDescriptor  & desc = m_descriptors[nIndexFile++];
		desc.Initialize(pFileDesc, VSS_FDT_FILELIST);
		//descriptors.push_back(desc); //sonmi01 avoid copy too much05
	}

	// Get database descriptors
	for(unsigned i = 0; i < pInfo->cDatabases; i++)
	{
		CComPtr<IVssWMFiledesc> pFileDesc;
		CHECK_COM(pComponent->GetDatabaseFile (i, &pFileDesc));

		VssFileDescriptor  & desc = m_descriptors[nIndexFile++];
		desc.Initialize(pFileDesc, VSS_FDT_DATABASE);
		//descriptors.push_back(desc);
	}

	// Get log descriptors
	for(unsigned i = 0; i < pInfo->cLogFiles; i++)
	{
		CComPtr<IVssWMFiledesc> pFileDesc;
		CHECK_COM(pComponent->GetDatabaseLogFile (i, &pFileDesc));

		VssFileDescriptor  & desc = m_descriptors[nIndexFile++];
		desc.Initialize(pFileDesc, VSS_FDT_DATABASE_LOG);
		//descriptors.push_back(desc);
	}


#ifdef VSS_SERVER
	// Get dependencies
	m_dependencies.resize(pInfo->cDependencies);
	for(unsigned i = 0; i < pInfo->cDependencies; i++)
	{
		CComPtr<IVssWMDependency> pDependency;
		CHECK_COM(pComponent->GetDependency(i, &pDependency));

		VssDependency & dependency = m_dependencies[i];
		dependency.Initialize(pDependency);
		//dependencies.push_back(dependency); //sonmi01 avoid copy too much06
	}
#endif


	pComponent->FreeComponentInfo (pInfo);

	// Compute the affected paths and volumes
	for(unsigned i = 0; i < m_descriptors.size(); i++)
	{
		if (!FindStringInList(m_descriptors[i].m_expandedPath, m_affectedPaths))
			m_affectedPaths.push_back(m_descriptors[i].m_expandedPath);

		if (!FindStringInList(m_descriptors[i].m_affectedVolume, m_affectedVolumes))
			m_affectedVolumes.push_back(m_descriptors[i].m_affectedVolume);
	}


	sort( m_affectedPaths.begin( ), m_affectedPaths.end( ) );
}


// Initialize from a IVssComponent
void VssComponent::Initialize(wstring writerNameParam, IVssComponent * pComponent)
{
	FunctionTracer ft(DBG_INFO);

	m_writerName = writerNameParam;

	// Get component type
	CHECK_COM(pComponent->GetComponentType(&m_type));

	// Get component name
	CComBSTR bstrComponentName;
	CHECK_COM(pComponent->GetComponentName(&bstrComponentName));
	m_name = BSTR2WString(bstrComponentName);

	// Get component logical path
	CComBSTR bstrLogicalPath;
	CHECK_COM(pComponent->GetLogicalPath(&bstrLogicalPath));
	m_logicalPath = BSTR2WString(bstrLogicalPath);

	// Compute the full path
	m_fullPath = AppendBackslash(m_logicalPath) + m_name;
	if (m_fullPath[0] != L'\\')
		m_fullPath = wstring(L"\\") + m_fullPath;
}


// Print summary/detalied information about this component
void VssComponent::Print(bool bListDetailedInfo)
{
	FunctionTracer ft(DBG_INFO);

	// Print writer identity information
	ft.WriteLine(L"    - Component \"%s\"\n"
		L"       - Name: '%s'\n"
		L"       - Logical Path: '%s'\n"
		L"       - Full Path: '%s'\n"
		L"       - Caption: '%s'\n"
		L"       - Type: %s [%d]\n"
		L"       - Is Selectable: '%s'\n"
		L"       - Is top level: '%s'\n"
		L"       - Notify on backup complete: '%s'",
		(m_writerName + L":" + m_fullPath).c_str(),
		m_name.c_str(),
		m_logicalPath.c_str(),
		m_fullPath.c_str(),
		m_caption.c_str(),
		GetStringFromComponentType(m_type).c_str(), m_type,
		BOOL2TXT(m_isSelectable),
		BOOL2TXT(m_isTopLevel),
		BOOL2TXT(m_notifyOnBackupComplete)
		);

	// Compute the affected paths and volumes
	if (bListDetailedInfo)
	{
		ft.WriteLine(L"       - Components:");
		for(unsigned i = 0; i < m_descriptors.size(); i++)
			m_descriptors[i].Print();
	}

	// Print the affected paths and volumes
	ft.WriteLine(L"       - Affected paths by this component:");
	for(unsigned i = 0; i < m_affectedPaths.size(); i++)
		ft.WriteLine(L"         - %s", m_affectedPaths[i].c_str());

	ft.WriteLine(L"       - Affected volumes by this component:");
	for(unsigned i = 0; i < m_affectedVolumes.size(); i++)
	{
		wstring wsLocalVolume;

		if (GetDisplayNameForVolumeNoThrow(m_affectedVolumes[i], wsLocalVolume))
			ft.WriteLine(L"         - %s [%s]", m_affectedVolumes[i].c_str(), wsLocalVolume.c_str());
		else
			ft.WriteLine(L"         - %s [Not valid for local machine]", m_affectedVolumes[i].c_str());

	}


	// Print dependencies on Windows Server 2003
#ifdef VSS_SERVER

	ft.WriteLine(L"       - Component Dependencies:");
	for(unsigned i = 0; i < m_dependencies.size(); i++)
		m_dependencies[i].Print();

#endif 
}


// Convert a component type into a string
inline wstring VssComponent::GetStringFromComponentType(VSS_COMPONENT_TYPE eComponentType)
{
	FunctionTracer ft(DBG_INFO);

	ft.Trace(DBG_INFO, L"Interpreting constant %d", (int)eComponentType);
	switch (eComponentType)
	{
		CHECK_CASE_FOR_CONSTANT(VSS_CT_DATABASE);
		CHECK_CASE_FOR_CONSTANT(VSS_CT_FILEGROUP);

	default:
		ft.WriteLine(L"Unknown constant: %d",eComponentType);
		_ASSERTE(false);
		return wstring(L"Undefined");
	}
}


// Return TRUE if the current component is parent of the given component
bool VssComponent::IsAncestorOf(VssComponent & descendent)
{
	// The child must have a longer full path
	if (descendent.m_fullPath.length() <= m_fullPath.length())
		return false;

	wstring fullPathAppendedWithBackslash = AppendBackslash(m_fullPath);
	wstring descendentPathAppendedWithBackslash = AppendBackslash(descendent.m_fullPath);

	// Return TRUE if the current full path is a prefix of the child full path
	return IsEqual(fullPathAppendedWithBackslash, 
		descendentPathAppendedWithBackslash.substr(0, 
		fullPathAppendedWithBackslash.length()));
}


// Return TRUE if the current component is parent of the given component
bool VssComponent::CanBeExplicitlyIncluded()
{
	if (m_isExcluded)
		return false;

	// selectable can be explictly included
	if (m_isSelectable) 
		return true;

	// Non-selectable top level can be explictly included
	if (m_isTopLevel)
		return true;

	return false;
}







//******************************************
//  VssFileDescriptor
//



// Initialize a file descriptor from a 
void VssFileDescriptor::Initialize(
	IVssWMFiledesc * pFileDesc, 
	VSS_DESCRIPTOR_TYPE typeParam
	)
{
	FunctionTracer ft(DBG_INFO);

	// Set the type
	m_type = typeParam;

	CComBSTR bstrPath;
	CHECK_COM(pFileDesc->GetPath(&bstrPath));

	CComBSTR bstrFilespec;
	CHECK_COM(pFileDesc->GetFilespec (&bstrFilespec));

	bool bRecursive = false;
	CHECK_COM(pFileDesc->GetRecursive(&bRecursive));

	CComBSTR bstrAlternate;
	CHECK_COM(pFileDesc->GetAlternateLocation(&bstrAlternate));

	// Initialize local data members
	m_path = BSTR2WString(bstrPath);
	m_filespec = BSTR2WString(bstrFilespec);
	m_alternatePath = BSTR2WString(bstrAlternate);
	m_isRecursive = bRecursive;

	// Get the expanded path
	WCHAR expandedPathTmp[MAX_PATH] = {0};
	_ASSERTE(bstrPath && bstrPath[0]);
	CHECK_WIN32(ExpandEnvironmentStringsW(bstrPath, (PWCHAR)expandedPathTmp, _countof(expandedPathTmp)));
	m_expandedPath = AppendBackslash(expandedPathTmp);

	// Get the affected volume 
	if (!GetUniqueVolumeNameForPathNoThrow(m_expandedPath, m_affectedVolume))
	{
		m_affectedVolume = m_expandedPath;
	}

}


// Print a file description object
inline void VssFileDescriptor::Print()
{
	FunctionTracer ft(DBG_INFO);

	wstring alternateDisplayPath;
	if (m_alternatePath.length() > 0)
		alternateDisplayPath = wstring(L", Alternate Location = ") + m_alternatePath;

	ft.WriteLine(L"       - %s: Path = %s, Filespec = %s%s%s",
		GetStringFromFileDescriptorType(m_type).c_str(),
		m_path.c_str(),
		m_filespec.c_str(),
		m_isRecursive? L", Recursive": L"",
		alternateDisplayPath.c_str());
}


// Convert a component type into a string
wstring VssFileDescriptor::GetStringFromFileDescriptorType(VSS_DESCRIPTOR_TYPE eType)
{
	FunctionTracer ft(DBG_INFO);

	ft.Trace(DBG_INFO, L"Interpreting constant %d", (int)eType);
	switch (eType)
	{
	case VSS_FDT_UNDEFINED:     return L"Undefined";
	case VSS_FDT_EXCLUDE_FILES: return L"Exclude";
	case VSS_FDT_FILELIST:      return L"File List";
	case VSS_FDT_DATABASE:      return L"Database";
	case VSS_FDT_DATABASE_LOG:  return L"Database Log";

	default:
		ft.WriteLine(L"Unknown constant: %d",eType);
		_ASSERTE(false);
		return wstring(L"Undefined");
	}
}


//******************************************
//  VssDependency
//

#ifdef VSS_SERVER


// Initialize a file descriptor from a 
void VssDependency::Initialize(
	IVssWMDependency * pDependency
	)
{
	FunctionTracer ft(DBG_INFO);

	VSS_ID guidWriterId;
	CHECK_COM(pDependency->GetWriterId(&guidWriterId));

	CComBSTR bstrLogicalPath;
	CHECK_COM(pDependency->GetLogicalPath(&bstrLogicalPath));

	CComBSTR bstrComponentName;
	CHECK_COM(pDependency->GetComponentName(&bstrComponentName));

	// Initialize local data members
	m_writerId = Guid2WString(guidWriterId);
	m_logicalPath = BSTR2WString(bstrLogicalPath);
	m_componentName = BSTR2WString(bstrComponentName);

	// Compute the full path
	m_fullPath = AppendBackslash(m_logicalPath) + m_componentName;
	if (m_fullPath[0] != L'\\')
		m_fullPath = wstring(L"\\") + m_fullPath;
}


// Print a file description object
inline void VssDependency::Print()
{
	FunctionTracer ft(DBG_INFO);

	ft.WriteLine(L"       - Dependency to \"%s:%s%s\"",
		m_writerId.c_str(), 
		m_fullPath.c_str());
}

#endif
