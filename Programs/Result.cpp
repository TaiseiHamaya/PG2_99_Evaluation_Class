#include "Result.h"

#include <Novice.h>
#include <SceneManager.h>
#include <GameInput.h>
#include <GameTextureManager.h>
#include <format>

#include "Title.h"
#include "Game.h"
#include "Moment.h"

void DrawNumber(int x, int y, int num_w, int num_h, int handle, const std::string& draw_num) {
	int left = x - int(draw_num.size()) * num_w / 2;
	for (int i = 0; i < draw_num.size(); ++i) {
		Novice::DrawQuad(
			left + i * num_w, y - num_h / 2,
			left + (i + 1) * num_w, y - num_h / 2,
			left + i * num_w, y + num_h / 2,
			left + (i + 1) * num_w, y + num_h / 2,
			(draw_num[i] - '0') * num_w, 0, num_w, num_h, handle, WHITE
		);
	}
}

Result::Result(int result) {
	killCount = result;
}

void Result::Initialize() {
	GameTextureManager::Register("Result", "./Resources/Result.png");
	GameTextureManager::Register("Number", "./Resources/Number.png");
}

void Result::Begin() {
}

void Result::Update() {
	if (GameInput::IsTriggerKey(DIK_SPACE)) {
		SceneManager::SetSceneChange(new Title{}, new Moment, false);
	}
}

void Result::BeginRendering() {
}

void Result::Draw() const {
	Novice::DrawBox(0, 0, 800, 800, 0.0f, BLACK, kFillModeSolid);
	Novice::DrawSprite(400 - 384 / 2, 150, GameTextureManager::GetHandle("Result"), 1, 1, 0, WHITE);
	DrawNumber(400, 600, 64, 96, GameTextureManager::GetHandle("Number"), std::format("{:d}", killCount));
}

void Result::Debug() {
	Novice::ScreenPrintf(0, 60, "Result");
}

void Result::DefaultTransitionUpdate() {
}
