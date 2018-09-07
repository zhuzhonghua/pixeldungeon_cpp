#include "postweener.h"
#include "util.h"
#include "visual.h"

PosTweener::PosTweener(Visual* visual, PointF pos, float time)
:Tweener(visual, time)
{
	this->visual = visual;
	start = visual->point();
	end = pos;
}

void PosTweener::updateValues(float progress)
{
	visual->point(GameMath::PointFInter(start, end, progress));
}
