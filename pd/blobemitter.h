#pragma once

#include "emitter.h"

class Blob;
class BlobEmitter :public Emitter{
private:
	static const int WIDTH;
	static const int LENGTH;

	Blob* blob;

public:
	BlobEmitter(Blob* blob);

protected:
	virtual void emit(int index);
};