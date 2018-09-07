#pragma once

#include "emitter.h"

class CellEmitter{
public:
	static Emitter* get(int cell);
	static Emitter* center(int cell);
	static Emitter* bottom(int cell);
};