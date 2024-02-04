#pragma once

#include <Transform2D.h>
#include <Collider2D.h>
#include <Color.h>

class Bullet {
public:
	Bullet(const Vector2& position, float radius, const Vector2& velocity_);
	~Bullet() = default;

public:
	void Begin();
	void Update();
	void BeginRendering();
	void Draw() const;
	void Debug();

public:
	bool IsErase();
	const std::unique_ptr<Capcel>& GetCollider() const;

private:
	int timer;
	std::shared_ptr<Transform2D> transform;
	std::shared_ptr<Color> color;
	std::unique_ptr<Capcel> colliderCapcel;
	Vector2 velocity;
};

