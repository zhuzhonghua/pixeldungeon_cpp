#include "gamelog.h"
#include "glog.h"
#include "pixelscene.h"

const pcrecpp::RE GameLog::PUNCTUATION(".*[.,;?! ]$", pcrecpp::UTF8());

std::vector<GameLog::Entry> GameLog::entries;

GameLog::GameLog()
{
	GLog::update.add(this);
	recreateLines();
}

void GameLog::destroy()
{
	GLog::update.remove(this);
	Component::destroy();
}

void GameLog::recreateLines()
{
	for (int i = 0; i < entries.size();i++) 
	{
		Entry& entry = entries[i];

		lastEntry = PixelScene::createMultiline(entry.text, 6);
		lastEntry->hardlight(lastColor = entry.color);
		add(lastEntry);
	}
}

void GameLog::newLine()
{
	lastEntry = NULL;
}

void GameLog::onSignal(std::string* text)
{
	//int color = CharSprite.DEFAULT;
	//if (text.startsWith(GLog.POSITIVE)) {
	//	text = text.substring(GLog.POSITIVE.length());
	//	color = CharSprite.POSITIVE;
	//}
	//else
	//if (text.startsWith(GLog.NEGATIVE)) {
	//	text = text.substring(GLog.NEGATIVE.length());
	//	color = CharSprite.NEGATIVE;
	//}
	//else
	//if (text.startsWith(GLog.WARNING)) {
	//	text = text.substring(GLog.WARNING.length());
	//	color = CharSprite.WARNING;
	//}
	//else
	//if (text.startsWith(GLog.HIGHLIGHT)) {
	//	text = text.substring(GLog.HIGHLIGHT.length());
	//	color = CharSprite.NEUTRAL;
	//}
	//
	//text = Utils.capitalize(text) +
	//	(PUNCTUATION.matcher(text).matches() ? "" : ".");
	//
	//if (lastEntry != null && color == lastColor && lastEntry.nLines < MAX_LINES) {
	//
	//	String lastMessage = lastEntry.text();
	//	lastEntry.text(lastMessage.length() == 0 ? text : lastMessage + " " + text);
	//	lastEntry.measure();
	//
	//	entries.get(entries.size() - 1).text = lastEntry.text();
	//
	//}
	//else {
	//
	//	lastEntry = PixelScene.createMultiline(text, 6);
	//	lastEntry.hardlight(color);
	//	lastColor = color;
	//	add(lastEntry);
	//
	//	entries.add(new Entry(text, color));
	//
	//}
	//
	//if (length > 0) {
	//	int nLines;
	//	do {
	//		nLines = 0;
	//		for (int i = 0; i < length; i++) {
	//			nLines += ((BitmapTextMultiline)members.get(i)).nLines;
	//		}
	//
	//		if (nLines > MAX_LINES) {
	//			remove(members.get(0));
	//
	//			entries.remove(0);
	//		}
	//	} while (nLines > MAX_LINES);
	//	if (entries.isEmpty()) {
	//		lastEntry = null;
	//	}
	//}

	layout();
}

void GameLog::layout()
{
	float pos = _y;
	for (int i = _members.size() - 1; i >= 0; i--) 
	{
		BitmapTextMultiline* entry = (BitmapTextMultiline*)_members[i];
		entry->maxWidth = (int)width();
		entry->measure();
		entry->x = _x;
		entry->y = pos - entry->getHeight();
		pos -= entry->getHeight();
	}
}