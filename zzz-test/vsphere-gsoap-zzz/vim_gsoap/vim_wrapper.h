////////////////////////////////////////////////////////////////////////
// Copyright (c) 2011, Bingfeng Zhao . All rights reserved.
// remove global object 2014-09-19 by smart.master...live.com
//

#ifndef VIM_WRAPPER_H
#define VIM_WRAPPER_H

#include <string>
#include <vector>
#include "soapStub.h"				// gSOAP generated
#include "soapVimBindingProxy.h"	// gSOAP generated

namespace vim_wrapper_ns
{
// forward declarations
typedef xsd__anyType anyType;

// Generic helper to retrieve property of managed object
class mor_handle;
class VimManager;
std::vector<gnvim1__ObjectContent> get_property_ex(const char* prop, VimManager & vimMgrInstance, const mor_handle* mo);

// Class definitions
class binding_proxy
{
	binding_proxy(const binding_proxy&);
	binding_proxy& operator=(const binding_proxy&);

	VimBindingProxy* _binding_proxy;
	std::string _url;
public:
	binding_proxy(const char* ip, bool use_ssl);
	~binding_proxy();
	operator VimBindingProxy*() { return _binding_proxy; }
	VimBindingProxy* SoapProxy() { return _binding_proxy; }
};

class mor_handle
{
protected:
	std::string	 _last_error;
	gnvim1__ManagedObjectReference* _mor;

public:
	explicit mor_handle(gnvim1__ManagedObjectReference* mor);
	virtual ~mor_handle();

	void set_last_error(const char* msg = 0) { _last_error = msg? msg : ""; }
	std::string get_last_error() const { return _last_error; }

	bool is_type_of(const char* type) const { return _strcmpi(_mor->type, type) == 0; }
	const char* present() const { return _mor->__item; }
	operator gnvim1__ManagedObjectReference*() const { return _mor; }
	operator gnvim1__ManagedObjectReference*() { return _mor; }
	operator bool() const { return _mor != 0; }
};



class VimManager
{
	binding_proxy* _proxy;
	gnvim1__ManagedObjectReference _mor;
	ServiceInstance* _si;
public:
	VimManager(binding_proxy* proxy = 0) : _proxy(proxy), _si(NULL) {}
	virtual ~VimManager()
	{
		if (_si)
		{
			delete _si;
			_si = NULL;
		}
	}
	//friend VimManager& vimMgrInstance(binding_proxy* proxy);

public:
	ServiceInstance& get_service_instance()
	{
		static char type[] =  "ServiceInstance";
		static char item[] =  "ServiceInstance";
		if (!_si)
		{
			_mor.type = type;
			_mor.__item = item;
			_si = new ServiceInstance(*this, &_mor);
		}
		return *_si;
	}
	binding_proxy* proxy() { return _proxy; }
};

//VimManager& vimMgrInstance(binding_proxy* proxy = 0);
//PropertyCollector& get_collector();
} // namespace vim_wrapper_ns
#endif // VIM_WRAPPER_H

