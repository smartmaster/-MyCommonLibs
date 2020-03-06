#include "stdafx.h"
#include "..\CommonFunctions\VDSVolumeDiskHelper.h"
#include "..\CommonFunctions\SingleVdsDiskInfo.h"
#include "..\CommonFunctions\SingleVdsVolumeInfo.h"
#include "..\CommonFunctions\AllVdsDiskVolumeInfo.h"

VOID AllVdsDiskVolumeInfoTest()
{
	CAllVdsDiskVolumeInfo vdsDVI;
	vdsDVI.Retrieve();

	//******************************************
	CComPtr<ILockBytes> spILockBytes;
	::CreateILockBytesOnHGlobal(
		NULL,//__in          HGLOBAL hGlobal,
		TRUE,//__in          BOOL fDeleteOnRelease,
		&spILockBytes//__out         ILockBytes** ppLkbyt
		);

	CComPtr<IStorage> spIStorage;
	::StgCreateDocfileOnILockBytes(
		spILockBytes,//__in          ILockBytes* plkbyt,
		STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,//__in          DWORD grfMode,
		0,//__in          DWORD reserved,
		&spIStorage//__out         IStorage** ppstgOpen
		);

	vdsDVI.WriteVdsDiskToStorage(spIStorage);

	//******************************************
	CComPtr<ILockBytes> spILockBytesVol;
	::CreateILockBytesOnHGlobal(
		NULL,//__in          HGLOBAL hGlobal,
		TRUE,//__in          BOOL fDeleteOnRelease,
		&spILockBytesVol//__out         ILockBytes** ppLkbyt
		);

	CComPtr<IStorage> spIStorageVol;
	::StgCreateDocfileOnILockBytes(
		spILockBytesVol,//__in          ILockBytes* plkbyt,
		STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,//__in          DWORD grfMode,
		0,//__in          DWORD reserved,
		&spIStorageVol//__out         IStorage** ppstgOpen
		);

	vdsDVI.WriteVdsVolumeToStorage(spIStorageVol);

	//******************************************
	CAllVdsDiskVolumeInfo vdsDVI01;
	vdsDVI01.ReadVdsDiskFromStorage(spIStorage);
	vdsDVI01.ReadVdsVolumeFromStorage(spIStorageVol);

	return;
}

INT VDSVolumeDiskHelperTest()
{
	DSTART(999);

	CVDSService service;
	service.Load();

	vector<IVdsSwProvider*> swProviders;
	service.QuerySoftwareProvides(swProviders);
	for (size_t ii=0; ii<swProviders.size(); ++ii)
	{
		CVdsSwProvider swProv;
		swProv.Initialize(swProviders[ii], FALSE);

		vector<IVdsPack*> Packs;
		swProv.QueryPacks(Packs);
		for (size_t jj=0; jj<Packs.size(); ++jj)
		{
			CVdsPack pack;
			pack.Initialize(Packs[jj], FALSE);

			vector<IVdsDisk*> disks;
			pack.QueryDisks(disks);
			for (size_t kk=0; kk<disks.size(); ++kk)
			{
				CVdsDisk disk;
				disk.Initialize(disks[kk], FALSE);

				
				//******************************************
				CComPtr<ILockBytes> spILockBytes;
				::CreateILockBytesOnHGlobal(
					NULL,//__in          HGLOBAL hGlobal,
					TRUE,//__in          BOOL fDeleteOnRelease,
					&spILockBytes//__out         ILockBytes** ppLkbyt
					);

				CComPtr<IStorage> spIStorage;
				::StgCreateDocfileOnILockBytes(
					spILockBytes,//__in          ILockBytes* plkbyt,
					STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,//__in          DWORD grfMode,
					0,//__in          DWORD reserved,
					&spIStorage//__out         IStorage** ppstgOpen
					);

				//******************************************
				CSingleVdsDiskInfo sdi00;
				sdi00.m_MyVDSDiskProp = disk.m_MyVDSDiskProp;
				sdi00.m_VDSDiskExtents = disk.m_VDSDiskExtents;
				sdi00.WriteToStorage(spIStorage);

				CSingleVdsDiskInfo sdi01;
				sdi01.ReadFromStorage(spIStorage);
			}

			vector<IVdsVolume*> Volumes;
			pack.QueryVolumes(Volumes);
			for (size_t kk=0; kk<Volumes.size(); ++kk)
			{
				//******************************************
				CComPtr<ILockBytes> spILockBytes;
				::CreateILockBytesOnHGlobal(
					NULL,//__in          HGLOBAL hGlobal,
					TRUE,//__in          BOOL fDeleteOnRelease,
					&spILockBytes//__out         ILockBytes** ppLkbyt
					);

				CComPtr<IStorage> spIStorage;
				::StgCreateDocfileOnILockBytes(
					spILockBytes,//__in          ILockBytes* plkbyt,
					STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,//__in          DWORD grfMode,
					0,//__in          DWORD reserved,
					&spIStorage//__out         IStorage** ppstgOpen
					);

				CSingleVdsVolumeInfo svvi00;
				//******************************************

				CVdsVolume Volume;
				Volume.Initialize(Volumes[kk], FALSE);

				//******************************************
				svvi00.m_MyVDSVolumeProp = Volume.m_MyVDSVolumeProp;
				svvi00.m_strVolumeGUID = Volume.m_strVolumeGUID;
				//******************************************

				vector<IVdsVolumePlex*> vdsPlexes;
				Volume.QueryPlexes(vdsPlexes);
				for (size_t nn=0; nn<vdsPlexes.size(); ++nn)
				{
					CVdsVolumePlex VolumePlex;
					VolumePlex.Initialize(vdsPlexes[nn], FALSE);

					//******************************************
					CSingleVdsVolumePlex svvp;
					svvp.m_VDSVolumePlexProp = VolumePlex.m_VDSVolumePlexProp;
					svvp.m_VDSDiskExtents = VolumePlex.m_VDSDiskExtents;
					svvi00.m_VolumePlexes.push_back(svvp);
					//******************************************
				}

				//******************************************
				svvi00.WriteToStorage(spIStorage);
				CSingleVdsVolumeInfo svvi01;
				svvi01.ReadFromStorage(spIStorage);
				//******************************************
			}
		}
	}

	vector<IVdsHwProvider*> HwProviders;
	service.QueryHardwareProvides(HwProviders);
	for (size_t ii=0; ii<HwProviders.size(); ++ii)
	{
		CVdsHwProvider hwProv;
		hwProv.Initialize(HwProviders[ii], FALSE);
	}

	vector<IVdsVdProvider*> VdProviders;
	service.QueryVirtualDiskProvides(VdProviders);
	for (size_t ii=0; ii<VdProviders.size(); ++ii)
	{
		CVdsVdProvider vdProv;
		vdProv.Initialize(VdProviders[ii], FALSE);
	}

	return 0;
}