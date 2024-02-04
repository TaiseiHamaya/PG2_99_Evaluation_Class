#pragma once

#include "BaseObject.h"

#include <RenderingObjects2D.h>

#include "Bullet.h"

class Player : public BaseObject {
public:
	Player();
	~Player() = default;

public:
	void Begin() override;
	void Input();
	void Update() override;
	void Collision() override;
	void BeginRendering() override;
	void Draw() const override;

public:
	void BulletUpdate();
	const Vector2& GetPos() const;
	const Vector2& GetVelocity() const;
	const std::list<Bullet>& GetBullet() const;
	std::list<Bullet>::iterator EraseBullet(std::list<Bullet>::const_iterator itr);
	bool IsDied() const;

private:
	float rotate;
	Vector2 direction;
	std::list<Bullet> bullet;
};

