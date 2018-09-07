#include "lightningtrap.h"
#include "util.h"
#include "char.h"
#include "dungeon.h"
#include "camera.h"
#include "glog.h"
#include "belongings.h"
#include "cellemitter.h"
#include "char.h"
#include "hero.h"

const std::string LightningTrap::name = "lightning trap";

LightningTrap::Electricity* LightningTrap::LIGHTNING = new LightningTrap::Electricity();

void LightningTrap::trigger(int pos, Char* ch)
{
	if (ch != NULL) 
	{
		ch->damage(std::max(1, Random::Int(ch->HP / 3, 2 * ch->HP / 3)), "LIGHTNING");
		if (ch == Dungeon::hero) 
		{
			Camera::mainCamera->shake(2, 0.3f);

			if (!ch->isAlive()) 
			{
				//Dungeon.fail(Utils.format(ResultDescriptions.TRAP, name, Dungeon.depth));
				GLog::n("You were killed by a discharge of a lightning trap...");
			}
			else 
			{
				//((Hero*)ch)->belongings->charge(false);
			}
		}

		std::vector<int> points(2);

		points[0] = pos - Level::WIDTH;
		points[1] = pos + Level::WIDTH;
		//ch->sprite->parent->add(new Lightning(points, 2, null));

		points[0] = pos - 1;
		points[1] = pos + 1;
		//ch.sprite.parent.add(new Lightning(points, 2, null));
	}

	//CellEmitter.center(pos).burst(SparkParticle.FACTORY, Random.IntRange(3, 4));
}
