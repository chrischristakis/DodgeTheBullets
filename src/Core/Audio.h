#pragma once

#include <string>
#include <map>
#include <queue>
#include <fmod/fmod.hpp>

class Sound;
class Playback;

class Audio {
public:

	using PlaybackID = unsigned int;

	Audio(int MaxChannels);
	~Audio();

	void Update();

	void CreateSound(const std::string& id, const std::string& path);
	PlaybackID Play(const std::string& id, bool loop = false);
	void Stop(PlaybackID);

private:

	// FMOD's system that we use as an interface to all of its other components
	FMOD::System* m_system;

	using SoundMap = std::map<std::string, FMOD::Sound*>;
	using ChannelMap = std::map<PlaybackID, FMOD::Channel*>;

	std::queue<PlaybackID> m_idQueue;
	SoundMap m_soundMap;
	ChannelMap m_channelMap;

	int m_maxChannels;
};