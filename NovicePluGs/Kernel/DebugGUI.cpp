#include "DebugGUI.h"

#ifdef _DEBUG
#include "imgui.h"
#endif

#include "GameTextureManager.h"
#include "GameSoundManager.h"
#include "Easing.h"
#include "GameInput.h"
#include "GameCamera.h"

std::unique_ptr<DebugGui> DebugGui::instance;

DebugGui::DebugGui() {
	isDebug = true;
	pauseGame = false;
	nextFrame = false;
	masterVolume = 1.0f;
	preNextFrame = false;
}

void DebugGui::Initialize() {
	#ifdef _DEBUG
	instance.reset(new DebugGui);
	#endif
}

void DebugGui::Run() {
	instance->_Run();
}

bool DebugGui::IsDebug() {
	return instance->isDebug;
}

bool DebugGui::IsPause() {
	return instance->_IsPause();
}

bool DebugGui::IsKeyUpdate() {
	return instance->_IsKeyUpdate();
}

void DebugGui::_Run() {
	#ifdef _DEBUG
	ImGui::SetNextWindowPos(ImVec2{ 100, 200 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 200, 200 }, ImGuiCond_Once);
	ImGui::Begin("DebugGUI", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
	ImGui::Checkbox("Pause", &pauseGame);
	ImGui::SameLine();
	preNextFrame = nextFrame;
	nextFrame = ImGui::Button("NextFrame");
	ImGui::Checkbox("Debug", &isDebug);
	if (ImGui::Button("TextureReload")) {
		GameTextureManager::Reload();
	}
	if (!pauseGame || preNextFrame) {
		end = std::chrono::system_clock::now();
	}
	double temp = (double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	ImGui::Text("%lf", temp / 1000.0);
	if (!pauseGame || nextFrame) {
		start = std::chrono::system_clock::now();
	}
	GameSoundManager::SetMasterVolume(Lerp(0.0f, 0.5f, masterVolume));
	ImGui::SliderFloat("MasterVolume", &masterVolume, 0, 1, "%.6f");
	Vector2 mousePos = Transform2D::Homogeneous(GameInput::GetMousePos(), GameCamera::GetVPVMatrix().inverse());
	ImGui::Text("%f %f", mousePos.x, mousePos.y);
	ImGui::End();
	#endif
}

bool DebugGui::_IsPause() {
	#ifdef _DEBUG
	return pauseGame && !nextFrame;
	#endif
}

bool DebugGui::_IsKeyUpdate() {
	return !pauseGame || (pauseGame && nextFrame);
}
