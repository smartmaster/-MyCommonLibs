////////////////////////////////////////////////////////////////////////
// Copyright (c) 2011, Bingfeng Zhao . All rights reserved.
// remove global object 2014-09-19 by smart.master...live.com
//

#ifndef VIM_BROWSER_H
#define VIM_BROWSER_H

#include <iostream>
#include <cassert>
#include "vim_wrapper.h"

namespace vim_browser_ns
{
using namespace vim_wrapper_ns;
//
// == The simple browsing protocol ==
// Browse will send following message to receiver in order:
// 1. pair("Begin", class_name). indicate start the browsing
// 2. pair("Name", member_name), pair("Type", member_type) and
//    pair("Value", member_value)
//    if the value is an array, it's seperated by '|', started with '@'
// 3. optional pair("Base", base_class_name), report the base member to be browsed,
//    followed by a list of base members, just as item 2 format
// 4. pair("End", ""), indicate end the browsing
//
typedef void (*pf_update)(const std::string& key, const std::string& value, void* context);

// Interface for browser. All specific browsable types should derive from it
struct closure
{
	virtual ~closure() {}
	virtual closure* get_sub_closure(size_t index) const = 0;
	virtual void browse(pf_update update, void* context) const = 0;
	virtual const char* type() const = 0;
};

// browse handler for different types
//template<typename T> std::string handle_obj(T t);

template<typename T>
std::string handle_obj_ptr(const T* value)
{
	return value? handle_obj(*value) : "_unset_";
}

closure* get_closure_from_mor(gnvim1__ManagedObjectReference* mor);
} // namespace vim_browser_ns
#endif // VIM_BROWSER_H
