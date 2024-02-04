#pragma once

#include <BaseObject.h>
#include <list>

#include "Bullet.h"

class Enemy : public BaseObject {
public:
	Enemy(const Vector2 position);
	~Enemy() = default;

public:
	void Begin() override;
	void Update() override;
	void Collision() override;
	void BeginRendering() override;
	void Draw() const override;

public:
	bool IsErase();
	const std::list<Bullet>& GetBullet() const;
	std::list<Bullet>::iterator EraseBullet(std::list<Bullet>::const_iterator& bullet_itr);

private:
	float rotate;
	Vector2 direction;
	std::list<Bullet> bullet;
};
