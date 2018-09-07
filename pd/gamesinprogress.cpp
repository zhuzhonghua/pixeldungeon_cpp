#include "gamesinprogress.h"
#include "dungeon.h"

std::map<HeroClass, GamesInProgress::Info> GamesInProgress::state;

bool GamesInProgress::check(HeroClass cl, GamesInProgress::Info& info)
{
	std::map<HeroClass, GamesInProgress::Info>::iterator itr = state.find(cl);
	if (itr != state.end()) 
	{
		info = itr->second;
		return true;
	}
	else 
	{
		Bundle* bundle = Dungeon::gameBundle(Dungeon::gameFile(cl));
		if (!bundle) return false;

		Dungeon::preview(info, bundle);

		state.insert(std::make_pair(cl, info));
		delete bundle;

		return true;
	}
}