#pragma once

#include "image.h"

class BusyIndicator :public Image{
public:
	BusyIndicator();
	virtual void update();
};