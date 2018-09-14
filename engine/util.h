#pragma once

#include <time.h>
#include <random>
#include <set>
#include <list>
#include <map>
#include "define.h"
#include "regex/pcrecpp.h"

class Room;
class Random{
	static Random* _inst;
	static Random* inst();

	Random();
public:
	static float Float(float begin, float end);
	static float Float(float end);
	static float Float();
	static float random();
	static int Int(int begin, int end);
	static int IntRange(int begin, int end);
	static int NormalIntRange(int min, int max);
	static int Int(int end);
	static Room* element(const std::set<Room*>& s);
	static int chances(std::vector<float> chas);
};

template<class T>
class RandomT{
public:
	static T element(const std::list < T >& s)
	{
		int i = Random::Int(0, s.size());
		int c = 0;
		for (std::list<T>::const_iterator itr = s.begin();
			itr != s.end(); itr++)
		{
			if (c == i) return *itr;
			c++;
		}
		return (T)NULL;
	}
	static T element(const std::set < T >& s)
	{
		int i = Random::Int(0, s.size());
		int c = 0;
		for (std::set<T>::const_iterator itr = s.begin();
			itr != s.end(); itr++)
		{
			if (c == i) return *itr;
			c++;
		}
		return (T)NULL;
	}
	static T element(const std::vector < T >& s)
	{
		int i = Random::Int(0, s.size());
		int c = 0;
		for (std::vector<T>::const_iterator itr = s.begin();
			itr != s.end(); itr++)
		{
			if (c == i) return *itr;
			c++;
		}
		return (T)NULL;
	}
	static T chances(const std::map < T, float >& chs)
	{
		int size = chs.size();
		std::vector<T> values(size);
		int c = 0;
		for (std::map < T, float >::const_iterator itr = chs.begin();
			itr != chs.end(); itr++){
			values[c++] = itr->first;
		}

		std::vector<float> probs(size);
		float sum = 0;
		for (int i = 0; i < size; i++) {
			probs[i] = chs.find(values[i])->second;
			sum += probs[i];
		}

		float value = Random::Float(sum);

		sum = probs[0];
		for (int i = 0; i < size; i++) {
			if (value < sum) {
				return (T)values[i];
			}
			sum += probs[i + 1];
		}

		return (T)NULL;
	}
};

class GameMath {
public:
	static const float POINTF_PI;
	static const float POINTF_PI2;
	static const float POINTF_G2R;
	static const float G2RAD;

	static float speed(float speed, float acc);
	static float gate(float min, float value, float max);
	static void MATRIXRotate(Matrix& mat, float angle);
	static void MATRIXScale(Matrix& mat, float sx, float sy);
	static void MATRIXSkewX(Matrix& mat, float a);
	static void RECTFOffset(RectF& frame, float dx, float dy);
	static float RECTFWidth(const RectF& frame);
	static float RECTFHeight(const RectF& frame);
	static RectF RECTF(float left, float top, float right, float bottom);
	static float RECTFLeft(const RectF& frame);
	static float RECTFTop(const RectF& frame);
	static float RECTFRight(const RectF& frame);
	static float RECTFBottom(const RectF& frame);
	
	static Rect RECT(int left, int top, int right, int bottom);
	static Rect RECTEmpty();
	static Rect RECTIntersect(const Rect& a, const Rect& b);
	static int RECTWidth(const Rect& frame);
	static int RECTHeight(const Rect& frame);
	static int RECTLeft(const Rect& frame);
	static int RECTTop(const Rect& frame);
	static int RECTRight(const Rect& frame);
	static int RECTBottom(const Rect& frame);
	static int RECTSquare(const Rect& frame);
	static bool isRECTEmpty(const Rect& frame);
	static Rect* RECTUnion(Rect* frame, int x, int y);

	static float PointFLength(PointF* p);
	static PointF* PointFNormalize(PointF* p);
	static PointF* PointFSet(PointF* p, PointF p2);
	static PointF* PointFSet(PointF* p, float v);
	static PointF* PointFSet(PointF* p, float x, float y);
	static PointF* PointFOffSet(PointF* p, PointF dp);
	static PointF* PointFOffSet(PointF* p, PointF* dp);
	static PointF* PointFOffSet(PointF* p, float dx, float dy);
	static PointF* PointFPolar(PointF* p, float a, float l);
	static PointF* PointFScale(PointF* p, float f);
	static PointF* PointFInvScale(PointF* p, float f);
	static PointF* PointFNegate(PointF* p);
	static Point PointFFloor(PointF* a);
	static PointF PointFInter(PointF a, PointF b, float d);
	static float PointFAngle(PointF start, PointF end);
	static float PointFDistance(PointF a, PointF b);
	static PointF PointFDiff(PointF a, PointF b);
	static bool PointFEqual(PointF a, PointF b);

	static void splitString(const std::string& s, std::vector<std::string>& v, const std::string& c);
	static void splitStringRe(const std::string& s, std::vector<std::string>& v, pcrecpp::RE re);
	static void splitUTF8String(const std::string& s, std::vector<Uint16>& chs);
	static void splitUTF8(const std::string& s, std::vector<std::string>& chs);

	static std::string format(const char* format, ...);
};

class Integer{
public:
	static std::string toString(int i);
};
class ColorMath 
{
public: 
	static int interpolate(int A, int B, float p);
	static int interpolate(float p, const std::vector<int>& colors);
	static int random(int a, int b);
};

class IOManager
{
protected:
	static std::string _buffer;
public:	
	static bool readFileToBuffer(const std::string& filePath, std::vector<unsigned char> &buffer);
	static bool readFileToBuffer(const std::string& filePath, std::string &buffer);
	static bool readFileToBuffer(const std::string& filePath, std::stringstream &buffer, bool ignore=false);
	static bool writeFile(const std::string& filePath, std::string& buffer);
};

class FloatBuffer{
protected:
	int pos;
	std::vector<float> buf;
public:
	FloatBuffer(int size);
	void position(int pos);
	void put(const std::vector<float>& indices);
	float* buffer() { return &buf[0]; }
};


class ShortBuffer{
protected:
	int pos;
	std::vector<short> buf;
public:
	ShortBuffer(int size);
	void position(int pos);
	void put(const std::vector<short>& indices);
	void put(short s);
	short* buffer() { return &buf[0]; }
};

class Patch{
private:
	static std::vector<bool> cur;
	static std::vector<bool> off;

public:
	static std::vector<bool> generate(float seed, int nGen);
};

template<class T>
class Arrays{
public:
	static void fill(std::vector<T>& arr, T d)
	{
		for (int i = 0; i < arr.size(); i++)
		{
			arr[i] = d;
		}
	}
	static void fill(std::vector<T>& arr, int from, int to, T d)
	{
		for (int i = from; i < to; i++)
		{
			arr[i] = d;
		}
	}
	static void arraycopy(std::vector<T>& src, int srcFrom, std::vector<T>& dst, int dstFrom, int len)
	{
		for (int i = 0; i < len; i++)
		{
			dst[dstFrom + i] = src[srcFrom + i];
		}
	}
};

class BArray{
public:
	static std::vector<bool> not(std::vector<bool>&a, std::vector<bool>& result);
	static std::vector<bool> or(std::vector<bool>& a, std::vector<bool>& b, std::vector<bool>& result);
};

class Ballistica{
public:
	static std::vector<int> trace;
	static int distance;

	static int cast(int from, int to, bool magic, bool hitChars);
};
