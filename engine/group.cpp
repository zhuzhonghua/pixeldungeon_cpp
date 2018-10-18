#include "stdafx.h"

#include "group.h"

Group::Group()
{
	_members.clear();
}

Group::~Group()
{
	this->destroy();
}

void Group::destroy()
{
	for (std::vector<Gizmo*>::iterator itr = _members.begin();
		itr != _members.end(); itr++)
	{
		Gizmo* g = *itr;
		if (g != NULL)
		{
			g->destroy();
			delete g;
		}
	}

	_members.clear();
}

void Group::update()
{
	for (int i=0; i<_members.size();i++)
	{
		Gizmo* g = _members[i];
		if (g != NULL && g->exists && g->active)
		{
			g->update();
		}
	}

	for (int i = 0; i < _toDel.size(); i++)
	{
		delete _toDel[i];
	}
	_toDel.clear();
}

void Group::draw()
{
	for (std::vector<Gizmo*>::iterator itr = _members.begin();
		itr != _members.end(); itr++)
	{
		Gizmo* g = *itr;
		if (g != NULL && g->exists && g->visible)
		{
			g->draw();
		}
	}
}

void Group::kill()
{
	for (std::vector<Gizmo*>::iterator itr = _members.begin();
		itr != _members.end(); itr++)
	{
		Gizmo* g = *itr;
		if (g != NULL && g->exists)
		{
			g->kill();
		}
	}

	Gizmo::kill();
}

int Group::indexOf(Gizmo* g)
{
	int idx = 0;
	for (std::vector<Gizmo*>::iterator itr = _members.begin();
		itr != _members.end(); itr++, idx++)
	{
		if (g != NULL && g == *itr)
		{
			return idx;
		}
	}

	return -1;
}

Gizmo* Group::addToBack(Gizmo* g)
{
	if (g->parent == this) {
		sendToBack(g);
		return g;
	}

	if (g->parent != NULL) {
		g->parent->remove(g);
	}

	if (_members.size() > 0 && _members[0] == NULL)
	{
		_members[0] = g;
		g->parent = this;
		return g;
	}

	_members.insert(_members.begin(), g);
	g->parent = this;

	return g;
}

Gizmo* Group::add(Gizmo* g)
{
	if (g->parent == this)
	{
		return g;
	}

	if (g->parent != NULL)
	{
		g->parent->remove(g);
	}

	for (int i = 0; i < _members.size(); i++)
	{
		if (_members[i] == NULL)
		{
			_members[i] = g;
			g->parent = this;
			return g;
		}
	}

	_members.push_back(g);
	g->parent = this;
	return g;
}

Gizmo* Group::recycle(const std::string& tag)
{
	Gizmo* g = getFirstAvailable(tag);
	if (g != NULL) 
	{
		return g;
	}

	return NULL;
}

Gizmo* Group::getFirstAvailable(const std::string& tag)
{
	for (int i = 0; i < _members.size(); i++)
	{
		Gizmo* g = _members[i];
		std::string name = typeid(*g).name();
		if (g != NULL && !g->exists && name.compare(tag) == 0) 
		{
			return g;
		}
	}

	return NULL;
}

Gizmo* Group::erase(Gizmo* g)
{
	int idx = this->indexOf(g);
	if (idx != -1)
	{
		_members[idx] = NULL;
		g->parent = NULL;
		return g;
	}
	
	return NULL;
}

Gizmo* Group::remove(Gizmo* g, bool autoDel/* = false*/)
{
	for (std::vector<Gizmo*>::iterator itr = _members.begin();
		itr != _members.end(); itr++)
	{
		if (g != NULL && g == *itr)
		{
			_members.erase(itr);
			g->parent = NULL;
			if (autoDel) _toDel.push_back(g);
			return g;
		}
	}

	return NULL;
}

Gizmo* Group::replace(Gizmo* oldOne, Gizmo* newOne)
{
	int idx = this->indexOf(oldOne);
	if (idx != -1)
	{
		_members[idx] = newOne;
		newOne->parent = this;
		oldOne->parent = NULL;
		return newOne;
	}
	else
	{
		return NULL;
	}
}

int Group::countLiving()
{
	int count = 0;
	for (std::vector<Gizmo*>::iterator itr = _members.begin();
		itr != _members.end(); itr++)
	{
		Gizmo* g = *itr;
		if (g != NULL && g->exists && g->alive)
		{
			count++;
		}
	}
	return count;
}

int Group::countDead()
{
	int count = 0;
	for (std::vector<Gizmo*>::iterator itr = _members.begin();
		itr != _members.end(); itr++)
	{
		Gizmo* g = *itr;
		if (g != NULL && !g->alive)
		{
			count++;
		}
	}
	return count;
}

void Group::clear()
{
	for (std::vector<Gizmo*>::iterator itr = _members.begin();
		itr != _members.end(); itr++)
	{
		Gizmo* g = *itr;
		if (g != NULL)
		{
			g->parent = NULL;
		}
	}
	_members.clear();
}

Gizmo* Group::bringToFront(Gizmo* g)
{
	std::vector<Gizmo*>::iterator itr = std::find(_members.begin(), _members.end(), g);
	if (itr != _members.end())
	{
		_members.erase(itr);
		_members.push_back(g);
		return g;
	}
	
	return NULL;
}

Gizmo* Group::sendToBack(Gizmo* g)
{
	std::vector<Gizmo*>::iterator itr = std::find(_members.begin(), _members.end(), g);
	if (itr != _members.end())
	{
		_members.erase(itr);
		_members.insert(_members.begin(), g);
		return g;
	}

	return NULL;
}