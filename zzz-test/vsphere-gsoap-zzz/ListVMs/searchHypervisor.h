#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include "soapVimBindingProxy.h"
#include "vim_wrapper.h"
//using namespace vim_wrapper_ns;
using namespace std;


namespace vim_wrapper_ns
{

void search_folder(VimManager & vimMgrInstance, vector<Datacenter> & vecDc, const char * dcName, const ManagedEntity& me);
void search_datacenter(VimManager & vimMgrInstance, vector<Datacenter> & vecDc, const char * dcName, const ManagedEntity& me);

void search_datacenter_from_root_folder(VimManager & vimMgrInstance, vector<Datacenter> & vecDc, const char * dcName, gnvim1__ServiceContent & sc);
void search_vm_by_id(VimManager & vimMgrInstance, vector<VirtualMachine> & vecVM, Datacenter & datacenter, char* uuid, gnvim1__ServiceContent & sc);
void search_vm_by_moid(VimManager & vimMgrInstance, vector<VirtualMachine> & vecVM, Datacenter & datacenter, char* moid, gnvim1__ServiceContent & sc);
void search_vm_by_path(VimManager & vimMgrInstance, vector<VirtualMachine> & vecVM, Datacenter & datacenter, char* path, gnvim1__ServiceContent & sc);


} //end namespace vim_wrapper_ns
