#include "EnemyManager.h"

#include <RandomSystem.h>

#include "GameData.h"

EnemyManager::EnemyManager() {
	timer = 0;
	enemy.clear();
}

void EnemyManager::Begin() {
	timer++;
	for (auto enemy_itr = enemy.begin(); enemy_itr != enemy.end(); ++enemy_itr) {
		enemy_itr->Begin();
	}
}

void EnemyManager::Update() {
	for (auto enemy_itr = enemy.begin(); enemy_itr != enemy.end();) {
		enemy_itr->Update();
		if (enemy_itr->IsErase()) {
			enemy_itr = enemy.erase(enemy_itr);
			GameData::KillCountPlus();
		}
		else {
			++enemy_itr;
		}
	}
	if (timer % 180 == 0 && enemy.size() < 3) {
		enemy.emplace_back(Vector2{ RandomSystem::Random_11_Float() * 400,RandomSystem::Random_11_Float() * 400 });
	}
}

void EnemyManager::Collision() {
	for (auto enemy_itr = enemy.begin(); enemy_itr != enemy.end(); ++enemy_itr) {
		enemy_itr->Collision();
	}
}

void EnemyManager::BeginRendering() {
	for (auto enemy_itr = enemy.begin(); enemy_itr != enemy.end(); ++enemy_itr) {
		enemy_itr->BeginRendering();
	}
}

void EnemyManager::Draw() const {
	for (auto enemy_itr = enemy.begin(); enemy_itr != enemy.end(); ++enemy_itr) {
		enemy_itr->Draw();
	}
}

void EnemyManager::Debug() {
	for (auto enemy_itr = enemy.begin(); enemy_itr != enemy.end(); ++enemy_itr) {
		enemy_itr->Debug();
	}
}

const std::list<Enemy>& EnemyManager::GetEnemy() const {
	return enemy;
}
