#include "GameSoundManager.h"

#include <Novice.h>
#include <cassert>

#include <mutex>

#undef PlaySound

std::unordered_map<std::string, SoundData> GameSoundManager::soundData;
float GameSoundManager::masterVolume;
std::mutex sound_mutex;

void GameSoundManager::Initialize() {
	std::lock_guard<std::mutex> lock(sound_mutex);
	soundData.clear();
	masterVolume = 1.0f;
}

void GameSoundManager::Register(const std::string& name, const std::string& fileDir) {
	if (soundData.find(name) == soundData.end()) {
		static std::mutex sound_load_mutex;
		sound_load_mutex.lock();
		int handle = Novice::LoadAudio(fileDir.c_str());// ロードだけマルチスレッド処理
		sound_load_mutex.unlock();

		std::lock_guard<std::mutex> lock(sound_mutex);
		soundData.emplace(name, SoundData{ handle, -1,fileDir,false });
	}
}

void GameSoundManager::Begin() {
	std::lock_guard<std::mutex> lock(sound_mutex);
	for (auto itr = soundData.begin(); itr != soundData.end(); ++itr) {
		itr->second.isPlayAlready = false;
	}
}

void GameSoundManager::PlaySound(const std::string& name, bool isLoop, float volume) {
	assert(soundData.find(name) != soundData.end());
	std::lock_guard<std::mutex> lock(sound_mutex);
	if ((soundData.at(name).playHandle != -1 && isLoop) || soundData.at(name).isPlayAlready) {
		return;
	}
	soundData.at(name).isPlayAlready = true;
	soundData.at(name).playHandle = Novice::PlayAudio(soundData.at(name).soundHandle, isLoop, volume * masterVolume);
}

void GameSoundManager::StopSound(const std::string& name) {
	assert(soundData.find(name) != soundData.end());
	std::lock_guard<std::mutex> lock(sound_mutex);
	Novice::StopAudio(soundData.at(name).playHandle);
	soundData.at(name).playHandle = -1;
}

void GameSoundManager::SetVolume(const std::string& name, float volume) {
	assert(soundData.find(name) != soundData.end());
	std::lock_guard<std::mutex> lock(sound_mutex);
	Novice::SetAudioVolume(soundData.at(name).playHandle, volume * masterVolume);
}

//void GameSoundManager::Reload() {
//	auto itr = soundData.begin();
//	while (itr == soundData.end()) {
//		itr++;
//	}
//}

void GameSoundManager::SetMasterVolume(float ___masterVolume) {
	if (masterVolume != ___masterVolume) {
		for (auto itr = soundData.begin(); itr != soundData.end(); ++itr) {
			if (itr->second.isPlayAlready || itr->second.playHandle) {
				
			}
		}
	}
	masterVolume = ___masterVolume;
}