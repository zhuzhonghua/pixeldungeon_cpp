#pragma once

#include "gizmo.h"

#include <vector>

class Group:public Gizmo{
public:
	Group();
	~Group();

	void destroy();
	void update();
	void draw();
	void kill();

	int indexOf(Gizmo* g);
	Gizmo* addToBack(Gizmo* g);
	Gizmo* add(Gizmo* g);

	Gizmo* recycle(const std::string& tag);

	Gizmo* getFirstAvailable(const std::string& tag);

	Gizmo* erase(Gizmo* g);
	Gizmo* remove(Gizmo* g, bool autoDel=false);

	Gizmo* replace(Gizmo* oldOne, Gizmo* newOne);
	int countLiving();
	int countDead();
	void clear();
	Gizmo*		bringToFront(Gizmo* g);
	Gizmo*		sendToBack(Gizmo* g);
public:

protected:
	std::vector<Gizmo*> _members;
	std::vector<Gizmo*> _toDel;
};