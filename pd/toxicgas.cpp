#include "toxicgas.h"
#include "dungeon.h"
#include "util.h"
#include "blobemitter.h"
#include "speck.h"
#include "glog.h"
#include "simpleresource.h"
#include "char.h"

void ToxicGas::evolve()
{
	Blob::evolve();

	int levelDamage = 5 + Dungeon::depth * 5;

	Char* ch;
	for (int i = 0; i < LENGTH; i++) {
		if (cur[i] > 0 && (ch = Actor::findChar(i)) != NULL) 
		{
			int damage = (ch->HT + levelDamage) / 40;
			if (Random::Int(40) < (ch->HT + levelDamage) % 40) 
			{
				damage++;
			}

			ch->damage(damage, this->getClassName());
		}
	}

	std::map<std::string, Blob*>::iterator itr = Dungeon::level->blobs.find("ParalyticGas");
	if (itr != Dungeon::level->blobs.end())
	{
		Blob* blob = itr->second;
		std::vector<int> par = blob->cur;

		for (int i = 0; i < LENGTH; i++) 
		{
			int t = cur[i];
			int p = par[i];

			if (p >= t) 
			{
				volume -= t;
				cur[i] = 0;
			}
			else
			{
				blob->volume -= p;
				par[i] = 0;
			}
		}
	}
}

void ToxicGas::use(BlobEmitter* emitter)
{
	Blob::use(emitter);

	emitter->pour(Speck::factory(Speck::TOXIC), 0.6f);
}

std::string ToxicGas::tileDesc()
{
	return BPT::getText("lang.ToxicGas_tileDesc");// "A greenish cloud of toxic gas is swirling here.";
}

void ToxicGas::onDeath()
{	
	Badges::validateDeathFromGas();
	
	Dungeon::fail(GameMath::format(ResultDescriptions::GAS.c_str(), Dungeon::depth));
	GLog::n(BPT::getText("lang.ToxicGas_onDeath"));// "You died from a toxic gas..");
}

REFLECTBLOB(ToxicGas);