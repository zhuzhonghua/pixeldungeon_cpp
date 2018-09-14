#pragma once

#include "script.h"
#include "camera.h"

class FloatBuffer;
class ShortBuffer;

class NoosaScript :public Script{
public:
	Uniform uCamera;
	Uniform uModel;
	Uniform uTex;
	Uniform uColorM;
	Uniform uColorA;
	Attribute aXY;
	Attribute aUV;

	NoosaScript();
	void use();
	void lighting(float rm, float gm, float bm, float am, float ra, float ga, float ba, float aa);

	void drawQuad(float* buffer);
	void drawQuadSet(float* buffer, int size);
	void drawElements(FloatBuffer* vertices, ShortBuffer* indices, int size);

	void camera(Camera* camera);
	void resetCamera() { _lastCamera = NULL; }

	static NoosaScript* get();
private:
	Camera* _lastCamera;
};