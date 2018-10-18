#pragma once

#include "gizmo.h"
#include "visual.h"

class Tweener :public Gizmo{
public:
	class Listener {
	public:
		virtual void onComplete(Tweener* tweener) = 0;;
	};

	Gizmo* target;

	float interval;
	float elapsed;

	Listener* listener;

	Tweener(Gizmo* target, float interval);
	virtual void update();

protected:
	void onComplete() { if (listener != NULL) listener->onComplete(this); }
	virtual void updateValues(float progress) = 0;
};

class ScaleTweener :public Tweener{
public:
	Visual* visual;

	PointF start;
	PointF end;

	ScaleTweener(Visual* visual, PointF scale, float time);

protected:
	virtual void updateValues(float progress);
};

class CameraScrollTweener :public Tweener{
public:
	Camera* camera;

	PointF start;
	PointF end;

	CameraScrollTweener(Camera* camera, PointF pos, float time);
protected:
	virtual void updateValues(float progress);
};

class Delayer:public Tweener {
public:
	Delayer() :Tweener(NULL, 0) {}
	Delayer(float time):Tweener(NULL, time) {}

protected:
	virtual void updateValues(float progress) {}
};
