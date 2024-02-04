#include "GameData.h"

std::unique_ptr<GameData> GameData::instance;

GameData::GameData() {
	player = std::make_unique<Player>();
	enemyManager = std::make_unique<EnemyManager>();
}

void GameData::Initialize() {
	instance.reset(new GameData());
}

void GameData::Finalize() {
	instance.reset();
}

const Player& GameData::GetPlayer() {
	return *instance->player;
}

const EnemyManager& GameData::GetEnemyManager() {
	return *instance->enemyManager;
}

void GameData::KillCountPlus() {
	++instance->killCount;
}
