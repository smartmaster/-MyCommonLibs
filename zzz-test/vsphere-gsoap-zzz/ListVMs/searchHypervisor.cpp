#include "searchHypervisor.h"

namespace vim_wrapper_ns
{




void search_datacenter(VimManager & vimMgrInstance, vector<Datacenter> & vecDc, const char * dcName, const ManagedEntity& me)
{
	Datacenter dc(vimMgrInstance, me);
	std::string strCurDcName = dc.get_name();
	cout << "\tDataCenter: " << strCurDcName << endl;

	if (0 == _stricmp(dcName, strCurDcName.c_str()))
	{
		vecDc.push_back(dc);
	}

	std::vector<ManagedEntity> mes = dc.get_hostFolder().get_childEntity();
	for (size_t i = 0; i < mes.size(); ++i)
	{
		cout << i << ". " << mes[i].get_name() << ":" << endl;
		if (mes[i].is_type_of(Folder::type))
		{
			search_folder(vimMgrInstance, vecDc, dcName, mes[i]);
		}
	}
}

void search_folder(VimManager & vimMgrInstance, vector<Datacenter> & vecDc, const char * dcName, const ManagedEntity& me)
{
	Folder fd(vimMgrInstance, me);
	std::vector<ManagedEntity> mes = fd.get_childEntity();
	for (size_t i = 0; i < mes.size(); ++i)
	{
		cout << i << ". " << mes[i].get_name() << ":" << endl;
		if (mes[i].is_type_of(Datacenter::type))
		{
			search_datacenter(vimMgrInstance, vecDc, dcName, mes[i]);
		}
		else if (mes[i].is_type_of(Folder::type))
		{
			search_folder(vimMgrInstance, vecDc, dcName, mes[i]);
		}

	}
}

void search_datacenter_from_root_folder(VimManager & vimMgrInstance, vector<Datacenter> & vecDc, const char * dcName, gnvim1__ServiceContent & sc)
{
	Folder rf(vimMgrInstance, sc.rootFolder);
	std::vector<ManagedEntity> mes = rf.get_childEntity();
	for (size_t i = 0; i < mes.size(); ++i)
	{
		cout << i << ". " << mes[i].get_name() << ":" << endl;
		if (mes[i].is_type_of(Datacenter::type))
		{
			search_datacenter(vimMgrInstance, vecDc, dcName, mes[i]);
		}

		else if (mes[i].is_type_of(Folder::type))
		{
			search_folder(vimMgrInstance, vecDc, dcName, mes[i]);
		}
	}
}

void search_vm_by_id(VimManager & vimMgrInstance, vector<VirtualMachine> & vecVM, Datacenter & datacenter, char* uuid, gnvim1__ServiceContent & sc)
{
	SearchIndex si(vimMgrInstance, sc.searchIndex);
	bool instanceUuid = true;
	std::vector<ManagedEntity> vecME = si.FindAllByUuid(datacenter, uuid, true, &instanceUuid);
	for (size_t ii = 0; ii < vecME.size(); ++ii)
	{
		ManagedEntity & me = vecME[ii];
		if (me.is_type_of(VirtualMachine::type))
		{
			VirtualMachine vm(vimMgrInstance, me);
			vecVM.push_back(vm);
		}
	}
}

void search_vm_by_path(VimManager & vimMgrInstance, vector<VirtualMachine> & vecVM, Datacenter & datacenter, char* path, gnvim1__ServiceContent & sc)
{
	SearchIndex si(vimMgrInstance, sc.searchIndex);
	VirtualMachine vm = si.FindByDatastorePath(datacenter, path);
	vecVM.push_back(vm);
}

void search_vm_by_moid(VimManager & vimMgrInstance, vector<VirtualMachine> & vecVM, Datacenter & datacenter, char* moid, gnvim1__ServiceContent & sc)
{
	SearchIndex si(vimMgrInstance, sc.searchIndex);
	bool instanceUuid = false;
	std::vector<ManagedEntity> vecME = si.FindAllByUuid(datacenter, moid, true, &instanceUuid);
	for (size_t ii = 0; ii < vecME.size(); ++ii)
	{
		ManagedEntity & me = vecME[ii];
		if (me.is_type_of(VirtualMachine::type))
		{
			VirtualMachine vm(vimMgrInstance, me);
			vecVM.push_back(vm);
		}
	}
}


} //end namespace vim_wrapper_ns