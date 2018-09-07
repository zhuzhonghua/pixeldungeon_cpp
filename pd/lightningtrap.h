#pragma once

#include <string>
#include "typedefine.h"

class Char;
class LightningTrap{
private:
	static const std::string name;

public:
	static void trigger(int pos, Char* ch);
	class Electricity:public Object{
	public:
		CLASSOBJECT(Electricity);
	};
	static Electricity* LIGHTNING;
};