#include "buffindicator.h"
#include "dungeon.h"
#include "texturecache.h"
#include "define.h"
#include "texturefilm.h"
#include "buff.h"
#include "util.h"
#include "alphatweener.h"
#include "char.h"
#include "hero.h"

BuffIndicator* BuffIndicator::heroInstance;

BuffIndicator::BuffIndicator(Char* ch)
{
	init();
	this->ch = ch;

	if (ch == Dungeon::hero)
	{
		heroInstance = this;
	}
}

void BuffIndicator::destroy()
{
	Component::destroy();
	if (this == heroInstance)
	{
		heroInstance = NULL;
	}
}

void BuffIndicator::refreshHero()
{
	if (heroInstance != NULL)
	{
		heroInstance->layout();
	}
}

void BuffIndicator::createChildren()
{
	texture = TextureCache::getTexture(Assets::BUFFS_SMALL);
	film = new TextureFilm(texture, SIZE, SIZE);
}

namespace{
	class AlphaTweenerNew :public AlphaTweener{
	public:
		AlphaTweenerNew(Visual* image, float alpha, float time)
			:AlphaTweener(image, alpha, time)
		{}
	protected:
		virtual void updateValues(float progress) 
		{
			AlphaTweener::updateValues(progress);
			GameMath::PointFSet(&image->scale, 1 + 5 * progress);
			//image->scale.set(1 + 5 * progress);
		};
	};
}
void BuffIndicator::layout()
{
	clear();

	std::map<int, Image*> newIcons;
	
	for (std::set<Buff*>::iterator itr = ch->buffs.begin();
		itr != ch->buffs.end(); itr++)
	{
		Buff* buff = *itr;
		int icon = buff->icon();
		if (icon != NONE) 
		{
			Image* img = new Image(texture);
			img->frame(film->get(icon));
			img->x = _x + _members.size() * (SIZE + 2);
			img->y = _y;
			add(img);

			newIcons.insert(std::make_pair(icon, img));
		}
	}
	
	for (std::map<int, Image*>::iterator itr = icons.begin();
		itr != icons.end(); itr++)
	{
		int key = itr->first;
		if (newIcons.find(key) == newIcons.end()) 
		{
			Image* icon = itr->second;
			GameMath::PointFSet(&icon->origin, SIZE / 2);
			//icon.origin.set(SIZE / 2);
			add(icon);
			add(new AlphaTweenerNew(icon, 0, 0.6f));
		}
	}

	icons = newIcons;
}
