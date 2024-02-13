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

	static void Init(int MaxChannels);
	static void Update();

	static void CreateSound(const std::string& id, const std::string& path);
	static PlaybackID PlaySound(const std::string& id, bool loop = false);

private:

	// FMOD's system that we use as an interface to all of its other components
	inline static FMOD::System* m_system;

	using SoundMap = std::map<std::string, FMOD::Sound*>;
	using ChannelMap = std::map<PlaybackID, FMOD::Channel*>;

	inline static std::queue<PlaybackID> m_idQueue;
	inline static SoundMap m_soundMap;
	inline static ChannelMap m_channelMap;

	inline static int m_maxChannels;
};