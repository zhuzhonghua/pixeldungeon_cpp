#include "stdafx.h"

#include "audio_sample.h"

namespace Zengine{
AudioSample* AudioSample::_inst;

AudioSample::AudioSample()
{

}

AudioSample* AudioSample::inst()
{
	if (_inst == NULL)
	{
		_inst = new AudioSample();
	}

	return _inst;
}

void AudioSample::play(const std::string& file)
{
	Mix_Chunk* sound;
	std::map<std::string, Mix_Chunk*>::iterator itr = _sounds.find(file);
	if (itr != _sounds.end())
	{
		sound = itr->second;
	}
	else
	{
		sound = Mix_LoadWAV(file.c_str());
		_sounds.insert(std::make_pair(file, sound));
	}

	Mix_PlayChannel(-1, sound, 0);
	//Mix_VolumeChunk(sound, 100);
}

void AudioSample::enable(bool value)
{
	if (value)
	{
		Mix_Resume(-1);
	}
	else
	{
		Mix_Pause(-1);
	}
}
};