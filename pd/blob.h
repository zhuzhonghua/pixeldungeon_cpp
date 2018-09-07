#pragma once

#include "actor.h"
#include "level.h"
#include "bpt.h"
#include "typedefine.h"

class BlobEmitter;
class Blob :public Actor{
public:
	static const int WIDTH = Level::WIDTH;
	static const int HEIGHT = Level::HEIGHT;
	static const int LENGTH = Level::LENGTH;

	int volume;

	std::vector<int> cur;

	BlobEmitter* emitter;

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);
	virtual bool act();
	virtual void use(BlobEmitter* emitter){ this->emitter = emitter; }
	void seed(int cell, int amount);
	void clear(int cell);
	virtual std::string tileDesc() { return ""; }
	static Blob* seed(int cell, int amount, const std::string& type);
protected:
	std::vector<int> off;
	Blob();
	virtual void evolve();
private:
	static const std::string CUR;
	static const std::string START;

	std::vector<int> trim(int start, int end);
};

FACTORY(Blob);

class BlobFire :public Blob{
protected:
	virtual void evolve();

private:
	void burn(int pos);

public:
	CLASSNAME(BlobFire);

	void seed(int cell, int amount);
	virtual void use(BlobEmitter* emitter);

	virtual String tileDesc() {
		return BPT::getText("lang.blobfire_tiledesc");;
	}
};

class ConfusionGas :public Blob{
public:
	CLASSNAME(ConfusionGas);
	virtual void use(BlobEmitter* emitter);
	virtual String tileDesc() {
		return BPT::getText("lang.ConfusionGas_tileDesc");// "A cloud of confusion gas is swirling here.";
	}
protected:
	virtual void evolve();
};

class ParalyticGas :public Blob{
protected:
	virtual void evolve();

public:
	CLASSNAME(ParalyticGas);
	virtual void use(BlobEmitter* emitter);
	virtual String tileDesc() {
		return BPT::getText("lang.ParalyticGas_tileDesc");// "A cloud of paralytic gas is swirling here.";
	}
};