#pragma once

namespace Zengine{
class AudioSample{
protected:
	static AudioSample* _inst;

	AudioSample();

	std::map<std::string, Mix_Chunk*> _sounds;
public:
	static AudioSample* inst();

	void play(const std::string& file);

	void enable(bool value);
};
};