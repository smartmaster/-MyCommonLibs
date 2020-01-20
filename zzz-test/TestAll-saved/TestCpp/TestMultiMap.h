#pragma once

#include <string>
#include <map>
#include "log.h"


#ifndef TestMultiMap_h_4F9EE881_4196_4B8E_8173_4E56EBB3F636
#define TestMultiMap_h_4F9EE881_4196_4B8E_8173_4E56EBB3F636

class TestTestMultiMap
{
public:
	static void Case0()
	{
		std::multimap<std::string, std::string> mm;
		//std::map<std::string, std::string> mm;

		mm.insert(std::pair<std::string, std::string>("1", "1"));
		mm.insert(std::pair<std::string, std::string>("1", "1"));
		mm.insert(std::pair<std::string, std::string>("1", "01"));
		mm.insert(std::pair<std::string, std::string>("1", "001"));

		mm.insert(std::pair<std::string, std::string>("2", "2"));
		mm.insert(std::pair<std::string, std::string>("2", "02"));
		mm.insert(std::pair<std::string, std::string>("2", "002"));

		mm.insert(std::pair<std::string, std::string>("3", "3"));
		mm.insert(std::pair<std::string, std::string>("3", "03"));
		mm.insert(std::pair<std::string, std::string>("3", "003"));

		auto p1 = mm.lower_bound(std::string("2"));
		auto p2 = mm.upper_bound(std::string("2"));
		for (auto p = p1; p != p2; p++)
		{
			LOG_LINE(TEXT("[%S, %S]"), p->first.c_str(), p->second.c_str());
		}

	}
};

#endif