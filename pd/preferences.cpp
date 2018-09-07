#include "preferences.h"
#include "stdafx.h"
#include "util.h"
#include <boost/property_tree/xml_parser.hpp>

const std::string Preferences::KEY_LANDSCAPE = "landscape";
const std::string Preferences::KEY_IMMERSIVE = "immersive";
const std::string Preferences::KEY_GOOGLE_PLAY = "google_play";
const std::string Preferences::KEY_SCALE_UP = "scaleup";
const std::string Preferences::KEY_MUSIC = "music";
const std::string Preferences::KEY_SOUND_FX = "soundfx";
const std::string Preferences::KEY_ZOOM = "zoom";
const std::string Preferences::KEY_LAST_CLASS = "last_class";
const std::string Preferences::KEY_CHALLENGES = "challenges";
const std::string Preferences::KEY_DONATED = "donated";
const std::string Preferences::KEY_INTRO = "intro";
const std::string Preferences::KEY_BRIGHTNESS = "brightness";

Preferences* Preferences::_inst;

Preferences::Preferences()
{
	std::stringstream ss;
	IOManager::readFileToBuffer("data/pref.xml", ss, true);
	read_xml(ss, pt);
}

void Preferences::flush()
{
	std::stringstream ss;
	write_xml(ss, pt);
	IOManager::writeFile("data/pref.xml", ss.str());
}

Preferences* Preferences::get()
{
	if (_inst == NULL) _inst = new Preferences();

	return _inst;
}

int Preferences::getInt(const std::string& key, int defValue)
{
	return pt.get<int>(key, defValue);
}

bool Preferences::getBool(const std::string& key, bool defValue)
{
	return pt.get<bool>(key, defValue);
}

std::string Preferences::getString(const std::string& key, const std::string& defValue)
{
	return pt.get(key, defValue);
}

void Preferences::put(const std::string& key, int value)
{
	pt.put(key, value);
	flush();
}

void Preferences::put(const std::string& key, bool value)
{
	pt.put(key, value);
	flush();
}

void Preferences::put(const std::string& key, const std::string& value)
{
	pt.put(key, value);
	flush();
}