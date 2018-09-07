#include "blobemitter.h"
#include "blob.h"
#include "dungeontilemap.h"
#include "dungeon.h"
#include "util.h"

const int BlobEmitter::WIDTH = Blob::WIDTH;
const int BlobEmitter::LENGTH = Blob::LENGTH;

BlobEmitter::BlobEmitter(Blob* blob)
{
	this->blob = blob;
	blob->use(this);
}

void BlobEmitter::emit(int index)
{
	if (blob->volume <= 0) 
	{
		return;
	}

	std::vector<int>& map = blob->cur;
	float size = DungeonTilemap::SIZE;

	for (int i = 0; i < LENGTH; i++) 
	{
		if (map[i] > 0 && Dungeon::visible[i]) 
		{
			float x = ((i % WIDTH) + Random::Float()) * size;
			float y = ((i / WIDTH) + Random::Float()) * size;
			factory->emit(this, index, x, y);
		}
	}
}
