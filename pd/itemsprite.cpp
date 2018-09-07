#include "itemsprite.h"
#include "util.h"
#include "simpleresource.h"
#include "dungeontilemap.h"
#include "level.h"
#include "gold.h"
#include "cellemitter.h"
#include "speck.h"
#include "dungeon.h"
#include "game.h"
#include "gamescene.h"
#include "terrain.h"
#include "heap.h"
#include "item.h"

const float ItemSprite::DROP_INTERVAL = 0.4f;
TextureFilm* ItemSprite::film = NULL;

ItemSprite::Glowing* ItemSprite::Glowing::WHITE = new Glowing(0xFFFFFF, 0.6f);

ItemSprite::Glowing::Glowing(int color)
:Glowing(color, 1.0f)
{
	
}

ItemSprite::Glowing::Glowing(int color, float period)
{
	this->color = color;

	red = (color >> 16) / 255.0f;
	green = ((color >> 8) & 0xFF) / 255.0f;
	blue = (color & 0xFF) / 255.0f;

	this->period = period;
}

ItemSprite::ItemSprite()
:ItemSprite(ItemSpriteSheet::SMTH, NULL)
{

}

ItemSprite::ItemSprite(Item* item)
: ItemSprite(item->Image(), item->glowing())
{

}

ItemSprite::ItemSprite(int image, Glowing* glowing)
: MovieClip(Assets::ITEMS)
{
	tag = "ItemSprite";
	if (film == NULL) 
	{
		film = new TextureFilm(tex, SIZE, SIZE);
	}

	view(image, glowing);
}

void ItemSprite::originToCenter()
{
	GameMath::PointFSet(&origin, SIZE / 2);
	//origin.set(SIZE / 2);
}

void ItemSprite::link()
{
	link(heap);
}

void ItemSprite::link(Heap* heap)
{
	this->heap = heap;
	view(heap->image(), heap->glowing());
	place(heap->pos);
}

void ItemSprite::revive()
{
	MovieClip::revive();

	GameMath::PointFSet(&speed, 0);
	//speed.set(0);

	GameMath::PointFSet(&acc, 0);
	//acc.set(0);

	dropInterval = 0;

	delete heap;
	heap = NULL;
}

PointF ItemSprite::worldToCamera(int cell)
{
	const int csize = DungeonTilemap::SIZE;

	return PointF(
		cell % Level::WIDTH * csize + (csize - SIZE) * 0.5f,
		cell / Level::WIDTH * csize + (csize - SIZE) * 0.5f
		);
}

void ItemSprite::place(int p)
{
	point(worldToCamera(p));
}

void ItemSprite::drop()
{
	if (heap->isEmpty()) 
	{
		return;
	}

	dropInterval = DROP_INTERVAL;

	GameMath::PointFSet(&speed, 0, -100);
	//speed.set(0, -100);

	GameMath::PointFSet(&acc, 0, -speed.y / DROP_INTERVAL * 2);
	//acc.set(0, -speed.y / DROP_INTERVAL * 2);

	if (visible && heap != NULL && dynamic_cast<Gold*>(heap->peek())) 
	{
		CellEmitter::center(heap->pos)->burst(Speck::factory(Speck::COIN), 5);
		//Sample.INSTANCE.play(Assets::SND_GOLD, 1, 1, Random::Float(0.9f, 1.1f));
	}
}

void ItemSprite::drop(int from)
{
	if (heap->pos == from) 
	{
		drop();
	}
	else 
	{

		float px = x;
		float py = y;
		drop();

		place(from);

		GameMath::PointFOffSet(&speed, (px - x) / DROP_INTERVAL, (py - y) / DROP_INTERVAL);
		//speed.offset((px - x) / DROP_INTERVAL, (py - y) / DROP_INTERVAL);
	}
}

ItemSprite* ItemSprite::view(int image, ItemSprite::Glowing* glowing)
{
	frame(film->get(image));
	if ((this->glowing = glowing) == NULL) 
	{
		resetColor();
	}
	return this;
}

void ItemSprite::update()
{
	MovieClip::update();

	visible = (heap == NULL || Dungeon::visible[heap->pos]);

	if (dropInterval > 0 && (dropInterval -= Game::elapsed) <= 0) 
	{
		GameMath::PointFSet(&speed, 0);
		//speed.set(0);
		GameMath::PointFSet(&acc, 0);
		//acc.set(0);
		place(heap->pos);

		if (visible) 
		{
			bool water = Level::water[heap->pos];

			if (water) 
			{
				GameScene::ripple(heap->pos);
			}
			else 
			{
				int cell = Dungeon::level->map[heap->pos];
				water = (cell == Terrain::WELL || cell == Terrain::ALCHEMY);
			}

			if (!(dynamic_cast<Gold*>(heap->peek()) != NULL)) 
			{
				//Sample.INSTANCE.play(water ? Assets.SND_WATER : Assets.SND_STEP, 0.8f, 0.8f, 1.2f);
			}
		}
	}

	if (visible && glowing != NULL) 
	{
		if (glowUp && (phase += Game::elapsed) > glowing->period) 
		{
			glowUp = false;
			phase = glowing->period;
		}
		else if (!glowUp && (phase -= Game::elapsed) < 0) 
		{
			glowUp = true;
			phase = 0;
		}

		float value = phase / glowing->period * 0.6f;

		rm = gm = bm = 1 - value;
		ra = glowing->red * value;
		ga = glowing->green * value;
		ba = glowing->blue * value;
	}
}

int ItemSprite::pick(int index, int x, int y)
{
	SmartTexture* tex = TextureCache::getTexture(Assets::ITEMS);
	int rows = tex->width / SIZE;
	int row = index / rows;
	int col = index % rows;
	return tex->getPixel(col * SIZE + x, row * SIZE + y);
}