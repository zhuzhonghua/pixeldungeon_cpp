#pragma once

#include "component.h"
#include "signal.h"
#include "regex/pcrecpp.h"
#include "bitmaptextmultiline.h"

#include <string>
#include <vector>

class GameLog :public Component, public Signal<std::string>::Listener{
private:
	struct Entry {
		std::string text;
		int color;
		Entry(const std::string& text, int color) {
			this->text = text;
			this->color = color;
		}
	};

private:
	static const int MAX_LINES = 3;

	static const pcrecpp::RE PUNCTUATION;

	BitmapTextMultiline* lastEntry;
	int lastColor;

	static std::vector<Entry> entries;

	void recreateLines();
public:
	static void wipe() { entries.clear(); }

	GameLog();
	virtual void destroy();
	void newLine();
	virtual void onSignal(std::string* text);

protected:
	virtual void layout();
};