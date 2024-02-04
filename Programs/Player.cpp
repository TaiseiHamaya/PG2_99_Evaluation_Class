#include "Player.h"

#include <Novice.h>
#include <GameCamera.h>
#include <GameInput.h>
#include <GameTextureManager.h>

#include "GameData.h"
#include "EnemyManager.h"
#include "Enemy.h"

float sign(float A) {
	return float((A > 0) - (A < 0));
}

Player::Player() :
	BaseObject(zeroVec2, 16, 8, 0.5f) {
	bullet.clear();
	direction = basisVec2Y;
	accelMag = 0.8f;
	hp = 100;
}

void Player::Begin() {
	timer++;
	BaseObject::Begin();
	for (auto bullet_itr = bullet.begin(); bullet_itr != bullet.end(); ++bullet_itr) {
		bullet_itr->Begin();
	}
	*color = 0xFFFFFFFF;
}

void Player::Input() {
	accel = GameInput::InputWASD();
	rotate = GameInput::InputArrow().x;
}

void Player::Update() {
	direction = velocity != zeroVec2 ? velocity.normalize() : direction;
	BaseObject::Update();
	BulletUpdate();
}

void Player::Collision() {
	for (auto enemy_itr = GameData::GetEnemyManager().GetEnemy().begin(); enemy_itr != GameData::GetEnemyManager().GetEnemy().end(); ++enemy_itr) {
		for (auto enemy_bullet_itr = enemy_itr->GetBullet().begin(); enemy_bullet_itr != enemy_itr->GetBullet().end();) {
			if (Collider::CircleToCapcel(*colliderCircle, *enemy_bullet_itr->GetCollider())) {
				--hp;
				*color = 0xFF0000FF;
				enemy_bullet_itr = const_cast<Enemy*>(&*enemy_itr)->EraseBullet(enemy_bullet_itr);
			}
			else {
				++enemy_bullet_itr;
			}
		}
	}
}

void Player::BeginRendering() {
	top = Transform2D::Homogeneous(direction * 32, transform->get_matrix() * GameCamera::GetVPVMatrix());
	left = Transform2D::Homogeneous(Vector2::Rotate(direction * 16, -1, 0), transform->get_matrix() * GameCamera::GetVPVMatrix());
	right = Transform2D::Homogeneous(Vector2::Rotate(direction * 16, 1, 0), transform->get_matrix() * GameCamera::GetVPVMatrix());
}

void Player::Draw() const {
	Vector2 screenPos = Transform2D::Homogeneous(zeroVec2, transform->get_matrix() * GameCamera::GetVPVMatrix());

	if (!this->IsDied()) {
		Novice::DrawEllipse(int(screenPos.x), int(screenPos.y), int(colliderCircle->radius), int(colliderCircle->radius), 0, color->hex(), kFillModeSolid);

		Novice::DrawTriangle(
			int(top.x), int(top.y),
			int(left.x), int(left.y),
			int(right.x), int(right.y),
			color->hex(), kFillModeSolid
		);
	}
	for (auto bullet_itr = bullet.begin(); bullet_itr != bullet.end(); ++bullet_itr) {
		bullet_itr->Draw();
	}
	Novice::DrawQuad(
		250, 720, 550, 720,
		250, 770, 550, 770,
		1, 1, 1, 1, GameTextureManager::GetHandle("white1x1"), 0x808080FF
	);
	Novice::DrawQuad(
		250, 720, 250 + hp * 3, 720,
		250, 770, 250 + hp * 3, 770,
		1, 1, 1, 1, GameTextureManager::GetHandle("white1x1"), 0x10cF10FF
	);
}

void Player::BulletUpdate() {
	for (auto bullet_itr = bullet.begin(); bullet_itr != bullet.end();) {
		bullet_itr->Update();
		if (bullet_itr->IsErase()) {
			bullet_itr = bullet.erase(bullet_itr);
		}
		else {
			++bullet_itr;
		}
	}
	if (timer % 6 == 0) {
		bullet.emplace_back(Transform2D::Homogeneous(direction * 32, transform->get_matrix()), 4.0f, direction * 10);
	}
}

const Vector2& Player::GetPos() const {
	return transform->get_translate();
}

const Vector2& Player::GetVelocity() const {
	return velocity;
}

const std::list<Bullet>& Player::GetBullet() const {
	return bullet;
}

std::list<Bullet>::iterator Player::EraseBullet(std::list<Bullet>::const_iterator itr) {
	return bullet.erase(itr);
}

bool Player::IsDied() const {
	return hp <= 0;
}

