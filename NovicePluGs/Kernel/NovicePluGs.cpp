#include "NovicePluGs.h"

#include "Novice.h"
#include "GameInput.h"
#include "DeviceData.h"
#include "GameCamera.h"
#include "GameTextureManager.h"
#include "GameSoundManager.h"
#include "Definition.h"
#include "RandomSystem.h"
#include "ThreadLoad.h"
#include "SceneManager.h"
#include "Debug.h"
#include "DebugGUI.h"

void NovicePluGs::Initialize(
	const char* kWindowTitle, const float windowSizeX, const float windowSizeY
) {
	Novice::Initialize(kWindowTitle, int(windowSizeX), int(windowSizeY));
	DeviceData::Initialize(int(windowSizeX), int(windowSizeY));
	GameCamera::Initialize();
	GameInput::Initialize();
	RandomSystem::Initialize();
	GameTextureManager::Initialize();
	GameSoundManager::Initialize();
	ThreadLoad::Initialize();
#ifdef _DEBUG
	DebugGui::Initialize();
#endif // _DEBUG
}

void NovicePluGs::Finalize() {
	Novice::Finalize();
	GameInput::Finalize();
	ThreadLoad::Finalize();
}

void NovicePluGs::Run() {
	// ---------------------------------------------ゲームループ---------------------------------------------
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力の記録
#ifdef _DEBUG
		if (DebugGui::IsKeyUpdate()) {
			GameInput::Update();
		}
#else
		GameInput::Update();
#endif // _DEBUG

		///
		/// ----------------------------------------更新処理ここから----------------------------------------
		///

#ifdef _DEBUG
		if (DebugGui::IsPause()) {
			goto skipUpdate;
		}
#endif // _DEBUG

		SceneManager::Update();

#ifdef _DEBUG
		skipUpdate :
#endif

		if (SceneManager::IsEndProgram()) {
			break;
		}

		///
		/// ----------------------------------------更新処理ここまで----------------------------------------
		///

		///
		/// ----------------------------------------描画処理ここから----------------------------------------
		///

#ifdef _DEBUG
		Debug::BlackBakcground();
#endif // _DEBUG
		SceneManager::Draw();

#ifdef _DEBUG
		if (DebugGui::IsDebug()) {
			SceneManager::Debug();
			Debug::Grid();
		}
		DebugGui::Run();
#endif // _DEBUG


		///
		/// ----------------------------------------描画処理ここまで----------------------------------------
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (GameInput::IsTriggerKey(DIK_ESCAPE)) {
			break;
		}
	}
}