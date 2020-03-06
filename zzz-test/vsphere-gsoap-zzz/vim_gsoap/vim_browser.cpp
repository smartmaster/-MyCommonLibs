////////////////////////////////////////////////////////////////////////
// Copyright (c) 2011, Bingfeng Zhao . All rights reserved.
// remove global object 2014-09-19 by smart.master...live.com
//

#include <vector>
#include <sstream>
#include "vim_browser.h"

namespace vim_browser_ns
{
// convert to string, instance
template <typename T> std::string basic2str(const T& t)
{
	std::stringstream ss;
	ss << std::boolalpha << t;
	return ss.str();
}

// convert to string, xsd__base64Binary instance
template <> std::string basic2str(const xsd__base64Binary& t)
{
	std::stringstream ss;
	ss.write((const char*)t.__ptr, t.__size);
	ss << char(0);
	return ss.str();
}

// convert to string, pointer
template <typename T> std::string basic2str(T* t)
{
	if (!t)
		return "_unset_";
	std::stringstream ss;
	ss << *t;
	return ss.str();
}

// specialize for xsd_anyType
template <> std::string basic2str(xsd__anyType* t)
{
	if (!t)
		return "_unset_";
	std::stringstream ss;
	ss << std::boolalpha << t->__item;
	return ss.str();
}

// specialize for char*
template <> std::string basic2str(char* t)
{
	if (!t)
		return "_unset_";
	std::stringstream ss;
	ss << t;
	return ss.str();
}

// specialize for bool*
template <> std::string basic2str(bool* t)
{
	if (!t)
		return "_unset_";
	std::stringstream ss;
	ss << std::boolalpha <<  *t;
	return ss.str();
}

// convert to string, C-style array
template <typename T> std::string a2s_basic(const T* t, int size)
{
	if (!t)
		return "_unset_";
	std::stringstream ss;
	ss << '@';
	for (int i = 0; i < size; ++i)
	{
		if (i != 0)
			ss << '|';
		ss << std::boolalpha << t[i];
	}
	return ss.str();
}

// convert to string, STL-style array
template <typename T> std::string a2s_basic(const std::vector<T>& t)
{
	std::stringstream ss;
	ss << '@';
	for (size_t i = 0; i < t.size(); ++i)
	{
		if (i != 0)
			ss << '|';
		ss << std::boolalpha << t[i];
	}
	return ss.str();
}

// convert to string, C-style object array
template <typename T> std::string a2s_object(const T* t, int size)
{
	if (!t)
		return "_unset_";
	std::stringstream ss;
	ss << '@';
	for (int i = 0; i < size; ++i)
	{
		if (i != 0)
			ss << '|';
		ss << std::boolalpha << handle_obj(*t[i]);
	}
	return ss.str();
}

// convert to string, STL-style object array
template <typename T> std::string a2s_object(const std::vector<T>& t)
{
	std::stringstream ss;
	ss << '@';
	for (size_t i = 0; i < t.size(); ++i)
	{
		if (i != 0)
			ss << '|';
		ss << std::boolalpha << handle_obj(t[i]);
	}
	return ss.str();
}

// version check routine
extern bool is_higher_version_than(const char* ver);

void update_member(pf_update update, const std::string& name, 
	const std::string& type, const std::string& value, void* context)
{
	update("Name", name, context);
	update("Type", type, context);
	update("Value", value, context);
}

void update_member_with_version(pf_update update, const std::string& name,
	const std::string& type, const std::string& value, void* context, const char* ver)
{
	update("Name", name, context);
	update("Type", type, context);
	if (is_higher_version_than(ver))
		update("Value", value, context);
	else
		update("Value", "_unsupported_", context);
}

closure* get_closure_from_mor(gnvim1__ManagedObjectReference* mor)
{
	if (0)
		;
	else
		return 0;
}
} // namespace vim_browser_ns
