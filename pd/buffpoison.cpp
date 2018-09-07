#include "buffpoison.h"
#include "buffindicator.h"
#include "glog.h"
#include "ring.h"
#include "simpleresource.h"
#include "dungeon.h"
#include "util.h"
#include "char.h"

const std::string BuffPoison::LEFT = "left";

void BuffPoison::storeInBundle(Bundle* bundle)
{
	Buff::storeInBundle(bundle);
	bundle->put(LEFT, left);
}

void BuffPoison::restoreFromBundle(Bundle* bundle)
{
	Buff::restoreFromBundle(bundle);
	left = bundle->getFloat(LEFT);
}

int BuffPoison::icon()
{
	return BuffIndicator::POISON;
}

bool BuffPoison::act()
{
	if (target->isAlive()) 
	{
		target->damage((int)(left / 3) + 1, this->getClassName());
		spend(TICK);

		if ((left -= TICK) <= 0) 
		{
			detach();
		}
	}
	else 
	{
		detach();
	}

	return true;
}

float BuffPoison::durationFactor(Char* ch)
{
	RingOfElements::Resistance* r = (RingOfElements::Resistance*)ch->buff("Resistance");
	return r != NULL ? r->durationFactor() : 1;
}

void BuffPoison::onDeath()
{
	Badges::validateDeathFromPoison();

	Dungeon::fail(GameMath::format(ResultDescriptions::POISON.c_str(), Dungeon::depth));
	GLog::n("You died from poison...");
}

REFLECTBUFF(BuffPoison);