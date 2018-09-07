#include "torchhalo.h"
#include "game.h"
#include "util.h"
#include "charsprite.h"

TorchHalo::TorchHalo(CharSprite* sprite)
:Halo(24, 0xFFDDCC, 0.15f)
{
	target = sprite;
	am = 0;
}

void TorchHalo::update()
{
	Halo::update();

	if (phase < 0) 
	{
		if ((phase += Game::elapsed) >= 0) 
		{
			killAndErase();
		}
		else 
		{
			GameMath::PointFSet(&scale, (2 + phase) * radius / RADIUS);
			//scale.set((2 + phase) * radius / RADIUS);
			am = -phase * brightness;
		}
	}
	else if (phase < 1) 
	{
		if ((phase += Game::elapsed) >= 1) 
		{
			phase = 1;
		}
		GameMath::PointFSet(&scale, phase * radius / RADIUS);
		//scale.set(phase * radius / RADIUS);
		am = phase * brightness;
	}

	point(target->x + target->width / 2, target->y + target->height / 2);
}

void TorchHalo::draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	Halo::draw();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
