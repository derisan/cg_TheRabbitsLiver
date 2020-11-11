#include "sound_engine.h"

#include <iostream>

SoundEngine::SoundEngine()
{
	FMOD_RESULT res;

	res = FMOD::System_Create(&mSystem);
	if (res != FMOD_OK) {
		std::cout << "FMOD::System_Create() fail" << std::endl;
		exit(-1);
	}

	unsigned int version;
	res = mSystem->getVersion(&version);
	if (res != FMOD_OK) {
		std::cout << "getVersion() fail" << std::endl;
		exit(-1);
	}
	else printf("FMOD version %08x\n", version);

	void* extraDriverData{ nullptr };
	res = mSystem->init(32, FMOD_INIT_NORMAL, extraDriverData);
	if (res != FMOD_OK) {
		std::cout << "system->init() fail" << std::endl;
		exit(-1);
	}
}

SoundEngine::~SoundEngine()
{
	mSystem->release();
}

SoundEngine* SoundEngine::Get()
{
	if (mInstance == nullptr)
		mInstance = new SoundEngine{};

	return mInstance;
}

void SoundEngine::Create(const std::string& file, const std::string& sound, bool loop)
{
	auto iter = mSounds.find(sound);
	if (iter != std::end(mSounds))
		return;

	mSounds[sound] = nullptr;
	auto& soundPtr = mSounds[sound];

	auto flag = loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	FMOD_RESULT res = mSystem->createSound(file.c_str(), flag, 0, &soundPtr);

	if (res != FMOD_OK)
	{
		std::cout << "Failed to create sound" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void SoundEngine::Play(const std::string& sound, float volume)
{
	auto iter = mSounds.find(sound);
	if (iter == std::end(mSounds))
	{
		std::cout << "Couldn't find any sound files " << sound << std::endl;
		return;
	}

	const auto& soundPtr = mSounds[sound];
	auto& channelPtr = mChannels[soundPtr];

	bool isPlaying{ false };
	FMOD_RESULT res = channelPtr->isPlaying(&isPlaying);

	if (isPlaying)
		return;

	res = mSystem->playSound(soundPtr, 0, false, &channelPtr);
	channelPtr->setVolume(volume);

	if (res != FMOD_OK)
	{
		std::cout << "Failed to play sound " << sound << std::endl;
		exit(EXIT_FAILURE);
	}
}

void SoundEngine::Stop(const std::string& sound)
{
	auto iter = mSounds.find(sound);
	if (iter == std::end(mSounds))
	{
		std::cout << "Couldn't find any sound files " << sound << std::endl;
		return;
	}

	const auto& soundPtr = mSounds[sound];
	auto& channelPtr = mChannels[soundPtr];

	bool is_playing = false;
	FMOD_RESULT res = channelPtr->isPlaying(&is_playing);

	if (is_playing == false) 
		return; // don't stop playing if this is not playing

	res = channelPtr->stop();

	if (res != FMOD_OK) {
		std::cout << "system->playSound() fail" << std::endl;
		exit(EXIT_FAILURE);
	}
}

SoundEngine* SoundEngine::mInstance;