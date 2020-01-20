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
	void list_folder(VimManager & vimMgrInstance, const ManagedEntity& me);
	void list_host(VimManager & vimMgrInstance, const HostSystem& hs);
	void list_computerresource(VimManager & vimMgrInstance, const ManagedEntity& me);
	void list_datastore(VimManager & vimMgrInstance, const ManagedEntity& me);
	void list_datacenter(VimManager & vimMgrInstance, const ManagedEntity& me);
	void list_folder(VimManager & vimMgrInstance, const ManagedEntity& me);


	void list_all(VimManager & vimMgrInstance, gnvim1__ServiceContent & sc);
}

