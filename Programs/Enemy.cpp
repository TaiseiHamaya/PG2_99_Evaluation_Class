#include "Enemy.h"

#include <Novice.h>
#include <GameCamera.h>
#include <GameInput.h>

#include "GameData.h"
#include "Player.h"

Enemy::Enemy(const Vector2 position) : BaseObject(position, 16, 8, 0.5f) {
	bullet.clear();
	direction = basisVec2Y;
	accelMag = 0.8f;
	hp = 3;
}

void Enemy::Begin() {
	timer++;
	BaseObject::Begin();
	for (auto bullet_itr = bullet.begin(); bullet_itr != bullet.end(); ++bullet_itr) {
		bullet_itr->Begin();
	}
	*color = 0x0000FFFF;
}

void Enemy::Update() {
	direction = (GameData::GetPlayer().GetPos() - transform->get_translate()).normalize();
	BaseObject::Update();
	for (auto bullet_itr = bullet.begin(); bullet_itr != bullet.end();) {
		bullet_itr->Update();
		if (bullet_itr->IsErase()) {
			bullet_itr = bullet.erase(bullet_itr);
		}
		else {
			++bullet_itr;
		}
	}
	if (timer % 20 == 0) {
		bullet.emplace_back(Transform2D::Homogeneous(direction * 32, transform->get_matrix()), 4.0f, direction * 10);
	}
}

void Enemy::Collision() {
	for (auto player_bullet_itr = GameData::GetPlayer().GetBullet().begin(); player_bullet_itr != GameData::GetPlayer().GetBullet().end();) {
		if (Collider::CircleToCapcel(*colliderCircle, *player_bullet_itr->GetCollider())) {
			--hp;
			*color = 0xFF0000FF;
			player_bullet_itr = const_cast<Player*>(&GameData::GetPlayer())->EraseBullet(player_bullet_itr);
		}
		else {
			++player_bullet_itr;
		}
	}
}

void Enemy::BeginRendering() {
	top = Transform2D::Homogeneous(direction * 32, transform->get_matrix() * GameCamera::GetVPVMatrix());
	left = Transform2D::Homogeneous(Vector2::Rotate(direction * 16, -1, 0), transform->get_matrix() * GameCamera::GetVPVMatrix());
	right = Transform2D::Homogeneous(Vector2::Rotate(direction * 16, 1, 0), transform->get_matrix() * GameCamera::GetVPVMatrix());
}

void Enemy::Draw() const {
	Vector2 screenPos = Transform2D::Homogeneous(zeroVec2, transform->get_matrix() * GameCamera::GetVPVMatrix());
	Novice::DrawEllipse(int(screenPos.x), int(screenPos.y), int(colliderCircle->radius), int(colliderCircle->radius), 0, color->hex(), kFillModeSolid);

	Novice::DrawTriangle(
		int(top.x), int(top.y),
		int(left.x), int(left.y),
		int(right.x), int(right.y),
		color->hex(), kFillModeSolid
	);
	for (auto bullet_itr = bullet.begin(); bullet_itr != bullet.end(); ++bullet_itr) {
		bullet_itr->Draw();
	}
}

bool Enemy::IsErase() {
	return hp <= 0;
}

const std::list<Bullet>& Enemy::GetBullet() const {
	return bullet;
}

std::list<Bullet>::iterator Enemy::EraseBullet(std::list<Bullet>::const_iterator& bullet_itr) {
	return bullet.erase(bullet_itr);
}
