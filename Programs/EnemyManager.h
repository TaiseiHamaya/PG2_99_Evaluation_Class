#pragma once

#include <Enemy.h>
#include <list>
#include <Particle.h>

class EnemyManager {
public:
	EnemyManager();
	~EnemyManager() = default;

public:
	void Begin();
	void Update();
	void Collision();
	void BeginRendering();
	void Draw() const;
	void Debug();

public:
	const std::list<Enemy>& GetEnemy() const;

private:
	int timer;
	std::list<Enemy> enemy;
};

