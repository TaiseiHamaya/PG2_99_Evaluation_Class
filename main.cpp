#include <Novice.h>
#include <NovicePluGs.h>
#include <GameInput.h>
#include <MapchipModule.h>
#include <Renderer.h>
#include <SceneManager.h>
#include <GameCamera.h>
#include <ThreadLoad.h>
#include <DeviceData.h>

#include "Title.h"

const char kWindowTitle[] = "LC1A_19_ハマヤ_タイセイ_Hit_and_Away";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// -------------------------------------------ライブラリの初期化------------------------------------------
	NovicePluGs::Initialize(kWindowTitle, 800, 800);
	GameCamera::SetCameraPos(zeroVec2);
	GameCamera::SetNDCPosition(-DeviceData::GetWindowSize().x / 2, DeviceData::GetWindowSize().y / 2, DeviceData::GetWindowSize().x / 2, -DeviceData::GetWindowSize().y / 2);
	GameCamera::SetViewportInformation(zeroVec2, DeviceData::GetWindowSize());
	GameCamera::CameraUpdate();

	SceneManager::Initialize(new Title);

	NovicePluGs::Run();

	// ライブラリの終了
	GameInput::Finalize();
	SceneManager::Finalize();
	Novice::Finalize();

	return 0;
}
