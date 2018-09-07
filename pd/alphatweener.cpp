#include "alphatweener.h"
#include "visual.h"

AlphaTweener::AlphaTweener(Visual* image, float alpha, float time)
:Tweener(image, time)
{
	this->image = image;
	start = image->alpha();
	delta = alpha - start;
}

void AlphaTweener::updateValues(float progress)
{
	image->alpha(start + delta * progress);
}
