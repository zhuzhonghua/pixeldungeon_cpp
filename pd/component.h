#pragma once

#include "group.h"

class Component:public Group {
protected:
	float _x;
	float _y;
	float _width;
	float _height;
public:
	Component() 
	{
		_x = 0;
		_y = 0;
		_width = 0;
		_height = 0;		
	}
	virtual void init()
	{
		createChildren();
	}
	virtual ~Component() { }

	Component* setPos(float x, float y) 
	{
		this->_x = x;
		this->_y = y;
		layout();

		return this;
	}

	Component* setSize(float width, float height) 
	{
		this->_width = width;
		this->_height = height;
		layout();

		return this;
	}

	Component* setRect(float x, float y, float width, float height) 
	{
		this->_x = x;
		this->_y = y;
		this->_width = width;
		this->_height = height;
		layout();

		return this;
	}

	bool inside(float x, float y) { return x >= this->_x && y >= this->_y && x < this->_x + _width && y < this->_y + _height; }
	void fill(Component* c) { setRect(c->_x, c->_y, c->_width, c->_height); }
	float left() { return _x; }
	float right() { return _x + _width; }
	float centerX() { return _x + _width / 2; }
	float top() { return _y; }
	float bottom() { return _y + _height; }
	float centerY() { return _y + _height / 2; }
	float width() { return _width; }
	float height() { return _height; }
protected:
	virtual void createChildren() { }
	virtual void layout() { }
};
