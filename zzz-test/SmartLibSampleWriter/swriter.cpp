/*
**++
**
** Copyright (c) Microsoft Corporation
**
**
** Module Name:
**
**	swriter.cpp
**
**
** Abstract:
**
**	Demonstrate a writer
**
**--
*/


///////////////////////////////////////////////////////////////////////////////
// Includes

#include "pch.h"
#include "swriter.h"
#include "log.h"
#include "CodeBlock.h"
#include "CommonHelper.h"
#include "ScopeExec.h"
#include "VolumeHepler.h"
///////////////////////////////////////////////////////////////////////////////

// Initialize the writer
HRESULT STDMETHODCALLTYPE SmlSampleWriter::Initialize()
{
	SmartLib::ScopeExec se
	{ 
		[]() 
		{
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::start")); 
		}, 
		[]() 
		{
				SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::end"));
		}
	};

	HRESULT hr = CVssWriter::Initialize(SmlSampleWriterId, 		// WriterID
				            SmlSampleWriterName, 	        // wszWriterName
					    VSS_UT_USERDATA,		// ut
					    VSS_ST_OTHER);              // st
	if (FAILED(hr))	{
	    SML_LOG_LINE(TEXT(__FUNCTION__) L"::CVssWriter::Initialize failed!");
	    return hr;
	}

    // subscribe for events
	hr = Subscribe();
	if (FAILED(hr))
	    SML_LOG_LINE(TEXT(__FUNCTION__) L"::CVssWriter::Subscribe failed!");

	return hr;
}

// OnIdentify is called as a result of the requestor calling GatherWriterMetadata
bool STDMETHODCALLTYPE SmlSampleWriter::OnIdentify(IN IVssCreateWriterMetadata *pMetadata)
{
#if TRUE
	SmartLib::ScopeExec se
	{
		[]()
		{
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::start"));
		},
		[]()
		{
				SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::end"));
		}
	};


	// Set the restore method to restore if can replace
	HRESULT hr = pMetadata->SetRestoreMethod(VSS_RME_RESTORE_IF_CAN_REPLACE, 
						 NULL,
						 NULL, 
						 VSS_WRE_ALWAYS,
						 false);
	if (FAILED(hr))	{
	    SML_LOG_LINE(TEXT(__FUNCTION__) L"::SetRestoreMethod failed!");	
	    return false;
	}

	
	std::vector<CString> 	vols;
	SmartLib::VolumeHepler::FindAllVolumes(vols);
	//vols.push_back(TEXT("\\fkjfslkdjfslakdfje\\"));

	for (int ii = 0; ii < vols.size(); ++ ii)
	{
		static LPCTSTR COMP_CAPTION = L"SmartLib Sample Writer FileGroup";
		CString strComponentName = vols[ii];
		strComponentName += TEXT(" SmartLib Sample Writer FilesComponent");
		// add simple FileGroup component
		hr = pMetadata->AddComponent(VSS_CT_FILEGROUP,
			NULL,
			strComponentName.GetString(),
			COMP_CAPTION,
			NULL,
			0,
			false,
			true,
			true,
			true);
		if (FAILED(hr)) {
			SML_LOG_LINE(TEXT(__FUNCTION__) L"::AddComponent failed!");
			return false;
		}

		// add some sample files to the group	
		hr = pMetadata->AddFilesToFileGroup(NULL,
			strComponentName.GetString(),//L"FilesComponent",
			vols[ii],//L"C:\\MyFiles",
			TEXT("SmartLib Sample Writer files-*.dat"),//L"*.dat",
			false,
			NULL);
		if (FAILED(hr)) {
			SML_LOG_LINE(TEXT(__FUNCTION__) L"::AddFilesToFileGroup failed!");
			return false;
		}
	}

#if FALSE

	// add simple FileGroup component
	hr = pMetadata->AddComponent(VSS_CT_FILEGROUP,		
				     NULL,
				     L"FilesComponent",
				     L"My FileGroup",
				     NULL,
				     0,
				     false,
				     true,
				     true,
			             true);
	if (FAILED(hr))	{
	    SML_LOG_LINE(TEXT(__FUNCTION__) L"::AddComponent failed!");	
	    return false;
	}

    // add some sample files to the group	
	hr = pMetadata->AddFilesToFileGroup(NULL,
					    L"FilesComponent",
					    L"C:\\MyFiles",
				            L"*.dat",
					    false,
					    NULL);
	if (FAILED(hr))	{
	    SML_LOG_LINE(TEXT(__FUNCTION__) L"::AddFilesToFileGroup failed!");	
	    return false;
	}
#endif


	CComBSTR xml; //<sonmi01>2018-9-21 ###???
	hr = pMetadata->SaveAsXML(&xml);
	if (FAILED(hr))
	{
		SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::Failed to SaveAsXML with error [x%08x]"), hr);
	}
	else
	{
		SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::Save xml start~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
		SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("<---%s--->"), (LPCTSTR)(xml));
		SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::Save xml end~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
	}


#endif

	return SUCCEEDED(hr);
}

// This function is called as a result of the requestor calling PrepareForBackup
// this indicates to the writer that a backup sequence is being initiated
bool STDMETHODCALLTYPE SmlSampleWriter::OnPrepareBackup(_In_ IVssWriterComponents* components)
{
	SmartLib::ScopeExec se
	{
		[]()
		{
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::start"));
		},
		[]()
		{
				SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::end"));
		}
	};

#if TRUE
	HRESULT hr = Dump(components);
#else


	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	VSS_ID idInstance{ 0 };
	VSS_ID idWriter{ 0 };
	hr = components->GetWriterInfo(&idInstance, &idWriter);
	if (FAILED(hr))
	{
		SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::Failed to GetWriterInfo with error [x%08x]"), hr);
		LEAVE_BLOCK(0);
	}
	else
	{
		CString stridInstance;
		CString stridWriter;
		SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("idInstance=[%s], idWriter=[%s]"), SmartLib::StringHelper::ToString(idInstance, stridInstance).GetString(), SmartLib::StringHelper::ToString(idWriter, stridWriter).GetString());
	}


	UINT compCount = 0;
	hr = components->GetComponentCount(&compCount);
	for (int ii = 0; ii < compCount; ++ ii)
	{
		CComPtr<IVssComponent> comp;
		hr = components->GetComponent(ii, &comp);
		if (FAILED(hr))
		{
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::failed to GetComponent [%d] with error [x%08x]"), ii, hr);
		}
		else
		{
			CComBSTR logicalPath;
			comp->GetLogicalPath(&logicalPath);
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::logicalPath=[%s]"), (LPCTSTR)(logicalPath));

			VSS_COMPONENT_TYPE pc = VSS_CT_UNDEFINED;
			comp->GetComponentType(&pc);
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::ComponentType=[%s]"), SmartLib::StringHelper::ToString(pc));


			CComBSTR componentName;
			comp->GetComponentName(&componentName);
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::componentName=[%s]"), (LPCTSTR)(componentName));

			bool backupSucceeded = false;
			comp->GetBackupSucceeded(&backupSucceeded);
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::backupSucceeded=[%d]"), backupSucceeded);

			CComBSTR backupMetadata;
			comp->GetBackupMetadata(&backupMetadata);
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::backupMetadata=[<---%s--->]"), (LPCTSTR)(backupMetadata));

			CComBSTR backupStamp;
			comp->GetBackupStamp(&backupStamp);
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::backupStamp=[%s]"), (LPCTSTR)(backupStamp));

			CComBSTR previousBackupStamp;
			comp->GetPreviousBackupStamp(&previousBackupStamp);
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::previousBackupStamp=[%s]"), (LPCTSTR)(previousBackupStamp));

			CComBSTR backupOptions;
			comp->GetBackupOptions(&backupOptions);
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::backupOptions=[%s]"), (LPCTSTR)(backupOptions));

			UINT differencedFilesCount = 0;
			comp->GetDifferencedFilesCount(&differencedFilesCount);
			for (int kk = 0; kk < differencedFilesCount; ++ kk)
			{
				CComBSTR Path;
				CComBSTR Filespec;
				BOOL Recursive;
				CComBSTR strLsnString;
				FILETIME LastModifyTime = { 0 };
				comp->GetDifferencedFile(
					kk,//IN UINT iDifferencedFile,
					&Path,//OUT BSTR *pbstrPath,
					&Filespec,//OUT BSTR *pbstrFilespec,
					&Recursive,//OUT BOOL *pbRecursive,
					&strLsnString,//OUT BSTR *pbstrLsnString,
					&LastModifyTime//,//OUT FILETIME *pftLastModifyTime
				);

				CString strFT;
				SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::GetDifferencedFile index=[%d], Path=[%s], Filespec=[%s], Recursive=[%d], LastModifyTime=[%s]"),
					kk, (LPCTSTR)(Path), (LPCTSTR)(Filespec), Recursive, (LPCTSTR)(strLsnString), SmartLib::StringHelper::ToString(LastModifyTime, strFT).GetString()
					);
			}
		}
	}
	
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
#endif

    return true;
}

// This function is called after a requestor calls DoSnapshotSet
// time-consuming actions related to Freeze can be performed here
bool STDMETHODCALLTYPE SmlSampleWriter::OnPrepareSnapshot()
{
	SmartLib::ScopeExec se
	{
		[]()
		{
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::start"));
		},
		[]()
		{
				SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::end"));
		}
	};

	int loopCount = 1;
	for (int ii = 0; ii < loopCount; ++ ii)
	{
		::Sleep(1000);
	}
	//SetWriterFailure(E_UNEXPECTED);
	//return false;
    return true;
}

// This function is called after a requestor calls DoSnapshotSet
// here the writer is expected to freeze its store
bool STDMETHODCALLTYPE SmlSampleWriter::OnFreeze()
{
	SmartLib::ScopeExec se
	{
		[]()
		{
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::start"));
		},
		[]()
		{
				SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::end"));
		}
	};
	return true;	
}	

// This function is called after a requestor calls DoSnapshotSet
// here the writer is expected to thaw its store
bool STDMETHODCALLTYPE SmlSampleWriter::OnThaw()
{
	SmartLib::ScopeExec se
	{
		[]()
		{
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::start"));
		},
		[]()
		{
				SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::end"));
		}
	};
	return true;	
}

// This function is called after a requestor calls DoSnapshotSet
bool STDMETHODCALLTYPE SmlSampleWriter::OnPostSnapshot(_In_ IVssWriterComponents* components)
{
	SmartLib::ScopeExec se
	{
		[]()
		{
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::start"));
		},
		[]()
		{
				SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::end"));
		}
	};

	VSS_BACKUP_TYPE bt = CVssWriter::GetBackupType();
	SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::GetBackupType=[%s]"), SmartLib::StringHelper::ToString(bt));

	LONG context = CVssWriter::GetContext();
	SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::GetContext=[%d, x%08x]"), context, context);

	VSS_APPLICATION_LEVEL  applevel = CVssWriter::GetCurrentLevel();
	SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::GetCurrentLevel=[%s]"), SmartLib::StringHelper::ToString(applevel));

	VSS_ID ssid = CVssWriter::GetCurrentSnapshotSetId();
	CString strssid;
	SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::GetCurrentSnapshotSetId=[%s]"), SmartLib::StringHelper::ToString(ssid, strssid).GetString());

	UINT volCount = CVssWriter::GetCurrentVolumeCount();
	SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::GetCurrentVolumeCount=[%d]"), volCount);

	LPCWSTR * strarray = CVssWriter::GetCurrentVolumeArray();
	for (int ii = 0; ii < volCount; ii++)
	{
		SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::volume[%d]=[%s]"), ii, strarray[ii]);

		LPCWSTR snapshotDevice = nullptr;
		CVssWriter::GetSnapshotDeviceName(strarray[ii], &snapshotDevice);
		SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::volume[%d]=[%s], SnapshotDeviceName[%d]=[%s]"), ii, strarray[ii], ii, snapshotDevice);
	}
	
	

	HRESULT hr = Dump(components);

    //return false;
	return true;
}

// This function is called to abort the writer's backup sequence.
// This should only be called between OnPrepareBackup and OnPostSnapshot
bool STDMETHODCALLTYPE SmlSampleWriter::OnAbort()
{
	SmartLib::ScopeExec se
	{
		[]()
		{
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::start"));
		},
		[]()
		{
				SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::end"));
		}
	};


    return true;
}

// This function is called as a result of the requestor calling BackupComplete
bool STDMETHODCALLTYPE SmlSampleWriter::OnBackupComplete(_In_ IVssWriterComponents* components)
{
	SmartLib::ScopeExec se
	{
		[]()
		{
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::start"));
		},
		[]()
		{
				SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::end"));
		}
	};

	HRESULT hr = Dump(components);

	return true;	
}	

// This function is called at the end of the backup process.  This may happen as a result
// of the requestor shutting down, or it may happen as a result of abnormal termination 
// of the requestor.
bool STDMETHODCALLTYPE SmlSampleWriter::OnBackupShutdown(_In_ VSS_ID)
{
	SmartLib::ScopeExec se
	{
		[]()
		{
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::start"));
		},
		[]()
		{
				SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::end"));
		}
	};


	return true;
}

// This function is called as a result of the requestor calling PreRestore
// This will be called immediately before files are restored
bool STDMETHODCALLTYPE SmlSampleWriter::OnPreRestore(_In_ IVssWriterComponents *)
{
	SmartLib::ScopeExec se
	{
		[]()
		{
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::start"));
		},
		[]()
		{
				SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::end"));
		}
	};


    return true;
}

// This function is called as a result of the requestor calling PreRestore
// This will be called immediately after files are restored
bool STDMETHODCALLTYPE SmlSampleWriter::OnPostRestore(_In_ IVssWriterComponents*)
{

	SmartLib::ScopeExec se
	{



		[]()
		{
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::start"));
		},
		[]()
		{
				SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::end"));
		}
	};


    return true;
}



HRESULT SmlSampleWriter::Dump(_In_ IVssWriterComponents *components)
{
#if TRUE
	return S_OK;
#else

	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	VSS_ID idInstance{ 0 };
	VSS_ID idWriter{ 0 };
	hr = components->GetWriterInfo(&idInstance, &idWriter);
	if (FAILED(hr))
	{
		SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::Failed to GetWriterInfo with error [x%08x]"), hr);
		LEAVE_BLOCK(0);
	}
	else
	{
		CString stridInstance;
		CString stridWriter;
		SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("idInstance=[%s], idWriter=[%s]"), SmartLib::StringHelper::ToString(idInstance, stridInstance).GetString(), SmartLib::StringHelper::ToString(idWriter, stridWriter).GetString());
	}


	UINT compCount = 0;
	hr = components->GetComponentCount(&compCount);
	for (int ii = 0; ii < compCount; ++ii)
	{
		CComPtr<IVssComponent> comp;
		hr = components->GetComponent(ii, &comp);
		if (FAILED(hr))
		{
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::failed to GetComponent [%d] with error [x%08x]"), ii, hr);
		}
		else
		{
			CComBSTR logicalPath;
			comp->GetLogicalPath(&logicalPath);
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::logicalPath=[%s]"), (LPCTSTR)(logicalPath));

			VSS_COMPONENT_TYPE pc = VSS_CT_UNDEFINED;
			comp->GetComponentType(&pc);
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::ComponentType=[%s]"), SmartLib::StringHelper::ToString(pc));


			CComBSTR componentName;
			comp->GetComponentName(&componentName);
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::componentName=[%s]"), (LPCTSTR)(componentName));

			bool backupSucceeded = false;
			comp->GetBackupSucceeded(&backupSucceeded);
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::backupSucceeded=[%d]"), backupSucceeded);

			CComBSTR backupMetadata;
			comp->GetBackupMetadata(&backupMetadata);
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::backupMetadata=[<---%s--->]"), (LPCTSTR)(backupMetadata));

			CComBSTR backupStamp;
			comp->GetBackupStamp(&backupStamp);
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::backupStamp=[%s]"), (LPCTSTR)(backupStamp));

			CComBSTR previousBackupStamp;
			comp->GetPreviousBackupStamp(&previousBackupStamp);
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::previousBackupStamp=[%s]"), (LPCTSTR)(previousBackupStamp));

			CComBSTR backupOptions;
			comp->GetBackupOptions(&backupOptions);
			SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::backupOptions=[%s]"), (LPCTSTR)(backupOptions));

			UINT differencedFilesCount = 0;
			comp->GetDifferencedFilesCount(&differencedFilesCount);
			for (int kk = 0; kk < differencedFilesCount; ++kk)
			{
				CComBSTR Path;
				CComBSTR Filespec;
				BOOL Recursive;
				CComBSTR strLsnString;
				FILETIME LastModifyTime = { 0 };
				comp->GetDifferencedFile(
					kk,//IN UINT iDifferencedFile,
					&Path,//OUT BSTR *pbstrPath,
					&Filespec,//OUT BSTR *pbstrFilespec,
					&Recursive,//OUT BOOL *pbRecursive,
					&strLsnString,//OUT BSTR *pbstrLsnString,
					&LastModifyTime//,//OUT FILETIME *pftLastModifyTime
				);

				CString strFT;
				SML_LOG_LINE(TEXT(__FUNCTION__) TEXT("::GetDifferencedFile index=[%d], Path=[%s], Filespec=[%s], Recursive=[%d], LastModifyTime=[%s]"),
					kk, (LPCTSTR)(Path), (LPCTSTR)(Filespec), Recursive, (LPCTSTR)(strLsnString), SmartLib::StringHelper::ToString(LastModifyTime, strFT).GetString()
				);
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);


	return hr;
#endif
}
