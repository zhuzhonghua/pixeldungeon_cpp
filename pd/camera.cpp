#include "stdafx.h"

#include "camera.h"
#include "game.h"
#include "util.h"
#include "visual.h"

float Camera::_invW2;
float Camera::_invH2;

std::vector<Camera*> Camera::_all;

Camera* Camera::mainCamera;

Camera::Camera(int x, int y, int width, int height, float zoom)
{
	_shakeMagX = 10.0f;
	_shakeMagY = 10.0f;
	_shakeTime = 0.0f;
	_shakeDuration = 1.0f;

	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->zoomf = zoom;

	screenWidthf = width*zoom;
	screenHeightf = height*zoom;

	mat = Matrix(1.0f);
	target = NULL;
}

Camera::~Camera()
{

}

Camera* Camera::reset(Camera* newCamera)
{
	_invW2 = 2.0f / Game::width;
	_invH2 = 2.0f / Game::height;

	int length = _all.size();
	for (int i = 0; i < length; i++) 
	{
		_all[i]->destroy();
		delete _all[i];
	}
	_all.clear();

	return mainCamera = add(newCamera);
}

Camera* Camera::add(Camera* camera)
{
	_all.push_back(camera);
	return camera;
}

Camera* Camera::remove(Camera* camera)
{
	std::vector<Camera*>::iterator itr = std::find(_all.begin(), _all.end(), camera);
	if (itr != _all.end())
	{
		_all.erase(itr);
	}
	return camera;
}

Camera* Camera::createFullscreen(float zoom)
{
	int w = std::ceil(Game::width / zoom);
	int h = std::ceil(Game::height / zoom);
	return new Camera((Game::width - w*zoom)/2.0f,
						(Game::height - h*zoom)/2.0f,
						w, h, zoom);
}

void Camera::resize(int width, int height)
{
	this->width = width;
	this->height = height;
	screenWidthf = width*zoomf;
	screenHeightf = height*zoomf;
}

void Camera::focusOn(Visual* visual)
{
	focusOn(visual->center());
}

void Camera::focusOn(float x, float y)
{
	scroll.x = x - width / 2, scroll.y = y - height / 2;
}

void Camera::zoom(float value, float fx, float fy)
{
	zoomf = value;
	width = (int)(screenWidthf / zoomf);
	height = (int)(screenHeightf / zoomf);

	focusOn(fx, fy);
}

void Camera::zoom(float value)
{
	zoom(value,
		scroll.x + width / 2,
		scroll.y + height / 2);
}

void Camera::updateMatrix()
{
	mat[0][0] = +zoomf*_invW2;
	mat[1][1] = -zoomf*_invH2;

	mat[3][0] = -1 + x*_invW2 - (scroll.x + _shakeX)*mat[0][0];
	mat[3][1] = +1 - y*_invH2 - (scroll.y + _shakeY)*mat[1][1];
}

void Camera::update()
{
	Gizmo::update();


	if (target != NULL) 
	{
		focusOn(target);
	}


	//
	if ((_shakeTime -= Game::elapsed) > 0)
	{
		float damping = _shakeTime / _shakeDuration;
		_shakeX = Random::Float(-_shakeMagX, +_shakeMagX) * damping;
		_shakeY = Random::Float(-_shakeMagY, +_shakeMagY) * damping;
	}
	else
	{
		_shakeX = 0;
		_shakeY = 0;
	}

	updateMatrix();
}

void Camera::updateAll()
{
	for (int i = 0; i < _all.size(); i++)
	{
		Camera* c = _all[i];
		if (c->exists && c->active)
		{
			c->update();
		}
	}
}