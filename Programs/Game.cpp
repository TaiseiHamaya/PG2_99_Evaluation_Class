#include "Game.h"

#include <Novice.h>
#include <GameInput.h>
#include <SceneManager.h>
#include <GameTextureManager.h>
#include <GameSoundManager.h>
#include <GameCamera.h>
#include <SceneManager.h>

#include "GameData.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Result.h"
#include "Moment.h"

#define instance GameData::instance->

Game::Game() {
	isEnd = false;
	endTimer = 0;
}

Game::~Game() {
	GameData::Finalize();
	GameCamera::SetCameraPos(zeroVec2);
	GameCamera::CameraUpdate();
}

void Game::Load() {
}

void Game::Initialize() {
	GameData::Initialize();
}

void Game::Begin() {
	if (isEnd) {
		++endTimer;
	}
	instance player->Begin();
	instance enemyManager->Begin();
}

void Game::Update() {
	if (isEnd) {
		instance enemyManager->Update();
		instance player->BulletUpdate();
		if (endTimer > 60) {
			SceneManager::SetSceneChange(new Result{ instance killCount }, new Moment, false);
		}
		return;
	}
	instance player->Input();
	instance player->Update();
	instance enemyManager->Update();
	instance player->Collision();
	instance enemyManager->Collision();
	GameCamera::SetCameraPos(instance player->GetPos() - instance player->GetVelocity());
	GameCamera::CameraUpdate();
	if (instance player->IsDied()) {
		isEnd = true;
	}
}

void Game::BeginRendering() {
	instance player->BeginRendering();
	instance enemyManager->BeginRendering();
}

void Game::Draw() const {
	Novice::DrawBox(0, 0, 800, 800, 0.0f, BLACK, kFillModeSolid);
	instance enemyManager->Draw();
	instance player->Draw();
}

void Game::Debug() {
	Novice::ScreenPrintf(0, 60, "Game");
	instance player->Debug();
	instance enemyManager->Debug();
}

void Game::DefaultTransitionUpdate() {
}
