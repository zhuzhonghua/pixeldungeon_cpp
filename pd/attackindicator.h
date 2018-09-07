#pragma once

#include "tag.h"
#include <list>

class CharSprite;
class Mob;
class Char;

class AttackIndicator :public Tag{
private:
	static const float ENABLED;
	static const float DISABLED;

	static AttackIndicator* instance;

	CharSprite* sprite;
	bool enabled;
	static Mob* lastTarget;
	std::list<Mob*> candidates;

	void checkEnemies();
	void updateImage();
	void enable(bool value);
	void visible(bool value);
public:
	AttackIndicator();

	virtual void update();
	static void target(Char* target);
	static void updateState();
protected:
	virtual void layout();
	virtual void onClick();
};