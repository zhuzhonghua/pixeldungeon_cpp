#include "archs.h"

#include "skinnedblock.h"
#include "define.h"
#include "game.h"

float Archs::_offsB = 0;
float Archs::_offsF = 0;

Archs::Archs()
:reversed(false)
{
	init();
}

void Archs::createChildren()
{
	//_arcsBg = new SkinnedBlock(1, 1, Assets::ARCS_BG);
	////_arcsBg = new SkinnedBlock(1, 1, Assets::ARCS_FG);
	//_arcsBg->autoAdjust = true;
	//_arcsBg->offsetTo(0, _offsB);
	//add(_arcsBg);

	_arcsFg = new SkinnedBlock(1, 1, Assets::ARCS_FG);
	_arcsFg->autoAdjust = true;
	_arcsFg->offsetTo(0, _offsF);
	add(_arcsFg);
}

void Archs::layout()
{
	//_arcsBg->size(_width, _height);
	//_arcsBg->offset(_arcsBg->tex->width / 4 - ((int)_width % _arcsBg->tex->width) / 2, 0);

	_arcsFg->size(_width, _height);
	_arcsFg->offset(_arcsFg->tex->width / 4 - ((int)_width % _arcsFg->tex->width) / 2, 0);
}

void Archs::update()
{
	Component::update();

	float shift = Game::elapsed * SCROLL_SPEED;
	if (reversed) 
	{
		shift = -shift;
	}

	//_arcsBg->offset(0, shift);
	_arcsFg->offset(0, shift * 2);

	//_offsB = _arcsBg->offsetY();
	_offsF = _arcsFg->offsetY();
}