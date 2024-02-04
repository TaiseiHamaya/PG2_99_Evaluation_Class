#pragma once

#include <unordered_map>
#include <string>

struct GameTexture {
	int handle;
	const std::string fileDir;
};

class GameTextureManager final {
public:
	static void Initialize();

	static void Register(const std::string& name, const std::string& fileDir);

	static void Unload(const std::string& name);

	static int GetHandle(const std::string& name);

	static void Reload();

private:
	static std::unordered_map<std::string, GameTexture> textureHandle;
};