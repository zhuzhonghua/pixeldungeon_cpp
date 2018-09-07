#pragma once

#include "pixelscene.h"

class IntroScene :public PixelScene{
private:
	static const std::string TEXT;

public:
	virtual void init();
};