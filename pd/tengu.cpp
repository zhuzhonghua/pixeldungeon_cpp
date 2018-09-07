#include "tengu.h"
#include "mobsprite.h"
#include "dungeon.h"
#include "statistics.h"
#include "gamescene.h"
#include "terrain.h"
#include "speck.h"
#include "cellemitter.h"
#include "hero.h"

CharSprite* Tengu::Sprite()
{
	return new TenguSprite();
}

Mob* Tengu::CreateTengu()
{
	return new Tengu();
}

Tengu::Tengu()
{
	name = Dungeon::depth == Statistics::deepestFloor ? "Tengu" : "memory of Tengu";
	//spriteClass = TenguSprite.class;

	HP = HT = 120;
	EXP = 20;
	_defenseSkill = 20;

	timeToJump = JUMP_DELAY;
}

int Tengu::damageRoll()
{
	return Random::NormalIntRange(8, 15);
}

void Tengu::die(const std::string& cause)
{
	Badges::Badge badgeToCheck = Badges::MASTERY_WARRIOR;
	switch (Dungeon::hero->heroClass.type()) {
	case HeroClass::E_WARRIOR:
		badgeToCheck = Badges::MASTERY_WARRIOR;
		break;
	case HeroClass::E_MAGE:
		badgeToCheck = Badges::MASTERY_MAGE;
		break;
	case HeroClass::E_ROGUE:
		badgeToCheck = Badges::MASTERY_ROGUE;
		break;
	case HeroClass::E_HUNTRESS:
		badgeToCheck = Badges::MASTERY_HUNTRESS;
		break;
	}
	if (!Badges::isUnlocked(badgeToCheck) || Dungeon::hero->subClass != HeroSubClass::NONE) 
	{
		//Dungeon.level.drop(new TomeOfMastery(), pos).sprite.drop();
	}

	GameScene::bossSlain();
	//Dungeon.level.drop(new SkeletonKey(), pos).sprite.drop();
	Mob::die(cause);

	//Badges::validateBossSlain();

	yell("Free at last...");
}

void Tengu::notice()
{
	Mob::notice();
	yell("Gotcha, " + Dungeon::hero->heroClass.title() + "!");
}

std::string Tengu::description()
{
	return
		std::string("Tengu are members of the ancient assassins clan, which is also called Tengu. ") +
		std::string("These assassins are noted for extensive use of shuriken and traps.");
}

void Tengu::jump()
{
	timeToJump = JUMP_DELAY;

	for (int i = 0; i < 4; i++) 
	{
		int trapPos;
		do {
			trapPos = Random::Int(Level::LENGTH);
		} while (!Level::fieldOfView[trapPos] || !Level::passable[trapPos]);

		if (Dungeon::level->map[trapPos] == Terrain::INACTIVE_TRAP) 
		{
			Level::set(trapPos, Terrain::POISON_TRAP);
			GameScene::updateMap(trapPos);
			//ScrollOfMagicMapping.discover(trapPos);
		}
	}

	int newPos;
	do {
		newPos = Random::Int(Level::LENGTH);
	} while (
		!Level::fieldOfView[newPos] ||
		!Level::passable[newPos] ||
		(enemy != NULL && Level::adjacent(newPos, enemy->pos)) ||
		Actor::findChar(newPos) != NULL);

	sprite->move(pos, newPos);
	move(newPos);

	if (Dungeon::visible[newPos]) {
		CellEmitter::get(newPos)->burst(Speck::factory(Speck::WOOL), 6);
		//Sample.INSTANCE.play(Assets::SND_PUFF);
	}

	spend(1 / speed());
}

bool Tengu::getCloser(int target)
{
	if (Level::fieldOfView[target]) 
	{
		jump();
		return true;
	}
	else 
	{
		return Mob::getCloser(target);
	}
}

bool Tengu::canAttack(Char* enemy)
{
	return true;// Ballistica.cast(pos, enemy.pos, false, true) == enemy.pos;
}

bool Tengu::doAttack(Char* enemy)
{
	timeToJump--;
	if (timeToJump <= 0 && Level::adjacent(pos, enemy->pos)) 
	{
		jump();
		return true;
	}
	else 
	{
		return Mob::doAttack(enemy);
	}
}
