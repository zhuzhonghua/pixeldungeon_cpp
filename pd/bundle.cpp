#include "bundle.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

const std::string Bundle::CLASS_NAME = "__className";

HashMap<std::string, std::string> Bundle::aliases;
std::map<std::string, CreateBundlable> Bundle::allBundlables;

Bundle* Bundle::read(std::stringstream& ss)
{
	Bundle* b = new Bundle();
	read_json(ss, b->data);
	b->isnull = false;

	return b;
}

bool Bundle::write(Bundle* bundle, std::stringstream& ss)
{
	write_json(ss, bundle->data);
	return true;
}

bool Bundle::getIntArray(const std::string& key, std::list<int>& ret)
{
	boost::property_tree::ptree image_array = data.get_child(key);
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, image_array)
	{
		ret.push_back(v.second.get_value<int>());
	}

	return true;
}

bool Bundle::getIntArray(const std::string& key, std::vector<int>& ret)
{
	boost::property_tree::ptree image_array = data.get_child(key);
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, image_array)
	{
		ret.push_back(v.second.get_value<int>());
	}
	return true;
}

bool Bundle::getBooleanArray(const std::string& key, std::list<bool>& ret)
{
	boost::property_tree::ptree image_array = data.get_child(key);
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, image_array)
	{
		ret.push_back(v.second.get_value<bool>());
	}

	return true;
}

bool Bundle::getStringArray(const std::string& key, std::list<std::string>& ret)
{
	boost::property_tree::ptree image_array = data.get_child(key);
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, image_array)
	{
		std::stringstream s;
		write_json(s, v.second);
		ret.push_back(s.str());
	}

	return true;
}

bool Bundle::isNull()
{
	return isnull;
}

bool Bundle::contains(const std::string& key)
{
	//return !data.isNull(key);
	return false;
}

bool Bundle::getBoolean(const std::string& key)
{
	return data.get<bool>(key);
}

float Bundle::getFloat(const std::string& key)
{
	return data.get<float>(key);
}

int Bundle::getInt(const std::string& key)
{
	return data.get<int>(key);
}

std::string Bundle::getString(const std::string& key)
{
	return data.get<std::string>(key);
}

Bundle* Bundle::getBundle(const std::string& key)
{
	return new Bundle(data.get_child(key));
}

Bundle::Bundle(const boost::property_tree::ptree& d)
{
	this->data = d;
}

Bundlable* Bundle::getBundlable(const std::string& className)
{
	if (allBundlables.size() <= 0)
	{
		//init();
	}
	return allBundlables[className]();
}

Bundlable* Bundle::get()
{
	String clName = getString(CLASS_NAME);
	if (aliases.containsKey(clName)) {
		clName = aliases.get(clName);
	}

	Bundlable* object = getBundlable(clName);
	object->restoreFromBundle(this);
	return object;
}

Bundlable* Bundle::get(const std::string& key)
{
	Bundle* b = getBundle(key);
	Bundlable* object = b->get();
	delete b;
	return object;
}

void Bundle::put(const std::string& key, bool value)
{
	data.put(key, value);
}

void Bundle::put(const std::string& key, int value)
{
	data.put(key, value);
}
void Bundle::put(const std::string& key, float value)
{
	data.put(key, value);
}
void Bundle::put(const std::string& key, const std::string& value)
{
	data.put(key, value);
}

void Bundle::put(const std::string& key, Bundle* value)
{
	data.put_child(key, value->data);
}

void Bundle::put(const std::string& key, Bundlable* object)
{
	Bundle bundle;
	bundle.put(CLASS_NAME, object->getClassName());
	object->storeInBundle(&bundle);
	data.put_child(key, bundle.data);
}

void Bundle::put(const std::string& key, const std::vector<int>& arr)
{
	boost::property_tree::ptree array1;
	for (int i = 0; i < arr.size(); i++)
	{
		array1.put("", arr[i]);
	}
	data.put_child(key, array1);
}

void Bundle::put(const std::string& key, const std::vector<bool>& arr)
{
	boost::property_tree::ptree array1;
	for (int i = 0; i < arr.size(); i++)
	{
		array1.put("", arr[i]);
	}
	data.put_child(key, array1);
}

void Bundle::put(const std::string& key, const std::vector<std::string>& arr)
{
	boost::property_tree::ptree array1;
	for (int i = 0; i < arr.size(); i++)
	{
		array1.put("", arr[i]);
	}
	data.put_child(key, array1);
}

void Bundle::put(const std::string& key, const std::vector<Bundlable*>& arr)
{
	boost::property_tree::ptree array1;

	for (std::vector<Bundlable*>::const_iterator itr = arr.begin();
		itr != arr.end(); itr++)
	{
		Bundle bundle;
		bundle.put(CLASS_NAME, (*itr)->getClassName());
		(*itr)->storeInBundle(&bundle);
		
		array1.put_child("", bundle.data);		
	}
	data.put_child(key, array1);
}

void Bundle::getCollection(const std::string& key, std::vector<Bundlable*>&ret, GETCOLLECTIONCALLBACK cb)
{
	boost::property_tree::ptree image_array = data.get_child(key);
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, image_array)
	{
		Bundle bd;
		bd.data = v.second;
		Bundlable* bdInst = cb();
		bdInst->restoreFromBundle(&bd);
		ret.push_back(bdInst);
	}
}

void Bundle::RegisterBundlable(const std::string& clsName, CreateBundlable cb)
{
	allBundlables.insert(std::make_pair(clsName, cb));
}