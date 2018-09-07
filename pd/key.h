#pragma once

#include "item.h"
#include "bpt.h"

class Key :public Item{
public:
	static const float TIME_TO_UNLOCK;
	int depth;

	Key();

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	virtual boolean isUpgradable() {
		return false;
	}
	virtual boolean isIdentified() {
		return true;
	}
	virtual String status();
private:
	static const String DEPTH;
};

class SkeletonKey :public Key{
public:
	SkeletonKey();
	CLASSNAME(SkeletonKey);
	virtual String info() {
		return BPT::getText("lang.skeletonkey_info");
	}
};

class GoldenKey :public Key{
public:
	GoldenKey();

	CLASSNAME(GoldenKey);

	virtual String info() {
		return BPT::getText("lang.GoldenKey_Info");
			//std::string("The notches on this golden key are tiny and intricate. ") +
			//std::string("Maybe it can open some chest lock?");
	}
};

class IronKey :public Key{
private:
	static const String TXT_FROM_DEPTH;
public:
	static int curDepthQuantity;
	IronKey();
	
	CLASSNAME(IronKey);
	virtual boolean collect(Bag* bag);
	virtual void onDetach();
	virtual String toString();
	virtual String info() {
		return BPT::getText("lang.IronKey_Info");
			//"The notches on this ancient iron key are well worn; its leather lanyard " +
			//"is battered by age. What door might it open?";
	}
};