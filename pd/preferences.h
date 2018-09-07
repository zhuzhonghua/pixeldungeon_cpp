#pragma once

#include <string>

#include <boost/property_tree/ptree.hpp>

class Preferences{
private:
	static Preferences* _inst;
	Preferences();

	boost::property_tree::ptree pt;

	void flush();
public:
	static const std::string KEY_LANDSCAPE;
	static const std::string KEY_IMMERSIVE;
	static const std::string KEY_GOOGLE_PLAY;
	static const std::string KEY_SCALE_UP;
	static const std::string KEY_MUSIC;
	static const std::string KEY_SOUND_FX;
	static const std::string KEY_ZOOM;
	static const std::string KEY_LAST_CLASS;
	static const std::string KEY_CHALLENGES;
	static const std::string KEY_DONATED;
	static const std::string KEY_INTRO;
	static const std::string KEY_BRIGHTNESS;

	static Preferences* get();
	int getInt(const std::string& key, int defValue);
	bool getBool(const std::string& key, bool defValue);
	std::string getString(const std::string& key, const std::string& defValue);
	void put(const std::string& key, int value);
	void put(const std::string& key, bool value);
	void put(const std::string& key, const std::string& value);
};