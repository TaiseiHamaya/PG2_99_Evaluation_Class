#include "BaseObject.h"

BaseObject::BaseObject(const Vector2& position, float radius, float MAX_SPEED_, float accelMag) :
	MAX_SPEED(MAX_SPEED_),
	defaultAccelMag(accelMag) {
	timer = 0;
	transform = std::make_shared<Transform2D>(basisVec2, 0.0f, position);
	color = std::make_shared<Color>(0xFFFFFFFF);
	colliderCircle = std::make_unique<Circle>(kColliderTypeArea, transform, radius);
}

void BaseObject::Begin() {
	transform->begin();
	colliderCircle->begin();
}

void BaseObject::Update() {
	velocity += accel * accelMag;
	if (velocity.length() >= MAX_SPEED) {
		velocity = velocity.normalize() * MAX_SPEED;
	}
	velocity *= 0.99f;
	transform->plus_translate(velocity);
	transform->update();
}

void BaseObject::Debug() {
	colliderCircle->debug();
}
