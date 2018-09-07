#pragma once

#include <string>

#include "SDL_ttf.h"
#include "visual.h"
#include "smarttexture.h"

class BitmapText :public Visual{
public:
	BitmapText();
	BitmapText(const std::string& text);
	BitmapText(Uint16 ch);

	virtual void destroy();
	
	virtual void draw();

	std::string text() {return _text;}
	void text(const std::string& text);
	void measure();

	float baseLine();
	float lineHeight();
	float tracking();
protected:
	bool _dirty;
	static TTF_Font* _font;
	std::string _text;
	std::vector<float> _vertices;
	int realLength;

	SmartTexture* _texture;

	virtual void updateVertices();
	virtual void updateMatrix();
};