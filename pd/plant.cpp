#include "plant.h"
#include "hero.h"
#include "dungeon.h"
#include "level.h"
#include "plantsprite.h"
#include "util.h"
#include "generator.h"
#include "terrain.h"
#include "cellemitter.h"
#include "flameparticle.h"
#include "belongings.h"
#include "charsprite.h"
#include "simpleresource.h"
#include "heap.h"
#include "dewdrop.h"
#include "pathfinder.h"
#include "speck.h"
#include "buffindicator.h"
#include "camera.h"
#include "scroll.h"
#include "mob.h"
#include "glog.h"
#include "blob.h"
#include "gamescene.h"
#include "buffpoison.h"

void Plant::activate(Char* ch)
{
	if (dynamic_cast<Hero*>(ch) != NULL && ((Hero*)ch)->subClass == HeroSubClass::WARDEN) 
	{
		((Barkskin*)Buff::affect(ch, "Barkskin"))->Level(ch->HT / 3);
	}

	wither();
}

void Plant::wither()
{
	Dungeon::level->uproot(pos);

	sprite->kill();
	if (Dungeon::visible[pos]) 
	{
		CellEmitter::get(pos)->burst(LeafParticle::general(), 6);
	}

	if (Dungeon::hero->subClass == HeroSubClass::WARDEN) 
	{
		if (Random::Int(5) == 0) 
		{
			Dungeon::level->drop(Generator::random(Generator::Category::SEED), pos)->sprite->drop();
		}
		if (Random::Int(5) == 0) 
		{
			Dungeon::level->drop(new Dewdrop(), pos)->sprite->drop();
		}
	}
}

void Plant::restoreFromBundle(Bundle* bundle)
{
	pos = bundle->getInt(POS);
}

void Plant::storeInBundle(Bundle* bundle)
{
	bundle->put(POS, pos);
}

const std::string Plant::POS = "pos";

const std::string Plant::Seed::AC_PLANT = "PLANT";

Plant::Seed::Seed()
{
	stackable = true;
	defaultAction = AC_THROW;
}

void Plant::Seed::actions(Hero* hero, std::vector<std::string>& actions)
{
	Item::actions(hero, actions);
	actions.push_back(AC_PLANT);
}

void Plant::Seed::execute(Hero* hero, std::string action)
{
	if (action.compare(AC_PLANT) == 0) 
	{
		hero->spend(TIME_TO_PLANT);
		hero->busy();
		((Seed*)detach(hero->belongings->backpack))->onThrow(hero->pos);

		hero->sprite->Operate(hero->pos);
	}
	else 
	{
		Item::execute(hero, action);
	}
}

Plant* Plant::Seed::couch(int pos)
{
	if (Dungeon::visible[pos]) {
		//Sample.INSTANCE.play(Assets.SND_PLANT);
	}
	Plant* plant = plantClass();
	plant->pos = pos;
	return plant;
	return NULL;
}

std::string Plant::Seed::info()
{
	return GameMath::format(TXT_INFO.c_str(), plantName.c_str(), desc());
}

void Plant::Seed::onThrow(int cell)
{
	if (Dungeon::level->map[cell] == Terrain::ALCHEMY || Level::pit[cell]) 
	{
		Item::onThrow(cell);
	}
	else 
	{
		Dungeon::level->plant(this, cell);
	}
}

const std::string Plant::Seed::TXT_INFO = BPT::getText("lang.Plant_Seed_Info");// "Throw this seed to the place where you want to grow %s.\n\n%s";

const float Plant::Seed::TIME_TO_PLANT = 1.0f;

const String Firebloom::TXT_DESC = BPT::getText("lang.Firebloom_Desc");// "When something touches a Firebloom, it bursts into flames.";
Firebloom::Firebloom()
{
	image = 0;
	plantName = "Firebloom";
}

void Firebloom::activate(Char* ch)
{
	Plant::activate(ch);

	GameScene::addBlob(Blob::seed(pos, 2, "BlobFire"));

	if (Dungeon::visible[pos]) {
		CellEmitter::get(pos)->burst(FlameParticle::FACTORY, 5);
	}
}

Firebloom::Seed::Seed()
{
	plantName = "Firebloom";

	name = "seed of " + plantName;
	image = ItemSpriteSheet::SEED_FIREBLOOM;

	plantClass = Firebloom::Create;
	alchemyClass = "PotionOfLiquidFlame";
}

const String Icecap::TXT_DESC = BPT::getText("lang.Icecap_Desc");// "Upon touching an Icecap excretes a pollen, which freezes everything in its vicinity.";

Icecap::Icecap()
{
	image = 1;
	plantName = "Icecap";
}

void Icecap::activate(Char* ch)
{
	Plant::activate(ch);

	PathFinder::buildDistanceMap(pos, BArray::not(Level::losBlocking, std::vector<bool>()), 1);

	BlobFire* fire = (BlobFire*)Dungeon::level->blobs.get("BlobFire");

	for (int i = 0; i < Level::LENGTH; i++) {
		if (PathFinder::distance[i] < std::numeric_limits<int>::max()) {
			Freezing::affect(i, fire);
		}
	}
}

Icecap::Seed::Seed()
{
	plantName = "Icecap";

	name = "seed of " + plantName;
	image = ItemSpriteSheet::SEED_ICECAP;

	plantClass = Icecap::Create;
	alchemyClass = "PotionOfFrost";
}

const String Sorrowmoss::TXT_DESC = BPT::getText("lang.Sorrowmoss_Desc");
//"A Sorrowmoss is a flower (not a moss) with razor-sharp petals, coated with a deadly venom.";

Sorrowmoss::Sorrowmoss()
{
	image = 2;
	plantName = "Sorrowmoss";
}

void Sorrowmoss::activate(Char* ch)
{
	Plant::activate(ch);

	if (ch != NULL) {
		((BuffPoison*)Buff::affect(ch, "BuffPoison"))->set(BuffPoison::durationFactor(ch) * (4 + Dungeon::depth / 2));
	}

	if (Dungeon::visible[pos]) {
		CellEmitter::center(pos)->burst(PoisonParticle::splash(), 3);
	}
}

Sorrowmoss::Seed::Seed()
{
	plantName = "Sorrowmoss";

	name = "seed of " + plantName;
	image = ItemSpriteSheet::SEED_SORROWMOSS;

	plantClass = Sorrowmoss::Create;
	alchemyClass = "PotionOfToxicGas";
}

const String Dreamweed::TXT_DESC = BPT::getText("lang.Dreamweed_Desc");
//"Upon touching a Dreamweed it secretes a glittering cloud of confusing gas.";

Dreamweed::Dreamweed()
{
	image = 3;
	plantName = "Dreamweed";
}

void Dreamweed::activate(Char* ch)
{
	Plant::activate(ch);

	if (ch != NULL) {
		GameScene::addBlob(Blob::seed(pos, 400, "ConfusionGas"));
	}
}

Dreamweed::Seed::Seed()
{
	plantName = "Dreamweed";

	name = "seed of " + plantName;
	image = ItemSpriteSheet::SEED_DREAMWEED;

	plantClass = Dreamweed::Create;
	alchemyClass = "PotionOfInvisibility";
}

const String Sungrass::TXT_DESC = BPT::getText("lang.Sungrass_Desc");// "Sungrass is renowned for its sap's healing properties.";

Sungrass::Sungrass()
{
	image = 4;
	plantName = "Sungrass";
}

void Sungrass::activate(Char* ch)
{
	Plant::activate(ch);

	if (ch != NULL) {
		Buff::affect(ch, "Health");
	}

	if (Dungeon::visible[pos]) {
		CellEmitter::get(pos)->start(ShaftParticle::factory(), 0.2f, 3);
	}
}

Sungrass::Seed::Seed()
{
	plantName = "Sungrass";

	name = "seed of " + plantName;
	image = ItemSpriteSheet::SEED_SUNGRASS;

	plantClass = Sungrass::Create;
	alchemyClass = "PotionOfHealing";
}

const float Sungrass::Health::STEP=5.0f;

const String Sungrass::Health::POS = "pos";

REFLECTBUFF2(Health, Sungrass::Health);

bool Sungrass::Health::attachTo(Char* target)
{
	pos = target->pos;
	return Buff::attachTo(target);
}

boolean Sungrass::Health::act()
{
	if (target->pos != pos || target->HP >= target->HT) {
		detach();
	}
	else {
		target->HP = std::min(target->HT, target->HP + target->HT / 10);
		target->sprite->emitter()->burst(Speck::factory(Speck::HEALING), 1);
	}
	spend(STEP);
	return true;
}

int Sungrass::Health::icon()
{
	return BuffIndicator::HEALING;
}

void Sungrass::Health::storeInBundle(Bundle* bundle)
{
	Buff::storeInBundle(bundle);
	bundle->put(POS, pos);
}

void Sungrass::Health::restoreFromBundle(Bundle* bundle)
{
	Buff::restoreFromBundle(bundle);
	pos = bundle->getInt(POS);
}

const String Earthroot::TXT_DESC = BPT::getText("lang.Earthroot_Desc");
//std::string("When a creature touches an Earthroot, its roots ") +
//std::string("create a kind of natural armor around it.");

Earthroot::Earthroot()
{
	image = 5;
	plantName = "Earthroot";
}

void Earthroot::activate(Char* ch)
{
	Plant::activate(ch);

	if (ch != NULL) {
		((Earthroot::Armor*)Buff::affect(ch, "Armor"))->level = ch->HT;
	}

	if (Dungeon::visible[pos]) {
		CellEmitter::bottom(pos)->start(EarthParticle::factory(), 0.05f, 8);
		Camera::mainCamera->shake(1, 0.4f);
	}
}

Earthroot::Seed::Seed()
{
	plantName = "Earthroot";

	name = "seed of " + plantName;
	image = ItemSpriteSheet::SEED_EARTHROOT;

	plantClass = Earthroot::Create;
	alchemyClass = "PotionOfParalyticGas";
}

const float Earthroot::Armor::STEP = 1.0f;

const String Earthroot::Armor::POS = "pos";
const String Earthroot::Armor::LEVEL = "level";

//REFLECTBUFF2(Armor, Earthroot::Armor);
bool Earthroot::Armor::attachTo(Char* target)
{
	pos = target->pos;
	return Buff::attachTo(target);
}

boolean Earthroot::Armor::act()
{
	if (target->pos != pos) {
		detach();
	}
	spend(STEP);
	return true;
}

int Earthroot::Armor::absorb(int damage)
{
	if (damage >= level) {
		detach();
		return damage - level;
	}
	else {
		level -= damage;
		return 0;
	}
}

int Earthroot::Armor::icon()
{
	return BuffIndicator::ARMOR;
}

void Earthroot::Armor::storeInBundle(Bundle* bundle)
{
	Buff::storeInBundle(bundle);
	bundle->put(POS, pos);
	bundle->put(LEVEL, level);
}

void Earthroot::Armor::restoreFromBundle(Bundle* bundle)
{
	Buff::restoreFromBundle(bundle);
	pos = bundle->getInt(POS);
	level = bundle->getInt(LEVEL);
}
REFLECTBUFF2(Armor, Earthroot::Armor);

const String Fadeleaf::TXT_DESC = BPT::getText("lang.Fadeleaf_Desc");
//std::string("Touching a Fadeleaf will teleport any creature ") +
//std::string("to a random place on the current level.");

Fadeleaf::Fadeleaf()
{
	image = 6;
	plantName = "Fadeleaf";
}

void Fadeleaf::activate(Char* ch)
{
	Plant::activate(ch);

	if (dynamic_cast<Hero*>(ch)) {

		ScrollOfTeleportation::teleportHero((Hero*)ch);
		((Hero*)ch)->curAction = NULL;

	}
	else if (dynamic_cast<Mob*>(ch)) {

		int count = 10;
		int newPos;
		do {
			newPos = Dungeon::level->randomRespawnCell();
			if (count-- <= 0) {
				break;
			}
		} while (newPos == -1);

		if (newPos != -1) {

			ch->pos = newPos;
			ch->sprite->place(ch->pos);
			ch->sprite->visible = Dungeon::visible[pos];

		}

	}

	if (Dungeon::visible[pos]) {
		CellEmitter::get(pos)->start(Speck::factory(Speck::LIGHT), 0.2f, 3);
	}
}

Fadeleaf::Seed::Seed()
{
	plantName = "Fadeleaf";

	name = "seed of " + plantName;
	image = ItemSpriteSheet::SEED_FADELEAF;

	plantClass = Fadeleaf::Create;
	alchemyClass = "PotionOfMindVision";
}

const String Rotberry::TXT_DESC = BPT::getText("lang.Rotberry_Desc");
//"Berries of this shrub taste like sweet, sweet death.";

Rotberry::Rotberry()
{
	image = 7;
	plantName = "Rotberry";
}

void Rotberry::activate(Char* ch)
{
	Plant::activate(ch);

	GameScene::addBlob(Blob::seed(pos, 100, "ToxicGas"));

	Dungeon::level->drop(new Seed(), pos)->sprite->drop();

	if (ch != NULL) {
		Buff::prolong(ch, "Roots", Roots::TICK * 3);
	}
}

Rotberry::Seed::Seed()
{
	plantName = "Rotberry";

	name = "seed of " + plantName;
	image = ItemSpriteSheet::SEED_ROTBERRY;

	plantClass = Rotberry::Create;
	alchemyClass = "PotionOfStrength";
}

boolean Rotberry::Seed::collect(Bag* container)
{
	if (Plant::Seed::collect(container)) {

		if (Dungeon::level != NULL) {

			for (std::set<Mob*>::iterator itr = Dungeon::level->mobs.begin();
				itr != Dungeon::level->mobs.end(); itr++){
				Mob* mob = *itr;
				mob->beckon(Dungeon::hero->pos);
			}

			GLog::w(BPT::getText("lang.Rotberry_Seed_collect").c_str());// "The seed emits a roar that echoes throughout the dungeon!");
			CellEmitter::center(Dungeon::hero->pos)->start(Speck::factory(Speck::SCREAM), 0.3f, 3);
			//Sample.INSTANCE.play(Assets.SND_CHALLENGE);
		}

		return true;
	}
	else {
		return false;
	}
}
