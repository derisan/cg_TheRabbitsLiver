#pragma once

#include <fmod/fmod.hpp>

#include <unordered_map>
#include <string>

class SoundEngine
{
public:
	~SoundEngine();

	static SoundEngine* Get();

	void Create(const std::string& file, const std::string& sound, bool loop = false);
	void Play(const std::string& sound, float volume = 1.0f);
	void Stop(const std::string& sound);

private:
	SoundEngine();

	std::unordered_map<std::string, FMOD::Sound*> mSounds;
	std::unordered_map <FMOD::Sound*, FMOD::Channel*> mChannels;

	FMOD::System* mSystem;

	static SoundEngine* mInstance;
};

