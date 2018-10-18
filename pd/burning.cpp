#include "burning.h"
#include "util.h"
#include "gamescene.h"
#include "level.h"
#include "buffindicator.h"
#include "glog.h"
#include "ring.h"
#include "belongings.h"
#include "scroll.h"
#include "heap.h"
#include "food.h"
#include "dungeon.h"
#include "thief.h"
#include "charsprite.h"
#include "pixelparticle.h"
#include "blob.h"
#include "simpleresource.h"
#include "bpt.h"
#include "hero.h"

const std::string Burning::TXT_BURNS_UP = BPT::getText("lang.burning_burns_up");
const std::string Burning::TXT_BURNED_TO_DEATH = BPT::getText("lang.burning_burned_to_death");

const float Burning::DURATION = 8.0f;
const std::string Burning::LEFT = "left";

void Burning::storeInBundle(Bundle* bundle)
{
	Buff::storeInBundle(bundle);
	bundle->put(LEFT, left);
}

void Burning::restoreFromBundle(Bundle* bundle)
{
	Buff::restoreFromBundle(bundle);
	left = bundle->getFloat(LEFT);
}

bool Burning::act()
{
	if (target->isAlive()) 
	{
		if (dynamic_cast<Hero*>(target)) 
		{
			Buff::prolong(target, "Light", TICK * 1.01f);
		}

		target->damage(Random::Int(1, 5), this->getClassName());

		if (dynamic_cast<Hero*>(target)) 
		{
			Item* item = ((Hero*)target)->belongings->randomUnequipped();
			if (dynamic_cast<Scroll*>(item)) {
			
				item = item->detach(((Hero*)target)->belongings->backpack);
				GLog::w(TXT_BURNS_UP.c_str(), item->toString());
			
				Heap::burnFX(target->pos);			
			}
			else if (dynamic_cast<MysteryMeat*>(item)) {
			
				item = item->detach(((Hero*)target)->belongings->backpack);
				ChargrilledMeat* steak = new ChargrilledMeat();
				if (!steak->collect(((Hero*)target)->belongings->backpack)) {
					Dungeon::level->drop(steak, target->pos)->sprite->drop();
				}
				GLog::w(TXT_BURNS_UP.c_str(), item->toString());
			
				Heap::burnFX(target->pos);
			
			}

		}
		else if (dynamic_cast<Thief*>(target) && dynamic_cast<Scroll*>(((Thief*)target)->item))
		{
		
			((Thief*)target)->item = NULL;
			target->sprite->emitter()->burst(ElmoParticle::factory(), 6);
		}
	}
	else 
	{
		detach();
	}

	if (Level::flamable[target->pos]) 
	{
		GameScene::addBlob(Blob::seed(target->pos, 4, "BlobFire"));
	}

	spend(TICK);
	left -= TICK;

	if (left <= 0 ||
		Random::Float() > (2 + (float)target->HP / target->HT) / 3 ||
		(Level::water[target->pos] && !target->flying))
	{
		detach();
	}

	return true;
}

void Burning::reignite(Char* ch)
{
	left = duration(ch);
}

int Burning::icon()
{
	return BuffIndicator::FIRE;
}

float Burning::duration(Char* ch)
{
	RingOfElements::Resistance* r = (RingOfElements::Resistance*)ch->buff("Resistance");
	return r != NULL ? r->durationFactor() * DURATION : DURATION;
}

void Burning::onDeath()
{
	Badges::validateDeathFromFire();
	
	Dungeon::fail(GameMath::format(ResultDescriptions::BURNING.c_str(), Dungeon::depth));
	GLog::n(TXT_BURNED_TO_DEATH.c_str());
}

REFLECTBUFF(Burning);