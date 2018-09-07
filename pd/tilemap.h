#pragma once

#include "visual.h"
#include "define.h"

#include <vector>

class SmartTexture;
class TextureFilm;
class FloatBuffer;

class Tilemap :public Visual{
protected:
	SmartTexture* texture;
	TextureFilm* tileset;

	std::vector<int> data;

	int mapWidth;
	int mapHeight;
	int size;

private:
	float cellW;
	float cellH;

protected:
	std::vector<float> vertices;
	FloatBuffer* quads;

	void updateVertices();
public:
	Rect updated;

	Tilemap(const std::string& tx, TextureFilm* tileset);
	~Tilemap();

	void map(const std::vector<int>& data, int cols);
	virtual void draw();
};