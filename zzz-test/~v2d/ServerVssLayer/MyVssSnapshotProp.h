#pragma once

#include "stdafx.h"
#include "VssCommon.h"

struct MY_VSS_SNAPSHOT_PROP
{
	VSS_ID m_SnapshotId;
	VSS_ID m_SnapshotSetId;
	LONG m_lSnapshotsCount;
	CString m_pwszSnapshotDeviceObject;
	CString m_pwszOriginalVolumeName;
	CString m_pwszOriginatingMachine;
	CString m_pwszServiceMachine;
	CString m_pwszExposedName;
	CString m_pwszExposedPath;
	VSS_ID m_ProviderId;
	LONG m_lSnapshotAttributes;
	VSS_TIMESTAMP m_tsCreationTimestamp;
	VSS_SNAPSHOT_STATE m_eStatus;

	VOID Copy(VSS_SNAPSHOT_PROP & VssSnapshotProp);
	INT WriteToStorage(IStorage * pIStorage);
	INT ReadFromStorage(IStorage * pIStorage);

	static INT ReadFromVectorRootStorage(IStorage * pIStorage, vector<MY_VSS_SNAPSHOT_PROP> & MyVssSnapshotProps);
	static CONST MY_VSS_SNAPSHOT_PROP* FindByVulumeGuid(LPCTSTR pszVolGuid, CONST vector<MY_VSS_SNAPSHOT_PROP> & MyVssSnapshotProps);
};