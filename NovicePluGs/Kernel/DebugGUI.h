#pragma once

#ifdef _DEBUG
#include "imgui.h"
#endif

#include <memory>
#include <chrono>

class DebugGui final {
private:
	DebugGui();

public:
	static void Initialize();

	static void Run();
	static bool IsDebug();
	static bool IsPause();
	static bool IsKeyUpdate();

private:
	static std::unique_ptr<DebugGui> instance;

public:
	void _Run();
	bool _IsPause();
	bool _IsKeyUpdate();

private:
	bool pauseGame;
	bool nextFrame;
	bool preNextFrame;
	std::chrono::system_clock::time_point start, end;
	float masterVolume;
	bool isDebug;
};