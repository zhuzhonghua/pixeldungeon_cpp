#include "missilesprite.h"
#include "dungeontilemap.h"
#include "util.h"
#include "postweener.h"
#include "group.h"
#include "item.h"
#include "callback.h"

const float MissileSprite::SPEED = 240.0f;

MissileSprite::MissileSprite()
{
	tag = "MissileSprite";
	originToCenter();
	callback = NULL;
}

void MissileSprite::reset(int from, int to, Item* item, Callback* listener)
{
	if (item == NULL) {
		reset(from, to, 0, NULL, listener);
	}
	else {
		reset(from, to, item->Image(), item->glowing(), listener);
	}
}

void MissileSprite::reset(int from, int to, int image, Glowing* glowing, Callback* listener)
{
	revive();

	view(image, glowing);

	this->callback = listener;

	point(DungeonTilemap::tileToWorld(from));
	PointF dest = DungeonTilemap::tileToWorld(to);

	PointF d = GameMath::PointFDiff(dest, point());
	GameMath::PointFSet(&speed, d);
	GameMath::PointFNormalize(&speed);
	GameMath::PointFScale(&speed, SPEED);

	if (image == 31 || image == 108 || image == 109 || image == 110) {

		angularSpeed = 0;
		angle = 135 - (float)(std::atan2(d.x, d.y) / 3.1415926 * 180);

	}
	else {

		angularSpeed = image == 15 || image == 106 ? 1440 : 720;

	}

	PosTweener* tweener = new PosTweener(this, dest, d.length() / SPEED);
	tweener->listener = this;
	parent->add(tweener);
}

void MissileSprite::onComplete(Tweener* tweener)
{
	kill();
	if (callback != NULL) callback->call();
}
