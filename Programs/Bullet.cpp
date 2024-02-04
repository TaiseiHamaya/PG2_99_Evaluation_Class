#include "Bullet.h"

#include <Novice.h>
#include <GameCamera.h>

Bullet::Bullet(const Vector2& position, float radius, const Vector2& velocity_) {
	timer = 0;
	transform = std::make_shared<Transform2D>(basisVec2, 0.0f, position);
	color = std::make_shared<Color>(0xFFFFFFFF);
	colliderCapcel = std::make_unique<Capcel>(kColliderTypeArea, transform, radius);
	velocity = velocity_;
}

void Bullet::Begin() {
	timer++;
	transform->begin();
	colliderCapcel->begin();
}

void Bullet::Update() {
	transform->plus_translate(velocity);
	transform->update();
}

void Bullet::BeginRendering() {
}

void Bullet::Draw() const {
	Vector2 screenPos = Transform2D::Homogeneous(zeroVec2, transform->get_matrix() * GameCamera::GetVPVMatrix());
	Novice::DrawEllipse(int(screenPos.x), int(screenPos.y), int(colliderCapcel->radius), int(colliderCapcel->radius), 0, color->hex(), kFillModeSolid);
}

void Bullet::Debug() {
	colliderCapcel->debug();
}

bool Bullet::IsErase() {
	return timer >= 600;
}

const std::unique_ptr<Capcel>& Bullet::GetCollider() const {
	return colliderCapcel;
}
