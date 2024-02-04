#include "GameTextureManager.h"

#include <Novice.h>
#include <mutex>
#include <assert.h>

std::mutex texture_mutex;

std::unordered_map<std::string, GameTexture> GameTextureManager::textureHandle;

void GameTextureManager::Initialize() {
	std::lock_guard<std::mutex> lock(texture_mutex);
	textureHandle.clear();
	textureHandle.emplace("white1x1", GameTexture{ Novice::LoadTexture("white1x1.png"), "white1x1.png" });
}

void GameTextureManager::Register(const std::string& name, const std::string& fileDir) {
	if (textureHandle.find(name) == textureHandle.end()) {
		static std::mutex texture_load_mutex;
		texture_load_mutex.lock();
		int handle = Novice::LoadTexture(fileDir.c_str());
		texture_load_mutex.unlock();
		std::lock_guard<std::mutex> lock(texture_mutex);
		textureHandle.emplace(name, GameTexture{ handle,fileDir });
	}
}

void GameTextureManager::Unload(const std::string& name) {
	if (textureHandle.find(name) == textureHandle.end()) {
		Novice::UnloadTexture(textureHandle[name].handle);
		textureHandle.erase(name);
	}
}

int GameTextureManager::GetHandle(const std::string& name) {
	std::lock_guard<std::mutex> lock(texture_mutex);
	return textureHandle.find(name) != textureHandle.end() ? textureHandle.at(name).handle : textureHandle["white1x1"].handle;
}

void GameTextureManager::Reload() {
	std::lock_guard<std::mutex> lock(texture_mutex);
	auto itr = textureHandle.begin();
	while (itr != textureHandle.end()) {
		Novice::UnloadTexture(itr->second.handle);
		itr->second.handle = Novice::LoadTexture(itr->second.fileDir.c_str());
		itr++;
	}
}