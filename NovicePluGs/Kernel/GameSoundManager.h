#pragma once

#include <string>
#include <unordered_map>
#include <functional>

#undef PlaySound

struct SoundData {
	int soundHandle;
	int playHandle;
	const std::string fileDir;
	float volume;
	bool isPlayAlready;
};

class GameSoundManager final {
public:
	static void Initialize();

	static void Register(const std::string& name, const std::string& fileDir);

	static void Begin();

	static void PlaySound(const std::string& name, bool isLoop, float volume);

	static void StopSound(const std::string& name);

	static void SetVolume(const std::string& name, float volume);

	//static void Reload();

	static void SetMasterVolume(float _masterVolume);

private:
	static std::unordered_map<std::string, SoundData> soundData;
	static float masterVolume;
	static std::function<void(void)> load_func;
};