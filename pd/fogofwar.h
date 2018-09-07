#pragma once

#include "image.h"

class FogOfWar :public Image{
private:
	class FogTexture :public SmartTexture{
	public:
		FogOfWar* fow;
		FogTexture(FogOfWar* fowp);
		//virtual void reload();
	};
private:
	static const int VISIBLE = 0x00000000;
	static const int VISITED = 0xcc111111;
	static const int MAPPED = 0xcc442211;
	static const int INVISIBLE = 0xFF000000;

	std::vector<int> pixels;

	int pWidth;
	int pHeight;

	int width2;
	int height2;

public:
	FogOfWar(int mapWidth, int mapHeight);

	void updateVisibility(const std::vector<bool>& visible, const std::vector<bool>& visited, const std::vector<bool>& mapped);
};