#include "key.h"
#include "simpleresource.h"
#include "dungeon.h"
#include "util.h"
#include "hero.h"
#include "belongings.h"

SkeletonKey::SkeletonKey()
{
	name = "skeleton key";
	image = ItemSpriteSheet::SKELETON_KEY;
}

const float Key::TIME_TO_UNLOCK = 1.0f;

Key::Key()
{
	stackable = false;

	depth = Dungeon::depth;
}

void Key::storeInBundle(Bundle* bundle)
{
	Item::storeInBundle(bundle);
	bundle->put(DEPTH, depth);
}

void Key::restoreFromBundle(Bundle* bundle)
{
	Item::restoreFromBundle(bundle);
	depth = bundle->getInt(DEPTH);
}

String Key::status()
{
	return GameMath::format("%d", depth);
}

const String Key::DEPTH = "depth";

GoldenKey::GoldenKey()
{
	name = "golden key";
	image = ItemSpriteSheet::GOLDEN_KEY;
}

IronKey::IronKey()
{
	name = "iron key";
	image = ItemSpriteSheet::IRON_KEY;
}

boolean IronKey::collect(Bag* bag)
{
	boolean result = Key::collect(bag);
	if (result && depth == Dungeon::depth && Dungeon::hero != null) {
		Dungeon::hero->belongings->countIronKeys();
	}
	return result;
}

void IronKey::onDetach()
{
	if (depth == Dungeon::depth) {
		Dungeon::hero->belongings->countIronKeys();
	}
}

String IronKey::toString()
{
	return GameMath::format(TXT_FROM_DEPTH.c_str(), depth);
}

int IronKey::curDepthQuantity = 0;

const String IronKey::TXT_FROM_DEPTH = BPT::getText("lang.IronKey_From_Depth");// "iron key from depth %d";
