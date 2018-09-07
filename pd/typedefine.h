#pragma once

#include <map>
#include <list>
#include <set>

typedef std::string String;
typedef bool boolean;

// 与boost中某些final定义冲突
//#define final const
#define null NULL

template<class T>
class HashSet :public std::set<T>{
public:
	bool contains(T t){
		return this->find(t) != this->end();
	}
	void add(T t){
		this->insert(t);
	}
	void remove(T t){
		iterator itr = this->find(t);
		if (itr != this->end()){
			this->erase(itr);
		}
	}
};

template<class T>
class List :public std::list<T>{
public:
	bool contains(T t){
		return std::find(this->begin(), this->end(), t) != this->end();
	}
};

template<class K, class V>
class HashMap :public std::map<K, V>{
public:
	bool containsKey(const K& k){
		return this->find(k) != this->end();
	}
	V get(const K& k){
		return this->operator[](k);
	}
	std::list<V> values(){
		std::list<V> re;
		for (iterator itr = this->begin(); itr != this->end(); itr++){
			re.push_back(itr->second);
		}
		return re;
	}
};

template<class T>
class ArrayList :public std::list<T>{
public:
	bool contains(T t){
		return std::find(this->begin(), this->end(), t) != this->end();
	}
	void add(T t){
		this->push_back(t);
	}
};

class Object{
public:
	virtual std::string getObject() = 0;
};

#define CLASSOBJECT(name) virtual std::string getObject() { return #name;}
#define CLASSNAME(name) virtual std::string getClassName() { return #name;}

#define __FACTORY__(name, cls) \
class Factory##name{ \
public:\
	static cls* Create(const std::string& clsName)\
	{\
		return facs[clsName]->Create();\
	}\
protected:\
	virtual cls* Create() = 0; \
	static std::map<std::string, Factory##name*> facs;\
};

#define FACTORY(name) __FACTORY__(name, name)

#define FACTORYIMPL(name) std::map<std::string, Factory##name*> Factory##name::facs;

#define REFLECT(name, clsName, type) \
class Factory##type##name:public Factory##type{ \
public: \
	Factory##type##name(){\
		facs.insert(std::make_pair(#name, this)); \
	}\
protected:\
	virtual clsName* Create(){\
		return new clsName(); \
	}\
}; \
namespace{ Factory##type##name reg##name; }

//#define REFLECTBUFF(name) REFLECT(name, name, Buff)
//#define REFLECTBUFF2(name, clsName) REFLECT(name, clsName, Buff)
//#define REFLECTBLOB(name) REFLECT(name, name, Blob)
//#define REFLECTITEM(name) REFLECT(name, name, Item)
//#define REFLECMOB(name) REFLECT(name, name, Mob)
//#define REFLECTGLYPH(name) REFLECT(name, name, Glyph)

#define REFLECTBUFF(name) 
#define REFLECTBUFF2(name, clsName) 
#define REFLECTBLOB(name) 
#define REFLECTITEM(name) 
#define REFLECMOB(name) 
#define REFLECTGLYPH(name) 

#define RECYCLE(var, parent, cls) \
	var = (cls*)(parent)->recycle(#cls);\
	if (var == NULL) \
	{ \
		var = new cls(); \
		(parent)->add(var); \
	}

#define RECYCLE2(var, parent, cls) \
	cls* var = (cls*)(parent)->recycle(#cls); \
	if (var == NULL) \
	{ \
		var = new cls(); \
		(parent)->add(var); \
	}