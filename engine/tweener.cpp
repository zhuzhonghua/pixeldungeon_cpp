#include "tweener.h"
#include "visual.h"
#include "game.h"
#include "util.h"

Tweener::Tweener(Gizmo* target, float interval)
{
	this->target = target;
	this->interval = interval;
	listener = NULL;
	elapsed = 0;
}

void Tweener::update()
{
	elapsed += Game::elapsed;
	if (elapsed >= interval) 
	{
		updateValues(1);
		onComplete();
		kill();
	}
	else 
	{
		updateValues(elapsed / interval);
	}
}

ScaleTweener::ScaleTweener(Visual* visual, PointF scale, float time)
:Tweener(visual, time)
{
	this->visual = visual;
	start = visual->scale;
	end = scale;
}

void ScaleTweener::updateValues(float progress)
{	
	visual->scale = GameMath::PointFInter(start, end, progress);
}

CameraScrollTweener::CameraScrollTweener(Camera* camera, PointF pos, float time)
:Tweener(camera, time)
{
	this->camera = camera;
	start = camera->scroll;
	end = pos;
}

void CameraScrollTweener::updateValues(float progress)
{
	camera->scroll = GameMath::PointFInter(start, end, progress);
}