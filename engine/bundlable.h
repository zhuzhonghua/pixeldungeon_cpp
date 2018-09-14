#pragma once
#include <string>

class Bundle;
class Bundlable{
public:
	virtual void restoreFromBundle(Bundle* bundle) = 0;
	virtual void storeInBundle(Bundle* bundle) = 0;
	virtual std::string getClassName() = 0;
};

typedef Bundlable* (*CreateBundlable)();

class RegisterBundlable{
public:
	RegisterBundlable(const std::string& clsName, CreateBundlable cb);

	static void init();
};