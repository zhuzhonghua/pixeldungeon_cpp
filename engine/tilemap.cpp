#include "tilemap.h"
#include "texturecache.h"
#include "texturefilm.h"
#include "util.h"
#include "quad.h"
#include "noosascript.h"

void Tilemap::updateVertices()
{
	float y1 = cellH * GameMath::RECTTop(updated);
	float y2 = y1 + cellH;

	for (int i = GameMath::RECTTop(updated); i < GameMath::RECTBottom(updated); i++) 
	{

		float x1 = cellW * GameMath::RECTLeft(updated);
		float x2 = x1 + cellW;

		int pos = i * mapWidth + GameMath::RECTLeft(updated);
		quads->position(16 * pos);

		for (int j = GameMath::RECTLeft(updated); j < GameMath::RECTRight(updated); j++) 
		{

			RectF uv = tileset->get(data[pos++]);

			vertices[0] = x1;
			vertices[1] = y1;

			vertices[2] = GameMath::RECTFLeft(uv);
			vertices[3] = GameMath::RECTFTop(uv);

			vertices[4] = x2;
			vertices[5] = y1;

			vertices[6] = GameMath::RECTFRight(uv);
			vertices[7] = GameMath::RECTFTop(uv);

			vertices[8] = x2;
			vertices[9] = y2;

			vertices[10] = GameMath::RECTFRight(uv);
			vertices[11] = GameMath::RECTFBottom(uv);

			vertices[12] = x1;
			vertices[13] = y2;

			vertices[14] = GameMath::RECTFLeft(uv);
			vertices[15] = GameMath::RECTFBottom(uv);

			quads->put(vertices);

			x1 = x2;
			x2 += cellW;

		}

		y1 = y2;
		y2 += cellH;
	}

	updated = GameMath::RECTEmpty();
}

Tilemap::Tilemap(const std::string& tx, TextureFilm* tileset)
:Visual(0,0,0,0)
{
	this->texture = TextureCache::getTexture(tx);
	this->tileset = tileset;

	RectF r = tileset->get(0);
	cellW = tileset->width(r);
	cellH = tileset->height(r);

	vertices.resize(16);
	quads = NULL;
}

Tilemap::~Tilemap()
{
	delete quads;
	delete tileset;
}

void Tilemap::map(const std::vector<int>& data, int cols)
{
	this->data = data;

	mapWidth = cols;
	mapHeight = data.size() / cols;
	size = mapWidth * mapHeight;

	width = cellW * mapWidth;
	height = cellH * mapHeight;

	quads = Quad::createSet(size);

	updated = GameMath::RECT(0, 0, mapWidth, mapHeight);
}

void Tilemap::draw()
{
	Visual::draw();

	NoosaScript* script = NoosaScript::get();

	texture->bind();

	script->uModel.valueM4(_mat);
	script->lighting(
		rm, gm, bm, am,
		ra, ga, ba, aa);

	if (!GameMath::isRECTEmpty(updated)) 
	{
		updateVertices();
	}

	script->camera(cameraf);
	script->drawQuadSet(quads->buffer(), size);
}
