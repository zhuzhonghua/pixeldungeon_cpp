#include "lightning.h"
#include "simpleresource.h"
#include "image.h"
#include "level.h"
#include "dungeontilemap.h"
#include "util.h"
#include "game.h"
#include "callback.h"

const float Lightning::DURATION = 0.3f;

const double Lightning::A = 180 / 3.1415926f;

Lightning::Lightning(std::vector<int> cells, int length, Callback* callback)
{
	this->callback = callback;

	Image* proto = Effects::get(Effects::Type::LIGHTNING);
	float ox = 0;
	float oy = proto->height / 2;

	this->length = length;
	cx.resize(length);
	cy.resize(length);

	for (int i = 0; i < length; i++) {
		int c = cells[i];
		cx[i] = (c % Level::WIDTH + 0.5f) * DungeonTilemap::SIZE;
		cy[i] = (c / Level::WIDTH + 0.5f) * DungeonTilemap::SIZE;
	}

	arcsS.resize(length - 1);
	arcsE.resize(length - 1);
	for (int i = 0; i < length - 1; i++) {

		Image* arc = arcsS[i] = new Image(proto);

		arc->x = cx[i] - arc->origin.x;
		arc->y = cy[i] - arc->origin.y;

		GameMath::PointFSet(&arc->origin, ox, oy);
		//arc.origin.set(ox, oy);
		add(arc);

		arc = arcsE[i] = new Image(proto);
		GameMath::PointFSet(&arc->origin, ox, oy);
		//arc.origin.set(ox, oy);
		add(arc);
	}

	life = DURATION;

	//Sample.INSTANCE.play(Assets.SND_LIGHTNING);
	delete proto;
}

void Lightning::update()
{
	Group::update();

	if ((life -= Game::elapsed) < 0) {

		killAndErase();
		if (callback != NULL) {
			callback->call();
		}

	}
	else {

		float alpha = life / DURATION;

		for (int i = 0; i < length - 1; i++) {

			float sx = cx[i];
			float sy = cy[i];
			float ex = cx[i + 1];
			float ey = cy[i + 1];

			float x2 = (sx + ex) / 2 + Random::Float(-4, +4);
			float y2 = (sy + ey) / 2 + Random::Float(-4, +4);

			float dx = x2 - sx;
			float dy = y2 - sy;
			Image arc = arcsS[i];
			arc.am = alpha;
			arc.angle = (float)(std::atan2(dy, dx) * A);
			arc.scale.x = (float)std::sqrt(dx * dx + dy * dy) / arc.width;

			dx = ex - x2;
			dy = ey - y2;
			arc = arcsE[i];
			arc.am = alpha;
			arc.angle = (float)(std::atan2(dy, dx) * A);
			arc.scale.x = (float)std::sqrt(dx * dx + dy * dy) / arc.width;
			arc.x = x2 - arc.origin.x;
			arc.y = y2 - arc.origin.x;
		}
	}
}

void Lightning::draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	Group::draw();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
