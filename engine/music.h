#pragma once

namespace Zengine{
class Music{
protected:
	static Music* _inst;

	Mix_Music* _music;

	Music();
public:
	static Music* inst();

	void play(const std::string & assetName, bool looping);
	void stop();
	void pause();
	void resume();
	void volume(int value);
};
};