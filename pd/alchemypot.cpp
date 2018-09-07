#include "alchemypot.h"
#include "bpt.h"
#include "wndbag.h"
#include "gamescene.h"
#include "item.h"

const String AlchemyPot::TXT_SELECT_SEED = BPT::getText("lang.AlchemyPot_Select_Seed");// "Select a seed to throw";

Hero* AlchemyPot::hero;

int AlchemyPot::pos;

void AlchemyPot::operate(Hero* hero, int pos)
{
	AlchemyPot::hero = hero;
	AlchemyPot::pos = pos;

	GameScene::selectItem(itemSelector, WndBag::Mode::SEED, TXT_SELECT_SEED);
}

WndBag::Listener* AlchemyPot::itemSelector = new WndBagListenerNew;;
