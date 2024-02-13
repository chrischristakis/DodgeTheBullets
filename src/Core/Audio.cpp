#include "Audio.h"

#include <iostream>
#include <fmod/fmod_errors.h>

// ----- AUDIO ----- //

#define FMOD_ASSERT(result, message) \
    if (result != FMOD_OK) { \
        std::cerr << message << ": " << FMOD_ErrorString(result) << std::endl; \
        std::terminate(); \
    }

void Audio::Init(int MaxChannels) {
	m_maxChannels = MaxChannels;

	FMOD_RESULT result = FMOD::System_Create(&m_system);
	FMOD_ASSERT(result, "Failed to create FMOD system");

	result = m_system->init(MaxChannels, FMOD_INIT_NORMAL, nullptr);
	FMOD_ASSERT(result, "Failed to initialize FMOD system");

	// Populate queue with available IDS
	for (int i = 0; i < MaxChannels; i++)
		m_idQueue.push(i);
}

void Audio::Update() {
	m_system->update();

	// Remove finished channels from the map
	for ( ChannelMap::iterator it = m_channelMap.begin(); it != m_channelMap.end(); ) {
		bool playing;

		PlaybackID playbackId = it->first;
		FMOD::Channel* channel = it->second;

		channel->isPlaying(&playing);
		if (!playing) {
			m_idQueue.push(playbackId);
			it = m_channelMap.erase(it);
		}
		else
			it++;
	}
}

void Audio::CreateSound(const std::string& id, const std::string& path) {

	FMOD::Sound* fmodSound;

	FMOD_RESULT result = m_system->createSound(path.c_str(), FMOD_DEFAULT, 0, &fmodSound);
	FMOD_ASSERT(result, "Failed to load FMOD sound");

	m_soundMap.insert({ id, fmodSound });
}

Audio::PlaybackID Audio::PlaySound(const std::string& id, bool loop) {

	SoundMap::iterator found = m_soundMap.find(id);
	if (found == m_soundMap.end()) {
		std::cerr << "Sound with id '" << id << "' doesn't exist" << std::endl;
		return -1;
	}

	if (m_idQueue.size() == 0) {
		std::cerr << "Not enough channels to play new sound" << std::endl;
		return -1;
	}

	PlaybackID playbackId = m_idQueue.front();
	m_idQueue.pop();

	FMOD::Sound* fmodSound = found->second;
	FMOD::Channel* channel;

	FMOD_RESULT result = m_system->playSound(fmodSound, 0, false, &channel);
	FMOD_ASSERT(result, "Failed to play FMOD sound");

	if (loop)
		channel->setMode(FMOD_LOOP_NORMAL);

	m_channelMap.insert({ playbackId, channel });
	return playbackId;
}
