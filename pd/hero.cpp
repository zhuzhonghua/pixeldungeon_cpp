#include "hero.h"
#include "attackindicator.h"
#include "dungeon.h"
#include "mobsprite.h"
#include "gamescene.h"
#include "heroaction.h"
#include "terrain.h"
#include "ring.h"
#include "checkedcell.h"
#include "glog.h"
#include "speck.h"
#include "belongings.h"
#include "weapon.h"
#include "simpleresource.h"
#include "chasm.h"
#include "mob.h"
#include "heap.h"
#include "dewdrop.h"
#include "scroll.h"
#include "potion.h"
#include "key.h"
#include "interlevelscene.h"
#include "game.h"
#include "wndmessage.h"
#include "surfacescene.h"
#include "alchemypot.h"

const std::string Hero::ATTACK = "attackSkill";
const std::string Hero::DEFENSE = "defenseSkill";
const std::string Hero::STRENGTH = "STR";
const std::string Hero::LEVEL = "lvl";
const std::string Hero::EXPERIENCE = "exp";


const std::string Hero::TXT_LEAVE = BPT::getText("lang.Hero_Leave");// "One does not simply leave Pixel Dungeon.";

const std::string Hero::TXT_LEVEL_UP = BPT::getText("lang.Hero_Level_Up");// "level up!";
const std::string Hero::TXT_NEW_LEVEL = BPT::getText("lang.Hero_New_Level");
//std::string("Welcome to level %d! Now you are healthier and more focused. ") +
//std::string("It's easier for you to hit enemies and dodge their attacks.");

const std::string Hero::TXT_SOMETHING_ELSE = BPT::getText("lang.Hero_Something_Else");// "There is something else here";
const std::string Hero::TXT_LOCKED_CHEST = BPT::getText("lang.Hero_Locked_Chest");// "This chest is locked and you don't have matching key";
const std::string Hero::TXT_LOCKED_DOOR = BPT::getText("lang.Hero_Locked_Door");// "You don't have a matching key";
const std::string Hero::TXT_NOTICED_SMTH = BPT::getText("lang.Hero_Noticed_Smth");// "You noticed something";

const std::string Hero::TXT_WAIT = "...";
const std::string Hero::TXT_SEARCH = BPT::getText("lang.Hero_Search");// "search";
const float Hero::TIME_TO_REST = 1.0f;
const float Hero::TIME_TO_SEARCH = 2.0f;

const std::string Hero::TXT_YOU_NOW_HAVE = BPT::getText("lang.Hero_You_Now_Have");//"You now have %s";

void Hero::preview(GamesInProgress::Info& info, Bundle* bundle)
{
	info.level = bundle->getInt(LEVEL);
}

int Hero::tier()
{
	return belongings->armor == NULL ? 0 : belongings->armor->tier;
}

boolean Hero::shoot(Char* enemy, MissileWeapon* wep)
{
	rangedWeapon = wep;
	boolean result = attack(enemy);
	rangedWeapon = NULL;

	return result;
}

void Hero::live()
{
	Buff::affect(this, "Regeneration");
	Buff::affect(this, "Hunger");
}

String Hero::className()
{
	return subClass == HeroSubClass::NONE ? heroClass.title() : subClass.Title();
}

void Hero::updateAwareness()
{
	awareness = (float)(1 - std::pow(
		(heroClass.type() == HeroClass::E_ROGUE ? 0.85 : 0.90),
		(1 + std::min(lvl, 9)) * 0.5
		));
}

Hero::Hero()
:heroClass(HeroClass::ROGUE)
, subClass(HeroSubClass::NONE)
{
	lvl = 1;
	exp = 0;

	_attackSkill = 10;
	_defenseSkill = 5;

	restoreHealth = false;
	ready = false;

	curAction = NULL;
	lastAction = NULL;

	killerGlyph = NULL;

	rangedWeapon = NULL;

	weakened = false;

	theKey = NULL;
	enemy = NULL;
	killerGlyph = NULL;
}

void Hero::resurrect(int resetLevel)
{
	HP = HT;
	Dungeon::gold = 0;
	exp = 0;

	belongings->resurrect(resetLevel);

	live();
}

void Hero::Ready()
{
	sprite->Idle();
	curAction = NULL;
	ready = true;

	GameScene::ready();
}

bool Hero::actMove(HeroActionMove* action)
{
	if (getCloser(action->dst)) 
	{
		return true;
	}
	else
	{
		if (Dungeon::level->map[pos] == Terrain::SIGN) 
		{
			Sign::read(pos);
		}
		Ready();

		return false;
	}
}

bool Hero::getCloser(int target)
{
	if (rooted) 
	{
		Camera::mainCamera->shake(1, 1.0f);
		return false;
	}

	int step = -1;

	if (Level::adjacent(pos, target)) 
	{
		if (Actor::findChar(target) == NULL) 
		{
			if (Level::pit[target] && !flying && !Chasm::jumpConfirmed) {
				Chasm::heroJump(this);
				interrupt();
				return false;
			}
			if (Level::passable[target] || Level::avoid[target]) 
			{
				step = target;
			}
		}
	}
	else 
	{
		int len = Level::LENGTH;
		std::vector<bool> p = Level::passable;
		std::vector<bool> v = Dungeon::level->visited;
		std::vector<bool> m = Dungeon::level->mapped;
		std::vector<bool> passable(len);
		for (int i = 0; i < len; i++) 
		{
			passable[i] = p[i] && (v[i] || m[i]);
		}

		step = Dungeon::findPath(this, pos, target, passable, Level::fieldOfView);
	}

	if (step != -1) 
	{
		int oldPos = pos;
		move(step);
		sprite->move(oldPos, pos);
		spend(1 / speed());

		return true;
	}
	else 
	{
		return false;
	}
}

bool Hero::act()
{
	Char::act();

	if (paralysed) 
	{
		curAction = NULL;

		spendAndNext(TICK);
		return false;
	}

	checkVisibleMobs();
	AttackIndicator::updateState();

	if (curAction == NULL) 
	{
		if (restoreHealth) {
			if (isStarving() || HP >= HT) {
				restoreHealth = false;
			}
			else {
				spend(TIME_TO_REST); next();
				return false;
			}
		}

		Ready();
		return false;
	}
	else 
	{
		restoreHealth = false;

		ready = false;

		if (dynamic_cast<HeroActionMove*>(curAction) != NULL) 
		{		
			return actMove((HeroActionMove*)curAction);
		}
		else
		if (dynamic_cast<HeroActionInteract*>(curAction)) {

			return actInteract((HeroActionInteract*)curAction);

		}
		else
		if (dynamic_cast<HeroActionBuy*>(curAction)) {

			return actBuy((HeroActionBuy*)curAction);

		}
		else
		if (dynamic_cast<HeroActionPickUp*>(curAction)) {

			return actPickUp((HeroActionPickUp*)curAction);

		}
		else
		if (dynamic_cast<HeroActionOpenChest*>(curAction)) {

			return actOpenChest((HeroActionOpenChest*)curAction);

		}
		else
		if (dynamic_cast<HeroActionUnlock*>(curAction)) {

			return actUnlock((HeroActionUnlock*)curAction);

		}
		else
		if (dynamic_cast<HeroActionDescend*>(curAction)) {

			return actDescend((HeroActionDescend*)curAction);

		}
		else
		if (dynamic_cast<HeroActionAscend*>(curAction)) {

			return actAscend((HeroActionAscend*)curAction);

		}
		else
		if (dynamic_cast<HeroActionAttack*>(curAction)) {

			return actAttack((HeroActionAttack*)curAction);

		}
		else
		if (dynamic_cast<HeroActionCook*>(curAction)) {

			return actCook((HeroActionCook*)curAction);

		}
	}

	return false;
}

bool Hero::handle(int cell)
{
	if (cell == -1) 
	{
		return false;
	}
	
	Char* ch=NULL;
	Heap* heap = NULL;
	
	if (Dungeon::level->map[cell] == Terrain::ALCHEMY && cell != pos) {
	
		curAction = new HeroActionCook(cell);
	
	}
	else if (Level::fieldOfView[cell] && dynamic_cast<Mob*>(ch = Actor::findChar(cell))) {
	
		if (dynamic_cast<NPC*>(ch)) {
			curAction = new HeroActionInteract((NPC*)ch);
		}
		else {
			curAction = new HeroActionAttack(ch);
		}
	
	}
	else if (Level::fieldOfView[cell] && (heap = Dungeon::level->heaps.get(cell)) != null && heap->type != Heap::Type::HIDDEN) {
	
		switch (heap->type) {
		case Heap::Type::HEAP:
			curAction = new HeroActionPickUp(cell);
			break;
		case Heap::Type::FOR_SALE:
			if (heap->size() == 1 && heap->peek()->price() > 0)
			{
				curAction = new HeroActionBuy(cell);
			}
			else
			{
				curAction = new HeroActionPickUp(cell);
			}
			break;
		default:
			curAction = new HeroActionOpenChest(cell);
		}
	
	}
	else if (Dungeon::level->map[cell] == Terrain::LOCKED_DOOR || Dungeon::level->map[cell] == Terrain::LOCKED_EXIT) {
	
		curAction = new HeroActionUnlock(cell);
	
	}
	else if (cell == Dungeon::level->exit) {
	
		curAction = new HeroActionDescend(cell);
	
	}
	else if (cell == Dungeon::level->entrance) {
	
		curAction = new HeroActionAscend(cell);
	
	}
	else  {
	
	curAction = new HeroActionMove(cell);
	lastAction = NULL;
	
	}
	
	return act();
}

int Hero::VisibleEnemies()
{
	return visibleEnemies.size();
}

Mob* Hero::visibleEnemy(int index)
{
	int count = 0;
	int idx = index % visibleEnemies.size();
	for (std::list<Mob*>::iterator itr = visibleEnemies.begin();
		itr != visibleEnemies.end(); itr++)
	{
		if (count == idx) return *itr;
		count++;
	}
	return NULL;
}

void Hero::resume()
{
	curAction = lastAction;
	lastAction = null;
	act();
}

void Hero::spendAndNext(float time)
{
	busy();
	spend(time);
	next();
}

void Hero::move(int step)
{
	Char::move(step);

	if (!flying) 
	{
		if (Level::water[pos]) 
		{
			//Sample.INSTANCE.play(Assets.SND_WATER, 1, 1, Random.Float(0.8f, 1.25f));
		}
		else 
		{
			//Sample.INSTANCE.play(Assets.SND_STEP);
		}
		Dungeon::level->press(pos, this);
	}
}

void Hero::interrupt()
{
	if (isAlive() && curAction != NULL && curAction->dst != pos) 
	{
		lastAction = curAction;
	}
	curAction = NULL;
}

bool Hero::search(bool intentional)
{
	bool smthFound = false;

	int positive = 0;
	int negative = 0;
	std::set<Buff*> result = buffSet(RingOfDetection::Detection::className());
	//for (Buff buff : buffs(RingOfDetection.Detection.class)) {
	for (std::set<Buff*>::iterator itr = result.begin(); itr != result.end(); itr++)
	{
		Buff* buff = *itr;
		int bonus = ((RingOfDetection::Detection*)buff)->level;
		if (bonus > positive) 
		{
			positive = bonus;
		}
		else if (bonus < 0) 
		{
			negative += bonus;
		}
	}
	int distance = 1 + positive + negative;

	float level = intentional ? (2 * awareness - awareness * awareness) : awareness;
	if (distance <= 0) 
	{
		level /= 2 - distance;
		distance = 1;
	}

	int cx = pos % Level::WIDTH;
	int cy = pos / Level::WIDTH;
	int ax = cx - distance;
	if (ax < 0) 
	{
		ax = 0;
	}
	int bx = cx + distance;
	if (bx >= Level::WIDTH) 
	{
		bx = Level::WIDTH - 1;
	}
	int ay = cy - distance;
	if (ay < 0) {
		ay = 0;
	}
	int by = cy + distance;
	if (by >= Level::HEIGHT) 
	{
		by = Level::HEIGHT - 1;
	}

	for (int y = ay; y <= by; y++) 
	{
		for (int x = ax, p = ax + y * Level::WIDTH; x <= bx; x++, p++) 
		{
			if (Dungeon::visible[p]) 
			{
				if (intentional) 
				{
					sprite->parent->addToBack(new CheckedCell(p));
				}

				if (Level::secret[p] && (intentional || Random::Float() < level))
				{
					int oldValue = Dungeon::level->map[p];

					GameScene::discoverTile(p, oldValue);

					Level::set(p, Terrain::discover(oldValue));

					GameScene::updateMap(p);

					ScrollOfMagicMapping::discover(p);

					smthFound = true;
				}

				if (intentional) 
				{
					Heap* heap = Dungeon::level->heaps.get(p);
					if (heap != null && heap->type == Heap::Type::HIDDEN) {
						heap->open(this);
						smthFound = true;
					}
				}
			}
		}
	}


	if (intentional) 
	{
		sprite->showStatus(CharSprite::DEFAULT, TXT_SEARCH);
		sprite->Operate(pos);
		if (smthFound) 
		{
			spendAndNext(Random::Float() < level ? TIME_TO_SEARCH : TIME_TO_SEARCH * 2);
		}
		else 
		{
			spendAndNext(TIME_TO_SEARCH);
		}

	}

	if (smthFound) {
		GLog::w(TXT_NOTICED_SMTH.c_str());
		//Sample.INSTANCE.play(Assets.SND_SECRET);
		interrupt();
	}

	return smthFound;
}

void Hero::earnExp(int exp)
{
	this->exp += exp;

	bool levelUp = false;
	while (this->exp >= maxExp()) {
		this->exp -= maxExp();
		lvl++;

		HT += 5;
		HP += 5;
		_attackSkill++;
		_defenseSkill++;

		if (lvl < 10) {
			updateAwareness();
		}

		levelUp = true;
	}

	if (levelUp) {

		GLog::p(TXT_NEW_LEVEL.c_str(), lvl);
		sprite->showStatus(CharSprite::POSITIVE, TXT_LEVEL_UP);
		//Sample.INSTANCE.play(Assets.SND_LEVELUP);

		Badges::validateLevelReached();
	}

	if (subClass == HeroSubClass::WARLOCK) {

		int value = std::min(HT - HP, 1 + (Dungeon::depth - 1) / 5);
		if (value > 0) {
			HP += value;
			sprite->emitter()->burst(Speck::factory(Speck::HEALING), 1);
		}

		((Hunger*)buff("Hunger"))->satisfy(10);
	}
}

void Hero::rest(boolean tillHealthy)
{
	spendAndNext(TIME_TO_REST);
	if (!tillHealthy) {
		sprite->showStatus(CharSprite::DEFAULT, TXT_WAIT);
	}
	restoreHealth = tillHealthy;
}

void Hero::spend(float time)
{
	int hasteLevel = 0;
	std::set<Buff*> re;
	Buffs("RingOfHaste::Haste", re);
	for (std::set<Buff*>::iterator itr = re.begin();
		itr != re.end(); itr++){
	//for (Buff buff : buffs(RingOfHaste.Haste.class)) {
		Buff* buff = *itr;
		hasteLevel += ((RingOfHaste::Haste*)buff)->level;
	}
	Char::spend(hasteLevel == 0 ? time : (float)(time * std::pow(1.1, -hasteLevel)));
}

int Hero::attackSkill(Char* target)
{
	int bonus = 0;
	std::set<Buff*> bfs = buffSet("Accuracy");
	for (std::set<Buff*>::iterator itr = bfs.begin();
		itr != bfs.end(); itr++)
	{
		Buff* buff = *itr;
		bonus += ((RingOfAccuracy::Accuracy*)buff)->level;
	}

	float accuracy = (bonus == 0) ? 1 : (float)std::pow(1.4, bonus);
	if (rangedWeapon != NULL && Level::distance(pos, target->pos) == 1) {
		accuracy *= 0.5f;
	}

	KindOfWeapon* wep = rangedWeapon != NULL ? rangedWeapon : belongings->weapon;
	if (wep != NULL) {
		return (int)(_attackSkill * accuracy * wep->acuracyFactor(this));
	}
	else {
		return (int)(_attackSkill * accuracy);
	}
}

boolean Hero::isStarving()
{
	return ((Hunger*)buff("Hunger"))->isStarving();
}

void Hero::checkVisibleMobs()
{
	ArrayList<Mob*> visible;

	boolean newMob = false;

	for (HashSet<Mob*>::iterator itr = Dungeon::level->mobs.begin();
		itr != Dungeon::level->mobs.end(); itr++)
	{
		Mob* m = *itr;
		if (Level::fieldOfView[m->pos] && m->hostile) {
			visible.add(m);
			if (!visibleEnemies.contains(m)) {
				newMob = true;
			}
		}
	}

	if (newMob) {
		interrupt();
		restoreHealth = false;
	}

	visibleEnemies = visible;
}

boolean Hero::actInteract(HeroActionInteract* action)
{
	NPC* npc = action->npc;

	if (Level::adjacent(pos, npc->pos)) {

		Ready();
		sprite->turnTo(pos, npc->pos);
		npc->interact();
		return false;

	}
	else {

		if (Level::fieldOfView[npc->pos] && getCloser(npc->pos)) {

			return true;

		}
		else {
			Ready();
			return false;
		}

	}
}

boolean Hero::actBuy(HeroActionBuy* action)
{
	int dst = action->dst;
	if (pos == dst || Level::adjacent(pos, dst)) {

		Ready();

		Heap* heap = Dungeon::level->heaps.get(dst);
		if (heap != NULL && heap->type == Heap::Type::FOR_SALE && heap->size() == 1) {
			GameScene::show(new WndTradeItem(heap, true));
		}

		return false;

	}
	else if (getCloser(dst)) {

		return true;

	}
	else {
		Ready();
		return false;
	}
}

boolean Hero::actPickUp(HeroActionPickUp* action)
{
	int dst = action->dst;
	if (pos == dst) {

		Heap* heap = Dungeon::level->heaps.get(pos);
		if (heap != NULL) {
			Item* item = heap->pickUp();
			if (item->doPickUp(this)) {

				if (dynamic_cast<Dewdrop*>(item)) {
					// Do nothing
				}
				else {
					boolean important =
						((dynamic_cast<ScrollOfUpgrade*>(item) != NULL || dynamic_cast<ScrollOfEnchantment*>(item) != NULL) && ((Scroll*)item)->isKnown()) ||
						((dynamic_cast<PotionOfStrength*>(item) != NULL || dynamic_cast<PotionOfMight*>(item) != NULL) && ((Potion*)item)->isKnown());
					if (important) {
						GLog::p(TXT_YOU_NOW_HAVE.c_str(), item->Name());
					}
					else {
						GLog::i(TXT_YOU_NOW_HAVE.c_str(), item->Name());
					}
				}

				if (!heap->isEmpty()) {
					GLog::i(TXT_SOMETHING_ELSE.c_str());
				}
				curAction = NULL;
			}
			else {
				Dungeon::level->drop(item, pos)->sprite->drop();
				Ready();
			}
		}
		else {
			Ready();
		}

		return false;

	}
	else if (getCloser(dst)) {

		return true;

	}
	else {
		Ready();
		return false;
	}
}

boolean Hero::actOpenChest(HeroActionOpenChest* action)
{
	int dst = action->dst;
	if (Level::adjacent(pos, dst) || pos == dst) {

		Heap* heap = Dungeon::level->heaps.get(dst);
		if (heap != NULL && (heap->type != Heap::Type::HEAP && heap->type != Heap::Type::FOR_SALE)) {

			theKey = NULL;

			if (heap->type == Heap::Type::LOCKED_CHEST || heap->type == Heap::Type::CRYSTAL_CHEST) {

				theKey = belongings->getKey("GoldenKey", Dungeon::depth);

				if (theKey == NULL) {
					GLog::w(TXT_LOCKED_CHEST.c_str());
					Ready();
					return false;
				}
			}

			switch (heap->type) {
			case Heap::Type::TOMB:
				//Sample.INSTANCE.play(Assets.SND_TOMB);
				Camera::mainCamera->shake(1, 0.5f);
				break;
			case Heap::Type::SKELETON:
				break;
			default:
				//Sample.INSTANCE.play(Assets.SND_UNLOCK);
				break;
			}

			spend(Key::TIME_TO_UNLOCK);
			sprite->Operate(dst);

		}
		else {
			Ready();
		}

		return false;

	}
	else if (getCloser(dst)) {

		return true;

	}
	else {
		Ready();
		return false;
	}
}

boolean Hero::actUnlock(HeroActionUnlock* action)
{
	int doorCell = action->dst;
	if (Level::adjacent(pos, doorCell)) {

		theKey = NULL;
		int door = Dungeon::level->map[doorCell];

		if (door == Terrain::LOCKED_DOOR) {

			theKey = belongings->getKey("IronKey", Dungeon::depth);

		}
		else if (door == Terrain::LOCKED_EXIT) {

			theKey = belongings->getKey("SkeletonKey", Dungeon::depth);

		}

		if (theKey != null) {

			spend(Key::TIME_TO_UNLOCK);
			sprite->Operate(doorCell);

			//Sample.INSTANCE.play(Assets.SND_UNLOCK);

		}
		else {
			GLog::w(TXT_LOCKED_DOOR.c_str());
			Ready();
		}

		return false;

	}
	else if (getCloser(doorCell)) {

		return true;

	}
	else {
		Ready();
		return false;
	}
}

boolean Hero::actDescend(HeroActionDescend* action)
{
	int stairs = action->dst;
	if (pos == stairs && pos == Dungeon::level->exit) {

		curAction = null;

		Hunger* hunger = (Hunger*)buff("Hunger");
		if (hunger != null && !hunger->isStarving()) {
			hunger->satisfy(-Hunger::STARVING / 10);
		}

		InterlevelScene::mode = InterlevelScene::Mode::DESCEND;
		Game::switchScene(new InterlevelScene());

		return false;

	}
	else if (getCloser(stairs)) {

		return true;

	}
	else {
		Ready();
		return false;
	}
}

boolean Hero::actAscend(HeroActionAscend* action)
{
	int stairs = action->dst;
	if (pos == stairs && pos == Dungeon::level->entrance) {

		if (Dungeon::depth == 1) {

			if (belongings->getItem("Amulet") == null) {
				GameScene::show(new WndMessage(TXT_LEAVE));
				Ready();
			}
			else {
				Dungeon::win(ResultDescriptions::WIN);
				Dungeon::deleteGame(Dungeon::hero->heroClass, true);
				Game::switchScene(new SurfaceScene);
			}

		}
		else {

			curAction = null;

			Hunger* hunger = (Hunger*)buff("Hunger");
			if (hunger != null && !hunger->isStarving()) {
				hunger->satisfy(-Hunger::STARVING / 10);
			}

			InterlevelScene::mode = InterlevelScene::Mode::ASCEND;
			Game::switchScene(new InterlevelScene);
		}

		return false;

	}
	else if (getCloser(stairs)) {

		return true;

	}
	else {
		Ready();
		return false;
	}
}

boolean Hero::actAttack(HeroActionAttack* action)
{
	enemy = action->target;

	if (Level::adjacent(pos, enemy->pos) && enemy->isAlive() && !isCharmedBy(enemy)) {

		spend(attackDelay());
		sprite->Attack(enemy->pos);

		return false;

	}
	else {

		if (Level::fieldOfView[enemy->pos] && getCloser(enemy->pos)) {

			return true;

		}
		else {
			Ready();
			return false;
		}

	}
}

float Hero::attackDelay()
{
	KindOfWeapon* wep = rangedWeapon != null ? rangedWeapon : belongings->weapon;
	if (wep != null) {

		return wep->speedFactor(this);

	}
	else {
		return 1.0f;
	}
}

boolean Hero::actCook(HeroActionCook* action)
{
	int dst = action->dst;
	if (Dungeon::visible[dst]) {

		Ready();
		AlchemyPot::operate(this, dst);
		return false;

	}
	else if (getCloser(dst)) {

		return true;

	}
	else {
		Ready();
		return false;
	}
}
