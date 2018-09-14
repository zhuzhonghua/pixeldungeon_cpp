#pragma once

#include "group.h"
#include "camera.h"

class Scene:public Group{
public:
	Scene();
	~Scene();

	virtual void init(){};
	virtual void pause(){};
	virtual void resume(){}

	virtual Camera* camera() 
	{
		return Camera::mainCamera;
	}

	virtual void onBackPressed(){}
	virtual void onMenuPressed(){}
};