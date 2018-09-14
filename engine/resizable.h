#pragma once

class Resizable {
public:
	virtual void size(float width, float height) = 0;
	virtual float width() = 0;
	virtual float height() = 0;
};