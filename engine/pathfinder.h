#pragma once

#include <vector>

class PathFinder{
public:
	static std::vector<int> distance;

	static void setMapSize(int width, int height);
	static int getStep(int from, int to, std::vector<bool>& passable);
	static int getStepBack(int cur, int from, std::vector<bool>& passable);
	static void buildDistanceMap(int to, std::vector<bool>& passable, int limit);
private:
	static std::vector<bool> goals;
	static std::vector<int> queue;

	static int size;

	static std::vector<int> dir;

	static bool buildDistanceMap(int from, std::vector<bool>& to, std::vector<bool>& passable);
	static bool buildDistanceMap(int from, int to, std::vector<bool>& passable);
	static int buildEscapeDistanceMap(int cur, int from, float factor, std::vector<bool>& passable);
};