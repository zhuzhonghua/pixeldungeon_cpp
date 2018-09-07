#include "bundlable.h"
#include "bundle.h"

RegisterBundlable::RegisterBundlable(const std::string& clsName, CreateBundlable cb)
{
	Bundle::RegisterBundlable(clsName, cb);
}

void RegisterBundlable::init()
{

}
