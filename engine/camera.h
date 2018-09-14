#pragma once

#include <vector>
#include "glm/glm.hpp"

#include "define.h"

#include "gizmo.h"

class Visual;

class Camera :public Gizmo{
public:
	Camera(int x, int y, int width, int height, float zoom);
	virtual	~Camera();

	static Camera* reset(Camera* newCamera);
	static Camera* add(Camera* camera);
	static Camera* remove(Camera* camera);
	static Camera* createFullscreen(float zoom);
	static Camera* reset() {
		return reset(createFullscreen(1));
	}
	static void updateAll();

	void resize(int width, int height);
	PointF center() { return PointF(width / 2, height / 2); }
	bool hitTest(float x, float y) { return x >= this->x && y >= this->y && x < (this->x + screenWidthf) && y < (this->y + screenHeightf); }
	void focusOn(float x, float y);
	void focusOn(PointF p) { focusOn(p.x, p.y); }
	void focusOn(Visual* visual);
	void shake(float magnitude, float duration) { _shakeMagX = _shakeMagY = magnitude; _shakeTime = _shakeDuration = duration; }
	float screenHeight() { return height*zoomf; }
	float screenWidth() { return width * zoomf; }
	Point cameraToScreen(float x, float y) { return Point((x - scroll.x)*zoomf + this->x, (y - scroll.y)*zoomf + this->y); }
	PointF screenToCamera(int x, int y) { return PointF((x - this->x) / zoomf + scroll.x, (y - this->y) / zoomf + scroll.y); }

	void zoom(float value, float fx, float fy);
	void zoom(float value);
	void update();
	virtual void updateMatrix();

	float zoomf;

	int x;
	int y;
	int width;
	int height;

	int screenWidthf;
	int screenHeightf;

	PointF	scroll;
	Visual* target;
	Matrix	mat;

	static Camera* mainCamera;
protected:
	static std::vector<Camera*> _all;
	static float _invW2;
	static float _invH2;

	

	float _shakeX;
	float _shakeY;
private:
	float _shakeMagX;
	float _shakeMagY;
	float _shakeTime;
	float _shakeDuration;
};