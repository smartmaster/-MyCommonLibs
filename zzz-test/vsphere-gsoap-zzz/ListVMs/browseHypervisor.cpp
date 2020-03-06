#include "browseHypervisor.h"
#include "VimHelper.h"

namespace vim_wrapper_ns
{

void list_host(VimManager & vimMgrInstance, const HostSystem& hs)
{
	vector<VirtualMachine> vms = hs.get_vm();
	for (size_t i = 0; i < vms.size(); ++i)
	{
		VirtualMachine vm(vimMgrInstance, vms[i]);
		//gnvim1__VirtualMachineConfigInfo vmci = vm.get_config();
		//cout << vm.get_name() << " (" << vm.get_runtime().powerState << ")\n";
		PrintVMInfo(vm);
	}
}
void list_computerresource(VimManager & vimMgrInstance, const ManagedEntity& me)
{
	ComputeResource cr(vimMgrInstance, me);
	vector<HostSystem> hss = cr.get_host();
	for (size_t i = 0; i < hss.size(); ++i)
		list_host(vimMgrInstance, hss[i]);
}
void list_datastore(VimManager & vimMgrInstance, const ManagedEntity& me)
{
	Datastore ds(vimMgrInstance, me);
	gnvim1__DatastoreInfo dsi = ds.get_info();
	return;
}
void list_datacenter(VimManager & vimMgrInstance, const ManagedEntity& me)
{
	Datacenter dc(vimMgrInstance, me);
	cout << "\tDataCenter: " << dc.get_name() << endl;
	std::vector<ManagedEntity> mes = dc.get_hostFolder().get_childEntity();
	for (size_t i = 0; i < mes.size(); ++i)
	{
		cout << i << ". " << mes[i].get_name() << ":" << endl;
		if (mes[i].is_type_of(Folder::type))
		{
			list_folder(vimMgrInstance, mes[i]);
		}
		else if (mes[i].is_type_of(ComputeResource::type))
		{
			list_computerresource(vimMgrInstance, mes[i]);
		}
		else if (mes[i].is_type_of(Datastore::type))
		{
			list_datastore(vimMgrInstance, mes[i]);
		}
			
	}
}
void list_folder(VimManager & vimMgrInstance, const ManagedEntity& me)
{
	Folder fd(vimMgrInstance, me);
	std::vector<ManagedEntity> mes = fd.get_childEntity();
	for (size_t i = 0; i < mes.size(); ++i)
	{
		cout << i << ". " << mes[i].get_name() << ":" << endl;
		if (mes[i].is_type_of(Datacenter::type))
		{
			list_datacenter(vimMgrInstance, mes[i]);
		}
		else if (mes[i].is_type_of(Folder::type))
		{
			list_folder(vimMgrInstance, mes[i]);
		}
		else if (mes[i].is_type_of(ComputeResource::type))
		{
			list_computerresource(vimMgrInstance, mes[i]);
		}
			
	}
}

void list_all(VimManager & vimMgrInstance, gnvim1__ServiceContent & sc)
{
	Folder rf(vimMgrInstance, sc.rootFolder);
	std::vector<ManagedEntity> mes = rf.get_childEntity();
	for (size_t i = 0; i < mes.size(); ++i)
	{
		cout << i << ". " << mes[i].get_name() << ":" << endl;
		if (mes[i].is_type_of(Datacenter::type))
			list_datacenter(vimMgrInstance, mes[i]);
		else if (mes[i].is_type_of(Folder::type))
			list_folder(vimMgrInstance, mes[i]);
	}
}

} //end namespace vim_wrapper_ns