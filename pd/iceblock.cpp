#include "iceblock.h"
#include "charsprite.h"
#include "game.h"
#include "splash.h"

IceBlock::IceBlock(CharSprite* target)
{
	this->target = target;
	phase = 0;
}

void IceBlock::update()
{
	Gizmo::update();

	if ((phase += Game::elapsed * 2) < 1) 
	{
		target->tint(0.83f, 1.17f, 1.33f, phase * 0.6f);
	}
	else 
	{
		target->tint(0.83f, 1.17f, 1.33f, 0.6f);
	}
}

void IceBlock::melt()
{
	target->resetColor();
	killAndErase();

	if (visible) 
	{
		Splash::at(target->center(), 0xFFB2D6FF, 5);
		//Sample.INSTANCE.play(Assets.SND_SHATTER);
	}
}

IceBlock* IceBlock::freeze(CharSprite* sprite)
{
	IceBlock* iceBlock = new IceBlock(sprite);
	sprite->parent->add(iceBlock);

	return iceBlock;
}
