#pragma once

#include "tag.h"

class Image;
class ResumeButton :public Tag{
private:
	Image* icon;

public:
	ResumeButton();

	virtual void update();
protected:
	virtual void createChildren();
	virtual void layout();
	virtual void onClick();
};