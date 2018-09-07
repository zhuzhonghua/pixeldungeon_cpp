#pragma once

#include "wndtabbed.h"
#include "heroclass.h"

class WndClass :public WndTabbed{
private:
	static const std::string TXT_MASTERY;
	static const int WIDTH = 110;
	static const int TAB_WIDTH = 50;

	class RankingTab :public LabeledTab{
	private:
		Group* page;
	public:
		RankingTab(WndTabbed* wnd, const std::string& label, Group* page);
	protected:
		virtual void select(bool value);
	};

	class PerksTab :public Group{
	private:
		static const int MARGIN = 4;
		static const int GAP = 4;

		static const std::string DOT;

	public:
		float height;
		float width;

		WndClass* wnd;
		PerksTab(WndClass* wnd);
	};

	class MasteryTab :public Group{
	private:
		static const int MARGIN = 4;

	public:
		float height;
		float width;

		WndClass* wnd;
		MasteryTab(WndClass* wnd);
	};

private:
	PerksTab* tabPerks;
	MasteryTab* tabMastery;

public:
	HeroClass cl;
	WndClass(HeroClass cl);
};