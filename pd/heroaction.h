#pragma once

class HeroAction{
public:
	virtual ~HeroAction(){}
	int dst;
};

class HeroActionMove :public HeroAction{
public:
	HeroActionMove(int dst)
	{
		this->dst = dst;
	}
};

class HeroActionPickUp:public HeroAction{
public:
	HeroActionPickUp(int dst)
	{
		this->dst = dst;
	}
};

class HeroActionOpenChest :public HeroAction{
public:
	HeroActionOpenChest(int dst)
	{
		this->dst = dst;
	}
};

class HeroActionBuy :public HeroAction{
public:
	HeroActionBuy(int dst)
	{
		this->dst = dst;
	}
};

class NPC;
class HeroActionInteract :public HeroAction{
public:
	NPC* npc;
	HeroActionInteract(NPC* npc)
	{
		this->npc = npc;
	}
};


class HeroActionUnlock :public HeroAction{
public:
	HeroActionUnlock(int dst)
	{
		this->dst = dst;
	}
};

class HeroActionDescend :public HeroAction{
public:
	HeroActionDescend(int dst)
	{
		this->dst = dst;
	}
};

class HeroActionAscend :public HeroAction{
public:
	HeroActionAscend(int dst)
	{
		this->dst = dst;
	}
};

class HeroActionCook :public HeroAction{
public:
	HeroActionCook(int dst)
	{
		this->dst = dst;
	}
};

class Char;
class HeroActionAttack :public HeroAction{
public:
	Char* target;
	HeroActionAttack(Char* target)
	{
		this->target = target;
	}
};