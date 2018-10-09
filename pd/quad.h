#pragma once

#include <vector>

class FloatBuffer;

class Quad{
public:
	// 0---1
	// | \ |
	// 3---2
	static const int SIZE = 6;

	static std::vector<short>* getIndices(int size);
	static void fill(float v[],
		float x1, float x2, float y1, float y2,
		float u1, float u2, float v1, float v2);

	static FloatBuffer* create();
	static FloatBuffer* createSet(int size);
private:
	static std::vector<short> _indices;
};