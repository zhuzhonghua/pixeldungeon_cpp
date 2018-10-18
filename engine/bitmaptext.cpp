#include "stdafx.h"
#include "bitmaptext.h"
#include "util.h"
#include "noosascript.h"

TTF_Font* BitmapText::_font = NULL;

BitmapText::BitmapText() :BitmapText("")
{

}

BitmapText::BitmapText(const std::string& t) : Visual(0,0,0,0)
{
	if (_font == NULL)
	{
		_font = TTF_OpenFont("data/msyhbd.ttf", 32);
		if (!_font) fatalError("fail open font");
		//TTF_SetFontKerning(_font, 2);
	}
	
	_vertices.resize(16);
	_texture = NULL;
	_dirty = false;
	text(t);
}

BitmapText::BitmapText(Uint16 ch) : Visual(0, 0, 0, 0)
{
	if (_font == NULL)
	{
		_font = TTF_OpenFont("data/msyhbd.ttf", 32);
		if (!_font) fatalError("fail open font");
		//TTF_SetFontKerning(_font, 4);
	}
	_vertices.resize(16);
	_texture = NULL;
	_dirty = false;

	const SDL_Color c = { 255, 255, 255 };

	SDL_Surface* surface = TTF_RenderGlyph_Blended(_font, ch, c);
	if (_texture == NULL)
	{
		_texture = new SmartTexture(surface);
	}
	else
	{
		_texture->Texture::bitmap(surface);
	}
	SDL_FreeSurface(surface);
}

void BitmapText::destroy()
{
	//if (_font)
	//{
	//	TTF_CloseFont(_font);
	//}
	//_font = NULL;
}

void BitmapText::updateMatrix()
{
	// "origin" field is ignored
	//Matrix.setIdentity(matrix);
	_mat = Matrix(1.0f);

	_mat = glm::translate(_mat, glm::vec3(x, y, 0));
	//Matrix.translate(matrix, x, y);

	GameMath::MATRIXScale(_mat, scale.x, scale.y);
	//Matrix.scale(matrix, scale.x, scale.y);

	GameMath::MATRIXRotate(_mat, angle);
	//Matrix.rotate(matrix, angle);
}

void BitmapText::draw()
{
	Visual::draw();

	NoosaScript* script = NoosaScript::get();

	if (_texture)
	{
		if (_dirty)
		{
			updateVertices();
			_dirty = false;
		}
		_texture->bind();

		script->camera(camera());

		script->uModel.valueM4(_mat);
		script->lighting(
			rm, gm, bm, am,
			ra, ga, ba, aa);
		script->drawQuad(&_vertices[0]);

		//float vertices[] = { 0, 0, 0, 0,
		//					 1, 0, 1, 0,
		//					 1, 1, 1, 1,
		//					 0, 1, 0, 1 };
		//script->drawQuad(vertices);
	}	
}

void BitmapText::updateVertices()
{
	int w = 0, h = 0;
	TTF_SizeUTF8(_font, _text.c_str(), &w, &h);

	width = w;
	height = h;

	float left = 0;
	float top = 0;
	float right = 1;
	float bottom = 1;

	_vertices[0] = 0;
	_vertices[1] = 0;

	_vertices[2] = left;
	_vertices[3] = top;

	_vertices[4] = width;
	_vertices[5] = 0;

	_vertices[6] = right;
	_vertices[7] = top;

	_vertices[8] = width;
	_vertices[9] = height;

	_vertices[10] = right;
	_vertices[11] = bottom;

	_vertices[12] = 0;
	_vertices[13] = height;

	_vertices[14] = left;
	_vertices[15] = bottom;
}

void BitmapText::text(const std::string& str)
{
	if (str.length() <= 0) return;

	const SDL_Color c = { 255, 255, 255};

	_text = str;

	// TODO: Rewrite,split line to single word
	// dynamic input string
	SDL_Surface* surface = TTF_RenderUTF8_Blended(_font, str.c_str(), c);

	if (_texture == NULL)
	{
		_texture = new SmartTexture(surface);
	}
	else
	{
		_texture->Texture::bitmap(surface);
	}

	SDL_FreeSurface(surface);

	_dirty = true;
}

void BitmapText::measure()
{
	int w=0, h=0;
	TTF_SizeUTF8(_font, _text.c_str(), &w, &h);

	width = w;
	height = h;
}

float BitmapText::baseLine()
{
	return 32 * scale.y;
}

float BitmapText::lineHeight()
{
	return 32 * scale.y;
}

float BitmapText::tracking()
{
	return -1;
}