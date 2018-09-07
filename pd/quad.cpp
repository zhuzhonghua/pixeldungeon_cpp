#include "quad.h"
#include "util.h"

std::vector<short> Quad::_indices;

std::vector<short>* Quad::getIndices(int size)
{
	if (size > _indices.size())
	{
		_indices.resize(size*SIZE);

		int pos = 0;
		int limit = size * 4;
		for (int ofs = 0; ofs < limit; ofs += 4)
		{
			_indices[pos++] = ofs + 0;
			_indices[pos++] = ofs + 1;
			_indices[pos++] = ofs + 2;
			_indices[pos++] = ofs + 0;
			_indices[pos++] = ofs + 2;
			_indices[pos++] = ofs + 3;
		}
	}

	return &_indices;
}

void Quad::fill(float v[],
	float x1, float x2, float y1, float y2,
	float u1, float u2, float v1, float v2) 
{
	v[0] = x1;
	v[1] = y1;
	v[2] = u1;
	v[3] = v1;

	v[4] = x2;
	v[5] = y1;
	v[6] = u2;
	v[7] = v1;

	v[8] = x2;
	v[9] = y2;
	v[10] = u2;
	v[11] = v2;

	v[12] = x1;
	v[13] = y2;
	v[14] = u1;
	v[15] = v2;
}

FloatBuffer* Quad::createSet(int size)
{
	return new FloatBuffer(size*16);
}

FloatBuffer* Quad::create()
{
	return new FloatBuffer(16);
}
