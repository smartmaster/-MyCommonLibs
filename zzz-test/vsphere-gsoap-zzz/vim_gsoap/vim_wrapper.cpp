////////////////////////////////////////////////////////////////////////
// Copyright (c) 2011, Bingfeng Zhao . All rights reserved.
// remove global object 2014-09-19 by smart.master...live.com
//

#include <sstream>
#include <cassert>
#include "soapVimBindingProxy.h"
// #include "VimBinding.nsmap"
#include "vim_wrapper.h"

namespace vim_wrapper_ns
{
binding_proxy::binding_proxy(const char* ip, bool use_ssl) : _binding_proxy(new VimBindingProxy)
{
	std::stringstream ss;
	ss << (use_ssl? std::string("https") : std::string("http"))
		<< "://" << ip << "/sdk" << std::ends;
	_url = ss.str();
	_binding_proxy->soap_endpoint = _url.c_str();
	soap_set_mode(_binding_proxy, SOAP_C_UTFSTRING);
	_binding_proxy->mode |= SOAP_C_UTFSTRING;
	if (use_ssl)
	{
		soap_ssl_init();
		if (soap_ssl_client_context(_binding_proxy, SOAP_SSL_NO_AUTHENTICATION,
			NULL, NULL, NULL, NULL, NULL))
		{
			std::ostringstream oss;
			soap_stream_fault(_binding_proxy, oss);
			throw oss.str().c_str();
		}
	}
}

binding_proxy::~binding_proxy()
{
	delete _binding_proxy;
}

mor_handle::mor_handle(gnvim1__ManagedObjectReference* mor) : _mor(mor) {}
mor_handle::~mor_handle() {}

template<typename REQ, typename RSP, typename OBJ, typename PROXY>
bool proxy_call_defs_ex(VimManager & vimMgrInstance, REQ& req, RSP& rsp, OBJ& obj, int (PROXY::*pfm)(REQ*, RSP&))
{
	VimBindingProxy* p = vimMgrInstance.proxy()->operator VimBindingProxy*();
	if ((p->*pfm)(&req, rsp) == SOAP_OK)
	{
		obj.set_last_error();
		return true;
	}
	else
	{
		const char * temp = p->soap_fault_detail();
		if (NULL == temp || 0 == temp[0])
		{
			temp = p->soap_fault_string();
		}
		obj.set_last_error(temp);
		return false;
	}
}


// The entry of ServiceInstance singleton
//VimManager& vimMgrInstance(binding_proxy* proxy)
//{
//	static VimManager v;
//	if (proxy)
//		v._proxy = proxy;
//	return v;
//}
//
//PropertyCollector& get_collector()
//{
//	ServiceInstance& si = vimMgrInstance().get_service_instance();
//	vw1__ServiceContent sc = si.RetrieveServiceContent();
//	static PropertyCollector pc(vimMgrInstance(), sc.propertyCollector);
//	return pc;
//}

PropertyCollector get_collector_ex(VimManager & vimMgrInstance)
{
	ServiceInstance& si = vimMgrInstance.get_service_instance();
	gnvim1__ServiceContent sc = si.RetrieveServiceContent();
	PropertyCollector pc(vimMgrInstance, sc.propertyCollector);
	return pc;
}

std::vector<gnvim1__ObjectContent> get_property_ex(const char* prop, VimManager & vimMgrInstance, const mor_handle* mo)
{
	// ObjectSpec specifies the starting object
	gnvim1__ObjectSpec os;
	os.obj = *mo;
	gnvim1__ObjectSpec* oss[] = {&os};

	// PropertySpec specifies what properties to retrieve and from type of Managed Object
	char* pps[1]; pps[0] = const_cast<char*>(prop);
	gnvim1__PropertySpec ps;
	ps.type = os.obj->type;
	ps.__sizepathSet = 1;
	ps.pathSet = pps;
	gnvim1__PropertySpec* pss[] = {&ps};

	// PropertyFilterSpec is used to hold the ObjectSpec and PropertySpec for the call_defs
	gnvim1__PropertyFilterSpec pfs;
	pfs.__sizeobjectSet = 1;
	pfs.objectSet = oss;
	pfs.__sizepropSet = 1;
	pfs.propSet = pss;
	gnvim1__PropertyFilterSpec* pfSpec[] = {&pfs};

	vim_wrapper_ns::PropertyCollector pc = get_collector_ex(vimMgrInstance);
	return pc.RetrieveProperties(1, pfSpec);
}


} // namespace vim_wrapper_ns

