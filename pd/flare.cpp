#include "flare.h"
#include "smarttexture.h"
#include "group.h"
#include "game.h"
#include "noosascript.h"

void Flare::drawRays()
{
	NoosaScript* script = NoosaScript::get();

	texture->bind();

	script->uModel.valueM4(_mat);
	script->lighting(
		rm, gm, bm, am,
		ra, ga, ba, aa);

	script->camera(cameraf);
	script->drawElements(&vertices, &indices, nRays * 3);
}

Flare::Flare(int nRays, float radius)
:Visual(0, 0, 0, 0),
vertices((nRays * 2 + 1) * 4),
indices(nRays * 3)
{
	duration = 0;
	lightMode = true;

	int gradient[] = { 0xFFFFFFFF, 0x00FFFFFF };
	texture = new Gradient(std::vector<int>(gradient, gradient+sizeof(gradient)/sizeof(int)));

	this->nRays = nRays;

	std::vector<float> v(4);

	v[0] = 0;
	v[1] = 0;
	v[2] = 0.25f;
	v[3] = 0;
	vertices.put(v);

	v[2] = 0.75f;
	v[3] = 0;

	for (int i = 0; i < nRays; i++) {

		float a = i * 3.1415926f * 2 / nRays;
		v[0] = std::cos(a) * radius;
		v[1] = std::sin(a) * radius;
		vertices.put(v);

		a += 3.1415926f * 2 / nRays / 2;
		v[0] = std::cos(a) * radius;
		v[1] = std::sin(a) * radius;
		vertices.put(v);

		indices.put((short)0);
		indices.put((short)(1 + i * 2));
		indices.put((short)(2 + i * 2));
	}

	indices.position(0);
}

Flare* Flare::color(int color, bool lightMode)
{
	this->lightMode = lightMode;
	hardlight(color);

	return this;
}

Flare* Flare::show(Visual* visual, float duration)
{
	point(visual->center());
	visual->parent->addToBack(this);

	lifespan = this->duration = duration;

	return this;
}

Flare* Flare::show(Group* parent, PointF pos, float duration)
{
	point(pos);
	parent->add(this);

	lifespan = this->duration = duration;

	return this;
}

void Flare::update()
{
	Visual::update();

	if (duration > 0) {
		if ((lifespan -= Game::elapsed) > 0) {

			float p = 1 - lifespan / duration;	// 0 -> 1
			p = p < 0.25f ? p * 4 : (1 - p) * 1.333f;
			//scale.set(p);
			GameMath::PointFSet(&scale, p);
			alpha(p);

		}
		else {
			killAndErase();
		}
	}
}

void Flare::draw()
{
	Visual::draw();

	if (lightMode) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		drawRays();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else {
		drawRays();
	}
}
