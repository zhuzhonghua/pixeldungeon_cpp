#include "eye.h"
#include "mobsprite.h"

CharSprite* Eye::Sprite()
{
	return new EyeSprite();
}

Mob* Eye::CreateEye()
{
	return new Eye();
}

Eye::Eye()
{
	name = "evil eye";
	//spriteClass = EyeSprite.class;

	HP = HT = 100;
	_defenseSkill = 20;
	//viewDistance = Light.DISTANCE;

	EXP = 13;
	maxLvl = 25;

	flying = true;

	//loot = new Dewdrop();
	lootChance = 0.5f;
}

bool Eye::attack(Char* enemy)
{
	//for (int i = 1; i < Ballistica.distance; i++) {
	//
	//	int pos = Ballistica.trace[i];
	//
	//	Char ch = Actor.findChar(pos);
	//	if (ch == null) {
	//		continue;
	//	}
	//
	//	if (hit(this, ch, true)) {
	//		ch.damage(Random.NormalIntRange(14, 20), this);
	//
	//		if (Dungeon.visible[pos]) {
	//			ch.sprite.flash();
	//			CellEmitter.center(pos).burst(PurpleParticle.BURST, Random.IntRange(1, 2));
	//		}
	//
	//		if (!ch.isAlive() && ch == Dungeon.hero) {
	//			Dungeon.fail(Utils.format(ResultDescriptions.MOB, Utils.indefinite(name), Dungeon.depth));
	//			GLog.n(TXT_DEATHGAZE_KILLED, name);
	//		}
	//	}
	//	else {
	//		ch.sprite.showStatus(CharSprite.NEUTRAL, ch.defenseVerb());
	//	}
	//}

	return true;
}

bool Eye::canAttack(Char* enemy)
{
	//hitCell = Ballistica.cast(pos, enemy.pos, true, false);
	//
	//for (int i = 1; i < Ballistica.distance; i++) {
	//	if (Ballistica.trace[i] == enemy.pos) {
	//		return true;
	//	}
	//}
	return false;
}

bool Eye::doAttack(Char* enemy)
{
	spend(attackDelay());

	bool rayVisible = false;

	//for (int i = 0; i < Ballistica.distance; i++) {
	//	if (Dungeon.visible[Ballistica.trace[i]]) {
	//		rayVisible = true;
	//	}
	//}

	if (rayVisible) 
	{
		sprite->Attack(hitCell);
		return false;
	}
	else 
	{
		attack(enemy);
		return true;
	}
}

const std::string Eye::TXT_DEATHGAZE_KILLED = "%s's deathgaze killed you...";