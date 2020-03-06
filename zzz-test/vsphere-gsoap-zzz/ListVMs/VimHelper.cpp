#include "VimHelper.h"


namespace vim_wrapper_ns
{

VOID VimWaitForTask(Task & task, gnvim1__TaskInfo & ti, ULONG IntervalMillSec, ULONG LoopCount)
{
	ULONG ii;
	for (ii = 0; ii < LoopCount; ++ii)
	{
		Sleep(IntervalMillSec);

		ti = task.get_info();

		if (gnvim1__TaskInfoState__success == ti.state || gnvim1__TaskInfoState__error == ti.state)
		{
			break;
		}
	}
}

VOID VimNoOp()
{
	return;
}

#define ptr_to_ptr(x) (x? x : "null")
#define ptr_to_value(x) (x? *x : -1)

void Print_gnvim1__VirtualMachineConfigInfo(const gnvim1__VirtualMachineConfigInfo & vmci)
{
	cout << "name\t" << ptr_to_ptr((vmci).name) << endl;
	cout << "instanceUuid\t" << ptr_to_ptr((vmci).instanceUuid) << endl;
	cout << "uuid\t" << ptr_to_ptr((vmci).uuid) << endl;
	cout << "guestId\t" << ptr_to_ptr((vmci).guestId) << endl;
	cout << "guestFullName\t" << ptr_to_ptr((vmci).guestFullName) << endl;
	if ((vmci).files)
	{
		cout << "vmPathName\t" << ptr_to_ptr(((vmci).files)->vmPathName) << endl;
	}



	for (int ii = 0; (vmci).hardware && ii < ((vmci).hardware)->__sizedevice; ++ii)
	{
		gnvim1__VirtualDisk * disk = dynamic_cast<gnvim1__VirtualDisk *>(vmci.hardware->device[ii]);
		if (disk)
		{
			cout << endl;
			cout << "fileName\t" << ptr_to_ptr((*((gnvim1__VirtualDeviceFileBackingInfo*)(&(*((gnvim1__VirtualDiskFlatVer2BackingInfo*)((vmci.hardware->device[ii])->backing)))))).fileName) << endl;
			cout << "diskMode\t" << (*((gnvim1__VirtualDiskFlatVer2BackingInfo*)((vmci.hardware->device[ii])->backing))).diskMode << endl;
			cout << "label\t" << ptr_to_ptr(((vmci.hardware->device[ii])->deviceInfo)->label) << endl;
			//if ((*((gnvim1__VirtualDisk*)(vmci.hardware->device[ii]))).capacityInBytes)
			{
				cout << "capacityInBytes\t" << ptr_to_value((*((gnvim1__VirtualDisk*)(vmci.hardware->device[ii]))).capacityInBytes) << endl;
			}
			cout << "capacityInKB\t" << (*((gnvim1__VirtualDisk*)(vmci.hardware->device[ii]))).capacityInKB << endl;

			//if ((*((gnvim1__VirtualDiskFlatVer2BackingInfo*)((vmci.hardware->device[ii])->backing))).thinProvisioned)
			{
				cout << "thinProvisioned\t" << ptr_to_value((*((gnvim1__VirtualDiskFlatVer2BackingInfo*)((vmci.hardware->device[ii])->backing))).thinProvisioned) << endl;
			}

			//if ((*((gnvim1__VirtualDiskFlatVer2BackingInfo*)((vmci.hardware->device[ii])->backing))).eagerlyScrub)
			{
				cout << "teagerlyScrub\t" << ptr_to_value((*((gnvim1__VirtualDiskFlatVer2BackingInfo*)((vmci.hardware->device[ii])->backing))).eagerlyScrub) << endl;
			}
			cout << endl;
		}
	}
}

void Print_gnvim1__GuestInfo(const gnvim1__GuestInfo & gi)
{
	cout << endl;
	cout << "toolsVersionStatus\t" << ptr_to_ptr((gi).toolsVersionStatus) << endl;
	cout << "toolsVersionStatus2\t" << ptr_to_ptr((gi).toolsVersionStatus2) << endl;
	cout << "toolsRunningStatus\t" << ptr_to_ptr((gi).toolsRunningStatus) << endl;
	cout << "toolsVersion\t" << ptr_to_ptr((gi).toolsVersion) << endl;
	cout << "guestId\t" << ptr_to_ptr((gi).guestId) << endl;
	cout << "hostName\t" << ptr_to_ptr((gi).hostName) << endl;
	cout << "ipAddress\t" << ptr_to_ptr((gi).ipAddress) << endl;
	cout << "guestState\t" << ptr_to_ptr((gi).guestState) << endl;
	cout << endl;
}

void PrintSnapshotInfo_rec(const VirtualMachineSnapshot & snapshot)
{
	//cout << endl;
	cout << "snapshot\t" << snapshot.present() << endl;

	
	gnvim1__VirtualMachineConfigInfo vmci = snapshot.get_config();
	Print_gnvim1__VirtualMachineConfigInfo(vmci);

	std::vector<VirtualMachineSnapshot> children = snapshot.get_childSnapshot();
	for (const auto & child : children)
	{
		cout << "Parent snapshot\t" << snapshot.present() << endl;
		PrintSnapshotInfo_rec(child); //recursively call
	}
	cout << endl;
}

void Print_VM_snapshotinfo(const VirtualMachine & vmById)
{
	cout << endl;
	std::vector<VirtualMachineSnapshot> snapshots = vmById.get_rootSnapshot();
	int ii = 0;
	for (const auto & snapshot : snapshots)
	{
		cout << "this is a root snapshot " << ii++ << endl;
		PrintSnapshotInfo_rec(snapshot); 
	}
	cout << endl;
}


void Print_gnvim1__VirtualMachineSnapshotTree_rec(VimManager & vimMgr, const gnvim1__VirtualMachineSnapshotTree & snaptree)
{
	VirtualMachineSnapshot vmsnapshot(vimMgr, snaptree.snapshot);
	cout << "snapshot\t" << vmsnapshot.present() << endl;
	cout << "name\t" << ptr_to_ptr(snaptree.name) << endl;
	cout << "description\t" << ptr_to_ptr(snaptree.description) << endl;
	cout << "id\t" << ptr_to_value(snaptree.id )<< endl;
	cout << "quiesced\t" << (snaptree.quiesced) << endl;
	cout << "backupManifest\t" << ptr_to_ptr(snaptree.backupManifest) << endl;
	cout << "replaySupported\t" << ptr_to_value(snaptree.replaySupported) << endl;
	

	gnvim1__VirtualMachineConfigInfo vmci = vmsnapshot.get_config();
	Print_gnvim1__VirtualMachineConfigInfo(vmci);

	for (int ii = 0; ii < snaptree.__sizechildSnapshotList; ++ ii)
	{
		cout << "Parent snapshot\t" << vmsnapshot.present() << endl;
		Print_gnvim1__VirtualMachineSnapshotTree_rec(vimMgr, snaptree.childSnapshotList[ii][0]);
	}

	cout << endl;
}

void Print_vm_snapshot_info_tree(const VirtualMachine & vmById)
{
	gnvim1__VirtualMachineSnapshotInfo vmsi = vmById.get_snapshot();

	if (vmsi.currentSnapshot)
	{
		cout << "currentSnapshot\t" << vmsi.currentSnapshot->__item << endl;
	}

	VimManager & vmmgr = const_cast<VirtualMachine&>(vmById).GetVimManagerInstance();
	for (int ii = 0; ii < vmsi.__sizerootSnapshotList; ++ ii)
	{
		Print_gnvim1__VirtualMachineSnapshotTree_rec(vmmgr, vmsi.rootSnapshotList[ii][0]);
	}

	cout << endl;
}

void PrintVMInfo(const VirtualMachine & vmById)
{
	cout << "#######################################################" << endl;
	cout << "moref\t" << ptr_to_ptr(vmById.present()) << endl;

	gnvim1__VirtualMachineConfigInfo vmci = vmById.get_config();
	Print_gnvim1__VirtualMachineConfigInfo(vmci);

	gnvim1__GuestInfo gi = vmById.get_guest();
	Print_gnvim1__GuestInfo(gi);

	if (1)
	{
		Print_vm_snapshot_info_tree(vmById);
	}
	else
	{
		Print_VM_snapshotinfo(vmById);
	}
	
	
	cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl << endl;
}

struct soap * soap_new_utf_helper()
{
	struct soap * soap_ptr = soap_new();
	soap_set_mode(soap_ptr, SOAP_C_UTFSTRING);
	soap_ptr->mode |= SOAP_C_UTFSTRING;
	return soap_ptr;
}

void soap_delete_helper(struct soap * soap)
{
	soap_destroy(soap);
	soap_end(soap);
	soap_done(soap);
	soap_free(soap);
}

std::string soap_error_string_helper(struct soap * soap)
{
	const char * temp = *soap_faultdetail(soap);
	if (NULL == temp || 0 == temp[0])
	{
		temp = *soap_faultstring(soap);
	}
	return temp ? temp : "";
}

void soap_reset_utf_helper(struct soap * soap)
{
	soap_destroy(soap);
	soap_end(soap);
	soap_done(soap);
	soap_initialize(soap);
	soap_set_mode(soap, SOAP_C_UTFSTRING);
	soap->mode |= SOAP_C_UTFSTRING;
}

} //namespace vim_wrapper_ns