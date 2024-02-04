#include "Title.h"

#include <Novice.h>
#include <SceneManager.h>
#include <GameInput.h>
#include <GameTextureManager.h>
#include <Renderer.h>

#include "Game.h"
#include "Moment.h"

void Title::Initialize() {
	GameTextureManager::Register("Title", "./Resources/Title.png");
	GameTextureManager::Register("Space", "./Resources/Space.png");
}

void Title::Begin() {
}

void Title::Update() {
	if (GameInput::IsTriggerKey(DIK_SPACE)) {
		SceneManager::SetSceneChange(new Game{}, new Moment, false);
	}
}

void Title::BeginRendering() {
}

void Title::Draw() const {
	Novice::DrawBox(0, 0, 800, 800, 0.0f, BLACK, kFillModeSolid);
	Novice::DrawSprite(400 - 284, 150, GameTextureManager::GetHandle("Title"), 1, 1, 0, WHITE);
	Novice::DrawSprite(400 -78, 500, GameTextureManager::GetHandle("Space"), 1, 1, 0, WHITE);
}

void Title::Debug() {
	Novice::ScreenPrintf(0, 60, "Title");
}

void Title::DefaultTransitionUpdate() {
}
