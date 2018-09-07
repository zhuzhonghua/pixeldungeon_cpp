#include "mob.h"
#include "util.h"
#include "dungeon.h"
#include "charsprite.h"
#include "mobsprite.h"
#include "wound.h"
#include "glog.h"
#include "statistics.h"
#include "heap.h"
#include "cellemitter.h"
#include "gamescene.h"
#include "flare.h"
#include "hero.h"

FACTORYIMPL(Mob);

Mob::Mob()
{
	target = -1;
	_defenseSkill = 0;
	EXP = 1;
	maxLvl = 30;

	enemy = NULL;
	enemySeen = false;
	alerted = false;

	hostile = true;

	SLEEPEING = new Sleeping(this);
	HUNTING = new Hunting(this);
	WANDERING = new Wandering(this);
	FLEEING = new Fleeing(this);
	PASSIVE = new Passive(this);
	state = SLEEPEING;

	lootChance = 0;
}

void Mob::storeInBundle(Bundle* bundle)
{
	Char::storeInBundle(bundle);

	if (state == SLEEPEING) 
	{
		bundle->put(STATE, Sleeping::TAG);
	}
	else if (state == WANDERING) 
	{
		bundle->put(STATE, Wandering::TAG);
	}
	else if (state == HUNTING) 
	{
		bundle->put(STATE, Hunting::TAG);
	}
	else if (state == FLEEING) 
	{
		bundle->put(STATE, Fleeing::TAG);
	}
	else if (state == PASSIVE) 
	{
		bundle->put(STATE, Passive::TAG);
	}
	bundle->put(TARGET, target);
}

void Mob::restoreFromBundle(Bundle* bundle)
{
	Char::restoreFromBundle(bundle);

	std::string state = bundle->getString(STATE);
	if (state.compare(Sleeping::TAG) == 0) 
	{
		this->state = SLEEPEING;
	}
	else if (state.compare(Wandering::TAG) == 0) 
	{
		this->state = WANDERING;
	}
	else if (state.compare(Hunting::TAG) == 0) 
	{
		this->state = HUNTING;
	}
	else if (state.compare(Fleeing::TAG) == 0) 
	{
		this->state = FLEEING;
	}
	else if (state.compare(Passive::TAG) == 0) 
	{
		this->state = PASSIVE;
	}

	target = bundle->getInt(TARGET);
}

void Mob::add(Buff* buff)
{
	Char::add(buff);
	if (dynamic_cast<Amok*>(buff)) {
		if (sprite != NULL) {
			sprite->showStatus(CharSprite::NEGATIVE, TXT_RAGE);
		}
		state = HUNTING;
	}
	else if (dynamic_cast<Terror*>(buff)) {
		state = FLEEING;
	}
	else if (dynamic_cast<Sleep*>(buff)) {
		if (sprite != NULL) {
			// TODO: MemoryLeak
			Flare* f = new Flare(4, 32);
			f->color(0x44ffff, true);
			f->show(sprite, 2.0f);
		}
		state = SLEEPEING;
		postpone(Sleep::SWS);
	}
}

void Mob::remove(Buff* buff)
{
	Char::remove(buff);
	if (dynamic_cast<Terror*>(buff)) {
		sprite->showStatus(CharSprite::NEGATIVE, TXT_RAGE);
		state = HUNTING;
	}
}

void Mob::move(int step)
{
	Char::move(step);

	if (!flying) 
	{
		Dungeon::level->mobPress(this);
	}
}

void Mob::onAttackComplete()
{
	attack(enemy);
	Char::onAttackComplete();
}

int Mob::defenseSkill(Char* enemy)
{
	return (enemySeen && !paralysed) ? _defenseSkill : 0;
}

int Mob::defenseProc(Char* enemy, int damage)
{
	if (!enemySeen && enemy == Dungeon::hero && ((Hero*)enemy)->subClass == HeroSubClass::ASSASSIN) 
	{
		damage += Random::Int(1, damage);
		Wound::hit(this);
	}
	return damage;
}

void Mob::notice()
{
	sprite->showAlert();
}

void Mob::beckon(int cell)
{
	notice();

	if (state != HUNTING) 
	{
		state = WANDERING;
	}
	target = cell;
}

void Mob::yell(const std::string& str)
{
	GLog::n("%s: \"%s\" ", name, str.c_str());
}

void Mob::destroy()
{
	Char::destroy();

	Dungeon::level->mobs.remove(this);

	if (Dungeon::hero->isAlive()) {

		if (hostile) {
			Statistics::enemiesSlain++;
			Badges::validateMonstersSlain();
			Statistics::qualifiedForNoKilling = false;

			if (Dungeon::nightMode) {
				Statistics::nightHunt++;
			}
			else {
				Statistics::nightHunt = 0;
			}
			Badges::validateNightHunter();
		}

		int exp = this->exp();
		if (exp > 0) {
			Dungeon::hero->sprite->showStatus(CharSprite::POSITIVE, TXT_EXP, exp);
			Dungeon::hero->earnExp(exp);
		}
	}

	delete this;
}

int Mob::exp()
{
	return Dungeon::hero->lvl <= maxLvl ? EXP : 0;
}

void Mob::die(const std::string& cause)
{
	Char::die(cause);

	if (Dungeon::hero->lvl <= maxLvl + 2) {
		//dropLoot();
	}

	if (Dungeon::hero->isAlive() && !Dungeon::visible[pos]) {
		GLog::i(TXT_DIED.c_str());
	}
}

const String Mob::TXT_DIED = BPT::getText("lang.mob_died");// "You hear something died in the distance";

const std::string Mob::STATE = "state";
const std::string Mob::TARGET = "target";

const String Mob::TXT_ECHO = "echo of ";
const String Mob::TXT_NOTICE1 = "?!";
const String Mob::TXT_RAGE = "#$%^";
const String Mob::TXT_EXP = "%+dEXP";

const float Mob::TIME_TO_WAKE_UP = 1.0f;

bool Mob::act()
{
	Char::act();

	bool justAlerted = alerted;
	alerted = false;

	sprite->hideAlert();

	if (paralysed) 
	{
		enemySeen = false;
		spend(TICK);
		return true;
	}

	enemy = chooseEnemy();

	bool enemyInFOV =
		enemy != NULL && enemy->isAlive() &&
		Level::fieldOfView[enemy->pos] && enemy->invisible <= 0;

	return state->act(enemyInFOV, justAlerted);
}

Char* Mob::chooseEnemy()
{
	if (buff("Amok") != NULL) {
		if (enemy == Dungeon::hero || enemy == NULL) {

			std::set<Mob*> enemies;
			for (HashSet<Mob*>::iterator itr = Dungeon::level->mobs.begin();
				itr != Dungeon::level->mobs.end(); itr++)
			{
				Mob* mob = *itr;
				if (mob != this && Level::fieldOfView[mob->pos]) {
					enemies.insert(mob);
				}
			}

			if (enemies.size() > 0) {
				return RandomT<Mob*>::element(enemies);
			}

		}
	}

	Terror* terror = (Terror*)buff("Terror");
	if (terror != NULL) {
		Char* source = (Char*)Actor::findById(terror->object);
		if (source != NULL) {
			return source;
		}
	}

	return enemy != NULL && enemy->isAlive() ? enemy : Dungeon::hero;
}

bool Mob::moveSprite(int from, int to)
{
	if (sprite->isVisible() && (Dungeon::visible[from] || Dungeon::visible[to])) 
	{
		sprite->move(from, to);
		return true;
	}
	else 
	{
		sprite->place(to);
		return true;
	}
}

bool Mob::canAttack(Char* enemy)
{
	return Level::adjacent(pos, enemy->pos) && !isCharmedBy(enemy);
}

bool Mob::getCloser(int target)
{
	if (rooted) 
	{
		return false;
	}

	int step = Dungeon::findPath(this, pos, target,
		Level::passable,
		Level::fieldOfView);
	if (step != -1) 
	{
		move(step);
		return true;
	}
	else 
	{
		return false;
	}
}

bool Mob::getFurther(int target)
{
	int step = Dungeon::flee(this, pos, target,
		Level::passable,
		Level::fieldOfView);
	if (step != -1) 
	{
		move(step);
		return true;
	}
	else 
	{
		return false;
	}
}

bool Mob::doAttack(Char* enemy)
{
	bool visible = Dungeon::visible[pos];

	if (visible) 
	{
		sprite->Attack(enemy->pos);
	}
	else 
	{
		attack(enemy);
	}

	spend(attackDelay());

	return !visible;
}

void Mob::damage(int dmg, const std::string& src)
{
	Terror::recover(this);

	if (state == SLEEPEING) {
		state = WANDERING;
	}
	alerted = true;

	Char::damage(dmg, src);
}

const std::string Mob::Fleeing::TAG = "FLEEING";

bool Mob::Fleeing::act(bool enemyInFOV, bool justAlerted)
{
	owner->enemySeen = enemyInFOV;
	if (enemyInFOV) 
	{
		owner->target = owner->enemy->pos;
	}

	int oldPos = owner->pos;
	if (owner->target != -1 && owner->getFurther(owner->target)) 
	{
		owner->spend(1 / owner->speed());
		return owner->moveSprite(oldPos, owner->pos);
	}
	else 
	{
		owner->spend(TICK);
		nowhereToRun();

		return true;
	}
}

std::string Mob::Fleeing::status()
{
	return GameMath::format(BPT::getText("lang.mob_flee_status").c_str(), owner->name);
}

const std::string Mob::Sleeping::TAG = "SLEEPING";

bool Mob::Sleeping::act(bool enemyInFOV, bool justAlerted)
{
	if (enemyInFOV && Random::Int(owner->distance(owner->enemy) + owner->enemy->stealth() + (owner->enemy->flying ? 2 : 0)) == 0)
	{
		owner->enemySeen = true;
	
		owner->notice();
		owner->state = owner->HUNTING;
		owner->target = owner->enemy->pos;
	
		if (Dungeon::isChallenged(Challenges::SWARM_INTELLIGENCE)) 
		{
			for (std::set<Mob*>::iterator itr = Dungeon::level->mobs.begin();
				itr != Dungeon::level->mobs.end(); itr++)
			{
				Mob* mob = *itr;
				if (mob != owner)
				{
					mob->beckon(owner->target);
				}
			}
		}
	
		owner->spend(TIME_TO_WAKE_UP);
	}
	else 
	{
		owner->enemySeen = false;
	
		owner->spend(TICK);
	}
	return true;
}

std::string Mob::Sleeping::status()
{
	return GameMath::format(BPT::getText("lang.mob_sleeping_status").c_str(), owner->name);
}


const std::string Mob::Wandering::TAG = "WANDERING";

bool Mob::Wandering::act(bool enemyInFOV, bool justAlerted)
{
	if (enemyInFOV && (justAlerted || Random::Int(owner->distance(owner->enemy) / 2 + owner->enemy->stealth()) == 0)) 
	{
		owner->enemySeen = true;

		owner->notice();
		owner->state = owner->HUNTING;
		owner->target = owner->enemy->pos;

	}
	else 
	{
		owner->enemySeen = false;

		int oldPos = owner->pos;
		if (owner->target != -1 && owner->getCloser(owner->target)) 
		{
			owner->spend(1 / owner->speed());
			return owner->moveSprite(oldPos, owner->pos);
		}
		else 
		{
			owner->target = Dungeon::level->randomDestination();
			owner->spend(TICK);
		}

	}
	return true;
}

std::string Mob::Wandering::status()
{
	return GameMath::format(BPT::getText("lang.mob_wandering_status").c_str(), owner->name);
}

const std::string Mob::Hunting::TAG = "HUNTING";

bool Mob::Hunting::act(bool enemyInFOV, bool justAlerted)
{
	owner->enemySeen = enemyInFOV;
	if (enemyInFOV && owner->canAttack(owner->enemy)) 
	{
		return owner->doAttack(owner->enemy);
	}
	else 
	{
		if (enemyInFOV) 
		{
			owner->target = owner->enemy->pos;
		}

		int oldPos = owner->pos;
		if (owner->target != -1 && owner->getCloser(owner->target)) 
		{
			owner->spend(1 / owner->speed());
			return owner->moveSprite(oldPos, owner->pos);
		}
		else 
		{
			owner->spend(TICK);
			owner->state = owner->WANDERING;
			owner->target = Dungeon::level->randomDestination();
			return true;
		}
	}
}

std::string Mob::Hunting::status()
{
	return GameMath::format(BPT::getText("lang.mob_huning_status").c_str(), owner->name);
}

const std::string Mob::Passive::TAG = "PASSIVE";

bool Mob::Passive::act(bool enemyInFOV, bool justAlerted)
{
	owner->enemySeen = false;
	owner->spend(TICK);
	return true;
}

std::string Mob::Passive::status()
{
	return GameMath::format(BPT::getText("lang.mob_passive_status").c_str(), owner->name);
}

NPC::NPC()
{
	HP = HT = 1;
	EXP = 0;

	hostile = false;
	state = PASSIVE;
}

void NPC::throwItem()
{
	Heap* heap = Dungeon::level->heaps.get(pos);
	if (heap != NULL) {
		int n;
		do {
			n = pos + Level::NEIGHBOURS8[Random::Int(8)];
		} while (!Level::passable[n] && !Level::avoid[n]);
		Dungeon::level->drop(heap->pickUp(), n)->sprite->drop(pos);
	}
}

WndBag::Listener* Shopkeeper::itemSelector;

Shopkeeper::Shopkeeper()
{
	name = "shopkeeper";
	//spriteClass = ShopkeeperSprite.class;
}

CharSprite* Shopkeeper::Sprite()
{
	return new ShopkeeperSprite();
}

boolean Shopkeeper::act()
{
	throwItem();

	sprite->turnTo(pos, Dungeon::hero->pos);
	spend(TICK);
	return true;
}

void Shopkeeper::flee()
{
	std::list<Heap*> re = Dungeon::level->heaps.values();

	for (std::list<Heap*>::iterator itr = re.begin();
		itr != re.end(); itr++)
	{
		Heap* heap = *itr;
		if (heap->type == Heap::Type::FOR_SALE) {
			CellEmitter::get(heap->pos)->burst(ElmoParticle::FACTORY, 4);
			heap->destroy();
		}
	}

	sprite->killAndErase();
	delete sprite;
	sprite = NULL;

	CellEmitter::get(pos)->burst(ElmoParticle::FACTORY, 6);

	destroy();
}

WndBag* Shopkeeper::sell()
{
	return GameScene::selectItem(itemSelector, WndBag::Mode::FOR_SALE, "Select an item to sell");
}

REFLECMOB(Shopkeeper);

namespace{
	class WndBagListenerNew :public WndBag::Listener{
	public:
		virtual void onSelect(Item* item){
			if (item != NULL){
				WndBag* parentWnd = Shopkeeper::sell();
				GameScene::show(new WndTradeItem(item, parentWnd));
			}
		}
	};
}
WndBag::Listener* itemSelector = new WndBagListenerNew();

MirrorImage::MirrorImage()
{
	name = "mirror image";
	//spriteClass = MirrorSprite.class;

	state = HUNTING;
}

CharSprite* MirrorImage::Sprite()
{
	MirrorSprite* ms = new MirrorSprite();
	ms->updateArmor(tier);
	return ms;
}

void MirrorImage::storeInBundle(Bundle* bundle)
{
	NPC::storeInBundle(bundle);
	bundle->put(TIER, tier);
	bundle->put(ATTACK, attack);
	bundle->put(DAMAGE, damage);
}

void MirrorImage::restoreFromBundle(Bundle* bundle)
{
	NPC::restoreFromBundle(bundle);
	tier = bundle->getInt(TIER);
	attack = bundle->getInt(ATTACK);
	damage = bundle->getInt(DAMAGE);
}

void MirrorImage::duplicate(Hero* hero)
{
	tier = hero->tier();
	attack = hero->attackSkill(hero);
	damage = hero->damageRoll();
}

int MirrorImage::attackProc(Char* enemy, int damage)
{
	int dmg = NPC::attackProc(enemy, damage);

	destroy();
	sprite->Die();

	return dmg;
}

Char* MirrorImage::chooseEnemy()
{
	if (enemy == NULL || !enemy->isAlive()) {
		std::set<Mob*> enemies;
		for (HashSet<Mob*>::iterator itr = Dungeon::level->mobs.begin();
			itr != Dungeon::level->mobs.end(); itr++) {
			Mob* mob = *itr;
			if (mob->hostile && Level::fieldOfView[mob->pos]) {
				enemies.insert(mob);
			}
		}

		return enemies.size() > 0 ? RandomT<Mob*>::element(enemies) : NULL;
	}

	return enemy;
}

void MirrorImage::interact()
{
	int curPos = pos;

	moveSprite(pos, Dungeon::hero->pos);
	move(Dungeon::hero->pos);

	Dungeon::hero->sprite->move(Dungeon::hero->pos, curPos);
	Dungeon::hero->move(curPos);

	Dungeon::hero->spend(1 / Dungeon::hero->speed());
	Dungeon::hero->busy();
}

const String MirrorImage::DAMAGE = "damage";
const String MirrorImage::ATTACK = "attack";
const String MirrorImage::TIER = "tier";
