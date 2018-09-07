#pragma once
#include "image.h"
#include "heroclass.h"

class Icons{
public:
	enum Type{
		SKULL,
		BUSY,
		COMPASS,
		PREFS,
		WARNING,
		TARGET,
		WATA,
		WARRIOR,
		MAGE,
		ROGUE,
		HUNTRESS,
		CLOSE,
		DEPTH,
		SLEEP,
		ALERT,
		SUPPORT,
		SUPPORTED,
		BACKPACK,
		SEED_POUCH,
		SCROLL_HOLDER,
		WAND_HOLSTER,
		KEYRING,
		CHECKED,
		UNCHECKED,
		EXIT,
		CHALLENGE_OFF,
		CHALLENGE_ON,
		RESUME
	};

	Type type;
	Image* get();
	static Image* get(Type type);
	static Image* get(HeroClass& cl);
};