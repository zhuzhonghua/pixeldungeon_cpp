#include "char.h"
#include "terrain.h"
#include "dungeon.h"
#include "buff.h"
#include "util.h"
#include "charsprite.h"
#include "glog.h"
#include "burning.h"
#include "buffpoison.h"
#include "cellemitter.h"
#include "pixelparticle.h"
#include "camera.h"
#include "simpleresource.h"
#include "bestiary.h"
#include "door.h"
#include "hero.h"

const std::string Char::TXT_HIT = BPT::getText("lang.s_hit_s");// "%s hit %s";
const std::string Char::TXT_KILL = BPT::getText("lang.killed_you");// "%s killed you...";
const std::string Char::TXT_DEFEAT = BPT::getText("lang.s_defeated_s");// "%s defeated %s";

const std::string Char::TXT_YOU_MISSED = BPT::getText("lang.s_s_your_attack");// "%s %s your attack";
const std::string Char::TXT_SMB_MISSED = BPT::getText("lang.s_s_s_s_attack");// "%s %s %s's attack";

const std::string Char::TXT_OUT_OF_PARALYSIS = BPT::getText("lang.The_pain_snapped_out_paralysis");// "The pain snapped %s out of paralysis";

const std::string Char::POS = "pos";
const std::string Char::TAG_HP = "HP";
const std::string Char::TAG_HT = "HT";
const std::string Char::BUFFS = "buffs";

HashSet<std::string> Char::EMPTY;

Char::Char()
{
	paralysed = false;
	rooted = false;
	flying = false;
	invisible = 0;

	viewDistance = 8;

	baseSpeed = 1;
	name = "mob";
	pos = 0;
	sprite = NULL;
}

bool Char::act()
{
	Dungeon::level->updateFieldOfView(this);
	return false;
}

void Char::spend(float time)
{
	float timeScale = 1.0f;
	if (buff("BuffSlow") != NULL) {
		timeScale *= 0.5f;
	}
	if (buff("Speed") != NULL) {
		timeScale *= 2.0f;
	}

	Actor::spend(time / timeScale);
}

void Char::onRemove()
{
	std::vector<Buff*> arr(buffs.begin(), buffs.end());
	for (int i = 0; i < arr.size(); i++)
	{
		arr[i]->detach();
	}
}

void Char::storeInBundle(Bundle* bundle)
{
	Actor::storeInBundle(bundle);

	bundle->put(POS, pos);
	bundle->put(TAG_HP, HP);
	bundle->put(TAG_HT, HT);

	// 
	std::vector<Bundlable*> temp;
	for (std::set<Buff*>::iterator itr = buffs.begin();
		itr != buffs.end(); itr++)
	{
		temp.push_back(*itr);
	}
	bundle->put(BUFFS, temp);
}

namespace{
	static Bundlable* getBuffInstance()
	{
		//return new Buff();
		return NULL;
	}

}
void Char::restoreFromBundle(Bundle* bundle)
{
	Actor::restoreFromBundle(bundle);

	pos = bundle->getInt(POS);
	HP = bundle->getInt(TAG_HP);
	HT = bundle->getInt(TAG_HT);

	//std::vector<Bundlable*> result;
	////bundle->getCollection(BUFFS, result, getBuffInstance);
	//
	//for (int i = 0; i < result.size();i++)
	//{
	//	((Buff*)result[i])->attachTo(this);
	//}
}

HashSet<std::string>& Char::resistances()
{
	return EMPTY;
}

HashSet<std::string>& Char::immunities()
{
	return EMPTY;
}

bool Char::immunitiesContain(const std::string& cls)
{
	return EMPTY.find(cls) != EMPTY.end();
}

void Char::add(Buff* buff)
{
	buffs.insert(buff);
	Actor::add(buff);

	if (sprite != NULL) {
		if (dynamic_cast<BuffPoison*>(buff)) {

			CellEmitter::center(pos)->burst(PoisonParticle::SPLASH, 5);
			sprite->showStatus(CharSprite::NEGATIVE, "poisoned");

		}
		else if (dynamic_cast<Amok*>(buff)) {

			sprite->showStatus(CharSprite::NEGATIVE, "amok");

		}
		else if (dynamic_cast<BuffSlow*>(buff)) {

			sprite->showStatus(CharSprite::NEGATIVE, "slowed");

		}
		else if (dynamic_cast<MindVision*>(buff)) {

			sprite->showStatus(CharSprite::POSITIVE, "mind");
			sprite->showStatus(CharSprite::POSITIVE, "vision");

		}
		else if (dynamic_cast<BuffParalysis*>(buff)) {

			sprite->add(CharSprite::State::PARALYSED);
			sprite->showStatus(CharSprite::NEGATIVE, "paralysed");

		}
		else if (dynamic_cast<Terror*>(buff)) {

			sprite->showStatus(CharSprite::NEGATIVE, "frightened");

		}
		else if (dynamic_cast<Roots*>(buff)) {

			sprite->showStatus(CharSprite::NEGATIVE, "rooted");

		}
		else if (dynamic_cast<Cripple*>(buff)) {

			sprite->showStatus(CharSprite::NEGATIVE, "crippled");

		}
		else if (dynamic_cast<Bleeding*>(buff)) {

			sprite->showStatus(CharSprite::NEGATIVE, "bleeding");

		}
		else if (dynamic_cast<Vertigo*>(buff)) {

			sprite->showStatus(CharSprite::NEGATIVE, "dizzy");

		}
		else if (dynamic_cast<Sleep*>(buff)) {
			sprite->Idle();
		}

		else if (dynamic_cast<Burning*>(buff)) {
			sprite->add(CharSprite::State::BURNING);
		}
		else if (dynamic_cast<Levitation*>(buff)) {
			sprite->add(CharSprite::State::LEVITATING);
		}
		else if (dynamic_cast<Frost*>(buff)) {
			sprite->add(CharSprite::State::FROZEN);
		}
		else if (dynamic_cast<Invisibility*>(buff)) {
			if (!(dynamic_cast<Shadows*>(buff))) {
				sprite->showStatus(CharSprite::POSITIVE, "invisible");
			}
			sprite->add(CharSprite::State::INVISIBLE);
		}
	}
}

void Char::remove(Buff* buff)
{
	std::set<Buff*>::iterator itr = buffs.find(buff);
	if (itr != buffs.end()) buffs.erase(itr);
	
	if (dynamic_cast<Burning*>(buff)) {
		sprite->remove(CharSprite::State::BURNING);
	}
	else if (dynamic_cast<Levitation*>(buff)) {
		sprite->remove(CharSprite::State::LEVITATING);
	}
	else if (dynamic_cast<Invisibility*>(buff) && invisible <= 0) {
		sprite->remove(CharSprite::State::INVISIBLE);
	}
	else if (dynamic_cast<BuffParalysis*>(buff)) {
		sprite->remove(CharSprite::State::PARALYSED);
	}
	else if (dynamic_cast<Frost*>(buff)) {
		sprite->remove(CharSprite::State::FROZEN);
	}

	Actor::remove(buff);
	delete buff;
}

void Char::remove(const std::string& buffClass)
{
	std::set<Buff*> re;
	Buffs(buffClass, re);
	for (std::set<Buff*>::iterator itr = re.begin();
		itr != re.end(); itr++)
	{
		remove(*itr);
	}
}

bool Char::attack(Char* enemy)
{
	bool visibleFight = Dungeon::visible[pos] || Dungeon::visible[enemy->pos];

	if (hit(this, enemy, false)) 
	{
	
		if (visibleFight) {
			GLog::i(TXT_HIT.c_str(), name, enemy->name);
		}
	
		// FIXME
		int dr = dynamic_cast<Hero*>(this)  && ((Hero*)this)->rangedWeapon != NULL && ((Hero*)this)->subClass == HeroSubClass::SNIPER ? 0 :
			Random::IntRange(0, enemy->dr());
	
		int dmg = damageRoll();
		int effectiveDamage = std::max(dmg - dr, 0);
	
		effectiveDamage = attackProc(enemy, effectiveDamage);
		effectiveDamage = enemy->defenseProc(this, effectiveDamage);
		enemy->damage(effectiveDamage, this->getClassName());
	
		if (visibleFight) {
			//Sample.INSTANCE.play(Assets.SND_HIT, 1, 1, Random.Float(0.8f, 1.25f));
		}
	
		if (enemy == Dungeon::hero) {
			Dungeon::hero->interrupt();
			if (effectiveDamage > enemy->HT / 4) {
				Camera::mainCamera->shake(GameMath::gate(1, effectiveDamage / (enemy->HT / 4), 5), 0.3f);
			}
		}
	
		enemy->sprite->bloodBurstA(sprite->center(), effectiveDamage);
		enemy->sprite->flash();
	
		if (!enemy->isAlive() && visibleFight) {
			if (enemy == Dungeon::hero) {
	
				if (Dungeon::hero->killerGlyph != NULL) {
	
					// FIXME
					//	Dungeon.fail( Utils.format( ResultDescriptions.GLYPH, Dungeon.hero.killerGlyph.name(), Dungeon.depth ) );
					//	GLog.n( TXT_KILL, Dungeon.hero.killerGlyph.name() );
	
				}
				else {
					if (Bestiary::isBoss(this)) {
						Dungeon::fail(GameMath::format(ResultDescriptions::BOSS.c_str(), name, Dungeon::depth));
					}
					else {
						Dungeon::fail(GameMath::format(ResultDescriptions::MOB.c_str(), name, Dungeon::depth));
					}
	
					GLog::n(TXT_KILL.c_str(), name);
				}
	
			}
			else {
				GLog::i(TXT_DEFEAT.c_str(), name, enemy->name);
			}
		}
	
		return true;
	
	}
	else {
	
		if (visibleFight) {
			String defense = enemy->defenseVerb();
			enemy->sprite->showStatus(CharSprite::NEUTRAL, defense);
			if (this == Dungeon::hero) {
				GLog::i(TXT_YOU_MISSED.c_str(), enemy->name, defense);
			}
			else {
				GLog::i(TXT_SMB_MISSED.c_str(), enemy->name, defense, name);
			}
	
			//Sample.INSTANCE.play(Assets.SND_MISS);
		}
	
		return false;
	
	}
}

bool Char::hit(Char* attacker, Char* defender, bool magic)
{
	float acuRoll = Random::Float(attacker->attackSkill(defender));
	float defRoll = Random::Float(defender->defenseSkill(attacker));
	return (magic ? acuRoll * 2 : acuRoll) >= defRoll;
}

float Char::speed()
{
	return buff("Cripple") == NULL ? baseSpeed : baseSpeed * 0.5f;
}

void Char::damage(int dmg, const std::string& src)
{
	if (HP <= 0) {
		return;
	}

	Buff::detach(this, "Frost");
	
	if (immunities().contains(src)) {
		dmg = 0;
	}
	else if (resistances().contains(src)) {
		dmg = Random::IntRange(0, dmg);
	}
	
	if (buff("BuffParalysis") != NULL) {
		if (Random::Int(dmg) >= Random::Int(HP)) {
			Buff::detach(this, "BuffParalysis");
			if (Dungeon::visible[pos]) {
				GLog::i(TXT_OUT_OF_PARALYSIS.c_str(), name.c_str());
			}
		}
	}
	
	HP -= dmg;
	//if (dmg > 0 || dynamic_cast<Char*>(src)) {
	//	std::stringstream ss;
	//	ss << dmg;
	//	sprite->showStatus(HP > HT / 2 ?
	//		CharSprite::WARNING :
	//		CharSprite::NEGATIVE,
	//		ss.str());
	//}
	if (HP <= 0) {
		die(src);
	}
}

void Char::destroy()
{
	HP = 0;
	Actor::remove(this);
	Actor::freeCell(pos);
}

void Char::die(const std::string& src)
{
	destroy();
	sprite->Die();
}

void Char::Buffs(const std::string& c, std::set<Buff*>& re)
{
	for (std::set<Buff*>::iterator itr = buffs.begin();
		itr != buffs.end(); itr++)
	{
		if ((*itr)->getClassName().compare(c) == 0)
		{
			re.insert(*itr);
		}
	}
}

Buff* Char::buff(const std::string& c)
{
	for (std::set<Buff*>::iterator itr = buffs.begin();
		itr != buffs.end(); itr++)
	{
		if ((*itr)->getClassName().compare(c) == 0)
		{
			return *itr;
		}
	}
	return NULL;
}

std::set<Buff*> Char::buffSet(const std::string& c)
{
	std::set<Buff*> filter;
	for (std::set<Buff*>::iterator itr = buffs.begin();
		itr != buffs.end(); itr++)
	{
		if ((*itr)->getClassName().compare(c) == 0)
		{
			filter.insert(*itr);
		}
	}
	return filter;
}

bool Char::isCharmedBy(Char* ch)
{
	int chID = ch->Id();
	for (std::set<Buff*>::iterator itr = buffs.begin();
		itr != buffs.end(); itr++)
	{
		Buff* b = *itr;
		if (dynamic_cast<Charm*>(b) && ((Charm*)b)->object == chID)
			return true;
	}

	return false;
}

void Char::updateSpriteState()
{
	for (std::set<Buff*>::iterator itr = buffs.begin();
		itr != buffs.end(); itr++)
	{
		Buff* buff = *itr;
		if (dynamic_cast<Burning*>(buff)) {
			sprite->add(CharSprite::State::BURNING);
		}
		else if (dynamic_cast<Levitation*>(buff)) {
			sprite->add(CharSprite::State::LEVITATING);
		}
		else if (dynamic_cast<Invisibility*>(buff)) {
			sprite->add(CharSprite::State::INVISIBLE);
		}
		else if (dynamic_cast<BuffParalysis*>(buff)) {
			sprite->add(CharSprite::State::PARALYSED);
		}
		else if (dynamic_cast<Frost*>(buff)) {
			sprite->add(CharSprite::State::FROZEN);
		}
		else if (dynamic_cast<Light*>(buff)) {
			sprite->add(CharSprite::State::ILLUMINATED);
		}
	}
}

void Char::move(int step)
{
	if (Level::adjacent(step, pos) && buff("Vertigo") != NULL) 
	{
		step = pos + Level::NEIGHBOURS8[Random::Int(8)];
		if (!(Level::passable[step] || Level::avoid[step]) || Actor::findChar(step) != NULL) 
		{
			return;
		}
	}
	
	if (Dungeon::level->map[pos] == Terrain::OPEN_DOOR) 
	{
		Door::leave(pos);
	}
	
	pos = step;
	
	if (flying && Dungeon::level->map[pos] == Terrain::DOOR) 
	{
		Door::enter(pos);
	}
	
	if (this != Dungeon::hero) 
	{
		sprite->visible = Dungeon::visible[pos];
	}
}

int Char::distance(Char* other)
{
	return Level::distance(pos, other->pos);
}
