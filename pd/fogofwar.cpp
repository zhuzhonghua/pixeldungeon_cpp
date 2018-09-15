#include "fogofwar.h"
#include "dungeontilemap.h"
#include "texturecache.h"
#include "util.h"
#include "gamescene.h"

FogOfWar::FogOfWar(int mapWidth, int mapHeight)
{
	pWidth = mapWidth + 1;
	pHeight = mapHeight + 1;

	width2 = 1;
	while (width2 < pWidth) 
	{
		width2 <<= 1;
	}

	height2 = 1;
	while (height2 < pHeight) 
	{
		height2 <<= 1;
	}

	float size = DungeonTilemap::SIZE;
	width = width2 * size;
	height = height2 * size;

	texture(new FogTexture(this));

	GameMath::PointFSet(&scale, DungeonTilemap::SIZE, DungeonTilemap::SIZE);

	x = y = -size / 2;
}

void FogOfWar::updateVisibility(const std::vector<bool>& visible, const std::vector<bool>& visited, const std::vector<bool>& mapped)
{
	if (pixels.size() <= 0) 
	{
		pixels.resize(width2 * height2);
		Arrays<int>::fill(pixels, INVISIBLE);
	}

	for (int i = 1; i < pHeight - 1; i++) 
	{
		int pos = (pWidth - 1) * i;
		for (int j = 1; j < pWidth - 1; j++) 
		{
			pos++;
			int c = INVISIBLE;
			if (visible[pos] && visible[pos - (pWidth - 1)] &&
				visible[pos - 1] && visible[pos - (pWidth - 1) - 1]) 
			{
				c = VISIBLE;
			}
			else
			if (visited[pos] && visited[pos - (pWidth - 1)] &&
				visited[pos - 1] && visited[pos - (pWidth - 1) - 1]) 
			{
				c = VISITED;
			}
			else
			if (mapped[pos] && mapped[pos - (pWidth - 1)] &&
				mapped[pos - 1] && mapped[pos - (pWidth - 1) - 1]) 
			{
				c = MAPPED;
			}
			//if (Random::Int(100) < 20)
			//{
			//	c = VISIBLE;
			//}

			pixels[i * width2 + j] = c;
		}
	}

	tex->pixels(width2, height2, pixels);
}

FogOfWar::FogTexture::FogTexture(FogOfWar* fowp)
:SmartTexture(TextureCache::createSurface(fowp->width2, fowp->height2, 0)),
fow(fowp)
{
	filter(Texture::LINEAR, Texture::LINEAR);
	TextureCache::add<FogOfWar>(this);
}

//void FogOfWar::FogTexture::reload()
//{
//	//SmartTexture::reload();
//	GameScene::afterObserve();
//}
