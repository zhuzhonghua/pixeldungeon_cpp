#include "buff.h"
#include "char.h"
#include "buffindicator.h"
#include "dungeon.h"
#include "ring.h"
#include "burning.h"
#include "food.h"
#include "belongings.h"
#include "heap.h"
#include "util.h"
#include "splash.h"
#include "charsprite.h"
#include "dungeon.h"
#include "simpleresource.h"
#include "glog.h"
#include "hero.h"

std::map<std::string, FactoryBuff*> FactoryBuff::facs;

Buff::Buff()
{
	target = NULL;
}

bool Buff::attachTo(Char* target)
{
	if (target->immunitiesContain(getClassName())) 
	{
		return false;
	}
	
	this->target = target;
	target->add(this);

	return true;
}

void Buff::detach()
{
	target->remove(this);
}

void Buff::detach(Buff* buff)
{
	if (buff != NULL) 
	{
		buff->detach();
	}
}

void Buff::detach(Char* target, const std::string& cl)
{
	detach(target->buff(cl));
}

int Buff::icon()
{
	return BuffIndicator::NONE;
}

Buff* Buff::append(Char* target, const std::string& buff)
{
	Buff* buf = FactoryBuff::Create(buff);
	buf->attachTo(target);
	return buf;
}

FlavourBuff* Buff::append(Char* target, const std::string& buffClass, float duration)
{
	FlavourBuff* buff = (FlavourBuff*)append(target, buffClass);
	buff->spend(duration);
	return buff;
}

Buff* Buff::affect(Char* target, const std::string& buffClass)
{
	Buff* buff = target->buff(buffClass);
	if (buff != NULL) 
	{
		return buff;
	}
	else 
	{
		return append(target, buffClass);
	}
}

FlavourBuff* Buff::affect(Char* target, const std::string& buffClass, float duration)
{
	FlavourBuff* buff = (FlavourBuff*)affect(target, buffClass);
	buff->spend(duration);
	return buff;
}

FlavourBuff* Buff::prolong(Char* target, const std::string& buffClass, float duration)
{
	FlavourBuff* buff = (FlavourBuff*)affect(target, buffClass);
	buff->postpone(duration);
	return buff;
}

REFLECTBUFF(FlavourBuff);

const float Light::DURATION = 250.0f;

boolean Light::attachTo(Char* target)
{
	if (FlavourBuff::attachTo(target)) {
		if (Dungeon::level != NULL) {
			target->viewDistance = std::max(Dungeon::level->viewDistance, DISTANCE);
			Dungeon::observe();
		}
		return true;
	}
	else {
		return false;
	}
}

void Light::detach()
{
	target->viewDistance = Dungeon::level->viewDistance;
	Dungeon::observe();
	FlavourBuff::detach();
}

int Light::icon()
{
	return BuffIndicator::LIGHT;
}

REFLECTBUFF(Light);

const float BuffParalysis::DURATION = 10.0f;

boolean BuffParalysis::attachTo(Char* target)
{
	if (FlavourBuff::attachTo(target)) {
		target->paralysed = true;
		return true;
	}
	else {
		return false;
	}
}

void BuffParalysis::detach()
{
	FlavourBuff::detach();
	unfreeze(target);
}

int BuffParalysis::icon()
{
	return BuffIndicator::PARALYSIS;
}

float BuffParalysis::duration(Char* ch)
{
	RingOfElements::Resistance* r = (RingOfElements::Resistance*)ch->buff("Resistance");
	return r != NULL ? r->durationFactor() * DURATION : DURATION;
}

void BuffParalysis::unfreeze(Char* ch)
{
	if (ch->buff("BuffParalysis") == NULL &&
		ch->buff("Frost") == NULL) {
		ch->paralysed = false;
	}
}

REFLECTBUFF(BuffParalysis);

const float Frost::DURATION = 5.0f;

boolean Frost::attachTo(Char* target)
{
	if (FlavourBuff::attachTo(target)) {

		target->paralysed = true;
		Burning::detach(target, "Burning");

		if (dynamic_cast<Hero*>(target)) {
			Hero* hero = (Hero*)target;
			Item* item = hero->belongings->randomUnequipped();
			if (dynamic_cast<MysteryMeat*>(item)) {

				item = item->detach(hero->belongings->backpack);
				FrozenCarpaccio* carpaccio = new FrozenCarpaccio();
				if (!carpaccio->collect(hero->belongings->backpack)) {
					Dungeon::level->drop(carpaccio, target->pos)->sprite->drop();
				}
			}
		}

		return true;
	}
	else {
		return false;
	}
}

void Frost::detach()
{
	FlavourBuff::detach();
	BuffParalysis::unfreeze(target);
}

int Frost::icon()
{
	return BuffIndicator::FROST;
}

float Frost::duration(Char* ch)
{
	RingOfElements::Resistance* r = (RingOfElements::Resistance*)ch->buff("Resistance");
	return r != NULL ? r->durationFactor() * DURATION : DURATION;
}

REFLECTBUFF(Frost);

const float BuffSlow::DURATION = 10.0f;

int BuffSlow::icon()
{
	return BuffIndicator::SLOW;
}

float BuffSlow::duration(Char* ch)
{
	RingOfElements::Resistance* r = (RingOfElements::Resistance*)ch->buff("Resistance");
	return r != NULL ? r->durationFactor() * DURATION : DURATION;
}
REFLECTBUFF(BuffSlow);

const float Vertigo::DURATION=10.0f;

int Vertigo::icon()
{
	return BuffIndicator::VERTIGO;
}

float Vertigo::duration(Char* ch)
{
	RingOfElements::Resistance* r = (RingOfElements::Resistance*)ch->buff("Resistance");
	return r != NULL ? r->durationFactor() * DURATION : DURATION;
}

REFLECTBUFF(Vertigo);

const String Terror::OBJECT = "object";

const float Terror::DURATION = 10.0f;

Terror::Terror()
{
	object = 0;
}

void Terror::storeInBundle(Bundle* bundle)
{
	FlavourBuff::storeInBundle(bundle);
	bundle->put(OBJECT, object);
}

void Terror::restoreFromBundle(Bundle* bundle)
{
	FlavourBuff::restoreFromBundle(bundle);
	object = bundle->getInt(OBJECT);
}

int Terror::icon()
{
	return BuffIndicator::TERROR;
}

void Terror::recover(Char* target)
{
	Terror* terror = (Terror*)target->buff("Terror");
	if (terror != NULL && terror->cooldown() < DURATION) {
		target->remove(terror);
	}
}

REFLECTBUFF(Terror);

const float Cripple::DURATION = 10.0f;

int Cripple::icon()
{
	return BuffIndicator::CRIPPLE;
}

REFLECTBUFF(Cripple);

const String Bleeding::LEVEL = "level";

void Bleeding::storeInBundle(Bundle* bundle)
{
	Buff::storeInBundle(bundle);
	bundle->put(LEVEL, level);
}

void Bleeding::restoreFromBundle(Bundle* bundle)
{
	Buff::restoreFromBundle(bundle);
	level = bundle->getInt(LEVEL);
}

int Bleeding::icon()
{
	return BuffIndicator::BLEEDING;
}

boolean Bleeding::act()
{
	if (target->isAlive()) {

		if ((level = Random::Int(level / 2, level)) > 0) {

			target->damage(level, this->getClassName());
			if (target->sprite->visible) {
				Splash::at(target->sprite->center(), -GameMath::POINTF_PI / 2, GameMath::POINTF_PI / 6,
					target->sprite->blood(), std::min(10 * level / target->HT, 10));
			}

			if (target == Dungeon::hero && !target->isAlive()) {
				Dungeon::fail(GameMath::format(ResultDescriptions::BLEEDING.c_str(), Dungeon::depth));
				GLog::n(BPT::getText("lang.bled_to_death").c_str());
			}

			spend(TICK);
		}
		else {
			detach();
		}

	}
	else {

		detach();

	}

	return true;
}

REFLECTBUFF(Bleeding);

int Amok::icon()
{
	return BuffIndicator::AMOK;
}

REFLECTBUFF(Amok);

const float Sleep::SWS = 1.5f;
REFLECTBUFF(Sleep);

const String Ooze::TXT_HERO_KILLED = BPT::getText("lang.killed_you");// "%s killed you...";

REFLECTBUFF(Ooze);
Ooze::Ooze()
{
	damage = 1;
}

int Ooze::icon()
{
	return BuffIndicator::OOZE;
}

boolean Ooze::act()
{
	if (target->isAlive()) {
		target->damage(damage, this->getClassName());
		if (!target->isAlive() && target == Dungeon::hero) {
			Dungeon::fail(GameMath::format(ResultDescriptions::OOZE.c_str(), Dungeon::depth));
			GLog::n(TXT_HERO_KILLED.c_str(), toString());
		}
		spend(TICK);
	}
	if (Level::water[target->pos]) {
		detach();
	}
	return true;
}

const float Levitation::DURATION = 20.0f;

boolean Levitation::attachTo(Char* target)
{
	if (FlavourBuff::attachTo(target)) {
		target->flying = true;
		Buff::detach(target, "Roots");
		return true;
	}
	else {
		return false;
	}
}

void Levitation::detach()
{
	target->flying = false;
	Dungeon::level->press(target->pos, target);
	FlavourBuff::detach();
}

int Levitation::icon()
{
	return BuffIndicator::LEVITATION;
}
REFLECTBUFF(Levitation);

const float Invisibility::DURATION = 15.0f;

boolean Invisibility::attachTo(Char* target)
{
	if (FlavourBuff::attachTo(target)) {
		target->invisible++;
		return true;
	}
	else {
		return false;
	}
}

void Invisibility::detach()
{
	target->invisible--;
	FlavourBuff::detach();
}

int Invisibility::icon()
{
	return BuffIndicator::INVISIBLE;
}

void Invisibility::dispel()
{
	Invisibility* buff = (Invisibility*)Dungeon::hero->buff("Invisibility");
	if (buff != NULL && Dungeon::hero->visibleEnemies.size() > 0) {
		buff->detach();
	}
}
REFLECTBUFF(Invisibility);

boolean Roots::attachTo(Char* target)
{
	if (!target->flying && FlavourBuff::attachTo(target)) {
		target->rooted = true;
		return true;
	}
	else {
		return false;
	}
}

void Roots::detach()
{
	target->rooted = false;
	FlavourBuff::detach();
}

int Roots::icon()
{
	return BuffIndicator::ROOTS;
}
REFLECTBUFF(Roots);

const float Speed::DURATION = 10.0f;
REFLECTBUFF(Speed);

const float MindVision::DURATION = 20.0f;

MindVision::MindVision()
{
	distance = 2;
}

int MindVision::icon()
{
	return BuffIndicator::MIND_VISION;
}

void MindVision::detach()
{
	FlavourBuff::detach();
	Dungeon::observe();
}
REFLECTBUFF(MindVision);

const String Shadows::LEFT = "left";

void Shadows::storeInBundle(Bundle* bundle)
{
	Invisibility::storeInBundle(bundle);
	bundle->put(LEFT, left);
}

void Shadows::restoreFromBundle(Bundle* bundle)
{
	Invisibility::restoreFromBundle(bundle);
	left = bundle->getFloat(LEFT);
}

boolean Shadows::attachTo(Char* target)
{
	if (Invisibility::attachTo(target)) {
		//Sample.INSTANCE.play(Assets.SND_MELD);
		Dungeon::observe();
		return true;
	}
	else {
		return false;
	}
}

void Shadows::detach()
{
	Invisibility::detach();
	Dungeon::observe();
}

boolean Shadows::act()
{
	if (target->isAlive()) {

		spend(TICK * 2);

		if (--left <= 0 || Dungeon::hero->visibleEnemies.size() > 0) {
			detach();
		}

	}
	else {

		detach();

	}

	return true;
}

int Shadows::icon()
{
	return BuffIndicator::SHADOWS;
}
REFLECTBUFF(Shadows);

Charm::Charm()
{
	object = 0;
}

void Charm::storeInBundle(Bundle* bundle)
{
	FlavourBuff::storeInBundle(bundle);
	bundle->put(OBJECT, object);
}

void Charm::restoreFromBundle(Bundle* bundle)
{
	FlavourBuff::restoreFromBundle(bundle);
	object = bundle->getInt(OBJECT);
}

int Charm::icon()
{
	return BuffIndicator::HEART;
}

float Charm::durationFactor(Char* ch)
{
	RingOfElements::Resistance* r = (RingOfElements::Resistance*)(ch->buff("Resistance"));
	return r != NULL ? r->durationFactor() : 1;
}

const String Charm::OBJECT = "object";

REFLECTBUFF(Charm);

Barkskin::Barkskin()
{
	level = 0;
}

boolean Barkskin::act()
{
	if (target->isAlive()) {

		spend(TICK);
		if (--level <= 0) {
			detach();
		}

	}
	else {

		detach();

	}

	return true;
}

int Barkskin::icon()
{
	return BuffIndicator::BARKSKIN;
}
REFLECTBUFF(Barkskin);

const float Hunger::STEP = 10.0f;
const float Hunger::HUNGRY = 260.0f;
const float Hunger::STARVING = 360.0f;
const String Hunger::TXT_HUNGRY = BPT::getText("lang.Hunger_Txt_Hungry");// "You are hungry.";
const String Hunger::TXT_STARVING = BPT::getText("lang.Hunger_Txt_Starving");//"You are starving!";
const String Hunger::TXT_DEATH = BPT::getText("lang.Hunger_Txt_Death");//"You starved to death...";

const String Hunger::LEVEL = "level";

void Hunger::storeInBundle(Bundle* bundle)
{
	Buff::storeInBundle(bundle);
	bundle->put(LEVEL, level);
}

void Hunger::restoreFromBundle(Bundle* bundle)
{
	Buff::restoreFromBundle(bundle);
	level = bundle->getFloat(LEVEL);
}

boolean Hunger::act()
{
	if (target->isAlive()) {

		Hero* hero = (Hero*)target;

		if (isStarving()) {
			if (Random::Float() < 0.3f && (target->HP > 1 || !target->paralysed)) {

				GLog::n(TXT_STARVING.c_str());
				hero->damage(1, this->getClassName());

				hero->interrupt();
			}
		}
		else {

			int bonus = 0;
			std::set<Buff*> tempBufs;
			//RingOfSatiety.Satiety
			target->Buffs("Satiety", tempBufs);
			for (std::set<Buff*>::iterator itr = tempBufs.begin();
				itr != tempBufs.end(); itr++)
			{
				Buff* buff = *itr;
				bonus += ((RingOfSatiety::Satiety*)buff)->level;
			}

			float newLevel = level + STEP - bonus;
			boolean statusUpdated = false;
			if (newLevel >= STARVING) {

				GLog::n(TXT_STARVING.c_str());
				statusUpdated = true;

				hero->interrupt();

			}
			else if (newLevel >= HUNGRY && level < HUNGRY) {

				GLog::w(TXT_HUNGRY.c_str());
				statusUpdated = true;

			}
			level = newLevel;

			if (statusUpdated) {
				BuffIndicator::refreshHero();
			}

		}

		float step = ((Hero*)target)->heroClass.type() == HeroClass::E_ROGUE ? STEP * 1.2f : STEP;
		spend(target->buff("Shadows") == NULL ? step : step * 1.5f);

	}
	else {

		diactivate();

	}

	return true;
}

void Hunger::satisfy(float energy)
{
	level -= energy;
	if (level < 0) {
		level = 0;
	}
	else if (level > STARVING) {
		level = STARVING;
	}

	BuffIndicator::refreshHero();
}

int Hunger::icon()
{
	if (level < HUNGRY) {
		return BuffIndicator::NONE;
	}
	else if (level < STARVING) {
		return BuffIndicator::HUNGER;
	}
	else {
		return BuffIndicator::STARVATION;
	}
}

void Hunger::onDeath()
{
	Badges::validateDeathFromHunger();

	Dungeon::fail(GameMath::format(ResultDescriptions::HUNGER.c_str(), Dungeon::depth));
	GLog::n(TXT_DEATH.c_str());
}
REFLECTBUFF(Hunger);

const float Regeneration::REGENERATION_DELAY = 10;

boolean Regeneration::act()
{
	if (target->isAlive()) {

		if (target->HP < target->HT && !((Hero*)target)->isStarving()) {
			target->HP += 1;
		}

		int bonus = 0;
		std::set<Buff*> tempBufs;
		target->Buffs("Rejuvenation", tempBufs);
		for (std::set<Buff*>::iterator itr = tempBufs.begin();
			itr != tempBufs.end(); itr++)
		{
			Buff* buff = *itr;
			bonus += ((RingOfMending::Rejuvenation*)buff)->level;
		}

		spend((float)(REGENERATION_DELAY / std::pow(1.2, bonus)));

	}
	else {

		diactivate();

	}

	return true;
}

REFLECTBUFF(Regeneration);
