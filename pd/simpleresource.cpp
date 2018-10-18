#include "simpleresource.h"
#include "define.h"
#include "image.h"
#include "actor.h"
#include "char.h"
#include "buff.h"
#include "util.h"
#include "blob.h"
#include "dungeon.h"
#include "level.h"
#include "heap.h"
#include "cellemitter.h"
#include "pixelparticle.h"
#include "gamescene.h"
#include "wndmessage.h"
#include "glog.h"

NinePatch* Chrome::get(Type type)
{
	switch (type) 
	{
	case WINDOW:
		return new NinePatch(Assets::CHROME, 0, 0, 22, 22, 7);
	case TOAST:
		return new NinePatch(Assets::CHROME, 22, 0, 18, 18, 5);
	case TOAST_TR:
		return new NinePatch(Assets::CHROME, 40, 0, 18, 18, 5);
	case BUTTON:
		return new NinePatch(Assets::CHROME, 58, 0, 6, 6, 2);
	case TAG:
		return new NinePatch(Assets::CHROME, 22, 18, 16, 14, 3);
	case SCROLL:
		return new NinePatch(Assets::CHROME, 32, 32, 32, 32, 5, 11, 5, 11);
	case TAB_SET:
		return new NinePatch(Assets::CHROME, 64, 0, 22, 22, 7, 7, 7, 7);
	case TAB_SELECTED:
		return new NinePatch(Assets::CHROME, 64, 22, 10, 14, 4, 7, 4, 6);
	case TAB_UNSELECTED:
		return new NinePatch(Assets::CHROME, 74, 22, 10, 14, 4, 7, 4, 6);
	default:
		return NULL;
	}
}

Image* Effects::get(Type type)
{
	Image* icon = new Image(Assets::EFFECTS);
	switch (type) 
	{
	case RIPPLE:
		icon->frame(icon->tex->uvRect(0, 0, 16, 16));
		break;
	case LIGHTNING:
		icon->frame(icon->tex->uvRect(16, 0, 32, 8));
		break;
	case WOUND:
		icon->frame(icon->tex->uvRect(16, 8, 32, 16));
		break;
	case RAY:
		icon->frame(icon->tex->uvRect(16, 16, 32, 24));
		break;
	}
	return icon;
}

#include "terrain.h"
int Terrain::flags[256] = {0};
Terrain Terrain::inst;

const std::string ResultDescriptions::MOB = BPT::getText("lang.ResultDescriptions_MOB");// "Killed by %s on level %d";
const std::string ResultDescriptions::BOSS = BPT::getText("lang.ResultDescriptions_BOSS");// "Killed by the %s on level %d";

// Items
const std::string ResultDescriptions::WAND = BPT::getText("lang.ResultDescriptions_WAND");// "Killed by your own %s on level %d";
const std::string ResultDescriptions::GLYPH = BPT::getText("lang.ResultDescriptions_GLYPH");// "Killed by the %s on level %d";
const std::string ResultDescriptions::BOMB = BPT::getText("lang.ResultDescriptions_BOMB");// "Killed by an explosion of a bomb on level %d";

// Dungeon features
const std::string ResultDescriptions::TRAP = BPT::getText("lang.ResultDescriptions_TRAP");// "Killed by discharge of %s on level %d";

// Debuffs & blobs
const std::string ResultDescriptions::BURNING = BPT::getText("lang.ResultDescriptions_Burning");// "Burned to death on level %d";
const std::string ResultDescriptions::HUNGER = BPT::getText("lang.ResultDescriptions_Hunger");// "Starved to death on level %d";
const std::string ResultDescriptions::POISON = BPT::getText("lang.ResultDescriptions_Poison");// "Died from poison on level %d";
const std::string ResultDescriptions::GAS = BPT::getText("lang.ResultDescriptions_Gas");// "Died from toxic gas on level %d";
const std::string ResultDescriptions::BLEEDING = BPT::getText("lang.ResultDescriptions_Bleeding");// "Bled to death on level %d";
const std::string ResultDescriptions::OOZE = BPT::getText("lang.ResultDescriptions_Ooze");// "Killed by a caustic ooze on level %d";
const std::string ResultDescriptions::FALL = BPT::getText("lang.ResultDescriptions_Fall");// "Fell to death on level %d";

const std::string ResultDescriptions::WIN = BPT::getText("lang.ResultDescriptions_Win");// "Obtained the Amulet of Yendor";

boolean Freezing::affect(int cell, BlobFire* fire)
{
	Char* ch = Actor::findChar(cell);
	if (ch != NULL) {
		Buff::prolong(ch, "Frost", Frost::duration(ch) * Random::Float(1.0f, 1.5f));
	}

	if (fire != NULL) {
		fire->clear(cell);
	}

	Heap* heap = Dungeon::level->heaps.get(cell);
	if (heap != NULL) {
		heap->freeze();
	}

	if (Dungeon::visible[cell]) {
		CellEmitter::get(cell)->start(SnowParticle::factory(), 0.2f, 6);
		return true;
	}
	else {
		return false;
	}
}

const String Sign::TXT_DEAD_END =
"What are you doing here?!";

const String Sign::_TIPS[] = {
	"Don't overestimate your strength, use weapons and armor you can handle.",
	"Not all doors in the dungeon are visible at first sight. If you are stuck, search for hidden doors.",
	std::string("Remember, that raising your strength is not the only way to access better equipment. You can go ") +
	std::string("the other way, lowering its strength requirement with Scrolls of Upgrade."),
	"You can spend your gold in shops on deeper levels of the dungeon. The first one is on the 6th level.",

	"Beware of Goo!",

	"Pixel-Mart - all you need for successful adventure!",
	std::string("Identify your potions and scrolls as soon as possible. Don't put it off to the moment ") +
	std::string("when you actually need them."),
	"Being hungry doesn't hurt, but starving does hurt.",
	std::string("Surprise attack has a better chance to hit. For example, you can ambush your enemy behind ") +
	std::string("a closed door when you know it is approaching."),

	"Don't let The Tengu out!",

	"Pixel-Mart. Spend money. Live longer.",
	"When you're attacked by several monsters at the same time, try to retreat behind a door.",
	"If you are burning, you can't put out the fire in the water while levitating.",
	"There is no sense in possessing more than one Ankh at the same time, because you will lose them upon resurrecting.",

	"DANGER! Heavy machinery can cause injury, loss of limbs or death!",

	"Pixel-Mart. A safer life in dungeon.",
	"When you upgrade an enchanted weapon, there is a chance to destroy that enchantment.",
	"Weapons and armors deteriorate faster than wands and rings, but there are more ways to fix them.",
	"The only way to obtain a Scroll of Wipe Out is to receive it as a gift from the dungeon spirits.",

	"No weapons allowed in the presence of His Majesty!",

	"Pixel-Mart. Special prices for demon hunters!"
};

const String Sign::TXT_BURN =
"As you try to read the sign it bursts into greenish flames.";

void Sign::read(int pos)
{
	//if (Dungeon.level instanceof DeadEndLevel) {
	//
	//	GameScene.show(new WndMessage(TXT_DEAD_END));
	//
	//}
	//else {

		int index = Dungeon::depth - 1;

		int len = sizeof(_TIPS) / sizeof(std::string);
		if (index < len) {
			GameScene::show(new WndMessage(_TIPS[index]));
		}
		else {

			Dungeon::level->destroy(pos);
			GameScene::updateMap(pos);
			GameScene::discoverTile(pos, Terrain::SIGN);

			CellEmitter::get(pos)->burst(ElmoParticle::factory(), 6);
			//Sample.INSTANCE.play(Assets.SND_BURNING);

			GLog::w(TXT_BURN.c_str());

		}
	//}
}
