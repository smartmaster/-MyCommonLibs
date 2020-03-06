/////////////////////////////////////////////////////////////////////////
// Copyright ?Microsoft Corporation. All rights reserved.
// 
//  This file may contain preliminary information or inaccuracies, 
//  and may not correctly represent any associated Microsoft 
//  Product as commercially released. All Materials are provided entirely 
//  ???????To the extent permitted by law, MICROSOFT MAKES NO 
//  WARRANTY OF ANY KIND, DISCLAIMS ALL EXPRESS, IMPLIED AND STATUTORY 
//  WARRANTIES, AND ASSUMES NO LIABILITY TO YOU FOR ANY DAMAGES OF 
//  ANY TYPE IN CONNECTION WITH THESE MATERIALS OR ANY INTELLECTUAL PROPERTY IN THEM. 
// 


// Main header
#include "stdafx.h"
#include "MyVssResults.h" //<sonmi01>2015-6-2 ###???
#include "MyProviderInfo.h"


// Query all the shadow copies in the given set
// If snapshotSetID is NULL, just query all shadow copies in the system
void VssClient::QuerySnapshotSet(VSS_ID snapshotSetID, wstring bcdFileName)
{
    FunctionTracer ft(DBG_INFO);

	CString strResultFile; //<sonmi01>2015-6-2 ###???
	if (bcdFileName.size())
	{
		strResultFile = bcdFileName.c_str();
		strResultFile += TEXT(".VssResults.xml");
	}

    if (snapshotSetID == GUID_NULL)
        ft.WriteLine(L"Querying all shadow copies in the system ...");
    else
        ft.WriteLine(L"Querying all shadow copies with the SnapshotSetID " WSTR_GUID_FMT L" ...", GUID_PRINTF_ARG(snapshotSetID));

	My_VSS_Results vssResults;
	if (strResultFile.GetLength())
	{
		vssResults.SnapshotSetID = snapshotSetID;
	}
    
    // Get list all shadow copies. 
    CComPtr<IVssEnumObject> pIEnumSnapshots;
    HRESULT hr = m_pVssObject->Query( GUID_NULL, 
            VSS_OBJECT_NONE, 
            VSS_OBJECT_SNAPSHOT, 
            &pIEnumSnapshots );

    CHECK_COM_ERROR(hr, L"m_pVssObject->Query(GUID_NULL, VSS_OBJECT_NONE, VSS_OBJECT_SNAPSHOT, &pIEnumSnapshots )")

    // If there are no shadow copies, just return
    if (hr == S_FALSE) {
        if (snapshotSetID == GUID_NULL)
            ft.WriteLine(L"There are no shadow copies in the system");
        return;
    } 

    // Enumerate all shadow copies. 
    VSS_OBJECT_PROP Prop;
    VSS_SNAPSHOT_PROP& Snap = Prop.Obj.Snap;
    
    while(true)
    {
        // Get the next element
        ULONG ulFetched;
        hr = pIEnumSnapshots->Next( 1, &Prop, &ulFetched );
        CHECK_COM_ERROR(hr, L"pIEnumSnapshots->Next( 1, &Prop, &ulFetched )")

        // We reached the end of list
        if (ulFetched == 0)
            break;

        // Automatically call VssFreeSnapshotProperties on this structure at the end of scope
        CAutoSnapPointer snapAutoCleanup(&Snap);

        // Print the shadow copy (if not filtered out)
        if ((snapshotSetID == GUID_NULL) || (Snap.m_SnapshotSetId == snapshotSetID))
        {
			PrintSnapshotProperties(Snap);
			if (strResultFile.GetLength()) //<sonmi01>2015-6-2 ###???
			{
				My_VSS_SNAPSHOT_PROP myVssProp;
				myVssProp.FromVSS_SNAPSHOT_PROP(Snap);
				vssResults.SnapshotProps.push_back(myVssProp);
			}
        }
            
    }


	if (strResultFile.GetLength()) //<sonmi01>2015-6-2 ###???
	{
		vssResults.ToFile(FIELD_NAME(My_VSS_Results), strResultFile.GetString());
	}
}

//<sonmi01>2016-10-11 ###???
void VssClient::QueryProviders(OUT vector<MyProviderInfo>& vecPI)
{
	FunctionTracer ft(DBG_INFO);

	ft.WriteLine(L"Querying VSS providers in the system ...");


	// Get list all shadow copies. 
	CComPtr<IVssEnumObject> pIEnumProviders;
	HRESULT hr = m_pVssObject->Query(GUID_NULL,
		VSS_OBJECT_NONE,
		VSS_OBJECT_PROVIDER,
		&pIEnumProviders);

	CHECK_COM_ERROR(hr, L"m_pVssObject->Query(GUID_NULL, VSS_OBJECT_NONE, VSS_OBJECT_PROVIDER, &pIEnumSnapshots )")

		// Enumerate all shadow copies. 
		VSS_OBJECT_PROP Prop;
	VSS_PROVIDER_PROP& Prov = Prop.Obj.Prov;
	int index = 0;
	while (true)
	{
		// Get the next element
		ULONG ulFetched;
		hr = pIEnumProviders->Next(1, &Prop, &ulFetched);
		CHECK_COM_ERROR(hr, L"pIEnumSnapshots->Next( 1, &Prop, &ulFetched )")

			// We reached the end of list
			if (ulFetched == 0)
				break;

		// Automatically call VssFreeSnapshotProperties on this structure at the end of scope
		CAutoProvPointer provAutoCleanup(&Prov);

		vecPI.push_back(MyProviderInfo(
			Prov.m_ProviderId,
			Prov.m_pwszProviderName,
			Prov.m_eProviderType,
			Prov.m_pwszProviderVersion,
			Prov.m_ProviderVersionId,
			Prov.m_ClassId
			));
		
		ft.WriteLine(L"VSS provider [%d, ProviderId=%s]", index, Guid2WString(Prov.m_ProviderId).c_str());
		ft.WriteLine(L"VSS provider [%d, pwszProviderName=%s]", index, Prov.m_pwszProviderName);
		ft.WriteLine(L"VSS provider [%d, eProviderType=%d]", index, Prov.m_eProviderType);
		ft.WriteLine(L"VSS provider [%d, pwszProviderVersion=%s]", index, Prov.m_pwszProviderVersion);
		ft.WriteLine(L"VSS provider [%d, ProviderVersionId=%s]", index, Guid2WString(Prov.m_ProviderVersionId).c_str());
		ft.WriteLine(L"VSS provider [%d, ClassId=%s]", index, Guid2WString(Prov.m_ClassId).c_str());
		ft.WriteLine(L"VSS provider [%d]\t\t\t\t\t", index);
		++index;

#if 0
		_ftprintf_s(stdout, TEXT("m_ProviderId=%s") TEXT("\r\n"), Guid2WString(Prov.m_ProviderId).c_str());
		_ftprintf_s(stdout, TEXT("m_pwszProviderName=%s") TEXT("\r\n"), Prov.m_pwszProviderName);
		_ftprintf_s(stdout, TEXT("m_eProviderType=%d") TEXT("\r\n"), Prov.m_eProviderType);
		_ftprintf_s(stdout, TEXT("m_pwszProviderVersion=%s") TEXT("\r\n"), Prov.m_pwszProviderVersion);
		_ftprintf_s(stdout, TEXT("m_ProviderVersionId=%s") TEXT("\r\n"), Guid2WString(Prov.m_ProviderVersionId).c_str());
		_ftprintf_s(stdout, TEXT("m_ClassId=%s") TEXT("\r\n"), Guid2WString(Prov.m_ClassId).c_str());
		_ftprintf_s(stdout, TEXT("") TEXT("\r\n"));
#endif
	}
}

// Query the properties of the given shadow copy
void VssClient::GetSnapshotProperties(VSS_ID snapshotID)
{
    FunctionTracer ft(DBG_INFO);

    // Get the shadow copy properties
    VSS_SNAPSHOT_PROP Snap;
    CHECK_COM(m_pVssObject->GetSnapshotProperties(snapshotID, &Snap));

    // Automatically call VssFreeSnapshotProperties on this structure at the end of scope
    CAutoSnapPointer snapAutoCleanup(&Snap);

    // Print the properties of this shadow copy 
    PrintSnapshotProperties(Snap);
}


// Print the properties for the given snasphot
void VssClient::PrintSnapshotProperties(VSS_SNAPSHOT_PROP & prop)
{
    FunctionTracer ft(DBG_INFO);
    
    LONG lAttributes = prop.m_lSnapshotAttributes;

    ft.WriteLine(L"* SNAPSHOT ID = " WSTR_GUID_FMT L" ...", GUID_PRINTF_ARG(prop.m_SnapshotId));
    ft.WriteLine(L"   - Shadow copy Set: " WSTR_GUID_FMT, GUID_PRINTF_ARG(prop.m_SnapshotSetId));
    ft.WriteLine(L"   - Original count of shadow copies = %d", prop.m_lSnapshotsCount);
    ft.WriteLine(L"   - Original Volume name: %s [%s]", 
        prop.m_pwszOriginalVolumeName, 
        GetDisplayNameForVolume(prop.m_pwszOriginalVolumeName).c_str()
        );
    ft.WriteLine(L"   - Creation Time: %s", VssTimeToString(prop.m_tsCreationTimestamp).c_str());
    ft.WriteLine(L"   - Shadow copy device name: %s", prop.m_pwszSnapshotDeviceObject);
    ft.WriteLine(L"   - Originating machine: %s", prop.m_pwszOriginatingMachine);
    ft.WriteLine(L"   - Service machine: %s", prop.m_pwszServiceMachine);

    if (prop.m_lSnapshotAttributes & VSS_VOLSNAP_ATTR_EXPOSED_LOCALLY)
        ft.WriteLine(L"   - Exposed locally as: %s", prop.m_pwszExposedName);
    else if (prop.m_lSnapshotAttributes & VSS_VOLSNAP_ATTR_EXPOSED_REMOTELY) 
    {
        ft.WriteLine(L"   - Exposed remotely as %s", prop.m_pwszExposedName);
        if (prop.m_pwszExposedPath && wcslen(prop.m_pwszExposedPath) > 0)
            ft.WriteLine(L"   - Path exposed: %s", prop.m_pwszExposedPath);
    }
    else
        ft.WriteLine(L"   - Not Exposed");

    ft.WriteLine(L"   - Provider id: " WSTR_GUID_FMT, GUID_PRINTF_ARG(prop.m_ProviderId));

    // Display the attributes
    wstring attributes;
    if (lAttributes & VSS_VOLSNAP_ATTR_TRANSPORTABLE)
        attributes  += wstring(L" Transportable");
    
    if (lAttributes & VSS_VOLSNAP_ATTR_NO_AUTO_RELEASE)
        attributes  += wstring(L" No_Auto_Release");
    else
        attributes  += wstring(L" Auto_Release");

    if (lAttributes & VSS_VOLSNAP_ATTR_PERSISTENT)
        attributes  += wstring(L" Persistent");

    if (lAttributes & VSS_VOLSNAP_ATTR_CLIENT_ACCESSIBLE)
        attributes  += wstring(L" Client_accessible");

    if (lAttributes & VSS_VOLSNAP_ATTR_HARDWARE_ASSISTED)
        attributes  += wstring(L" Hardware");

    if (lAttributes & VSS_VOLSNAP_ATTR_NO_WRITERS)
        attributes  += wstring(L" No_Writers");

    if (lAttributes & VSS_VOLSNAP_ATTR_IMPORTED)
        attributes  += wstring(L" Imported");

    if (lAttributes & VSS_VOLSNAP_ATTR_PLEX)
        attributes  += wstring(L" Plex");
    
    if (lAttributes & VSS_VOLSNAP_ATTR_DIFFERENTIAL)
        attributes  += wstring(L" Differential");

    ft.WriteLine(L"   - Attributes: %s", attributes.c_str());
    
    ft.WriteLine(L"");
}

