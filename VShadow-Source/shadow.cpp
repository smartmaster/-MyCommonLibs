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
#include <VersionHelpers.h>

bool OSVersionCheck()
{
	return false; //<sonmi01>2014-10-10 ###???

    bool bIsWin8 = FALSE;
    bIsWin8 = IsWindows8OrGreater();
    return !bIsWin8;
}


///////////////////////////////////////////////////////////////////
//
//  Main routine 
//
//  Return values:
//      0 - Success
//      1 - Object not found
//      2 - Runtime Error 
//      3 - Memory allocation error
//
int vshadow_wmain(__in int argc, __in_ecount(argc) WCHAR ** argv)
{
    (void)HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    FunctionTracer ft(DBG_INFO);
    CommandLineParser obj;

    try
    {
        if (OSVersionCheck())
        {
            ft.WriteLine(L"This version of vshadow is not supported on this version of Windows." );
            return 2;
        }

        ft.WriteLine(
            L""
            L"VSHADOW.EXE 3.0 - Volume Shadow Copy sample client."
            L"Copyright (C) 2005 Microsoft Corporation. All rights reserved."
            L""
            );

        // Build the argument vector 
        vector<wstring> arguments;
        for(int i = 1; i < argc; i++)
            arguments.push_back(argv[i]);

        // Process the arguments and execute the main options
        return obj.MainRoutine(arguments);
    }
    catch(bad_alloc ex)
    {
        // Generic STL allocation error
        ft.WriteLine(L"ERROR: Memory allocation error");
        return 3;
    }
    catch(exception ex)
    {
        // We should never get here (unless we have a bug)
        _VSHADOW_ASSERTE(false);
        ft.WriteLine(L"ERROR: STL Exception caught: %S", ex.what());
        return 2;
    }
    catch(HRESULT hr)
    {
        ft.Trace(DBG_INFO, L"HRESULT Error caught: 0x%08lx", hr);
        return 2;
    }
}


//
//  Main routine
//  - First, parse the command-line options
//  - Then execute the appropriate client calls
//
//  WARNING: 
//  - The routine does not check for mutually exclusive flags/options
//
//  On error, this function throws
//
int CommandLineParser::MainRoutine(const vector<wstring> & arguments)
{
    FunctionTracer ft(DBG_INFO);

    // Context for the VSS operation
    DWORD dwContext = VSS_CTX_BACKUP;

    // List of selected writers during the shadow copy creation process
    vector<wstring> excludedWriterList;

    // List of selected writers during the shadow copy creation process
    vector<wstring> includedWriterList;

    // the volume for which to delete the oldest shadow copy
    wstring stringVolume;

    // The script file prefix 
    // Non-empty if scripts have to be generated 
    wstring stringFileName;

    // Script to execute between snapshot creation and backup complete
    wstring execCommand;

    // The backup components document
    wstring xmlBackupComponentsDoc;

    // Resync destination
    wstring wsSnapIdDest;

    // Break flags for BreakSnapshotSet, used by -bex
    DWORD dwBreakExFlags = 0;

    DWORD dwResyncFlags = 0;

    // Enumerate each argument
    for(unsigned argIndex = 0; argIndex < arguments.size(); argIndex++)
    {
        //
        //  Flags
        //

#ifdef VSS_SERVER

        // Check for the persistent flag
        if (MatchArgument(arguments[argIndex], L"p"))
        {
            ft.WriteLine(L"(Option: Persistent shadow copy)");
            m_bPersistent = true;

            // The final dwContext needs to be updated based on m_bPersistent and m_bWithWriters
            continue;
        }

        // Check for the no-writers flag
        if (MatchArgument(arguments[argIndex], L"nw"))
        {
            ft.WriteLine(L"(Option: No-writers option detected)");
            m_bWithWriters = false;

            // The final dwContext needs to be updated based on m_bPersistent and m_bWithWriters
            continue;
        }

        // Check for the no-writers flag
        if (MatchArgument(arguments[argIndex], L"nar"))
        {
            ft.WriteLine(L"(Option: No-auto-recovery option detected)");
            dwContext |= VSS_VOLSNAP_ATTR_NO_AUTORECOVERY;
            continue;
        }

        // Check for the TxF recovery flag
        if (MatchArgument(arguments[argIndex], L"tr"))
        {
            ft.WriteLine(L"(Option: TxF-recover option detected)");
            dwContext |= VSS_VOLSNAP_ATTR_TXF_RECOVERY;
            continue;
        }

        // Check for the transportability option
        if (MatchArgument(arguments[argIndex], L"t", xmlBackupComponentsDoc))
        {
            ft.WriteLine(L"(Option: Transportable shadow set. Saving xml to file '%s')", xmlBackupComponentsDoc.c_str());
            dwContext |= VSS_VOLSNAP_ATTR_TRANSPORTABLE;
            continue;
        }

        // Check for the backup components document option
        if (MatchArgument(arguments[argIndex], L"bc", xmlBackupComponentsDoc))
        {
            ft.WriteLine(L"(Option: Saving xml to file '%s')", xmlBackupComponentsDoc.c_str());
            continue;
        }

        // Check for the differential flag option
        if (MatchArgument(arguments[argIndex], L"ad"))
        {
            ft.WriteLine(L"(Option: Creating differential HW shadow copies)");
            dwContext |= VSS_VOLSNAP_ATTR_DIFFERENTIAL;
            continue;
        }

        // Check for the plex flag option
        if (MatchArgument(arguments[argIndex], L"ap"))
        {
            ft.WriteLine(L"(Option: Creating plex HW shadow copies)");
            dwContext |= VSS_VOLSNAP_ATTR_PLEX;
            continue;
        }

        // Check for the Shadow Copy for Shared Folders (i.e. Client Accessible) flag option
        if (MatchArgument(arguments[argIndex], L"scsf"))
        {
            ft.WriteLine(L"(Option: Creating Shadow Copies for Shared Folders - Client Accessible)");
            dwContext = VSS_CTX_CLIENT_ACCESSIBLE;
            continue;
        }

#ifdef TEST_FEATURES
        // Check for the delayed post snapshot flag
        if (MatchArgument(arguments[argIndex], L"dps"))
        {
            ft.WriteLine(L"(Option: Delayed post-snapshot option detected)");
            dwContext |= VSS_VOLSNAP_ATTR_DELAYED_POSTSNAPSHOT;

            // The final dwContext needs to be updated based on m_bPersistent and m_bWithWriters
            continue;
        }
#endif


#endif

        // Check for the writer exclusion flag
        wstring excludedWriter;
        if (MatchArgument(arguments[argIndex], L"wx", excludedWriter))
        {
            ft.WriteLine(L"(Option: Excluding writer/component '%s')", excludedWriter.c_str());
            excludedWriterList.push_back(excludedWriter);
            continue;
        }

        // Check for the writer inclusion flag
        wstring includedWriter;
        if (MatchArgument(arguments[argIndex], L"wi", includedWriter))
        {
            ft.WriteLine(L"(Option: Verifying inclusion of writer/component '%s')", includedWriter.c_str());
            includedWriterList.push_back(includedWriter);
            continue;
        }

        // Check for the "wait" flag
        if (MatchArgument(arguments[argIndex], L"wait"))
        {
            ft.WriteLine(L"(Option: Wait on finish)");
            m_bWaitForFinish = true;
            continue;
        }

        // Check for the script generation option
        if (MatchArgument(arguments[argIndex], L"script", stringFileName))
        {
            ft.WriteLine(L"(Option: Generate SETVAR script '%s')", stringFileName.c_str());
            continue;
        }

        // Check for the command execution option
        if (MatchArgument(arguments[argIndex], L"exec", execCommand))
        {
            ft.WriteLine(L"(Option: Execute binary/script after shadow creation '%s')", execCommand.c_str());

            // Check if the command is a valid CMD/EXE file
            DWORD dwAttributes = GetFileAttributes((LPWSTR)execCommand.c_str());
            if ((dwAttributes == INVALID_FILE_ATTRIBUTES) || ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
            {
                ft.WriteLine(L"ERROR: the parameter '%s' must be an existing file!", execCommand.c_str());
                ft.WriteLine(L"- Note: the -exec command cannot have parameters!");
                throw(E_INVALIDARG);
            }

            continue;
        }

        // Check for the tracing option
        if (MatchArgument(arguments[argIndex], L"tracing"))
        {
            ft.WriteLine(L"(Option: Enable tracing)");
            FunctionTracer::EnableTracingMode();
            continue;
        }


        // Checks for BreakEx flags

        if (MatchArgument(arguments[argIndex], L"mask"))
        {
            ft.WriteLine(L"(Option: Mask shadow copy luns)");
            dwBreakExFlags |= VSS_BREAKEX_FLAG_MASK_LUNS;
            continue;
        }

        if (MatchArgument(arguments[argIndex], L"rw"))
        {
            ft.WriteLine(L"(Option: make readwrite)");
            dwBreakExFlags |= VSS_BREAKEX_FLAG_MAKE_READ_WRITE;
            continue;
        }

        if (MatchArgument(arguments[argIndex], L"forcerevert"))
        {
            ft.WriteLine(L"(Option: force disk identity revert)");
            dwBreakExFlags |= VSS_BREAKEX_FLAG_REVERT_IDENTITY_ALL;
            continue;
        }

        if (MatchArgument(arguments[argIndex], L"norevert"))
        {
            ft.WriteLine(L"(Option: no disk identity revert)");
            dwBreakExFlags |= VSS_BREAKEX_FLAG_REVERT_IDENTITY_NONE;
            continue;
        }


        //
        //  Operations 
        //

        // Check for /? or -?
        if (MatchArgument(arguments[argIndex], L"?"))
            break;

        // Query all shadow copies in the set 
        if (MatchArgument(arguments[argIndex], L"q"))
        {
            ft.WriteLine(L"(Option: Query all shadow copies)");

            // Initialize the VSS client
            m_vssClient.Initialize(VSS_CTX_ALL);

            // List all shadow copies in the system
            m_vssClient.QuerySnapshotSet(GUID_NULL, L""); //<sonmi01>2015-6-2 ###???

            return 0;
        }

        // Query all shadow copies in the set 
        wstring id;
        if (MatchArgument(arguments[argIndex], L"qx", id))
        {
            ft.WriteLine(L"(Option: Query shadow copy set)");

            GUID queryingSnapshotSetID = WString2Guid(id);

            // Initialize the VSS client
            m_vssClient.Initialize(VSS_CTX_ALL);

            // List all shadow copies in the set
            m_vssClient.QuerySnapshotSet(queryingSnapshotSetID, L""); //<sonmi01>2015-6-2 ###???

            return 0;
        }

        // Query the specified shadow copy 
        if (MatchArgument(arguments[argIndex], L"s", id))
        {
            ft.WriteLine(L"(Option: Query shadow copy)");
            
            VSS_ID queryingSnapshotID = WString2Guid(id);

            // Initialize the VSS client
            m_vssClient.Initialize(VSS_CTX_ALL);

            // List all shadow copies in the set
            m_vssClient.GetSnapshotProperties(queryingSnapshotID);

            return 0;
        }

        // Delete all shadow copies in the system
        if (MatchArgument(arguments[argIndex], L"da"))
        {
            ft.WriteLine(L"(Option: Delete all shadow copies)");

            // Test if the user agrees
#if 0 //<sonmi01>2014-10-11 ###???
            string response;
            cout << "This will delete all shadow copies in the system. Are you sure? [Y/N] ";
            cin >> response;
            if ((response.length() != 1) || ((response[0] != 'Y') && (response[0] != 'y')))
                return 0;
            cout << "";
#endif

            // Initialize the VSS client
            m_vssClient.Initialize(VSS_CTX_ALL);

            // Gather writer metadata
            m_vssClient.DeleteAllSnapshots();

            return 0;
        }

        // Delete a certain shadow copy set
        if (MatchArgument(arguments[argIndex], L"dx", id))
        {
            ft.WriteLine(L"(Option: Delete a shadow copy set)");

            VSS_ID deletingSnapshotSetID = WString2Guid(id);

            // Initialize the VSS client
            m_vssClient.Initialize(VSS_CTX_ALL);

            // Gather writer metadata
            m_vssClient.DeleteSnapshotSet(deletingSnapshotSetID);

            return 0;
        }

        // Delete a certain shadow copy 
        if (MatchArgument(arguments[argIndex], L"ds", id))
        {
            ft.WriteLine(L"(Option: Delete a shadow copy)");

            VSS_ID deletingSnapshotID = WString2Guid(id);

            // Initialize the VSS client
            m_vssClient.Initialize(VSS_CTX_ALL);

            // Gather writer metadata
            m_vssClient.DeleteSnapshot(deletingSnapshotID);

            return 0;
        }

        // List the summary writer metadata
        if (MatchArgument(arguments[argIndex], L"wm"))
        {
            ft.WriteLine(L"(Option: List writer metadata)");

            // Initialize the VSS client
            dwContext = UpdateFinalContext(dwContext);
            m_vssClient.Initialize(dwContext);

            // Gather writer metadata
            m_vssClient.GatherWriterMetadata();

            // List summary writer metadata
            m_vssClient.ListWriterMetadata(false);

            return 0;
        }

        // List the extended writer metadata
        if (MatchArgument(arguments[argIndex], L"wm2"))
        {
            ft.WriteLine(L"(Option: List extended writer metadata)");

            // Initialize the VSS client
            dwContext = UpdateFinalContext(dwContext);
            m_vssClient.Initialize(dwContext);

            // Gather writer metadata
            m_vssClient.GatherWriterMetadata();

            // List writer metadata
            m_vssClient.ListWriterMetadata(true);

            return 0;
        }

        if (MatchArgument(arguments[argIndex], L"wm3"))
        {
            ft.WriteLine(L"(Option: List writer metadata in XML)");

            // Initialize the VSS client
            dwContext = UpdateFinalContext(dwContext);
            m_vssClient.Initialize(dwContext);

            // Gather writer metadata
            m_vssClient.GatherWriterMetadataToScreen();

            return 0;
        }

        // List the writer state
        if (MatchArgument(arguments[argIndex], L"ws"))
        {
            ft.WriteLine(L"(Option: List writer status)");

            // Initialize the VSS client
            dwContext = UpdateFinalContext(dwContext);
            m_vssClient.Initialize(dwContext);

            // Gather writer metadata
            m_vssClient.GatherWriterMetadata();

            // Gather writer status
            m_vssClient.GatherWriterStatus();

            // List writer status
            m_vssClient.ListWriterStatus();

            return 0;
        }

#ifdef VSS_SERVER
        // Delete the oldest shadow copy for a volume
        if (MatchArgument(arguments[argIndex], L"do", stringVolume))
        {
            ft.WriteLine(L"(Option: Delete the oldest shadow copy for %s)", stringVolume.c_str());

            m_vssClient.Initialize(VSS_CTX_ALL);

            m_vssClient.DeleteOldestSnapshot(stringVolume);

            return 0;
        }

        // Revert a volume to the specified shadow copy
        if (MatchArgument(arguments[argIndex], L"revert", id))
        {
            ft.WriteLine(L"(revert a shadow copy)");

            VSS_ID revertingShadowId = WString2Guid(id);

            m_vssClient.Initialize(VSS_CTX_ALL);

            m_vssClient.RevertToSnapshot(revertingShadowId);

            return 0;
        }

        // Break this shadow copy set to readonly
        if (MatchArgument(arguments[argIndex], L"b", id))
        {
            ft.WriteLine(L"(Option: Break shadow copy set)");

            VSS_ID breakingSnapshotSetID = WString2Guid(id);

            // Initialize the VSS client
            m_vssClient.Initialize(VSS_CTX_ALL);

            // List all shadow copies in the set
            m_vssClient.BreakSnapshotSet(breakingSnapshotSetID, false);

            return 0;
        }

        // Break this shadow copy set to writable volumes
        if (MatchArgument(arguments[argIndex], L"bw", id))
        {
            ft.WriteLine(L"(Option: Break shadow copy set as writable)");

            VSS_ID breakingSnapshotSetID = WString2Guid(id);

            // Initialize the VSS client
            m_vssClient.Initialize(VSS_CTX_ALL);

            // Break all shadow copies in the set
            if (m_bWaitForFinish || execCommand.length()>0)
            {
                vector<wstring> volumeList;
                m_vssClient.BreakSnapshotSet(breakingSnapshotSetID, true, &volumeList);

                // Executing the custom command if needed
                if (execCommand.length()>0)
                    ExecCommand(execCommand);

                // Wait if needed
                if (m_bWaitForFinish)
                {
                    ft.WriteLine(L"Press <ENTER> to make the volumes writable...");
#pragma warning(suppress: 6031)  //Intentionally ignore the return value of getchar()
                    getchar();  
                    m_bWaitForFinish = false;
                }

                // Make the volumes read-write
                ft.WriteLine(L"- Making shadow copy devices from " WSTR_GUID_FMT L" read-write...", 
                    GUID_PRINTF_ARG(breakingSnapshotSetID));

                m_vssClient.MakeVolumesReadWrite(volumeList);
            }
            else
            {
                // Break and make them read-write
                m_vssClient.BreakSnapshotSet(breakingSnapshotSetID, true);
            }

            return 0;
        }


        // Break this shadow copy set using BreakSnapshotSetEx
        if (MatchArgument(arguments[argIndex], L"bex", id))
        {
            ft.WriteLine(L"(Option: Break shadow copy set using BreakSnapshotSetEx)");

            VSS_ID breakingSnapshotSetID = WString2Guid(id);

            // Initialize the VSS client
            m_vssClient.Initialize(VSS_CTX_ALL);

            // List all shadow copies in the set
            m_vssClient.BreakSnapshotSetEx(breakingSnapshotSetID, dwBreakExFlags);

            return 0;
        }

        // Expose the shadow copy locally
        wstring exposeArgs;
        if (MatchArgument(arguments[argIndex], L"el", exposeArgs))
        {
            ft.WriteLine(L"(Option: Expose a shadow copy)");

            vector<wstring> exposeArgsArray = SplitWString(exposeArgs, L',');
            if (exposeArgsArray.size() != 2)
            {
                ft.WriteLine(L"ERROR: the -el arguments must contain a GUID and a local path separated by a comma.");
                throw(E_INVALIDARG);
            }

            VSS_ID snapshotID = WString2Guid(exposeArgsArray[0]);

            // Initialize the VSS client
            m_vssClient.Initialize(VSS_CTX_ALL);

            // Expose locally this shadow copy
            m_vssClient.ExposeSnapshotLocally(snapshotID, exposeArgsArray[1]);

            return 0;
        }

        // Expose the shadow copy remotely
        if (MatchArgument(arguments[argIndex], L"er", exposeArgs))
        {
            ft.WriteLine(L"(Option: Expose a shadow copy)");

            vector<wstring> exposeArgsArray = SplitWString(exposeArgs, L',');
            if ((exposeArgsArray.size() != 2) && (exposeArgsArray.size() != 3))
            {
                ft.WriteLine(L"ERROR: the -er arguments must contain a GUID, a share name and an optional local path separated by a comma.");
                throw(E_INVALIDARG);
            }

            VSS_ID snapshotID = WString2Guid(exposeArgsArray[0]);

            // Initialize the VSS client
            m_vssClient.Initialize(VSS_CTX_ALL);

            // Expose this shadow copy as a share
            m_vssClient.ExposeSnapshotRemotely(snapshotID, 
                exposeArgsArray[1], 
                exposeArgsArray.size() == 3? exposeArgsArray[2]: L"");

            return 0;
        }

        // Import with the given XML file
        if (MatchArgument(arguments[argIndex], L"i", xmlBackupComponentsDoc))
        {
            ft.WriteLine(L"(Option: Import shadow copy set from file '%s')", xmlBackupComponentsDoc.c_str());

            // Reading the backup components document
            wstring xmlDoc = ReadFileContents(xmlBackupComponentsDoc);
            ft.Trace(DBG_INFO, L"XML document: '%s'", xmlDoc.c_str());

            // Initialize the VSS client
            m_vssClient.Initialize(VSS_CTX_ALL, xmlDoc);

            // List all shadow copies in the system
            m_vssClient.ImportSnapshotSet();

            // Executing the custom command if needed
            if (execCommand.length() > 0)
                ExecCommand(execCommand);

            return 0;
        }

#endif

        // Perform a restore
        if (MatchArgument(arguments[argIndex], L"r", xmlBackupComponentsDoc))
        {
            ft.WriteLine(L"(Option: Perform a restore)");

            // Reading the backup components document
            wstring xmlDoc = ReadFileContents(xmlBackupComponentsDoc);
            ft.Trace(DBG_INFO, L"XML document: '%s'", xmlDoc.c_str());

            // Initialize the VSS client
            m_vssClient.Initialize(VSS_CTX_ALL, xmlDoc, true);

            // Gather writer metadata
            m_vssClient.GatherWriterMetadata();

            // Gather writer status
            m_vssClient.GatherWriterStatus();

            // List writer status
            m_vssClient.ListWriterStatus();

            // Initialize the list of writers and components for restore
            m_vssClient.InitializeWriterComponentsForRestore();

            // Select required components for restore
            m_vssClient.SelectComponentsForRestore(excludedWriterList, includedWriterList);

            // Issue a PreRestore event to the writers
            m_vssClient.PreRestore();

            // Execute the optional custom command between PreRestore and PostRestore
            try
            {
                // Check selected writer status
				m_vssClient.CheckSelectedWriterStatus(L""); //<sonmi01>2015-6-2 ###???

                // Executing the custom command if needed
                if (execCommand.length() > 0)
                    ExecCommand(execCommand);

            }
            catch(HRESULT)
            {
                // Notify writers about a failed restore
                m_vssClient.SetFileRestoreStatus(false);

                // Issue a PostRestore event to the writers
                m_vssClient.PostRestore();

                throw;
            }

            // Notify writers about a succesful restore
            m_vssClient.SetFileRestoreStatus(true);

            // Issue a PostRestore event to the writers
            m_vssClient.PostRestore();

            // Check selected writer status
			m_vssClient.CheckSelectedWriterStatus(L""); //<sonmi01>2015-6-2 ###???

            ft.WriteLine(L"Restore done.");

            return 0;
        }

        // Perform a simulated restore
        if (MatchArgument(arguments[argIndex], L"rs", xmlBackupComponentsDoc))
        {
            ft.WriteLine(L"(Option: Perform a Simulated restore)");

            // Reading the backup components document
            wstring xmlDoc = ReadFileContents(xmlBackupComponentsDoc);
            ft.Trace(DBG_INFO, L"XML document: '%s'", xmlDoc.c_str());

            // Initialize the VSS client
            m_vssClient.Initialize(VSS_CTX_ALL, xmlDoc, true);

            // Gather writer metadata
            m_vssClient.GatherWriterMetadata();

            // Gather writer status
            m_vssClient.GatherWriterStatus();

            // List writer status
            m_vssClient.ListWriterStatus();

            // Initialize the list of writers and components for restore
            m_vssClient.InitializeWriterComponentsForRestore();

            // Select required components for restore
            m_vssClient.SelectComponentsForRestore(excludedWriterList, includedWriterList);

            ft.WriteLine(L"Restore simulation done.");

            return 0;
        }

        // Add a resync pair
        if (MatchArgument(arguments[argIndex], L"addresync", wsSnapIdDest))
        {

            vector<wstring> resyncArgsArray = SplitWString(wsSnapIdDest, L',');

            VSS_ID snapshotID   = WString2Guid(resyncArgsArray[0]);

            wstring wsDest;

            if (resyncArgsArray.size() == 1)   
                ft.WriteLine(L"(Option: Add snapshot Id for resync to original volume location specified in BCD)");
            else if (resyncArgsArray.size() == 2)
            {
                ft.WriteLine(L"(Option: Add snapshot Id for resync to location: %s)", resyncArgsArray[1].c_str());
                wsDest = resyncArgsArray[1];
            }
            else
                throw(E_INVALIDARG);

            m_vssClient.AddResyncSet(snapshotID, wsDest);

            continue;

        }

        // Set disk signature flag
        if (MatchArgument(arguments[argIndex], L"revertsig"))
        {
            ft.WriteLine(L"(Option: Resync will revert destination to the original disk's signature)");
            dwResyncFlags |= VSS_RECOVERY_REVERT_IDENTITY_ALL;
            continue;
        }

        // Set NoVolCheck flag
        if (MatchArgument(arguments[argIndex], L"novolcheck"))
        {
            ft.WriteLine(L"(Option: novolcheck flag used. Resync will not check for unselected volumes)");
            dwResyncFlags |= VSS_RECOVERY_NO_VOLUME_CHECK;
            continue;
        }
        
        // Perform hardware resync
        if (MatchArgument(arguments[argIndex], L"resync", xmlBackupComponentsDoc))
        {
            wstring     xmlDoc;

            if (xmlBackupComponentsDoc.size())
            {
                if (dwResyncFlags & VSS_RECOVERY_REVERT_IDENTITY_ALL)
                    ft.WriteLine(L"Resyncing using source signature as the final disk signature");
                else
                    ft.WriteLine(L"Resyncing using target signature as the final disk signature");
            }
            else
                throw(E_INVALIDARG);
            
            xmlDoc = ReadFileContents(xmlBackupComponentsDoc);

            m_vssClient.Initialize(VSS_CTX_ALL, xmlDoc, true);
            m_vssClient.DoResync(dwResyncFlags);

            return 0;
        }

		//<sonmi01>2016-10-11 ###???
		if (MatchArgument(arguments[argIndex], L"queryproviders"))
		{
			m_vssClient.Initialize(VSS_CTX_ALL);
			std::vector<MyProviderInfo> vecPI;
			m_vssClient.QueryProviders(vecPI);
			return 0;
		}

        // Check if the arguments are volumes or file share paths. If yes, try to create the shadow copy set 
        if (IsVolume(arguments[argIndex]) || IsUNCPath((VSS_PWSZ)arguments[argIndex].c_str()))
        {
            ft.WriteLine(L"(Option: Create shadow copy set)");

            ft.Trace(DBG_INFO, L"Attempting to create a shadow copy set... (volume %s was added as parameter)", arguments[argIndex].c_str());

            // Make sure that all the arguments are volumes
            vector<wstring> volumeList; 
            volumeList.push_back(GetUniqueVolumeNameForPath(arguments[argIndex], true));

            // Process the rest of the arguments
            for(unsigned i = argIndex + 1; i < arguments.size(); i++)
            {
                if (!(IsVolume(arguments[i]) || IsUNCPath((VSS_PWSZ)arguments[i].c_str())))
                {
                    // No match. Print an error and the usage
                    ft.WriteLine(L"ERROR: invalid parameters %s", GetCommandLine());
                    ft.WriteLine(L"- Parameter %s is expected to be a volume or a file share path!  (shadow copy creation is assumed)", arguments[i].c_str());
                    ft.WriteLine(L"- Example: VSHADOW C:");
                    PrintUsage();
                    return 1;
                }

                // Add the volume to the list
                volumeList.push_back(GetUniqueVolumeNameForPath(arguments[i], true));
            }
            
            // Initialize the VSS client
            dwContext = UpdateFinalContext(dwContext);
            m_vssClient.Initialize(dwContext);

            // Create the shadow copy set
            m_vssClient.CreateSnapshotSet(
                volumeList, 
                xmlBackupComponentsDoc, 
                excludedWriterList,
                includedWriterList
                );

            // Execute BackupComplete, except in fast snapshot creation
            if ((dwContext & VSS_VOLSNAP_ATTR_DELAYED_POSTSNAPSHOT) == 0)
            {
                try
                {
                    // Generate management scripts if needed
                    if (stringFileName.length() > 0)
                        m_vssClient.GenerateSetvarScript(stringFileName);

                    // Executing the custom command if needed
                    if (execCommand.length() > 0)
                        ExecCommand(execCommand);

                }
                catch(HRESULT)
                {
                    // Mark backup failure and exit
                    if ((dwContext & VSS_VOLSNAP_ATTR_NO_WRITERS) == 0)
                        m_vssClient.BackupComplete(false, L""); //<sonmi01>2015-6-2 ###???

                    throw;
                }

                // Complete the backup
                // Note that this will notify writers that the backup is succesful! 
                // (which means eventually log truncation)
                if ((dwContext & VSS_VOLSNAP_ATTR_NO_WRITERS) == 0)
                    m_vssClient.BackupComplete(true, L""); //<sonmi01>2015-6-2 ###???
            }

            ft.WriteLine(L"Snapshot creation done.");
            
            return 0;
        }

        // No match. Print an error and the usage
        ft.WriteLine(L"ERROR: invalid parameter '%s'", arguments[argIndex].c_str());
        PrintUsage();
        return 1;
    }

    PrintUsage();
    return 0;
}



// Returns TRUE if the argument is in the following formats
//  -xxxx
//  /xxxx
// where xxxx is the option pattern
bool CommandLineParser::MatchArgument(wstring argument, wstring optionPattern)
{
    FunctionTracer ft(DBG_INFO);

    ft.Trace(DBG_INFO, L"Matching Arg: '%s' with '%s'", argument.c_str(), optionPattern.c_str());

    bool retVal = (IsEqual(argument, wstring(L"/") + optionPattern) || IsEqual(argument, wstring(L"-") + optionPattern) );

    ft.Trace(DBG_INFO, L"Return: %s", BOOL2TXT(retVal));
    return retVal;
}


// Returns TRUE if the argument is in the following formats
//  -xxxx=yyyy
//  /xxxx=yyyy
// where xxxx is the option pattern and yyyy the additional parameter (eventually enclosed in ' or ")
bool CommandLineParser::MatchArgument(wstring argument, wstring optionPattern, wstring & additionalParameter)
{
    FunctionTracer ft(DBG_INFO);
    
    ft.Trace(DBG_INFO, L"Matching Arg: '%s' with '%s'", argument.c_str(), optionPattern.c_str());

    _VSHADOW_ASSERTE(argument.length() > 0);
    
    if ((argument[0] != L'/') && (argument[0] != L'-'))
        return false;

    // Find the '=' separator between the option and the additional parameter
    size_t equalSignPos = argument.find(L'=');
    if ((equalSignPos == wstring::npos) || (equalSignPos == 0))
        return false;

    ft.Trace(DBG_INFO, L"%s %d", argument.substr(1, equalSignPos - 1).c_str(), equalSignPos);
    
    // Check to see if this is our option
    if (!IsEqual(argument.substr(1, equalSignPos - 1), optionPattern))
        return false;

    // Isolate the additional parameter
    additionalParameter = argument.substr(equalSignPos + 1);

    ft.Trace(DBG_INFO, L"- Additional Param: [%s]", additionalParameter.c_str());

    // We cannot have an empty additional parameter
    if (additionalParameter.length() == 0)
        return false;

    // Eliminate the enclosing quotes, if any


#if 0
    size_t lastPos = additionalParameter.length() - 1;
    if (
		((additionalParameter[0] == L'"') && (additionalParameter[lastPos] == L'"')) ||
		((additionalParameter[0] == L'\"') && (additionalParameter[lastPos] == L'\"')) ||
		((additionalParameter[0] == L'\'') && (additionalParameter[lastPos] == L'\''))
		)
#else
	auto ContainSep = [](const wstring & str) -> bool
	{
		static CONST TCHAR SEP[] = { TEXT('"'), TEXT('\"'), TEXT('\''), TEXT('$') };
		CONST int sep_len = _countof(SEP);

		bool brc = false;
		size_t lastPos = str.length() - 1;
		for (int ii = 0; ii < sep_len; ++ii)
		{
			if ((str[0] == SEP[ii]) && (str[lastPos] == SEP[ii]))
			{
				brc = true;
				break;
			}
		}
		return brc;
	};
	if (ContainSep(additionalParameter))
#endif
	{
		additionalParameter = additionalParameter.substr(1, additionalParameter.length() - 2);
	}
        
    
    ft.Trace(DBG_INFO, L"Return true; (additional param = %s)", additionalParameter.c_str());
    
    return true;
}


// 
//  Prints the command line options
//
void CommandLineParser::PrintUsage()
{
    FunctionTracer ft(DBG_INFO);

#ifdef VSS_SERVER
    ft.WriteLine(
        L"Usage:" L"\r\n"
        L"   VSHADOW [optional flags] [commands]" L"\r\n"
        L"" L"\r\n"
        L"List of optional flags:" L"\r\n"
        L"  -?                 - Displays the usage screen" L"\r\n"
        L"  -p                 - Manages persistent shadow copies" L"\r\n"
        L"  -nw                - Manages no-writer shadow copies" L"\r\n"
        L"  -nar               - Creates shadow copies with no auto-recovery" L"\r\n"
        L"  -tr                - Creates TxF-recovered shadow copies" L"\r\n"
        L"  -ad                - Creates differential HW shadow copies" L"\r\n"
        L"  -ap                - Creates plex HW shadow copies" L"\r\n"
        L"  -scsf              - Creates Shadow Copies for Shared Folders (Client Accessible)" L"\r\n"
        L"  -t={file.xml}      - Transportable shadow set. Generates also the backup components doc." L"\r\n"
        L"  -bc={file.xml}     - Generates the backup components doc for non-transportable shadow set." L"\r\n"
        L"  -wi={Writer Name}  - Verify that a writer/component is included" L"\r\n"
        L"  -wx={Writer Name}  - Exclude a writer/component from set creation or restore" L"\r\n"
        L"  -mask              - BreakSnapshotSetEx flag: Mask shadow copy luns from system on break." L"\r\n"
        L"  -rw                - BreakSnapshotSetEx flag: Make shadow copy luns read-write on break." L"\r\n"
        L"  -forcerevert       - BreakSnapshotSetEx flag: Complete operation only if all disk signatures revertable." L"\r\n"
        L"  -norevert          - BreakSnapshotSetEx flag: Do not revert disk signatures." L"\r\n"
        L"  -revertsig         - Revert to the original disk's signature during resync." L"\r\n"
        L"  -novolcheck        - Ignore volume check during resync. Unselected volumes will be overwritten." L"\r\n"
        L"  -script={file.cmd} - SETVAR script creation" L"\r\n"
        L"  -exec={command}    - Custom command executed after shadow creation, import or between break and make-it-write" L"\r\n"
        L"  -wait              - Wait before program termination or between shadow set break and make-it-write" L"\r\n"
        L"  -tracing           - Runs VSHADOW.EXE with enhanced diagnostics" L"\r\n"
        L""  L"\r\n");
    ft.WriteLine(
        L"List of commands:" L"\r\n"
        L"  {volume list}                   - Creates a shadow set on these volumes" L"\r\n"
        L"  -ws                             - List writer status" L"\r\n"
        L"  -wm                             - List writer summary metadata" L"\r\n"
        L"  -wm2                            - List writer detailed metadata" L"\r\n"
        L"  -wm3                            - List writer detailed metadata in raw XML format" L"\r\n"
        L"  -q                              - List all shadow copies in the system" L"\r\n"
        L"  -qx={SnapSetID}                 - List all shadow copies in this set" L"\r\n"
        L"  -s={SnapID}                     - List the shadow copy with the given ID" L"\r\n"
        L"  -da                             - Deletes all shadow copies in the system" L"\r\n"
        L"  -do={volume}                    - Deletes the oldest shadow of the specified volume" L"\r\n"
        L"  -dx={SnapSetID}                 - Deletes all shadow copies in this set" L"\r\n"
        L"  -ds={SnapID}                    - Deletes this shadow copy" L"\r\n"
        L"  -i={file.xml}                   - Transportable shadow copy import" L"\r\n"
        L"  -b={SnapSetID}                  - Break the given shadow set into read-only volumes" L"\r\n"
        L"  -bw={SnapSetID}                 - Break the shadow set into writable volumes" L"\r\n"
        L"  -bex={SnapSetID}                - Break using BreakSnapshotSetEx and flags, see options for available flags" L"\r\n"
        L"  -el={SnapID},dir                - Expose the shadow copy as a mount point" L"\r\n"
        L"  -el={SnapID},drive              - Expose the shadow copy as a drive letter" L"\r\n"
        L"  -er={SnapID},share              - Expose the shadow copy as a network share" L"\r\n"
        L"  -er={SnapID},share,path         - Expose a child directory from the shadow copy as a share" L"\r\n"
        L"  -r={file.xml}                   - Restore based on a previously-generated Backup Components document" L"\r\n"
        L"  -rs={file.xml}                  - Simulated restore based on a previously-generated Backup Components doc"  L"\r\n"
        L"  -revert={SnapID}                - Revert a volume to the specified shadow copy" L"\r\n"
        L"  -addresync={SnapID},drive       - Resync the given shadow copy to the specified volume" L"\r\n"
        L"  -addresync={SnapID}             - Resync the given shadow copy to it's original volume" L"\r\n"
        L"  -resync=bcd.xml                 - Perform Resync using the specified BCD" L"\r\n"
        L""  L"\r\n");
    ft.WriteLine(
        L"Examples:" L"\r\n"
        L"" L"\r\n"
        L" - Non-persistent shadow copy creation on C: and E:" L"\r\n"
        L"     VSHADOW C: E:" L"\r\n"
        L"" L"\r\n"
        L" - Non-persistent shadow copy creation on a CSV named Volume1" L"\r\n"
        L"     VSHADOW C:\\ClusterStorage\\Volume1" L"\r\n"
        L"" L"\r\n"
        L" - Persistent shadow copy creation on C: (with no writers)" L"\r\n"
        L"     VSHADOW -p -nw C:" L"\r\n"
        L"" L"\r\n"
        L" - Transportable shadow copy creation on X:" L"\r\n"
        L"     VSHADOW -t=file1.xml X:" L"\r\n"
        L"" L"\r\n"
        L" - Transportable shadow copy import" L"\r\n"
        L"     VSHADOW -i=file1.xml" L"\r\n"
        L"" L"\r\n"
        L" - List all shadow copies in the system:" L"\r\n"
        L"     VSHADOW -q" L"\r\n"
        L"" L"\r\n"
        L"Please see the README.DOC file for more details." L"\r\n"
        L"" L"\r\n"
        L"" L"\r\n"
        );
#else
    ft.WriteLine(
        L"Usage:" L"\r\n"
        L"   VSHADOW [optional flags] [commands]" L"\r\n"
        L"" L"\r\n"
        L"List of optional flags:" L"\r\n"
        L"  -?                 - Displays the usage screen" L"\r\n"
        L"  -wi={Writer Name}  - Verify that a writer/component is included" L"\r\n"
        L"  -wx={Writer Name}  - Exclude a writer/component from set creation or restore" L"\r\n"
        L"  -bc={file.xml}     - Generates the backup components document during shadow creation." L"\r\n"
        L"  -script={file.cmd} - SETVAR script creation" L"\r\n"
        L"  -exec={command}    - Custom command executed after shadow creation" L"\r\n"
        L"  -wait              - Wait before program termination " L"\r\n"
        L"  -tracing           - Runs VSHADOW.EXE with enhanced diagnostics" L"\r\n"
        L"" L"\r\n"
        L"List of commands:" L"\r\n"
        L"  {volume list}      - Creates a shadow set on these volumes" L"\r\n"
        L"  -ws                - List writer status" L"\r\n"
        L"  -wm                - List writer summary metadata" L"\r\n"
        L"  -wm2               - List writer detailed metadata" L"\r\n"
        L"  -wm3               - List writer detailed metadata in raw XML format" L"\r\n"
        L"  -q                 - List all shadow copies in the system" L"\r\n"
        L"  -qx={SnapSetID}    - List all shadow copies in this set" L"\r\n"
        L"  -s={SnapID}        - List the shadow copy with the given ID" L"\r\n"
        L"  -da                - Deletes all shadow copies in the system" L"\r\n"
        L"  -dx={SnapSetID}    - Deletes all shadow copies in this set" L"\r\n"
        L"  -ds={SnapID}       - Deletes this shadow copy" L"\r\n"
        L"  -r={file.xml}      - Restore based on a previously-generated Backup Components doc" L"\r\n"
        L"  -rs={file.xml}     - Simulated restore based on a previously-generated Backup Components doc" L"\r\n"
        L"" L"\r\n");
    ft.WriteLine(
        L"Examples:" L"\r\n"
        L"" L"\r\n"
        L" - Non-persistent shadow copy creation on C: and D:" L"\r\n"
        L"     VSHADOW C: E:" L"\r\n"
        L"" L"\r\n"
        L" - List all shadow copies in the system:" L"\r\n"
        L"     VSHADOW -q" L"\r\n"
        L"" L"\r\n"
        L"Please see the README.DOC file for more details." L"\r\n"
        L"" L"\r\n"
        L"" L"\r\n"
        );
#endif
}



DWORD CommandLineParser::UpdateFinalContext(DWORD dwContext)
{
#ifdef VSS_SERVER
    if (m_bPersistent)
    {
        if (m_bWithWriters) 
            dwContext |= VSS_CTX_APP_ROLLBACK;
        else
            dwContext |= VSS_CTX_NAS_ROLLBACK;
    }
    else
    {
        if (m_bWithWriters) 
            dwContext |= VSS_CTX_BACKUP;
        else
            dwContext |= VSS_CTX_FILE_SHARE_BACKUP;
    }
#endif

    return dwContext;
}



CommandLineParser::CommandLineParser()
{
    // true if it is a is persistent snapshot
    m_bPersistent = false;

    // false if the snapshot creation is without writers
    m_bWithWriters = true;

    // true if the user wants to wait for termination
    m_bWaitForFinish = false;
}



// Destructor
CommandLineParser::~CommandLineParser()
{
    FunctionTracer ft(DBG_INFO);

    if (m_bWaitForFinish)
    {
        ft.WriteLine(L"Press <ENTER> to continue...");

#pragma warning(suppress: 6031)  //Intentionally ignore the return value of getchar() 
        getchar();
    }
}
