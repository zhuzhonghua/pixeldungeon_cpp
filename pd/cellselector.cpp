#include "cellselector.h"
#include "dungeontilemap.h"
#include "pixelscene.h"
#include "gamescene.h"
#include "pixeldungeon.h"

CellSelector::CellSelector(DungeonTilemap* map)
:TouchArea(map)
{
	listener = NULL;
	enabled = false;
	dragging = false;
	pinching = false;
	another = NULL;

	cameraf = map->camera();

	dragThreshold = PixelScene::defaultZoom * DungeonTilemap::SIZE / 2;
}

void CellSelector::select(int cell)
{
	if (enabled && listener != NULL && cell != -1) 
	{
		listener->onSelect(cell);
		GameScene::ready();
	}
	else 
	{
		GameScene::cancel();
	}
}

void CellSelector::cancel()
{
	if (listener != NULL) 
	{
		listener->onSelect(NULL);
	}

	GameScene::ready();
}

void CellSelector::onClick(TouchScreen::Touch* touch)
{
	if (dragging) 
	{
		dragging = false;
	}
	else 
	{
		select(((DungeonTilemap*)target)->screenToTile(
			(int)touch->current.x,
			(int)touch->current.y));
	}
}

void CellSelector::onTouchDown(TouchScreen::Touch* t)
{
	if (t != _touch && another == NULL) 
	{
		if (!_touch->down) 
		{
			_touch = t;
			onTouchDown(t);
			return;
		}

		pinching = true;

		another = t;
		startSpan = GameMath::PointFDistance(_touch->current, another->current);
		startZoom = cameraf->zoomf;

		dragging = false;
	}
}

void CellSelector::onTouchUp(TouchScreen::Touch* t)
{
	if (pinching && (t == _touch || t == another)) 
	{
		pinching = false;

		int zoom = std::round(cameraf->zoomf);
		cameraf->zoom(zoom);
		PixelDungeon::zoom((int)(zoom - PixelScene::defaultZoom));

		dragging = true;
		if (t == _touch) 
		{
			_touch = another;
		}
		another = NULL;

		lastPos = _touch->current;
	}
}

void CellSelector::onDrag(TouchScreen::Touch* t)
{
	cameraf->target = NULL;

	if (pinching) 
	{
		float curSpan = GameMath::PointFDistance(_touch->current, another->current);
		cameraf->zoom(GameMath::gate(
			PixelScene::minZoom,
			startZoom * curSpan / startSpan,
			PixelScene::maxZoom));
	}
	else 
	{
		if (!dragging && GameMath::PointFDistance(t->current, t->start) > dragThreshold) 
		{
			dragging = true;
			lastPos = t->current;
		}
		else if (dragging) 
		{
			PointF p = GameMath::PointFDiff(lastPos, t->current);
			GameMath::PointFInvScale(&p, cameraf->zoomf);

			GameMath::PointFOffSet(&cameraf->scroll, &p);

			lastPos = t->current;
		}
	}
}
