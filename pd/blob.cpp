#include "blob.h"
#include "bundle.h"
#include "util.h"
#include "level.h"
#include "dungeon.h"
#include "gamescene.h"
#include "blobemitter.h"
#include "flameparticle.h"
#include "heap.h"
#include "burning.h"
#include "speck.h"

const std::string Blob::CUR = "cur";
const std::string Blob::START = "start";

std::map<std::string, FactoryBlob*> FactoryBlob::facs;

std::vector<int> Blob::trim(int start, int end)
{
	int len = end - start;
	std::vector<int> copy(len);
	copy.assign(&cur[start], &cur[end]);

	return copy;
}

void Blob::storeInBundle(Bundle* bundle)
{
	Actor::storeInBundle(bundle);

	if (volume > 0) 
	{
		int start;
		for (start = 0; start < LENGTH; start++) 
		{
			if (cur[start] > 0) 
			{
				break;
			}
		}
		int end;
		for (end = LENGTH - 1; end > start; end--) 
		{
			if (cur[end] > 0) 
			{
				break;
			}
		}

		bundle->put(START, start);
		bundle->put(CUR, trim(start, end + 1));

	}
}

void Blob::restoreFromBundle(Bundle* bundle)
{
	Actor::restoreFromBundle(bundle);

	std::vector<int> data;
	bundle->getIntArray(CUR, data);
	if (data.size() > 0) 
	{
		int start = bundle->getInt(START);
		for (int i = 0; i < data.size(); i++) 
		{
			cur[i + start] = data[i];
			volume += data[i];
		}
	}

	if (Level::resizingNeeded) 
	{
		std::vector<int> cur(Level::LENGTH);

		int loadedMapSize = Level::loadedMapSize;
		for (int i = 0; i < loadedMapSize; i++) 
		{			
			Arrays<int>::arraycopy(this->cur, i * loadedMapSize, cur, i * Level::WIDTH, loadedMapSize);
		}

		this->cur = cur;
	}
}

bool Blob::act()
{
	spend(TICK);

	if (volume > 0) 
	{
		volume = 0;
		evolve();

		std::vector<int> tmp = off;
		off = cur;
		cur = tmp;

	}

	return true;
}

void Blob::seed(int cell, int amount)
{
	cur[cell] += amount;
	volume += amount;
}

Blob* Blob::seed(int cell, int amount, const std::string& type)
{
	Blob* gas = NULL;
	std::map<std::string, Blob*>::iterator itr = Dungeon::level->blobs.find(type);
	if (itr == Dungeon::level->blobs.end())
	{
		gas = FactoryBlob::Create(type);
		Dungeon::level->blobs.insert(std::make_pair(type, gas));
	}
	else
	{
		gas = itr->second;
	}

	gas->seed(cell, amount);

	return gas;
}

void Blob::clear(int cell)
{
	volume -= cur[cell];
	cur[cell] = 0;
}

Blob::Blob()
{
	cur.resize(LENGTH);
	off.resize(LENGTH);

	volume = 0;
}

void Blob::evolve()
{
	std::vector<bool> notBlocking = BArray::not(Level::solid, std::vector<bool>());

	for (int i = 1; i < HEIGHT - 1; i++) 
	{
		int from = i * WIDTH + 1;
		int to = from + WIDTH - 2;

		for (int pos = from; pos < to; pos++) 
		{
			if (notBlocking[pos]) 
			{
				int count = 1;
				int sum = cur[pos];

				if (notBlocking[pos - 1]) 
				{
					sum += cur[pos - 1];
					count++;
				}
				if (notBlocking[pos + 1]) 
				{
					sum += cur[pos + 1];
					count++;
				}
				if (notBlocking[pos - WIDTH]) 
				{
					sum += cur[pos - WIDTH];
					count++;
				}
				if (notBlocking[pos + WIDTH]) 
				{
					sum += cur[pos + WIDTH];
					count++;
				}

				int value = sum >= count ? (sum / count) - 1 : 0;
				off[pos] = value;

				volume += value;
			}
			else 
			{
				off[pos] = 0;
			}
		}
	}
}

void BlobFire::evolve()
{
	std::vector<bool> flamable = Level::flamable;

	int from = WIDTH + 1;
	int to = Level::LENGTH - WIDTH - 1;

	boolean observe = false;

	for (int pos = from; pos < to; pos++) {

		int fire;

		if (cur[pos] > 0) {

			burn(pos);

			fire = cur[pos] - 1;
			if (fire <= 0 && flamable[pos]) {

				int oldTile = Dungeon::level->map[pos];
				Dungeon::level->destroy(pos);

				observe = true;
				GameScene::updateMap(pos);
				if (Dungeon::visible[pos]) {
					GameScene::discoverTile(pos, oldTile);
				}
			}

		}
		else {

			if (flamable[pos] && (cur[pos - 1] > 0 || cur[pos + 1] > 0 || cur[pos - WIDTH] > 0 || cur[pos + WIDTH] > 0)) {
				fire = 4;
				burn(pos);
			}
			else {
				fire = 0;
			}

		}

		volume += (off[pos] = fire);

	}

	if (observe) {
		Dungeon::observe();
	}
}

void BlobFire::burn(int pos)
{
	Char* ch = Actor::findChar(pos);
	if (ch != NULL) {
		((Burning*)Buff::affect(ch, "Burning"))->reignite(ch);
	}

	Heap* heap = Dungeon::level->heaps.get(pos);
	if (heap != NULL) {
		heap->burn();
	}
}

void BlobFire::seed(int cell, int amount)
{
	if (cur[cell] == 0) {
		volume += amount;
		cur[cell] = amount;
	}
}

void BlobFire::use(BlobEmitter* emitter)
{
	Blob::use(emitter);
	emitter->start(FlameParticle::FACTORY, 0.03f, 0);
}

REFLECTBLOB(BlobFire);

void ConfusionGas::evolve()
{
	Blob::evolve();

	Char* ch;
	for (int i = 0; i < LENGTH; i++) {
		if (cur[i] > 0 && (ch = Actor::findChar(i)) != NULL) {
			Buff::prolong(ch, "Vertigo", Vertigo::duration(ch));
		}
	}
}

void ConfusionGas::use(BlobEmitter* emitter)
{
	Blob::use(emitter);

	emitter->pour(Speck::factory(Speck::CONFUSION, true), 0.6f);
}

REFLECTBLOB(ConfusionGas);

void ParalyticGas::evolve()
{
	Blob::evolve();

	Char* ch;
	for (int i = 0; i < LENGTH; i++) {
		if (cur[i] > 0 && (ch = Actor::findChar(i)) != NULL) {
			Buff::prolong(ch, "BuffParalysis", BuffParalysis::duration(ch));
		}
	}
}

void ParalyticGas::use(BlobEmitter* emitter)
{
	Blob::use(emitter);

	emitter->pour(Speck::factory(Speck::PARALYSIS), 0.6f);
}

REFLECTBLOB(ParalyticGas);