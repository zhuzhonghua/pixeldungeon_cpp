#pragma once

#include "component.h"

#include "bitmaptextmultiline.h"

class HighlightedText:public Component{
protected:
	BitmapTextMultiline* normal;
	BitmapTextMultiline* highlighted;

	int nColor = 0xFFFFFF;
	int hColor = 0xFFFF44;

protected:
	virtual void layout();
public:
	HighlightedText(float size);
	void text(const std::string& value, int maxWidth);
	void setColor(int n, int h);
};