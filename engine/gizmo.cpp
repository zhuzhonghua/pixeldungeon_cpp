#include "stdafx.h"

#include "gizmo.h"

#include "group.h"

Gizmo::Gizmo()
{
	exists = true;
	alive = true;
	active = true;
	visible = true;

	parent = NULL;
	cameraf = NULL;
}

bool Gizmo::isVisible()
{
	if (parent == NULL)
	{
		return visible;
	}
	else
	{
		return visible && parent->isVisible();
	}
}

bool Gizmo::isActive()
{
	if (parent == NULL)
	{
		return active;
	}
	else
	{
		return active && parent->isActive();
	}
}

void Gizmo::killAndErase()
{
	kill();
	if (parent != NULL)
	{
		parent->erase(this);
	}
}

void Gizmo::remove()
{
	if (parent != NULL)
	{
		parent->remove(this);
	}
}

Camera* Gizmo::camera()
{
	if (cameraf != NULL) 
	{
		return cameraf;
	}
	else if (parent != NULL) 
	{
		return parent->camera();
	}
	else 
	{
		return NULL;
	}
}