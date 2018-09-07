#include "pushing.h"
#include "char.h"
#include "game.h"
#include "charsprite.h"
#include "util.h"

Pushing::Pushing(Char* ch, int from, int to)
{
	sprite = ch->sprite;
	this->from = from;
	this->to = to;

	effect = NULL;
}

boolean Pushing::act()
{
	if (sprite != NULL) {

		if (effect == NULL) {
			new Effect(this);
		}
		return false;

	}
	else {

		Actor::remove(this);
		return true;
	}
}

const float Pushing::Effect::DELAY = 0.15f;

Pushing::Effect::Effect(Pushing* pi)
:Visual(0, 0, 0, 0),
p(pi)
{
	point(p->sprite->worldToCamera(p->from));
	end = p->sprite->worldToCamera(p->to);

	GameMath::PointFSet(&speed, 2 * (end.x - x) / DELAY, 2 * (end.y - y) / DELAY);
	//speed.set(2 * (end.x - x) / DELAY, 2 * (end.y - y) / DELAY);
	GameMath::PointFSet(&acc, -speed.x / DELAY, -speed.y / DELAY);
	//acc.set(-speed.x / DELAY, -speed.y / DELAY);

	delay = 0;

	p->sprite->parent->add(this);
}

void Pushing::Effect::update()
{
	Visual::update();

	if ((delay += Game::elapsed) < DELAY) {

		p->sprite->x = x;
		p->sprite->y = y;

	}
	else {

		p->sprite->point(end);

		Actor::remove(p);

		p->next();
		killAndErase();

		delete this;
	}
}
