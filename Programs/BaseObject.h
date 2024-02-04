#pragma once

#include <Transform2D.h>
#include <Collider2D.h>
#include <Color.h>

class BaseObject {
public:
	BaseObject(const Vector2& position, float radius, float MAX_SPEED_, float accelMag);
	virtual ~BaseObject() = default;

public:
	virtual void Begin();
	virtual void Update();
	virtual void Collision() = 0;
	virtual void BeginRendering() = 0;
	virtual void Draw() const = 0;
	virtual void Debug();

protected:
	int timer;
	std::shared_ptr<Transform2D> transform;
	std::shared_ptr<Color> color;
	std::unique_ptr<Circle> colliderCircle;
	Vector2 accel;
	Vector2 velocity;
	const float MAX_SPEED;
	float accelMag;
	const float defaultAccelMag;
	Vector2 top;
	Vector2 left;
	Vector2 right;
	int hp;
};

