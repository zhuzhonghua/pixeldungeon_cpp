#pragma once

#include <map>
#include "heroclass.h"

class GamesInProgress{
public:
	struct Info {
		int depth;
		int level;
		bool challenges;
		Info(){}
		Info(int d, int l, bool c) :depth(d), level(l), challenges(c){}
	};

	static bool check(HeroClass cl, Info& info);
private:
	static std::map<HeroClass, Info> state;
};