#pragma once

#include <memory>

#include "EnemyManager.h"
#include "Player.h"
#include "Bullet.h"

class GameData final {
	friend class Game;
private:
	GameData();

public:
	GameData(const GameData&) = delete;
	GameData& operator=(const GameData&) = delete;

public:
	static void Initialize();
	static void Finalize();
	static std::unique_ptr<GameData> instance;

public:
	static const Player& GetPlayer();
	static const EnemyManager& GetEnemyManager();
	static void KillCountPlus();

private:
	int killCount;
	std::unique_ptr<Player> player;
	std::unique_ptr<EnemyManager> enemyManager;
};