#pragma once

#include <string>

#include <boost/property_tree/ptree.hpp>

// boost property tree
class BPT{
	static BPT* _inst;
	static BPT* inst();

	BPT();
	~BPT();

	boost::property_tree::ptree pt;
public:
	static std::string getText(const std::string& key);
};