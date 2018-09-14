#pragma once

#include <string>
#include <map>
#include <list>
#include <vector>
#include <set>

#include <boost/property_tree/ptree.hpp>
#include "bundlable.h"
#include "typedefine.h"

typedef Bundlable* (*GETCOLLECTIONCALLBACK) ();

class Bundle{
private:
	static const std::string CLASS_NAME;
	static HashMap<std::string, std::string> aliases;
	static std::map<std::string, CreateBundlable> allBundlables;

	bool isnull;
	boost::property_tree::ptree data;

	Bundle(const boost::property_tree::ptree& d);
	
	static Bundlable* getBundlable(const std::string& className);

	Bundlable* get();
public:
	static void RegisterBundlable(const std::string& clsName, CreateBundlable cb);

	Bundle() :isnull(true){}
	std::string toString();
	bool isNull();
	void fields(std::list<std::string>& ret);
	bool contains(const std::string& key);
	bool getBoolean(const std::string& key);
	int getInt(const std::string& key);
	float getFloat(const std::string& key);
	std::string getString(const std::string& key);
	Bundle* getBundle(const std::string& key);
	Bundlable* get(const std::string& key);
	bool getIntArray(const std::string& key, std::vector<int>& ret);
	bool getIntArray(const std::string& key, std::list<int>& ret);
	bool getBooleanArray(const std::string& key, std::list<bool>& ret);
	bool getStringArray(const std::string& key, std::list<std::string>& ret);

	void getCollection(const std::string& key, std::vector<Bundlable*>&ret, GETCOLLECTIONCALLBACK cb);

	void put(const std::string& key, bool value);
	void put(const std::string& key, int value);
	void put(const std::string& key, float value);
	void put(const std::string& key, const std::string& value);
	void put(const std::string& key, Bundle* value);
	void put(const std::string& key, Bundlable* object);

	void put(const std::string& key, const std::vector<int>& arr);
	void put(const std::string& key, const std::vector<bool>& arr);
	void put(const std::string& key, const std::vector<std::string>& arr);
	void put(const std::string& key, const std::vector<Bundlable*>& arr);

	static Bundle* read(std::stringstream& stream);
	static bool write(Bundle* bundle, std::stringstream& stream);
};