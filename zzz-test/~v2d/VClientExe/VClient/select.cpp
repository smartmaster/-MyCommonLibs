/////////////////////////////////////////////////////////////////////////
// Copyright ?2006 Microsoft Corporation. All rights reserved.
// 
//  This file may contain preliminary information or inaccuracies, 
//  and may not correctly represent any associated Microsoft 
//  Product as commercially released. All Materials are provided entirely 
//  “AS IS.?To the extent permitted by law, MICROSOFT MAKES NO 
//  WARRANTY OF ANY KIND, DISCLAIMS ALL EXPRESS, IMPLIED AND STATUTORY 
//  WARRANTIES, AND ASSUMES NO LIABILITY TO YOU FOR ANY DAMAGES OF 
//  ANY TYPE IN CONNECTION WITH THESE MATERIALS OR ANY INTELLECTUAL PROPERTY IN THEM. 
// 


// Main header
#include "stdafx.h"



////////////////////////////////////////////////////////////////////////////////////
//  Main routines for writer component selection
//


// Select the maximum number of components such that their 
// file descriptors are pointing only to volumes to be shadow copied
void VssClient::SelectComponentsForBackup(
        vector<wstring> shadowSourceVolumes, 
        vector<wstring> excludedWriterAndComponentList,
        vector<wstring> includedWriterAndComponentList
        )
{
    FunctionTracer ft(DBG_INFO);

    // First, exclude all components that have data outside of the shadow set
    DiscoverDirectlyExcludedComponents(excludedWriterAndComponentList, m_writerList);

    // Then discover excluded components that have file groups outside the shadow set
    DiscoverNonShadowedExcludedComponents(shadowSourceVolumes);

    // Now, exclude all componenets that are have directly excluded descendents
    DiscoverAllExcludedComponents();

    // Next, exclude all writers that:
    // - either have a top-level nonselectable excluded component
    // - or do not have any included components (all its components are excluded)
    DiscoverExcludedWriters();

    // Now, discover the components that should be included (explicitly or implicitly)
    // These are the top components that do not have any excluded children
    DiscoverExplicitelyIncludedComponents();

    // Verify if the specified writers/components were included
    ft.WriteLine(L"Verifying explicitly specified writers/components ...");

    for(unsigned i = 0; i < includedWriterAndComponentList.size(); i++)
    {
        // Check whether a component or a writer is specified
        if (includedWriterAndComponentList[i].find(L':') != wstring::npos)
            VerifyExplicitelyIncludedComponent(includedWriterAndComponentList[i], m_writerList);
        else
            VerifyExplicitelyIncludedWriter(includedWriterAndComponentList[i], m_writerList);
    }

    // Finally, select the explicitly included components
    SelectExplicitelyIncludedComponents();
}


// Select the maximum number of components such that their 
// file descriptors are pointing only to volumes to be shadow copied
void VssClient::SelectComponentsForRestore(
        vector<wstring> excludedWriterAndComponentList,
        vector<wstring> includedWriterAndComponentList
        )
{
    FunctionTracer ft(DBG_INFO);

    // First, exclude all components that have data outside of the shadow set
    DiscoverDirectlyExcludedComponents(excludedWriterAndComponentList, m_writerComponentsForRestore);

    // Exclude all writers that do not support restore events
    ExcludeWritersWithNoRestoreEvents();

    // Verify if the specified writers/components were included
    ft.WriteLine(L"Verifying explicitly specified writers/components ...");

    for(unsigned i = 0; i < includedWriterAndComponentList.size(); i++)
    {
        // Check whether a component or a writer is specified
        if (includedWriterAndComponentList[i].find(L':') != wstring::npos)
            VerifyExplicitelyIncludedComponent(includedWriterAndComponentList[i], m_writerComponentsForRestore);
        else
            VerifyExplicitelyIncludedWriter(includedWriterAndComponentList[i], m_writerComponentsForRestore);
    }

    // Finally, select the explicitly included components
    SelectNonexcludedComponentsForRestore();
}


// Discover directly excluded components (that were excluded through the command-line)
void VssClient::DiscoverDirectlyExcludedComponents(
    vector<wstring> excludedWriterAndComponentList,
    vector<VssWriter> & writerList
)
{
    FunctionTracer ft(DBG_INFO);

    ft.WriteLine(L"Discover directly excluded components ...");

    // Discover components that should be excluded from the shadow set 
    // This means components that have at least one File Descriptor requiring 
    // volumes not in the shadow set. 
    for (unsigned iWriter = 0; iWriter < writerList.size(); iWriter++)
    {
        VssWriter & writer = writerList[iWriter];

        // Check if the writer is excluded
        if (FindStringInList(writer.m_name, excludedWriterAndComponentList) || 
            FindStringInList(writer.m_id, excludedWriterAndComponentList) ||
            FindStringInList(writer.m_instanceId, excludedWriterAndComponentList))
        {
            writer.m_isExcluded = true;
            continue;
        }

        // Check if the component is excluded
        for (unsigned iComponent = 0; iComponent < writer.m_components.size(); iComponent++)
        {
            VssComponent & component = writer.m_components[iComponent]; 

            // Check to see if this component is explicitly excluded

            // Compute various component paths
            // Format: Writer:logicaPath\componentName
            wstring componentPathWithWriterName = writer.m_name + L":" + component.m_fullPath;
            wstring componentPathWithWriterID = writer.m_id + L":" + component.m_fullPath;
            wstring componentPathWithWriterIID = writer.m_instanceId + L":" + component.m_fullPath;

            // Check to see if this component is explicitly excluded
            if (FindStringInList(componentPathWithWriterName, excludedWriterAndComponentList) || 
                FindStringInList(componentPathWithWriterID, excludedWriterAndComponentList) ||
                FindStringInList(componentPathWithWriterIID, excludedWriterAndComponentList))
            {
                ft.WriteLine(L"- Component '%s' from writer '%s' is explicitly excluded from backup ",
                    component.m_fullPath.c_str(), writer.m_name.c_str());

                component.m_isExcluded = true;
                continue;
            }
        }

        // Now, discover if we have any selected components. If none, exclude the whole writer
        bool nonExcludedComponents = false;
        for (unsigned i = 0; i < writer.m_components.size(); i++)
        {
            VssComponent & component = writer.m_components[i]; 

            if (!component.m_isExcluded)
                nonExcludedComponents = true;
        }

        // If all components are missing or excluded, then exclude the writer too
        if (!nonExcludedComponents)
        {
            ft.WriteLine(L"- Excluding writer '%s' since it has no selected components for restore.", writer.m_name.c_str());
            writer.m_isExcluded = true;
        }
    }
}


// Exclude writers that do not support restore events
void VssClient::ExcludeWritersWithNoRestoreEvents()
{
    FunctionTracer ft(DBG_INFO);

    ft.WriteLine(L"Exclude writers that do not support restore events ...");

    for (unsigned iWriter = 0; iWriter < m_writerComponentsForRestore.size(); iWriter++)
    {
        VssWriter & writer = m_writerComponentsForRestore[iWriter];

        // Check if the writer is excluded
        if (writer.m_isExcluded)
            continue;

        if (!writer.m_supportsRestore)
        {
            ft.WriteLine(L"- Excluding writer '%s' since it does not support restore events.", writer.m_name.c_str());
            writer.m_isExcluded = true;
        }
    }
}



// Discover excluded components that have file groups outside the shadow set
void VssClient::DiscoverNonShadowedExcludedComponents(
    vector<wstring> shadowSourceVolumes
)
{
    FunctionTracer ft(DBG_INFO);

    ft.WriteLine(L"Discover components that reside outside the shadow set ...");

    // Discover components that should be excluded from the shadow set 
    // This means components that have at least one File Descriptor requiring 
    // volumes not in the shadow set. 
    for (unsigned iWriter = 0; iWriter < m_writerList.size(); iWriter++)
    {
        VssWriter & writer = m_writerList[iWriter];

        // Check if the writer is excluded
        if (writer.m_isExcluded)
            continue;

        // Check if the component is excluded
        for (unsigned iComponent = 0; iComponent < writer.m_components.size(); iComponent++)
        {
            VssComponent & component = writer.m_components[iComponent]; 

            // Check to see if this component is explicitly excluded
            if (component.m_isExcluded)
                continue;

            // Try to find an affected volume outside the shadow set
            // If yes, exclude the component
            for (unsigned iVol = 0; iVol < component.m_affectedVolumes.size(); iVol++)
            {
                if (ClusterIsPathOnSharedVolume(component.m_affectedVolumes[iVol].c_str()))
                {
                    wstring wsUniquePath(MAX_PATH, L'\0');
                    ClusterGetVolumeNameForVolumeMountPoint(component.m_affectedVolumes[iVol].c_str(),
                        WString2Buffer(wsUniquePath),
                        (DWORD)wsUniquePath.length());

                    component.m_affectedVolumes[iVol] = wsUniquePath;
                }

                if (!FindStringInList(component.m_affectedVolumes[iVol], shadowSourceVolumes))
                {
                    wstring wsLocalVolume;

                    if (GetDisplayNameForVolumeNoThrow(component.m_affectedVolumes[iVol], wsLocalVolume))
                        ft.WriteLine(L"- Component '%s' from writer '%s' is excluded from backup (it requires %s in the shadow set)",
                                     component.m_fullPath.c_str(), writer.m_name.c_str(), wsLocalVolume.c_str());
                    else
                        ft.WriteLine(L"- Component '%s' from writer '%s' is excluded from backup", component.m_fullPath.c_str(), writer.m_name.c_str());

                    component.m_isExcluded = true;
                    break;
                }
            }
        }
    }
}



// Discover the components that should not be included (explicitly or implicitly)
// These are componenets that are have directly excluded descendents
void VssClient::DiscoverAllExcludedComponents()
{
    FunctionTracer ft(DBG_INFO);

    ft.WriteLine(L"Discover all excluded components ...");

    // Discover components that should be excluded from the shadow set 
    // This means components that have at least one File Descriptor requiring 
    // volumes not in the shadow set. 
    for (unsigned iWriter = 0; iWriter < m_writerList.size(); iWriter++)
    {
        VssWriter & writer = m_writerList[iWriter];
        if (writer.m_isExcluded)
            continue;

        // Enumerate all components
        for (unsigned i = 0; i < writer.m_components.size(); i++)
        {
            VssComponent & component = writer.m_components[i]; 

            // Check if this component has any excluded children
            // If yes, deselect it
            for (unsigned j = 0; j < writer.m_components.size(); j++)
            {
                VssComponent & descendent = writer.m_components[j];
                if (component.IsAncestorOf(descendent) && descendent.m_isExcluded)
                {
                    ft.WriteLine(L"- Component '%s' from writer '%s' is excluded from backup "
                        L"(it has an excluded descendent: '%s')",
                        component.m_fullPath.c_str(), writer.m_name.c_str(), descendent.m_name.c_str());

                    component.m_isExcluded = true; 
                    break; 
                }
            }
        }
    }
}


// Discover excluded writers. These are writers that:
// - either have a top-level nonselectable excluded component
// - or do not have any included components (all its components are excluded)
void VssClient::DiscoverExcludedWriters()
{
    FunctionTracer ft(DBG_INFO);

    ft.WriteLine(L"Discover excluded writers ...");

    // Enumerate writers
    for (unsigned iWriter = 0; iWriter < m_writerList.size(); iWriter++)
    {
        VssWriter & writer = m_writerList[iWriter];
        if (writer.m_isExcluded)
            continue;

        // Discover if we have any:
        // - non-excluded selectable components 
        // - or non-excluded top-level non-selectable components
        // If we have none, then the whole writer must be excluded from the backup
        writer.m_isExcluded = true;
        for (unsigned i = 0; i < writer.m_components.size(); i++)
        {
            VssComponent & component = writer.m_components[i]; 
            if (component.CanBeExplicitlyIncluded())
            {
                writer.m_isExcluded = false;
                break;
            }
        }

        // No included components were found
        if (writer.m_isExcluded)
        {
            ft.WriteLine(L"- The writer '%s' is now entirely excluded from the backup:", writer.m_name.c_str());
            ft.WriteLine(L"  (it does not contain any components that can be potentially included in the backup)");
            continue;
        }

        // Now, discover if we have any top-level excluded non-selectable component 
        // If this is true, then the whole writer must be excluded from the backup
        for (unsigned i = 0; i < writer.m_components.size(); i++)
        {
            VssComponent & component = writer.m_components[i]; 

            if (component.m_isTopLevel && !component.m_isSelectable && component.m_isExcluded)
            {
                ft.WriteLine(L"- The writer '%s' is now entirely excluded from the backup:", writer.m_name.c_str());
                ft.WriteLine(L"  (the top-level non-selectable component '%s' is an excluded component)",
                    component.m_fullPath.c_str());
                writer.m_isExcluded = true;
                break;
            }
        }
    }
}




// Discover the components that should be explicitly included 
// These are any included top components 
void VssClient::DiscoverExplicitelyIncludedComponents()
{
    FunctionTracer ft(DBG_INFO);

    ft.WriteLine(L"Discover explicitly included components ...");

    // Enumerate all writers
    for (unsigned iWriter = 0; iWriter < m_writerList.size(); iWriter++)
    {
        VssWriter & writer = m_writerList[iWriter];
        if (writer.m_isExcluded)
            continue;

        // Compute the roots of included components
        for (unsigned i = 0; i < writer.m_components.size(); i++)
        {
            VssComponent & component = writer.m_components[i]; 

            if (!component.CanBeExplicitlyIncluded())
                continue;

            // Test if our component has a parent that is also included
            component.m_isExplicitlyIncluded = true;
            for (unsigned j = 0; j < writer.m_components.size(); j++)
            {
                VssComponent & ancestor = writer.m_components[j];
                if (ancestor.IsAncestorOf(component) && ancestor.CanBeExplicitlyIncluded())
                {
                    // This cannot be explicitely included since we have another 
                    // ancestor that that must be (implictely or explicitely) included
                    component.m_isExplicitlyIncluded = false; 
                    break; 
                }
            }
        }
    }
}


// Verify that the given components will be explicitly or implicitly selected
void VssClient::VerifyExplicitelyIncludedComponent(
    wstring includedComponent,
    vector<VssWriter> & writerList
    )
{
    FunctionTracer ft(DBG_INFO);

    ft.WriteLine(L"- Verifing component \"%s\" ...", includedComponent.c_str());

    // Enumerate all writers
    for (unsigned iWriter = 0; iWriter < writerList.size(); iWriter++)
    {
        VssWriter & writer = writerList[iWriter];

        // Ignore explicitly excluded writers
        if (writer.m_isExcluded)
            continue;

        // Find the associated component 
        for (unsigned j = 0; j < writer.m_components.size(); j++)
        {
            VssComponent & component = writer.m_components[j]; 

            // Ignore explicitly excluded components
            if (component.m_isExcluded)
                continue;

            // Compute various component paths
            // Format: Writer:logicaPath\componentName
            wstring componentPathWithWriterName = writer.m_name + L":" + component.m_fullPath;
            wstring componentPathWithWriterID = writer.m_id + L":" + component.m_fullPath;
            wstring componentPathWithWriterIID = writer.m_instanceId + L":" + component.m_fullPath;

            // Check to see if this component is (implicitly or explicitly) included
            if (IsEqual(componentPathWithWriterName, includedComponent) || 
                IsEqual(componentPathWithWriterID, includedComponent) ||
                IsEqual(componentPathWithWriterIID, includedComponent))
            {
                ft.Trace(DBG_INFO, L"- Found component '%s' from writer '%s'", 
                    component.m_fullPath.c_str(), writer.m_name.c_str());

                // If we are during restore, we just found our component
                if (m_bDuringRestore)
                {
                    ft.WriteLine(L"  - The component \"%s\" is selected", includedComponent.c_str());
                    return;
                }

                // If not explicitly included, check to see if there is an explicitly included ancestor
                bool isIncluded = component.m_isExplicitlyIncluded;
                if (!isIncluded)
                {
                    for (unsigned k = 0; k < writer.m_components.size(); k++)
                    {
                        VssComponent & ancestor = writer.m_components[k];
                        if (ancestor.IsAncestorOf(component) && ancestor.m_isExplicitlyIncluded)
                        {
                            isIncluded = true;
                            break;
                        }
                    }
                }

                if (isIncluded)
                {
                    ft.WriteLine(L"  - The component \"%s\" is selected", includedComponent.c_str());
                    return;
                }
                else
                {
                    ft.WriteLine(L"ERROR: The component \"%s\" was not included in the backup! Aborting backup ...", includedComponent.c_str());
                    ft.WriteLine(L"- Please reveiw the component/subcomponent definitions");
                    ft.WriteLine(L"- Also, please verify list of volumes to be shadow copied.");

                    throw(E_INVALIDARG);
                }
            }
        }
    }

    ft.WriteLine(L"ERROR: The component \"%s\" was not found in the writer components list! Aborting backup ...", includedComponent.c_str());
    ft.WriteLine(L"- Please check the syntax of the component name.");

    throw(E_INVALIDARG);
}



// Verify that all the components of this writer are selected
void VssClient::VerifyExplicitelyIncludedWriter(
    wstring writerName,
    vector<VssWriter> & writerList
    )
{
    FunctionTracer ft(DBG_INFO);

    ft.WriteLine(L"- Verifing that all components of writer \"%s\" are included in backup ...", writerName.c_str());

    // Enumerate all writers
    for (unsigned iWriter = 0; iWriter < writerList.size(); iWriter++)
    {
        VssWriter & writer = writerList[iWriter];

        // Ignore explicitly excluded writers
        if (writer.m_isExcluded)
            continue;

        // Check if we found the writer
        if (IsEqual(writerName, writer.m_name) 
            || IsEqual(writerName, writer.m_id) 
            || IsEqual(writerName, writer.m_instanceId))
        {
            if (writer.m_isExcluded)
            {
                ft.WriteLine(L"ERROR: The writer \"%s\" was not included in the backup! Aborting backup ...", writer.m_name.c_str());
                ft.WriteLine(L"- Please reveiw the component/subcomponent definitions");
                ft.WriteLine(L"- Also, please verify list of volumes to be shadow copied.");
                throw(E_INVALIDARG);
            }

            // Make sure all its associated components are selected
            for (unsigned j = 0; j < writer.m_components.size(); j++)
            {
                VssComponent & component = writer.m_components[j]; 

                if (component.m_isExcluded)
                {
                    ft.WriteLine(L"ERROR: The writer \"%s\" has components not included in the backup! Aborting backup ...", writer.m_name.c_str());
                    ft.WriteLine(L"- The component \"%s\" was not included in the backup.", component.m_fullPath.c_str());
                    ft.WriteLine(L"- Please reveiw the component/subcomponent definitions");
                    ft.WriteLine(L"- Also, please verify list of volumes to be shadow copied.");
                    throw(E_INVALIDARG);
                }
            }

            ft.WriteLine(L"   - All components from writer \"%s\" are selected", writerName.c_str());
            return; 
        }
    }

    ft.WriteLine(L"ERROR: The writer \"%s\" was not found! Aborting backup ...", writerName.c_str());
    ft.WriteLine(L"- Please check the syntax of the writer name/id.");

    throw(E_INVALIDARG);
}



// Discover the components that should be explicitly included 
// These are any included top components 
void VssClient::SelectExplicitelyIncludedComponents()
{
    FunctionTracer ft(DBG_INFO);

    ft.WriteLine(L"Select explicitly included components ...");

    // Enumerate all writers
    for (unsigned iWriter = 0; iWriter < m_writerList.size(); iWriter++)
    {
        VssWriter & writer = m_writerList[iWriter];
        if (writer.m_isExcluded)
            continue;

        ft.WriteLine(L" * Writer '%s':", writer.m_name.c_str());

        // Compute the roots of included components
        for (unsigned i = 0; i < writer.m_components.size(); i++)
        {
            VssComponent & component = writer.m_components[i]; 

            if (!component.m_isExplicitlyIncluded)
                continue;

            ft.WriteLine(L"   - Add component %s", component.m_fullPath.c_str());

            // Add the component
            CHECK_COM(m_pVssObject->AddComponent(
                WString2Guid(writer.m_instanceId),
                WString2Guid(writer.m_id),
                component.m_type,
                component.m_logicalPath.c_str(),
                component.m_name.c_str()));
        }
    }
}


// Select non excluded components for restore
void VssClient::SelectNonexcludedComponentsForRestore()
{
    FunctionTracer ft(DBG_INFO);

    ft.WriteLine(L"Select components for restore...");

    // Enumerate all writers
    for (unsigned iWriter = 0; iWriter < m_writerComponentsForRestore.size(); iWriter++)
    {
        VssWriter & writer = m_writerComponentsForRestore[iWriter];

        // Ignore explicitly excluded writers
        if (writer.m_isExcluded)
            continue;

        ft.WriteLine(L" * Writer '%s':", writer.m_name.c_str());

        // Compute the roots of included components
        for (unsigned i = 0; i < writer.m_components.size(); i++)
        {
            VssComponent & component = writer.m_components[i]; 

            // Do not select excluded components
            if (component.m_isExcluded)
                continue;

            ft.WriteLine(L"   - Select component %s", component.m_fullPath.c_str());

            // Select the component for restore
            CHECK_COM(m_pVssObject->SetSelectedForRestore(
                WString2Guid(writer.m_id),
                component.m_type,
                component.m_logicalPath.c_str(),
                component.m_name.c_str(),
                true));
        }
    }
}



// Notify the writer on the restore status
void VssClient::SetFileRestoreStatus(bool bSuccesfullyRestored)
{
    FunctionTracer ft(DBG_INFO);

    ft.WriteLine(L"Set restore status for all components components for restore...");

    //
    // All-or-nothing policy
    //
    // WARNING: this might be insufficient since we cannot distinguish
    // between a partial failed restore and a completely failed restore!
    // A true requestor should be able to make this difference (see the documentation for more details)
    VSS_FILE_RESTORE_STATUS restoreStatus = bSuccesfullyRestored? VSS_RS_ALL: VSS_RS_NONE;

    // Enumerate all writers
    for (unsigned iWriter = 0; iWriter < m_writerComponentsForRestore.size(); iWriter++)
    {
        VssWriter & writer = m_writerComponentsForRestore[iWriter];
        if (writer.m_isExcluded)
            continue;

        ft.WriteLine(L" * Writer '%s':", writer.m_name.c_str());

        // Compute the roots of included components
        for (unsigned i = 0; i < writer.m_components.size(); i++)
        {
            VssComponent & component = writer.m_components[i]; 

            // Do not select excluded components
            if (component.m_isExcluded)
                continue;

            ft.WriteLine(L"   - Select component %s", component.m_fullPath.c_str());

            // Select the component for restore
            CHECK_COM(m_pVssObject->SetFileRestoreStatus(
                WString2Guid(writer.m_id),
                component.m_type,
                component.m_logicalPath.c_str(),
                component.m_name.c_str(),
                restoreStatus)
                );
        }
    }
}



// Returns TRUE if the writer was previously selected
bool VssClient::IsWriterSelected(GUID guidInstanceId)
{
    // If this writer was not selected for backup, ignore it
    wstring instanceId = Guid2WString(guidInstanceId);
    for (unsigned i = 0; i < m_writerList.size(); i++)
        if ( (instanceId == m_writerList[i].m_instanceId) && !m_writerList[i].m_isExcluded)
            return true;

    return false;
}



// Check the status for all selected writers
void VssClient::CheckSelectedWriterStatus()
{
    FunctionTracer ft(DBG_INFO);

    if ((m_dwContext & VSS_VOLSNAP_ATTR_NO_WRITERS) != 0)
        return;

    // Gather writer status to detect potential errors
    GatherWriterStatus();
    
    // Gets the number of writers in the gathered status info
    // (WARNING: GatherWriterStatus must be called before)
    unsigned cWriters = 0;
    CHECK_COM(m_pVssObject->GetWriterStatusCount(&cWriters));

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

        // If the writer is not selected, just continue
        if (!IsWriterSelected(idInstance))
            continue;

        // If the writer is in non-stable state, break
        switch(eWriterStatus)
        {
            case VSS_WS_FAILED_AT_IDENTIFY:
            case VSS_WS_FAILED_AT_PREPARE_BACKUP:
            case VSS_WS_FAILED_AT_PREPARE_SNAPSHOT:
            case VSS_WS_FAILED_AT_FREEZE:
            case VSS_WS_FAILED_AT_THAW:
            case VSS_WS_FAILED_AT_POST_SNAPSHOT:
            case VSS_WS_FAILED_AT_BACKUP_COMPLETE:
            case VSS_WS_FAILED_AT_PRE_RESTORE:
            case VSS_WS_FAILED_AT_POST_RESTORE:
#ifdef VSS_SERVER
            case VSS_WS_FAILED_AT_BACKUPSHUTDOWN:
#endif
                break;

            default:
                continue;
        }

        // Print writer status
        ft.WriteLine(L"\n"
            L"ERROR: Selected writer '%s' is in failed state!\n"
            L"   - Status: %d (%s)\n" 
            L"   - Writer Failure code: 0x%08lx (%s)\n" 
            L"   - Writer ID: " WSTR_GUID_FMT L"\n"
            L"   - Instance ID: " WSTR_GUID_FMT L"\n",
            (PWCHAR)bstrWriterName,
            eWriterStatus, GetStringFromWriterStatus(eWriterStatus).c_str(), 
            hrWriterFailure,FunctionTracer::HResult2String(hrWriterFailure).c_str(),
            GUID_PRINTF_ARG(idWriter),
            GUID_PRINTF_ARG(idInstance)
            );

        // Stop here
        throw(E_UNEXPECTED);
    }
}
