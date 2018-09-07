#include "potion.h"
#include "simpleresource.h"
#include "dungeon.h"
#include "terrain.h"
#include "splash.h"
#include "glog.h"
#include "buff.h"
#include "speck.h"
#include "charsprite.h"
#include "pathfinder.h"
#include "level.h"
#include "util.h"
#include "blob.h"
#include "cellemitter.h"
#include "alphatweener.h"
#include "char.h"
#include "hero.h"

const String Potion::AC_DRINK = "DRINK";

const String Potion::TXT_HARMFUL = "Harmful potion!";
const String Potion::TXT_BENEFICIAL = "Beneficial potion";
const String Potion::TXT_YES = "Yes, I know what I'm doing";
const String Potion::TXT_NO = "No, I changed my mind";
const String Potion::TXT_R_U_SURE_DRINK =
"Are you sure you want to drink it? In most cases you should throw such potions at your enemies.";
const String Potion::TXT_R_U_SURE_THROW =
"Are you sure you want to throw it? In most cases it makes sense to drink it.";

const float Potion::TIME_TO_DRINK = 1.0f;

String Potion::_potions[] = {
	"PotionOfHealing",
	"PotionOfExperience",
	"PotionOfToxicGas",
	"PotionOfLiquidFlame",
	"PotionOfStrength",
	"PotionOfParalyticGas",
	"PotionOfLevitation",
	"PotionOfMindVision",
	"PotionOfPurity",
	"PotionOfInvisibility",
	"PotionOfMight",
	"PotionOfFrost"
};

std::vector<String> Potion::potions(_potions, _potions+sizeof(_potions)/sizeof(String));

String Potion::_colors[] = {
	"turquoise", "crimson", "azure", "jade", "golden", "magenta",
	"charcoal", "ivory", "amber", "bistre", "indigo", "silver" };

std::vector<String> Potion::colors(_colors, _colors + sizeof(_colors) / sizeof(String));

int Potion::_images[] = {
	ItemSpriteSheet::POTION_TURQUOISE,
	ItemSpriteSheet::POTION_CRIMSON,
	ItemSpriteSheet::POTION_AZURE,
	ItemSpriteSheet::POTION_JADE,
	ItemSpriteSheet::POTION_GOLDEN,
	ItemSpriteSheet::POTION_MAGENTA,
	ItemSpriteSheet::POTION_CHARCOAL,
	ItemSpriteSheet::POTION_IVORY,
	ItemSpriteSheet::POTION_AMBER,
	ItemSpriteSheet::POTION_BISTRE,
	ItemSpriteSheet::POTION_INDIGO,
	ItemSpriteSheet::POTION_SILVER };

std::vector<int> Potion::images(_images, _images + sizeof(_images) / sizeof(int));

ItemStatusHandler* Potion::handler;

Potion::Potion()
{
	stackable = true;
	defaultAction = AC_DRINK;

	//image = handler.image(this);
	//color = handler.label(this);
}

void Potion::initColors()
{
	handler = new ItemStatusHandler(potions, colors, images);
}

void Potion::save(Bundle* bundle)
{
	handler->save(bundle);
}

void Potion::restore(Bundle* bundle)
{
	handler = new ItemStatusHandler(potions, colors, images, bundle);
}

void Potion::actions(Hero* hero, std::vector<std::string>& actions)
{
	Item::actions(hero, actions);
	actions.push_back(AC_DRINK);
}

void Potion::execute(Hero* hero, std::string action)
{
	if (action.compare(AC_DRINK) == 0) 
	{
		//if (isKnown() && (
		//	this instanceof PotionOfLiquidFlame ||
		//	this instanceof PotionOfToxicGas ||
		//	this instanceof PotionOfParalyticGas)) {
		//
		//	GameScene.show(
		//		new WndOptions(TXT_HARMFUL, TXT_R_U_SURE_DRINK, TXT_YES, TXT_NO){
		//		@Override
		//		protected void onSelect(int index) {
		//			if (index == 0) {
		//				drink(hero);
		//			}
		//		};
		//	}
		//	);
		//
		//}
		//else {
			drink(hero);
		//}
	}
	else 
	{
		Item::execute(hero, action);
	}
}

void Potion::doThrow(Hero* hero)
{
	//if (isKnown() && (
	//	this instanceof PotionOfExperience ||
	//	this instanceof PotionOfHealing ||
	//	this instanceof PotionOfLevitation ||
	//	this instanceof PotionOfMindVision ||
	//	this instanceof PotionOfStrength ||
	//	this instanceof PotionOfInvisibility ||
	//	this instanceof PotionOfMight)) {
	//
	//	GameScene.show(
	//		new WndOptions(TXT_BENEFICIAL, TXT_R_U_SURE_THROW, TXT_YES, TXT_NO){
	//		@Override
	//		protected void onSelect(int index) {
	//			if (index == 0) {
	//				Potion.super.doThrow(hero);
	//			}
	//		};
	//	}
	//	);
	//
	//}
	//else {
	Item::doThrow(hero);
	//}
}

void Potion::shatter(int cell)
{
	if (Dungeon::visible[cell]) {
		//GLog.i("The flask shatters and " + color() + " liquid splashes harmlessly");
		//Sample.INSTANCE.play(Assets.SND_SHATTER);
		splash(cell);
	}
}

boolean Potion::isKnown()
{
	return handler->isKnown(this->getClassName());
}

void Potion::setKnown()
{
	if (!isKnown()) {
		handler->know(this->getClassName());
	}

	//Badges.validateAllPotionsIdentified();
}

void Potion::drink(Hero* hero)
{
	//detach(hero.belongings.backpack);

	//hero->spend(TIME_TO_DRINK);
	hero->busy();
	onThrow(hero->pos);

	//Sample.INSTANCE.play(Assets.SND_DRINK);

	//hero.sprite.operate(hero.pos);
}

void Potion::onThrow(int cell)
{
	if (Dungeon::hero->pos == cell) 
	{
		apply(Dungeon::hero);
	}
	else if (Dungeon::level->map[cell] == Terrain::WELL || Level::pit[cell]) {

		Item::onThrow(cell);

	}
	else  {

		shatter(cell);

	}
}

void Potion::apply(Hero* hero)
{
	shatter(hero->pos);
}

void Potion::splash(int cell)
{
	int color = ItemSprite::pick(image, 8, 10);
	Splash::at(cell, color, 5);
}
REFLECTITEM(PotionOfHealing);

PotionOfHealing::PotionOfHealing()
{
	name = "Potion of Healing";
}

void PotionOfHealing::heal(Hero* hero)
{
	hero->HP = hero->HT;
	//Buff::detach(hero, Poison.class);
	//Buff::detach(hero, Cripple.class);
	//Buff::detach(hero, Weakness.class);
	//Buff::detach(hero, Bleeding.class);

	hero->sprite->emitter()->start(Speck::factory(Speck::HEALING), 0.4f, 4);
}

void PotionOfHealing::apply(Hero* hero)
{
	setKnown();
	heal(Dungeon::hero);
	GLog::p("Your wounds heal completely.");
}

PotionOfExperience::PotionOfExperience()
{
	name = "Potion of Experience";
}

void PotionOfExperience::apply(Hero* hero)
{
	setKnown();
	hero->earnExp(hero->maxExp() - hero->exp);
}

REFLECTITEM(PotionOfToxicGas);

PotionOfToxicGas::PotionOfToxicGas()
{
	name = "Potion of Toxic Gas";
}

void PotionOfToxicGas::shatter(int cell)
{
	if (Dungeon::visible[cell]) {
		setKnown();

		splash(cell);
		//Sample.INSTANCE.play(Assets.SND_SHATTER);
	}

	//GameScene.add(Blob.seed(cell, 1000, ToxicGas.class));
}

PotionOfLiquidFlame::PotionOfLiquidFlame()
{
	name = "Potion of Liquid Flame";
}
REFLECTITEM(PotionOfLiquidFlame);

void PotionOfLiquidFlame::shatter(int cell)
{
	if (Dungeon::visible[cell]) {
		setKnown();

		splash(cell);
		//Sample.INSTANCE.play(Assets.SND_SHATTER);
	}

	//GameScene.add(Blob.seed(cell, 2, Fire.class));
}
REFLECTITEM(PotionOfStrength);

PotionOfStrength::PotionOfStrength()
{
	name = "Potion of Strength";
}

void PotionOfStrength::apply(Hero* hero)
{
	setKnown();

	hero->STR++;
	hero->sprite->showStatus(CharSprite::POSITIVE, "+1 str");
	GLog::p("Newfound strength surges through your body.");

	//Badges.validateStrengthAttained();
}

REFLECTITEM(PotionOfParalyticGas);

PotionOfParalyticGas::PotionOfParalyticGas()
{
	name = "Potion of Paralytic Gas";
}

void PotionOfParalyticGas::shatter(int cell)
{
	if (Dungeon::visible[cell]) {
		setKnown();

		splash(cell);
		//Sample.INSTANCE.play(Assets.SND_SHATTER);
	}

	//GameScene.add(Blob.seed(cell, 1000, ParalyticGas.class));
}

PotionOfLevitation::PotionOfLevitation()
{
	name = "Potion of Levitation";
}

void PotionOfLevitation::apply(Hero* hero)
{
	setKnown();
	//Buff.affect(hero, Levitation.class, Levitation.DURATION);
	GLog::i("You float into the air!");
}

PotionOfMindVision::PotionOfMindVision()
{
	name = "Potion of Mind Vision";
}
REFLECTITEM(PotionOfMindVision);

void PotionOfMindVision::apply(Hero* hero)
{
	setKnown();
	//Buff.affect(hero, MindVision.class, MindVision.DURATION);
	Dungeon::observe();

	if (Dungeon::level->mobs.size() > 0) {
		GLog::i("You can somehow feel the presence of other creatures' minds!");
	}
	else {
		GLog::i("You can somehow tell that you are alone on this level at the moment.");
	}
}


const String PotionOfPurity::TXT_FRESHNESS = "You feel uncommon freshness in the air.";
const String PotionOfPurity::TXT_NO_SMELL = "You've stopped sensing any smells!";

PotionOfPurity::PotionOfPurity()
{
	name = "Potion of Purification";
}

void PotionOfPurity::shatter(int cell)
{
	PathFinder::buildDistanceMap(cell, BArray::not(Level::losBlocking, std::vector<bool>()), DISTANCE);

	boolean procd = false;

	std::vector<Blob*> blobs;
	//Blob blobs[] = {
	//	Dungeon.level.blobs.get(ToxicGas.class),
	//	Dungeon.level.blobs.get(ParalyticGas.class)
	//};

	for (int j = 0; j < blobs.size(); j++) {

		Blob* blob = blobs[j];
		if (blob == NULL) {
			continue;
		}

		for (int i = 0; i < Level::LENGTH; i++) {
			if (PathFinder::distance[i] < std::numeric_limits<int>::max()) {

				int value = blob->cur[i];
				if (value > 0) {

					blob->cur[i] = 0;
					blob->volume -= value;
					procd = true;

					if (Dungeon::visible[i]) {
						CellEmitter::get(i)->burst(Speck::factory(Speck::DISCOVER), 1);
					}
				}

			}
		}
	}

	boolean heroAffected = PathFinder::distance[Dungeon::hero->pos] < std::numeric_limits<int>::max();

	if (procd) {

		if (Dungeon::visible[cell]) {
			splash(cell);
			//Sample.INSTANCE.play(Assets.SND_SHATTER);
		}

		setKnown();

		if (heroAffected) {
			GLog::p(TXT_FRESHNESS.c_str());
		}

	}
	else {

		Potion::shatter(cell);

		if (heroAffected) {
			GLog::i(TXT_FRESHNESS.c_str());
			setKnown();
		}

	}
}

void PotionOfPurity::apply(Hero* hero)
{
	GLog::w(TXT_NO_SMELL.c_str());
	//Buff.prolong(hero, GasesImmunity.class, GasesImmunity.DURATION);
	setKnown();
}

const float PotionOfInvisibility::ALPHA = 0.4f;
PotionOfInvisibility::PotionOfInvisibility()
{
	name = "Potion of Invisibility";
}

void PotionOfInvisibility::melt(Char* ch)
{
	if (ch->sprite->parent != NULL) {
		ch->sprite->parent->add(new AlphaTweener(ch->sprite, ALPHA, 0.4f));
	}
	else {
		ch->sprite->alpha(ALPHA);
	}
}

REFLECTITEM(PotionOfInvisibility);

void PotionOfInvisibility::apply(Hero* hero)
{
	setKnown();
	//Buff.affect(hero, Invisibility.class, Invisibility.DURATION);
	GLog::i("You see your hands turn invisible!");
	//Sample.INSTANCE.play(Assets.SND_MELD);
}

PotionOfMight::PotionOfMight()
{
	name = "Potion of Might";
}

void PotionOfMight::apply(Hero* hero)
{
	setKnown();

	hero->STR++;
	hero->HT += 5;
	hero->HP += 5;
	hero->sprite->showStatus(CharSprite::POSITIVE, "+1 str, +5 ht");
	GLog::p("Newfound strength surges through your body.");

	//Badges.validateStrengthAttained();
}

REFLECTITEM(PotionOfFrost);

PotionOfFrost::PotionOfFrost()
{
	name = "Potion of Frost";
}

void PotionOfFrost::shatter(int cell)
{
	PathFinder::buildDistanceMap(cell, BArray::not(Level::losBlocking, std::vector<bool>()), DISTANCE);

	//Fire fire = (Fire)Dungeon.level.blobs.get(Fire.class);

	boolean visible = false;
	for (int i = 0; i < Level::LENGTH; i++) {
		if (PathFinder::distance[i] < std::numeric_limits<int>::max()) {
			//visible = Freezing.affect(i, fire) || visible;
		}
	}

	if (visible) {
		splash(cell);
		//Sample.INSTANCE.play(Assets.SND_SHATTER);

		setKnown();
	}
}
