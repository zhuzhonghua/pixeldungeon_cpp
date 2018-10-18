#include "wndclass.h"
#include "pixelscene.h"
#include "highlightedtext.h"
#include "badge.h"
#include "bpt.h"

const std::string WndClass::TXT_MASTERY = "lang.mastery";

WndClass::RankingTab::RankingTab(WndTabbed* wnd, const std::string& label, Group* page)
:LabeledTab(wnd, label)
{
	this->page = page;
}

void WndClass::RankingTab::select(bool value)
{
	LabeledTab::select(value);
	if (page != NULL) 
	{
		page->visible = page->active = selected;
	}
}

const std::string WndClass::PerksTab::DOT = "lang.dot";

WndClass::PerksTab::PerksTab(WndClass* wnd)
{
	width = 0;
	height = 0;

	this->wnd = wnd;

	float dotWidth = 0;

	std::vector<std::string> items;
	wnd->cl.perks(items);

	float pos = MARGIN;

	for (int i = 0; i < items.size(); i++) 
	{
		if (i > 0) 
		{
			pos += GAP;
		}

		BitmapText* dot = PixelScene::createText(BPT::getText(DOT), 6);
		dot->x = MARGIN;
		dot->y = pos;
		if (dotWidth == 0) 
		{
			dot->measure();
			dotWidth = dot->getWidth();
		}
		add(dot);

		BitmapTextMultiline* item = PixelScene::createMultiline(items[i], 6);
		item->x = dot->x + dotWidth;
		item->y = pos;
		item->maxWidth = (int)(WIDTH - MARGIN * 2 - dotWidth);
		item->measure();
		add(item);

		pos += item->getHeight();
		float w = item->getWidth();
		if (w > width) 
		{
			width = w;
		}
	}

	width += MARGIN + dotWidth;
	height = pos + MARGIN;
}

WndClass::MasteryTab::MasteryTab(WndClass* wnd)
{
	this->wnd = wnd;
	std::string message;
	switch (wnd->cl.type()) 
	{
	case HeroClass::E_WARRIOR:
		message = HeroSubClass::GLADIATOR.Desc() + "\n\n" + HeroSubClass::BERSERKER.Desc();
		break;
	case HeroClass::E_MAGE:
		message = HeroSubClass::BATTLEMAGE.Desc() + "\n\n" + HeroSubClass::WARLOCK.Desc();
		break;
	case HeroClass::E_ROGUE:
		message = HeroSubClass::FREERUNNER.Desc() + "\n\n" + HeroSubClass::ASSASSIN.Desc();
		break;
	case HeroClass::E_HUNTRESS:
		message = HeroSubClass::SNIPER.Desc() + "\n\n" + HeroSubClass::WARDEN.Desc();
		break;
	}

	HighlightedText* text = new HighlightedText(6);
	text->text(message, WIDTH - MARGIN * 2);
	text->setPos(MARGIN, MARGIN);
	add(text);

	height = text->bottom() + MARGIN;
	width = text->right() + MARGIN;
}

WndClass::WndClass(HeroClass c)
:cl(c)
{
	tabPerks = new PerksTab(this);
	Group::add(tabPerks);

	Tab* tab = new RankingTab(this, cl.name(), tabPerks);
	tab->setSize(TAB_WIDTH, tabHeight());
	add(tab);

	if (Badges::isUnlocked(cl.masteryBadge())) 
	{
		tabMastery = new MasteryTab(this);
		Group::add(tabMastery);
	
		tab = new RankingTab(this, BPT::getText(TXT_MASTERY), tabMastery);
		tab->setSize(TAB_WIDTH, tabHeight());
		add(tab);
	
		resize(
			(int)std::max(tabPerks->width, tabMastery->width),
			(int)std::max(tabPerks->height, tabMastery->height));
	}
	else 
	{
		resize((int)tabPerks->width, (int)tabPerks->height);
	}

	select(0);
}